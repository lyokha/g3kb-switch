/*
 * =============================================================================
 *
 *       Filename:  switch.c
 *
 *    Description:  g3kb-switch implementation
 *
 *        Version:  1.0
 *        Created:  13.12.2019 12:13:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  
 *
 * =============================================================================
 */

#include <gio/gio.h>
#include <errno.h>
#include <stdlib.h>

#include "switch.h"

#define G3KB_SWITCH_MAX_LAYOUTS 256
#define G3KB_SWITCH_DBUS_CALL_TIMEOUT 2000

#define G3KB_SWITCH_ERROR g3kb_switch_error()

#define G3KB_SWITCH_ERROR_RUN_METHOD 1
#define G3KB_SWITCH_ERROR_BUILD_LAYOUTS_MAP 2
#define G3KB_SWITCH_ERROR_GET_LAYOUT 3
#define G3KB_SWITCH_ERROR_SET_LAYOUT 4


struct value_search_data
{
    const gchar *value;
    guintptr idx;
};


struct next_key_search_data
{
    guintptr key;
    guintptr next;
    gboolean found;
};


static GQuark g3kb_switch_error( void )
{
    return g_quark_from_static_string( "g3kb-switch-error-quark" );
}


/* compare two guintptr values */
static gint key_compare( gconstpointer k1, gconstpointer k2, gpointer unused )
{
    guintptr a, b;

    a = ( guintptr ) k1;
    b = ( guintptr ) k2;

    if ( a > b ) {
        return 1;
    }
    if ( a == b ) {
        return 0;
    }

    return -1;  /* if a < b */
}


static gboolean value_search( gpointer k, gpointer v, gpointer data )
{
    guintptr key;
    gchar *value;
    struct value_search_data *vs;

    key = ( guintptr ) k;
    value = ( gchar * ) v;
    vs = ( struct value_search_data * ) data;

    vs->idx = G3KB_SWITCH_MAX_LAYOUTS;

    if ( g_strcmp0( vs->value, value ) == 0 ) {
        vs->idx = key;
        return TRUE;
    }

    return FALSE;
}


static gboolean next_key_search( gpointer k, gpointer unused, gpointer data )
{
    guintptr key;
    struct next_key_search_data *nks;

    key = ( guintptr ) k;
    nks = ( struct next_key_search_data * ) data;

    if ( nks->next == G3KB_SWITCH_MAX_LAYOUTS ) {
        nks->next = key;
    }
    if ( nks->found ) {
        nks->next = key;
        return TRUE;
    }
    if ( key == nks->key ) {
        nks->found = TRUE;
    }

    return FALSE;
}


gboolean g3kb_print_layouts( gpointer k, gpointer v, gpointer unused )
{
    g_print( "%s\n", ( const char * ) v );

    return FALSE;
}


static gboolean run_method( const gchar *name, const gchar *method,
                            gchar **value, GError **err )
{
    GDBusConnection *c = NULL;
    GVariant *result = NULL;
    GVariant *vmethod = NULL;
    GVariant *param = NULL;
    const GVariantType *vtype = NULL;
    GVariantBuilder builder;
    gboolean success;

    c = g_bus_get_sync( G_BUS_TYPE_SESSION, NULL, err );
    if ( c == NULL ) {
        return FALSE;
    }

    if ( method != NULL ) {
        g_variant_builder_init( &builder, G_VARIANT_TYPE_TUPLE );

        if ( g_strcmp0( name, "Set" ) == 0 ) {
            vtype = G_VARIANT_TYPE( "u" );
        }

        vmethod = g_variant_parse( vtype, method, NULL, NULL, NULL );
        if ( vmethod == NULL ) {
            g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_RUN_METHOD,
                         "Failed to parse input method" );
            g_object_unref( c );
            return FALSE;
        }

        g_variant_builder_add_value( &builder, vmethod );

        param = g_variant_builder_end( &builder );
        if ( param == NULL ) {
            g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_RUN_METHOD,
                         "Failed to build call parameter" );
            g_object_unref( c );
            return FALSE;
        }
    }

    result = g_dbus_connection_call_sync( c,
                                          "org.gnome.Shell",
#ifdef G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION
                                          "/org/g3kbswitch/G3kbSwitch",
                                          "org.g3kbswitch.G3kbSwitch",
#else
                                          "/org/gnome/Shell",
                                          "org.gnome.Shell",
#endif
                                          name,
                                          param,
                                          NULL,
                                          G_DBUS_CALL_FLAGS_NONE,
                                          G3KB_SWITCH_DBUS_CALL_TIMEOUT,
                                          NULL,
                                          err
                                        );

    if ( method != NULL ) {
        g_variant_unref( vmethod );
        g_object_unref( c );
    }

    if ( result == NULL ) {
        return FALSE;
    }

    if ( ! g_variant_is_of_type( result, G_VARIANT_TYPE( "(bs)" ) ) ) {
        g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_RUN_METHOD,
                     "Unexpected response type" );
        g_variant_unref( result );
        return FALSE;
    }

    g_variant_get( result, "(bs)", &success, value );
    if ( ! success ) {
        g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_RUN_METHOD,
                     "Bad response: %s",
                     value == NULL || *value == NULL ? "<empty>" : *value );
        if ( value ) {
            g_free( *value );
        }
        g_variant_unref( result );
        return FALSE;
    }

    g_variant_unref( result );

    return TRUE;
}


GTree *g3kb_build_layouts_map( GError **err )
{
    GVariant *vdict = NULL;
    GVariantIter iter1, *iter2;
    GTree *layouts = NULL;
    const gchar *name = NULL;
    const gchar *method = NULL;
    gchar *key, *value;
    gpointer k, v;
    gchar *dict = NULL;

#ifdef G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION
    name = "List";
#else
    name = "Eval";
    /* BEWARE: g3kb_get_layout() takes currentSource.index while here we simply
     * put counter i as the value of the key when iterating inputSources, this
     * should be correct as soon as currentSource.index drives iteration
     * order; to ensure correctness we could also put inputSources[i].index
     * instead of i */
    method = "\"var ids=[];"
             "for(var i in imports.ui.status.keyboard.getInputSourceManager()"
                 ".inputSources){"
                 "ids.push({key:i,value:"
                     "imports.ui.status.keyboard.getInputSourceManager()"
                         ".inputSources[i].id})};"
             "ids\"";
#endif

    if ( ! run_method( name, method, &dict, err ) ) {
        return NULL;
    }

    vdict = g_variant_parse( NULL, dict, NULL, NULL, NULL );
    if ( vdict == NULL ) {
        g_set_error( err, G3KB_SWITCH_ERROR,
                     G3KB_SWITCH_ERROR_BUILD_LAYOUTS_MAP,
                     "Failed to parse response value" );
        g_free( dict );
        return NULL;
    }

    g_free( dict );

    if ( ! g_variant_is_of_type( vdict, G_VARIANT_TYPE( "aa{ss}" ) ) ) {
        g_set_error( err, G3KB_SWITCH_ERROR,
                     G3KB_SWITCH_ERROR_BUILD_LAYOUTS_MAP,
                     "Unexpected type of response value" );
        g_variant_unref( vdict );
        return NULL;
    }

    layouts = g_tree_new_full( key_compare, NULL, NULL, g_free );

    g_variant_iter_init( &iter1, vdict );
    while ( g_variant_iter_loop( &iter1, "a{ss}", &iter2 ) ) {
        k = ( gpointer ) G3KB_SWITCH_MAX_LAYOUTS;
        v = NULL;
        while ( g_variant_iter_loop( iter2, "{ss}", &key, &value ) ) {
            if ( g_strcmp0( key, "key" ) == 0 ) {
                guintptr idx = G3KB_SWITCH_MAX_LAYOUTS;
                errno = 0;
                /* weirdly, g_ascii_strtoull() and g_ascii_string_to_unsigned()
                 * sometimes fail here with errno set to EAGAIN! */
                idx = ( guintptr ) strtoull( value, NULL, 10 );
                if ( errno == 0 && idx < G3KB_SWITCH_MAX_LAYOUTS ) {
                    k = ( gpointer ) idx;
                } else {
                    g_set_error( err, G3KB_SWITCH_ERROR,
                                 G3KB_SWITCH_ERROR_BUILD_LAYOUTS_MAP,
                                 "Key %s is not a valid index",
                                 value == NULL ? "<empty>" : value );
                    g_variant_unref( vdict );
                    g_tree_unref( layouts );
                    return NULL;
                }
            } else if ( g_strcmp0( key, "value" ) == 0 ) {
                if ( g_strcmp0( value, "" ) == 0 ) {
                    v = g_strdup( "(nil)" );
                } else {
                    v = g_strdup( value );
                }
            }
            if ( ( guintptr ) k < G3KB_SWITCH_MAX_LAYOUTS && v != NULL ) {
                g_tree_insert( layouts, k, v );
                continue;
            }
        }
    }

    g_variant_unref( vdict );

    return layouts;
}


guint g3kb_get_layout( GError **err )
{
    const gchar *name = NULL;
    const gchar *method = NULL;
    gchar *value = NULL;
    guintptr idx = G3KB_SWITCH_MAX_LAYOUTS;

#ifdef G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION
    name = "Get";
#else
    name = "Eval";
    method = "\"imports.ui.status.keyboard.getInputSourceManager()"
             ".currentSource.index\"";
#endif

    if ( ! run_method( name, method, &value, err ) ) {
        return G3KB_SWITCH_MAX_LAYOUTS;
    }

    errno = 0;
    /* see why we do not use g_ascii_strtoull() or g_ascii_string_to_unsigned()
     * in a comment inside g3kb_build_layouts_map() */
    idx = ( guintptr ) strtoull( value, NULL, 10 );
    if ( errno != 0 || idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_GET_LAYOUT,
                     "Key %s is not a valid index",
                     value == NULL ? "<empty>" : value );
        g_free( value );
        /* BEWARE: return invalid value G3KB_SWITCH_MAX_LAYOUTS, but as far as
         * the returned value is supposed to be later passed to
         * g3kb_search_layout(), this should not be a problem */
        return G3KB_SWITCH_MAX_LAYOUTS;
    }

    g_free( value );

    return ( guint ) idx;
}


gboolean g3kb_set_layout( guint idx, GError **err )
{
#ifdef G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION
    static const gsize method_activate_len =
        /* zero terminator */ /* max 3 digits for index */
           1 +                   3;
#else
    static const gchar *method_activate_head =
        "\"imports.ui.status.keyboard.getInputSourceManager().inputSources[";
    static const gchar *method_activate_tail =
        "].activate()\"";
    static const gsize method_activate_len =
        /* head */ /* tail */ /* zero terminator */ /* max 3 digits for index */
           65 +       13 +       1 +                   3;
#endif

    const gchar *name = NULL;
    gchar method[ method_activate_len ];

    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        g_set_error( err, G3KB_SWITCH_ERROR, G3KB_SWITCH_ERROR_SET_LAYOUT,
                     "Index %u is not valid", idx );
        return FALSE;
    }

#ifdef G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION
    name = "Set";
    g_snprintf( method, method_activate_len, "%u", idx );
#else
    name = "Eval";
    g_snprintf( method, method_activate_len, "%s%u%s",
                method_activate_head, idx, method_activate_tail );
#endif

    return run_method( name, method, NULL, err );
}


gconstpointer g3kb_search_layout( GTree *layouts, guint idx )
{
    guintptr layout_idx = ( guintptr ) idx;

    return g_tree_lookup( layouts, ( gconstpointer ) layout_idx );
}


guintptr g3kb_reverse_search_layout( GTree *layouts, const gchar *layout )
{
    struct value_search_data vs;

    vs.value = layout;
    g_tree_foreach( layouts, value_search, &vs );

    /* BEWARE: may return invalid value G3KB_SWITCH_MAX_LAYOUTS, but as far as
     * the returned value is supposed to be later passed to g3kb_set_layout(),
     * this should not be a problem */
    return vs.idx;
}


gconstpointer g3kb_safe_get_layout( GTree *layouts, GError **err )
{
    guint idx;

    idx = g3kb_get_layout( err );
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return NULL;
    }

    return g3kb_search_layout( layouts, idx );
}


gboolean g3kb_safe_set_layout( GTree *layouts, const gchar *layout,
                               GError **err )
{
    guint idx;

    idx = ( guint ) g3kb_reverse_search_layout( layouts, layout );
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return FALSE;
    }

    return g3kb_set_layout( idx, err );
}


guintptr g3kb_get_next_layout( GTree *layouts, GError **err )
{
    struct next_key_search_data nks;

    /* BEWARE: may return invalid value G3KB_SWIkCH_MAX_LAYOUTS, but as far as
     * the returned value is supposed to be later passed to
     * g3kb_set_next_layout(), this should not be a problem */

    nks.key = ( guintptr ) g3kb_get_layout( err );
    if ( nks.key >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return G3KB_SWITCH_MAX_LAYOUTS;
    }

    nks.next = G3KB_SWITCH_MAX_LAYOUTS;
    nks.found = FALSE;
    g_tree_foreach( layouts, next_key_search, &nks );

    return nks.next;
}


gboolean g3kb_set_next_layout( GTree *layouts, GError **err )
{
    guint idx;

    idx = ( guint ) g3kb_get_next_layout( layouts, err );
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return FALSE;
    }

    return g3kb_set_layout( idx, err );
}


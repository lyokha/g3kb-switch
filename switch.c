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

#include <errno.h>
#include "switch.h"

#define G3KB_SWITCH_MAX_LAYOUTS 256
#define G3KB_SWITCH_DBUS_CALL_TIMEOUT 2000
#define G3KB_QUARK 1


struct value_search_data
{
    const gchar *value;
    guintptr idx;
};


static gint key_cmp( gconstpointer ap, gconstpointer bp, gpointer unused )
{
    guintptr a, b;

    a = ( guintptr ) ap;
    b = ( guintptr ) bp;

    if ( a > b ) {
        return 1;
    }
    if ( a == b ) {
        return 0;
    }

    return -1;  /* if a < b */
}


static gint key_search( gconstpointer ap, gconstpointer bp )
{
    guintptr a, b;

    a = ( guintptr ) ap;
    b = ( guintptr ) bp;

    if ( a > b ) {
        return -1;
    }
    if ( a == b ) {
        return 0;
    }

    return 1;  /* if a < b */
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


gboolean g3kb_print_layouts( gpointer k, gpointer v, gpointer unused )
{
    g_print( "%s\n", ( const char * ) v );

    return FALSE;
}


static gboolean run_method( const gchar *method, gchar **value, GError **err )
{
    GDBusConnection *c = NULL;
    GVariant *result = NULL;
    GVariant *vmethod = NULL;
    GVariant *param = NULL;
    GVariantBuilder builder;
    gboolean success;

    c = g_bus_get_sync( G_BUS_TYPE_SESSION, NULL, err );
    if ( c == NULL ) {
        return FALSE;
    }

    g_variant_builder_init( &builder, G_VARIANT_TYPE_TUPLE );

    vmethod = g_variant_parse( NULL, method, NULL, NULL, NULL );
    if ( vmethod == NULL ) {
        *err = g_error_new_literal( G3KB_QUARK, 0, "Failed to parse method" );
        g_object_unref( c );
        return FALSE;
    }

    g_variant_builder_add_value( &builder, vmethod );

    param = g_variant_builder_end( &builder );
    if ( param == NULL ) {
        *err = g_error_new_literal( G3KB_QUARK, 0, "Failed to build param" );
        g_object_unref( c );
        return FALSE;
    }

    result = g_dbus_connection_call_sync( c,
                                          "org.gnome.Shell",
                                          "/org/gnome/Shell",
                                          "org.gnome.Shell",
                                          "Eval",
                                          param,
                                          NULL,
                                          G_DBUS_CALL_FLAGS_NONE,
                                          G3KB_SWITCH_DBUS_CALL_TIMEOUT,
                                          NULL,
                                          err
                                        );

    g_variant_unref( vmethod );
    g_object_unref( c );

    if ( result == NULL ) {
        return FALSE;
    }

    if ( ! g_variant_is_of_type( result, G_VARIANT_TYPE( "(bs)" ) ) ) {
        *err = g_error_new_literal( G3KB_QUARK, 0,
                                    "Unexpected type of response" );
        g_variant_unref( result );
        return FALSE;
    }

    g_variant_get( result, "(bs)", &success, value );
    if ( ! success ) {
        if ( value ) {
            g_free( *value );
        }
        *err = g_error_new_literal( G3KB_QUARK, 0,
                                    "Failed to parse response value" );
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
    const gchar *method = NULL;
    gchar *key, *value;
    gpointer k, v;
    gchar *dict = NULL;

    /* BEWARE: g3kb_get_layout() takes currentSource.index while here we simply
     * put counter i as the value of the key when iterating inputSources, this
     * should be correct as soon as currentSource.index drives iteration
     * order; to ensure correctness we could also put inputSources[i].index
     * instead of i */
    method = "\"ids = [];"
             "for (i in imports.ui.status.keyboard.getInputSourceManager()"
                 ".inputSources){"
                 "ids.push({key:i, value:"
                     "imports.ui.status.keyboard.getInputSourceManager()"
                         ".inputSources[i].id})};"
              "ids\"";

    if ( ! run_method( method, &dict, err ) ) {
        return NULL;
    }

    vdict = g_variant_parse( NULL, dict, NULL, NULL, NULL );
    if ( vdict == NULL ) {
        *err = g_error_new_literal( G3KB_QUARK, 0, "Failed to parse dict" );
        g_free( dict );
        return NULL;
    }

    g_free( dict );

    if ( ! g_variant_is_of_type( vdict, G_VARIANT_TYPE( "aa{ss}" ) ) ) {
        *err = g_error_new_literal( G3KB_QUARK, 0, "Unexpected type of dict" );
        g_variant_unref( vdict );
        return NULL;
    }

    layouts = g_tree_new_full( ( GCompareDataFunc ) key_cmp,
                               NULL, NULL, g_free );

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
                    *err = g_error_new_literal( G3KB_QUARK, 0,
                                                "Failed to read key as index" );
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


guint g3kb_get_layout( void )
{
    const gchar *method = NULL;
    gchar *value = NULL;
    guintptr idx = G3KB_SWITCH_MAX_LAYOUTS;

    method = "\"imports.ui.status.keyboard.getInputSourceManager()"
             ".currentSource.index\"";

    if ( ! run_method( method, &value, NULL ) ) {
        return G3KB_SWITCH_MAX_LAYOUTS;
    }

    errno = 0;
    /* see why we do not use g_ascii_strtoull() or g_ascii_string_to_unsigned()
     * in a comment inside g3kb_build_layouts_map() */
    idx = ( guintptr ) strtoull( value, NULL, 10 );
    if ( errno != 0 || idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        g_free( value );
        /* BEWARE: return invalid value G3KB_SWITCH_MAX_LAYOUTS, but as far as
         * the returned value is supposed to be later passed to
         * g3kb_search_layout(), this should not be a problem */
        return G3KB_SWITCH_MAX_LAYOUTS;
    }

    g_free( value );

    return ( guint ) idx;
}


gboolean g3kb_set_layout( guint idx )
{
    static const gchar *method_activate_head =
        "\"imports.ui.status.keyboard.getInputSourceManager().inputSources[";
    static const gchar *method_activate_tail =
        "].activate()\"";
    static const gsize method_activate_len =
        /* head */ /* tail */ /* zero terminator */ /* max 3 digits for index */
           65 +       13 +       1 +                   3;

    gchar method[ method_activate_len ];

    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return FALSE;
    }

    g_snprintf( method, method_activate_len, "%s%u%s",
                method_activate_head, idx, method_activate_tail );

    return run_method( method, NULL, NULL );
}


gconstpointer g3kb_search_layout( GTree *layouts, guint idx )
{
    guintptr layout_idx = ( guintptr ) idx;

    return g_tree_search( layouts, key_search, ( gconstpointer ) layout_idx );
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


gconstpointer g3kb_safe_get_layout( GTree *layouts )
{
    guint idx;

    idx = g3kb_get_layout();
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return NULL;
    }

    return g3kb_search_layout( layouts, idx );
}


gboolean g3kb_safe_set_layout( GTree *layouts, const gchar *layout )
{
    guint idx;

    idx = ( guint ) g3kb_reverse_search_layout( layouts, layout );
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return FALSE;
    }

    return g3kb_set_layout( idx );
}


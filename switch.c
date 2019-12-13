/*
 * =============================================================================
 *
 *       Filename:  switch.c
 *
 *    Description:  g3kb-switch implementation and vim-xkbswitch API
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

#include "switch.h"

#define DBUS_CALL_TIMEOUT 2000

static const gchar *method_activate_head =
    "\"imports.ui.status.keyboard.getInputSourceManager().inputSources[";
static const gchar *method_activate_tail =
    "].activate()\"";
static const gsize method_activate_len =
    /* head */ /* tail */ /* zero terminator */ /* max 3 digits for index */
    65 +       13 +       1 +                   3;

struct value_search_data {
    const gchar *value;
    guintptr idx;
};


static gint key_cmp( gconstpointer ap, gconstpointer bp, gpointer unused ) {
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


static gint key_search( gconstpointer ap, gconstpointer bp ) {
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


static gboolean value_search( gpointer k, gpointer v, gpointer data ) {
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


gboolean Xkb_Switch_printXkbLayouts( gpointer k, gpointer v, gpointer unused ) {
    g_print( "%s\n", ( const char * ) v );
    return FALSE;
}


static inline GVariant *call_dbus( GDBusConnection *c, GVariant *param ) {
    return g_dbus_connection_call_sync( c,
                                        "org.gnome.Shell",
                                        "/org/gnome/Shell",
                                        "org.gnome.Shell",
                                        "Eval",
                                        param,
                                        NULL,
                                        G_DBUS_CALL_FLAGS_NONE,
                                        DBUS_CALL_TIMEOUT,
                                        NULL,
                                        NULL
                                      );
}


GTree *Xkb_Switch_buildXkbLayoutsMap() {
    GVariant *result = NULL;
    GVariant *vmethod = NULL;
    GVariant *vdict = NULL;
    GVariant *param = NULL;
    GVariantBuilder builder;
    GVariantIter iter1, *iter2;
    GTree *layouts = NULL;
    gchar *method = NULL;
    gchar *res = NULL;
    gchar *key, *value;
    gpointer k, v;
    gboolean *success;
    gchar *dict = NULL;

    GDBusConnection *c = g_bus_get_sync( G_BUS_TYPE_SESSION, NULL, NULL );
    if (c == NULL) {
        return NULL;
    }

    method = "\"ids = [];"
             "for (i in imports.ui.status.keyboard.getInputSourceManager()"
                 ".inputSources){"
                 "ids.push({key:i, value:"
                     "imports.ui.status.keyboard.getInputSourceManager()"
                         ".inputSources[i].id})};"
              "ids\"";

    g_variant_builder_init( &builder, G_VARIANT_TYPE_TUPLE );

    vmethod = g_variant_parse( NULL, method, NULL, NULL, NULL );
    if ( vmethod == NULL ) {
        g_object_unref( c );
        return NULL;
    }

    g_variant_builder_add_value( &builder, vmethod );

    param = g_variant_builder_end( &builder );
    if ( param == NULL ) {
        g_object_unref( c );
        return NULL;
    }

    result = call_dbus( c, param );

    g_variant_unref( vmethod );
    g_object_unref( c );

    if ( result == NULL ) {
        return NULL;
    }

    if ( ! g_variant_is_of_type( result, G_VARIANT_TYPE( "(bs)" ) ) ) {
        g_variant_unref( result );
        return NULL;
    }

    g_variant_get( result, "(bs)", &success, &dict );
    if ( ! success ) {
        g_variant_unref( result );
        return NULL;
    }

    g_variant_unref( result );

    vdict = g_variant_parse( NULL, dict, NULL, NULL, NULL );
    if ( vdict == NULL ) {
        g_free( dict );
        return NULL;
    }

    g_free( dict );

    if ( ! g_variant_is_of_type( vdict, G_VARIANT_TYPE( "aa{ss}" ) ) ) {
        return NULL;
        g_variant_unref( vdict );
    }

    layouts = g_tree_new_full( ( GCompareDataFunc ) key_cmp,
                               NULL, NULL, g_free );

    g_variant_iter_init( &iter1, vdict );
    while ( g_variant_iter_loop( &iter1, "a{ss}", &iter2 ) ) {
        k = ( gpointer ) G3KB_SWITCH_MAX_LAYOUTS;
        v = NULL;
        while ( g_variant_iter_loop( iter2, "{ss}", &key, &value ) ) {
            if ( g_strcmp0( key, "key" ) == 0 ) {
                guintptr idx;
                if ( g_ascii_string_to_unsigned( value,
                                                 10, 0,
                                                 G3KB_SWITCH_MAX_LAYOUTS,
                                                 &idx,
                                                 NULL
                                               ) )
                {
                    k = ( gpointer ) idx;
                } else {
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


gchar *Xkb_Switch_getXkbLayoutRaw() {
    GVariant *result = NULL;
    GVariant *vmethod = NULL;
    GVariant *param = NULL;
    GVariantBuilder builder;
    gchar *method = NULL;
    gboolean *success;
    gchar *value = NULL;

    GDBusConnection *c = g_bus_get_sync( G_BUS_TYPE_SESSION, NULL, NULL );
    if (c == NULL) {
        return NULL;
    }

    method = "\"imports.ui.status.keyboard.getInputSourceManager()"
             ".currentSource.index\"";

    g_variant_builder_init( &builder, G_VARIANT_TYPE_TUPLE );

    vmethod = g_variant_parse( NULL, method, NULL, NULL, NULL );
    if ( vmethod == NULL ) {
        g_object_unref( c );
        return NULL;
    }

    g_variant_builder_add_value( &builder, vmethod );

    param = g_variant_builder_end( &builder );
    if ( param == NULL ) {
        g_object_unref( c );
        return NULL;
    }

    result = call_dbus( c, param );

    g_variant_unref( vmethod );
    g_object_unref( c );

    if ( result == NULL ) {
        return NULL;
    }

    if ( ! g_variant_is_of_type( result, G_VARIANT_TYPE( "(bs)" ) ) ) {
        g_variant_unref( result );
        return NULL;
    }

    g_variant_get( result, "(bs)", &success, &value );
    if ( ! success ) {
        g_variant_unref( result );
        return NULL;
    }

    g_variant_unref( result );

    return value;
}


gboolean Xkb_Switch_setXkbLayoutRaw( const gchar *value ) {
    GVariant *result = NULL;
    GVariant *vmethod = NULL;
    GVariant *param = NULL;
    GVariantBuilder builder;
    gchar method[ method_activate_len ];
    gboolean *success;

    GDBusConnection *c = g_bus_get_sync( G_BUS_TYPE_SESSION, NULL, NULL );
    if (c == NULL) {
        return FALSE;
    }

    g_snprintf( method, method_activate_len, "%s%.3s%s",
                method_activate_head, value, method_activate_tail );

    g_variant_builder_init( &builder, G_VARIANT_TYPE_TUPLE );

    vmethod = g_variant_parse( NULL, method, NULL, NULL, NULL );
    if ( vmethod == NULL ) {
        g_object_unref( c );
        return FALSE;
    }

    g_variant_builder_add_value( &builder, vmethod );

    param = g_variant_builder_end( &builder );
    if ( param == NULL ) {
        g_object_unref( c );
        return FALSE;
    }

    result = call_dbus( c, param );

    g_variant_unref( vmethod );
    g_object_unref( c );

    if ( result == NULL ) {
        return FALSE;
    }

    if ( ! g_variant_is_of_type( result, G_VARIANT_TYPE( "(bs)" ) ) ) {
        g_variant_unref( result );
        return FALSE;
    }

    g_variant_get( result, "(bs)", &success, NULL );
    if ( ! success ) {
        g_variant_unref( result );
        return FALSE;
    }

    g_variant_unref( result );

    return TRUE;
}


gpointer Xkb_Switch_searchXkbLayout( GTree *layouts, gchar *layout ) {
    guintptr idx = G3KB_SWITCH_MAX_LAYOUTS;

    if ( ! g_ascii_string_to_unsigned( layout,
                                       10, 0, G3KB_SWITCH_MAX_LAYOUTS,
                                       &idx,
                                       NULL
                                     ) )
    {
        return NULL;
    }

    return g_tree_search( layouts, key_search, ( gconstpointer ) idx );
}


guintptr Xkb_Switch_reverseSearchXkbLayout( GTree *layouts, gchar *layout ) {
    struct value_search_data vs;

    vs.value = layout;

    g_tree_foreach( layouts, value_search, &vs );

    return vs.idx;
}


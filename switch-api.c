/*
 * =============================================================================
 *
 *       Filename:  switch-api.c
 *
 *    Description:  vim-xkbswitch API
 *
 *        Version:  1.0
 *        Created:  14.12.2019 11:07:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  
 *
 * =============================================================================
 */

#include "switch.h"


GTree *layouts = NULL;


const char *Xkb_Switch_getXkbLayout( const char *unused )
{
    gchar *layout_raw = NULL;
    gpointer layout = NULL;

    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    layout_raw = g3kb_get_layout();
    if ( layout_raw == NULL ) {
        return "";
    }

    layout = g3kb_search_layout( layouts, layout_raw );
    if ( layout == NULL ) {
        return "";
    }

    return layout;
}


const char *Xkb_Switch_setXkbLayout( const char *new_layout )
{
    guintptr idx;
    gchar s_idx[ 3 ];

    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    idx = g3kb_reverse_search_layout( layouts, new_layout );
    if ( idx >= G3KB_SWITCH_MAX_LAYOUTS ) {
        return "";
    }

    g_snprintf( s_idx, 3, "%d", ( int ) idx );
    if ( ! g3kb_set_layout( s_idx ) ) {
        return "";
    }

    return "";
}


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
    gchar *layout_idx = NULL;
    gpointer layout = NULL;

    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    layout_idx = g3kb_get_layout();
    if ( layout_idx == NULL ) {
        return "";
    }

    layout = g3kb_search_layout( layouts, layout_idx );
    if ( layout == NULL ) {
        g_free( layout_idx );
        return "";
    }

    g_free( layout_idx );

    return layout;
}


const char *Xkb_Switch_setXkbLayout( const char *new_layout )
{
    guint idx;

    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    idx = ( guint ) g3kb_reverse_search_layout( layouts, new_layout );
    if ( ! g3kb_set_layout( idx ) ) {
        return "";
    }

    return "";
}


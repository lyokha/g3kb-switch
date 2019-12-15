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
    gconstpointer layout = NULL;

    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    layout = g3kb_safe_get_layout( layouts );
    if ( layout == NULL ) {
        return "";
    }

    return layout;
}


const char *Xkb_Switch_setXkbLayout( const char *layout )
{
    if ( layouts == NULL ) {
        layouts = g3kb_build_layouts_map();
        if ( layouts == NULL ) {
            return "";
        }
    }

    if ( ! g3kb_safe_set_layout( layouts, layout ) ) {
        return "";
    }

    return "";
}


/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  g3kb-switch program
 *
 *        Version:  1.0
 *        Created:  10.12.2019 14:59:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  
 *
 * =============================================================================
 */

#include <stdlib.h>
#include "switch.h"

#define VERSION_MAJ 0
#define VERSION_MIN 1


void usage( void )
{
    g_print( "Usage: g3kb-switch -s ARG    Sets current layout group to ARG\n"
             "       g3kb-switch -l        Displays all layout groups\n"
             "       g3kb-switch -h        Displays this message\n"
             "       g3kb-switch -v        Shows version number\n"
             "       g3kb-switch [-p]      Displays current layout group\n" );
}

int main( int argc, char **argv )
{
    GTree *layouts;
    gchar *layout_idx = NULL;
    gchar *new_layout = NULL;
    gpointer layout;
    guint idx;
    gboolean print_layouts = FALSE;
    gboolean display_current_layout = FALSE;
    gboolean activate_new_layout = FALSE;

    if ( argc > 1 ) {
        if ( g_strcmp0( argv[ 1 ], "-h" ) == 0 ||
             g_strcmp0( argv[ 1 ], "--help" ) == 0 )
        {
            usage ();
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-v" ) == 0 ||
                    g_strcmp0( argv[ 1 ], "--version" ) == 0 )
        {
            g_print( "g3kb-switch version %d.%d\n", VERSION_MAJ, VERSION_MIN );
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-l" ) == 0 )
        {
            print_layouts = TRUE;
        } else if ( g_strcmp0( argv[ 1 ], "-p" ) == 0 )
        {
            display_current_layout = TRUE;
        } else if ( g_strcmp0( argv[ 1 ], "-s" ) == 0 )
        {
            if ( argc < 3 ) {
                usage ();
                return 1;
            }
            activate_new_layout = TRUE;
            new_layout = argv[ 2 ];
        }
    } else {
        display_current_layout = TRUE;
    }

    layouts = g3kb_build_layouts_map();
    if ( layouts == NULL ) {
        g_printerr( "Failed to build keyboard layouts map!\n" );
        exit( 1 );
    }

    if ( print_layouts ) {
        g_tree_foreach( layouts, g3kb_print_layouts, NULL );
    } else if ( display_current_layout ) {
        layout_idx = g3kb_get_layout();
        if ( layout_idx == NULL ) {
            g_printerr( "Failed to get current keyboard layout!\n" );
            g_tree_unref( layouts );
            exit( 1 );
        }
        layout = g3kb_search_layout( layouts, layout_idx );
        if ( layout == NULL ) {
            g_printerr( "Failed to find layout with index %s!\n", layout_idx );
            g_free( layout_idx );
            g_tree_unref( layouts );
            exit( 1 );
        }
        g_print( "%s\n", ( char * ) layout );
        g_free( layout_idx );
    } else if ( activate_new_layout ) {
        idx = ( guint ) g3kb_reverse_search_layout( layouts, new_layout );
        if ( ! g3kb_set_layout( idx ) ) {
            g_printerr( "Failed to activate layout %s!\n", new_layout );
            g_tree_unref( layouts );
            exit( 1 );
        }
    }

    g_tree_unref( layouts );

    return 0;
}


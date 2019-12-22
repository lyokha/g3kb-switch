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

#include "switch.h"

#define VERSION_MAJ 0
#define VERSION_MIN 3


void version( void )
{
    g_print( "g3kb-switch version %d.%d\n", VERSION_MAJ, VERSION_MIN );
}


void usage( void )
{
    version();
    g_print( "Usage: g3kb-switch [-p]      Show current layout group\n"
             "       g3kb-switch -l        Show all layout groups\n"
             "       g3kb-switch -n        Switch to the next layout group\n"
             "       g3kb-switch -s ARG    Set current layout group to ARG\n"
             "       g3kb-switch -h        Show this message and exit\n"
             "       g3kb-switch -v        Show program version and exit\n" );
}


int main( int argc, char **argv )
{
    GTree *layouts;
    gchar *new_layout = NULL;
    gconstpointer layout;
    gboolean print_layouts = FALSE;
    gboolean display_current_layout = FALSE;
    gboolean activate_new_layout = FALSE;
    gboolean activate_next_layout = FALSE;
    GError *err = NULL;

    if ( argc > 1 ) {
        if ( g_strcmp0( argv[ 1 ], "-h" ) == 0 ||
             g_strcmp0( argv[ 1 ], "--help" ) == 0 )
        {
            usage ();
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-v" ) == 0 ||
                    g_strcmp0( argv[ 1 ], "--version" ) == 0 )
        {
            version();
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-l" ) == 0 ) {
            print_layouts = TRUE;
        } else if ( g_strcmp0( argv[ 1 ], "-p" ) == 0 ) {
            display_current_layout = TRUE;
        } else if ( g_strcmp0( argv[ 1 ], "-s" ) == 0 ) {
            if ( argc < 3 ) {
                usage ();
                return 1;
            }
            activate_new_layout = TRUE;
            new_layout = argv[ 2 ];
        } else if ( g_strcmp0( argv[ 1 ], "-n" ) == 0 ) {
            activate_next_layout = TRUE;
        } else {
            usage ();
            return 1;
        }
    } else {
        display_current_layout = TRUE;
    }

    layouts = g3kb_build_layouts_map( &err );
    if ( layouts == NULL ) {
        g_printerr( "Failed to build keyboard layouts map: %s\n",
                    err == NULL ? "<no details>" : err->message );
        g_clear_error( &err );
        return 1;
    }

    if ( print_layouts ) {
        g_tree_foreach( layouts, g3kb_print_layouts, NULL );
    } else if ( display_current_layout ) {
        layout = g3kb_safe_get_layout( layouts, &err );
        if ( layout == NULL ) {
            g_printerr( "Failed to find current layout: %s\n",
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            g_tree_unref( layouts );
            return 1;
        }
        g_print( "%s\n", ( char * ) layout );
    } else if ( activate_new_layout ) {
        if ( ! g3kb_safe_set_layout( layouts, new_layout, &err ) ) {
            g_printerr( "Failed to activate layout %s: %s\n", new_layout,
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            g_tree_unref( layouts );
            return 1;
        }
    } else if ( activate_next_layout ) {
        if ( ! g3kb_set_next_layout( layouts, &err ) ) {
            g_printerr( "Failed to activate next layout: %s\n",
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            g_tree_unref( layouts );
            return 1;
        }
    }

    g_tree_unref( layouts );

    return 0;
}


/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  g3kb-switch program
 *
 *        Version:  1.4
 *        Created:  10.12.2019 14:59:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (),
 *        Company:
 *
 * =============================================================================
 */

#include "config.h"
#include "switch.h"

static void version( void )
{
    g_print( "g3kb-switch version %s\n", PROJECT_VERSION );
}

static void usage( void )
{
    version();
    g_print(
        "Usage: g3kb-switch [-p]      Show the current layout group\n"
        "       g3kb-switch -l        Show available layout groups\n"
        "       g3kb-switch -n        Switch to the next layout group\n"
        "       g3kb-switch -s ARG    Switch to layout group ARG\n"
        "       g3kb-switch -h        Show this message and exit\n"
        "       g3kb-switch -v        Show the program version and exit\n" );
}

int main( int argc, char **argv )
{
    GTree *layouts;
    gchar *new_layout = NULL;
    gconstpointer layout;
    enum command {
        display_current_layout,
        print_layouts,
        activate_new_layout,
        activate_next_layout,
    };
    enum command cmd = display_current_layout;
    GError *err = NULL;

    if ( argc > 1 ) {
        if ( g_strcmp0( argv[ 1 ], "-h" ) == 0 ||
             g_strcmp0( argv[ 1 ], "--help" ) == 0 ) {
            usage();
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-v" ) == 0 ||
                    g_strcmp0( argv[ 1 ], "--version" ) == 0 ) {
            version();
            return 0;
        } else if ( g_strcmp0( argv[ 1 ], "-l" ) == 0 ) {
            cmd = print_layouts;
        } else if ( g_strcmp0( argv[ 1 ], "-p" ) == 0 ) {
            cmd = display_current_layout;
        } else if ( g_strcmp0( argv[ 1 ], "-s" ) == 0 ) {
            if ( argc < 3 ) {
                usage();
                return 1;
            }
            cmd = activate_new_layout;
            new_layout = argv[ 2 ];
        } else if ( g_strcmp0( argv[ 1 ], "-n" ) == 0 ) {
            cmd = activate_next_layout;
        } else {
            usage();
            return 1;
        }
    }

    layouts = g3kb_build_layouts_map( &err );
    if ( layouts == NULL ) {
        g_printerr( "Failed to build keyboard layouts map: %s\n",
                    err == NULL ? "<no details>" : err->message );
        g_clear_error( &err );
        return 1;
    }

    int status = 0;
    switch ( cmd ) {
    case print_layouts:
        g_tree_foreach( layouts, g3kb_print_layouts, NULL );
        break;
    case display_current_layout:
        layout = g3kb_safe_get_layout( layouts, &err );
        if ( layout == NULL ) {
            g_printerr( "Failed to find current layout: %s\n",
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            status = 1;
            break;
        }
        g_print( "%s\n", (char *)layout );
        break;
    case activate_new_layout:
        if ( !g3kb_safe_set_layout( layouts, new_layout, &err ) ) {
            g_printerr( "Failed to activate layout %s: %s\n", new_layout,
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            status = 1;
        }
        break;
    case activate_next_layout:
        if ( !g3kb_set_next_layout( layouts, &err ) ) {
            g_printerr( "Failed to activate next layout: %s\n",
                        err == NULL ? "<no details>" : err->message );
            g_clear_error( &err );
            status = 1;
        }
    }

    g_tree_unref( layouts );

    return status;
}

/*
 * =============================================================================
 *
 *       Filename:  switch.h
 *
 *    Description:  g3kb-switch implementation
 *
 *        Version:  1.0
 *        Created:  13.12.2019 12:23:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  
 *
 * =============================================================================
 */

#ifndef G3KB_SWITCH_H
#define G3KB_SWITCH_H

#include <gio/gio.h>

#define G3KB_SWITCH_MAX_LAYOUTS 256


gboolean g3kb_print_layouts( gpointer k, gpointer v, gpointer ignored );
GTree *g3kb_build_layouts_map( void );
gchar *g3kb_get_layout( void );
gboolean g3kb_set_layout( const gchar *value );
gpointer g3kb_search_layout( GTree *layouts, gchar *layout );
guintptr g3kb_reverse_search_layout( GTree *layouts, const gchar *layout );

#endif  /* G3KB_SWITCH_H */


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


gboolean g3kb_print_layouts( gpointer k, gpointer v, gpointer ignored );
GTree *g3kb_build_layouts_map( void );
guint g3kb_get_layout( void );
gboolean g3kb_set_layout( guint idx );
gconstpointer g3kb_search_layout( GTree *layouts, guint idx );
guintptr g3kb_reverse_search_layout( GTree *layouts, const gchar *layout );
gconstpointer g3kb_safe_get_layout( GTree *layouts );
gboolean g3kb_safe_set_layout( GTree *layouts, const gchar *layout );

#endif  /* G3KB_SWITCH_H */


/*
 * =============================================================================
 *
 *       Filename:  switch.h
 *
 *    Description:  g3kb-switch implementation and vim-xkbswitch API
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


gboolean Xkb_Switch_printXkbLayouts( gpointer k, gpointer v, gpointer ignored );
GTree *Xkb_Switch_buildXkbLayoutsMap();
gchar *Xkb_Switch_getXkbLayoutRaw();
gboolean Xkb_Switch_setXkbLayoutRaw( const gchar *value );
gpointer Xkb_Switch_searchXkbLayout( GTree *layouts, gchar *layout );
guintptr Xkb_Switch_reverseSearchXkbLayout( GTree *layouts, gchar *layout );
gchar *Xkb_Switch_getXkbLayoutsMap( GTree *layouts );

#endif  /* G3KB_SWITCH_H */


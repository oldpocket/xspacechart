/***********************************************************************
 * map.h : Definition of the map.
 *         The map consists of a GTK widget containing the map graphic
 *         and necesary controls. When a star is clicked it calls a set
 *         callback function.
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 1999 Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 ***********************************************************************/

#ifndef _INCLUDE_MAP_H_
#define _INCLUDE_MAP_H_

#include <gtk/gtk.h>
#include "link.h"
#include "star.h"
#include "settings.h"

typedef struct st_map map_t;

/* Creates a new empty map */
map_t* map_new( settings_t* settings );

/* Returns the widget that contains the map. */
GtkWidget* map_get_widget( map_t* map );

/* Get the settings data structure used by the map. */
settings_t* map_get_settings( map_t* map );

/* Set the function called when the user clicks on a star. */
void map_set_star_clicked( map_t* map, 
                           void (*clicked)( star_t* star, void* data ), 
                           void* data );

/* Rotate the map by the indicated amount. */
void map_rotate( map_t* map, double v_ang, double h_ang );

void map_set_shown_objects( map_t* map, star_t *star_list[], 
                            link_t *link_list[] );

/* Destroy the map and related data structures. */
void map_destroy( map_t* map );

#endif

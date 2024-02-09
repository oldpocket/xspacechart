/***********************************************************************
 * link_config.h : A group of widget that allows specification of a list
 *                of drawing rules for links.
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

#ifndef _INCLUDE_LINK_CONFIG_H_
#define _INCLUDE_LINK_CONFIG_H_

#include <gtk/gtk.h>
#include "starmap.h"
#include "link_draw_rules.h"

typedef struct st_link_conf link_config_t;

/* Create a new link drawing rules widget set. */
link_config_t* link_config_new( void (*changed)( link_config_t *config,
                                                 void *data ), void *data );

/* Set the list of rules displayed. */
void link_config_set_rules( link_config_t* config,
                            link_drawing_rules_t* rules );

/* Set the unit ( ly or parsecs ) used when interacting with the user. */
void link_config_set_unit( link_config_t* config,
                           distance_unit_t unit );

/* Returns the main widget of the set. */
GtkWidget* link_config_get_widget( link_config_t* config );

/* Get a new list of rules that reflects the current configuration. */
link_drawing_rules_t* link_config_get_rules(link_config_t* config);

/* Destroy the configuration set and related data structures. */
void link_config_destroy( link_config_t* conf );

#endif

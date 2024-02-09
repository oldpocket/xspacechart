/***********************************************************************
 * star_config.h : Definition of a group of widgets that allow the user to
 *                view and edit a star drawing rules list.
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

#ifndef _INCLUDE_STAR_CONFIG_H_
#define _INCLUDE_STAR_CONFIG_H_

#include "starmap.h"
#include "star_draw_rules.h"

typedef struct st_star_config star_config_t;

/* Create a new star configuration widget set. */
star_config_t *star_config_new( void (*changed)( star_config_t *config,
                                                 void *data ), void *data );

/* Set the star drawing rules to be displayed. */
void star_config_set_rules( star_config_t *config, 
                            star_drawing_rules_t* rules );

/* Return the parent widget in the set. */
GtkWidget *star_config_get_widget( star_config_t *config );

/* Create a list of rules from the current configuration. */
star_drawing_rules_t *star_config_get_rules( star_config_t *config );

/* Destroy the widgets and related data. */
void star_config_destroy( star_config_t *config );

#endif

/***********************************************************************
 * settings.h : Definition of the general settings of the map.
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

#ifndef _INCLUDE_SETTINGS_H_
#define _INCLUDE_SETTINGS_H_

#include <gdk/gdk.h>
#include "starmap.h"
#include "star.h"
#include "link.h"
#include "star_selection.h"
#include "link_selection.h"
#include "star_draw_rules.h"
#include "link_draw_rules.h"

#define PROPERTIES_STAR_FILTER          1
#define PROPERTIES_LINK_FILTER          2
#define PROPERTIES_VIEW_RADIUS          4 
#define PROPERTIES_SHOW_LINKS           8 
#define PROPERTIES_SHOW_LINK_LABELS    16 
#define PROPERTIES_SHOW_STAR_LABELS    32 
#define PROPERTIES_LABELS_COLOR        64 
#define PROPERTIES_LABELS_FONT        128 
#define PROPERTIES_DISTANCE_UNIT      256 
#define PROPERTIES_CENTER             512 
#define PROPERTIES_SIGHT_PARAMS      1024 
#define PROPERTIES_STAR_DRAW_RULES   2048 
#define PROPERTIES_LINK_DRAW_RULES   4096
#define PROPERTIES_CATALOG_FILENAME  8192
#define PROPERTIES_KEYBINDINGS      16384

typedef unsigned int properties_set_t;

typedef struct st_settings settings_t;

settings_t* settings_new( void );

int settings_load_from_files( settings_t *settings );
int settings_save_settings( settings_t* settings );

int settings_add_callback( settings_t* settings, properties_set_t prop_set,
                            void (*callback)(settings_t* settings, void* data),
                            void* data );

void settings_disable_callbacks( settings_t* settings );
void settings_enable_callbacks( settings_t* settings );

void settings_set_catalog_filename( settings_t *settings, 
                                    const char *filename );
char *settings_get_catalog_filename( settings_t *settings );

void settings_set_star_filter( settings_t* settings,star_selection_t* filter );
star_selection_t* settings_get_star_filter( settings_t* settings );

void settings_set_link_filter( settings_t* settings,link_selection_t* filter );
link_selection_t* settings_get_link_filter( settings_t* settings );

void settings_set_view_radius( settings_t* settings, double view_radius );
double settings_get_view_radius( settings_t* settings );

void settings_set_show_links( settings_t* settings, int value );
int settings_get_show_links( settings_t* settings );

void settings_set_show_link_labels( settings_t* settings, int value );
int settings_get_show_link_labels( settings_t* settings );

void settings_set_show_star_labels( settings_t* settings, int value );
int settings_get_show_star_labels( settings_t* settings );

void settings_set_labels_color( settings_t* settings, double rgb[] );
void settings_get_labels_color( settings_t* settings, double rgb[] );

void settings_set_labels_font( settings_t* settings, const char *font );
char *settings_get_labels_font( settings_t* settings );

void settings_set_distance_unit( settings_t* settings, distance_unit_t unit );
distance_unit_t settings_get_distance_unit( settings_t* settings );

void settings_set_center( settings_t* settings, coords_3d_t* center );
void settings_get_center( settings_t* settings, coords_3d_t* center );

/* Access the vectors that follow the line of sight (center to from) and which
 * side is up. Set to NULL any value you don't want to receive.
 * NOTE: All vectors are assumed to be unitarian. */
void settings_set_sight_params( settings_t* settings, 
                                coords_3d_t* line_of_sight,
                                coords_3d_t* up );
void settings_get_sight_params( settings_t* settings, 
                                coords_3d_t* line_of_sight,
                                coords_3d_t* up );
void settings_set_sight_params_polar( settings_t* settings,
                                      double longitude, double latitude,
                                      double north );
void settings_get_sight_params_polar( settings_t* settings,
                                      double *longitude, double *latitude,
                                      double *north );

void settings_set_star_draw_rules( settings_t* settings, 
                                   star_drawing_rules_t* rules );
star_drawing_rules_t* settings_get_star_draw_rules( settings_t* settings );
void settings_find_star_draw( settings_t* settings, star_t* star, int* radius, 
                              double rgb[], int* show_name );

void settings_set_link_draw_rules( settings_t* settings, 
                                   link_drawing_rules_t* rules );
link_drawing_rules_t* settings_get_link_draw_rules( settings_t* settings );
void settings_find_link_draw( settings_t* settings, link_t* link, int* width, 
                              GdkLineStyle* style, double rgb[] );

void settings_set_keybindings( settings_t* settings, key_press_t *keys );
void settings_get_keybindings( settings_t* settings, key_press_t *keys );

void settings_destroy( settings_t* settings );

#endif

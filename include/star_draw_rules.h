/***********************************************************************
 * star_draw_rules.h : Definition of the list of rules for drawing stars.
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

#ifndef _INCLUDE_STAR_DRAW_RULES_H_
#define _INCLUDE_STAR_DRAW_RULES_H_

#include "starmap.h"
#include "star.h"
#include "star_selection.h"

#define SIZE_0    1
#define SIZE_1    2
#define SIZE_2    3
#define SIZE_3    4
#define SIZE_4    5

/* Maximum priority for user defined rules. */
#define PRIORITY_USER_MAX    10

/* Priorities of the rule for each spectral class color. */
#define PRIORITY_SPECTRUM_O  11
#define PRIORITY_SPECTRUM_B  12
#define PRIORITY_SPECTRUM_A  13
#define PRIORITY_SPECTRUM_F  14
#define PRIORITY_SPECTRUM_G  15
#define PRIORITY_SPECTRUM_K  16
#define PRIORITY_SPECTRUM_M  17
#define PRIORITY_SPECTRUM_WD 18

/* Priority used for luminosity rules. */
#define PRIORITY_LUMINOSITY  19

/* Priority used for the rule to display the star name. */
#define PRIORITY_SHOW_NAME   20

#define DEFAULT_SIZE         1

extern double default_color[];

typedef struct st_star_draw_rules star_drawing_rules_t;

/* Create an empty list of rules, using the given attributes as the default. */
star_drawing_rules_t* star_drawing_rules_new( int default_radius, 
                                              double default_rgb[] );

/* Add a new rule of the given priority, that applies to stars that match
 * selection, and with the attributes given. */
int star_drawing_rules_add( star_drawing_rules_t* rules,
                            star_selection_t* selection,
                            int priority,
                            int radius, double rgb[], int show_name );

/* Find the attributes used to draw the given star. */
void star_drawing_rules_find( star_drawing_rules_t* rules,
                              star_t* star, int* radius, double rgb[],
                              int* show_name );

/* Loop over the rules within a priority range. */
void star_drawing_rules_foreach( star_drawing_rules_t* rules,
                                 int min_priority, int max_priority,
                                 void (*function)( star_selection_t* selection,
                                            int is_radius, int radius, 
                                            int is_rgb, double rgb[],
                                            int show_name, void *data ),
                                 void *data );

/* Destroy the list. */
void star_drawing_rules_destroy( star_drawing_rules_t* rules );

#endif

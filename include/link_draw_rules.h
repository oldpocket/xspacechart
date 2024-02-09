/***********************************************************************
 * link_draw_rules.h : Definition of the list of rules for drawing links.
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

#ifndef _INCLUDE_DISTRULE_H_
#define _INCLUDE_DISTRULE_H_

#include <gdk/gdk.h>
#include "starmap.h"
#include "link.h"
#include "link_selection.h"

typedef struct st_link_drawing_rules link_drawing_rules_t;

link_drawing_rules_t* link_drawing_rules_new( int default_width, 
                                              GdkLineStyle default_style,
                                              double default_rgb[] );

void link_drawing_rules_get_default( link_drawing_rules_t* rules, int *width, 
                                     GdkLineStyle *style, double rgb[] );

void link_drawing_rules_find( link_drawing_rules_t* rules, 
                              link_t *link, int* width, 
                              GdkLineStyle* style, double rgb[] );

int link_drawing_rules_add( link_drawing_rules_t* rules, 
                            link_selection_t* selection, int width, 
                            GdkLineStyle style, double rgb[] );

void link_drawing_rules_foreach( link_drawing_rules_t* rules, 
                                 void (*function)( link_selection_t* selection,
                                                   int width,
                                                   GdkLineStyle style,
                                                   double rgb[],
                                                   void* data ),
                                 void* data );

void link_drawing_rules_destroy( link_drawing_rules_t* rules );

#endif

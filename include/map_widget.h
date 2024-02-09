/***********************************************************************
 * map_widget.h : Definition of the map widget.
 *                Includes the physical area of the map and the zoom 
 *               control.
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 1999, 2000 Free Software Foundation, Inc.
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

#ifndef _INCLUDE_MAP_WIDGET_H_
#define _INCLUDE_MAP_WIDGET_H_

#include "starmap.h"

/* Respond to rotations only after this movement of the cursor */
#define MIN_ROT 2

typedef struct st_map_widget map_widget_t;

map_widget_t *map_widget_new( void (*redraw)(map_widget_t*, void*),
                              void (*rotate)( map_widget_t*, double v_ang,
                                              double h_ang, void* data ),
                              void (*clicked)( map_widget_t*, double x, 
                                               double y, double lambda,
                                               void* data ),
                              void *data );

GtkWidget* map_widget_get_widget( map_widget_t* map_widget );

void map_widget_draw_point( map_widget_t* map_widget, double x, double y,
                            int radius, double rgb[] );

void map_widget_draw_line( map_widget_t* map_widget, double x1, double y1,
                           double x2, double y2, int width, double rgb[],
                           GdkLineStyle style );

void map_widget_draw_label( map_widget_t* map_widget, double x, double y,
                            int displace, GdkFont *font, double rgb[], 
                            const char label[] );

void map_widget_clear( map_widget_t *map_widget );

void map_widget_show( map_widget_t *map_widget );

void map_widget_destroy( map_widget_t* map_widget );

#endif

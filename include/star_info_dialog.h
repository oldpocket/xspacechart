/***********************************************************************
 * star_info_dialog.h : Definition of the star info dialog.
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2002 Free Software Foundation, Inc.
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

#ifndef _INCLUDE_STARINFO_H_
#define _INCLUDE_STARINFO_H_

#include "star.h"
#include "starmap.h"

typedef struct st_star_info star_info_t;

/* Create a new star information dialog. It receives the function to be called
 * when the user asks to center the star in display. */
star_info_t* star_info_new( void (*center)(star_t* star, void* data), 
                            void* data );

void star_info_set_star( star_info_t* info, star_t* star );

void star_info_set_distance_units( star_info_t* info, distance_unit_t unit );

void star_info_hide( star_info_t* info );

GtkWidget *star_info_get_dialog( star_info_t* info );

void star_info_destroy(  star_info_t* info );

#endif

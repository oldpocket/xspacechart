/***********************************************************************
 * star_index.h : Definition of a index of stars, ordered by their
 *               distance from the 'from' point of a perspective, with
 *               their 2D coordinates in that perspective.
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

#ifndef _INCLUDE_STAR_INDEX_H_
#define _INCLUDE_STAR_INDEX_H_

#include "starmap.h"
#include "star.h"
#include "perspective.h"

typedef struct st_star_index star_index_t;

typedef enum 
{
        STAR_INDEX_START_BEGIN,
        STAR_INDEX_START_END
} star_index_find_start_t;

star_index_t* star_index_new( perspective_t *persp, star_t *star_list[] );

void star_index_foreach( star_index_t* index, 
                         void (*function)(star_t* star, double x, double y, 
                                          void* data ), void* data );

star_t* star_index_find_first( star_index_t* index, 
                               int (*function)(star_t* star, 
                                               double x, 
                                               double y, 
                                               void* data ),
                               void* data, star_index_find_start_t type );

void star_index_destroy( star_index_t* index );

#endif

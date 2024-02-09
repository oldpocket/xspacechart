/***********************************************************************
 * distind.h : Definition of the index of distances between  stars.
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

#ifndef _INCLUDE_DISTIND_H_
#define _INCLUDE_DISTIND_H_

#include "starmap.h"
#include "link.h"
#include "perspective.h"

typedef struct st_link_index link_index_t;

typedef enum 
{
        LINK_INDEX_START_BEGIN,
        LINK_INDEX_START_END
} link_index_find_start_t;

link_index_t* link_index_new( perspective_t* persp, link_t *link_list[] );

void link_index_foreach( link_index_t* index, void (*function)(link_t* link, 
                                                               double x1, 
                                                               double y1,
                                                               double x2,
                                                               double y2, 
                                                               void* data ),
                         void* data );

link_t* link_index_find_first( link_index_t* index, 
                               int (*function)(link_t* link, 
                                               double x1, double y1,
                                               double x2, double y2, 
                                               void* data ),
                               void* data, link_index_find_start_t type );

void link_index_destroy( link_index_t* index );

#endif

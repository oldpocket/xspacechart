/***********************************************************************
 * link_selection.h : Definition of a pattern for links. It allows to
 *                   filter the links in several ways. Only the length
 *                   of the link is supported yet.
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

#ifndef _INCLUDE_LINK_SELECTION_H_
#define _INCLUDE_LINK_SELECTION_H_
#include "link.h"

typedef struct st_link_selection link_selection_t;

link_selection_t* link_selection_new( void );

int link_selection_match( link_selection_t* selection, link_t* link );

void link_selection_act_min_length( link_selection_t* selection, double min );

void link_selection_deact_min_length( link_selection_t* selection );

int link_selection_is_min_length( link_selection_t* selection );

double link_selection_get_min_length( link_selection_t* selection );

void link_selection_act_max_length( link_selection_t* selection, double max );

void link_selection_deact_max_length( link_selection_t* selection );

int link_selection_is_max_length( link_selection_t* selection );

double link_selection_get_max_length( link_selection_t* selection );

void link_selection_destroy( link_selection_t* selection );

#endif

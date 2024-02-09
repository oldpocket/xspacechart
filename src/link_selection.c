/***********************************************************************
 * link_selection.c : Implementation of a pattern for links. It allows to
 *                   filter the links in several ways. Only the length
 *                   of the link is supported yet.
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2000 Free Software Foundation, Inc.
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

#include <stdlib.h>
#include "../include/starmap.h"
#include "../include/link.h"
#include "../include/link_selection.h"

struct st_link_selection
{
        int is_min_length;
        double min_length;
        int is_max_length;
        double max_length;
};

link_selection_t* link_selection_new( void )
{
        link_selection_t* selection;
        
        if((selection = (link_selection_t*) malloc(sizeof(link_selection_t))))
        {
                selection->is_min_length = 0;
                selection->min_length = 0;
                selection->is_max_length = 0;
                selection->max_length = 0;
        }
        
        return selection;
}

int link_selection_match( link_selection_t* selection, link_t* link )
{
        int matches = 1;
        if( selection->is_min_length )
                matches = matches && 
                        (link_get_distance(link) >= selection->min_length);
        if( selection->is_max_length )
                matches = matches && 
                        (link_get_distance(link) <= selection->max_length);
        return matches;
}

void link_selection_act_min_length( link_selection_t* selection, double min )
{
        selection->is_min_length = 1;
        selection->min_length = min;
}

void link_selection_deact_min_length( link_selection_t* selection )
{
        selection->is_min_length = 0;
}

int link_selection_is_min_length( link_selection_t* selection )
{
        return selection->is_min_length;
}

double link_selection_get_min_length( link_selection_t* selection )
{
        return selection->min_length;
}

void link_selection_act_max_length( link_selection_t* selection, double max )
{
        selection->is_max_length = 1;
        selection->max_length = max;
}

void link_selection_deact_max_length( link_selection_t* selection )
{
        selection->is_max_length = 0;
}

int link_selection_is_max_length( link_selection_t* selection )
{
        return selection->is_max_length;
}

double link_selection_get_max_length( link_selection_t* selection )
{
        return selection->max_length;
}

void link_selection_destroy( link_selection_t* selection )
{
        free( selection );
}

/***********************************************************************
 * link.c : Implementation of the link. A link is a pair of stars and 
 *         facilities to access extra information.
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
#include "../include/star.h"
#include "../include/link.h"

struct st_link
{
        star_t *a, *b;
        double distance;
        coords_3d_t middle;
};

link_t *link_new( star_t* a, star_t* b )
{
        link_t* link;
        coords_3d_t coords_a, coords_b;
        
        if( (link = (link_t*) malloc( sizeof(struct st_link) ) ))
        {
                link->a = a;
                link->b = b;
                star_get_coords( a, &coords_a );
                star_get_coords( b, &coords_b );
                link->distance = distance_3d( &coords_a, &coords_b );
                link->middle.x = (coords_b.x + coords_a.x) / 2;
                link->middle.y = (coords_b.y + coords_a.y) / 2;
                link->middle.z = (coords_b.z + coords_a.z) / 2;
        }

        return link;
}

void link_get_stars( link_t* link, star_t **a, star_t **b )
{
        *a = link->a;
        *b = link->b;
}

double link_get_distance( link_t* link )
{
        return link->distance;
}

void link_get_middle( link_t* link, coords_3d_t *middle )
{
        *middle = link->middle;
}

void link_destroy( link_t* link )
{
        free( link );
}

/***********************************************************************
 * perspective.c: Data concerning the point of view. It is defined by
 *                three coordinates: the location of the observer 
 *                ('from'), the point she is looking at ('center' or 
 *                'at') and a vector indicating which side is at the top
 *                of his view ('up'). 
 *                   It also contains any extra data necessary to make
 *                calculation of a point's projection faster.
 **********************************************************************/

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
#include "../include/perspective.h"

struct st_perspective
{
        coords_3d_t from, center, up;
        coords_3d_t a1, a2, a3, off;
        double dval;
};

/* Declaration of private functions. */
static void change_base( coords_3d_t* from, coords_3d_t* at, coords_3d_t* up,
                         coords_3d_t* point );
static void compute_aux( perspective_t *persp );

/* Public Functions */

perspective_t* perspective_new( coords_3d_t* from, coords_3d_t* center,
                                coords_3d_t* up )
{
        perspective_t* persp;

        if( ( persp = ( perspective_t* ) malloc( sizeof( perspective_t ) ) ) )
        {
                persp->from = *from;
                persp->center = *center;
                persp->up = *up;
                persp->dval = cos( THETA/2 ) / sin( THETA/2 );
                compute_aux( persp );
        }     

        return persp;
}

void perspective_get_from( perspective_t *persp, coords_3d_t* from )
{
        *from = persp->from;
}

void perspective_get_up( perspective_t *persp, coords_3d_t* up )
{
        *up = persp->up;
}

void perspective_get_center( perspective_t *persp, coords_3d_t* center )
{
        *center = persp->center;
}

/* Rotate the perspective v_ang radians in the vertical and h_ang radians
 * in the horizontal. */
void perspective_rotate( perspective_t *persp, double v_ang, double h_ang )
{
        coords_3d_t new_from, new_up;
        double r;

        r = distance_3d( &persp->from, &persp->center );
        
        v_ang = M_PI/2 - v_ang;

        new_from.x = r * cos(h_ang) * sin(v_ang);
        new_from.y = r * sin(h_ang) * sin(v_ang);
        new_from.z = r * cos(v_ang);
        
        new_up.x = (-1) *cos( h_ang ) * cos( v_ang );
        new_up.y = (-1) * sin( h_ang ) * cos( v_ang );
        new_up.z = sin( v_ang );

        change_base( &persp->from, &persp->center, &persp->up, &new_from );
        change_base( &persp->from, &persp->center, &persp->up, &new_up );
        new_from.x = new_from.x + persp->center.x;
        new_from.y = new_from.y + persp->center.y;
        new_from.z = new_from.z + persp->center.z;

        persp->from = new_from;
        persp->up = new_up;

        compute_aux(persp);
}

/* Set the coordinates given as the new center of the perspective system, 
 * keeping the current orientation of the view. */
void perspective_change_center(perspective_t *persp, coords_3d_t* new_center)
{
        coords_3d_t direction;
        double distance;
        
        distance = distance_3d( &persp->center, new_center );
        direction.x = new_center->x - persp->center.x;
        direction.y = new_center->y - persp->center.y;
        direction.z = new_center->z - persp->center.z;

        perspective_move_center( persp, &direction, distance );
}

/* Move the center distance parsecs in the given direction, keeping the
 * orientation of the view. */
void perspective_move_center( perspective_t *persp, coords_3d_t* direction,
                              double distance )
{
        double dir_mag;
        if( distance == 0.0)
                return;
        dir_mag = magnitude( direction );
        persp->from.x = persp->from.x + (direction->x/dir_mag)*distance;
        persp->from.y = persp->from.y + (direction->y/dir_mag)*distance;
        persp->from.z = persp->from.z + (direction->z/dir_mag)*distance;
        persp->center.x = persp->center.x + (direction->x/dir_mag)*distance;
        persp->center.y = persp->center.y + (direction->y/dir_mag)*distance;
        persp->center.z = persp->center.z + (direction->z/dir_mag)*distance;
        compute_aux(persp);
}

/* Calculate the projection of the given point in 2D. The results are given
 * in the [0..1] range. */
void perspective_compute( perspective_t* persp,
                          coords_3d_t* star_coords, 
                          double* x, double* y )
{
        coords_3d_t eye;

        eye.x = (star_coords->x * persp->a1.x ) + 
                (star_coords->y * persp->a1.y) + 
                (star_coords->z * persp->a1.z ) + persp->off.x;
        eye.y = (star_coords->x * persp->a2.x ) + 
                (star_coords->y * persp->a2.y) + 
                (star_coords->z * persp->a2.z ) + persp->off.y;
        eye.z = (star_coords->x * persp->a3.x ) + 
                (star_coords->y * persp->a3.y) + 
                (star_coords->z * persp->a3.z ) + persp->off.z;

        *x = (eye.x * persp->dval) / eye.z;
        *y = (eye.y * persp->dval) / eye.z;
}

void perspective_destroy( perspective_t* persp )
{
        free( persp );
}

/* Auxiliary Functions */

void change_base( coords_3d_t* from, coords_3d_t* center, coords_3d_t* up,
                  coords_3d_t* point )
{
        coords_3d_t new_point, radius;
        double r;
        
        r = distance_3d( from, center );
        radius.x = from->x - center->x;
        radius.y = from->y - center->y;
        radius.z = from->z - center->z;

        /* Operations to get the point in the standard coordinates system */
        new_point.x = (radius.x/r) * point->x 
                + ( (up->y*radius.z)/r - (up->z*radius.y)/r ) * point->y
                + up->x * point->z;
        new_point.y = (radius.y/r) * point->x
                + ( (up->z*radius.x)/r - (up->x*radius.z)/r ) * point->y
                + up->y * point->z; 
        new_point.z = (radius.z/r) * point->x
                + ( (up->x*radius.y)/r - (up->y*radius.x)/r ) * point->y
                + up->z * point->z;
        
        *point = new_point;
}

void compute_aux( perspective_t *persp )
{
        coords_3d_t a_prime, tmp;
        double a_prime_mag, tmp_mag;

        a_prime.x = persp->center.x - persp->from.x;
        a_prime.y = persp->center.y - persp->from.y;
        a_prime.z = persp->center.z - persp->from.z;
        a_prime_mag = magnitude( &a_prime );
        persp->a3.x = a_prime.x / a_prime_mag;
        persp->a3.y = a_prime.y / a_prime_mag;
        persp->a3.z = a_prime.z / a_prime_mag;
        tmp.x = ( a_prime.y * persp->up.z ) - ( persp->up.y * a_prime.z );
        tmp.y = ( a_prime.z * persp->up.x ) - ( persp->up.z * a_prime.x );
        tmp.z = ( a_prime.x * persp->up.y ) - ( persp->up.x * a_prime.y );
        tmp_mag = magnitude( &tmp );
        persp->a1.x = tmp.x / tmp_mag;
        persp->a1.y = tmp.y / tmp_mag;
        persp->a1.z = tmp.z / tmp_mag;
        tmp_mag = magnitude( &persp->up );
        persp->a2.x = persp->up.x / tmp_mag;
        persp->a2.y = persp->up.y / tmp_mag;
        persp->a2.z = persp->up.z / tmp_mag;
        persp->off.x = -(persp->a1.x*persp->from.x) - 
                (persp->a1.y*persp->from.y)-(persp->a1.z*persp->from.z);
        persp->off.y = -(persp->a2.x*persp->from.x) - 
                (persp->a2.y*persp->from.y)-(persp->a2.z*persp->from.z);
        persp->off.z = -(persp->a3.x*persp->from.x) - 
                (persp->a3.y*persp->from.y)-(persp->a3.z*persp->from.z);
}

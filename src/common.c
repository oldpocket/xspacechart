/***********************************************************************
 * common.c : Types and functions used in many parts of the program that
 *            don't really belong anywhere.
 * $Id: common.c,v 1.1 2017/09/23 00:04:10 strauss Exp $
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

#include "../include/starmap.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* Public Functions */

double magnitude( coords_3d_t* coord )
{
        return sqrt( coord->x*coord->x + 
                     coord->y*coord->y + 
                     coord->z*coord->z );
}

double distance_3d( coords_3d_t* a, coords_3d_t* b )
{
        register double x, y, z;
        
        x = (a->x - b->x);
        y = (a->y - b->y);
        z = (a->z - b->z);
        return sqrt( x*x + y*y + z*z );
}

void emit_error_message( const char* message )
{
        GtkWidget *error;
        error = gnome_message_box_new( message, 
                                       "error",
                                       GTK_STOCK_OK,
                                       NULL );
        gnome_dialog_run_and_close(GNOME_DIALOG(error));
}

/* Converts a vector to polar form */
void cartesian_to_polar( coords_3d_t *vector )
{
        double latitude, longitude, dist;
        double tmp;

        dist = magnitude( vector );
        tmp = vector->z/dist;
        /* Yes, this test is stupid... since this should never be greater
         * than 1, but floating point errors can make Bad Things happen */ 
        if( tmp > 1 )
                tmp = 1;
        else if( tmp < -1 )
                tmp = -1;
        latitude = asin( tmp );
        tmp = vector->x/(dist * cos(latitude));
        if( tmp > 1 )
                tmp = 1;
        else if( tmp < -1 )
                tmp = -1;
        longitude = acos( tmp );
        if( vector->y < 0 )
                longitude = 2*M_PI-longitude;

        vector->x = longitude;
        vector->y = latitude;
        vector->z = dist;
}

/* Converts a vector to cartesian form */
void polar_to_cartesian( coords_3d_t *vector )
{
        double x, y, z;

        x = vector->z * cos(vector->y) * cos( vector->x );
        y = vector->z * cos(vector->y) * sin( vector->x );
        z = vector->z * sin(vector->y);

        vector->x = x;
        vector->y = y;
        vector->z = z;
}

/* Converts from RA to longitude (RADIANS) */
void ra_to_lon( double h, double m, double *lon )
{
        *lon = h * 15 + m * (15.0/60.0);
        *lon *= M_PI / 180;
}

/* Converts from longitude (RADIANS) to RA */
void lon_to_ra( double lon, double *h, double *m )
{
        lon *= 180 / M_PI;
        *h = (int) (lon / 15.0);
        *m = (int) ( (lon - *h * 15.0 ) / (15.0/60.0) );
}

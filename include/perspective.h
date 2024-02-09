/***********************************************************************
 * perspective.h: Data concerning the point of view. It is defined by
 *                three coordinates: the location of the observer 
 *                ('from'), the point she is looking at ('center' or 
 *                'at') and a vector indicating which side is at the top
 *                of his view ('up'). 
 *                   It also contains any extra data necessary to make
 *                calculation of a point's projection faster.
 **********************************************************************/

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

#ifndef _INCLUDE_PERSPECTIVE_H_
#define _INCLUDE_PERSPECTIVE_H_

#include "starmap.h"

typedef struct st_perspective perspective_t;

perspective_t* perspective_new( coords_3d_t* from, coords_3d_t* center,
                                coords_3d_t* up );

void perspective_get_from( perspective_t *persp, coords_3d_t* from );

void perspective_get_up( perspective_t *persp, coords_3d_t* up );

void perspective_get_center( perspective_t *persp, coords_3d_t* center );

/* Rotate the perspective v_ang radians in the vertical and h_ang radians
 * in the horizontal. */
void perspective_rotate( perspective_t *persp, double v_ang, double h_ang );

/* Set the coordinates given as the new center of the perspective system, 
 * keeping the current orientation of the view. */
void perspective_change_center(perspective_t *persp, coords_3d_t* new_center);

/* Move the center distance parsecs in the given direction, keeping the
 * orientation of the view. */
void perspective_move_center( perspective_t *persp, coords_3d_t* direction,
                              double distance );

/* Calculate the projection of the given point in 2D. The results are given
 * in the [-1..1] range. */
void perspective_compute( perspective_t* persp,
                          coords_3d_t* star_coords, 
                          double* x, double* y );

void perspective_destroy( perspective_t* persp );

#endif

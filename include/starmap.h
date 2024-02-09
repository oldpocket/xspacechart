/***********************************************************************
 * starmap.h: Declaration of global variables, constants, data types, 
 *            and functions used all around the program.
 * $Id: starmap.h,v 1.1 2017/09/23 00:04:09 strauss Exp $
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

#ifndef _INCLUDE_STARMAP_H_
#define _INCLUDE_STARMAP_H_

#include <math.h>

#define THETA ( M_PI / 3 )
#define PARSEC_TO_LY 3.2

/* #include "config.h"
#include <gnome.h> */
#include <gtk/gtk.h>

/* Global variables. */
extern const char *app_name;
extern const char *app_version;
extern const char *app_authors[];

typedef int star_id_t;

typedef enum
{
        DISTANCE_PARSECS,
        DISTANCE_LIGHT_YEARS
} distance_unit_t;

typedef struct
{
        double x;
        double y;
        double z;
} coords_3d_t;

typedef struct
{
        guint key;
        GdkModifierType mods;
} key_press_t;

typedef enum
{
        KEY_MOVE_UP, 
        KEY_MOVE_DOWN,
        KEY_MOVE_LEFT, 
        KEY_MOVE_RIGHT,
        KEY_MOVE_FRONT, 
        KEY_MOVE_REAR,
        KEY_SAVE_SETTINGS,
        KEY_ROT_UP, 
        KEY_ROT_DOWN, 
        KEY_ROT_LEFT, 
        KEY_ROT_RIGHT, 
        KEY_SETTINGS,
        KEY_STAR_SEL,
        KEY_TOGGLE_LINKS, 
        KEY_TOGGLE_LINK_LABELS, 
        KEY_TOGGLE_STAR_LABELS,
        KEY_RELOAD_SETTINGS,
        KEY_END_OF_LIST /* This one is always the last */
} keybinding_t;

/* Converts a vector to polar form */
void cartesian_to_polar( coords_3d_t *vector );

/* Converts a vector to cartesian form */
void polar_to_cartesian( coords_3d_t *vector );

/* Returns the distance between points a anb b in the space. */
double distance_3d( coords_3d_t* a, coords_3d_t* b );

/* Calculate the magnitude of vector. */
double magnitude( coords_3d_t *vector );

/* Display a message to the user. */
void emit_error_message( const char* message );

/* Converts from RA to longitude (RADIANS) */
void ra_to_lon( double h, double m, double *lon );

/* Converts from longitude (RADIANS) to RA */
void lon_to_ra( double lon, double *h, double *m );

#endif

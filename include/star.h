/***********************************************************************
 * star.h : Definition of operations on star_t
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

#ifndef _INCLUDE_STAR_H_
#define _INCLUDE_STAR_H_

#include <stdio.h>
#include "starmap.h"

/* Spectral classes for the stars. */
typedef enum
{
        SPECTRUM_O,
        SPECTRUM_B,
        SPECTRUM_A,
        SPECTRUM_F,
        SPECTRUM_G,
        SPECTRUM_K,
        SPECTRUM_M,
        SPECTRUM_WHITE_DWARF
} spectrum_t;

/* First the functions to access the star system. */

typedef struct st_star star_t;

/* Create a new star with default values. */
star_t* star_new( void );

/* Read a star from a file. */
int star_read( star_t* star, FILE* file );

/* Access to the unique internal id of the star. */
inline void star_set_id( star_t* star, star_id_t id );
inline star_id_t star_get_id( star_t* star );

/* Access to the Gliese catalog name of the star. */
inline void star_set_catalog_name( star_t* star, char* name );
inline void star_get_catalog_name( star_t* star, char* name );

/* Access to the short name of the star. */
inline void star_set_short_name( star_t* star, char* name );
inline void star_get_short_name( star_t* star, char* name );

/* Access to the heliocentric cartesian coordinates of the star. */
inline void star_set_coords( star_t* star, coords_3d_t* coords );
inline void star_get_coords( star_t* star, coords_3d_t* coords );

/* Access to the right ascension/declination coordinates of a star */
inline void star_set_ra( star_t* star, double hours, double minutes, 
                         double seconds );
inline void star_get_ra( star_t* star, double *hours, double *minutes, 
                         double *seconds );
inline void star_set_dec( star_t* star, double degrees, double minutes );
inline void star_get_dec( star_t* star, double *degrees, double *minutes );

/* Returns the number of components of the star system. */
inline int star_components( star_t* star );

/* Destroy the star structure and free allocated memory. */
inline void star_destroy( star_t* star );

/**************************************************** 
 * Now the functions to access specific components. 
 ****************************************************/

typedef struct st_star_component star_component_t;

/* Create a new component. */
star_component_t *star_component_new( void );

/* Add a component to the star system. */
void star_add_component( star_t* star, star_component_t* comp );

/* Loop over the components of the star system. */
void star_foreach_component( star_t* star, 
                             void (*function)( star_component_t *component,
                                               void *data ),
                             void *data );

/* Returns the character that identifies the component. */
void star_component_set_component( star_component_t *comp, char name );
char star_component_get_component( star_component_t *comp );

/* Access to the proper name of the star. */
void star_component_set_proper_name( star_component_t* comp, char* name );
void star_component_get_proper_name( star_component_t* comp, char* name );

/* Access to the Bayer-Flamsteed name of the star. */
void star_component_set_bayer_name( star_component_t* comp, char* name );
void star_component_get_bayer_name( star_component_t* comp, char* name );

/* Access to the DM catalog number of the star. */
void star_component_set_dm_name( star_component_t* comp, char* name );
void star_component_get_dm_name( star_component_t* comp, char* name );

/* Access to the string that marks the spectral class of the star. */
void star_component_set_spectrum( star_component_t* comp, char* spectrum );
void star_component_get_spectrum( star_component_t* comp, char* spectrum );
/* Returns the spectral class of the star inferred from the class string. */
spectrum_t star_component_get_spectrum_type( star_component_t* comp );

/* Access to the luminosity of the star. */
void star_component_set_luminosity( star_component_t* comp, float lum );
float star_component_get_luminosity( star_component_t* comp );

/* Destroy the component structure. */
void star_component_destroy( star_component_t* comp );

#endif

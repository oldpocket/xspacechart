/***********************************************************************
 * star_selection.h : The star selection structures defines a pattern
 *                    for stars. A star can be required to have a certain
 *                    luminosity and spectral class. Selection according
 *                    to the multiplicity of the star is also available.
 *                       A multiple system matches the selection if any
 *                    of it's components does.
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

#ifndef _INCLUDE_STARSEL_H_
#define _INCLUDE_STARSEL_H_

#include "starmap.h"
#include "star.h"

typedef struct st_star_selection star_selection_t;

star_selection_t* star_selection_new( void );

int star_selection_match( star_selection_t* selection, star_t* star );

void star_selection_act_min_lum( star_selection_t* selection, float min_lum );

void star_selection_act_max_lum( star_selection_t* selection, float max_lum );

void star_selection_act_spectrum( star_selection_t* selection, 
                                  spectrum_t spectrum );

void star_selection_deact_min_lum( star_selection_t* selection );

void star_selection_deact_max_lum( star_selection_t* selection );

void star_selection_deact_spectrum( star_selection_t* selection );

int star_selection_is_min_lum( star_selection_t* selection );

double star_selection_get_min_lum( star_selection_t* selection );

int star_selection_is_max_lum( star_selection_t* selection );

double star_selection_get_max_lum( star_selection_t* selection );

int star_selection_is_spectrum( star_selection_t* selection );

int star_selection_is_showed( star_selection_t* selection, 
                              spectrum_t spectrum );

void star_selection_set_allow_multiples( star_selection_t* selection, 
                                        int allow );
int star_selection_get_allow_multiples( star_selection_t* selection );

void star_selection_copy( star_selection_t* orig, star_selection_t* new );

void star_selection_destroy( star_selection_t* selection );

#endif

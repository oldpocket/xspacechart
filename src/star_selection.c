/***********************************************************************
 * star_selection.c : Implementation of the star "pattern" type.
 * $Id: star_selection.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
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
#include <string.h>
#include "../include/starmap.h"
#include "../include/star.h"
#include "../include/star_selection.h"

#define MAX_SPEC 30

struct st_star_selection
{
        int use_min_lum;
        float min_lum;
        int use_max_lum;
        float max_lum;
        int use_spectrum;
        spectrum_t spectrum[MAX_SPEC];
        int allow_multiples;
        int tmp_matched;
};

/* Declaration of private functions. */
static void match_component( star_component_t *comp, star_selection_t *sel );

/* Public Functions. */

star_selection_t* star_selection_new( void )
{
        star_selection_t* selection;
        
        if( ( selection = 
              (star_selection_t*) malloc( sizeof( star_selection_t ) ) ) )
        {
                selection->use_min_lum = 0;
                selection->use_max_lum = 0;
                selection->use_spectrum = 0;
                selection->allow_multiples = 1;
        }
        return selection;
}

int star_selection_match( star_selection_t* selection, star_t* star )
{
        if( !selection->allow_multiples && (star_components(star) > 1) )
                selection->tmp_matched = 0;
        else
        {
                selection->tmp_matched = 0;
                star_foreach_component( star, 
                                        (void(*)(star_component_t*, void*)) 
                                        match_component, selection );
        }
        return selection->tmp_matched;
}

void star_selection_act_min_lum( star_selection_t* selection, float min_lum )
{
        selection->use_min_lum = 1;
        selection->min_lum = min_lum;
}

void star_selection_act_max_lum( star_selection_t* selection, float max_lum )
{
        selection->use_max_lum = 1;
        selection->max_lum = max_lum;
}

void star_selection_act_spectrum( star_selection_t* selection, 
                                  spectrum_t spectrum )
{
        selection->spectrum[selection->use_spectrum] = spectrum;
        selection->use_spectrum++;
}

void star_selection_deact_min_lum( star_selection_t* selection )
{
        selection->use_min_lum = 0;
}

void star_selection_deact_max_lum( star_selection_t* selection )
{
        selection->use_max_lum = 0;
}

void star_selection_deact_spectrum( star_selection_t* selection )
{
        selection->use_spectrum = 0;
}

int star_selection_is_min_lum( star_selection_t* selection )
{
        return selection->use_min_lum;
}

double star_selection_get_min_lum( star_selection_t* selection )
{
        return selection->min_lum;
}

int star_selection_is_max_lum( star_selection_t* selection )
{
        return selection->use_max_lum;
}

double star_selection_get_max_lum( star_selection_t* selection )
{
         return selection->max_lum;
}

int star_selection_is_spectrum( star_selection_t* selection )
{
        return selection->use_spectrum;
}

int star_selection_is_showed( star_selection_t* selection, 
                              spectrum_t spectrum )
{
        int i, found_sp = 0;
        if( selection->use_spectrum )
        {
                for( i = 0; i < selection->use_spectrum && !found_sp; i++ )
                        if( selection->spectrum[i] == spectrum )
                                found_sp = 1;
                return found_sp;
        }
        else
                return 1;
}

void star_selection_set_allow_multiples( star_selection_t* selection, 
                                        int allow )
{
        selection->allow_multiples = allow;
}

int star_selection_get_allow_multiples( star_selection_t* selection )
{
        return selection->allow_multiples;
}

void star_selection_copy( star_selection_t* orig, star_selection_t* new )
{
        *new = *orig;
}

void star_selection_destroy( star_selection_t* selection )
{
        free( selection );
}

/* Private functions. */

void match_component( star_component_t *comp, star_selection_t *sel )
{
        int matches = 1;
        int found_sp;
        int i;
        spectrum_t spectrum;
        
        if( sel->use_min_lum )
                matches = (sel->min_lum < star_component_get_luminosity(comp));
        
        if( sel->use_max_lum && matches )
                matches = (sel->max_lum>= star_component_get_luminosity(comp));

        if( sel->use_spectrum && matches )
        {
                spectrum = star_component_get_spectrum_type( comp );
                found_sp = 0;
                for( i = 0; i < sel->use_spectrum && !found_sp; i++ )
                        if( sel->spectrum[i] == spectrum )
                                found_sp = 1;
                matches = found_sp;
        }
        
        sel->tmp_matched = sel->tmp_matched || matches;
}

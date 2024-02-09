/***********************************************************************
 * star.c : Implementation of star_t
 * $Id: star.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
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
#include <ctype.h>
#include <locale.h>
#include <glib.h>
#include "../include/star.h"

#define CNAME_LENGTH    15
#define SNAME_LENGTH    16
#define NAME_LENGTH     40
#define SPECTRUM_LENGTH 15
#define MAX_COMPONENTS   6
#define MAX_LINE       256

struct st_star
{
        star_id_t id;
        char cat_name[CNAME_LENGTH];
        char short_name[SNAME_LENGTH];
        coords_3d_t coords;
        double ra_h, ra_m, ra_s, dec_d, dec_m;
        star_component_t *components[MAX_COMPONENTS];
};

struct st_star_component
{
        char component;
        char dm_name[CNAME_LENGTH];
        char spectrum[SPECTRUM_LENGTH];
        char bayer_name[NAME_LENGTH];
        char proper_name[NAME_LENGTH];
        spectrum_t spectrum_type;
        float luminosity;
};

/* Definition of Local Functions */

static void terminate_string( char* string, size_t length );
static void set_spectrum_type( star_component_t* comp );

/* Public Functions */

/* Star Functions. */

star_t* star_new( void )
{
        star_t* star;

        star = (star_t*) g_malloc0( sizeof( struct st_star ) );

        star->id = 0;
        star->cat_name[0] = '\0';
        star->short_name[0] = '\0';
        star->coords.x = 0.0;
        star->coords.y = 0.0;
        star->coords.z = 0.0;
        star->ra_h = 0.0;
        star->ra_m = 0.0;
        star->ra_s = 0.0;
        star->dec_d = 0.0;
        star->dec_m = 0.0;
        star->components[0] = NULL;

        return star;
}

int star_read( star_t* star, FILE* file )
{
        char line[MAX_LINE];
        int success;
        star_component_t* component;
        char* locale;

        /* We change the locale number formatting to the standard, since the
         * program may have changed it, confusing scanf. */
        locale = setlocale( LC_NUMERIC, "C" );
        if( ( success = (int) fgets( line, MAX_LINE-1, file ) ) )
        {
                success = ( sscanf( line, "%d,%9c,%15c,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                    &star->id, star->cat_name, 
                                    star->short_name, &star->coords.x, 
                                    &star->coords.y, &star->coords.z,
                                    &star->ra_h, &star->ra_m, &star->ra_s,
                                    &star->dec_d, &star->dec_m ) == 11 );

                terminate_string( star->cat_name, CNAME_LENGTH );
                terminate_string( star->short_name, SNAME_LENGTH );

                while( success && fgets( line, MAX_LINE-1, file ) && 
                       line[0] != '%' )
                {
                        component = star_component_new();
                        success = ( sscanf( line, "%c,%14c,%39c,%39c,"
                                            "%14c,%f\n",
                                            &component->component, 
                                            component->dm_name, 
                                            component->proper_name,
                                            component->bayer_name,
                                            component->spectrum, 
                                            &component->luminosity ) == 6 );
                        if( !success )
                                star_component_destroy( component );
                        else
                        {
                                set_spectrum_type( component );
                                terminate_string( component->dm_name, 
                                                  CNAME_LENGTH );
                                terminate_string( component->proper_name,
                                                  NAME_LENGTH );
                                terminate_string( component->bayer_name,
                                                  NAME_LENGTH );
                                terminate_string( component->spectrum,
                                                  NAME_LENGTH );
                                star_add_component( star, component );
                        }
                }
        }
        setlocale( LC_NUMERIC, locale );

        return success;
}

inline void star_set_id( star_t* star, star_id_t id )
{
        star->id = id;
}

inline star_id_t star_get_id( star_t* star )
{
        return star->id;
}

inline void star_set_catalog_name( star_t* star, char* name )
{
        strncpy( star->cat_name, name, CNAME_LENGTH );
}

inline void star_get_catalog_name( star_t* star, char* name )
{
        strncpy( name, star->cat_name, CNAME_LENGTH );
}

inline void star_set_short_name( star_t* star, char* name )
{
        strncpy( star->short_name, name, SNAME_LENGTH );
}

inline void star_get_short_name( star_t* star, char* name )
{
        strncpy( name, star->short_name, SNAME_LENGTH );
}

inline void star_set_coords( star_t* star, coords_3d_t* coords )
{
        star->coords = *coords;
}

inline void star_get_coords( star_t* star, coords_3d_t* coords )
{
        *coords = star->coords;
}

inline void star_set_ra( star_t* star, double hours, double minutes, 
                         double seconds )
{
        star->ra_h = hours;
        star->ra_m = minutes;
        star->ra_s = seconds;
}

inline void star_get_ra( star_t* star, double *hours, double *minutes, 
                         double *seconds )
{
        *hours = star->ra_h;
        *minutes = star->ra_m;
        *seconds = star->ra_s;
}

inline void star_set_dec( star_t* star, double degrees, double minutes )
{
        star->dec_d = degrees;
        star->dec_m = minutes;
}

inline void star_get_dec( star_t* star, double *degrees, double *minutes )
{
        *degrees = star->dec_d;
        *minutes = star->dec_m;
}

inline int star_components( star_t* star )
{
        int i;
        for( i = 0; star->components[i]; i++ )
                ;
        return i;
}

void star_add_component( star_t* star, star_component_t* comp )
{
        int i;
        
        i = star_components( star );
        
        star->components[i] = comp;
        star->components[i+1] = NULL;
}

void star_foreach_component( star_t* star, 
                             void (*function)( star_component_t *component,
                                               void *data ),
                             void *data )
{
        int i;
        
        for( i = 0; star->components[i]; i++ )
                function( star->components[i], data );
}

inline void star_destroy( star_t* star )
{
        int i;
        for( i = 0; star->components[i]; i++ )
                star_component_destroy( star->components[i] );
        g_free( star );
}

/* Component Functions. */

star_component_t *star_component_new( void )
{
        star_component_t* comp;
        
        comp = g_malloc0( sizeof(star_component_t) );
        comp->component = ' ';
        comp->dm_name[0] = '\0';
        comp->dm_name[CNAME_LENGTH-1] = '\0';
        comp->spectrum[0] = '\0';
        comp->spectrum[SPECTRUM_LENGTH-1] = '\0';
        comp->bayer_name[0] = '\0';
        comp->bayer_name[NAME_LENGTH-1] = '\0';
        comp->proper_name[0] = '\0';
        comp->proper_name[NAME_LENGTH-1] = '\0';
        comp->spectrum_type = SPECTRUM_M;
        comp->luminosity = 0.0;
        
        return comp;
}

void star_component_set_component( star_component_t *comp, char name )
{
        comp->component = name;
}

char star_component_get_component( star_component_t *comp )
{
        return comp->component;
}

void star_component_set_proper_name( star_component_t* comp, char* name )
{
         strncpy( comp->proper_name, name, NAME_LENGTH );
}

void star_component_get_proper_name( star_component_t* comp, char* name )
{
        strncpy( name, comp->proper_name, NAME_LENGTH );
}

void star_component_set_bayer_name( star_component_t* comp, char* name )
{
        strncpy( comp->bayer_name, name, NAME_LENGTH );
}

void star_component_get_bayer_name( star_component_t* comp, char* name )
{
        strncpy( name, comp->bayer_name, NAME_LENGTH );
}

void star_component_set_dm_name( star_component_t* comp, char* name )
{
        strncpy( comp->dm_name, name, CNAME_LENGTH );
}

void star_component_get_dm_name( star_component_t* comp, char* name )
{
        strncpy( name, comp->dm_name, CNAME_LENGTH );
}

void star_component_set_spectrum( star_component_t* comp, char* spectrum )
{
        strncpy( comp->spectrum, spectrum, SPECTRUM_LENGTH );
}

void star_component_get_spectrum( star_component_t* comp, char* spectrum )
{
        strncpy( spectrum, comp->spectrum, SPECTRUM_LENGTH );
}

spectrum_t star_component_get_spectrum_type( star_component_t* comp )
{
        return comp->spectrum_type;
}

void star_component_set_luminosity( star_component_t* comp, float lum )
{
        comp->luminosity = lum;
}

float star_component_get_luminosity( star_component_t* comp )
{
        return comp->luminosity;
}

void star_component_destroy( star_component_t* comp )
{
        g_free( comp );
}

/* Local Functions */

void terminate_string( char* string, size_t length )
{
        register int i;
        
        string[length-1] = '\0';
        for( i = length-2; i && !isalnum( string[i] ); i-- )
                string[i] = '\0';
}

void set_spectrum_type( star_component_t* comp )
{
                if( comp->spectrum[0] == 'D' )
                comp->spectrum_type = SPECTRUM_WHITE_DWARF;
        else
        {
                switch( comp->spectrum[1] )
                {
                case 'O':
                        comp->spectrum_type = SPECTRUM_O;
                        break;
                case 'B':
                        comp->spectrum_type = SPECTRUM_B;
                        break;
                case 'A':
                        comp->spectrum_type = SPECTRUM_A;
                        break;
                case 'F':
                        comp->spectrum_type = SPECTRUM_F;
                        break;
                case 'G':
                        comp->spectrum_type = SPECTRUM_G;
                        break;
                case 'K':
                        comp->spectrum_type = SPECTRUM_K;
                        break;
                case 'M':
                default:
                        comp->spectrum_type = SPECTRUM_M;
                        break;
                }
        }
}

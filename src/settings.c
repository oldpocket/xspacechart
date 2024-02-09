/***********************************************************************
 * settings.c : Implementation of the settings of the map.
 * $Id: settings.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2000, 2001, 2002 Free Software Foundation, Inc.
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
#include <gdk/gdk.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "../include/starmap.h"
#include "../include/star_selection.h"
#include "../include/link_selection.h"
#include "../include/star_draw_rules.h"
#include "../include/link_draw_rules.h"
#include "../include/settings.h"
#include "../include/rcparser.h"
#include "../include/keys.h"

#define FNAME_LENGTH 256

typedef struct
{
        properties_set_t prop_set;
        void (*callback)(settings_t* settings, void* data);
        void *data;
} callback_t;

struct st_settings
{
        int callbacks_disabled;
        int cb_list_length;
        callback_t *callbacks;
        properties_set_t changed_properties;
        FILE *rcfile;
        char *catalog_filename;
        star_selection_t *star_filter;
        link_selection_t *link_filter;
        double view_radius;
        int show_links;
        int show_link_labels;
        int show_star_labels;
        double labels_color[3];
        char labels_font[FNAME_LENGTH];
        distance_unit_t distance_unit;
        double lat, lon, north;
        coords_3d_t center;
        coords_3d_t line_of_sight;
        coords_3d_t up;
        double view_distance;
        star_drawing_rules_t* star_draw_rules;
        link_drawing_rules_t* link_draw_rules;
        key_press_t keys[KEY_END_OF_LIST];
};

/* Macros for printing config files */
#define SETTINGS_UNIT(settings) settings->distance_unit == DISTANCE_PARSECS ? "pc" : "ly"

#define BOOL_STRING(bool) bool ? "true" : "false"

#define STYLE(style) (style == GDK_LINE_SOLID) ? "solid" : "dashed"

#define DISTANCE(dist,settings) settings->distance_unit == DISTANCE_PARSECS ? \
dist : dist * PARSEC_TO_LY

/* Definition of private functions. */
static int set_default_settings( settings_t *settings );
static void handle_callbacks( settings_t *settings );
static int read_config_files( rcparser_t *parser );

/* Auxiliar functions for saving the settings */

static void save_catalog( settings_t *settings );
static void save_coords( settings_t *settings );
static void save_view_radius( settings_t *settings );
static void save_max_link( settings_t *settings );
static void save_show( settings_t *settings );
static void save_labels( settings_t *settings );
static void save_distance_units( settings_t *settings );
static void save_star_selection( settings_t *settings );
static void save_star_rules( settings_t *settings );
static void save_link_rules( settings_t *settings );
static void save_keybindings( settings_t *settings );

/* Public Functions. */

settings_t* settings_new( void )
{
        settings_t *settings;
        int i;
        
        if( (settings = (settings_t*) malloc( sizeof( struct st_settings ))) &&
            (settings->callbacks = malloc(sizeof(callback_t))) )
        {
                settings->star_draw_rules = NULL;
                settings->link_draw_rules = NULL;
                settings->catalog_filename = NULL;
                settings->callbacks_disabled = 0;
                settings->callbacks[0].prop_set = 0;
                settings->cb_list_length = 0;
                settings->catalog_filename = NULL;
                settings->changed_properties = 0;
                for( i = 0; i < KEY_END_OF_LIST; i++ )
                {
                        settings->keys[i].key = 0;
                        settings->keys[i].mods = 0;
                }
                if( !set_default_settings( settings ) )
                {
                        free( settings );
                        settings = NULL;
                }
        }
        
        return settings;
}

int settings_load_from_files( settings_t *settings )
{
        rcparser_t *parser;

        parser = rcparser_new();

        if( !read_config_files( parser ) )
        {
                emit_error_message( _("Could not load configuration files. "
                                      "Aborting") );
                exit( EXIT_FAILURE );
        }

        settings_disable_callbacks( settings );
        rcparser_apply_to_settings( parser, settings );
        settings_enable_callbacks( settings );

        return 1;
}

int settings_save_settings( settings_t* settings )
{
        char filename[256];
        char buffer[256];
        
        snprintf( filename, sizeof(filename), "%s/.spacechartrc",
                  getenv("HOME") );
        if( !(settings->rcfile = fopen( filename, "w" )) )
        {
                snprintf( buffer, sizeof(buffer), "%s: %s", filename, 
                          strerror(errno) );
                emit_error_message(buffer);
                return 0;
        }

        save_catalog( settings );
        save_coords( settings );
        save_view_radius( settings );
        save_max_link( settings );
        save_show( settings );
        save_labels( settings );
        save_distance_units( settings );
        save_star_selection( settings );
        save_star_rules( settings );
        save_link_rules( settings );
        save_keybindings( settings );

        fclose( settings->rcfile );

        return 1;
}

int settings_add_callback( settings_t* settings, properties_set_t prop_set,
                           void (*callback)(settings_t* settings, void* data),
                           void* data )
{
        callback_t *list = settings->callbacks;
        if( (settings->callbacks = realloc( settings->callbacks,
                                            (settings->cb_list_length+2) *
                                            (sizeof(callback_t)) )) )
        {
                settings->callbacks[settings->cb_list_length].prop_set = 
                        prop_set;
                settings->callbacks[settings->cb_list_length].callback = 
                        callback;
                settings->callbacks[settings->cb_list_length].data = data;
                settings->callbacks[settings->cb_list_length+1].prop_set = 0;
                settings->cb_list_length++;
                return settings->cb_list_length;
        }
        else
        {
                settings->callbacks = list;
                return 0;
        }
}

void settings_disable_callbacks( settings_t* settings )
{
        settings->callbacks_disabled++;
}

void settings_enable_callbacks( settings_t* settings )
{
        if( settings->callbacks_disabled )
                settings->callbacks_disabled--;
        handle_callbacks( settings );
}

void settings_set_catalog_filename( settings_t *settings, 
                                    const char *filename )
{
        /* If the filename is NULL, no new catalog */
        if( !filename )
        {
                /* If we already had no catalog, don't do anything */
                if( settings->catalog_filename )
                {
                        free( settings->catalog_filename );
                        settings->catalog_filename = NULL;
                        settings->changed_properties = 
                                settings->changed_properties | 
                                PROPERTIES_CATALOG_FILENAME;
                        handle_callbacks( settings );
                }
        }
        else if( !( settings->catalog_filename && 
                    !strcmp( filename, settings->catalog_filename )) )
        {
                if( settings->catalog_filename )
                        free( settings->catalog_filename );
                settings->catalog_filename = strdup( filename );
                settings->changed_properties = 
                        settings->changed_properties | 
                        PROPERTIES_CATALOG_FILENAME;
                handle_callbacks( settings );
        }
}

char *settings_get_catalog_filename( settings_t *settings )
{
        return settings->catalog_filename;
}

void settings_set_star_filter( settings_t* settings,star_selection_t* filter )
{
        settings->star_filter = filter;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_STAR_FILTER;
        handle_callbacks( settings );
}

star_selection_t* settings_get_star_filter( settings_t* settings )
{
        return settings->star_filter;
}

void settings_set_link_filter( settings_t* settings,link_selection_t* filter )
{
        settings->link_filter = filter;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_LINK_FILTER;
        handle_callbacks( settings );
}

link_selection_t* settings_get_link_filter( settings_t* settings )
{
        return settings->link_filter;
}

void settings_set_view_radius( settings_t* settings, double view_radius )
{
        settings->view_radius = view_radius;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_VIEW_RADIUS;
        handle_callbacks( settings );
}

double settings_get_view_radius( settings_t* settings )
{
        return settings->view_radius;
}

void settings_set_show_links( settings_t* settings, int value )
{
        settings->show_links = value;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_SHOW_LINKS;
        handle_callbacks( settings );
}

int settings_get_show_links( settings_t* settings )
{
        return settings->show_links;
}

void settings_set_show_link_labels( settings_t* settings, int value )
{
        settings->show_link_labels = value;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_SHOW_LINK_LABELS;
        handle_callbacks( settings );
}

int settings_get_show_link_labels( settings_t* settings )
{
        return settings->show_link_labels;
}

void settings_set_show_star_labels( settings_t* settings, int value )
{
        settings->show_star_labels = value;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_SHOW_STAR_LABELS;
        handle_callbacks( settings );
}

int settings_get_show_star_labels( settings_t* settings )
{
        return settings->show_star_labels;
}

void settings_set_labels_color( settings_t* settings, double rgb[] )
{
        settings->labels_color[0] = rgb[0];
        settings->labels_color[1] = rgb[1];
        settings->labels_color[2] = rgb[2];
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_LABELS_COLOR;
        handle_callbacks( settings );
}

void settings_get_labels_color( settings_t* settings, double rgb[] )
{
        rgb[0] = settings->labels_color[0];
        rgb[1] = settings->labels_color[1];
        rgb[2] = settings->labels_color[2];
}

void settings_set_labels_font( settings_t* settings, const char *font )
{
        strncpy( settings->labels_font, font, FNAME_LENGTH );
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_LABELS_FONT;
        handle_callbacks( settings );
}

char *settings_get_labels_font( settings_t* settings )
{
        return settings->labels_font;
}

void settings_set_distance_unit( settings_t* settings, distance_unit_t unit )
{
        settings->distance_unit = unit;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_DISTANCE_UNIT;
        handle_callbacks( settings );
}

distance_unit_t settings_get_distance_unit( settings_t* settings )
{
        return settings->distance_unit;
}

void settings_set_center( settings_t* settings, coords_3d_t* center )
{
        settings->center = *center;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_CENTER;
        handle_callbacks( settings );
}

void settings_get_center( settings_t* settings, coords_3d_t* center )
{
        *center = settings->center;
}

void settings_set_sight_params( settings_t* settings, 
                                coords_3d_t* line_of_sight,
                                coords_3d_t* up )
{
        coords_3d_t los, north_pole;
        double product;

        settings->line_of_sight = *line_of_sight;
        settings->up = *up;
        
        los = settings->line_of_sight;

        cartesian_to_polar(&los);

        settings->lon = los.x;
        settings->lat = los.y;
        
        north_pole.x = -cos(settings->lon)*sin(settings->lat);
        north_pole.y =  -sin(settings->lon)*sin(settings->lat);
        north_pole.z = cos(settings->lat);

        product = (north_pole.x * settings->up.x) +
                (north_pole.y * settings->up.y) + 
                (north_pole.z * settings->up.z);

        /* Should never happen... but this is floating point we are
         * talking  about. */
        if( product > 1.0 )
                product = 1.0;
        else if( product < -1.0 )
                product = -1.0;

        settings->north = acos(product);

        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_SIGHT_PARAMS;
        handle_callbacks( settings );
}

void settings_get_sight_params( settings_t* settings, 
                                coords_3d_t* line_of_sight,
                                coords_3d_t* up )
{
        if( line_of_sight )
                *line_of_sight = settings->line_of_sight;
        if( up )
                *up = settings->up;
}

/* FIXME: The math knowledge should be out of these module. How? */
/* Many Thanks to Santiago Cifuentes, <tagore@apdo.com> for working out the
 * math used in this functions. */
void settings_set_sight_params_polar( settings_t* settings,
                                      double longitude, double latitude,
                                      double north )
{
        coords_3d_t los, up;

        settings->lon = longitude;
        settings->lat = latitude;
        settings->north = north;

        los.x = longitude;
        los.y = latitude;
        los.z = 1;
        polar_to_cartesian(&los);

        up.x = -cos(north)*cos(longitude)*sin(latitude)+sin(north)*sin(longitude);
        up.y = -cos(north)*sin(longitude)*sin(latitude)-sin(north)*cos(longitude);
        up.z = cos(north)*cos(latitude);

        settings_set_sight_params(settings, &los, &up);
}

void settings_get_sight_params_polar( settings_t* settings,
                                      double *longitude, double *latitude,
                                      double *north )
{
        *longitude = settings->lon;
        *latitude = settings->lat;
        *north = settings->north;
}

void settings_set_star_draw_rules( settings_t* settings, 
                                   star_drawing_rules_t* rules )
{
        if( settings->star_draw_rules )
                star_drawing_rules_destroy( settings->star_draw_rules );
        settings->star_draw_rules = rules;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_STAR_DRAW_RULES;
        handle_callbacks( settings );
}

star_drawing_rules_t* settings_get_star_draw_rules( settings_t* settings )
{
        return settings->star_draw_rules;
}

void settings_find_star_draw( settings_t* settings, star_t* star, int* radius, 
                              double rgb[], int* show_name )
{
        star_drawing_rules_find( settings->star_draw_rules, star, radius, rgb, 
                                 show_name );
}

void settings_set_link_draw_rules( settings_t* settings, 
                                   link_drawing_rules_t* rules )
{
        if( settings->link_draw_rules )
                link_drawing_rules_destroy( settings->link_draw_rules );
        settings->link_draw_rules = rules;
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_LINK_FILTER;
        handle_callbacks( settings );
}

link_drawing_rules_t* settings_get_link_draw_rules( settings_t* settings )
{
        return settings->link_draw_rules;
}

void settings_find_link_draw( settings_t* settings, link_t* link, int* width, 
                              GdkLineStyle *style, double rgb[] )
{
        link_drawing_rules_find( settings->link_draw_rules, link, width,
                                 style, rgb );
}

void settings_set_keybindings( settings_t* settings, key_press_t *keys )
{
        memcpy(settings->keys, keys, sizeof(settings->keys));
        settings->changed_properties = settings->changed_properties | 
                PROPERTIES_KEYBINDINGS;
        handle_callbacks( settings );
}

void settings_get_keybindings( settings_t* settings, key_press_t *keys )
{
        memcpy(keys, settings->keys, sizeof(settings->keys));
}

void settings_destroy( settings_t* settings )
{
        free( settings->callbacks );
        star_selection_destroy( settings->star_filter );
        link_selection_destroy( settings->link_filter );
        star_drawing_rules_destroy( settings->star_draw_rules );
        link_drawing_rules_destroy( settings->link_draw_rules );
}

/* Private Functions */

void handle_callbacks( settings_t* settings )
{
        int i;
        properties_set_t changed;

        /* A callback could change the settings, thus launching another 
         * handle_callbacks(), but if the changed properties are not cleared
         * after the end of this function, the cb would be called again, in an
         * possibly infinite way. So we save the current changes and clear the
         * ones in settings.
         */

        if( !settings->callbacks_disabled )
        {
                changed = settings->changed_properties;        
                settings->changed_properties = 0;
                for( i = 0; settings->callbacks[i].prop_set; i++ )
                {
                        if( (settings->callbacks[i].prop_set & changed) )
                        {
                                settings->callbacks[i].
                                        callback(settings,
                                                 settings->callbacks[i].data);
                        }
                }
        }
}

int read_config_files( rcparser_t *parser )
{
        char filename[256];
        char buffer[256];
        struct stat status;
        int success;

        /* Read the global configuration file */
        snprintf( filename, sizeof(filename), "%s/spacechartrc", "~/.spacechart" );
        if( access( filename, R_OK ) != -1 )
                success = rcparser_parse_file( parser, filename );
        else
        {
                snprintf( buffer, sizeof(buffer), "%s: %s", filename, 
                          strerror(errno) );
                emit_error_message(buffer);
                exit( EXIT_FAILURE );
        }

        if( success == 1 )
                return 0;

        /* Now the user's config file, if it exists */
        snprintf( filename, sizeof(filename), "%s/.spacechartrc",
                  getenv("HOME") );
        if( stat( filename, &status ) == -1 )
        {
                if( errno != ENOENT )
                {
                        snprintf( buffer, sizeof(buffer), "%s: %s",
                                  filename, strerror(errno) );
                        emit_error_message(buffer);
                }
                else
                        /* The file does not exist. Don't worry. */
                        return 1;
        }
        if( (access( filename, R_OK ) != -1) &&
            ( (status.st_mode & S_IFMT) != S_IFDIR ) )
                success = rcparser_parse_file( parser, filename );

        if( success == 1 )
                return 0;
        return 1;
}

int set_default_settings( settings_t *settings )
{
#ifdef undef
        coords_3d_t line_of_sight, center, up;
        star_drawing_rules_t* star_rules;
        link_drawing_rules_t* link_rules;
        gdouble color[3] = { 1.0, 0.0, 0.0 };
        star_selection_t* s_selection;
        link_selection_t* l_selection;
        double radius;

        /* FIXME: Check for memory errors */

        /* Just in case, we disable the callbacks. */
        settings_disable_callbacks( settings );

        /* First, the default coordinates */
        radius = 5.5;

        line_of_sight.x = 1;
        line_of_sight.y = 0;
        line_of_sight.z = 0;

        center.x = 0;
        center.y = 0;
        center.z = 0;

        up.x = 0;
        up.y = 0;
        up.z = 1;

        /* Now, the star drawing rules. */
        star_rules = star_drawing_rules_new( DEFAULT_SIZE, default_color );

        s_selection = star_selection_new();
        star_selection_act_min_lum( s_selection, 100.0 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_LUMINOSITY, 
                                5, NULL, FALSE );
        
        s_selection = star_selection_new();
        star_selection_act_min_lum( s_selection, 50.0 );
        star_selection_act_max_lum( s_selection, 100.0 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_LUMINOSITY, 
                                4, NULL, FALSE );

        s_selection = star_selection_new();
        star_selection_act_min_lum( s_selection, 10.0 );
        star_selection_act_max_lum( s_selection, 50.0 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_LUMINOSITY, 
                                3, NULL, FALSE );

        s_selection = star_selection_new();
        star_selection_act_min_lum( s_selection, 0.4 );
        star_selection_act_max_lum( s_selection, 10.0 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_LUMINOSITY, 
                                2, NULL, FALSE );

        s_selection = star_selection_new();
        star_selection_act_max_lum( s_selection, 0.4 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_LUMINOSITY, 
                                1, NULL, FALSE );

        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 1.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_O );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_O, 
                                0, color, FALSE );

        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 1.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_B );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_B, 
                                0, color, FALSE );

        color[0] = 0.90;
        color[1] = 1.0;
        color[2] = 1.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_A );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_A, 
                                0, color, FALSE );

        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 0.75;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_F );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_F, 
                                0, color, FALSE );

        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 0.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_G );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_G, 
                                0, color, FALSE );

        color[0] = 1.0;
        color[1] = 0.5;
        color[2] = 0.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_K );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_K, 
                                0, color, FALSE );

        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 0.0;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_M );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_M, 
                                0, color, FALSE );

        color[0] = 0.75;
        color[1] = 0.75;
        color[2] = 0.75;
        s_selection = star_selection_new();
        star_selection_act_spectrum( s_selection, SPECTRUM_WHITE_DWARF );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SPECTRUM_WD, 
                                0, color, FALSE );

        s_selection = star_selection_new();
        star_selection_act_min_lum( s_selection, 1.00 );
        star_drawing_rules_add( star_rules, s_selection, PRIORITY_SHOW_NAME, 
                                0, NULL, TRUE );

        /* Initialization of the line drawing rules */
        color[0] = 0.0;
        color[1] = 0.36;
        color[2] = 0.6;
        link_rules = link_drawing_rules_new( 0, GDK_LINE_DOUBLE_DASH, color );

        color[0] = 0.0;
        color[1] = 0.7;
        color[2] = 1.0;
        l_selection = link_selection_new();
        link_selection_act_max_length( l_selection, 1.5 );
        link_drawing_rules_add( link_rules, l_selection, 0, GDK_LINE_SOLID,
                                color );

        color[0] = 0.0;
        color[1] = 0.48;
        color[2] = 0.8;
        l_selection = link_selection_new();
        link_selection_act_min_length( l_selection, 1.5 );
        link_selection_act_max_length( l_selection, 2.0 );
        link_drawing_rules_add( link_rules, l_selection, 0, GDK_LINE_SOLID,
                                color );

        /* Now we set the selections */
        s_selection = star_selection_new(); /* Empty selection: all matches */
        l_selection = link_selection_new();
        link_selection_act_max_length( l_selection, 2.5 );

        /* Finally, write everything to the settings. */
        settings_set_center( settings, &center );
        settings_set_sight_params( settings, &line_of_sight, &up );
        settings_set_view_radius( settings, radius );
        settings_set_star_filter( settings, s_selection );
        settings_set_link_filter( settings, l_selection );
        settings_set_star_draw_rules( settings, star_rules );
        settings_set_link_draw_rules( settings, link_rules );
        settings_set_show_links( settings, TRUE );
        settings_set_show_link_labels( settings, FALSE );
        settings_set_show_star_labels( settings, TRUE );
        color[0] = 0.0;
        color[1] = 1.0;
        color[2] = 1.0;
        settings_set_labels_color( settings, color );
        settings_set_labels_font( settings, 
                                  "-misc-fixed-medium-r-semicondensed"
                                  "-*-13-*-*-*-c-*-iso8859-8" );
        settings_set_distance_unit( settings, DISTANCE_PARSECS );

        /* Enable the callbacks */
        settings_enable_callbacks( settings );

#endif
        return 1;
}

/* Auxiliar functions for saving the settings */

void save_catalog( settings_t *settings )
{
        if( settings->catalog_filename )
                fprintf( settings->rcfile, "catalog = \"%s\"\n\n",
                         settings->catalog_filename );
}

void save_coords( settings_t *settings )
{
        double h, m;
        lon_to_ra( settings->lon, &h, &m );
        fprintf( settings->rcfile, "user_coords = ( %f, %fh %fm )\n\n", 
                 settings->lat * 180/M_PI, h, m );
        fprintf( settings->rcfile, "north = %f\n\n", 
                 settings->north * 180/M_PI );
        fprintf( settings->rcfile, "center = ( %f, %f, %f)\n\n",
                 settings->center.x, settings->center.y, settings->center.z );
}

void save_view_radius( settings_t *settings )
{
        fprintf( settings->rcfile, "view_radius = %f %s\n\n", 
                 DISTANCE(settings->view_radius, settings),
                 SETTINGS_UNIT(settings) );
}

void save_max_link( settings_t *settings )
{
        fprintf( settings->rcfile, "max_link = %f %s\n\n",
                 DISTANCE(link_selection_get_max_length(settings->link_filter),
                          settings), SETTINGS_UNIT(settings) );
}

void save_show( settings_t *settings )
{
        fprintf( settings->rcfile, "show_links = %s\n\n",
                 BOOL_STRING(settings->show_links) );
        fprintf( settings->rcfile, "show_link_labels = %s\n\n", 
                 BOOL_STRING(settings->show_link_labels) );
        fprintf( settings->rcfile, "show_star_labels = %s\n\n", 
                 BOOL_STRING(settings->show_star_labels) );
}

static void print_rgb( double rgb[], FILE *file )
{
        fprintf( file, "(R:%4.2f; G:%4.2f; B:%4.2f)", 
                 rgb[0], rgb[1], rgb[2] );       
}

void save_labels( settings_t *settings )
{
        fprintf( settings->rcfile, "%s", "labels_color = " );
        print_rgb( settings->labels_color, settings->rcfile );
        fprintf( settings->rcfile, "%s", "\n\n" );
        fprintf( settings->rcfile, "labels_font = \"%s\"\n\n", settings->labels_font );
}

void save_distance_units( settings_t *settings )
{
        fprintf( settings->rcfile, "distance_unit = %s\n\n", SETTINGS_UNIT(settings) );
}

static void print_star_selection( star_selection_t *sel, FILE *file )
{
        fprintf( file, "%s", "(" );
        if( star_selection_is_min_lum( sel ) )
                fprintf(file, "%5.2f", star_selection_get_min_lum( sel ) );
        fprintf( file, ";" );
        if( star_selection_is_max_lum( sel ) )
                fprintf(file, "%5.2f", star_selection_get_max_lum( sel ) );
        fprintf( file, "%s", ";[" );
        if( star_selection_is_showed( sel, SPECTRUM_O ) )
                fprintf( file, "%c", 'O' );
        if( star_selection_is_showed( sel, SPECTRUM_B ) )
                fprintf( file, "%c", 'B' );
        if( star_selection_is_showed( sel, SPECTRUM_A ) )
                fprintf( file, "%c", 'A' );
        if( star_selection_is_showed( sel, SPECTRUM_F ) )
                fprintf( file, "%c", 'F' );
        if( star_selection_is_showed( sel, SPECTRUM_G ) )
                fprintf( file, "%c", 'G' );
        if( star_selection_is_showed( sel, SPECTRUM_K ) )
                fprintf( file, "%c", 'K' );
        if( star_selection_is_showed( sel, SPECTRUM_M ) )
                fprintf( file, "%c", 'M' );
        if( star_selection_is_showed( sel, SPECTRUM_WHITE_DWARF ) )
                fprintf( file, "%c", 'W' );
        fprintf( file, "];%s)", 
                 BOOL_STRING( star_selection_get_allow_multiples( sel ) ) );
}

void save_star_selection( settings_t *settings )
{
        fprintf( settings->rcfile, "%s", "star_selection = " );
        print_star_selection( settings->star_filter, settings->rcfile );
        fprintf( settings->rcfile, "%s", "\n\n" );
}

static void cb_print_color( star_selection_t* selection, 
                            int is_radius, int radius, int is_rgb, 
                            double rgb[], int show_name, FILE *file )
{
        if( is_rgb )
        {
                if( star_selection_is_showed( selection, SPECTRUM_O ) )
                        fprintf( file, "%s", "O: " );
                if( star_selection_is_showed( selection, SPECTRUM_B ) )
                        fprintf( file, "%s", "B: " );
                if( star_selection_is_showed( selection, SPECTRUM_A ) )
                        fprintf( file, "%s", "A: " );
                if( star_selection_is_showed( selection, SPECTRUM_F ) )
                        fprintf( file, "%s", "F: " );
                if( star_selection_is_showed( selection, SPECTRUM_G ) )
                        fprintf( file, "%s", "G: " );
                if( star_selection_is_showed( selection, SPECTRUM_K ) )
                        fprintf( file, "%s", "K: " );
                if( star_selection_is_showed( selection, SPECTRUM_M ) )
                        fprintf( file, "%s", "M: " );
                if( star_selection_is_showed( selection, SPECTRUM_WHITE_DWARF))
                        fprintf( file, "%s", "W: " );
                print_rgb( rgb, file );
                if(star_selection_is_showed( selection, SPECTRUM_WHITE_DWARF))
                        fprintf( file, "%s", " ), \\\n" );
                else
                        fprintf( file, "%s", "; \\\n" );
        }        
}

static void cb_save_size( star_selection_t* selection, 
                          int is_radius, int radius, int is_rgb, 
                          double rgb[], int show_name,  double size[] )
{
        if( is_radius )
        {
                if( radius == SIZE_1 )
                        size[0] = star_selection_get_min_lum( selection );
                if( radius == SIZE_2 )
                        size[1] = star_selection_get_min_lum( selection );
                if( radius == SIZE_3 )
                        size[2] = star_selection_get_min_lum( selection );
                if( radius == SIZE_4 )
                        size[3] = star_selection_get_min_lum( selection );
        }
}

static void cb_print_show_name( star_selection_t* selection, 
                                int is_radius, int radius, int is_rgb, 
                                double rgb[], int show_name, FILE *file )
{
        print_star_selection( selection, file );
}

void save_star_rules( settings_t *settings )
{
        double size[4];

        fprintf( settings->rcfile, "%s", "star_rules = ( ( " );
        star_drawing_rules_foreach( settings->star_draw_rules, 
                                    PRIORITY_SPECTRUM_O, PRIORITY_SPECTRUM_WD, 
                                    (void (*)( star_selection_t*, int, int, 
                                               int, double*, int, 
                                               void* ))cb_print_color, settings->rcfile );
        star_drawing_rules_foreach( settings->star_draw_rules,
                                    PRIORITY_LUMINOSITY, PRIORITY_LUMINOSITY, 
                                    (void (*)( star_selection_t*, int, int, 
                                               int, double*, int, 
                                               void* ))cb_save_size, size );
        fprintf( settings->rcfile, "( %5.2f; %5.2f; %5.2f; %5.2f ), \\\n", 
                 size[0], size[1], size[2], size[3] );
        star_drawing_rules_foreach( settings->star_draw_rules,
                                    PRIORITY_SHOW_NAME, PRIORITY_SHOW_NAME, 
                                    (void (*)(star_selection_t*, int, int, 
                                              int, double*, int, 
                                              void*))cb_print_show_name, settings->rcfile );
        fprintf( settings->rcfile, "%s", ")\n\n" );
}

static void cb_print_rule( link_selection_t* selection, int width, 
                           GdkLineStyle style, double rgb[],
                           settings_t *settings )
{
        fprintf( settings->rcfile, "%s", "( ");
        if( selection )
                fprintf( settings->rcfile, "%5.2f %s; ",  
                         DISTANCE(link_selection_get_max_length(selection),
                                  settings), SETTINGS_UNIT(settings) );
        print_rgb( rgb, settings->rcfile );
        fprintf( settings->rcfile, "; %s; %d )", STYLE(style), width );
        if( selection ) 
                fprintf( settings->rcfile, ",\\\n" );
}

void save_link_rules( settings_t *settings )
{
        int width;
        GdkLineStyle style;
        double rgb[3];

        fprintf( settings->rcfile, "%s", "link_rules = ( \\\n" );
        link_drawing_rules_foreach( settings->link_draw_rules, 
                                    (void(*)( link_selection_t*,int,
                                              GdkLineStyle,double*,void*))
                                    cb_print_rule, settings );
        link_drawing_rules_get_default( settings->link_draw_rules, 
                                        &width, &style, rgb );
        cb_print_rule( NULL, width, style, rgb, settings );
        fprintf( settings->rcfile, "%s", " )\n\n" );
}

void save_keybindings( settings_t *settings )
{
        int i;
        const char *name;
        for( i = 0; i < KEY_END_OF_LIST; i++ )
        {
                if( settings->keys[i].key )
                {
                        fprintf( settings->rcfile, "%s = ", get_keybinding_name(i) );
                        if( settings->keys[i].mods )
                                fprintf( settings->rcfile, "%s + ", 
                                         get_mod_name(settings->keys[i].mods) );
                        name = get_key_name(settings->keys[i].key);
                        if( name )
                                fprintf( settings->rcfile, "%s\n", 
                                         get_mod_name(settings->keys[i].key) );
                        else
                                fprintf( settings->rcfile, "'%c'\n", 
                                         settings->keys[i].key );
                }
        }
}

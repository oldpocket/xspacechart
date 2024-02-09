/***********************************************************************
 * map.c : Implementation of the map.
 * $Id: map.c,v 1.1 2017/09/23 00:04:10 strauss Exp $
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

#include "../include/map_widget.h"
#include "../include/map.h"
#include "../include/starmap.h"
#include "../include/star.h"
#include "../include/link.h"
#include "../include/star_index.h"
#include "../include/link_index.h"
#include "../include/perspective.h"
#include "../include/settings.h"

struct st_map
{
        settings_t *settings;
        map_widget_t *map_widget;
        link_t **link_list;
        link_index_t *link_index;
        star_t **star_list;
        star_index_t *star_index;
        perspective_t *persp;
        void (*star_clicked)( star_t* star, void* data );
        void* star_clicked_data;
        GdkFont *font;
        /* Data needed for checking which star was clicked on */
        double lambda;
        double clicked_x, clicked_y;
        /* This is a kludge to keep know when a callback from the sight params
         * comes from our own changes. */
        int changed_sight;
};

/* Definition of local functions */ 
static void cb_clicked_on_map( map_widget_t* map_widget, double x, double y,
                               double lambda, map_t *map );
static void cb_redraw_map( map_widget_t* map_widget, map_t *map );
static void cb_rotate_map( map_widget_t* map_widget, double v_ang, 
                           double h_ang, map_t *map );
static void cb_view_radius(settings_t* settings, map_t *map);
static void cb_drawing_settings(settings_t* settings, map_t *map);
static void cb_sight_params(settings_t* settings, map_t *map);
static void cb_center(settings_t* settings, map_t *map);
static int cb_is_clicked_star(star_t* star, double x, double y, map_t *map );

static void build_indexes( map_t *map );
static void draw( map_t *map );
static void draw_star( star_t* star, double x, double y, map_t* map );
static void draw_link( link_t* link, double x1, double y1, 
                       double x2, double y2, map_t* map );

/* Public functions */

map_t* map_new( settings_t* settings )
{
        map_t* map;
        
        if( (map = (map_t*) malloc( sizeof( struct st_map ) ) ) )
        {
                map->settings = settings;
                map->map_widget = map_widget_new( 
                        (void (*)(map_widget_t*, void*)) cb_redraw_map, 
                        (void (*)( map_widget_t*, double v_ang,
                                   double h_ang, void* data )) cb_rotate_map,
                        (void (*)( map_widget_t*, double, double, double,
                                   void* )) cb_clicked_on_map, 
                        map );
                map->link_list = NULL;
                map->link_index = NULL;
                map->star_list = NULL;
                map->star_index = NULL;
                map->star_clicked = NULL;
                map->star_clicked_data = NULL;
                map->changed_sight = 0;
                map->font = NULL;
                
                /* Set up the perspective. */
                map->persp = NULL;
                /* Now we set the callbacks we want from the settings. */
                settings_add_callback( settings, PROPERTIES_VIEW_RADIUS,
                                       (void(*)(settings_t*, void*))
                                       cb_view_radius, map );
                settings_add_callback( settings, 
                                       PROPERTIES_SHOW_LINKS | 
                                       PROPERTIES_SHOW_LINK_LABELS |
                                       PROPERTIES_SHOW_STAR_LABELS |
                                       PROPERTIES_LABELS_COLOR |
                                       PROPERTIES_LABELS_FONT |
                                       PROPERTIES_DISTANCE_UNIT |
                                       PROPERTIES_STAR_DRAW_RULES |
                                       PROPERTIES_LINK_DRAW_RULES,
                                       (void(*)(settings_t*, void*))
                                       cb_drawing_settings, map );
                settings_add_callback( settings,
                                       PROPERTIES_SIGHT_PARAMS,
                                       (void(*)(settings_t*, void*))
                                       cb_sight_params, map );
                settings_add_callback( settings,
                                       PROPERTIES_CENTER,
                                       (void(*)(settings_t*, void*))
                                       cb_center, map );
        }
        
        return map;
}

/* Returns the widget that contains the map. */
GtkWidget* map_get_widget( map_t* map )
{
        return map_widget_get_widget( map->map_widget );
}

/* Get the settings data structure used by the map. */
settings_t* map_get_settings( map_t* map )
{
        return map->settings;
}

/* Set the function called when the user clicks on a star. */
void map_set_star_clicked( map_t* map, 
                           void (*clicked)( star_t* star, void* data ), 
                           void* data )
{
        map->star_clicked = clicked;
        map->star_clicked_data = data;
}

void map_rotate( map_t* map, double v_ang, double h_ang )
{
        coords_3d_t from, up, center;
        double distance;
        
        /* Rotate and redraw */
        perspective_rotate( map->persp, v_ang, h_ang );
        build_indexes( map );
        draw( map );
        /* Now we tell the settings where we are. We remember to tell ourselves
           to ignore the callback from the settings. */
        map->changed_sight = 1;
        perspective_get_up( map->persp, &up );
        perspective_get_from( map->persp, &from );
        perspective_get_center( map->persp, &center );
        from.x -= center.x;
        from.y -= center.y;
        from.z -= center.z;
        distance = magnitude( &from );
        from.x /= distance;
        from.y /= distance;
        from.z /= distance;
        settings_set_sight_params( map->settings, &from, &up );
}

void map_set_shown_objects( map_t* map, star_t *star_list[], 
                            link_t *link_list[] )
{
        if( map->star_list )
                free( map->star_list );
        map->star_list = star_list;
        if( map->link_list )
        {
                int i;
                for( i = 0; map->link_list[i]; i++ )
                        link_destroy( map->link_list[i] );
                free( map->link_list );
        }
        map->link_list = link_list;
        build_indexes( map );
        draw( map );
}

/* Destroy the map and related data structures. */
void map_destroy( map_t* map )
{
        map_widget_destroy( map->map_widget );
        free( map->link_list );
        if( map->link_index )
                link_index_destroy( map->link_index );
        free( map->star_list );
        if( map->star_index )
                star_index_destroy( map->star_index );
        perspective_destroy( map->persp );
        free( map );
}

/* Private Functions */

void build_indexes( map_t *map )
{
        /* If the old one is not NULL destroy it. */
        if( map->star_index )
        {
                star_index_destroy( map->star_index );
                map->star_index = NULL;
        }
        /* Do not try to build the index if we have no stars yet */
        if( map->star_list )
                map->star_index = star_index_new( map->persp, map->star_list );
        if( map->link_index )
        {
                link_index_destroy( map->link_index );
                map->link_index = NULL;
        }
        if( map->link_list )
                map->link_index = link_index_new( map->persp, map->link_list );
}

void draw( map_t *map )
{
        map_widget_clear( map->map_widget );
        if(  map->link_index && settings_get_show_links(map->settings) )
                link_index_foreach( map->link_index, 
                                    (void(*)(link_t*, double, double, 
                                             double, double, void*))draw_link, 
                                    map );
        if(  map->star_index )
                star_index_foreach( map->star_index, 
                                    (void(*)(star_t*, double, double, void*))
                                    draw_star, map );
        map_widget_show( map->map_widget );
}

void draw_star( star_t* star, double x, double y, map_t* map )
{
        int show_name, radius;
        double rgb[3];
        char name[30]; /* FIXME: This size should be defined elsewhere. */
        
        settings_find_star_draw( map->settings, star, &radius, 
                                 rgb, &show_name );
        map_widget_draw_point( map->map_widget, x, y, radius, rgb );
        if( show_name && settings_get_show_star_labels( map->settings ) ) 
        {
                settings_get_labels_color( map->settings, rgb );
                star_get_short_name( star, name );
                map_widget_draw_label( map->map_widget, x, y, radius, 
                                       map->font, rgb, name );
        }                              
}

void draw_link( link_t* link, double x1, double y1, 
                double x2, double y2, map_t* map )
{
        int width;
        GdkLineStyle style;
        double rgb[3];
        double distance, x, y;
        char label[5];
        distance_unit_t unit;

        settings_find_link_draw( map->settings, link, &width, &style, rgb );
        map_widget_draw_line( map->map_widget, x1, y1, x2, y2, width, 
                              rgb, style );
        if( settings_get_show_link_labels( map->settings ) )
        {
                distance = link_get_distance( link );
                unit = settings_get_distance_unit( map->settings );
                sprintf( label, "%5.1f", (unit==DISTANCE_PARSECS) ? distance : 
                         distance*PARSEC_TO_LY );
                x = (x1+x2)/2;
                y = (y1+y2)/2;
                settings_get_labels_color( map->settings, rgb );
                map_widget_draw_label( map->map_widget, x, y, 0, map->font, 
                                       rgb, label );
        }
}

void cb_clicked_on_map( map_widget_t* map_widget, double x, double y,
                        double lambda, map_t *map )
{
        star_t *star;
        /* If there is no loaded map, just ignore the click */
        if( !map->star_index )
                return;
        map->lambda = lambda;
        map->clicked_x = x;
        map->clicked_y = y;
        star = star_index_find_first( map->star_index,
                                      (int(*)(star_t*,double,double,void*))
                                      cb_is_clicked_star,
                                      map, STAR_INDEX_START_BEGIN );
        if( star )
                map->star_clicked( star, map->star_clicked_data );
}

void cb_redraw_map( map_widget_t* map_widget, map_t *map )
{
        draw( map );
}

void cb_rotate_map( map_widget_t* map_widget, double v_ang, 
                    double h_ang, map_t *map )
{
        map_rotate( map, v_ang, h_ang );
}

void cb_view_radius(settings_t* settings, map_t *map)
{
        /* The radius of the map has changed. Therefore we must move closer
         * or further from the center, so that the map fills the window. */
        double view_radius = settings_get_view_radius( settings );
        double distance;
        coords_3d_t up, from, center;

        distance = sqrt( pow( view_radius, 2 ) + 
                         pow( view_radius * (cos(THETA/2)/sin(THETA/2)), 2 ) );
        settings_get_sight_params( settings, &from, &up );
        settings_get_center( settings, &center );
        from.x *= distance;
        from.y *= distance;
        from.z *= distance;
        from.x += center.x;
        from.y += center.y;
        from.z += center.z;
        if( map->persp )
                perspective_destroy( map->persp );
        map->persp = perspective_new( &from, &center, &up );
        build_indexes(map);
        draw( map );
}

void cb_drawing_settings(settings_t* settings, map_t *map)
{
        if( map->font )
                gdk_font_unref( map->font );
        map->font = gdk_font_load(settings_get_labels_font(settings));
        draw( map );
}

void cb_sight_params(settings_t* settings, map_t *map)
{
        coords_3d_t from, center, up;
        double distance, view_radius;
        
        /* We start by checking that we are not the cause of the callback */
        if( !map->changed_sight )
        {
                view_radius = settings_get_view_radius(settings);
                settings_get_center( settings, &center );
                settings_get_sight_params( settings, &from, &up );
                /* This is how far we must be from the center to see all stars
                 * under a 60º cone of vision */
                distance = sqrt( pow( view_radius, 2 ) + 
                                 pow( view_radius * (cos(THETA/2)/sin(THETA/2))
                                      , 2 ) );
                /* Move away from the center by "distance" */
                from.x *= distance;
                from.y *= distance;
                from.z *= distance;
                /* The perspective needs the absolute coordinates of the
                 * observer, not relative to the center */
                from.x += center.x;
                from.y += center.y;
                from.z += center.z;
                /* Rebuild the perspective */
                if( map->persp )
                        perspective_destroy(map->persp);
                map->persp = perspective_new( &from, &center, &up );
                build_indexes(map);
                draw(map);
        }
        else
                map->changed_sight = 0;
}

void cb_center(settings_t* settings, map_t *map)
{
        coords_3d_t center;
        settings_get_center( settings, &center );
        perspective_change_center( map->persp, &center );
        build_indexes(map);
        draw( map );
}

int cb_is_clicked_star(star_t* star, double x, double y, map_t *map )
{
        int radius, show_name, is_clicked;
        double rgb[3], star_x, star_y;

        settings_find_star_draw( map->settings, star, &radius, 
                                 rgb, &show_name );

        star_x = x*(map->lambda);
        star_y = y*(map->lambda);

        star_x = abs( map->clicked_x - star_x );
        star_y = abs( map->clicked_y - star_y );

        is_clicked = ( star_x < (radius+1) ) && ( star_y < (radius+1) );

        return is_clicked;
}

/***********************************************************************
 * starrule.c : Implementation of the rules for drawing stars.
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
#include "../include/star.h"
#include "../include/star_selection.h"
#include "../include/star_draw_rules.h"

struct list_node
{
        int priority;
        star_selection_t* selection;
        int is_radius;
        int radius;
        int is_rgb;
        double rgb[3];
        int show_name;
        struct list_node* next;
};

struct st_star_draw_rules
{
        int default_radius;
        double default_rgb[3];
        struct list_node* list;
};

/* Definition of local functions */

static int list_add( struct list_node** list, star_selection_t* selection,
                     int priority, int radius, double rgb[], int show_name );
static void list_find( struct list_node* list, star_t* star, int* have_radius,
                       int* radius, int* have_rgb, double rgb[], 
                       int* show_name );
static void list_foreach( struct list_node* list, int min_priority, 
                          int max_priority,
                          void (*function)( star_selection_t* selection,
                                            int is_radius, int radius, 
                                            int is_rgb, double rgb[],
                                            int show_name, void *data ),
                          void *data );
static void list_destroy( struct list_node* list );

/* Public functions */

star_drawing_rules_t* star_drawing_rules_new( int default_radius, 
                                              double default_rgb[] )
{
        star_drawing_rules_t* rules;
        
        if( ( rules = (star_drawing_rules_t*) 
              malloc( sizeof( struct st_star_draw_rules ) ) ) )
        {
                rules->default_radius = default_radius;
                rules->default_rgb[0] = default_rgb[0];
                rules->default_rgb[1] = default_rgb[1];
                rules->default_rgb[2] = default_rgb[2];
                rules->list = NULL;
        }
        
        return rules;
}

int star_drawing_rules_add( star_drawing_rules_t* rules,
                            star_selection_t* selection,
                            int priority,
                            int radius, double rgb[], int show_name )
{
        return list_add( &(rules->list), selection, priority, 
                         radius, rgb, show_name );
}

void star_drawing_rules_find( star_drawing_rules_t* rules,
                              star_t* star, int* radius, double rgb[],
                              int* show_name )
{
        int have_radius = 0, have_rgb = 0;

        *show_name = 0;
        list_find( rules->list, star, &have_radius, radius, 
                   &have_rgb, rgb, show_name );
        if( !have_radius )
                *radius = rules->default_radius;
        if( !have_rgb )
        {
                rgb[0] = rules->default_rgb[0];
                rgb[1] = rules->default_rgb[1];
                rgb[2] = rules->default_rgb[2];
        }
}

void star_drawing_rules_foreach( star_drawing_rules_t* rules,
                                 int min_priority, int max_priority,
                                 void (*function)( star_selection_t* selection,
                                            int is_radius, int radius, 
                                            int is_rgb, double rgb[],
                                            int show_name, void *data ),
                                 void *data )
{
        list_foreach( rules->list, min_priority, max_priority, function, data);
}

void star_drawing_rules_destroy( star_drawing_rules_t* rules )
{
        list_destroy( rules->list );
        free( rules );
}

/* Local Functions */

int list_add( struct list_node** list, star_selection_t* selection,
              int priority, int radius, double rgb[], int show_name )
{
        struct list_node* list_aux;

        if( !*list )
        {
                if( ( *list = ( struct list_node* ) 
                      malloc( sizeof( struct list_node ) )) )
                {
                        (*list)->priority = priority;
                        (*list)->selection = selection;
                        (*list)->show_name = show_name;
                        if( radius )
                        {
                                (*list)->is_radius = 1;
                                (*list)->radius = radius;
                        }
                        else
                                (*list)->is_radius = 0;
                        if( rgb )
                        {
                                (*list)->is_rgb = 1;
                                (*list)->rgb[0] = rgb[0];
                                (*list)->rgb[1] = rgb[1];
                                (*list)->rgb[2] = rgb[2];
                        }
                        else
                                (*list)->is_rgb = 0;
                        (*list)->next = NULL;
                }
                else
                        return 0;
        }
        else
        {
                if( priority < (*list)->priority )
                {
                        list_aux = *list;
                        if( ( *list = ( struct list_node* ) 
                              malloc( sizeof( struct list_node ) )) )
                        {
                                (*list)->priority = priority;
                                (*list)->selection = selection;
                                (*list)->show_name = show_name;
                                if( radius )
                                {
                                        (*list)->is_radius = 1;
                                        (*list)->radius = radius;
                                }
                                else
                                        (*list)->is_radius = 0;
                                if( rgb )
                                {
                                        (*list)->is_rgb = 1;
                                        (*list)->rgb[0] = rgb[0];
                                        (*list)->rgb[1] = rgb[1];
                                        (*list)->rgb[2] = rgb[2];
                                }
                                else
                                        (*list)->is_rgb = 0;
                                (*list)->next = list_aux;
                        }
                        else
                        {
                                *list = list_aux;
                                return 0;
                        }
                }
                else
                        return list_add( &( (*list)->next ), selection, 
                                         priority, radius, rgb, show_name );
        }
        return 1;
}

void list_find( struct list_node* list, star_t* star, int* have_radius,
                int* radius, int* have_rgb, double rgb[], 
                int* show_name )
{
        if( list )
        {
                if( star_selection_match( list->selection, star ) )
                {
                        if( list->is_radius && !*have_radius )
                        {
                                *radius = list->radius;
                                *have_radius = 1;
                        }
                        if( list->is_rgb && !*have_rgb )
                        {
                                rgb[0] = list->rgb[0];
                                rgb[1] = list->rgb[1];
                                rgb[2] = list->rgb[2];
                                *have_rgb = 1;
                        }
                        if( list->show_name )
                                *show_name = 1;
                }
                if( !*have_rgb || !*have_radius || !*show_name )
                        list_find( list->next, star, have_radius, 
                                   radius, have_rgb, rgb, show_name );
        }
}

static void list_foreach( struct list_node* list, int min_priority, 
                          int max_priority,
                          void (*function)( star_selection_t* selection,
                                            int is_radius, int radius, 
                                            int is_rgb, double rgb[],
                                            int show_name, void *data ),
                          void *data )
{
        if( list )
        {
                if( list->priority >= min_priority )
                {
                        if( list->priority <= max_priority )
                        {
                                function( list->selection, list->is_radius,
                                          list->radius, list->is_rgb,
                                          list->rgb, list->show_name, data );
                                list_foreach( list->next, min_priority, 
                                              max_priority, function, data );
                        }
                }
                else
                        list_foreach( list->next, min_priority, max_priority,
                                      function, data );
        }       
}

static void list_destroy( struct list_node* list )
{
        if( list )
        {
                list_destroy(  list->next );
                free( list );
        }
}

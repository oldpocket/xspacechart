/***********************************************************************
 * link_draw_rules.c: Implementation of the list of rules for drawing 
 *                   the links.
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
#include <float.h>
#include <gdk/gdk.h>
#include "../include/starmap.h"
#include "../include/link.h"
#include "../include/link_selection.h"
#include "../include/link_draw_rules.h"

struct list_node
{
        link_selection_t* selection;
        int width;
        GdkLineStyle style;
        double rgb[3];
};

struct st_link_drawing_rules
{
        GdkLineStyle default_style;
        double default_color[3];
        int default_width;
        struct list_node** list;
};

/* Definition of local functions */

static int rule_list_add( struct list_node** list[], 
                          link_selection_t* selection, int width, 
                          GdkLineStyle style, double rgb[] );

static void rule_list_find( struct list_node* list[], link_t *link, 
                            int* width, GdkLineStyle* style, double rgb[] );

static void rule_list_destroy( struct list_node* list[] );

static int rule_list_length( struct list_node* list[] );

static void rule_list_foreach( struct list_node* list[], 
                               void (*function)( link_selection_t *selection,
                                                 int width,
                                                 GdkLineStyle style,
                                                 double rgb[],
                                                 void* data ),
                               void* data );

/* Public Functions */

link_drawing_rules_t* link_drawing_rules_new( int default_width, 
                                              GdkLineStyle default_style,
                                              double default_rgb[] )
{
        link_drawing_rules_t* rules;
        
        if( ( rules = (link_drawing_rules_t*) 
              malloc( sizeof( link_drawing_rules_t ) ) ) &&
            ( rules->list = (struct list_node**) 
              malloc( sizeof(struct list_node*) ) ) )
        {
                rules->default_width = default_width;
                rules->default_style = default_style;
                rules->default_color[0] = default_rgb[0];
                rules->default_color[1] = default_rgb[1];
                rules->default_color[2] = default_rgb[2];
                rules->list[0] = NULL;
        }
        
        return rules;
}

void link_drawing_rules_get_default( link_drawing_rules_t* rules, int *width, 
                                     GdkLineStyle *style, double rgb[] )
{
        *width = rules->default_width;
        *style = rules->default_style;
        rgb[0] = rules->default_color[0];
        rgb[1] = rules->default_color[1];
        rgb[2] = rules->default_color[2];
}

void link_drawing_rules_find( link_drawing_rules_t* rules, 
                              link_t *link, int* width, 
                              GdkLineStyle* style, double rgb[] )
{
        *width = rules->default_width;
        *style = rules->default_style;
        rgb[0] = rules->default_color[0];
        rgb[1] = rules->default_color[1];
        rgb[2] = rules->default_color[2];
        rule_list_find( rules->list, link, width, style, rgb );
}

int link_drawing_rules_add( link_drawing_rules_t* rules, 
                            link_selection_t* selection, int width, 
                            GdkLineStyle style, double rgb[] )
{
        return rule_list_add( &(rules->list), selection, width, style, rgb );
}

void link_drawing_rules_foreach( link_drawing_rules_t* rules, 
                                 void (*function)( link_selection_t* selection,
                                                   int width,
                                                   GdkLineStyle style,
                                                   double rgb[],
                                                   void* data ),
                                 void* data )
{
        rule_list_foreach( rules->list, function, data );
}

void link_drawing_rules_destroy( link_drawing_rules_t* rules )
{
        rule_list_destroy( rules->list );
        free( rules );
}

/* Local Functions */

int rule_list_add( struct list_node** list[], link_selection_t* selection, 
                   int width, GdkLineStyle style, double rgb[] )
{
        struct list_node** aux;
        struct list_node* new_node;
        int length;

        if( ( new_node = (struct list_node*) 
              malloc( sizeof( struct list_node ) ) ) )
        {
                new_node->width = width;
                new_node->style = style;
                new_node->rgb[0] = rgb[0];
                new_node->rgb[1] = rgb[1];
                new_node->rgb[2] = rgb[2];
                new_node->selection = selection;
                length = rule_list_length( *list );
                if( ( aux = realloc( *list,(length+2) * 
                                     sizeof( struct list_node* ) ) ) )
                {
                        *list = aux;
                        (*list)[length] = new_node;
                        (*list)[length+1] = NULL;
                }
                else
                {
                        free( new_node );
                        return 0;
                }
                return 1;
        }
        else
                return 0;
}

void rule_list_find( struct list_node* list[], link_t *link, 
                     int* width, GdkLineStyle* style, double rgb[] )
{
        register int matches = 0;
        register struct list_node** list_aux;

        for( list_aux=list; *list_aux && !matches; list_aux++ )
        {
                matches = link_selection_match( (*list_aux)->selection, link );
                if( matches )
                {
                        if( width )
                                *width = (*list_aux)->width;
                        if( style )
                                *style = (*list_aux)->style;
                        if( rgb )
                        {
                                rgb[0] = (*list_aux)->rgb[0];
                                rgb[1] = (*list_aux)->rgb[1];
                                rgb[2] = (*list_aux)->rgb[2];
                        }
                }
        }
}

void rule_list_destroy( struct list_node* list[] )
{
        int i;
        for( i = 0; list[i]; i++ )
        {
                link_selection_destroy( list[i]->selection );
                free( list[i] );
        }
        free( list );
}

int rule_list_length( struct list_node* list[] )
{
        register int i;

        for( i = 0; list[i]; i++ )
                ;

        return i;
}

void rule_list_foreach( struct list_node* list[], 
                               void (*function)( link_selection_t *selection,
                                                 int width,
                                                 GdkLineStyle style,
                                                 double rgb[],
                                                 void* data ),
                               void* data )
{
        int i;
        for( i = 0; list[i]; i++ )
        {
                function( list[i]->selection, list[i]->width, 
                          list[i]->style, list[i]->rgb, data );
        }
}

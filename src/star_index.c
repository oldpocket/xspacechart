/***********************************************************************
 * star_index.c : Implementation of the index of stars. Implemented as
 *               a binary tree.
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
#include <math.h>
#include "../include/starmap.h"
#include "../include/star.h"
#include "../include/star_index.h"
#include "../include/perspective.h"

struct index_node 
{
        star_t *star;
        double distance;
        double x;
        double y;
        struct index_node* left;
        struct index_node* right;
};

struct st_star_index
{
        struct index_node* tree;
};

/* Declaration of local functions */

static void index_tree_destroy( struct index_node* tree );
static int index_tree_add( struct index_node** tree, double x, double y, 
                           double distance, star_t *star );
static void index_tree_foreach( struct index_node* tree,
                                void (*function)(star_t* star, double x, 
                                                 double y, void* data),
                                void* data );
static star_t* index_tree_find_first( struct index_node* tree,
                                      int (*function)(star_t* star, 
                                                      double x, 
                                                      double y, 
                                                      void* data ), void* data,
                                      star_index_find_start_t type );

/* Public functions */

star_index_t* star_index_new( perspective_t* persp, star_t *star_list[] )
{
        star_index_t* index;
        double distance, x, y;
        coords_3d_t from_point, star_coords;
        int i;

        perspective_get_from( persp, &from_point );
        if( (index = (star_index_t*) malloc( sizeof(struct st_star_index) ) ) )
        {
                index->tree = NULL;
                for( i = 0; star_list[i]; i++ )
                {
                        star_get_coords(  star_list[i],
                                          &star_coords );
                        distance = distance_3d( &star_coords, 
                                                &from_point );
                        perspective_compute( persp, &star_coords, &x, &y );
                        if( !index_tree_add( &index->tree, x, y, 
                                       distance, star_list[i] ) )
                        {
                                star_index_destroy( index );
                                return NULL;
                        }       
                }
        }

        return index;
}

void star_index_foreach( star_index_t* index, void (*function)(star_t* star, 
                                                               double x, 
                                                               double y, 
                                                               void* data ),
                         void* data )
{
        index_tree_foreach( index->tree, function, data );
}

star_t* star_index_find_first( star_index_t* index, 
                               int (*function)(star_t* star, 
                                               double x, 
                                               double y, 
                                               void* data ),
                               void* data, star_index_find_start_t type )
{
        return index_tree_find_first( index->tree, function, data, type );
}

void star_index_destroy( star_index_t* index )
{
        index_tree_destroy( index->tree );
        free( index );
}

/* Local functions */

void index_tree_destroy( struct index_node* tree )
{
        if( tree )
        {
                index_tree_destroy( tree->left );
                index_tree_destroy( tree->right );
                free( tree );
        }
}

int index_tree_add( struct index_node** tree, double x, double y, 
                    double distance, star_t *star )
{
        if( !(*tree) )
        {
                if( !(*tree = (struct index_node*) malloc( 
                        sizeof(struct index_node) ) ) )
                        return 0;
                (*tree)->star = star;
                (*tree)->x = x;
                (*tree)->y = y;
                (*tree)->distance = distance;
                (*tree)->left = NULL;
                (*tree)->right = NULL;
        }
        else
        {
                if( distance < (*tree)->distance )
                        return index_tree_add( &( (*tree)->left ), x, y, 
                                         distance, star );
                else
                        /* It could be that two stars are the same distance 
                         * away from the ref_point. Then we place them in
                         * order of appearance */
                        return index_tree_add( &( (*tree)->right ), x, y, 
                                         distance, star );
        }
        return 1;
}

void index_tree_foreach( struct index_node* tree,
                                void (*function)(star_t* star, double x, 
                                                 double y, void* data),
                                void* data )
{
        if( tree )
        {
                index_tree_foreach( tree->right, function, data );
                function( tree->star, tree->x, tree->y, data );
                index_tree_foreach( tree->left, function, data );
        }
}

star_t* index_tree_find_first( struct index_node* tree,
                                      int (*function)(star_t* star, 
                                                      double x, 
                                                      double y, 
                                                      void* data ), void* data,
                                      star_index_find_start_t type )
{
        star_t *star;
        
        if( tree )
        {
                if( type == STAR_INDEX_START_BEGIN )
                {
                        star = index_tree_find_first( tree->left, function, 
                                                      data, type );
                        if( !star )
                        {
                                star = tree->star;
                                if( !function( star, tree->x, tree->y, data ) )
                                {
                                        star = index_tree_find_first( 
                                                tree->right, function, data, 
                                                type );
                                }
                        }
                }
                else
                {
                       star = index_tree_find_first( tree->right, function, 
                                                      data, type );
                        if( !star )
                        {
                                star = tree->star;
                                if( !function( star, tree->x, tree->y, data ) )
                                {
                                        star = index_tree_find_first( 
                                                tree->left, function, data, 
                                                type );
                                }
                        }
                }
                return star;
        }
        else
                return NULL;
}


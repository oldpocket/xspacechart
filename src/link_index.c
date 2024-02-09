/***********************************************************************
 * link_index.c : Implementation of the index of links. Implemented as
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
#include "../include/link.h"
#include "../include/link_index.h"
#include "../include/perspective.h"

struct index_node 
{
        link_t *link;
        double distance;
        double x1, y1;
        double x2, y2;
        struct index_node* left;
        struct index_node* right;
};

struct st_link_index
{
        struct index_node* tree;
};

/* Declaration of local functions */

static void index_tree_destroy( struct index_node* tree );
static int index_tree_add(struct index_node** tree, double x1, double y1, 
                          double x2, double y2, double distance, link_t *link);
static void index_tree_foreach( struct index_node* tree,
                                void (*function)(link_t* link, 
                                                 double x1, double y1,
                                                 double x2, double y2, 
                                                 void* data ),
                                void* data );

/* Public functions */

link_index_t* link_index_new( perspective_t* persp, link_t *link_list[] )
{
        link_index_t* index;
        double distance, x1, y1, x2, y2;
        coords_3d_t from_point, coords;
        int i;
        star_t *star_a, *star_b;

        perspective_get_from( persp, &from_point );
        if( (index = (link_index_t*) malloc( sizeof(struct st_link_index) ) ) )
        {
                index->tree = NULL;
                for( i = 0; link_list[i]; i++ )
                {
                        link_get_middle(  link_list[i],
                                          &coords );
                        distance = distance_3d( &coords, 
                                                &from_point );
                        link_get_stars( link_list[i], &star_a, &star_b );
                        star_get_coords( star_a, &coords );
                        perspective_compute( persp, &coords, &x1, &y1 );
                        star_get_coords( star_b, &coords );
                        perspective_compute( persp, &coords, &x2, &y2 );
                        if( !index_tree_add( &index->tree, x1, y1, x2, y2, 
                                       distance, link_list[i] ) )
                        {
                                link_index_destroy( index );
                                return NULL;
                        }       
                }
        }

        return index;
}

void link_index_foreach( link_index_t* index, void (*function)(link_t* link, 
                                                               double x1, 
                                                               double y1,
                                                               double x2,
                                                               double y2, 
                                                               void* data ),
                         void* data )
{
        index_tree_foreach( index->tree, function, data );
}

void link_index_destroy( link_index_t* index )
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

int index_tree_add(struct index_node** tree, double x1, double y1, 
                          double x2, double y2, double distance, link_t *link)
{
        if( !(*tree) )
        {
                if( !(*tree = (struct index_node*) malloc( 
                        sizeof(struct index_node) ) ) )
                        return 0;
                (*tree)->link = link;
                (*tree)->x1 = x1;
                (*tree)->y1 = y1;
                (*tree)->x2 = x2;
                (*tree)->y2 = y2;
                (*tree)->distance = distance;
                (*tree)->left = NULL;
                (*tree)->right = NULL;
        }
        else
        {
                if( distance < (*tree)->distance )
                        return index_tree_add( &( (*tree)->left ), x1, y1,
                                               x2, y2, distance, link );
                else
                        /* It could be that two links are the same distance 
                         * away from the ref_point. Then we place them in
                         * order of appearance */
                        return index_tree_add( &( (*tree)->right ), x1, y1, 
                                               x2, y2, distance, link );
        }
        return 1;
}

void index_tree_foreach( struct index_node* tree,
                                void (*function)(link_t* link, 
                                                 double x1, double y1,
                                                 double x2, double y2, 
                                                 void* data ),
                                void* data )
{
        if( tree )
        {
                index_tree_foreach( tree->right, function, data );
                function( tree->link, tree->x1, tree->y1, 
                          tree->x2, tree->y2, data );
                index_tree_foreach( tree->left, function, data );
        }
}


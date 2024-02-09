/***********************************************************************
 * star_catalog.c : Implementation of the star catalog. It is a linked
 *                 list of stars. The links are generated as needed.
 * $Id: star_catalog.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
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
#include <libgen.h>
#include "../include/starmap.h"
#include "../include/star.h"
#include "../include/star_catalog.h"

struct star_node
{
        star_t *star;
        struct star_node *next;
};

struct st_star_catalog
{
        struct star_node *star_list;
        char *info;
        char *filename;
};

/* Local Functions */

static int star_list_add( struct star_node **list, star_t *star );
static void star_list_destroy( struct star_node *list );

/* Public Functions */

star_catalog_t* star_catalog_new( void )
{
        star_catalog_t* cat;
        if( (cat = ( star_catalog_t* ) malloc( sizeof( star_catalog_t ) )) )
        {
                cat->star_list = NULL;
                cat->info = NULL;
        }
        return cat;
}

char* star_catalog_info( star_catalog_t *catalog )
{
        return catalog->info;
}

char* star_catalog_filename( star_catalog_t *catalog )
{
        return catalog->filename;
}

int star_catalog_read( star_catalog_t* catalog, const char *file,
                       int (*report_progress)(float progress, void* data),
                       void *data )
{
        star_t* star;
        FILE* data_file = NULL;
        int error = 0;
        long pos = 0, size;
        char tmp[256]; /* Arbitrary */
        int length, old_length;

        /* First we read the list of stars. */
        if( !( data_file = fopen( file, "rt" ) ) )
                /* File not Found */
                error = 1;
        else
        {
                /* Find out the size of the file */
                fseek( data_file, 0, SEEK_END );
                size = ftell( data_file );
                fseek( data_file, 0, SEEK_SET );

                /* Before reading the stars we read the header. It is supposed
                 * to contain data the user should know */
                old_length = 0;
                while(  (fgets( tmp, 256, data_file )[0]) &&
                       tmp[0] != '%' )
                {
                        length = strlen(tmp);
                        if( !catalog->info )
                        {
                                catalog->info = malloc(length+1);
                                strncpy( catalog->info, tmp, length );
                        }
                        else
                        {
                                catalog->info = realloc(catalog->info,
                                                        length+old_length+1);
                                strncpy( catalog->info+old_length, tmp, 
                                         length );
                        }
                        old_length += length;
                        catalog->info[old_length] = '\0';
                }
                catalog->filename = basename(file);

                while( !error )
                {
                        star = star_new();
                        if( star_read( star, data_file ) )
                        {
                                if(!star_list_add(&(catalog->star_list), star))
                                {
                                        /* Not enough memory. Clean up and get 
                                         * out of here. */
                                        star_list_destroy(catalog->star_list);
                                        error = 3;
                                        break;
                                }
                        }
                        else
                        {
                                /* Couldn't read the next star: 
                                 * EOF or Corrupt file */
                                star_destroy(star);
                                break;
                        }
                        pos = ftell( data_file );
                        if(!report_progress( (float)pos/(float)size, data ))
                                return 4;
                }
                /* If we got out of the loop for an EOF, all is well, else.. */
                if( !feof( data_file ) )
                {
                        if( ferror( data_file ) )
                        {
                                /* Unexpected error. Report and return as file
                                 * Not found. */
                                error = 1;
                                perror( file );
                                star_list_destroy( catalog->star_list );
                        }
                        else
                        {
                                /* Format error in the file. */
                                error = 2;
                                star_list_destroy( catalog->star_list );
                        }
                }       
                fclose( data_file );
        }

        return error;
}

star_t* star_catalog_get( star_catalog_t* catalog, star_id_t id )
{
        struct star_node *list;

        for( list = catalog->star_list; list; list = list->next )
        {
                if( star_get_id( list->star ) == id )
                        break;
        }

        return list->star;
}

void star_catalog_foreach( star_catalog_t* catalog,
                           void (*func)( star_t *star, void *data ),
                           void *data )
{
        struct star_node *list;
        for( list = catalog->star_list; list; list = list->next )
                func( list->star, data );
}

void star_catalog_select_objs( star_catalog_t* catalog,
                               star_t *(*stars[]), link_t *(*links[]),
                               int (*star_sel)( star_t* star, void* data ),
                               int (*link_sel)( link_t* link, void* data ),
                               void *data )
{
        star_t **star_list = NULL, **s_backup;
        link_t **link_list = NULL, **l_backup;
        int list_length = 0, i, j;
        struct star_node *ref;
        link_t *link;
        
        if( !(star_list = (star_t**) malloc( sizeof(star_t*) )) )
        {
                *stars = NULL;
                return;
        }
        else
                star_list[0] = NULL;

        for( ref = catalog->star_list; ref; ref = ref->next )
        {
                if( star_sel( ref->star, data ) )
                {
                        s_backup = star_list;
                        if( !(star_list = (star_t**) realloc( star_list, 
                                                         (2+list_length) *
                                                         sizeof(star_t*) )) )
                        {
                                free( s_backup );
                                *stars = NULL;
                                return;
                        }
                        star_list[list_length] = ref->star;
                        list_length++;
                        star_list[list_length] = NULL;
                }
        }
        
        *stars = star_list;

        if( !links )
                return;

        list_length = 0;

        if( !(link_list = (link_t**) malloc( sizeof(link_t*) )) )
        {
                *links = NULL;
                return;
        }
        else
                link_list[0] = NULL;

        for( i = 0; star_list[i]; i++ )
        {
                for( j = i+1; star_list[j]; j++ )
                {
                        link = link_new( star_list[i],  star_list[j] );
                        if( link_sel( link, data ) )
                        {
                                l_backup = link_list;
                                if( !(link_list = (link_t**) 
                                      realloc( link_list, (2+list_length) * 
                                               sizeof(link_t*) )) )
                                {
                                        free( l_backup );
                                        *links = NULL;
                                        return;
                                }
                                link_list[list_length] = link;
                                list_length++;
                                link_list[list_length] = NULL;
                        }
                        else
                        {
                                link_destroy(link);
                        }
                }
        }
        
        *links = link_list;
}

void star_catalog_destroy( star_catalog_t* catalog )
{
        star_list_destroy( catalog->star_list );
        free( catalog );
}

/* Local Functions */
static int star_list_add( struct star_node **list, star_t *star )
{
        struct star_node *node;
        
        if( !*list )
        {
                if( (node = (struct star_node*) 
                     malloc( sizeof(struct star_node) )) )
                {
                        node->star = star;
                        node->next = NULL;
                        *list = node;
                        return 1;
                }
                else
                        return 0;
        }
        else
                return star_list_add( &((*list)->next), star );
}

static void star_list_destroy( struct star_node *list )
{
        if( list )
        {
                star_destroy( list->star );
                star_list_destroy( list->next );
                free( list );
        }
}


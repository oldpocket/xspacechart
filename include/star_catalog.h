/***********************************************************************
 * star_catalog.h: Definition of a catalogue of stars. It allows the user
 *                to select stars and links between them. 
 * $Id: star_catalog.h,v 1.1 2017/09/23 00:04:08 strauss Exp $
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
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

#ifndef _INCLUDE_STAR_CATALOG_H_
#define _INCLUDE_STAR_CATALOG_H_

#include <stdio.h>
#include "starmap.h"
#include "star.h"
#include "link.h"

typedef struct st_star_catalog star_catalog_t;

star_catalog_t* star_catalog_new( void );

/* Read the catalogue from a file.
 * Exit Codes: 0 - Success.
 *             1 - File Not Found.
 *             2 - File is not a Valid Catalogue File ( format error ).
 *             3 - Not enough memory to read Catalogue File.
 *             4 - User abort.
 */
int star_catalog_read( star_catalog_t* catalog, const char *file,
                       int (*report_progress)(float progress, void* data),
                       void *data );

/* Returns the notes about the catalogue */
char* star_catalog_info( star_catalog_t *catalog );

/* Returns the filename of the catalogue */
char* star_catalog_filename( star_catalog_t *catalog );

star_t* star_catalog_get( star_catalog_t* catalog, star_id_t id );

/* Calls  function "func" for everry star in the catalogue */
void star_catalog_foreach( star_catalog_t* catalog,
                           void (*func)( star_t *star, void *data ),
                           void *data );

/* Return the stars and links in the catalogue selected by the filters
 * give. It returns a NULL-terminated array of each. The star array can be
 * destroyed by free(), but each link must be destroyed before free()ing the
 * link array. Yes, this is not very orthogonal, but I can't think of an
 * efficient way to keep all the links in memory (for the Gliese data it
 * would require 80+ MB).
 */
void star_catalog_select_objs( star_catalog_t* catalog,
                               star_t *(*stars[]), link_t *(*links[]),
                               int (*star_sel)( star_t* star, void* data ),
                               int (*link_sel)( link_t* link, void* data ),
                               void *data );

void star_catalog_destroy( star_catalog_t* catalog );

#endif

/***********************************************************************
 * star_list_dialog.h : Definition of the dialog with the list of all stars
 *                      in the catalogue.
 * $Id: star_list_dialog.h,v 1.1 2017/09/23 00:04:09 strauss Exp $
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 1999 Free Software Foundation, Inc.
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

#ifndef _INCLUDE_STAR_LIST_DIALOG_H_
#define _INCLUDE_STAR_LIST_DIALOG_H_

#include "star_catalog.h"

typedef struct st_star_list_dialog star_list_dialog_t;

/* Create a new star list dialog. It receives the function to be called
 * when the user asks to center the star in display. */
star_list_dialog_t* star_list_dialog_new( void (*center)(star_t* star, 
                                                         void* data), 
                                          void* data );

/* Change the catalog the dialog uses */
void star_list_dialog_set_catalog( star_list_dialog_t *dialog,
                                   star_catalog_t *catalog );

/* Show the dialog */
void star_list_dialog_show( star_list_dialog_t *dialog );

GtkWidget *star_list_dialog_get_dialog( star_list_dialog_t *dialog );

/* Destroy the dialog */
void star_list_dialog_destroy( star_list_dialog_t *dialog );

#endif

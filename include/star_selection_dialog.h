/***********************************************************************
 * star_selection_dialog.h : Definition of a dialog that allow the user 
 *                           to create a star selection. 
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

#ifndef _INCLUDE_SELDIAL_H_
#define _INCLUDE_SELDIAL_H_

#include "starmap.h"
#include "star_selection.h"

typedef struct st_sel_dial selection_dialog_t;

/* Create a dialog, with the initial selection given, and the function to
 * be called when the selection changes, along with the data to be passed to
 * this function. */
selection_dialog_t* selection_dialog_new( star_selection_t* selection,
                                          void (*changed)( star_selection_t*,
                                                           void* ), 
                                          void* data );

/* Show the dialog. */
void selection_dialog_show( selection_dialog_t* dialog );

/* Destroy the dialog. */
void selection_dialog_destroy( selection_dialog_t* dialog );

#endif

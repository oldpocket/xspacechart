/***********************************************************************
 * gui.h : General module defining the Graphic User Interface. This is
 *        the real main module of the program. 
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

#ifndef _INCLUDE_GUI_H_
#define _INCLUDE_GUI_H_

#include "starmap.h"
#include "settings.h"
#include "star_catalog.h"

typedef struct st_gui gui_t;

gui_t* gui_new( settings_t *settings );
settings_t* gui_get_settings( gui_t *gui );
star_catalog_t* gui_get_catalog( gui_t *gui );
void gui_destroy( gui_t *gui );

#endif

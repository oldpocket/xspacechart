/***********************************************************************
 * key.h : Definition of auxiliary functions for key names in RC files.
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2002 Free Software Foundation, Inc.
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

#ifndef _INCLUDE_KEYS_H_
#define _INCLUDE_KEYS_H_

#include "starmap.h"

const char *get_keybinding_name( keybinding_t key );
keybinding_t get_keybinding( const char *name );

const char *get_mod_name( GdkModifierType mod );
GdkModifierType get_mod( const char *name );

const char *get_key_name( guint key );
guint get_key( const char *name );

#endif

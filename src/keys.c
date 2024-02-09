/***********************************************************************
 * key.c : Definition of auxiliary functions for key names in RC files.
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

#include <gtk/gtk.h>
#include "../include/starmap.h"
#include "../include/keys.h"

const struct 
{ 
        const char *name; 
        keybinding_t key; 
} keybindings[] = { {"MoveUp", KEY_MOVE_UP},
                    {"MoveDown", KEY_MOVE_DOWN},
                    {"MoveLeft", KEY_MOVE_LEFT},
                    {"MoveRight", KEY_MOVE_RIGHT},
                    {"MoveFront", KEY_MOVE_FRONT},
                    {"MoveRear", KEY_MOVE_REAR},
                    {"SaveSettings", KEY_SAVE_SETTINGS},
                    {"RotateUp", KEY_ROT_UP},
                    {"RotateDown", KEY_ROT_DOWN},
                    {"RotateLeft", KEY_ROT_LEFT},
                    {"RotateRight", KEY_ROT_RIGHT},
                    {"OpenSettings", KEY_SETTINGS},
                    {"OpenStarSelection", KEY_STAR_SEL},
                    {"ToggleLinks", KEY_TOGGLE_LINKS},
                    {"ToggleLinkLabels", KEY_TOGGLE_LINK_LABELS},
                    {"ToggleStarLabels", KEY_TOGGLE_STAR_LABELS},
                    {"ReloadSettings", KEY_RELOAD_SETTINGS} };

const struct 
{ 
        const char *name; 
        GdkModifierType mod; 
} mods[] = { {"Ctrl", GDK_CONTROL_MASK},
             {"Alt", GDK_MOD1_MASK},
             {"Shift", GDK_SHIFT_MASK} };

const struct 
{ 
        const char *name; 
        guint key; 
} keys[] = { 
        /*{"Escape", KEY_ESCAPE},
             {"F1", GDK_KEY_F1},
             {"F2", GDK_F2},
             {"F3", GDK_F3},
             {"F4", GDK_F4},
             {"F5", GDK_F5},
             {"F6", GDK_F6},
             {"F7", GDK_F7},
             {"F8", GDK_F8},
             {"F9", GDK_F9},
             {"F10", GDK_F10},
             {"F11", GDK_F11},
             {"F12", GDK_F12},
             {"Return", GDK_Return},
             {"PageUp", GDK_Page_Up},
             {"PageDown", GDK_Page_Down},
             {"Insert", GDK_Insert},
             {"Home", GDK_Home},
             {"End", GDK_End} */
        
};

const char *get_keybinding_name( keybinding_t key )
{
        int i;
        for( i = 0; i < sizeof(keybindings)/sizeof(keybindings[0]); i++ )
                if( keybindings[i].key == key )
                        return keybindings[i].name;
        return NULL;
}

keybinding_t get_keybinding( const char *name )
{
        int i;
        for( i = 0; i < sizeof(keybindings)/sizeof(keybindings[0]); i++ )
        {
                if( !strcmp(keybindings[i].name, name) )
                        return keybindings[i].key;
        }
        return -1;
}

const char *get_mod_name( GdkModifierType mod )
{
        int i;
        for( i = 0; i < sizeof(mods)/sizeof(mods[0]); i++ )
                if( mods[i].mod == mod )
                        return mods[i].name;
        return NULL;
}

GdkModifierType get_mod( const char *name )
{
        int i;
        for( i = 0; i < sizeof(mods)/sizeof(mods[0]); i++ )
                if( !strcmp(mods[i].name, name) )
                        return mods[i].mod;
        return -1;
}

const char *get_key_name( guint key )
{
        int i;
        for( i = 0; i < sizeof(keys)/sizeof(keys[0]); i++ )
                if( keys[i].key == key )
                        return keys[i].name;
        return NULL;
}

guint get_key( const char *name )
{
        int i;
        for( i = 0; i < sizeof(keys)/sizeof(keys[0]); i++ )
                if( !strcmp(keys[i].name, name) )
                        return keys[i].key;
        return -1;
}


        /***********************************************************************
 * main.c
 * $Id: main.c,v 1.1 2017/09/23 00:04:10 strauss Exp $
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
#include <math.h>
#include "../include/starmap.h"
#include "../include/gui.h"
#include "../include/settings.h"

const char *app_name = "GNU SpaceChart";
const char *app_version = "1.0";// VERSION;
const char *app_authors[] = { "Miguel Coca Barrionuevo "
                              "( mcoca@gnu.org )", NULL };
double default_color[3] = { 1.0, 0.0, 0.0 };

int main( int argc, char** argv ) 
{
        gui_t *gui;
        settings_t *settings;

        setbuf(stdout, NULL);           /* Unbuffered */
        setbuf(stdin, NULL);

     //   bindtextdomain(PACKAGE, LOCALEDIR);  
     //   textdomain(PACKAGE);

        gtk_init (&argc, &argv);
        
        if( !(settings = settings_new()) )
                exit(1);

        settings_disable_callbacks( settings );
        settings_load_from_files( settings );

        gui = gui_new(settings);

        if(!gui) exit(EXIT_FAILURE);

        gtk_init_add (settings_enable_callbacks, settings);

        /* Here we go... */
        gtk_main();

        gui_destroy( gui );

        return 0;
}

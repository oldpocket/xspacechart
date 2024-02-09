/***********************************************************************
 * star_info_dialog.c : Implementation of the star info dialog.
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
#include "../include/star_info_dialog.h"
#include "../include/star.h"
#include "../include/starmap.h"

struct st_star_info
{
        star_t *star;
        void (*center)(star_t* star, void* data);
        void* data;
        distance_unit_t unit;
        GtkWidget *dialog, *table;
        GtkWidget *notebook;
        GtkWidget *comp_frame;
        GtkWidget *cname_label, *dist_origin_label, *ra_label, *dec_label;
};

/* Definition of Local Functions */

static void button_close_press( GtkWidget* widget, star_info_t* info );
static void button_center_press( GtkWidget* widget, star_info_t* info );
static void display_component( star_component_t *comp, star_info_t *info );
static GtkWidget *component_page( star_component_t *comp );
static void write_star( star_info_t *info, star_t *star );

/* Public Functions */

star_info_t* star_info_new( void (*center)(star_t*, void* data), 
                            void* data )
{
        star_info_t* info;
        GtkWidget *label;
        GtkWidget *frame;
        GtkWidget *close_button, *center_button;

        if( ( info = (star_info_t*) malloc( sizeof( star_info_t ) ) ) )
        {
                info->star = NULL;
                info->data = data;
                info->center = center;
                info->dialog = gtk_dialog_new();
                frame = gtk_frame_new( _("Star Data") );
                gtk_container_border_width(GTK_CONTAINER(frame), 5);
                close_button = gtk_button_new_with_label( _("Close") );
                center_button = gtk_button_new_with_label( _("Center") );
                info->table = gtk_table_new( 5, 2, FALSE );
                info->notebook = NULL;
                info->comp_frame = gtk_frame_new( _("System Components") );
                gtk_container_border_width(GTK_CONTAINER(info->comp_frame), 5);

                label = gtk_label_new( _("Catalogue Number:") );
                info->cname_label = gtk_label_new( "" );
                gtk_table_attach( GTK_TABLE( info->table ),
                                  label, 0, 1, 0, 1, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );
                gtk_table_attach( GTK_TABLE( info->table ), 
                                  info->cname_label, 1, 2, 0, 1, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );

                label = gtk_label_new( _("Right Ascension:") );
                info->ra_label = gtk_label_new( "" );
                gtk_table_attach( GTK_TABLE( info->table ),
                                  label, 0, 1, 1, 2, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );
                gtk_table_attach( GTK_TABLE( info->table ), 
                                  info->ra_label, 1, 2, 1, 2, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );

                label = gtk_label_new( _("Declination:") );
                info->dec_label = gtk_label_new( "" );
                gtk_table_attach( GTK_TABLE( info->table ),
                                  label, 0, 1, 2, 3, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );
                gtk_table_attach( GTK_TABLE( info->table ), 
                                  info->dec_label, 1, 2, 2, 3, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );

                label = gtk_label_new( _("Distance to the Sun:") );
                info->dist_origin_label = gtk_label_new( "" );
                gtk_table_attach( GTK_TABLE( info->table ),
                                  label, 0, 1, 3, 4, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );
                gtk_table_attach( GTK_TABLE( info->table ), 
                                  info->dist_origin_label, 1, 2, 3, 4, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );

                gtk_table_attach( GTK_TABLE( info->table ), 
                                  info->comp_frame, 0, 2, 4, 5, 
                                  GTK_FILL | GTK_EXPAND, 
                                  GTK_FILL | GTK_EXPAND, 5, 5 );
                
                gtk_signal_connect( GTK_OBJECT(close_button), "clicked",
                                    (GtkSignalFunc) button_close_press, 
                                    (gpointer) info );
                gtk_signal_connect( GTK_OBJECT(center_button), "clicked",
                                    (GtkSignalFunc) button_center_press, 
                                    (gpointer) info );
                gtk_signal_connect( GTK_OBJECT(info->dialog), 
                                    "delete-event",
                                    (GtkSignalFunc) gtk_widget_hide, 
                                    NULL );

                gtk_container_add( GTK_CONTAINER(frame), info->table );
                gtk_box_pack_start_defaults( 
                        GTK_BOX(GTK_DIALOG(info->dialog)->vbox), frame );
                gtk_box_pack_start_defaults(
                        GTK_BOX(GTK_DIALOG(info->dialog)->action_area),
                        center_button );
                gtk_box_pack_start_defaults(
                        GTK_BOX(GTK_DIALOG(info->dialog)->action_area),
                        close_button );
                
                gtk_widget_show_all( frame );
        }

        return info;
}

void star_info_set_star( star_info_t* info, star_t* star )
{
        char title[50];
        char name[20];
        write_star( info, star );
        star_get_short_name( star, name );
        sprintf( title, _("Star Data: %s"), name );
        
        gtk_window_set_title( GTK_WINDOW( info->dialog), title );

        gtk_widget_show_all( info->dialog );
        gdk_window_raise( info->dialog->window );
}

void star_info_set_distance_units( star_info_t* info, distance_unit_t unit )
{
        info->unit = unit;
        /* Redraw the dialog with the new unit. */
        if( info->star )
                write_star( info, info->star );
}

void star_info_hide( star_info_t* info )
{
        gtk_widget_hide( GTK_WIDGET( info->dialog ) );
}

GtkWidget *star_info_get_dialog( star_info_t* info )
{
        return info->dialog;
}

void star_info_destroy(  star_info_t* info )
{
        gtk_widget_destroy( GTK_WIDGET( info->dialog ) );
        free( info );
}

/* Local Functions */

void button_close_press( GtkWidget* widget, star_info_t* info )
{
        star_info_hide( info );
}

void button_center_press( GtkWidget* widget, star_info_t* info )
{
        info->center( info->star, info->data );
}

void display_component( star_component_t *comp, star_info_t *info )
{
        GtkWidget *label;
        char label_title[2];
        
        sprintf( label_title, "%c", star_component_get_component(comp) );
        label = gtk_label_new( label_title );
        gtk_notebook_append_page( GTK_NOTEBOOK(info->notebook), 
                                  component_page( comp ), label );
}

GtkWidget *component_page( star_component_t *comp )
{
        GtkWidget *table, *label;
        char tmp[50];

        table = gtk_table_new( 5, 2, FALSE );

        label = gtk_label_new( _("Proper Name:") );
        gtk_table_attach( GTK_TABLE( table ), label, 0, 1, 0, 1, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        label = gtk_label_new( _("Bayer-Flamsteed:") );
        gtk_table_attach( GTK_TABLE( table ), label, 0, 1, 1, 2, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        label = gtk_label_new( _("DM:") );
        gtk_table_attach( GTK_TABLE( table ), label, 0, 1, 2, 3, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        label = gtk_label_new( _("Spectral Class:") );
        gtk_table_attach( GTK_TABLE( table ), label, 0, 1, 3, 4, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        label = gtk_label_new( _("Luminosity:") );
        gtk_table_attach( GTK_TABLE( table ), label, 0, 1, 4, 5, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        
        star_component_get_proper_name( comp, tmp );
        label = gtk_label_new( tmp );
        gtk_table_attach( GTK_TABLE( table ), label, 1, 2, 0, 1, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        star_component_get_bayer_name( comp, tmp );
        label = gtk_label_new( tmp );
        gtk_table_attach( GTK_TABLE( table ), label, 1, 2, 1, 2, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        star_component_get_dm_name( comp, tmp );
        label = gtk_label_new( tmp );
        gtk_table_attach( GTK_TABLE( table ), label, 1, 2, 2, 3, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        star_component_get_spectrum( comp, tmp );
        label = gtk_label_new( tmp );
        gtk_table_attach( GTK_TABLE( table ), label, 1, 2, 3, 4, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );
        sprintf( tmp, "%f", star_component_get_luminosity( comp ) );
        label = gtk_label_new( tmp );
        gtk_table_attach( GTK_TABLE( table ), label, 1, 2, 4, 5, 
                          GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 5, 5 );

        return table;
}

void write_star( star_info_t *info, star_t *star )
{
        char tmp[50];
        double distance, dec_deg, dec_min, ra_hours, ra_min, ra_sec;
        coords_3d_t origin = { 0, 0, 0};
        coords_3d_t coords;

        info->star = star;
        star_get_catalog_name( star, tmp );
        gtk_label_set_text( GTK_LABEL( info->cname_label ), tmp );
        
        star_get_coords( star, &coords );
        distance = distance_3d( &coords, &origin );

        if( info->unit != DISTANCE_PARSECS )
                distance = distance * PARSEC_TO_LY;
        sprintf( tmp, "%8.3f %s", distance, info->unit == DISTANCE_PARSECS ? 
                 _("parsecs") : _("light years") );
        gtk_label_set_text( GTK_LABEL( info->dist_origin_label ), tmp );

        star_get_dec( star, &dec_deg, &dec_min );
        star_get_ra( star, &ra_hours, &ra_min, &ra_sec );

        sprintf( tmp, "%i° %4.1f'", (int)dec_deg, dec_min );
        gtk_label_set_text( GTK_LABEL( info->dec_label ), tmp );

        sprintf( tmp, "%ih %im %4.1fs", (int)ra_hours, (int)ra_min, ra_sec );
        gtk_label_set_text( GTK_LABEL( info->ra_label ), tmp );

        if( info->notebook )
                gtk_widget_destroy( info->notebook );
        
        info->notebook = gtk_notebook_new();
        gtk_container_border_width(GTK_CONTAINER(info->notebook), 5);

        gtk_container_add( GTK_CONTAINER(info->comp_frame), info->notebook );

        star_foreach_component( star, (void(*)(star_component_t*,void*))
                                display_component, info );
}

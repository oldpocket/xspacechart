/***********************************************************************
 * config_dialog.c: The basic configuration dialog.
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
#include "../include/config_dialog.h"
#include "../include/link_config.h"
#include "../include/star_config.h"

struct st_config_dialog
{
        GtkWidget *dialog;
        GtkWidget *font_picker, *color_picker;
        settings_t *settings;
        link_config_t *link_config;
        star_config_t *star_config;
};

/* Declaration of private functions */

static void cb_changed_font( GtkWidget *widget, gchar* font_name, 
                             config_dialog_t *config );
static void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, 
                              guint a, config_dialog_t *config );
static void cb_changed_star_config( star_config_t *st_config, 
                                    config_dialog_t *config );
static void cb_changed_link_config( link_config_t *st_config, 
                                    config_dialog_t *config );
static void cb_apply( GtkWidget *box, int page_num, 
                      config_dialog_t *config );
static int cb_delete_event( GtkWidget* widget, config_dialog_t *config );

/* Public functions */

config_dialog_t* config_dialog_new( settings_t* settings )
{
        config_dialog_t *config;
        GtkWidget *label, *table, *frame;
        double rgb[3];

        if( (config = (config_dialog_t*) malloc( sizeof(config_dialog_t) )) )
        {
                config->settings = settings;
                config->dialog = gnome_property_box_new();
                gtk_signal_connect( GTK_OBJECT(config->dialog), "close",
                                    GTK_SIGNAL_FUNC(cb_delete_event), config );

                /* Create the labels page */
                frame = gtk_frame_new( _("Star and Link labels") );
                gtk_container_border_width(GTK_CONTAINER(frame),10);
                table = gtk_table_new( 2, 2, TRUE );
                gtk_container_add( GTK_CONTAINER(frame), table );
                gtk_container_border_width(GTK_CONTAINER(table), 5);
                gtk_table_set_row_spacings(GTK_TABLE(table), 5 );
                label = gtk_label_new( _("Label Font") );
                config->font_picker = gnome_font_picker_new();
                gnome_font_picker_set_font_name(
                        GNOME_FONT_PICKER(config->font_picker),
                        settings_get_labels_font(settings) );
                gtk_signal_connect( GTK_OBJECT(config->font_picker), 
                                    "font_set", cb_changed_font, config );
                gtk_table_attach( GTK_TABLE(table), label, 0, 1, 0, 1,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0 );
                gtk_table_attach( GTK_TABLE(table), config->font_picker, 
                                  1, 2, 0, 1,
                                  (GtkAttachOptions) 0, 
                                  (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( _("Label Color") );
                config->color_picker = gnome_color_picker_new();
                settings_get_labels_color( settings, rgb );
                gnome_color_picker_set_d( 
                        GNOME_COLOR_PICKER(config->color_picker), rgb[0],
                        rgb[1], rgb[2], 0 );
                gtk_signal_connect( GTK_OBJECT(config->color_picker), 
                                    "color_set", cb_changed_color, config );
                gtk_table_attach( GTK_TABLE(table), label, 0, 1, 1, 2,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0 );
                gtk_table_attach( GTK_TABLE(table), config->color_picker, 
                                  1, 2, 1, 2,
                                  (GtkAttachOptions) 0, 
                                  (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( _("Labels") );
                gnome_property_box_append_page( GNOME_PROPERTY_BOX(
                        config->dialog), frame, label );
                gtk_signal_connect( GTK_OBJECT(config->dialog), "apply",
                                    cb_apply, config );
                /* Create the Star Config page */
                config->star_config = star_config_new( 
                        (void(*)(star_config_t*,void *))cb_changed_star_config,
                        config );
                label = gtk_label_new( _("Star Drawing") );
                gnome_property_box_append_page( GNOME_PROPERTY_BOX(
                        config->dialog), star_config_get_widget(
                                config->star_config), label );
                star_config_set_rules(config->star_config, 
                                      settings_get_star_draw_rules(settings));
                /* Create the Link Config page */
                config->link_config = link_config_new( 
                        (void(*)(link_config_t*,void *))cb_changed_link_config,
                        config );
                label = gtk_label_new( _("Link Drawing") );
                gnome_property_box_append_page( GNOME_PROPERTY_BOX(
                        config->dialog), link_config_get_widget(
                                config->link_config), label );
                link_config_set_unit(config->link_config, 
                                     settings_get_distance_unit(config->settings));
                link_config_set_rules(config->link_config, 
                                      settings_get_link_draw_rules(settings));
                /* Draw the dialog */
                gtk_widget_show_all( config->dialog );
        }
        
        return config;
}

void config_dialog_destroy( config_dialog_t* config )
{
        star_config_destroy( config->star_config );
        free( config );
}

/* Private functions */

void cb_changed_font( GtkWidget *widget, gchar* font_name, 
                      config_dialog_t *config )
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(config->dialog) );
}

void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, guint a, 
                       config_dialog_t *config )
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(config->dialog) );
}

void cb_changed_star_config( star_config_t *st_config,config_dialog_t *config )
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(config->dialog) );
}

void cb_changed_link_config( link_config_t *st_config,config_dialog_t *config )
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(config->dialog) );
}

void cb_apply( GtkWidget *box, int page_num, config_dialog_t *config )
{
        double color[3];
        
        if( page_num == -1 )
        {
                settings_disable_callbacks( config->settings );
                settings_set_labels_font( 
                        config->settings, gnome_font_picker_get_font_name( 
                                GNOME_FONT_PICKER(config->font_picker)) );
                gnome_color_picker_get_d( GNOME_COLOR_PICKER(
                        config->color_picker), (color), (color+1), (color+2),
                                          NULL );
                settings_set_star_draw_rules( config->settings,
                                              star_config_get_rules(
                                                      config->star_config) );
                settings_set_link_draw_rules( config->settings,
                                              link_config_get_rules(
                                                      config->link_config) );
                settings_set_labels_color( config->settings, color );
                settings_enable_callbacks( config->settings );
        }
}

int cb_delete_event( GtkWidget* widget, config_dialog_t *config )
{
        config_dialog_destroy(config);
        return FALSE;
}

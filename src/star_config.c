/***********************************************************************
 * star_config.c : Implementation of a group of widgets that allow the user 
 *                to view and edit a star drawing rules list.
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

#include "../include/starmap.h"
#include "../include/star_config.h"
#include "../include/star_draw_rules.h"
#include "../include/star_selection_dialog.h"

struct st_star_config 
{
        star_selection_t *show_name;
        selection_dialog_t *show_name_dialog;
        void (*changed)( star_config_t *config, void *data );
        void *data;
        int is_set;
        GtkWidget *main_box;
        GtkWidget *O_color, *B_color, *A_color, *F_color,
                *G_color, *K_color, *M_color, *WD_color;
        GtkObject *adj_min_1, *adj_min_2, *adj_min_3, *adj_min_4;
};

/* Definition of local functions */
static GtkWidget* circle_pixmap( GtkWidget* widget, int radius );
static void cb_size_table_showed( GtkWidget *size_table );
static void cb_set_color( star_selection_t* selection, 
                          int is_radius, int radius, int is_rgb, double rgb[], 
                          int show_name, star_config_t *config );
static void cb_set_size( star_selection_t* selection, 
                         int is_radius, int radius, int is_rgb, double rgb[], 
                         int show_name, star_config_t *config );
static void cb_set_show_name( star_selection_t* selection, 
                              int is_radius, int radius, int is_rgb, 
                              double rgb[], int show_name, 
                              star_config_t *config );
static void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, 
                              guint a, star_config_t *config );
static void cb_changed_size( GtkAdjustment *adj, star_config_t* config );
static void cb_clicked_show_name( GtkWidget *button, star_config_t *config );
static void cb_changed_show_name( star_selection_t *selection, 
                                  star_config_t* config );

/* Public Functions */

star_config_t *star_config_new( void (*changed)( star_config_t *config,
                                                 void *data ), void *data )
{
        star_config_t *config;
        GtkWidget *gen_rules_frame;
        GtkWidget *gen_rules_box;
        GtkWidget *color_frame, *color_table, *size_frame, *size_table;
        GtkWidget *label;
        GtkWidget *spin_min_1, *spin_min_2, *spin_min_3, *spin_min_4;
        GtkWidget *names_button;

        if( (config = malloc(sizeof(star_config_t))) )
        {
                config->is_set = 0;
                config->changed = changed;
                config->data = data;
                config->main_box = gtk_hbox_new( FALSE, 10 );
                config->show_name = NULL;
                config->show_name_dialog = NULL;
                gtk_container_border_width(GTK_CONTAINER(config->main_box),10);
                gen_rules_frame = gtk_frame_new( _("General Rules") );
                gen_rules_box = gtk_table_new( 2, 2, FALSE );
                color_frame = gtk_frame_new( _("Color by Spectral Class") );
                gtk_container_border_width(GTK_CONTAINER(color_frame),5);
                color_table = gtk_table_new( 8, 2, FALSE );
                gtk_container_border_width(GTK_CONTAINER(color_table),10);
                gtk_table_set_row_spacings(GTK_TABLE(color_table), 5);
                gtk_table_set_col_spacings(GTK_TABLE(color_table), 5);
                gtk_container_add( GTK_CONTAINER(color_frame), color_table );
                size_frame = gtk_frame_new( _("Size by luminosity") );
                gtk_container_border_width(GTK_CONTAINER(size_frame),5);
                size_table = gtk_table_new( 6, 2, FALSE );
                gtk_container_border_width(GTK_CONTAINER(size_table),10);
                gtk_table_set_row_spacings(GTK_TABLE(size_table), 5);
                gtk_table_set_col_spacings(GTK_TABLE(size_table), 5);
                gtk_container_add( GTK_CONTAINER(size_frame), size_table );
                gtk_box_pack_start( GTK_BOX(config->main_box), gen_rules_frame,
                                    TRUE, TRUE, 0 );

                config->O_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table),
                                  config->O_color, 0, 1, 0, 1,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "O" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 0, 1,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->B_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->B_color, 0, 1, 1, 2,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "B" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 1, 2,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->A_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->A_color, 0, 1, 2, 3,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "A" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 2, 3,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->F_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->F_color, 0, 1, 3, 4,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "F" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 3, 4,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->G_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->G_color, 0, 1, 4, 5,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "G" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 4, 5,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->K_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->K_color, 0, 1, 5, 6,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "K" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 5, 6,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->M_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->M_color, 0, 1, 6, 7,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( "M" );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 6, 7,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                config->WD_color = gnome_color_picker_new();
                gtk_table_attach( GTK_TABLE(color_table), 
                                  config->WD_color, 0, 1, 7, 8,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                label = gtk_label_new( _("White Dwarfs") );
                gtk_table_attach( GTK_TABLE(color_table), 
                                  label, 1, 2, 7, 8,
                                  (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                                  0, 0 );
                gtk_signal_connect( GTK_OBJECT(config->O_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->B_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->A_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->F_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->G_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->K_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->M_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(config->WD_color), 
                                    "color_set", cb_changed_color, config );
                gtk_signal_connect( GTK_OBJECT(size_table), "show", 
                                    cb_size_table_showed, NULL );
                config->adj_min_1 = gtk_adjustment_new( 0, 0, 200, .1, 10, 0 );
                spin_min_1 = gtk_spin_button_new(
                        GTK_ADJUSTMENT(config->adj_min_1), 5, 1);
                gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                           spin_min_1, 1, 2, 1, 2 );
                gtk_signal_connect(GTK_OBJECT(config->adj_min_1), "value_changed", 
                                   GTK_SIGNAL_FUNC(cb_changed_size), config);
                config->adj_min_2 = gtk_adjustment_new( 0, 0, 200, .1, 10, 0 );
                spin_min_2 = gtk_spin_button_new(
                        GTK_ADJUSTMENT(config->adj_min_2), 5, 1);
                gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                           spin_min_2, 1, 2, 2, 3 );
                gtk_signal_connect(GTK_OBJECT(config->adj_min_2), "value_changed", 
                                   GTK_SIGNAL_FUNC(cb_changed_size), config);
                config->adj_min_3 = gtk_adjustment_new( 0, 0, 200, .1, 10, 0 );
                spin_min_3 = gtk_spin_button_new(
                        GTK_ADJUSTMENT(config->adj_min_3), 5, 1);
                gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                           spin_min_3, 1, 2, 3, 4 );
                gtk_signal_connect(GTK_OBJECT(config->adj_min_3), "value_changed", 
                                   GTK_SIGNAL_FUNC(cb_changed_size), config);
                config->adj_min_4 = gtk_adjustment_new( 0, 0, 200, .1, 10, 0 );
                spin_min_4 = gtk_spin_button_new(
                        GTK_ADJUSTMENT(config->adj_min_4), 5, 1);
                gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                           spin_min_4, 1, 2, 4, 5 );
                gtk_signal_connect(GTK_OBJECT(config->adj_min_4), "value_changed", 
                                   GTK_SIGNAL_FUNC(cb_changed_size), config);

                gtk_container_add( GTK_CONTAINER(gen_rules_frame), 
                                   gen_rules_box );
                names_button = gtk_button_new_with_label(_("Stars with label"));
                gtk_signal_connect( GTK_OBJECT(names_button), "clicked",
                                    cb_clicked_show_name, config );
                gtk_table_attach_defaults( GTK_TABLE(gen_rules_box), 
                                           names_button, 0, 1, 0, 1 );
                gtk_table_attach_defaults( GTK_TABLE(gen_rules_box), 
                                           color_frame, 0, 1, 1, 2);
                gtk_table_attach_defaults( GTK_TABLE(gen_rules_box), 
                                           size_frame, 1, 2, 1, 2);
        }
        
        return config;
}

void star_config_set_rules( star_config_t *config, 
                            star_drawing_rules_t* rules )
{
        star_drawing_rules_foreach( rules, PRIORITY_SPECTRUM_O, 
                                    PRIORITY_SPECTRUM_WD, 
                                    (void (*)( star_selection_t*, int, int, 
                                               int, double*, int, 
                                               void* ))cb_set_color, config );
        star_drawing_rules_foreach( rules, PRIORITY_LUMINOSITY,
                                    PRIORITY_LUMINOSITY, 
                                    (void (*)( star_selection_t*, int, int, 
                                               int, double*, int, 
                                               void* ))cb_set_size, config );
        star_drawing_rules_foreach( rules, PRIORITY_SHOW_NAME,
                                    PRIORITY_SHOW_NAME, 
                                    (void (*)(star_selection_t*, int, int, 
                                              int, double*, int, 
                                              void*))cb_set_show_name,config );
        config->is_set = 1;
}

GtkWidget *star_config_get_widget( star_config_t *config )
{
        return config->main_box;
}

star_drawing_rules_t *star_config_get_rules( star_config_t *config )
{
        star_drawing_rules_t *rules;
        double color[3];
        star_selection_t *selection;
        
        rules = star_drawing_rules_new( DEFAULT_SIZE, default_color );

        /* The color settings. */
        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->O_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_O );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_O, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->B_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_B );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_B, 0, 
                                color, FALSE );
        
        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->A_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_A );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_A, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->F_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_F );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_F, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->G_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_G );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_G, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->K_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_K );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_K, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->M_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_M );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_M, 0, 
                                color, FALSE );

        gnome_color_picker_get_d( GNOME_COLOR_PICKER(config->WD_color),
                                  (color), (color+1), (color+2), NULL );
        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_WHITE_DWARF );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_WD, 0,
                                color, FALSE );

        /* The size settings. */
        /* NOTE: The rules must be inserted in this order, so that in the case
           of multiple stars, if different components match different rules,
           the largest one will have preference. */

        selection = star_selection_new();
        star_selection_act_min_lum( selection,
                                    GTK_ADJUSTMENT(config->adj_min_4)->value);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_4, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection,
                                    GTK_ADJUSTMENT(config->adj_min_3)->value);
        star_selection_act_max_lum( selection, 
                                    GTK_ADJUSTMENT(config->adj_min_4)->value);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_3, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection,
                                    GTK_ADJUSTMENT(config->adj_min_2)->value);
        star_selection_act_max_lum( selection,
                                    GTK_ADJUSTMENT(config->adj_min_3)->value);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_2, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, 
                                    GTK_ADJUSTMENT(config->adj_min_1)->value);
        star_selection_act_max_lum( selection,
                                    GTK_ADJUSTMENT(config->adj_min_2)->value);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_1, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_max_lum( selection, 
                                    GTK_ADJUSTMENT(config->adj_min_1)->value );
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_0, NULL, FALSE );

        /* The name settings. */
        selection = star_selection_new();
        star_selection_copy( config->show_name, selection );
        star_drawing_rules_add( rules, selection, PRIORITY_SHOW_NAME, 
                                0, NULL, TRUE );

        return rules;
}

void star_config_destroy( star_config_t *config )
{
        gtk_widget_destroy( config->main_box );
        free(config);
}

/* Local Functions */

GtkWidget* circle_pixmap( GtkWidget* widget, int radius )
{
        GdkPixmap* circle;
        GtkWidget* pixmap;
        int l = 20;

        gtk_widget_realize( widget );
        circle = gdk_pixmap_new( widget->window, l, l, -1);
        gdk_draw_rectangle( circle, widget->style->black_gc, 
                            TRUE, 0, 0, l, l );
        gdk_draw_arc ( circle,
                       widget->style->white_gc,
                       TRUE,
                       l/2 - radius, l/2 - radius,
                       radius*2, radius*2,
                       0*64, 360*64 );
        pixmap = gtk_pixmap_new( circle, NULL );
        gtk_widget_show( pixmap );

        return pixmap;
}

/* Used to build the size example pixmaps after the table is visible. */
void cb_size_table_showed( GtkWidget *size_table )
{
        gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                   circle_pixmap(size_table, SIZE_0), 
                                   0, 1, 0, 1 );
        gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                   circle_pixmap(size_table, SIZE_1), 
                                   0, 1, 1, 2 );
        gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                   circle_pixmap(size_table, SIZE_2), 
                                   0, 1, 2, 3 );
        gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                   circle_pixmap(size_table, SIZE_3), 
                                   0, 1, 3, 4 );
        gtk_table_attach_defaults( GTK_TABLE(size_table), 
                                   circle_pixmap(size_table, SIZE_4), 
                                   0, 1, 4, 5 );
}

void cb_set_color( star_selection_t* selection, 
                   int is_radius, int radius, int is_rgb, double rgb[], 
                   int show_name, star_config_t *config )
{
        if( is_rgb )
        {
                if( star_selection_is_showed( selection, SPECTRUM_O ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->O_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_B ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->B_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_A ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->A_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_F ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->F_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_G ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->G_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_K ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->K_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_M ) )
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->M_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
                if( star_selection_is_showed( selection, SPECTRUM_WHITE_DWARF))
                {
                        gnome_color_picker_set_d( GNOME_COLOR_PICKER(
                                config->WD_color), rgb[0], rgb[1], 
                                                  rgb[2], 0.0 );
                }
        }
        
}

void cb_set_size( star_selection_t* selection, 
                  int is_radius, int radius, int is_rgb, double rgb[], 
                  int show_name, star_config_t *config )
{
        if( is_radius )
        {
                if( radius == SIZE_1 )
                {
                        gtk_adjustment_set_value( 
                                GTK_ADJUSTMENT(config->adj_min_1), 
                                star_selection_get_min_lum( selection ) );
                }
                else if( radius == SIZE_2 )
                {
                        gtk_adjustment_set_value( 
                                GTK_ADJUSTMENT(config->adj_min_2), 
                                star_selection_get_min_lum( selection ) );
                }
                else if( radius == SIZE_3 )
                {
                        gtk_adjustment_set_value( 
                                GTK_ADJUSTMENT(config->adj_min_3), 
                                star_selection_get_min_lum( selection ) );
                }
                else if( radius == SIZE_4 )
                {
                        gtk_adjustment_set_value( 
                                GTK_ADJUSTMENT(config->adj_min_4), 
                                star_selection_get_min_lum( selection ) );
                }
        }
}

void cb_set_show_name( star_selection_t* selection, int is_radius, 
                       int radius, int is_rgb, double rgb[], int show_name, 
                       star_config_t *config )
{
        if(config->show_name_dialog)
                selection_dialog_destroy(config->show_name_dialog);
        config->show_name_dialog = selection_dialog_new( 
                selection, (void (*)( star_selection_t*, void* ))
                cb_changed_show_name, config );
        config->show_name = selection;
}

void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, 
                       guint a, star_config_t *config )
{
        if( config->is_set )
                config->changed( config, config->data );
}

void cb_changed_size( GtkAdjustment *adj, star_config_t* config ) 
{
        if( config->is_set )
                config->changed( config, config->data );
}

void cb_clicked_show_name( GtkWidget *button, star_config_t *config )
{
        selection_dialog_show(config->show_name_dialog);
}

void cb_changed_show_name( star_selection_t *selection, star_config_t* config )
{
        config->show_name = selection;
        if( config->is_set )
                config->changed( config, config->data );
}


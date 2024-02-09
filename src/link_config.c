/***********************************************************************
 * link_config.c : Implementation of a group of widgets that allow the user 
 *                to view and edit a link drawing rules list.
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
#include "../include/starmap.h"
#include "../include/link.h"
#include "../include/link_selection.h"
#include "../include/link_draw_rules.h"
#include "../include/link_config.h"

struct st_list_data
{
        int is_default;
        double max_length;
        int width;
        GdkLineStyle style;
        double rgb[3];
};

struct st_rule_dialog
{
        GtkWidget *dialog;
        GtkWidget *color_picker;
        GtkObject *width_adj;
        GtkObject *max_length_adj;
        GtkWidget *solid_button;
        GtkWidget *dashed_button;
};

struct st_link_conf
{
        link_drawing_rules_t *rules;
        GtkWidget *main_widget;
        GtkWidget *list;
        GtkWidget *edit_button;
        GtkWidget *del_button;
        distance_unit_t unit;
        void (*changed)( link_config_t *config, void *data );
        void *changed_data;
        struct st_rule_dialog *rule_dialog;
        int current_row;
};

/* Declaration of Private Functions */

static void cb_insert_rule( link_selection_t* selection, int width, 
                            GdkLineStyle style, double rgb[], 
                            link_config_t* config );
static GdkPixmap* make_rule_pixmap( GtkWidget *widget, int width, 
                                    GdkLineStyle style, double rgb[] );
static void cb_edit_rule( GtkWidget *button, link_config_t *config );
static void cb_add_rule( GtkWidget *button, link_config_t *config );
static void cb_delete_rule( GtkWidget *button, link_config_t *config );
static struct st_rule_dialog *create_rule_dialog( struct st_list_data *data,
                                                  link_config_t *config );
static void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, 
                       guint a, struct st_rule_dialog *dialog );
static void cb_changed_size(GtkAdjustment *adj, struct st_rule_dialog *dialog);
static void cb_toggled_style(GtkWidget *widget, struct st_rule_dialog *dialog);
static void cb_apply_edit( GtkWidget *box, int page_num,
                           link_config_t *config );
static void cb_apply_add( GtkWidget *box, int page_num,
                          link_config_t *config );
static void cb_select_row( GtkWidget *widget, gint row, gint column,
                           GdkEventButton *event, link_config_t *config );
static void cb_unselect_row( GtkWidget *widget, gint row, gint column,
                             GdkEventButton *event, link_config_t *config );


/* Public functions */

link_config_t* link_config_new( void (*changed)( link_config_t *config,
                                                 void *data ), void *data )
{
        link_config_t *config;
        GtkWidget *table, *add_button;
        char *titles[] = {_("Max Length"),_("Example")};

        if( (config = (link_config_t*) malloc(sizeof(link_config_t))) )
        {
                config->changed = changed;
                config->changed_data = data;
                config->rules = NULL;
                config->current_row = 0;
                table = gtk_table_new( 2, 3, FALSE );
                gtk_container_border_width(GTK_CONTAINER(table),5);
                gtk_table_set_row_spacings(GTK_TABLE(table), 5);
                gtk_table_set_col_spacings(GTK_TABLE(table), 5);
                config->main_widget = gtk_frame_new( _("Links by length") );
                gtk_container_border_width(GTK_CONTAINER(config->main_widget),10);
                config->list = gtk_clist_new_with_titles( 2, titles );
                gtk_clist_set_selection_mode( GTK_CLIST(config->list),
                                              GTK_SELECTION_SINGLE );
                gtk_clist_column_titles_passive( GTK_CLIST(config->list) );
                gtk_signal_connect( GTK_OBJECT(config->list), "select_row",
                                    cb_select_row, config );
                gtk_signal_connect( GTK_OBJECT(config->list), "unselect_row",
                                    cb_unselect_row, config );
                add_button = gtk_button_new_with_label(_("Add Rule"));
                gtk_signal_connect( GTK_OBJECT(add_button), "clicked",
                                    cb_add_rule, config );
                config->del_button=gtk_button_new_with_label(_("Delete Rule"));
                gtk_widget_set_sensitive( config->del_button, FALSE );
                gtk_signal_connect( GTK_OBJECT(config->del_button), "clicked",
                                    cb_delete_rule, config );
                config->edit_button=gtk_button_new_with_label(_("Edit Rule"));
                gtk_widget_set_sensitive( config->edit_button, FALSE );
                gtk_signal_connect( GTK_OBJECT(config->edit_button), "clicked",
                                    cb_edit_rule, config );
                gtk_container_add( GTK_CONTAINER(config->main_widget), table );
                gtk_table_attach_defaults( GTK_TABLE(table), config->list,
                                           0, 3, 0, 1 );
                gtk_table_attach( GTK_TABLE(table), add_button, 0, 1, 1, 2,
                                  (GtkAttachOptions) GTK_FILL | GTK_EXPAND, 
                                  (GtkAttachOptions) 0, 0, 0 );
                gtk_table_attach( GTK_TABLE(table), config->edit_button, 
                                  1, 2, 1, 2,
                                  (GtkAttachOptions) GTK_FILL | GTK_EXPAND, 
                                  (GtkAttachOptions) 0, 0, 0 );
                gtk_table_attach( GTK_TABLE(table), config->del_button, 
                                  2, 3, 1, 2,
                                  (GtkAttachOptions) GTK_FILL | GTK_EXPAND, 
                                  (GtkAttachOptions) 0, 0, 0 );
        }

        return config;
}

void link_config_set_rules( link_config_t* config,
                            link_drawing_rules_t* rules )
{
        int width;
        GdkLineStyle style;
        double rgb[3];
        link_selection_t *selection;

        config->rules = rules;
        /* First, the default rule. */
        link_drawing_rules_get_default( rules, &width, &style, rgb );
        selection = link_selection_new();
        cb_insert_rule( selection, width, style, rgb, config );
        /* Insert all the others. */
        link_drawing_rules_foreach( rules, 
                                    (void(*)( link_selection_t*,int,
                                              GdkLineStyle,double*,void*))
                                    cb_insert_rule, config );
}

void link_config_set_unit( link_config_t* config,
                           distance_unit_t unit )
{
        config->unit = unit;
}

GtkWidget* link_config_get_widget( link_config_t* config )
{
        return config->main_widget;
}

link_drawing_rules_t* link_config_get_rules(link_config_t* config)
{
        int row;
        struct st_list_data *data;
        link_drawing_rules_t *rules;
        link_selection_t *selection;
        double prev = 0;

        /* Find the default rule. 
         * FIXME: Is there a way to tell how many rows there are in the list?*/
        for( row = 0; !((struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list), row))->is_default; row++ )
                ;
        data = (struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list),row);
        rules = link_drawing_rules_new( data->width, data->style, data->rgb );
        for( row = 0; !((struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list), row))->is_default; row++ )
        {
                data = (struct st_list_data*)gtk_clist_get_row_data(
                        GTK_CLIST(config->list),row);
                selection = link_selection_new();
                link_selection_act_min_length( selection, prev );
                link_selection_act_max_length( selection, data->max_length );
                link_drawing_rules_add( rules, selection, data->width, 
                                        data->style, data->rgb );
                prev = data->max_length;
        }

        return rules;
}

void link_config_destroy( link_config_t* conf )
{
        free( conf );
}

/* Private Functions */

void cb_insert_rule( link_selection_t* selection, int width, 
                     GdkLineStyle style, double rgb[], link_config_t* config )
{
        int row;
        char *text[2];
        char max_length[10];
        struct st_list_data *data = g_malloc(sizeof(struct st_list_data));

        data->is_default = !(link_selection_is_max_length(selection) ||
                             link_selection_is_max_length(selection) );
        data->max_length = link_selection_get_max_length(selection);
        data->width = width;
        data->style = style;
        data->rgb[0] = rgb[0];
        data->rgb[1] = rgb[1];
        data->rgb[2] = rgb[2];
        sprintf( max_length, "%5.2f", config->unit==DISTANCE_PARSECS ? 
                 data->max_length : data->max_length * PARSEC_TO_LY );
        text[0] = max_length;
        text[1] = NULL;
        if( data->is_default )
        {
                text[0] = _("Infinite");
                row = gtk_clist_append( GTK_CLIST(config->list), text );
                gtk_clist_set_pixmap(GTK_CLIST(config->list), row, 1,
                                     make_rule_pixmap(config->list,width,
                                                      style,rgb), NULL);
                gtk_clist_set_row_data( GTK_CLIST(config->list), row, data );
        }
        else
        {
                for( row = 0; !((struct st_list_data*)gtk_clist_get_row_data(
                        GTK_CLIST(config->list), row))->is_default; row++ )
                        if( ((struct st_list_data*)gtk_clist_get_row_data(
                                GTK_CLIST(config->list), row))->max_length > 
                            data->max_length )
                                break;
                gtk_clist_insert( GTK_CLIST(config->list), row, text );
                gtk_clist_set_pixmap(GTK_CLIST(config->list), row, 1,
                                     make_rule_pixmap(config->list,width,
                                                      style,rgb), NULL);
                gtk_clist_set_row_data( GTK_CLIST(config->list), row, data );
        }
}

GdkPixmap* make_rule_pixmap( GtkWidget *widget, int width, GdkLineStyle style,
                             double rgb[] )
{
        GdkPixmap* pixmap;
        GdkGC* gc;
        GdkColor color;

        gtk_widget_realize( widget );
        gc = gdk_gc_new( widget->window );
        color.red = (guint16) ( rgb[0] *  65535.0 );
        color.green = (guint16) ( rgb[1] * 65535.0 );
        color.blue = (guint16) ( rgb[2] * 65535.0 );
        color.pixel = (gulong) color.red * 65536 + 
                color.green * 256 + color.blue;
        gdk_color_alloc( gtk_widget_get_colormap(widget), &color );
        gdk_gc_set_foreground( gc, &color );
        gdk_gc_set_line_attributes( gc, width, style, 
                                    GDK_CAP_BUTT, 
                                    GDK_JOIN_MITER );
        pixmap = gdk_pixmap_new( widget->window, 50, 20, -1);
        gdk_draw_rectangle( pixmap, widget->style->black_gc, 
                            TRUE, 0, 0, 50, 20 );
        gdk_draw_line( pixmap, gc, 6, 10, 44, 10 );
        gdk_gc_destroy( gc );

        return pixmap;
}

void cb_edit_rule( GtkWidget *button, link_config_t *config )
{
        struct st_list_data *data;

        data = (struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list), config->current_row);
        config->rule_dialog = create_rule_dialog( data, config );
        
        gtk_signal_connect( GTK_OBJECT(config->rule_dialog->dialog), "apply",
                            cb_apply_edit, config );
        gnome_dialog_run_and_close(GNOME_DIALOG(config->rule_dialog->dialog));
        
        free(config->rule_dialog);
}

void cb_add_rule( GtkWidget *button, link_config_t *config )
{
        struct st_list_data *data;

        data = (struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list), config->current_row);
        config->rule_dialog = create_rule_dialog( data, config );
        
        gtk_signal_connect( GTK_OBJECT(config->rule_dialog->dialog), "apply",
                            cb_apply_add, config );
        gnome_dialog_run_and_close(GNOME_DIALOG(config->rule_dialog->dialog));
        
        free(config->rule_dialog);
}

void cb_delete_rule( GtkWidget *button, link_config_t *config )
{
        gtk_clist_remove( GTK_CLIST(config->list), config->current_row );
        config->changed(config, config->changed_data);
}

struct st_rule_dialog *create_rule_dialog( struct st_list_data *data, 
                                           link_config_t *config )
{
        struct st_rule_dialog *dialog;
        GtkWidget *table, *label, *width_spin, *maxl_spin;

        if( !(dialog = malloc( sizeof(struct st_rule_dialog) )) )
                return NULL;
        dialog->dialog = gnome_property_box_new();
        /* Color */
        dialog->color_picker = gnome_color_picker_new();
        gnome_color_picker_set_d(GNOME_COLOR_PICKER(dialog->color_picker), 
                                 data->rgb[0], data->rgb[1], data->rgb[2],0.0);
        gtk_signal_connect( GTK_OBJECT(dialog->color_picker), 
                            "color_set", cb_changed_color, dialog );
        /* Width */
        dialog->width_adj = gtk_adjustment_new( data->width, 0, 5, 1, 1, 0.0);
        width_spin = gtk_spin_button_new( GTK_ADJUSTMENT(
                dialog->width_adj), 1, 0);
        gtk_signal_connect(GTK_OBJECT(dialog->width_adj), "value_changed",
                           GTK_SIGNAL_FUNC(cb_changed_size), dialog);
        /* Max Length */
        dialog->max_length_adj = gtk_adjustment_new( 
                config->unit==DISTANCE_PARSECS ?  data->max_length : 
                data->max_length * PARSEC_TO_LY, 0,100,0.1,1,0.0);
        maxl_spin = gtk_spin_button_new( GTK_ADJUSTMENT(
                dialog->max_length_adj), 0.01, 2);
        gtk_signal_connect(GTK_OBJECT(dialog->max_length_adj), "value_changed",
                           GTK_SIGNAL_FUNC(cb_changed_size), dialog);
        gtk_widget_set_sensitive(maxl_spin, !data->is_default );
        /* The style */
        dialog->solid_button = gtk_radio_button_new_with_label(NULL,_("Solid Line"));
        dialog->dashed_button = gtk_radio_button_new_with_label( 
                gtk_radio_button_group(GTK_RADIO_BUTTON(dialog->solid_button)),
                _("Dashed Line") );
        if( data->style == GDK_LINE_SOLID )
                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(
                        dialog->solid_button), TRUE);
        else
                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(
                        dialog->dashed_button), TRUE);
        gtk_signal_connect( GTK_OBJECT(dialog->solid_button), "toggled", 
                            cb_toggled_style, dialog );
        gtk_signal_connect( GTK_OBJECT(dialog->dashed_button), "toggled", 
                            cb_toggled_style, dialog );
        /* Pack */
        table = gtk_table_new( 4, 2, TRUE );
        gtk_container_border_width(GTK_CONTAINER(table),10);
        gtk_table_set_row_spacings(GTK_TABLE(table), 5);
        gtk_table_set_col_spacings(GTK_TABLE(table), 5);
        label = gtk_label_new( _("Edit Rule") );
        gnome_property_box_append_page( GNOME_PROPERTY_BOX(dialog->dialog),
                                        table, label );
        label = gtk_label_new( _("Link Max Length") );
        gtk_table_attach( GTK_TABLE(table), label, 0, 1, 0, 1,
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          0, 0 );
        gtk_table_attach( GTK_TABLE(table), maxl_spin, 1, 2, 0, 1,
                          (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                          0, 0 );
        label = gtk_label_new( _("Link Width") );
        gtk_table_attach( GTK_TABLE(table), label, 0, 1, 1, 2,
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          0, 0 );
        gtk_table_attach( GTK_TABLE(table), width_spin, 1, 2, 1, 2,
                          (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                          0, 0 );
        label = gtk_label_new( _("Link Color") );
        gtk_table_attach( GTK_TABLE(table), label, 0, 1, 2, 3,
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          0, 0 );
        gtk_table_attach( GTK_TABLE(table), dialog->color_picker, 1, 2, 2, 3,
                          (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                          0, 0 );
        gtk_table_attach( GTK_TABLE(table), dialog->solid_button, 0, 1, 3, 4,
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                          0, 0 );
        gtk_table_attach( GTK_TABLE(table), dialog->dashed_button, 1, 2, 3, 4,
                          (GtkAttachOptions) 0, (GtkAttachOptions) 0, 
                          0, 0 );
        gtk_widget_show_all( dialog->dialog );

        return dialog;
}

void cb_changed_color( GtkWidget *widget, guint r, guint g, guint b, 
                       guint a, struct st_rule_dialog *dialog )
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(dialog->dialog) );
}

void cb_changed_size( GtkAdjustment *adj, struct st_rule_dialog *dialog ) 
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(dialog->dialog) );
}

void cb_toggled_style(GtkWidget *widget, struct st_rule_dialog *dialog)
{
        gnome_property_box_changed( GNOME_PROPERTY_BOX(dialog->dialog) );
}

void cb_apply_edit( GtkWidget *box, int page_num,link_config_t *config )
{
        int width;
        double max_length;
        double rgb[3];
        GdkLineStyle style;
        link_selection_t *selection;
        int is_default;
        
        if( page_num != -1 )
                return;
        gtk_clist_freeze( GTK_CLIST(config->list) );
        is_default = ((struct st_list_data*)gtk_clist_get_row_data(
                GTK_CLIST(config->list), config->current_row))->is_default;
        gtk_clist_remove( GTK_CLIST(config->list), config->current_row );
        width = GTK_ADJUSTMENT(config->rule_dialog->width_adj)->value;
        max_length=GTK_ADJUSTMENT(config->rule_dialog->max_length_adj)->value;
        max_length = config->unit==DISTANCE_PARSECS ?  max_length : 
                max_length / PARSEC_TO_LY;
        gnome_color_picker_get_d( GNOME_COLOR_PICKER(
                config->rule_dialog->color_picker), rgb, rgb+1, rgb+2, NULL );
        if( gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(config->rule_dialog->solid_button)) )
                style = GDK_LINE_SOLID;
        else
                style = GDK_LINE_ON_OFF_DASH;
        selection = link_selection_new();
        if( !is_default )
                link_selection_act_max_length( selection, max_length );
        cb_insert_rule( selection, width, style, rgb, config );
        link_selection_destroy( selection );
        gtk_clist_thaw( GTK_CLIST(config->list) );
        config->changed(config, config->changed_data);
}

void cb_apply_add( GtkWidget *box, int page_num,link_config_t *config )
{
        int width;
        double max_length;
        double rgb[3];
        GdkLineStyle style;
        link_selection_t *selection;
        
        if( page_num != -1 )
                return;
        gtk_clist_freeze( GTK_CLIST(config->list) );
        width = GTK_ADJUSTMENT(config->rule_dialog->width_adj)->value;
        max_length=GTK_ADJUSTMENT(config->rule_dialog->max_length_adj)->value;
        max_length = config->unit==DISTANCE_PARSECS ?  max_length : 
                max_length / PARSEC_TO_LY;
        gnome_color_picker_get_d( GNOME_COLOR_PICKER(
                config->rule_dialog->color_picker), rgb, rgb+1, rgb+2, NULL );
        if( gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(config->rule_dialog->solid_button)) )
                style = GDK_LINE_SOLID;
        else
                style = GDK_LINE_ON_OFF_DASH;
        selection = link_selection_new();
        link_selection_act_max_length( selection, max_length );
        cb_insert_rule( selection, width, style, rgb, config );
        link_selection_destroy( selection );
        gtk_clist_thaw( GTK_CLIST(config->list) );
        config->changed(config, config->changed_data);
}

void cb_select_row( GtkWidget *widget, gint row, gint column,
                    GdkEventButton *event, link_config_t *config )
{
        config->current_row = row;
        gtk_widget_set_sensitive( config->del_button,
                                  !((struct st_list_data*)
                                    gtk_clist_get_row_data(GTK_CLIST(config->list), row))->is_default );
        gtk_widget_set_sensitive( config->edit_button, TRUE );
}

void cb_unselect_row( GtkWidget *widget, gint row, gint column,
                    GdkEventButton *event, link_config_t *config )
{
        gtk_widget_set_sensitive( config->del_button, FALSE );
        gtk_widget_set_sensitive( config->edit_button, FALSE );
}

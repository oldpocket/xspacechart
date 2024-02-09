/***********************************************************************
 * star_list_dialog.c : Implementation of the dialog with the list of all
 *                      stars in the catalogue.
 * $Id: star_list_dialog.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
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

#include "../include/star_list_dialog.h"

struct st_star_list_dialog
{
        GtkWidget *dialog;
        GtkWidget *clist;
        GtkWidget *center_button;
        star_catalog_t *catalog;
        void (*center)(star_t* star, void* data);
        void *center_data;
        gint selected;
};

struct add_component_data 
{
        star_t *star;
        star_list_dialog_t *dialog;
};

static void cb_append_star( star_t *star, star_list_dialog_t *dialog );
static void cb_close_clicked( GtkWidget* widget, star_list_dialog_t* dialog );
static void cb_center_clicked( GtkWidget* widget, star_list_dialog_t* dialog );
static void cb_append_component( star_component_t *comp, 
                                 struct add_component_data *data );
static void cb_select_component( GtkCList *clist, gint row, gint column,
                                 GdkEventButton *event,
                                 star_list_dialog_t *dialog );
static void cb_unselect_component( GtkCList *clist, gint row, gint column,
                                   GdkEventButton *event,
                                   star_list_dialog_t *dialog );

/* Public Functions */

/* Create a new star list dialog. It receives the function to be called
 * when the user asks to center the star in display. */
star_list_dialog_t* star_list_dialog_new( void (*center)(star_t* star, 
                                                         void* data), 
                                          void* data )
{
        star_list_dialog_t *dialog;
        GtkWidget *close_button;
        GtkWidget *scroll;
        gchar *titles[] = {
                _("C. Number"),
                _("Spectral C."),
                _("Lum."),
                _("Name"),
                _("Bayer-Flamsteed Name"),
                _("DM")  
        };
        int i;

        if( (dialog = malloc(sizeof(star_list_dialog_t))) )
        {
                dialog->center = center;
                dialog->center_data = data;
                dialog->dialog = gtk_dialog_new();
                gtk_window_set_title( GTK_WINDOW( dialog->dialog),
                                      _("Stars in catalogue"));
                dialog->catalog = NULL;
                /* The list */
                scroll = gtk_scrolled_window_new( NULL, NULL );
                gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(scroll),
                                                GTK_POLICY_AUTOMATIC,
                                                GTK_POLICY_AUTOMATIC );
                dialog->clist = gtk_clist_new_with_titles( 
                        sizeof(titles)/sizeof(titles[0]), titles );
                for( i = 0; i < sizeof(titles)/sizeof(titles[0]); i++ )
                        gtk_clist_set_column_auto_resize( 
                                GTK_CLIST(dialog->clist), i, TRUE );
                gtk_container_add( GTK_CONTAINER(scroll), dialog->clist );
                gtk_box_pack_start_defaults( 
                        GTK_BOX(GTK_DIALOG(dialog->dialog)->vbox), scroll );
                gtk_widget_set_usize( dialog->clist, 500, 300 );
                gtk_signal_connect( GTK_OBJECT(dialog->clist), 
                                    "select_row",
                                    (GtkSignalFunc) cb_select_component, 
                                    (gpointer) dialog );
                gtk_signal_connect( GTK_OBJECT(dialog->clist), 
                                    "unselect_row",
                                    (GtkSignalFunc) cb_unselect_component, 
                                    (gpointer) dialog );
                /* The buttons */
                close_button = gtk_button_new_with_label( _("Close") );
                dialog->center_button = gtk_button_new_with_label(_("Center"));
                gtk_signal_connect( GTK_OBJECT(close_button), "clicked",
                                    (GtkSignalFunc) cb_close_clicked, 
                                    (gpointer) dialog );
                gtk_signal_connect( GTK_OBJECT(dialog->center_button), "clicked",
                                    (GtkSignalFunc) cb_center_clicked,
                                    (gpointer) dialog );
                gtk_box_pack_start_defaults(
                        GTK_BOX(GTK_DIALOG(dialog->dialog)->action_area),
                        dialog->center_button );
                gtk_box_pack_start_defaults(
                        GTK_BOX(GTK_DIALOG(dialog->dialog)->action_area),
                        close_button );
                gtk_widget_set_sensitive( dialog->center_button, FALSE );
        }
        return dialog;
}

/* Change the catalog the dialog uses */
void star_list_dialog_set_catalog( star_list_dialog_t *dialog,
                                   star_catalog_t *catalog )
{
        dialog->catalog = catalog;
        gtk_clist_clear(GTK_CLIST(dialog->clist));
        star_catalog_foreach( catalog, 
                              (void(*)(star_t*,void*))cb_append_star, dialog );
}

/* Show the dialog */
void star_list_dialog_show( star_list_dialog_t *dialog )
{
        gtk_widget_show_all( dialog->dialog );
        gdk_window_raise( dialog->dialog->window );
}

GtkWidget *star_list_dialog_get_dialog( star_list_dialog_t *dialog )
{
        return dialog->dialog;
}

/* Destroy the dialog */
void star_list_dialog_destroy( star_list_dialog_t *dialog )
{
        gtk_widget_destroy( dialog->dialog );
        free( dialog );
}

/* Callbacks */

void cb_close_clicked( GtkWidget* widget, star_list_dialog_t* dialog )
{
        gtk_widget_hide( GTK_WIDGET( dialog->dialog ) );
}

void cb_center_clicked( GtkWidget* widget, star_list_dialog_t* dialog )
{
        dialog->center( (star_t*)
                        gtk_clist_get_row_data( GTK_CLIST(dialog->clist),
                                                dialog->selected ),
                        dialog->center_data );
}

void cb_append_component( star_component_t *comp, 
                          struct add_component_data *data )
{
        gchar name[50], cat_name[50], bayer_name[50], dm[50], spectral[20],
                luminosity[20];
        gchar *row[] = {cat_name, spectral, luminosity, name, bayer_name, dm};
        gint i;
        int l;
        star_get_catalog_name( data->star, cat_name );
        l = strlen( cat_name );
        cat_name[l] = star_component_get_component( comp );
        cat_name[l+1] = '\0';
        star_component_get_proper_name( comp, name );
        star_component_get_bayer_name( comp, bayer_name );
        star_component_get_dm_name( comp, dm );
        star_component_get_spectrum( comp, spectral );
        snprintf( luminosity, 50, "%6.2f", star_component_get_luminosity(comp) );
        i = gtk_clist_append( GTK_CLIST(data->dialog->clist), row );
        gtk_clist_set_row_data( GTK_CLIST(data->dialog->clist), 
                                i, data->star );
}

void cb_append_star( star_t *star, star_list_dialog_t *dialog )
{
        struct add_component_data data = {star, dialog};
        star_foreach_component( star, (void (*)(star_component_t*,void *))
                                cb_append_component,
                                &data );
}

void cb_select_component( GtkCList *clist, gint row, gint column,
                          GdkEventButton *event,
                          star_list_dialog_t *dialog )
{
        dialog->selected = row;
        gtk_widget_set_sensitive( dialog->center_button, TRUE );
}

void cb_unselect_component( GtkCList *clist, gint row, gint column,
                            GdkEventButton *event,
                            star_list_dialog_t *dialog )
{
        gtk_widget_set_sensitive( dialog->center_button, FALSE );
}

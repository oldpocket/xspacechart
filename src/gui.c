/***********************************************************************
 * gui.c: The user interface.
 * $Id: gui.c,v 1.1 2017/09/23 00:04:10 strauss Exp $
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
#include "../include/star.h"
#include "../include/link.h"
#include "../include/config_dialog.h"
#include "../include/settings.h"
#include "../include/map.h"
#include "../include/star_info_dialog.h"
#include "../include/star_catalog.h"
#include "../include/star_selection.h"
#include "../include/star_selection_dialog.h"
#include "../include/star_list_dialog.h"
#include "../include/gui.h"

/* The maximum value you can select in "View Radius" */
#define MAX_RADIUS 200

/* How many parsecs the map moves when a movement button is pressed. */
#define DISPLACEMENT 1.0
/* How many degrees the map rotates when a rotation command is given */
#define ROTATION 2.0

typedef enum
{
        MOVEMENT_UP,
        MOVEMENT_DOWN,
        MOVEMENT_LEFT,
        MOVEMENT_RIGHT,
        MOVEMENT_FRONT,
        MOVEMENT_REAR
} movement_t;

typedef enum
{
        ROTATION_UP,
        ROTATION_DOWN,
        ROTATION_LEFT,
        ROTATION_RIGHT
} rotation_t;

struct st_gui
{
        star_catalog_t *catalog;
        map_t *map;
        settings_t *settings;
        selection_dialog_t* star_selection_dialog;
        config_dialog_t* config_dialog;
        star_info_t* star_info_dialog;
        star_list_dialog_t *star_list_dialog;
        int changing_coords; /* Used to avoid infinite recursion between the
                                settings and the coords adjustments */
        /* Signal handles */
        gint sig_lat, sig_rah, sig_ram, sig_north;
        int load;
        GtkObject *view_adjustment;
        GtkObject *max_link;
        GtkWidget *radio_unit_ly, *radio_unit_pc;
        GtkWidget *about_box, *open_dialog;
        GtkWidget *app, *appbar, *menubar;
        GtkAccelGroup *accel;
        GtkWidget *toggle_links_menu, *toggle_link_labels_menu, 
                *toggle_star_labels_menu;
        GtkWidget *toggle_links_bar, *toggle_link_labels_bar, 
                *toggle_star_labels_bar;
        GtkObject *lat_adj, *rah_adj, *ram_adj, *north_adj;
        GtkWidget *about_data_menu;
        GtkWidget *loading_dialog;
};

/* Toolbar Pixmaps */

#include "../pixmaps/selection.xpm"
#include "../pixmaps/toggle_links.xpm"
#include "../pixmaps/toggle_link_labels.xpm"
#include "../pixmaps/toggle_star_labels.xpm"
#include "../pixmaps/config.xpm"
#include "../pixmaps/up.xpm"
#include "../pixmaps/down.xpm"
#include "../pixmaps/right.xpm"
#include "../pixmaps/left.xpm"
#include "../pixmaps/front.xpm"
#include "../pixmaps/rear.xpm"

/* Definition of local functions */

static void cb_about_clicked( GtkWidget *button, gui_t* gui );
static void cb_about_data_file_clicked( GtkWidget *button, gui_t* gui );
static void cb_open_clicked( GtkWidget *button, gui_t* gui );
static void cb_save_clicked( GtkWidget *button, gui_t* gui );
static void cb_close_clicked( GtkWidget *button, gui_t* gui );
static void cb_reload_clicked( GtkWidget *button, gui_t* gui );
static void cb_list_stars_clicked( GtkWidget *button, gui_t* gui );
static void cb_open_ok_clicked( GtkWidget *button, gui_t* gui );
static void cb_open_cancel_clicked( GtkWidget *button, gui_t* gui );
static void cb_mov_clicked( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_links( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_link_labels( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_star_labels( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_links_menu( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_link_labels_menu( GtkWidget* widget, gui_t* gui );
static void cb_toggled_show_star_labels_menu( GtkWidget* widget, gui_t* gui );
static void cb_star_selection_clicked( GtkWidget* widget, gui_t* gui );
static void cb_settings_clicked( GtkWidget* widget, gui_t* gui );
static void cb_star_clicked( star_t *star, gui_t *gui );
static void cb_center_star( star_t *star, gui_t *gui );
static void cb_settings_changed_shown_objects( settings_t *settings, gui_t *gui );
static void cb_settings_changed_unit( settings_t *settings, gui_t *gui );
static void cb_settings_changed_view_radius( settings_t *settings, gui_t *gui );
static void cb_settings_changed_max_link( settings_t *settings, gui_t *gui );
static void cb_settings_changed_keybindings( settings_t *settings, gui_t *gui );
static void cb_changed_radius( GtkAdjustment *adj, gui_t* gui );
static void cb_changed_max_link( GtkAdjustment *adj, gui_t* gui );
static void cb_changed_selection( star_selection_t* selection, gui_t* gui );
static void cb_toggled_unit( GtkWidget *widget, gui_t *gui );
static int is_visible_star( star_t *star, gui_t *gui );
static int is_visible_link( link_t *link, gui_t *gui );
static void load_catalog( gui_t *gui, const char *file );
static int cb_load_progress( float progress, gui_t *gui );
static void cb_settings_changed_toggle_buttons( settings_t *settings, gui_t *gui );
static void cb_settings_changed_sight_params( settings_t *settings, gui_t *gui );
static void cb_settings_changed_catalog_filename( settings_t *settings, gui_t *gui );
static void cb_changed_user_coords( GtkAdjustment *adj, gui_t* gui );
static void cb_cancel_load_clicked( GtkWidget *dialog, int button, 
                                    gui_t* gui );
static void cb_rotate_clicked( GtkWidget* widget, gui_t* gui );

/* Auxiliar GUI building functions */

/* Creates the "User Cooords" frame and returns it */
static GtkWidget *create_user_coords( gui_t *gui )
{
        GtkWidget *user_coords_frame, *user_coords_table, *lat_spin, 
                *rah_spin, *ram_spin, *north_spin, *label;

        gui->rah_adj = gtk_adjustment_new( 0, 0, 23, 1, 1, 0.0);
        gui->ram_adj = gtk_adjustment_new( 0, 0, 59, 1, 1, 0.0);
        rah_spin = gtk_spin_button_new( GTK_ADJUSTMENT(gui->rah_adj), 1, 0);
        ram_spin = gtk_spin_button_new( GTK_ADJUSTMENT(gui->ram_adj), 1, 0);
        gtk_spin_button_set_wrap( GTK_SPIN_BUTTON(rah_spin), TRUE );
        gtk_spin_button_set_wrap( GTK_SPIN_BUTTON(ram_spin), TRUE );
        gui->lat_adj = gtk_adjustment_new( 0, -90, 90, 1, 1, 0.0);
        lat_spin = gtk_spin_button_new( GTK_ADJUSTMENT(gui->lat_adj), 1, 0);
        gui->north_adj = gtk_adjustment_new( 0, -0, 359, 1, 1, 0.0);
        north_spin = gtk_spin_button_new(GTK_ADJUSTMENT(gui->north_adj), 1, 0);
        gtk_spin_button_set_wrap( GTK_SPIN_BUTTON(north_spin), TRUE );
        user_coords_table = gtk_table_new( 7, 4, FALSE );
        label = gtk_label_new( _("Dec:") );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), label, 
                                   0, 1, 0, 1 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), lat_spin, 
                                   1, 4, 0, 1 );
        label = gtk_label_new( _("RA:") );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), label, 
                                   0, 1, 1, 2 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), rah_spin, 
                                   1, 2, 1, 2 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), 
                                   gtk_label_new(":"), 
                                   2, 3, 1, 2 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), ram_spin, 
                                   3, 4, 1, 2 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table),
                                   gtk_hseparator_new(), 0, 4, 2, 3 );
        label = gtk_label_new( _("North:") );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), label, 
                                   0, 1, 3, 4 );
        gtk_table_attach_defaults( GTK_TABLE(user_coords_table), north_spin, 
                                   1, 4, 3, 4 );
        gui->sig_rah = gtk_signal_connect(GTK_OBJECT(gui->rah_adj), 
                                          "value_changed", 
                                          GTK_SIGNAL_FUNC(
                                                  cb_changed_user_coords), 
                                          gui);
        gui->sig_ram = gtk_signal_connect(GTK_OBJECT(gui->ram_adj), 
                                          "value_changed", 
                                          GTK_SIGNAL_FUNC(
                                                  cb_changed_user_coords), 
                                          gui);
        gui->sig_lat = gtk_signal_connect(GTK_OBJECT(gui->lat_adj), 
                                          "value_changed", 
                                          GTK_SIGNAL_FUNC(
                                                  cb_changed_user_coords), 
                                          gui);
        gui->sig_north = gtk_signal_connect(GTK_OBJECT(gui->north_adj),
                                            "value_changed", 
                                            GTK_SIGNAL_FUNC(
                                                    cb_changed_user_coords),
                                            gui);
        user_coords_frame = gtk_frame_new( _("User Coords.") );
        gtk_container_add(GTK_CONTAINER(user_coords_frame), user_coords_table);

        return user_coords_frame;
}

/* Creates the menubar for the main window (and reloads it if needed for 
 * reloading the keybindings) */
static void create_menus( gui_t *gui )
{
  
}

/* Creates the menubar for the main window */
static void create_toolbar( gui_t *gui )
{
    
}

static void create_main_area( gui_t *gui )
{
        GtkWidget *central_box, *view_radius, *radius_frame, 
                *max_link_spin, *max_link_frame, *left_box, *unit_frame,
                *unit_box, *user_coords_frame;

        central_box = gtk_hbox_new( FALSE, 0 );
        left_box = gtk_vbox_new( FALSE, 0 );
        /* The view radius frame */
        gui->view_adjustment = gtk_adjustment_new( 1.0, 1.0, MAX_RADIUS,
                                                   0.5, 2.0, 0.0 );
        view_radius = gtk_vscale_new( GTK_ADJUSTMENT( gui->view_adjustment ) );
        gtk_signal_connect(GTK_OBJECT(gui->view_adjustment), "value_changed", 
                           GTK_SIGNAL_FUNC(cb_changed_radius), gui);
        radius_frame = gtk_frame_new( _("View Radius") );
        gtk_container_add( GTK_CONTAINER( radius_frame ), view_radius );
        /* The Max Link frame */
        gui->max_link = gtk_adjustment_new( 0.0, 0, 100, .1, 1, 0.0);
        max_link_spin = gtk_spin_button_new( GTK_ADJUSTMENT(gui->max_link), 
                                             0.01, 3);
        gtk_signal_connect(GTK_OBJECT(gui->max_link), "value_changed", 
                           GTK_SIGNAL_FUNC(cb_changed_max_link), gui);
        max_link_frame = gtk_frame_new( _("Max Link Shown") );
        gtk_container_add( GTK_CONTAINER( max_link_frame ), max_link_spin );
        /* The distance units frame */
        unit_frame = gtk_frame_new( _("Units") );
        gui->radio_unit_ly = gtk_radio_button_new_with_label(NULL,_("Light Years"));
        gui->radio_unit_pc = gtk_radio_button_new_with_label( 
                gtk_radio_button_group( GTK_RADIO_BUTTON(gui->radio_unit_ly) ), 
                _("Parsecs") );

        unit_box = gtk_vbox_new( FALSE, 0 );
        gtk_box_pack_start( GTK_BOX(unit_box), gui->radio_unit_ly, FALSE, FALSE, 0);
        gtk_signal_connect( GTK_OBJECT(gui->radio_unit_ly), "toggled", 
                            cb_toggled_unit, gui );
        gtk_object_set_data( GTK_OBJECT(gui->radio_unit_ly), "unit", 
                             GINT_TO_POINTER(DISTANCE_PARSECS) );
        gtk_signal_connect( GTK_OBJECT(gui->radio_unit_pc), "toggled", 
                            cb_toggled_unit, gui );
        gtk_object_set_data( GTK_OBJECT(gui->radio_unit_pc), "unit", 
                             GINT_TO_POINTER(DISTANCE_LIGHT_YEARS) );
        gtk_box_pack_start( GTK_BOX(unit_box), gui->radio_unit_pc, FALSE, FALSE, 0);
        gtk_container_add( GTK_CONTAINER(unit_frame), unit_box );
        /* The user coordinates frame */
        user_coords_frame = create_user_coords(gui);
        /* Pack everything together */
        gnome_app_set_contents( GNOME_APP(gui->app), central_box );
        gnome_app_set_statusbar( GNOME_APP(gui->app), gui->appbar );
        gtk_box_pack_start( GTK_BOX(left_box), unit_frame, FALSE, FALSE, 5 );
        gtk_box_pack_start( GTK_BOX(left_box), max_link_frame, 
                            FALSE, FALSE, 5 );
        gtk_box_pack_start( GTK_BOX(left_box), radius_frame, TRUE, TRUE, 5 );
        gtk_box_pack_start( GTK_BOX(left_box), user_coords_frame, FALSE, 
                            FALSE, 5 );
        gtk_box_pack_start( GTK_BOX(central_box), left_box, FALSE, FALSE, 5 );
        gtk_box_pack_start( GTK_BOX( central_box ), map_get_widget( gui->map ),
                            TRUE, TRUE, 0 );
        gtk_widget_show_all( GTK_WIDGET(gui->app) );        
}

/* Public Functions */
gui_t* gui_new( settings_t *settings )
{
        /* Basic initialization */
        gui_t *gui = (gui_t*) malloc( sizeof(gui_t) );
        gui->settings = settings;
        if( !(gui) ||
            !(gui->settings || (gui->settings = settings_new())) ||
            !(gui->catalog = star_catalog_new()) ||
            !(gui->map = map_new(gui->settings)) ||
            !(gui->star_info_dialog = star_info_new(
                      (void(*)(star_t*,void*))cb_center_star,gui)) ||
            !(gui->star_list_dialog = star_list_dialog_new(
                      (void(*)(star_t*,void*))cb_center_star,gui)) ||
            !(gui->star_selection_dialog = 
              selection_dialog_new(settings_get_star_filter(gui->settings),
                                   (void(*)(star_selection_t*,void*))
                                   cb_changed_selection,gui)))
        {
                emit_error_message( _("Error: Not enough memory") );
                return NULL;
        }
        gui->changing_coords = 0;
        gui->loading_dialog = NULL;
        /* What to do if a star is clicked */
        map_set_star_clicked( gui->map,(void(*)(star_t*,void*))
                              cb_star_clicked, gui );

        /* GUI setup */
        /* Main window */
        gui->app = gnome_app_new( app_name, 
                                  _("GNU SpaceChart: A 3D map of the stars") );
        gtk_signal_connect( GTK_OBJECT( gui->app ), "delete_event",
                            GTK_SIGNAL_FUNC( gtk_main_quit ), NULL );
        create_toolbar( gui );
        gui->menubar = NULL; /* The menus would be loaded now, but we must 
                              * wait until we have read the keybindings, so 
                              * they are created by 
                              * cb_settings_changed_keybindings */
        gui->accel = NULL;
     //   gui->appbar = gnome_appbar_new( TRUE, TRUE, GNOME_PREFERENCES_USER );
        /* Now the GUI main box */
        create_main_area( gui );

        /* File selection dialog */
        gui->open_dialog = gtk_file_selection_new( _("Load Star Database") );
        gtk_signal_connect( GTK_OBJECT(GTK_FILE_SELECTION(gui->open_dialog)->
                                       ok_button), "clicked", 
                            cb_open_ok_clicked, gui );
        gtk_signal_connect( GTK_OBJECT(GTK_FILE_SELECTION(gui->open_dialog)->
                                       cancel_button), "clicked", 
                            cb_open_cancel_clicked, gui );
        gtk_file_selection_set_filename( GTK_FILE_SELECTION(gui->open_dialog),
                                         "data" );

        /* Set the callbacks from the settings. */
        settings_add_callback( gui->settings, PROPERTIES_KEYBINDINGS,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_keybindings, gui );
        settings_add_callback( gui->settings, 
                               PROPERTIES_STAR_FILTER | PROPERTIES_LINK_FILTER
                               | PROPERTIES_VIEW_RADIUS | PROPERTIES_CENTER,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_shown_objects, gui );
        settings_add_callback( gui->settings, 
                               PROPERTIES_SHOW_LINKS | 
                               PROPERTIES_SHOW_LINK_LABELS
                               | PROPERTIES_SHOW_STAR_LABELS,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_toggle_buttons, gui );
        settings_add_callback( gui->settings, PROPERTIES_DISTANCE_UNIT,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_unit, gui );
        settings_add_callback( gui->settings, PROPERTIES_VIEW_RADIUS,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_view_radius, gui );
        settings_add_callback( gui->settings, PROPERTIES_LINK_FILTER,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_max_link, gui );
        settings_add_callback( gui->settings, PROPERTIES_SIGHT_PARAMS,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_sight_params, gui );
        settings_add_callback( gui->settings, PROPERTIES_CATALOG_FILENAME,
                               (void(*)(settings_t*, void*))
                               cb_settings_changed_catalog_filename, gui );

        return gui;
}

settings_t* gui_get_settings( gui_t *gui )
{
        return gui->settings;
}

star_catalog_t* gui_get_catalog( gui_t *gui )
{
        return gui->catalog;
}

void gui_destroy( gui_t *gui )
{
        map_destroy( gui->map );
        selection_dialog_destroy( gui->star_selection_dialog );
        star_info_destroy( gui->star_info_dialog );
        star_list_dialog_destroy( gui->star_list_dialog );
}

/* Local Functions. */
 
void cb_about_clicked( GtkWidget *button, gui_t* gui )
{
        gui->about_box = gnome_about_new( app_name, app_version, 
                                          "Copyright \251 2000, 2001, 2002, "
                                          "Free Software Foundation, Inc.",
                                          app_authors, 
                                          _("A map of the stars in 3D.\n"
                                            "For documentation check " 
                                            "doc" "."), NULL );
        gnome_dialog_set_parent( GNOME_DIALOG(gui->about_box), 
                                 GTK_WINDOW(gui->app) );
        gtk_widget_show(gui->about_box);
}

void cb_about_data_file_clicked( GtkWidget *button, gui_t* gui )
{
        GtkWidget *dialog;
        GtkWidget *label;
        GtkWidget *frame;

        dialog = gnome_dialog_new( _("About data file"),
                                   GTK_STOCK_OK, NULL );
        gnome_dialog_set_parent( GNOME_DIALOG(dialog), GTK_WINDOW(gui->app) );
        label = gtk_label_new( star_catalog_info( gui->catalog ) );
        frame = gtk_frame_new(NULL);
        gtk_container_add( GTK_CONTAINER(frame), label );
        //gtk_box_pack_start( GTK_BOX(GNOME_DIALOG(dialog)->vbox),frame,
          //                  FALSE,FALSE,0 );
     //   gtk_signal_connect( GTK_OBJECT(dialog), "clicked",
       //                     gnome_dialog_close, NULL );
        gtk_widget_show_all(dialog);
}

void cb_open_clicked( GtkWidget *button, gui_t* gui )
{
        gtk_widget_show_all( gui->open_dialog );
}

void cb_save_clicked( GtkWidget *button, gui_t* gui )
{
        GtkWidget *dialog = gnome_message_box_new( _("This will replace your "
                                                     "current default settings."
                                                     "\nDo you want to continue?"),
                                                   "error",
                                                   GTK_STOCK_YES,
                                                   GTK_STOCK_NO, 
                                                   NULL );
        int ret;
        gnome_dialog_set_parent( GNOME_DIALOG(dialog), GTK_WINDOW(gui->app) );
        ret = gnome_dialog_run( GNOME_DIALOG(dialog) );
        if( !ret )
                settings_save_settings( gui->settings );
}

void cb_close_clicked( GtkWidget *button, gui_t* gui )
{
        settings_set_catalog_filename( gui->settings, NULL );
}

void cb_reload_clicked( GtkWidget *button, gui_t* gui )
{
        settings_load_from_files( gui->settings );
}

void cb_list_stars_clicked( GtkWidget *button, gui_t* gui )
{
        star_list_dialog_show( gui->star_list_dialog );
        gtk_window_set_transient_for( 
                GTK_WINDOW(star_list_dialog_get_dialog(gui->star_list_dialog)),
                GTK_WINDOW(gui->app) );
}

void cb_open_ok_clicked( GtkWidget *button, gui_t* gui )
{
        char *filename = gtk_file_selection_get_filename( 
                GTK_FILE_SELECTION(gui->open_dialog));
        /* If the current file is a directory, do nothing */
        /*
        if( filename && !g_file_test(filename, G_FILE_TEST_ISDIR) )
        {
                gtk_widget_hide( gui->open_dialog );
                settings_set_catalog_filename( gui->settings, filename );
        }*/
}

void cb_open_cancel_clicked( GtkWidget *button, gui_t* gui )
{
        gtk_widget_hide( gui->open_dialog );
}

void cb_mov_clicked( GtkWidget* widget, gui_t* gui )
{
        coords_3d_t movement, center, los, up;
        double displace;

        settings_get_center( gui->settings, &center );
        settings_get_sight_params( gui->settings, &los, &up );
        /* Check which button is this and calculate the movement needed */
        switch( GPOINTER_TO_INT(gtk_object_get_data( GTK_OBJECT(widget), 
                                                     "mov_type")) )
        {
                char error[80];
        case MOVEMENT_UP:
                movement = up;
                break;
        case MOVEMENT_DOWN:
                movement.x = -up.x;
                movement.y = -up.y;
                movement.z = -up.z;
                break;
        case MOVEMENT_RIGHT:
                /* movement = los x up */
                movement.x = (up.y * los.z) - (up.z * los.y );
                movement.y = - (up.x * los.z) + (up.z * los.x );
                movement.z = (up.x * los.y) - (up.y * los.x );
                break;
        case MOVEMENT_LEFT:
               /* movement = up x los */
                movement.x = (los.y * up.z) - (los.z * up.y );
                movement.y = - (los.x * up.z) + (los.z * up.x );
                movement.z = (los.x * up.y) - (los.y * up.x );
                break;
        case MOVEMENT_FRONT:
                movement = los;
                break;
        case MOVEMENT_REAR:
                movement.x = -los.x;
                movement.y = -los.y;
                movement.z = -los.z;
                break;
        default:
                /* Can't happen. */
                sprintf( error, _("Error in file %s, line %d: Can't happen"),
                         __FILE__, __LINE__ );
                emit_error_message( error );
                return;
        }
        /* We move only one parsec */
        displace = DISPLACEMENT / magnitude( &movement );
        movement.x *= displace;
        movement.y *= displace;
        movement.z *= displace;
        center.x += movement.x;
        center.y += movement.y;
        center.z += movement.z;
        settings_set_center( gui->settings, &center );
}

void cb_toggled_show_links( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_links(gui->settings) !=
             GTK_TOGGLE_BUTTON(widget)->active) ) 
                settings_set_show_links( gui->settings, 
                                         GTK_TOGGLE_BUTTON(widget)->active );
}

void cb_toggled_show_link_labels( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_link_labels(gui->settings) !=
             GTK_TOGGLE_BUTTON(widget)->active) ) 
                settings_set_show_link_labels( gui->settings, 
                                               GTK_TOGGLE_BUTTON(widget)->active );
}

void cb_toggled_show_star_labels( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_star_labels(gui->settings) !=
             GTK_TOGGLE_BUTTON(widget)->active) ) 
                settings_set_show_star_labels(gui->settings, 
                                              GTK_TOGGLE_BUTTON( widget )->active );
}

void cb_toggled_show_links_menu( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_links(gui->settings) !=
             GTK_CHECK_MENU_ITEM(widget)->active) ) 
                settings_set_show_links( gui->settings, 
                                         GTK_CHECK_MENU_ITEM(widget)->active );
}

void cb_toggled_show_link_labels_menu( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_link_labels(gui->settings) !=
             GTK_CHECK_MENU_ITEM(widget)->active) ) 
                settings_set_show_link_labels( gui->settings, 
                                               GTK_CHECK_MENU_ITEM(widget)->active );
}

void cb_toggled_show_star_labels_menu( GtkWidget* widget, gui_t* gui )
{
        if( (settings_get_show_star_labels(gui->settings) !=
             GTK_CHECK_MENU_ITEM(widget)->active) ) 
                settings_set_show_star_labels( gui->settings, 
                                               GTK_CHECK_MENU_ITEM(widget)->active );
}

void cb_star_selection_clicked( GtkWidget* widget, gui_t* gui )
{
        selection_dialog_show( gui->star_selection_dialog );
}

void cb_settings_clicked( GtkWidget* widget, gui_t* gui )
{
        gui->config_dialog = config_dialog_new(gui->settings);
}

void cb_star_clicked( star_t *star, gui_t *gui )
{
        star_info_set_star( gui->star_info_dialog, star );
        gtk_window_set_transient_for( GTK_WINDOW(star_info_get_dialog(gui->star_info_dialog)), GTK_WINDOW(gui->app) );
}

void cb_center_star( star_t *star, gui_t *gui )
{
        coords_3d_t center;
        star_get_coords( star, &center );
        settings_set_center( gui->settings, &center );
}

void cb_settings_changed_shown_objects( settings_t *settings, gui_t *gui )
{
        star_t **star_list;
        link_t **link_list;
        
        star_catalog_select_objs( gui->catalog, &star_list, &link_list,
                                  (int(*)(star_t*,void*)) is_visible_star, 
                                  (int(*)(link_t*,void*)) is_visible_link, 
                                  gui );
        map_set_shown_objects( gui->map, star_list, link_list );
}

int is_visible_star( star_t *star, gui_t *gui )
{
        int matches;
        coords_3d_t center, star_coords;
        settings_get_center( gui->settings, &center );
        star_get_coords( star, &star_coords);
        matches = star_selection_match(
                settings_get_star_filter(gui->settings), star) && 
                (distance_3d( &center, &star_coords ) <= 
                 settings_get_view_radius(gui->settings));
        return matches;
}

int is_visible_link( link_t *link, gui_t *gui )
{
        return link_selection_match(settings_get_link_filter(gui->settings),
                                    link );
}

void cb_settings_changed_unit( settings_t *settings, gui_t *gui )
{
        double d;

        star_info_set_distance_units( gui->star_info_dialog, 
                                      settings_get_distance_unit( settings ) );
        d = settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS ?
                settings_get_view_radius(gui->settings) : 
                settings_get_view_radius(gui->settings) * PARSEC_TO_LY;
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->view_adjustment), d );
        d = settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS ?
                link_selection_get_max_length(
                        settings_get_link_filter(gui->settings)) : 
                link_selection_get_max_length(
                        settings_get_link_filter(gui->settings))*PARSEC_TO_LY;
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->max_link), d );

        if( settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS )
                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON
                                              (gui->radio_unit_pc), TRUE );
        else
                gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON
                                              (gui->radio_unit_ly), TRUE );
}

void cb_settings_changed_view_radius( settings_t *settings, gui_t *gui )
{
        double radius;
        if( settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS )
                radius = settings_get_view_radius(gui->settings);
        else
                radius = settings_get_view_radius(gui->settings) * 
                        PARSEC_TO_LY;
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->view_adjustment),
                                  radius ); 
}

void cb_settings_changed_max_link( settings_t *settings, gui_t *gui )
{
        double d;
        d = settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS ?
                link_selection_get_max_length(
                        settings_get_link_filter(gui->settings)) : 
                link_selection_get_max_length(
                        settings_get_link_filter(gui->settings))*PARSEC_TO_LY;
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->max_link), d );
}

void cb_settings_changed_keybindings( settings_t *settings, gui_t *gui )
{
        create_menus( gui );
}

void cb_changed_radius( GtkAdjustment *adj, gui_t* gui )
{
        double radius;
        if( settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS )
                radius = adj->value;
        else
                radius = adj->value / PARSEC_TO_LY;
        settings_set_view_radius(gui->settings, radius);
}

void cb_changed_max_link( GtkAdjustment *adj, gui_t* gui )
{
        link_selection_t *selection;
        double max;
        
        if( settings_get_distance_unit(gui->settings) == DISTANCE_PARSECS )
                max = adj->value;
        else
                max = adj->value / PARSEC_TO_LY;
        selection = settings_get_link_filter(gui->settings);
        link_selection_act_max_length(selection, max );
        settings_set_link_filter(gui->settings, selection);
}

void cb_changed_selection( star_selection_t* selection, gui_t* gui )
{
        settings_set_star_filter( gui->settings, selection );
}

void cb_toggled_unit( GtkWidget *widget, gui_t *gui )
{
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->radio_unit_pc))
                && (GPOINTER_TO_INT(gtk_object_get_data( 
                        GTK_OBJECT(widget), "unit")) == DISTANCE_PARSECS ) )
                settings_set_distance_unit( gui->settings, DISTANCE_PARSECS );
        else if( gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(gui->radio_unit_ly)) && (GPOINTER_TO_INT(
                        gtk_object_get_data(GTK_OBJECT(widget), 
                                            "unit")) == DISTANCE_LIGHT_YEARS ))
                settings_set_distance_unit(gui->settings,DISTANCE_LIGHT_YEARS);
}

void load_catalog( gui_t *gui, const char *file )
{
        int ret_code;
        char mesg[200];

        sprintf( mesg, _("Loading file %s"), file );
        star_catalog_destroy( gui->catalog );
        gui->catalog = star_catalog_new();
        gnome_app_flash( GNOME_APP(gui->app), mesg );
        gtk_widget_set_sensitive(gui->app, FALSE);
        gui->load = TRUE;
        ret_code = star_catalog_read(gui->catalog,file,(int(*)(float,void*))
                                     cb_load_progress,gui);
        switch(ret_code) {
                char error[200];
        case 1:
                sprintf( error, _("File %s not found"), file);
                emit_error_message( error );
                break;
        case 2:
                sprintf( error, _("File %s is not a valid catalogue file"), 
                         file );
                emit_error_message( error );
                break;
        case 3:
                emit_error_message( _("Error: Not enough memory") );
                break;
        case 4:
                star_catalog_destroy( gui->catalog );
                gui->catalog = star_catalog_new();
                break;
        }
        gtk_widget_set_sensitive(gui->app, TRUE);
        if(gui->loading_dialog) 
                gtk_widget_destroy( gui->loading_dialog );
        gui->loading_dialog = NULL;
        gtk_progress_set_percentage( 
                gnome_appbar_get_progress(GNOME_APPBAR(gui->appbar)), 0);
}

int cb_load_progress( float progress, gui_t *gui )
{
        /* If it's the first time the callback is called for this loading,
         * create the message box with the info about the catalog. We 
         * do it here, because that way we have access to the catalog info,
         * which is loaded before the first star is read. */
        if( progress > 0 && !gui->loading_dialog )
        {
                /*
                gui->loading_dialog = gnome_message_box_new
                        ( star_catalog_info(gui->catalog), 
                          GNOME_MESSAGE_BOX_INFO,
                          GNOME_STOCK_BUTTON_CANCEL, 
                          NULL ); */
                gnome_dialog_set_parent( GNOME_DIALOG(gui->loading_dialog), 
                                         GTK_WINDOW(gui->app) );
                gtk_signal_connect( GTK_OBJECT(gui->loading_dialog), 
                                    "clicked", cb_cancel_load_clicked, gui );
                gtk_widget_show( gui->loading_dialog );
        }
        gtk_progress_set_percentage( 
                gnome_appbar_get_progress(GNOME_APPBAR(gui->appbar)),progress);
        while (gtk_main_iteration_do(FALSE));
        return gui->load;
}

void cb_settings_changed_toggle_buttons( settings_t *settings, gui_t *gui )
{
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(gui->toggle_links_bar),
                                      settings_get_show_links(gui->settings) );
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(gui->toggle_link_labels_bar),
                                      settings_get_show_link_labels(gui->settings) );
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(gui->toggle_star_labels_bar),
                                      settings_get_show_star_labels(gui->settings) );
        gtk_check_menu_item_set_state( GTK_CHECK_MENU_ITEM(gui->toggle_links_menu),
                                       settings_get_show_links(gui->settings) );
        gtk_check_menu_item_set_state( GTK_CHECK_MENU_ITEM(gui->toggle_link_labels_menu),
                                       settings_get_show_link_labels(gui->settings) );
        gtk_check_menu_item_set_state( GTK_CHECK_MENU_ITEM(gui->toggle_star_labels_menu),
                                       settings_get_show_star_labels(gui->settings) );
}

void cb_settings_changed_sight_params( settings_t *settings, gui_t *gui )
{
        double lon, lat, north;
        double h, m;
        
        if( gui->changing_coords )
                return;
        settings_get_sight_params_polar( settings, &lon, &lat, &north );
        gtk_signal_handler_block( GTK_OBJECT(gui->rah_adj), gui->sig_rah );
        gtk_signal_handler_block( GTK_OBJECT(gui->ram_adj), gui->sig_ram );
        gtk_signal_handler_block( GTK_OBJECT(gui->lat_adj), gui->sig_lat );
        gtk_signal_handler_block( GTK_OBJECT(gui->north_adj), gui->sig_north );
        lon_to_ra( lon, &h, &m );
        lat *= 180/M_PI;
        north *= 180/M_PI;
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->rah_adj), h );
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->ram_adj), m );
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->lat_adj), lat );
        gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->north_adj), north );
        gtk_signal_handler_unblock( GTK_OBJECT(gui->rah_adj), gui->sig_rah );
        gtk_signal_handler_unblock( GTK_OBJECT(gui->ram_adj), gui->sig_ram );
        gtk_signal_handler_unblock( GTK_OBJECT(gui->lat_adj), gui->sig_lat );
        gtk_signal_handler_unblock( GTK_OBJECT(gui->north_adj), gui->sig_north );
}

void cb_settings_changed_catalog_filename( settings_t *settings, gui_t *gui )
{
        star_t **star_list;
        link_t **link_list;

        map_set_shown_objects( gui->map, NULL, NULL );
        if( settings_get_catalog_filename(settings) )
        {
                star_info_hide(gui->star_info_dialog);                
                load_catalog( gui, settings_get_catalog_filename(settings) );
                star_catalog_select_objs( gui->catalog, &star_list, &link_list,
                                          (int(*)(star_t*,void*)) is_visible_star, 
                                          (int(*)(link_t*,void*)) is_visible_link, 
                                          gui );
                map_set_shown_objects( gui->map, star_list, link_list );
                gtk_widget_set_sensitive( gui->about_data_menu, TRUE );
        }
        else
        {
                star_catalog_destroy( gui->catalog );
                gui->catalog = star_catalog_new();
                gtk_widget_set_sensitive( gui->about_data_menu, FALSE );
        }
        star_list_dialog_set_catalog( gui->star_list_dialog, gui->catalog );
}

void cb_changed_user_coords( GtkAdjustment *adj, gui_t* gui )
{
        static double h, m;
        double lon, lat, north;

        gui->changing_coords = 1;
        /* If the RA minutes just wrapped around, change the hours */
        /* FIXME: Is there a better way to detect wrap around? */
        if( GTK_ADJUSTMENT(gui->rah_adj)->value == h &&
            m == 59 && GTK_ADJUSTMENT(gui->ram_adj)->value == 0 )
        {
                gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->rah_adj),
                                          h == 23 ? 0 : 
                                          GTK_ADJUSTMENT(gui->rah_adj)
                                          ->value+1 );
                m = 0;
        }
        else if( GTK_ADJUSTMENT(gui->rah_adj)->value == h &&
                 m == 0 && GTK_ADJUSTMENT(gui->ram_adj)->value == 59 )
        {
                gtk_adjustment_set_value( GTK_ADJUSTMENT(gui->rah_adj),
                                          h == 0 ? 23 : 
                                          GTK_ADJUSTMENT(gui->rah_adj)
                                          ->value-1 );
                m = 59;
        }
        else
        {
                h = GTK_ADJUSTMENT(gui->rah_adj)->value;
                m = GTK_ADJUSTMENT(gui->ram_adj)->value;
                ra_to_lon( h, m, &lon );
                lat = GTK_ADJUSTMENT(gui->lat_adj)->value * (M_PI/180);
                north = GTK_ADJUSTMENT(gui->north_adj)->value * (M_PI/180);
                settings_set_sight_params_polar( gui->settings, lon, lat, 
                                                 north );
        }
        gui->changing_coords = 0;
}

void cb_cancel_load_clicked( GtkWidget *dialog, int button, gui_t* gui )
{
        gui->load = FALSE;
}

void cb_rotate_clicked( GtkWidget* widget, gui_t* gui )
{
        double v_ang, h_ang;

        switch( GPOINTER_TO_INT(gtk_object_get_data( GTK_OBJECT(widget), 
                                                     "rot_type")) )
        {
                char error[80];
        case ROTATION_UP:
                v_ang = ROTATION*((2*M_PI)/360);
                h_ang = 0;
                break;
        case ROTATION_DOWN:
                v_ang = (-1)*ROTATION*((2*M_PI)/360);
                h_ang = 0;
                break;
        case ROTATION_RIGHT:
                v_ang = 0;
                h_ang = ROTATION*((2*M_PI)/360);
                break;
        case ROTATION_LEFT:
                v_ang = 0;
                h_ang = (-1)*ROTATION*((2*M_PI)/360);
                break;
        default:
                /* Can't happen. */
                sprintf( error, _("Error in file %s, line %d: Can't happen"),
                         __FILE__, __LINE__ );
                emit_error_message( error );
                return;
        }
        map_rotate( gui->map, v_ang, h_ang );
}

/***********************************************************************
 * seldial.c : Implementation of the dialog for establishing a selection 
 *             of stars.
 * $Id: star_selection_dialog.c,v 1.1 2017/09/23 00:04:11 strauss Exp $
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
#include "../include/starmap.h"
#include "../include/star_selection.h"
#include "../include/star_selection_dialog.h"

struct st_sel_dial
{
        GtkWidget *dialog;
        GtkWidget *use_min_lum;
        GtkWidget *use_max_lum;
        GtkWidget *min_lum, *max_lum;
        GtkObject *min_lum_adj;
        GtkObject *max_lum_adj;
        GtkWidget *show_label;
        GtkWidget *show_O;
        GtkWidget *show_B;
        GtkWidget *show_A;
        GtkWidget *show_F;
        GtkWidget *show_G;
        GtkWidget *show_K;
        GtkWidget *show_M;
        GtkWidget *show_WD;
        GtkWidget *show_multiples;
        star_selection_t* selection;
        void (*changed)(star_selection_t*, void* );
        void* data;
};

/* Definition of local functions */

static void cb_clicked(GtkWidget *d, int but, selection_dialog_t* sel_dial);
static void cb_lum_button_clicked( GtkToggleButton *togglebutton,
                                   selection_dialog_t* sel_dial);
static void set_selection( selection_dialog_t* dialog );
static void get_selection( selection_dialog_t* dialog );

/* Public Functions */
selection_dialog_t* selection_dialog_new( star_selection_t* selection,
                                          void (*changed)( star_selection_t*,
                                                           void* ), 
                                          void* data )
{        
        selection_dialog_t* sel_dial;
        GtkWidget *table, *box, *frame, *label;

        if( (sel_dial = (selection_dialog_t*) 
             malloc( sizeof( selection_dialog_t ) ) ) )
        {
                sel_dial->selection = selection;
                sel_dial->changed = changed;
                sel_dial->data = data;
                
                sel_dial->dialog = gnome_dialog_new( _("Star Selection"),
                                                     GTK_STOCK_OK,
                                                     GTK_STOCK_APPLY,
                                                     GTK_STOCK_CANCEL,
                                                     NULL );
                gtk_signal_connect( GTK_OBJECT(sel_dial->dialog), "clicked",
                                    GTK_SIGNAL_FUNC(cb_clicked),
                                    sel_dial );

                table = gtk_table_new( 4, 2, FALSE );
        //        gtk_box_pack_start( GTK_BOX(GNOME_DIALOG(sel_dial->dialog)->
          //                                  vbox), table, TRUE, TRUE, 0 );
                gtk_container_border_width(GTK_CONTAINER(table), 10);

                sel_dial->use_min_lum = gtk_check_button_new_with_label(
                        _("Minimum Luminosity"));
                gtk_signal_connect( GTK_OBJECT(sel_dial->use_min_lum), 
                                    "toggled", 
                                    GTK_SIGNAL_FUNC(cb_lum_button_clicked),
                                    sel_dial );
                gtk_table_attach( GTK_TABLE(table), sel_dial->use_min_lum, 
                                  0, 1, 0, 1,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0 );
        
                sel_dial->use_max_lum = gtk_check_button_new_with_label(
                        _("Maximum Luminosity"));
                gtk_signal_connect( GTK_OBJECT(sel_dial->use_max_lum), 
                                    "toggled", 
                                    GTK_SIGNAL_FUNC(cb_lum_button_clicked),
                                    sel_dial );
                gtk_table_attach( GTK_TABLE(table), sel_dial->use_max_lum, 
                                  0, 1, 1, 2,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0 );
        
                sel_dial->min_lum_adj = gtk_adjustment_new(1, 0, 100, .01, 
                                                           10, 10);
                sel_dial->min_lum = gtk_spin_button_new( GTK_ADJUSTMENT(sel_dial->
                                                              min_lum_adj), 
                                               0.01, 3);
                gtk_table_attach( GTK_TABLE(table), sel_dial->min_lum, 1, 2, 0, 1,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0);
        
                sel_dial->max_lum_adj = gtk_adjustment_new(1, 0, 100, .01, 
                                                           10, 10);
                sel_dial->max_lum = gtk_spin_button_new( GTK_ADJUSTMENT(sel_dial->
                                                              max_lum_adj), 
                                               0.01, 3);
                gtk_table_attach( GTK_TABLE(table), sel_dial->max_lum, 1, 2, 1, 2,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0);

                sel_dial->show_multiples = gtk_check_button_new_with_label(
                        _("Show Multiples"));
                gtk_table_attach( GTK_TABLE(table), sel_dial->show_multiples, 
                                  0, 2, 2, 3,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0);

                frame = gtk_frame_new (_("Spectral Class"));
                gtk_table_attach( GTK_TABLE(table), frame, 0, 2, 3, 4, 
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL,
                                  (GtkAttachOptions) GTK_EXPAND | GTK_FILL, 
                                  0, 0);

                box = gtk_hbox_new( FALSE, 5 );
                gtk_container_add( GTK_CONTAINER(frame), box);
                gtk_container_border_width( GTK_CONTAINER(box), 10 );
                
                label = gtk_label_new(_("Show"));
                gtk_box_pack_start( GTK_BOX(box),label, TRUE, TRUE, 0);
                
                sel_dial->show_O = gtk_check_button_new_with_label ("O");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_O, 
                                    TRUE, TRUE, 0);
                
                sel_dial->show_B = gtk_check_button_new_with_label ("B");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_B, 
                                    TRUE, TRUE, 0);
        
                sel_dial->show_A = gtk_check_button_new_with_label ("A");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_A, 
                                    TRUE, TRUE, 0);

                sel_dial->show_F = gtk_check_button_new_with_label ("F");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_F, 
                                    TRUE, TRUE, 0);
        
                sel_dial->show_G = gtk_check_button_new_with_label ("G");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_G, 
                                    TRUE, TRUE, 0);
        
                sel_dial->show_K = gtk_check_button_new_with_label ("K");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_K, 
                                    TRUE, TRUE, 0);

                sel_dial->show_M = gtk_check_button_new_with_label ("M");
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_M, 
                                    TRUE, TRUE, 0);
        
                sel_dial->show_WD = gtk_check_button_new_with_label( 
                        _("White Dwarfs"));
                gtk_box_pack_start( GTK_BOX(box), sel_dial->show_WD, 
                                    TRUE, TRUE, 0);
                
                set_selection( sel_dial );
                
               // gtk_widget_show_all( GNOME_DIALOG(sel_dial->dialog)->vbox );
        }

        return sel_dial;
}

void selection_dialog_show( selection_dialog_t* dialog )
{
        set_selection( dialog );
        gtk_widget_show( dialog->dialog );
        gdk_window_raise( dialog->dialog->window );
}

void selection_dialog_destroy( selection_dialog_t* dialog )
{
        gtk_widget_destroy( dialog->dialog );
        free( dialog );
}

/* Local Functions */

void cb_clicked(GtkWidget *d, int but, selection_dialog_t* sel_dial)
{
        switch( but )
        {
        case 1:
                get_selection( sel_dial );
                if( sel_dial->changed )
                        sel_dial->changed(sel_dial->selection, sel_dial->data);
                break;
        case 0:
                get_selection( sel_dial );
                if( sel_dial->changed )
                        sel_dial->changed(sel_dial->selection, sel_dial->data);
                gtk_widget_hide( sel_dial->dialog );
                /* Fall Through */
        case 2:
                gtk_widget_hide( sel_dial->dialog );
        }
}

void cb_lum_button_clicked( GtkToggleButton *togglebutton,
                            selection_dialog_t* sel_dial)
{
        gtk_widget_set_sensitive(sel_dial->min_lum,
                                 gtk_toggle_button_get_active( 
                                         GTK_TOGGLE_BUTTON(sel_dial->
                                                           use_min_lum) ) );
        gtk_widget_set_sensitive(sel_dial->max_lum,
                                 gtk_toggle_button_get_active( 
                                         GTK_TOGGLE_BUTTON(sel_dial->
                                                           use_max_lum) ) );
}


static void set_selection( selection_dialog_t* dialog  )
{
        if( star_selection_is_min_lum( dialog->selection ) )
        {
                 gtk_toggle_button_set_active( 
                         GTK_TOGGLE_BUTTON(dialog->use_min_lum), TRUE );
                 gtk_adjustment_set_value( GTK_ADJUSTMENT( 
                         dialog->min_lum_adj ), star_selection_get_min_lum( 
                                 dialog->selection ) );
        }
        else
        {
                gtk_toggle_button_set_active( 
                         GTK_TOGGLE_BUTTON(dialog->use_min_lum), FALSE );
        }
        if( star_selection_is_max_lum( dialog->selection ) )
        {
                 gtk_toggle_button_set_active( 
                         GTK_TOGGLE_BUTTON(dialog->use_max_lum), TRUE );
                 gtk_adjustment_set_value( GTK_ADJUSTMENT( 
                         dialog->max_lum_adj ), star_selection_get_max_lum( 
                                 dialog->selection ) );
        }
        else
        {
                gtk_toggle_button_set_active( 
                         GTK_TOGGLE_BUTTON(dialog->use_max_lum), FALSE );
        }
        gtk_toggle_button_set_active(
                GTK_TOGGLE_BUTTON(dialog->show_multiples),
                star_selection_get_allow_multiples( dialog->selection ));
        if( star_selection_is_spectrum( dialog->selection ) )
        {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_O), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_O));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_B), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_B));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_A), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_A));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_F), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_F));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_G), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_G));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_K), 
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_K));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_M),
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_M));
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->show_WD),
                                             star_selection_is_showed(
                                                     dialog->selection,
                                                     SPECTRUM_WHITE_DWARF));
        }
        else
        {
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_O ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_B ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_A ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_F ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_G ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_K ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_M ), TRUE );
                gtk_toggle_button_set_active( 
                        GTK_TOGGLE_BUTTON( dialog->show_WD ), TRUE );
        }
        gtk_widget_set_sensitive(dialog->min_lum,
                                 gtk_toggle_button_get_active( 
                                         GTK_TOGGLE_BUTTON(dialog->
                                                           use_min_lum) ) );
        gtk_widget_set_sensitive(dialog->max_lum,
                                 gtk_toggle_button_get_active( 
                                         GTK_TOGGLE_BUTTON(dialog->
                                                           use_max_lum) ) );
}

static void get_selection( selection_dialog_t* dialog )
{
        int O, B, A, F, G, K, M, WD;

        if( GTK_TOGGLE_BUTTON( dialog->use_min_lum )->active )
                star_selection_act_min_lum( dialog->selection, 
                                            GTK_ADJUSTMENT
                                            (dialog->min_lum_adj)->value);
        else
                star_selection_deact_min_lum( dialog->selection );
        if( GTK_TOGGLE_BUTTON( dialog->use_max_lum )->active )
                star_selection_act_max_lum( dialog->selection, 
                                            GTK_ADJUSTMENT
                                            (dialog->max_lum_adj)->value);
        else
                star_selection_deact_max_lum( dialog->selection );
        star_selection_set_allow_multiples(
                dialog->selection, 
                GTK_TOGGLE_BUTTON(dialog->show_multiples)->active );
        O = GTK_TOGGLE_BUTTON( dialog->show_O )->active;
        B = GTK_TOGGLE_BUTTON( dialog->show_B )->active;
        A = GTK_TOGGLE_BUTTON( dialog->show_A )->active;
        F = GTK_TOGGLE_BUTTON( dialog->show_F )->active;
        G = GTK_TOGGLE_BUTTON( dialog->show_G )->active;
        K = GTK_TOGGLE_BUTTON( dialog->show_K )->active;
        M = GTK_TOGGLE_BUTTON( dialog->show_M )->active;
        WD = GTK_TOGGLE_BUTTON( dialog->show_WD )->active;
        star_selection_deact_spectrum( dialog->selection );
        if( O || B || A || F || G || K || M || WD )
        {
                if( O )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_O );
                if( B )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_B );
                if( A )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_A );
                if( F )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_F );
                if( G )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_G );
                if( K )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_K );
                if( M )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_M );
                if( WD )
                        star_selection_act_spectrum( dialog->selection, 
                                                     SPECTRUM_WHITE_DWARF );
        }
}


/***********************************************************************
 * map_widget.c : Implementation of the map widget.
 *                Includes the physical area of the map and the zoom 
 *               control.
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

#include "../include/map_widget.h"

struct st_map_widget
{
        GtkWidget* box;
        GtkWidget* map_widget;
        GdkPixmap* map;
        GtkWidget* zoom_scale;
        GtkObject* zoom_adj;
        GtkWidget* zoom_frame;
        int last_x;
        int last_y;
        double screen_size, x_center, y_center;
        void (*redraw)(map_widget_t*, void*);
        void (*rotate)( map_widget_t*, double v_ang, 
                        double h_ang, void* data );
        void (*clicked)( map_widget_t*, double x, double y, double lambda, 
                         void* data );
        void *data;
};

/* Declaration of private Functions */
static inline void get_screen_coords( double *x, double *y, 
                                      map_widget_t *map );
static gint cb_configure_event( GtkWidget *widget, GdkEventConfigure *event,
                                map_widget_t *map );
static gint cb_expose_event ( GtkWidget *widget, GdkEventExpose *event, 
                              map_widget_t *map );
static void cb_button_press_event( GtkWidget *widget, GdkEventButton* event,
                                   map_widget_t *map );
static void cb_motion_notify_event( GtkWidget *widget, GdkEventMotion* event,
                                    map_widget_t *map );
static void cb_changed_zoom( GtkAdjustment *adj, map_widget_t* map );

/* Public Functions */

map_widget_t *map_widget_new( void (*redraw)(map_widget_t*, void*),
                              void (*rotate)( map_widget_t*, double v_ang,
                                              double h_ang, void* data ),
                              void (*clicked)( map_widget_t*, double x, 
                                               double y, double lambda,
                                               void* data ),
                              void *data )
{
        map_widget_t *map;
        
        if( (map = (map_widget_t*) malloc( sizeof(map_widget_t) )) )
        {
                map->last_x = 0;
                map->last_y = 0;
                map->rotate = rotate;
                map->clicked = clicked;
                map->redraw = redraw;
                map->data = data;
                /* Create the map UI elements */
                map->map = NULL;
                map->map_widget = gtk_drawing_area_new();
                map->zoom_adj = gtk_adjustment_new( 1.0, 1.0, 20.0, 
                                                    0.5, 2.0, 0.0 );
                map->zoom_scale = gtk_vscale_new( GTK_ADJUSTMENT( 
                        map->zoom_adj ) );
                map->box = gtk_hbox_new( FALSE, 0 );
                map->zoom_frame = gtk_frame_new( _("Zoom") );
                /* Configure the widgets */
                gtk_container_border_width( GTK_CONTAINER(map->zoom_frame),5 );
                gtk_widget_set_events( map->map_widget, GDK_EXPOSURE_MASK
                                       | GDK_LEAVE_NOTIFY_MASK
                                       | GDK_BUTTON_PRESS_MASK
                                       | GDK_POINTER_MOTION_MASK
                                       | GDK_POINTER_MOTION_HINT_MASK);
                gtk_signal_connect( GTK_OBJECT( map->map_widget ), 
                                    "expose_event",
                                    GTK_SIGNAL_FUNC( cb_expose_event ), map );
                gtk_signal_connect( GTK_OBJECT( map->map_widget ), 
                                    "configure_event",
                                    GTK_SIGNAL_FUNC( cb_configure_event ), 
                                    map );
                gtk_signal_connect( GTK_OBJECT( map->map_widget ), 
                                    "button_press_event",
                                    GTK_SIGNAL_FUNC( cb_button_press_event ),
                                    map );
                gtk_signal_connect( GTK_OBJECT( map->map_widget ), 
                                    "motion_notify_event",
                                    GTK_SIGNAL_FUNC( cb_motion_notify_event ),
                                    map );
                gtk_signal_connect( GTK_OBJECT( map->zoom_adj ), 
                                    "value_changed", 
                                    GTK_SIGNAL_FUNC( cb_changed_zoom ), map );
                gtk_container_add( GTK_CONTAINER( map->zoom_frame ), 
                                   map->zoom_scale );
                gtk_box_pack_start( GTK_BOX( map->box ), map->map_widget, 
                                    TRUE, TRUE, 0 );
                gtk_box_pack_start( GTK_BOX( map->box ), map->zoom_frame, 
                                    FALSE, FALSE, 5 );
        }
        
        return map;
}


GtkWidget* map_widget_get_widget( map_widget_t* map_widget )
{
        return map_widget->box;
}

void map_widget_draw_point( map_widget_t* map_widget, double x, double y,
                            int radius, double rgb[] )
{
        GdkColor color;
        GdkGC* gc;

        gc = gdk_gc_new( map_widget->map_widget->window );
        
        color.red = (guint16) ( rgb[0] * 65535.0 );
        color.green = (guint16) ( rgb[1] * 65535.0 );
        color.blue = (guint16) ( rgb[2] * 65535.0 );

        color.pixel = (gulong) color.red * 65536 + 
                color.green * 256 + color.blue;

        get_screen_coords( &x, &y, map_widget );

        gdk_color_alloc( gtk_widget_get_colormap( map_widget->map_widget ), 
                         &color );
        gdk_gc_set_foreground( gc, &color );
        gdk_draw_arc ( map_widget->map, gc, TRUE, x - radius,y - radius,
                       radius*2, radius*2, 0*64, 360*64 );
        gdk_gc_destroy( gc );
}

void map_widget_draw_line( map_widget_t* map_widget, double x1, double y1,
                           double x2, double y2, int width, double rgb[],
                           GdkLineStyle style )
{
        GdkColor color;
        GdkGC* gc;

        gc = gdk_gc_new( map_widget->map_widget->window );
                
        color.red = (guint16) ( rgb[0] * 65535.0 );
        color.green = (guint16) ( rgb[1] * 65535.0 );
        color.blue = (guint16) ( rgb[2] * 65535.0 );
        
        color.pixel = (gulong) color.red * 65536 + color.green * 256 
                + color.blue;
        
        gdk_color_alloc( gtk_widget_get_colormap( map_widget->map_widget ), 
                         &color );
        gdk_gc_set_foreground( gc, &color );
        gdk_gc_set_line_attributes( gc, width, style, GDK_CAP_BUTT,
                                    GDK_JOIN_MITER );
        get_screen_coords( &x1, &y1, map_widget );
        get_screen_coords( &x2, &y2, map_widget );
        gdk_draw_line( map_widget->map, gc, x1, y1, x2, y2 );
        
        gdk_gc_destroy( gc );
}

void map_widget_draw_label( map_widget_t* map_widget, double x, double y,
                            int displace, GdkFont *font, double rgb[], 
                            const char label[] )
{
        GdkColor color;
        GdkGC* gc;

        gc = gdk_gc_new( map_widget->map_widget->window );

        color.red = (guint16) ( rgb[0] * 65535.0 );
        color.green = (guint16) ( rgb[1] * 65535.0 );
        color.blue = (guint16) ( rgb[2] * 65535.0 );
        
        color.pixel = (gulong) color.red * 65536 + 
                color.green * 256 + color.blue;

        get_screen_coords( &x, &y, map_widget );
        
        gdk_color_alloc( gtk_widget_get_colormap( 
                map_widget->map_widget ), &color );
        gdk_gc_set_foreground( gc, &color );
        gdk_draw_text( map_widget->map, font, gc, x+displace, y+displace, 
                       label, strlen(label) );

        gdk_gc_destroy( gc );
}

void map_widget_clear( map_widget_t *map_widget )
{
        gdk_draw_rectangle ( map_widget->map, 
                             map_widget->map_widget->style->black_gc,
                             TRUE, 0, 0, 
                             map_widget->map_widget->allocation.width,
                             map_widget->map_widget->allocation.height);
}

void map_widget_show( map_widget_t *map_widget )
{
        gdk_draw_pixmap(map_widget->map_widget->window,
                        map_widget->map_widget->style->fg_gc[GTK_WIDGET_STATE
                                                            (map_widget->map_widget)],
                        map_widget->map,
                        0, 0, 0, 0,
                        map_widget->map_widget->allocation.width,
                        map_widget->map_widget->allocation.height );
}

void map_widget_destroy( map_widget_t* map_widget )
{
        gtk_object_destroy( GTK_OBJECT( map_widget->box ) );
}

/* Private Functions */
inline void get_screen_coords( double *x, double *y, map_widget_t *map )
{
        *x = map->x_center + (*x * map->screen_size * 
                GTK_ADJUSTMENT(map->zoom_adj)->value);
        *y = map->y_center - (*y * map->screen_size * 
                GTK_ADJUSTMENT(map->zoom_adj)->value);
}

static gint cb_configure_event( GtkWidget *widget, GdkEventConfigure *event,
                                map_widget_t *map )
{
        if( map->map )
                gdk_pixmap_unref( map->map );
        map->map = gdk_pixmap_new( widget->window,
                                   widget->allocation.width,
                                   widget->allocation.height,
                                   -1);

        map->x_center = (widget->allocation.width/2);
        map->y_center = (widget->allocation.height/2);

        map->screen_size =  ( ( (widget->allocation.width) < 
                           (widget->allocation.height) ) ?
                         (widget->allocation.width/2) : 
                         (widget->allocation.height/2) ) - 10;
        
        map->redraw( map, map->data );

        return TRUE;
}

static gint cb_expose_event ( GtkWidget *widget, GdkEventExpose *event, 
                              map_widget_t *map )
{
        gdk_draw_pixmap(widget->window,
                        widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                        map->map,
                        event->area.x, event->area.y,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);

        return FALSE;
}

static void cb_button_press_event( GtkWidget *widget, GdkEventButton* event,
                                   map_widget_t *map )
{       
        if( event->button == 3 )
        {
                map->last_x = event->x;
                map->last_y = event->y;
        } else if( event->button == 1 )
        {
                double lambda = map->screen_size * 
                        GTK_ADJUSTMENT(map->zoom_adj)->value;
                map->last_x = event->x;
                map->last_y = event->y;
                map->clicked( map, event->x - map->x_center, 
                              map->y_center - event->y, lambda, 
                              map->data);
        }
}

static void cb_motion_notify_event( GtkWidget *widget, GdkEventMotion* event,
                                    map_widget_t *map )
{
        int x, y;
        double v_ang, h_ang;
        GdkModifierType state;

        if( event->is_hint )
                gdk_window_get_pointer( event->window, &x, &y, &state );
        else
        {
                x = event->x;
                y = event->y;
                state = event->state;
        }
        
        if( ( state & GDK_BUTTON3_MASK ) && 
            ( ( abs( (int)(map->last_x - x) ) > MIN_ROT ) ||
              ( abs( (int)(map->last_y - y) ) > MIN_ROT ) ) )
        {
                v_ang = -(map->last_y - y) * (M_PI/360); /* Rotate a degree for
                                                          * each 2 pixels */
                h_ang = (map->last_x - x) * (M_PI/360);
                map->rotate( map, v_ang, h_ang, map->data );                
                map->last_x = x;
                map->last_y = y;
        }
}

void cb_changed_zoom( GtkAdjustment *adj, map_widget_t* map )
{
        map->redraw( map, map->data );
}

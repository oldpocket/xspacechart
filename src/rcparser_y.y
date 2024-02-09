/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2000,2001,2002 Free Software Foundation, Inc.
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

%{
#include "../include/starmap.h"
#include "../include/settings.h"
#include "../include/rcparser.h"

extern int yylineno;
char *current_file;

#define YYPARSE_PARAM parser

struct link_rule
{
        double color[3];
        GdkLineStyle style;
        int width;
        double max_length;
        struct link_rule *next;
};

struct st_rcparser
{
        key_press_t *keybindings;
        double lon, lat, north, view_radius, max_link;
        coords_3d_t center;
        int show_links, show_link_labels, show_star_labels;
        double labels_color[3];
        char labels_font[256];
        distance_unit_t unit;
        star_selection_t *selection;
        double default_color[3];
        GdkLineStyle default_style;
        int default_width;
        struct link_rule *link_rules;
        star_drawing_rules_t *star_rules;
        char *filename;
};

/* Declaration of internal interface functions */

static void add_keybinding( rcparser_t *parser, keybinding_t binding,
                            key_press_t key_press );
static void set_user_coords( rcparser_t *parser, double lon, double lat );
static void set_north( rcparser_t *parser, double north );
static void set_center( rcparser_t *parser, coords_3d_t coords );
static void set_view_radius( rcparser_t *parser, double radius );
static void set_max_link( rcparser_t *parser, double max_link );
static void set_show_links( rcparser_t *parser, int show );
static void set_show_link_labels( rcparser_t *parser, int show );
static void set_show_star_labels( rcparser_t *parser, int show );
static void set_labels_color( rcparser_t *parser, double rgb[] );
static void set_labels_font( rcparser_t *parser, const char *string );
static void set_distance_unit( rcparser_t *parser, distance_unit_t unit );
static void set_star_selection( rcparser_t *parser, star_selection_t *sel );
static struct link_rule *new_link_rule( rcparser_t *parser, double distance,
                                        double rgb[], GdkLineStyle style,
                                        int width );
static void set_link_rules( rcparser_t *parser, struct link_rule *rules );
static void clear_link_rules( rcparser_t *parser );
static void set_link_defaults( rcparser_t *parser, double rgb[],
                               GdkLineStyle style, int width );
static void set_star_rules( rcparser_t *parser, star_drawing_rules_t *rules );
static void set_filename( rcparser_t *parser, char *filename );

%}

%union {
        int bool;
        double number;
        coords_3d_t coords;
        double rgb[3];
        distance_unit_t unit;
        key_press_t key_press;
        keybinding_t key_name;
        char *string;
        GdkLineStyle style;
        struct { int O, B, A, F, G, K, M, W; } spectral_class_list;
        struct { double O[3], B[3], A[3], F[3], 
                         G[3], K[3], M[3], W[3]; } star_colors;
        double star_sizes[4];
        star_selection_t *star_sel;
        struct link_rule *link_rule;
}

%token <bool> BOOL
%token <number> NUM
%token <unit> UNIT
%token <key_name> KEYNAME
%token <key_press> KEY
%token <key_press> KEY_MOD
%token <string> STRING
%token <style> STYLE
%token <spectral_class_list> SPECTRAL_CLASS_LIST
%token <modifier> KEY_MODIFIER
%token USER_COORDS, CENTER, VIEW_RADIUS, MAX_LINK, SHOW_LINKS, SHOW_LINK_LABELS
%token SHOW_STAR_LABELS, LABELS_COLOR, LABELS_FONT, DISTANCE_UNIT, NORTH
%token STAR_SELECTION, STAR_RULES, LINK_RULES, CATALOG

%type <number> distance
%type <star_sel> starselection
%type <coords> coords
%type <rgb> rgb
%type <star_colors> star_rules_colors
%type <star_sizes> star_rules_sizes
%type <key_press> key
%type <link_rule> link_rule
%type <link_rule> list_link_rules
%type <number> longitude

%%
input:          /* empty */ |
                input line
;

line:           '\n' | command '\n'
;

command:        keybinding
                | catalog
                | user_coords
                | north
                | center
                | view_radius
                | max_link
                | show_links
                | show_link_labels
                | show_star_labels
                | labels_color
                | labels_font
                | distance_unit
                | star_selection
                | star_rules
                | link_rules
;

keybinding:     KEYNAME '=' key
{ 
        add_keybinding((rcparser_t*)parser, $1, $3); 
};

catalog:        CATALOG '=' STRING
{
        set_filename((rcparser_t*)parser, $3 );
};

key:		KEY
		{
			$$ = $1;
		} |
		KEY_MOD '+' KEY
		{
			$$.mods = $1.mods;
			$$.key = $3.key;
		}

user_coords:    USER_COORDS '=' '(' NUM ',' longitude ')'
{
        set_user_coords((rcparser_t*)parser, $6, $4  * M_PI/180); 
};

longitude:      NUM
                {
                        $$ = $1 * M_PI/180;
                } |
                NUM 'h' NUM 'm'
                {
                        double lon;
                        ra_to_lon( $1, $3, &lon );
                        $$ = lon;
                }

north:          NORTH '=' NUM
{
        set_north((rcparser_t*)parser, $3 * M_PI/180);
};

center:         CENTER '=' coords
{
        set_center((rcparser_t*)parser, $3);
};

view_radius:    VIEW_RADIUS '=' distance
{
        set_view_radius((rcparser_t*)parser, $3);
};


max_link:       MAX_LINK '=' distance
{
        set_max_link((rcparser_t*)parser, $3);
};

show_links:     SHOW_LINKS '=' BOOL
{
        set_show_links((rcparser_t*)parser, $3);
};

show_link_labels: SHOW_LINK_LABELS '=' BOOL
{
        set_show_link_labels((rcparser_t*)parser, $3);
};

show_star_labels: SHOW_STAR_LABELS '=' BOOL
{
        set_show_star_labels((rcparser_t*)parser, $3);
};

labels_color: LABELS_COLOR '=' rgb
{
        set_labels_color((rcparser_t*)parser, $3);
};

labels_font: LABELS_FONT '=' STRING
{
        char msg[512];
        
        if( !gdk_font_load($3) )
        {       
                sprintf( msg, _("Font \"%s\" not found"), $3);
                yyerror(msg);
                YYERROR;
        }
        else
                set_labels_font((rcparser_t*)parser, $3);
};

distance_unit: DISTANCE_UNIT '=' UNIT
{
        set_distance_unit((rcparser_t*)parser, $3);
};

star_selection: STAR_SELECTION '=' starselection
{
        set_star_selection((rcparser_t*)parser, $3);
};

star_rules: STAR_RULES '=' '(' star_rules_colors ',' star_rules_sizes ','
                               starselection ')'
{
        star_drawing_rules_t *rules;
        star_selection_t *selection;

        rules = star_drawing_rules_new(DEFAULT_SIZE, default_color);

        /* Colors */

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_O );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_O, 0, 
                                $4.O, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_B );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_B, 0, 
                                $4.B, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_A );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_A, 0, 
                                $4.A, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_F );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_F, 0, 
                                $4.F, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_G );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_G, 0, 
                                $4.G, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_K );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_K, 0, 
                                $4.K, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_M );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_M, 0, 
                                $4.M, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_WHITE_DWARF );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_WD, 0, 
                                $4.W, FALSE );

        /* The size settings. */
        /* NOTE: The rules must be inserted in this order, so that in the case
           of multiple stars, if different components match different rules,
           the largest one will have preference. */

        selection = star_selection_new();
        star_selection_act_min_lum( selection, $6[3]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_4, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, $6[2]);
        star_selection_act_max_lum( selection, $6[3]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_3, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, $6[1]);
        star_selection_act_max_lum( selection, $6[2]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_2, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, $6[0]);
        star_selection_act_max_lum( selection, $6[1]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_1, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_max_lum( selection, $6[0] );
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_0, NULL, FALSE );

        /* The name settings. */
        star_drawing_rules_add( rules, $8, PRIORITY_SHOW_NAME, 
                                0, NULL, TRUE );

        set_star_rules( (rcparser_t*)parser, rules );
};

star_rules_colors:  '(' 'O' ':' rgb ';' 'B' ':' rgb ';' 'A' ':' rgb ';'
                        'F' ':' rgb ';' 'G' ':' rgb ';' 'K' ':' rgb ';' 
                        'M' ':' rgb ';' 'W' ':' rgb ')'
{
        $$.O[0] = $4[0];
        $$.O[1] = $4[1];
        $$.O[2] = $4[2];

        $$.B[0] = $8[0];
        $$.B[1] = $8[1];
        $$.B[2] = $8[2];

        $$.A[0] = $12[0];
        $$.A[1] = $12[1];
        $$.A[2] = $12[2];

        $$.F[0] = $16[0];
        $$.F[1] = $16[1];
        $$.F[2] = $16[2];

        $$.G[0] = $20[0];
        $$.G[1] = $20[1];
        $$.G[2] = $20[2];

        $$.K[0] = $24[0];
        $$.K[1] = $24[1];
        $$.K[2] = $24[2];

        $$.M[0] = $28[0];
        $$.M[1] = $28[1];
        $$.M[2] = $28[2];

        $$.W[0] = $32[0];
        $$.W[1] = $32[1];
        $$.W[2] = $32[2];
};

star_rules_sizes: '(' NUM ';' NUM ';' NUM ';' NUM ')'
{
        $$[0] = $2;
        $$[1] = $4;
        $$[2] = $6;
        $$[3] = $8;
};

link_rules: LINK_RULES '=' '(' list_link_rules ')'
{
        set_link_rules( (rcparser_t*)parser, $4 );
};

list_link_rules: link_rule ',' list_link_rules
{
        $1->next = $3;
        $$ = $1;
} |
link_rule 
{
        $$ = $1;
} | /* empty */
{
        $$ = NULL;
};

link_rule: '(' distance ';' rgb ';' STYLE ';' NUM ')'
{
        $$ = new_link_rule( (rcparser_t*)parser, $2, $4, $6, $8 );
} |
'(' rgb ';' STYLE ';' NUM ')' /* Default */
{
        set_link_defaults( (rcparser_t*)parser, $2, $4, $6 );
};

distance: NUM UNIT 
{ 
        $$ = $1 / ($2 == DISTANCE_PARSECS ? 1 : PARSEC_TO_LY);
};

rgb: '(' 'R' ':' NUM ';' 'G' ':' NUM ';' 'B' ':' NUM ')'
{
        $$[0] = $4;
        $$[1] = $8;
        $$[2] = $12;
}

coords: '(' NUM ',' NUM ',' NUM ')'
{
        $$.x = $2;
        $$.y = $4;
        $$.z = $6;
};

starselection: '(' NUM ';' NUM ';' SPECTRAL_CLASS_LIST ';' BOOL ')' 
{
        $$ = star_selection_new();
        star_selection_set_allow_multiples( $$, $8 );
        star_selection_act_min_lum( $$, $2 );
        star_selection_act_max_lum( $$, $4 );
        if( $6.O )
                star_selection_act_spectrum( $$, SPECTRUM_O );
        if( $6.B )
                star_selection_act_spectrum( $$, SPECTRUM_B );
        if( $6.A )
                star_selection_act_spectrum( $$, SPECTRUM_A );
        if( $6.F )
                star_selection_act_spectrum( $$, SPECTRUM_F );
        if( $6.G )
                star_selection_act_spectrum( $$, SPECTRUM_G );
        if( $6.K )
                star_selection_act_spectrum( $$, SPECTRUM_K );
        if( $6.M )
                star_selection_act_spectrum( $$, SPECTRUM_M );
        if( $6.W )
                star_selection_act_spectrum( $$, SPECTRUM_WHITE_DWARF );
} |
'(' NUM ';' ';' SPECTRAL_CLASS_LIST ';' BOOL ')' 
{
        $$ = star_selection_new();
        star_selection_set_allow_multiples( $$, $7 );
        star_selection_act_min_lum( $$, $2 );
        if( $5.O )
                star_selection_act_spectrum( $$, SPECTRUM_O );
        if( $5.B )
                star_selection_act_spectrum( $$, SPECTRUM_B );
        if( $5.A )
                star_selection_act_spectrum( $$, SPECTRUM_A );
        if( $5.F )
                star_selection_act_spectrum( $$, SPECTRUM_F );
        if( $5.G )
                star_selection_act_spectrum( $$, SPECTRUM_G );
        if( $5.K )
                star_selection_act_spectrum( $$, SPECTRUM_K );
        if( $5.M )
                star_selection_act_spectrum( $$, SPECTRUM_M );
        if( $5.W )
                star_selection_act_spectrum( $$, SPECTRUM_WHITE_DWARF );
} |
'(' ';' NUM ';' SPECTRAL_CLASS_LIST ';' BOOL ')'
{
        $$ = star_selection_new();
        star_selection_set_allow_multiples( $$, $7 );
        star_selection_act_max_lum( $$, $3 );
        if( $5.O )
                star_selection_act_spectrum( $$, SPECTRUM_O );
        if( $5.B )
                star_selection_act_spectrum( $$, SPECTRUM_B );
        if( $5.A )
                star_selection_act_spectrum( $$, SPECTRUM_A );
        if( $5.F )
                star_selection_act_spectrum( $$, SPECTRUM_F );
        if( $5.G )
                star_selection_act_spectrum( $$, SPECTRUM_G );
        if( $5.K )
                star_selection_act_spectrum( $$, SPECTRUM_K );
        if( $5.M )
                star_selection_act_spectrum( $$, SPECTRUM_M );
        if( $5.W )
                star_selection_act_spectrum( $$, SPECTRUM_WHITE_DWARF );
} |
'(' ';' ';' SPECTRAL_CLASS_LIST ';' BOOL ')'
{
        $$ = star_selection_new();
        star_selection_set_allow_multiples( $$, $6 );
        if( $4.O )
                star_selection_act_spectrum( $$, SPECTRUM_O );
        if( $4.B )
                star_selection_act_spectrum( $$, SPECTRUM_B );
        if( $4.A )
                star_selection_act_spectrum( $$, SPECTRUM_A );
        if( $4.F )
                star_selection_act_spectrum( $$, SPECTRUM_F );
        if( $4.G )
                star_selection_act_spectrum( $$, SPECTRUM_G );
        if( $4.K )
                star_selection_act_spectrum( $$, SPECTRUM_K );
        if( $4.M )
                star_selection_act_spectrum( $$, SPECTRUM_M );
        if( $4.W )
                star_selection_act_spectrum( $$, SPECTRUM_WHITE_DWARF );
};

%%

int yyerror ( char *s )
{
        char msg[256];
        sprintf( msg, _("File %s, Line %i: %s"), current_file, yylineno, s );
        emit_error_message(msg);
}

/* External Interface Functions */
        
rcparser_t *rcparser_new( void )
{
        rcparser_t *parser;
        int i;

        if( (parser = (rcparser_t*)malloc(sizeof(rcparser_t))) &&
            (parser->keybindings = (key_press_t*) 
             malloc(sizeof(key_press_t)*KEY_END_OF_LIST) ) )
        {
                parser->link_rules = NULL;
        }

        for( i = 0; i < KEY_END_OF_LIST; i++ )
        {
                parser->keybindings[i].key = 0;
                parser->keybindings[i].mods = 0;
        }

        parser->filename = NULL;

        return parser;
}

int rcparser_parse_file( rcparser_t *parser, const char *filename )
{
        int success;

        yylineno = 1;
        current_file = filename;
        rcparser_init_lex(filename);
        success = yyparse( parser );
        rcparser_close_lex();
}

void rcparser_apply_to_settings( rcparser_t *parser, settings_t *settings )
{
        link_selection_t *selection;
        link_drawing_rules_t *rules;
        double prev = 0;
        struct link_rule *r;

        /* Lots of changes here, save the callbacks for later. */
        settings_disable_callbacks( settings );

        settings_set_sight_params_polar( settings, 
                                         parser->lon, 
                                         parser->lat,
                                         parser->north );
        settings_set_center( settings, &parser->center );
        settings_set_view_radius( settings, parser->view_radius );
        selection = link_selection_new();
        link_selection_act_max_length( selection, parser->max_link );
        settings_set_link_filter( settings, selection );
        settings_set_show_links( settings, parser->show_links );
        settings_set_show_link_labels( settings, parser->show_link_labels );
        settings_set_show_star_labels( settings, parser->show_star_labels );
        settings_set_labels_color( settings, parser->labels_color );
        settings_set_labels_font( settings, parser->labels_font );
        settings_set_distance_unit( settings, parser->unit );
        settings_set_star_filter( settings, parser->selection );
        rules = link_drawing_rules_new( parser->default_width,
                                        parser->default_style,
                                        parser->default_color );
        for( r = parser->link_rules; r; r = r->next )
        {
                selection = link_selection_new();
                link_selection_act_min_length( selection, prev );
                link_selection_act_max_length( selection, r->max_length );
                link_drawing_rules_add( rules, selection, r->width, 
                                        r->style, r->color );
                prev = r->max_length;
        }
        settings_set_link_draw_rules( settings, rules );
        settings_set_star_draw_rules( settings, parser->star_rules );
        if( parser->filename )
                settings_set_catalog_filename( settings, parser->filename );
        settings_set_keybindings( settings, parser->keybindings );

        settings_enable_callbacks( settings );
}

void rcparser_destroy( rcparser_t *parser )
{
        struct link_rule *rule;

        for( rule = parser->link_rules; rule; rule = rule->next )
                free(rule);

        free(parser);
}

/* Internal interface */

void add_keybinding( rcparser_t *parser, keybinding_t binding,
                     key_press_t key_press )
{
        parser->keybindings[binding] = key_press;
}

void set_user_coords( rcparser_t *parser, double lon, double lat )
{
        parser->lon = lon;
        parser->lat = lat;
}

void set_north( rcparser_t *parser, double north )
{
        parser->north = north;
}

void set_center( rcparser_t *parser, coords_3d_t coords )
{
        parser->center = coords;
}

void set_view_radius( rcparser_t *parser, double radius )
{
        parser->view_radius = radius;
}

void set_max_link( rcparser_t *parser, double max_link )
{
        parser->max_link = max_link;
}

void set_show_links( rcparser_t *parser, int show )
{
        parser->show_links = show;
}

void set_show_link_labels( rcparser_t *parser, int show )
{
        parser->show_link_labels = show;
}

void set_show_star_labels( rcparser_t *parser, int show )
{
        parser->show_star_labels = show;
}

void set_labels_color( rcparser_t *parser, double rgb[] )
{
        parser->labels_color[0] = rgb[0];
        parser->labels_color[1] = rgb[1];
        parser->labels_color[2] = rgb[2];
}

void set_labels_font( rcparser_t *parser, const char *string )
{
        strncpy( parser->labels_font, string, 256 );
}

void set_distance_unit( rcparser_t *parser, distance_unit_t unit )
{
        parser->unit = unit;
}

void set_star_selection( rcparser_t *parser, star_selection_t *sel )
{
        parser->selection = sel;
}
                         
struct link_rule *new_link_rule( rcparser_t *parser, double distance,
                                 double rgb[], GdkLineStyle style, int width )
{
        struct link_rule *rule;
        
        rule = (struct link_rule*) malloc( sizeof(struct link_rule) );
        
        rule->next = NULL;
        rule->color[0] = rgb[0];
        rule->color[1] = rgb[1];
        rule->color[2] = rgb[2];
        rule->width = width;
        rule->style = style;
        rule->max_length = distance;

        return rule;
}

void clear_link_rules( rcparser_t *parser )
{
        struct link_rule *rule, *rule_tmp;

        for( rule = parser->link_rules; rule; rule = rule_tmp )
        {
                rule_tmp = rule->next;
                free( rule );
        }
        parser->link_rules = NULL;
}

void set_link_rules( rcparser_t *parser, struct link_rule *rules )
{
        if( parser->link_rules )
                clear_link_rules( parser );
        parser->link_rules = rules;
}

void set_link_defaults( rcparser_t *parser, double rgb[], GdkLineStyle style, 
                        int width )
{
        parser->default_color[0] = rgb[0];
        parser->default_color[1] = rgb[1];
        parser->default_color[2] = rgb[2];
        parser->default_style = style;
        parser->default_width = width;
}

void set_star_rules( rcparser_t *parser, star_drawing_rules_t *rules )
{
        parser->star_rules = rules;
}
                     
void set_filename( rcparser_t *parser, char *filename )
{
        parser->filename = filename;
}

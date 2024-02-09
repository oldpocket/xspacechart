/***********************************************************************
 * rcparser_l.l: Lexer for reading the config files.
 * $Id: rcparser_l.l,v 1.1 2017/09/23 00:04:11 strauss Exp $
 ***********************************************************************/

/***********************************************************************
 *  This file is part of SpaceChart.
 *  Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
#include <locale.h>
#include "../include/starmap.h"
#include "../include/star.h"
#include "../include/star_selection.h"
#include "../include/keys.h"
#include "rcparser_y.h"

static void is_in_list( char class, const char *list, int *is );

%}

%option yylineno

%%

#.*\n

[ \t\r]+

\\\n

catalog return CATALOG;

user_coords return USER_COORDS;

north return NORTH;

center return CENTER;

view_radius return VIEW_RADIUS;

max_link return MAX_LINK;

show_links return SHOW_LINKS;

show_link_labels return SHOW_LINK_LABELS;

show_star_labels return SHOW_STAR_LABELS;

labels_color return LABELS_COLOR;

labels_font return LABELS_FONT;

distance_unit return DISTANCE_UNIT;

star_selection return STAR_SELECTION;

star_rules return STAR_RULES;

link_rules return LINK_RULES;

pc { yylval.unit = DISTANCE_PARSECS; return UNIT; }

ly { yylval.unit = DISTANCE_LIGHT_YEARS; return UNIT; }

solid { yylval.style = GDK_LINE_SOLID; return STYLE; }

dashed { yylval.style = GDK_LINE_DOUBLE_DASH; return STYLE; }

-?([0-9]+|[0-9]*\.[0-9]+) { 
				char *locale;
				locale = setlocale( LC_NUMERIC, "C" );
				yylval.number = atof(yytext); 
				setlocale( LC_NUMERIC, locale );
				return NUM;
		      }

true { yylval.bool = 1; return BOOL; }

false { yylval.bool = 0; return BOOL; }

\[[OBAFGKMW]+\] {
        is_in_list( 'O', yytext, &(yylval.spectral_class_list.O) );
        is_in_list( 'B', yytext, &(yylval.spectral_class_list.B) );
        is_in_list( 'A', yytext, &(yylval.spectral_class_list.A) );
        is_in_list( 'F', yytext, &(yylval.spectral_class_list.F) );
        is_in_list( 'G', yytext, &(yylval.spectral_class_list.G) );
        is_in_list( 'K', yytext, &(yylval.spectral_class_list.K) );
        is_in_list( 'M', yytext, &(yylval.spectral_class_list.M) );
        is_in_list( 'W', yytext, &(yylval.spectral_class_list.W) );
        return SPECTRAL_CLASS_LIST;
}

\".*\" {
        int length;
        length = strlen(yytext);
        yylval.string = malloc( length-1 );
        strncpy( yylval.string, yytext+1, length-2 );
        yylval.string[length-2] = '\0';
        return STRING;
}

\n return '\n';

	/* Keybinding stuff */

[A-Z][a-zA-Z0-9]+ {
        keybinding_t keybinding = get_keybinding( yytext );
        GdkModifierType mod = get_mod( yytext );
        guint key = get_key( yytext );
        if( keybinding != -1 ) 
        {
                yylval.key_name = keybinding;
                return KEYNAME;
        }
        else if( mod != -1 )
        {
                yylval.key_press.mods = mod;
                return KEY_MOD;
        }
        else if( key != -1 )
        {
                yylval.key_press.mods = 0; 
                yylval.key_press.key = key; 
                return KEY;
        }
        else
                REJECT;
}

\'.\'	{ yylval.key_press.mods = 0; yylval.key_press.key = yytext[1]; return KEY; }

. return yytext[0];

%%

int yywrap( void )
{
        return 1;
}

int rcparser_init_lex( const char *filename )
{
        return (int) (yyin = fopen( filename, "r" ));
}

void rcparser_close_lex( void )
{
        fclose( yyin );
}

void is_in_list( char class, const char *list, int *is )
{
        int i;
        *is = 0;
        for( i = 0; list[i]; i++ )
                if( list[i] == class )
                        *is = 1;
}

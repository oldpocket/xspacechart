/* A Bison parser, made from rcparser_y.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	BOOL	257
# define	NUM	258
# define	UNIT	259
# define	KEYNAME	260
# define	KEY	261
# define	KEY_MOD	262
# define	STRING	263
# define	STYLE	264
# define	SPECTRAL_CLASS_LIST	265
# define	KEY_MODIFIER	266
# define	USER_COORDS	267
# define	CENTER	268
# define	VIEW_RADIUS	269
# define	MAX_LINK	270
# define	SHOW_LINKS	271
# define	SHOW_LINK_LABELS	272
# define	SHOW_STAR_LABELS	273
# define	LABELS_COLOR	274
# define	LABELS_FONT	275
# define	DISTANCE_UNIT	276
# define	NORTH	277
# define	STAR_SELECTION	278
# define	STAR_RULES	279
# define	LINK_RULES	280
# define	CATALOG	281

#line 20 "rcparser_y.y"

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


#line 85 "rcparser_y.y"
#ifndef YYSTYPE
typedef union {
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
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		199
#define	YYFLAG		-32768
#define	YYNTBASE	47

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 281 ? yytranslate[x] : 76)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      28,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      31,    33,     2,    30,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    37,    38,
       2,    29,     2,     2,     2,    40,    39,     2,     2,     2,
      41,    42,     2,     2,     2,    43,     2,    44,     2,    36,
       2,     2,    46,     2,     2,     2,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    34,     2,     2,     2,     2,    35,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    45,    49,    51,    55,    63,    65,    70,    74,
      78,    82,    86,    90,    94,    98,   102,   106,   110,   114,
     124,   158,   168,   174,   178,   180,   181,   191,   199,   202,
     216,   224,   234,   243,   252
};
static const short yyrhs[] =
{
      -1,    47,    48,     0,    28,     0,    49,    28,     0,    50,
       0,    51,     0,    53,     0,    55,     0,    56,     0,    57,
       0,    58,     0,    59,     0,    60,     0,    61,     0,    62,
       0,    63,     0,    64,     0,    65,     0,    66,     0,    69,
       0,     6,    29,    52,     0,    27,    29,     9,     0,     7,
       0,     8,    30,     7,     0,    13,    29,    31,     4,    32,
      54,    33,     0,     4,     0,     4,    34,     4,    35,     0,
      23,    29,     4,     0,    14,    29,    74,     0,    15,    29,
      72,     0,    16,    29,    72,     0,    17,    29,     3,     0,
      18,    29,     3,     0,    19,    29,     3,     0,    20,    29,
      73,     0,    21,    29,     9,     0,    22,    29,     5,     0,
      24,    29,    75,     0,    25,    29,    31,    67,    32,    68,
      32,    75,    33,     0,    31,    36,    37,    73,    38,    39,
      37,    73,    38,    40,    37,    73,    38,    41,    37,    73,
      38,    42,    37,    73,    38,    43,    37,    73,    38,    44,
      37,    73,    38,    45,    37,    73,    33,     0,    31,     4,
      38,     4,    38,     4,    38,     4,    33,     0,    26,    29,
      31,    70,    33,     0,    71,    32,    70,     0,    71,     0,
       0,    31,    72,    38,    73,    38,    10,    38,     4,    33,
       0,    31,    73,    38,    10,    38,     4,    33,     0,     4,
       5,     0,    31,    46,    37,     4,    38,    42,    37,     4,
      38,    39,    37,     4,    33,     0,    31,     4,    32,     4,
      32,     4,    33,     0,    31,     4,    38,     4,    38,    11,
      38,     3,    33,     0,    31,     4,    38,    38,    11,    38,
       3,    33,     0,    31,    38,     4,    38,    11,    38,     3,
      33,     0,    31,    38,    38,    11,    38,     3,    33,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   129,   129,   133,   133,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   154,   159,   164,   167,   174,   179,   182,   190,   195,
     200,   206,   211,   216,   221,   226,   231,   245,   250,   255,
     345,   382,   390,   395,   399,   403,   408,   411,   417,   422,
     429,   436,   458,   480,   502
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "BOOL", "NUM", "UNIT", "KEYNAME", "KEY", 
  "KEY_MOD", "STRING", "STYLE", "SPECTRAL_CLASS_LIST", "KEY_MODIFIER", 
  "USER_COORDS", "CENTER", "VIEW_RADIUS", "MAX_LINK", "SHOW_LINKS", 
  "SHOW_LINK_LABELS", "SHOW_STAR_LABELS", "LABELS_COLOR", "LABELS_FONT", 
  "DISTANCE_UNIT", "NORTH", "STAR_SELECTION", "STAR_RULES", "LINK_RULES", 
  "CATALOG", "'\\n'", "'='", "'+'", "'('", "','", "')'", "'h'", "'m'", 
  "'O'", "':'", "';'", "'B'", "'A'", "'F'", "'G'", "'K'", "'M'", "'W'", 
  "'R'", "input", "line", "command", "keybinding", "catalog", "key", 
  "user_coords", "longitude", "north", "center", "view_radius", 
  "max_link", "show_links", "show_link_labels", "show_star_labels", 
  "labels_color", "labels_font", "distance_unit", "star_selection", 
  "star_rules", "star_rules_colors", "star_rules_sizes", "link_rules", 
  "list_link_rules", "link_rule", "distance", "rgb", "coords", 
  "starselection", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    47,    47,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    50,    51,    52,    52,    53,    54,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    70,    70,    71,    71,    72,    73,
      74,    75,    75,    75,    75
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     7,     1,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     9,
      33,     9,     5,     3,     1,     0,     9,     7,     2,    13,
       7,     9,     8,     8,     7
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     2,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,    23,     0,    21,     0,     0,    29,
       0,    30,    31,    32,    33,    34,     0,    35,    36,    37,
      28,     0,    38,     0,    45,    22,     0,     0,     0,    48,
       0,     0,     0,     0,     0,     0,     0,    44,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
      45,    26,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,    50,     0,     0,     0,     0,    54,     0,     0,    39,
       0,     0,     0,     0,    52,    53,     0,     0,     0,    47,
       0,    51,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,    41,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,     0,     0
};

static const short yydefgoto[] =
{
       1,    19,    20,    21,    22,    56,    23,   102,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      84,   111,    36,    86,    87,    61,    67,    59,    72
};

static const short yypact[] =
{
  -32768,    33,   -23,   -22,   -21,   -18,   -17,   -16,   -15,   -14,
     -13,   -12,   -11,   -10,    -9,    -8,    -7,    -6,-32768,-32768,
      -3,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,     2,    -5,     3,
      24,    24,    26,    27,    28,     4,    31,    36,    38,    12,
      13,    32,    53,-32768,-32768,    34,-32768,    61,    62,-32768,
      63,-32768,-32768,-32768,-32768,-32768,    21,-32768,-32768,-32768,
  -32768,    -2,-32768,    39,    41,-32768,    66,    37,    42,-32768,
      40,    43,    -1,    44,    46,     1,    49,    47,-32768,    71,
      72,    79,     0,    48,    73,    50,    54,    51,    52,-32768,
      41,    57,    59,    64,    55,    65,    84,    86,    67,     4,
      95,    68,     4,    91,-32768,   100,-32768,   103,    69,    97,
      74,    75,   106,    76,    77,    12,    78,    80,    82,    87,
      85,    81,   118,   120,    92,    88,   122,    96,   114,   124,
  -32768,-32768,   126,   128,    99,   101,-32768,    98,   102,-32768,
     104,   105,   107,   108,-32768,-32768,     4,   129,   132,-32768,
     109,-32768,   111,   112,   110,   115,   113,   133,-32768,   135,
     117,   123,   125,     4,-32768,-32768,   119,   121,   127,     4,
     130,   131,   134,     4,   136,   116,   138,     4,   139,   137,
     141,     4,   142,   140,   145,     4,   143,-32768,   144,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    60,-32768,   -40,   -85,-32768,    22
};


#define	YYLAST		185


static const short yytable[] =
{
      98,    62,    81,    93,   105,    60,    37,    38,    39,    54,
      55,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,   123,    53,    57,   126,    60,    63,
      64,    65,    66,   198,    58,    66,    82,    94,   106,     2,
      68,    69,    70,    71,    73,    97,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    75,    74,    76,    77,    78,    80,    79,    89,
      83,   162,    85,    88,    90,   101,   103,    91,    96,   100,
      95,    92,    99,   104,   108,   110,   107,   109,   176,   112,
     113,   115,   116,   118,   180,   120,   117,   121,   184,   124,
     125,   127,   188,   119,   128,   122,   192,   129,   131,   134,
     196,   130,   132,   133,   135,   136,   138,   140,   139,   143,
     141,   144,   142,   145,   150,   146,   148,   147,   151,   149,
     152,   153,   154,   163,   155,   156,   164,   171,   159,   172,
     157,   161,   158,   168,   199,   160,     0,   137,   165,   166,
     167,     0,   169,   170,   173,     0,   174,   177,   175,   186,
     114,     0,   178,     0,   179,     0,     0,     0,   181,     0,
       0,   183,     0,   182,   185,   187,   197,   189,   191,     0,
     193,   190,   195,     0,     0,   194
};

static const short yycheck[] =
{
      85,    41,     4,     4,     4,     4,    29,    29,    29,     7,
       8,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,   109,    28,    31,   112,     4,     3,
       3,     3,    31,     0,    31,    31,    38,    38,    38,     6,
       9,     5,     4,    31,    31,    85,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     9,    31,    30,     4,     4,    46,     5,    32,
      31,   156,    31,     7,    32,     4,     4,    37,    32,    32,
      36,    38,    33,     4,    11,    31,    38,    37,   173,    38,
      38,    34,    33,    38,   179,    11,    32,    11,   183,     4,
      32,    10,   187,    38,     4,    38,   191,     4,    11,     3,
     195,    42,    38,    38,    38,    38,    38,    35,    38,    38,
      33,     3,    37,     3,    10,    33,     4,    39,     4,    33,
       4,     3,    33,     4,    33,    37,     4,     4,    33,     4,
      38,    33,    38,    33,     0,    38,    -1,   125,    39,    38,
      38,    -1,    37,    40,    37,    -1,    33,    38,    33,    43,
     100,    -1,    41,    -1,    37,    -1,    -1,    -1,    38,    -1,
      -1,    37,    -1,    42,    38,    37,    33,    38,    37,    -1,
      38,    44,    37,    -1,    -1,    45
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 21:
#line 155 "rcparser_y.y"
{ 
        add_keybinding((rcparser_t*)parser, yyvsp[-2].key_name, yyvsp[0].key_press); 
}
    break;
case 22:
#line 160 "rcparser_y.y"
{
        set_filename((rcparser_t*)parser, yyvsp[0].string );
}
    break;
case 23:
#line 165 "rcparser_y.y"
{
			yyval.key_press = yyvsp[0].key_press;
		}
    break;
case 24:
#line 169 "rcparser_y.y"
{
			yyval.key_press.mods = yyvsp[-2].key_press.mods;
			yyval.key_press.key = yyvsp[0].key_press.key;
		}
    break;
case 25:
#line 175 "rcparser_y.y"
{
        set_user_coords((rcparser_t*)parser, yyvsp[-1].number, yyvsp[-3].number  * M_PI/180); 
}
    break;
case 26:
#line 180 "rcparser_y.y"
{
                        yyval.number = yyvsp[0].number * M_PI/180;
                }
    break;
case 27:
#line 184 "rcparser_y.y"
{
                        double lon;
                        ra_to_lon( yyvsp[-3].number, yyvsp[-1].number, &lon );
                        yyval.number = lon;
                }
    break;
case 28:
#line 191 "rcparser_y.y"
{
        set_north((rcparser_t*)parser, yyvsp[0].number * M_PI/180);
}
    break;
case 29:
#line 196 "rcparser_y.y"
{
        set_center((rcparser_t*)parser, yyvsp[0].coords);
}
    break;
case 30:
#line 201 "rcparser_y.y"
{
        set_view_radius((rcparser_t*)parser, yyvsp[0].number);
}
    break;
case 31:
#line 207 "rcparser_y.y"
{
        set_max_link((rcparser_t*)parser, yyvsp[0].number);
}
    break;
case 32:
#line 212 "rcparser_y.y"
{
        set_show_links((rcparser_t*)parser, yyvsp[0].bool);
}
    break;
case 33:
#line 217 "rcparser_y.y"
{
        set_show_link_labels((rcparser_t*)parser, yyvsp[0].bool);
}
    break;
case 34:
#line 222 "rcparser_y.y"
{
        set_show_star_labels((rcparser_t*)parser, yyvsp[0].bool);
}
    break;
case 35:
#line 227 "rcparser_y.y"
{
        set_labels_color((rcparser_t*)parser, yyvsp[0].rgb);
}
    break;
case 36:
#line 232 "rcparser_y.y"
{
        char msg[512];
        
        if( !gdk_font_load(yyvsp[0].string) )
        {       
                sprintf( msg, _("Font \"%s\" not found"), yyvsp[0].string);
                yyerror(msg);
                YYERROR;
        }
        else
                set_labels_font((rcparser_t*)parser, yyvsp[0].string);
}
    break;
case 37:
#line 246 "rcparser_y.y"
{
        set_distance_unit((rcparser_t*)parser, yyvsp[0].unit);
}
    break;
case 38:
#line 251 "rcparser_y.y"
{
        set_star_selection((rcparser_t*)parser, yyvsp[0].star_sel);
}
    break;
case 39:
#line 257 "rcparser_y.y"
{
        star_drawing_rules_t *rules;
        star_selection_t *selection;

        rules = star_drawing_rules_new(DEFAULT_SIZE, default_color);

        /* Colors */

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_O );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_O, 0, 
                                yyvsp[-5].star_colors.O, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_B );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_B, 0, 
                                yyvsp[-5].star_colors.B, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_A );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_A, 0, 
                                yyvsp[-5].star_colors.A, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_F );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_F, 0, 
                                yyvsp[-5].star_colors.F, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_G );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_G, 0, 
                                yyvsp[-5].star_colors.G, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_K );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_K, 0, 
                                yyvsp[-5].star_colors.K, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_M );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_M, 0, 
                                yyvsp[-5].star_colors.M, FALSE );

        selection = star_selection_new();
        star_selection_act_spectrum( selection, SPECTRUM_WHITE_DWARF );
        star_drawing_rules_add( rules, selection, PRIORITY_SPECTRUM_WD, 0, 
                                yyvsp[-5].star_colors.W, FALSE );

        /* The size settings. */
        /* NOTE: The rules must be inserted in this order, so that in the case
           of multiple stars, if different components match different rules,
           the largest one will have preference. */

        selection = star_selection_new();
        star_selection_act_min_lum( selection, yyvsp[-3].star_sizes[3]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_4, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, yyvsp[-3].star_sizes[2]);
        star_selection_act_max_lum( selection, yyvsp[-3].star_sizes[3]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_3, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, yyvsp[-3].star_sizes[1]);
        star_selection_act_max_lum( selection, yyvsp[-3].star_sizes[2]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_2, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_min_lum( selection, yyvsp[-3].star_sizes[0]);
        star_selection_act_max_lum( selection, yyvsp[-3].star_sizes[1]);
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_1, NULL, FALSE );

        selection = star_selection_new();
        star_selection_act_max_lum( selection, yyvsp[-3].star_sizes[0] );
        star_drawing_rules_add( rules, selection, PRIORITY_LUMINOSITY, 
                                SIZE_0, NULL, FALSE );

        /* The name settings. */
        star_drawing_rules_add( rules, yyvsp[-1].star_sel, PRIORITY_SHOW_NAME, 
                                0, NULL, TRUE );

        set_star_rules( (rcparser_t*)parser, rules );
}
    break;
case 40:
#line 348 "rcparser_y.y"
{
        yyval.star_colors.O[0] = yyvsp[-29].rgb[0];
        yyval.star_colors.O[1] = yyvsp[-29].rgb[1];
        yyval.star_colors.O[2] = yyvsp[-29].rgb[2];

        yyval.star_colors.B[0] = yyvsp[-25].rgb[0];
        yyval.star_colors.B[1] = yyvsp[-25].rgb[1];
        yyval.star_colors.B[2] = yyvsp[-25].rgb[2];

        yyval.star_colors.A[0] = yyvsp[-21].rgb[0];
        yyval.star_colors.A[1] = yyvsp[-21].rgb[1];
        yyval.star_colors.A[2] = yyvsp[-21].rgb[2];

        yyval.star_colors.F[0] = yyvsp[-17].rgb[0];
        yyval.star_colors.F[1] = yyvsp[-17].rgb[1];
        yyval.star_colors.F[2] = yyvsp[-17].rgb[2];

        yyval.star_colors.G[0] = yyvsp[-13].rgb[0];
        yyval.star_colors.G[1] = yyvsp[-13].rgb[1];
        yyval.star_colors.G[2] = yyvsp[-13].rgb[2];

        yyval.star_colors.K[0] = yyvsp[-9].rgb[0];
        yyval.star_colors.K[1] = yyvsp[-9].rgb[1];
        yyval.star_colors.K[2] = yyvsp[-9].rgb[2];

        yyval.star_colors.M[0] = yyvsp[-5].rgb[0];
        yyval.star_colors.M[1] = yyvsp[-5].rgb[1];
        yyval.star_colors.M[2] = yyvsp[-5].rgb[2];

        yyval.star_colors.W[0] = yyvsp[-1].rgb[0];
        yyval.star_colors.W[1] = yyvsp[-1].rgb[1];
        yyval.star_colors.W[2] = yyvsp[-1].rgb[2];
}
    break;
case 41:
#line 383 "rcparser_y.y"
{
        yyval.star_sizes[0] = yyvsp[-7].number;
        yyval.star_sizes[1] = yyvsp[-5].number;
        yyval.star_sizes[2] = yyvsp[-3].number;
        yyval.star_sizes[3] = yyvsp[-1].number;
}
    break;
case 42:
#line 391 "rcparser_y.y"
{
        set_link_rules( (rcparser_t*)parser, yyvsp[-1].link_rule );
}
    break;
case 43:
#line 396 "rcparser_y.y"
{
        yyvsp[-2].link_rule->next = yyvsp[0].link_rule;
        yyval.link_rule = yyvsp[-2].link_rule;
}
    break;
case 44:
#line 401 "rcparser_y.y"
{
        yyval.link_rule = yyvsp[0].link_rule;
}
    break;
case 45:
#line 404 "rcparser_y.y"
{
        yyval.link_rule = NULL;
}
    break;
case 46:
#line 409 "rcparser_y.y"
{
        yyval.link_rule = new_link_rule( (rcparser_t*)parser, yyvsp[-7].number, yyvsp[-5].rgb, yyvsp[-3].style, yyvsp[-1].number );
}
    break;
case 47:
#line 413 "rcparser_y.y"
{
        set_link_defaults( (rcparser_t*)parser, yyvsp[-5].rgb, yyvsp[-3].style, yyvsp[-1].number );
}
    break;
case 48:
#line 418 "rcparser_y.y"
{ 
        yyval.number = yyvsp[-1].number / (yyvsp[0].unit == DISTANCE_PARSECS ? 1 : PARSEC_TO_LY);
}
    break;
case 49:
#line 423 "rcparser_y.y"
{
        yyval.rgb[0] = yyvsp[-9].number;
        yyval.rgb[1] = yyvsp[-5].number;
        yyval.rgb[2] = yyvsp[-1].number;
}
    break;
case 50:
#line 430 "rcparser_y.y"
{
        yyval.coords.x = yyvsp[-5].number;
        yyval.coords.y = yyvsp[-3].number;
        yyval.coords.z = yyvsp[-1].number;
}
    break;
case 51:
#line 437 "rcparser_y.y"
{
        yyval.star_sel = star_selection_new();
        star_selection_set_allow_multiples( yyval.star_sel, yyvsp[-1].bool );
        star_selection_act_min_lum( yyval.star_sel, yyvsp[-7].number );
        star_selection_act_max_lum( yyval.star_sel, yyvsp[-5].number );
        if( yyvsp[-3].spectral_class_list.O )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_O );
        if( yyvsp[-3].spectral_class_list.B )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_B );
        if( yyvsp[-3].spectral_class_list.A )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_A );
        if( yyvsp[-3].spectral_class_list.F )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_F );
        if( yyvsp[-3].spectral_class_list.G )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_G );
        if( yyvsp[-3].spectral_class_list.K )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_K );
        if( yyvsp[-3].spectral_class_list.M )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_M );
        if( yyvsp[-3].spectral_class_list.W )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_WHITE_DWARF );
}
    break;
case 52:
#line 460 "rcparser_y.y"
{
        yyval.star_sel = star_selection_new();
        star_selection_set_allow_multiples( yyval.star_sel, yyvsp[-1].bool );
        star_selection_act_min_lum( yyval.star_sel, yyvsp[-6].number );
        if( yyvsp[-3].spectral_class_list.O )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_O );
        if( yyvsp[-3].spectral_class_list.B )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_B );
        if( yyvsp[-3].spectral_class_list.A )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_A );
        if( yyvsp[-3].spectral_class_list.F )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_F );
        if( yyvsp[-3].spectral_class_list.G )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_G );
        if( yyvsp[-3].spectral_class_list.K )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_K );
        if( yyvsp[-3].spectral_class_list.M )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_M );
        if( yyvsp[-3].spectral_class_list.W )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_WHITE_DWARF );
}
    break;
case 53:
#line 482 "rcparser_y.y"
{
        yyval.star_sel = star_selection_new();
        star_selection_set_allow_multiples( yyval.star_sel, yyvsp[-1].bool );
        star_selection_act_max_lum( yyval.star_sel, yyvsp[-5].number );
        if( yyvsp[-3].spectral_class_list.O )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_O );
        if( yyvsp[-3].spectral_class_list.B )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_B );
        if( yyvsp[-3].spectral_class_list.A )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_A );
        if( yyvsp[-3].spectral_class_list.F )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_F );
        if( yyvsp[-3].spectral_class_list.G )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_G );
        if( yyvsp[-3].spectral_class_list.K )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_K );
        if( yyvsp[-3].spectral_class_list.M )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_M );
        if( yyvsp[-3].spectral_class_list.W )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_WHITE_DWARF );
}
    break;
case 54:
#line 504 "rcparser_y.y"
{
        yyval.star_sel = star_selection_new();
        star_selection_set_allow_multiples( yyval.star_sel, yyvsp[-1].bool );
        if( yyvsp[-3].spectral_class_list.O )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_O );
        if( yyvsp[-3].spectral_class_list.B )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_B );
        if( yyvsp[-3].spectral_class_list.A )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_A );
        if( yyvsp[-3].spectral_class_list.F )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_F );
        if( yyvsp[-3].spectral_class_list.G )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_G );
        if( yyvsp[-3].spectral_class_list.K )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_K );
        if( yyvsp[-3].spectral_class_list.M )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_M );
        if( yyvsp[-3].spectral_class_list.W )
                star_selection_act_spectrum( yyval.star_sel, SPECTRUM_WHITE_DWARF );
}
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 525 "rcparser_y.y"


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

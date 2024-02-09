#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

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


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */

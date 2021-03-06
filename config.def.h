/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const int gappx     = 5;                 /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
//static const char *fonts[]          = { "monospace:size=10" };
static const char *fonts[]          = { "Hack Nerd Font:size=12" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* autostart array */
static const char *const autostart[] = {
	"picom", "-b", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "","", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
//const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
//const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
        //{"spranger",    spcmd2},
	//{"keepassxc",   spcmd3},
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,       "spterm",	  NULL,	      SPTAG(0),	    1,	         -1 },
	{ NULL,       "spfm",     NULL,		SPTAG(1),		1,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ KeyPress, MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* type(Release/Press)     modifier                     key        function        argument */
	{ KeyPress, MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ KeyPress, MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ KeyPress, MODKEY,                       XK_b,      togglebar,      {0} },
	{ KeyPress, MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ KeyPress, MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ KeyPress, MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ KeyPress, MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ KeyPress, MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ KeyPress, MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ KeyPress, MODKEY,                       XK_Return, zoom,           {0} },
	{ KeyPress, MODKEY,                       XK_Tab,    view,           {0} },
	{ KeyPress, MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ KeyPress, MODKEY|ShiftMask,             XK_x,      killunsel,      {0} },
	{ KeyPress, MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ KeyPress, MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ KeyPress, MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ KeyPress, MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ KeyPress, MODKEY|ControlMask,           XK_r,      setlayout,      {.v = &layouts[4]} },
	{ KeyPress, MODKEY,                       XK_u,      setlayout,      {.v = &layouts[5]} },
	{ KeyPress, MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ KeyPress, MODKEY|ControlMask,		  XK_comma,  cyclelayout,    {.i = -1 } },
	{ KeyPress, MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ KeyPress, MODKEY,                       XK_space,  setlayout,      {0} },
	{ KeyPress, MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ KeyPress, MODKEY|ControlMask,           XK_space,  togglealwaysontop, {0} },
	{ KeyPress, MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ KeyPress, MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ KeyPress, MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ KeyPress, MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ KeyPress, MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ KeyPress, MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ KeyPress, MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ KeyPress, MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ KeyPress, MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ KeyPress, MODKEY,                       XK_n,      togglealttag,   {0} },
	{ KeyPress, MODKEY,                       XK_s,      togglescratch,  {.ui = 0 } },
	//{ KeyPress, MODKEY,                       XK_a,	     togglescratch,  {.ui = 1 } },
	//{ KeyPress, MODKEY,            	    XK_x,	     togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ KeyPress, MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ KeyPress, MODKEY|ShiftMask,             XK_r,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


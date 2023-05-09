/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int attachmode         = 0;        /* 0 master (default), 1 = above, 2 = aside, 3 = below, 4 = bottom */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_blue[]        = "#2C78BF";
static const char col_yellow[]      = "#fe8019";
static const char col_orange[]      = "#d65d0e";
static const char col_rust[]        = "#8b4000";
static const char col_green[]       = "#98971a";
static const char col_brown[]       = "#4e3946";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_rust,  col_rust  },
};
static const char *const autostart[] = {
	"st", NULL,
	NULL /* terminate */
};

/* tagging: refer to https://github.com/bakkeby/patches/wiki/tagicons */
static const char *tags[NUMTAGS] = { NULL };  /* left for compatibility reasons, i.e. code that checks LENGTH(tags) */
static char *tagicons[][NUMTAGS] = {
	[IconsDefault]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
	[IconsVacant]         = { NULL },
	[IconsOccupied]       = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                       instance    title   tags mask  switchtag  iscentered  isfloating   monitor */
	{ "Gimp",                      NULL,       NULL,   0,         1,         0,          0,           -1 },
	{ "Firefox",                   NULL,       NULL,   1 << 1,    1,         0,          0,           -1 },
	{ "Chromium-browser",          NULL,       NULL,   1 << 1,    1,         0,          0,           -1 },
	{ "Google-chrome",             NULL,       NULL,   1 << 1,    1,         0,          0,           -1 },
	{ "Brave-browser",             NULL,       NULL,   1 << 1,    1,         0,          0,           -1 },
	{ "Vivaldi-stable",            NULL,       NULL,   1 << 1,    1,         0,          0,           -1 },
	{ "thunderbird-default",       NULL,       NULL,   1 << 4,    0,         0,          0,           -1 },
        { "jetbrains-pycharm",         NULL,       NULL,   1 << 5,    1,         0,          0,           -1 },
	{ "TelegramDesktop",           NULL,       NULL,   1 << 6,    0,         0,          0,           -1 },
	{ "Zathura",                   NULL,       NULL,   1 << 7,    0,         0,          0,           -1 },
	{ "feh",                       NULL,       NULL,   0,         0,         1,          1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[]  = { "rofi", "-show", "run", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *slock[]  = { "slock", NULL };
static const char *chromecmd[]  = { "vivaldi-stable", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,                {.v = roficmd } },
	{ MODKEY,                       XK_Return, spawn,                {.v = termcmd } },
	{ MODKEY,                       XK_z,      spawn,                {.v = slock } },
	{ MODKEY,                       XK_c,      spawn,                {.v = chromecmd } },
	{ MODKEY,                       XK_grave,  togglescratch,        {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,            {0} },
	{ MODKEY,                       XK_Left,   focusdir,             {.i = 0 } }, // left
	{ MODKEY,                       XK_Right,  focusdir,             {.i = 1 } }, // right
	{ MODKEY,                       XK_Up,     focusdir,             {.i = 2 } }, // up
	{ MODKEY,                       XK_Down,   focusdir,             {.i = 3 } }, // down
	{ MODKEY|ControlMask,           XK_Left,   placedir,             {.i = 0 } }, // left
	{ MODKEY|ControlMask,           XK_Right,  placedir,             {.i = 1 } }, // right
	{ MODKEY|ControlMask,           XK_Up,     placedir,             {.i = 2 } }, // up
	{ MODKEY|ControlMask,           XK_Down,   placedir,             {.i = 3 } }, // down
	{ MODKEY,                       XK_i,      incnmaster,           {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,           {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,             {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,             {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,             {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,             {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,                 {0} },
	{ MODKEY,                       XK_k,      incrgaps,             {.i = +1 } },
	{ MODKEY,                       XK_j,      incrgaps,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      incrigaps,            {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      incrigaps,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_k,      incrogaps,            {.i = +1 } },
	{ MODKEY|ControlMask,           XK_j,      incrogaps,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_0,      togglegaps,           {0} },
	{ MODKEY|ShiftMask,             XK_0,      defaultgaps,          {0} },
	{ MODKEY,                       XK_Tab,    view,                 {0} },
	{ MODKEY,                       XK_q,      killclient,           {0} },
	{ MODKEY,                       XK_t,      setlayout,            {.v = &layouts[0]} },
	{ MODKEY,                       XK_e,      setlayout,            {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,            {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,            {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,       {0} },
	{ MODKEY,                       XK_f,      togglefullscreen,     {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefakefullscreen, {0} },
	{ MODKEY,                       XK_comma,  focusmon,             {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,             {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,               {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,               {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,          {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,          {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_a,      seticonset,           {.i = 0 } },
	{ MODKEY|ShiftMask,             XK_b,      seticonset,           {.i = 1 } },
	TAGKEYS(                        XK_1,                            0)
	TAGKEYS(                        XK_2,                            1)
	TAGKEYS(                        XK_3,                            2)
	TAGKEYS(                        XK_4,                            3)
	TAGKEYS(                        XK_5,                            4)
	TAGKEYS(                        XK_6,                            5)
	TAGKEYS(                        XK_7,                            6)
	TAGKEYS(                        XK_8,                            7)
	TAGKEYS(                        XK_9,                            8)
	{ MODKEY|ShiftMask,             XK_q,      quit,                 {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,           Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,           Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,           Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,        dragmfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,        dragcfact,      {0} },
	{ ClkTagBar,            0,                Button1,        view,           {0} },
	{ ClkTagBar,            0,                Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,           Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,           Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,                Button4,        cycleiconset,   {.i = +1 } },
	{ ClkTagBar,            0,                Button5,        cycleiconset,   {.i = -1 } },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};


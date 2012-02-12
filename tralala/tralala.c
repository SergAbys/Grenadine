/*
 *  tralala.c
 *
 *  Created : 25/06/11.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 31/01/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_jsurface              *tll_thirtySecond       [3];
static t_jsurface              *tll_sixteenth          [3];
static t_jsurface              *tll_sixteenthTriplet   [3];
static t_jsurface              *tll_sixteenthDotted    [3];
static t_jsurface              *tll_eighth             [3];
static t_jsurface              *tll_eighthTriplet      [3];
static t_jsurface              *tll_eighthDotted       [3];
static t_jsurface              *tll_quarter            [3];
static t_jsurface              *tll_quarterTriplet     [3];
static t_jsurface              *tll_quarterDotted      [3];
static t_jsurface              *tll_half               [3];
static t_jsurface              *tll_halfTriplet        [3];
static t_jsurface              *tll_halfDotted         [3];
static t_jsurface              *tll_whole              [3];

static tralalaSymbolsTableA    tll_symbolsA;
static tralalaSymbolsTableB    tll_symbolsB;

static PIZError                tll_clipboardError = PIZ_GOOD;
static PIZGrowingArray         *tll_clipboard = NULL;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark CLASS 
#pragma mark -

static t_class *tll_class = NULL;

int main (void *moduleRef) 
{   
t_class *c;

c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

c->c_flags |= CLASS_FLAG_NEWDICTIONARY;

jbox_initclass (c, JBOX_FONTATTR);

tralalaSymbolsTableAInit (&tll_symbolsA);
tralalaSymbolsTableBInit (&tll_symbolsB);

class_addmethod (c, (method)tralala_jsave,                  "jsave",                A_CANT, 0);
class_addmethod (c, (method)tralala_setvalueof,             "setvalueof",           A_CANT, 0);
class_addmethod (c, (method)tralala_getvalueof,             "getvalueof",           A_CANT, 0);
class_addmethod (c, (method)tralala_assist,                 "assist",               A_CANT, 0);
class_addmethod (c, (method)jbox_notify,                    "notify",               A_CANT, 0);
class_addmethod (c, (method)tralala_paint,                  "paint",                A_CANT, 0);
class_addmethod (c, (method)tralala_getdrawparams,          "getdrawparams",        A_CANT, 0);
class_addmethod (c, (method)tralala_mousedown,              "mousedown",            A_CANT, 0);
class_addmethod (c, (method)tralala_mousedrag,              "mousedrag",            A_CANT, 0);
class_addmethod (c, (method)tralala_mouseup,                "mouseup",              A_CANT, 0);
class_addmethod (c, (method)tralala_mouseenter,             "mouseenter",           A_CANT, 0);
class_addmethod (c, (method)tralala_mouseleave,             "mouseleave",           A_CANT, 0);
class_addmethod (c, (method)tralala_mousemove,              "mousemove",            A_CANT, 0);
class_addmethod (c, (method)tralala_mousewheel,             "mousewheel",           A_CANT, 0);
class_addmethod (c, (method)tralala_key,                    "key",                  A_CANT, 0);
class_addmethod (c, (method)tralala_focusgained,            "focusgained",          A_CANT, 0);
class_addmethod (c, (method)tralala_focuslost,              "focuslost",            A_CANT, 0);
class_addmethod (c, (method)tralala_patcherview_vis,        "patcherview_vis",      A_CANT, 0);
class_addmethod (c, (method)tralala_patcherview_invis,      "patcherview_invis",    A_CANT, 0);
class_addmethod (c, (method)object_obex_dumpout,            "dumpout",              A_CANT, 0);
class_addmethod (c, (method)tralala_int,                    "int",                  A_LONG, 0);
class_addmethod (c, (method)tralala_mute,                   "mute",                 A_LONG, 0);
class_addmethod (c, (method)tralala_list,                   "list",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handleMessages,         "user",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handleMessages,         "live",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handleMessages,         "listen",               A_GIMME, 0);
class_addmethod (c, (method)tralala_slot,                   "slot",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_anything,               "anything",             A_GIMME, 0);
class_addmethod (c, (method)tralala_play,                   "bang",                 0);
class_addmethod (c, (method)tralala_play,                   "play",                 0);
class_addmethod (c, (method)tralala_stop,                   "stop",                 0);
class_addmethod (c, (method)tralala_loop,                   "loop",                 0);
class_addmethod (c, (method)tralala_unloop,                 "unloop",               0);
class_addmethod (c, (method)tralala_pause,                  "pause",                0);
class_addmethod (c, (method)tralala_enter,                  "enter",                0);
class_addmethod (c, (method)tralala_copy,                   "copy",                 0);
class_addmethod (c, (method)tralala_forget,                 "forget",               0);

CLASS_ATTR_DEFAULT         (c, "patching_rect",    0, DEFAULT_PATCHING_RECT);
CLASS_ATTR_DEFAULT         (c, "fontname",         0, DEFAULT_FONTNAME);
CLASS_ATTR_DEFAULT         (c, "fontsize",         0, DEFAULT_FONTSIZE);
CLASS_ATTR_DEFAULT         (c, "fontface",         0, DEFAULT_FONTFACE);
CLASS_ATTR_MIN             (c, "patching_size",    0, PATCHING_SIZE_MINIMUM);

CLASS_ATTR_LONG            (c, "embedslots",       0, t_tralala, saveSlotsWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedslots",       0, "onoff", "Save Slots With Patcher");
CLASS_ATTR_CATEGORY        (c, "embedslots",       0, "Hint");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedslots",       0, "1"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedslots",       0, 1);

CLASS_ATTR_LONG            (c, "embedchannel",     0, t_tralala, saveChannelWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedchannel",     0, "onoff", "Save Channel With Patcher");
CLASS_ATTR_CATEGORY        (c, "embedchannel",     0, "Hint");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedchannel",     0, "1"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedchannel",     0, 1);

CLASS_ATTR_LONG            (c, "embedvalues",      0, t_tralala, saveValuesWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedvalues",      0, "onoff", "Save Values With Patcher");
CLASS_ATTR_CATEGORY        (c, "embedvalues",      0, "Hint");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedvalues",      0, "0"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedvalues",      0, 1);

CLASS_ATTR_ORDER           (c, "embedslots",       0, "1");
CLASS_ATTR_ORDER           (c, "embedvalues",      0, "2");
CLASS_ATTR_ORDER           (c, "embedchannel",     0, "3");

CLASS_ATTR_LONG            (c, "channel",          0, t_tralala, channel);
CLASS_ATTR_DEFAULT         (c, "channel",          0, DEFAULT_CHANNEL);
CLASS_ATTR_LABEL           (c, "channel",          0, "Channel");
CLASS_ATTR_CATEGORY        (c, "channel",          0, "Midi");
CLASS_ATTR_FILTER_CLIP     (c, "channel",          1, PIZ_MAGIC_CHANNEL);
CLASS_ATTR_ACCESSORS       (c, "channel",          NULL, tralala_setChannel);

CLASS_STICKY_ATTR          (c, "category",      0, "Value");

CLASS_ATTR_LONG            (c, "tempo",         0, t_tralala, tempo);
CLASS_ATTR_DEFAULT         (c, "tempo",         0, DEFAULT_TEMPO);
CLASS_ATTR_LABEL           (c, "tempo",         0, "Tempo");
CLASS_ATTR_FILTER_CLIP     (c, "tempo",         TEMPO_MINIMUM, TEMPO_MAXIMUM);

CLASS_ATTR_LONG            (c, "velocity",      0, t_tralala, velocity);
CLASS_ATTR_DEFAULT         (c, "velocity",      0, "0");
CLASS_ATTR_LABEL           (c, "velocity",      0, "Velocity Change");
CLASS_ATTR_ACCESSORS       (c, "velocity",      NULL, tralala_setVelocity);

CLASS_ATTR_SYM             (c, "scalekey",      0, t_tralala, scaleKey);
CLASS_ATTR_ENUM            (c, "scalekey",      0, "C C# D D# E F F# G G# A A# B");
CLASS_ATTR_DEFAULT         (c, "scalekey",      0, DEFAULT_SCALE_KEY);
CLASS_ATTR_ACCESSORS       (c, "scalekey",      NULL, tralala_setScaleKey);
CLASS_ATTR_LABEL           (c, "scalekey",      0, "Scale Key");

CLASS_ATTR_SYM             (c, "scaletype",     0, t_tralala, scaleType);
CLASS_ATTR_ENUM            (c, "scaletype",     0, SCALE_TYPE_LIST);
CLASS_ATTR_DEFAULT         (c, "scaletype",     0, DEFAULT_SCALE_TYPE);
CLASS_ATTR_ACCESSORS       (c, "scaletype",     NULL, tralala_setScaleType);
CLASS_ATTR_LABEL           (c, "scaletype",     0, "Scale Type");

CLASS_ATTR_LONG_ARRAY      (c, "scalecustom",   0, t_tralala, scaleCustom, PIZ_MAGIC_SCALE);
CLASS_ATTR_DEFAULT         (c, "scalecustom",   0, DEFAULT_SCALE_CUSTOM);
CLASS_ATTR_ACCESSORS       (c, "scalecustom",   NULL, tralala_setScaleCustom);
CLASS_ATTR_LABEL           (c, "scalecustom",   0, "Scale Custom");

CLASS_ATTR_SYM             (c, "patterncell",   0, t_tralala, patternCell);
CLASS_ATTR_ENUM            (c, "patterncell",   0, PATTERN_CELL_LIST);
CLASS_ATTR_DEFAULT         (c, "patterncell",   0, DEFAULT_PATTERN_CELL);
CLASS_ATTR_ACCESSORS       (c, "patterncell",   NULL, tralala_setPatternCell);
CLASS_ATTR_LABEL           (c, "patterncell",   0, "Pattern Cell");

CLASS_ATTR_LONG_VARSIZE    (c, "patterncustom", 0, t_tralala, patternCustom, patternSize, PATTERN_MAXIMUM_SIZE);
CLASS_ATTR_DEFAULT         (c, "patterncustom", 0, DEFAULT_PATTERN_CUSTOM);
CLASS_ATTR_LABEL           (c, "patterncustom", 0, "Pattern Custom");
CLASS_ATTR_ACCESSORS       (c, "patterncustom", NULL, tralala_setPatternCustom);

CLASS_ATTR_LONG            (c, "chance",        0, t_tralala, chance);
CLASS_ATTR_DEFAULT         (c, "chance",        0, DEFAULT_CHANCE);
CLASS_ATTR_LABEL           (c, "chance",        0, "Chance");
CLASS_ATTR_FILTER_CLIP     (c, "chance",        0, 100);
CLASS_ATTR_ACCESSORS       (c, "chance",        NULL, tralala_setChance);

CLASS_STICKY_ATTR_CLEAR     (c, "category");

CLASS_ATTR_ORDER            (c, "tempo",            0, "201");
CLASS_ATTR_ORDER            (c, "chance",           0, "202");
CLASS_ATTR_ORDER            (c, "scalekey",         0, "203");
CLASS_ATTR_ORDER            (c, "scaletype",        0, "204");
CLASS_ATTR_ORDER            (c, "scalecustom",      0, "205");
CLASS_ATTR_ORDER            (c, "patterncell",      0, "206");
CLASS_ATTR_ORDER            (c, "patterncustom",    0, "207");
CLASS_ATTR_ORDER            (c, "velocity",         0, "208");

CLASS_ATTR_LONG             (c, "viewtext",         0, t_tralala, viewText);
CLASS_ATTR_DEFAULT_SAVE     (c, "viewtext",         0, DEFAULT_VIEW_TEXT);
CLASS_ATTR_LABEL            (c, "viewtext",         0, "View Text");
CLASS_ATTR_CATEGORY         (c, "viewtext",         0, "Appearance");
CLASS_ATTR_STYLE            (c, "viewtext",         0, "onoff");
CLASS_ATTR_FILTER_CLIP      (c, "viewtext",         0, 1);

CLASS_STICKY_ATTR           (c, "category",         0, "Window");

CLASS_ATTR_DOUBLE           (c, "windowoffsetx",    0, t_tralala, windowOffsetX);
CLASS_ATTR_DEFAULT          (c, "windowoffsetx",    0, DEFAULT_WINDOW_OFFSET_X);
CLASS_ATTR_LABEL            (c, "windowoffsetx",    0, "Window X Offset");

CLASS_ATTR_DOUBLE           (c, "windowoffsety",    0, t_tralala, windowOffsetY);
CLASS_ATTR_DEFAULT          (c, "windowoffsety",    0, DEFAULT_WINDOW_OFFSET_Y);
CLASS_ATTR_LABEL            (c, "windowoffsety",    0, "Window Y Offset");

CLASS_ATTR_LONG             (c, "sequencemode",     0, t_tralala, sequenceMode);
CLASS_ATTR_DEFAULT          (c, "sequencemode",     0, DEFAULT_SEQUENCE_MODE);
CLASS_ATTR_ENUMINDEX        (c, "sequencemode",     0, "user live listen");
CLASS_ATTR_LABEL            (c, "sequencemode",     0, "Sequence Mode");
CLASS_ATTR_ACCESSORS        (c, "sequencemode",     NULL, tralala_setSequenceMode);

CLASS_ATTR_LONG             (c, "zoommode",         0, t_tralala, zoomMode);
CLASS_ATTR_DEFAULT          (c, "zoommode",         0, DEFAULT_ZOOM_MODE);
CLASS_ATTR_LABEL            (c, "zoommode",         0, "Zoom Mode");
CLASS_ATTR_FILTER_CLIP      (c, "zoommode",         0, 2);

CLASS_STICKY_ATTR_CLEAR     (c, "category");

CLASS_ATTR_ORDER            (c, "sequencemode",     0, "101");
CLASS_ATTR_ORDER            (c, "zoommode",         0, "102");
CLASS_ATTR_ORDER            (c, "windowoffsetx",    0, "103");
CLASS_ATTR_ORDER            (c, "windowoffsety",    0, "104");

CLASS_ATTR_INVISIBLE        (c, "textcolor", 0);

CLASS_STICKY_ATTR           (c, "category",             0, "Color");

CLASS_ATTR_RGBA             (c, "backgroundcolor",      0, t_tralala, backgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "backgroundcolor",      0, DEFAULT_BACKGROUND_COLOR);
CLASS_ATTR_INVISIBLE        (c, "backgroundcolor",      0);

CLASS_ATTR_RGBA             (c, "lassocolor",           0, t_tralala, lassoColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "lassocolor",           0, DEFAULT_LASSO_COLOR);
CLASS_ATTR_INVISIBLE        (c, "lassocolor",           0);

CLASS_ATTR_RGBA             (c, "unfocusedtextcolor",   0, t_tralala, unfocusedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedtextcolor",   0, DEFAULT_UNFOCUSED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedtextcolor",   0, "rgba", "Unfocused Text");
CLASS_ATTR_RGBA             (c, "unfocusedbordercolor", 0, t_tralala, unfocusedBorderColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedbordercolor", 0, DEFAULT_UNFOCUSED_BORDER_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedbordercolor", 0, "rgba", "Unfocused Border");
CLASS_ATTR_RGBA             (c, "focusedtextcolor",     0, t_tralala, focusedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedtextcolor",     0, DEFAULT_FOCUSED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedtextcolor",     0, "rgba", "Focused Text");
CLASS_ATTR_RGBA             (c, "focusedbordercolor",   0, t_tralala, focusedBorderColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedbordercolor",   0, DEFAULT_FOCUSED_BORDER_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedbordercolor",   0, "rgba", "Focused Border");
CLASS_ATTR_RGBA             (c, "selectedtextcolor",    0, t_tralala, selectedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "selectedtextcolor",    0, DEFAULT_SELECTED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "selectedtextcolor",    0, "rgba", "Selected Text");

CLASS_ATTR_RGBA             (c, "unfocusedusernotecolor",        0, t_tralala, unfocusedUserNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedusernotecolor",        0, DEFAULT_UNFOCUSED_USER_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedusernotecolor",        0, "rgba", "Unfocused User Note");
CLASS_ATTR_RGBA             (c, "focuseduserselectednotecolor",  0, t_tralala, focusedUserSelectedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focuseduserselectednotecolor",  0, DEFAULT_FOCUSED_USER_SELECTED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focuseduserselectednotecolor",  0, "rgba", "Focused User Selected Note");
CLASS_ATTR_RGBA             (c, "unfocuseduserselectednotecolor",0, t_tralala, unfocusedUserSelectedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocuseduserselectednotecolor",0, DEFAULT_UNFOCUSED_USER_SELECTED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocuseduserselectednotecolor",0, "rgba", "Unfocused User Selected Note");
CLASS_ATTR_RGBA             (c, "focusedusermarkednotecolor",    0, t_tralala, focusedUserMarkedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedusermarkednotecolor",    0, DEFAULT_FOCUSED_USER_MARKED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedusermarkednotecolor",    0, "rgba", "Focused User Marked Note");
CLASS_ATTR_RGBA             (c, "focusedusersilentnotecolor",    0, t_tralala, focusedUserSilentNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedusersilentnotecolor",    0, DEFAULT_FOCUSED_USER_SILENT_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedusersilentnotecolor",    0, "rgba", "Focused User Silent Note");

CLASS_ATTR_RGBA             (c, "unfocusedlivenotecolor",       0, t_tralala, unfocusedLiveNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedlivenotecolor",       0, DEFAULT_UNFOCUSED_LIVE_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedlivenotecolor",       0, "rgba", "Unfocused Live Note");
CLASS_ATTR_RGBA             (c, "unfocusedliveplayednotecolor", 0, t_tralala, unfocusedLivePlayedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedliveplayednotecolor", 0, DEFAULT_UNFOCUSED_LIVE_PLAYED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedliveplayednotecolor", 0, "rgba", "Unfocused Live Played Note");
CLASS_ATTR_RGBA             (c, "focusedlivenotecolor",         0, t_tralala, focusedLiveNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedlivenotecolor",         0, DEFAULT_FOCUSED_LIVE_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedlivenotecolor",         0, "rgba", "Focused Live Note");
CLASS_ATTR_RGBA             (c, "focusedliveplayednotecolor",   0, t_tralala, focusedLivePlayedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedliveplayednotecolor",   0, DEFAULT_FOCUSED_LIVE_PLAYED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedliveplayednotecolor",   0, "rgba", "Focused Live Played Note");

CLASS_ATTR_RGBA             (c, "unfocusedlistennotecolor",     0, t_tralala, unfocusedListenNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedlistennotecolor",     0, DEFAULT_UNFOCUSED_LISTEN_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedlistennotecolor",     0, "rgba", "Unfocused Listen Note");
CLASS_ATTR_RGBA             (c, "focusedlistennotecolor",       0, t_tralala, focusedListenNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedlistennotecolor",       0, DEFAULT_FOCUSED_LISTEN_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedlistennotecolor",       0, "rgba", "Focused Listen Note");

CLASS_ATTR_RGBA             (c, "popuptextcolor",    0, t_tralala, popupTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuptextcolor",    0, DEFAULT_POPUP_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuptextcolor",    0, "rgba", "Popup Text");
CLASS_ATTR_RGBA             (c, "popubgcolor",       0, t_tralala, popupBackgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popubgcolor",       0, DEFAULT_POPUP_BACKGROUND_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popubgcolor",       0, "rgba", "Popup Background");
CLASS_ATTR_RGBA             (c, "popuphlttextcolor", 0, t_tralala, popupHighlightedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuphlttextcolor", 0, DEFAULT_POPUP_HIGHLIGHTED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuphlttextcolor", 0, "rgba", "Popup Highlighted Text");
CLASS_ATTR_RGBA             (c, "popuhltbgcolor",    0, t_tralala, popupHighlightedBackgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuhltbgcolor",    0, DEFAULT_POPUP_HIGHLIGHTED_BACKGROUND_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuhltbgcolor",    0, "rgba", "Popup Highlighted Background");

CLASS_ATTR_RGBA             (c, "focuseduserzonecolor",     0, t_tralala, focusedUserZoneColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focuseduserzonecolor",     0, DEFAULT_FOCUSED_USER_ZONE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focuseduserzonecolor",     0, "rgba", "Focused User Zone");
CLASS_ATTR_RGBA             (c, "focusedlivezonecolor",     0, t_tralala, focusedLiveZoneColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedlivezonecolor",     0, DEFAULT_FOCUSED_LIVE_ZONE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedlivezonecolor",     0, "rgba", "Focused Live Zone");
CLASS_ATTR_RGBA             (c, "focusedselectedzonecolor", 0, t_tralala, focusedSelectedZoneColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedselectedzonecolor", 0, DEFAULT_FOCUSED_SELECTED_ZONE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedselectedzonecolor", 0, "rgba", "Focused Selected Zone");
CLASS_ATTR_RGBA             (c, "unfocusedzonecolor",       0, t_tralala, unfocusedZoneColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedzonecolor",       0, DEFAULT_UNFOCUSED_ZONE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedzonecolor",       0, "rgba", "Unfocused Zone");

CLASS_ATTR_RGBA             (c, "candycanec",       0, t_tralala, cNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanec",       0, DEFAULT_C_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanec",       0, "rgba", "Candycane C");
CLASS_ATTR_RGBA             (c, "candycanecsharp",  0, t_tralala, cSharpNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanecsharp",  0, DEFAULT_C_SHARP_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanecsharp",  0, "rgba", "Candycane C#");
CLASS_ATTR_RGBA             (c, "candycaned",       0, t_tralala, dNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycaned",       0, DEFAULT_D_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycaned",       0, "rgba", "Candycane D");
CLASS_ATTR_RGBA             (c, "candycanedsharp",  0, t_tralala, dSharpNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanedsharp",  0, DEFAULT_D_SHARP_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanedsharp",  0, "rgba", "Candycane D#");
CLASS_ATTR_RGBA             (c, "candycanee",       0, t_tralala, eNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanee",       0, DEFAULT_E_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanee",       0, "rgba", "Candycane E");
CLASS_ATTR_RGBA             (c, "candycanef",       0, t_tralala, fNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanef",       0, DEFAULT_F_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanef",       0, "rgba", "Candycane F");
CLASS_ATTR_RGBA             (c, "candycanefsharp",  0, t_tralala, fSharpNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanefsharp",  0, DEFAULT_F_SHARP_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanefsharp",  0, "rgba", "Candycane F#");
CLASS_ATTR_RGBA             (c, "candycaneg",       0, t_tralala, gNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycaneg",       0, DEFAULT_G_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycaneg",       0, "rgba", "Candycane G");
CLASS_ATTR_RGBA             (c, "candycanegsharp",  0, t_tralala, gSharpNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanegsharp",  0, DEFAULT_G_SHARP_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanegsharp",  0, "rgba", "Candycane G#");
CLASS_ATTR_RGBA             (c, "candycanea",       0, t_tralala, aNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycanea",       0, DEFAULT_A_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycanea",       0, "rgba", "Candycane A");
CLASS_ATTR_RGBA             (c, "candycaneasharp",  0, t_tralala, aSharpNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycaneasharp",  0, DEFAULT_A_SHARP_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycaneasharp",  0, "rgba", "Candycane A#");
CLASS_ATTR_RGBA             (c, "candycaneb",       0, t_tralala, bNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "candycaneb",       0, DEFAULT_B_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "candycaneb",       0, "rgba", "Candycane B");

CLASS_STICKY_ATTR_CLEAR     (c, "category");
CLASS_STICKY_ATTR           (c, "category",             0, "Font");

CLASS_ATTR_SYM              (c, "popupfontname",        0, t_tralala, popupFontName);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontname",        0, DEFAULT_POPUP_FONTNAME);
CLASS_ATTR_STYLE_LABEL      (c, "popupfontname",        0, "font", "Popup Font Name");
CLASS_ATTR_DOUBLE           (c, "popupfontsize",        0, t_tralala, popupFontSize);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontsize",        0, DEFAULT_POPUP_FONTSIZE);
CLASS_ATTR_ENUM             (c, "popupfontsize",        0, FONTSIZE_LIST);
CLASS_ATTR_LABEL            (c, "popupfontsize",        0, "Popup Font Size");
CLASS_ATTR_LONG             (c, "popupfontface",        0, t_tralala, popupFontFace);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontface",        0, DEFAULT_POPUP_FONTFACE);
CLASS_ATTR_ENUMINDEX        (c, "popupfontface",        0, FONTSTYLE_LIST);
CLASS_ATTR_LABEL            (c, "popupfontface",        0, "Popup Font Style");

CLASS_STICKY_ATTR_CLEAR     (c, "category");

CLASS_ATTR_ORDER            (c, "focusedtextcolor",                 0, "1");
CLASS_ATTR_ORDER            (c, "focusedbordercolor",               0, "2");
CLASS_ATTR_ORDER            (c, "focuseduserselectednotecolor",     0, "3");
CLASS_ATTR_ORDER            (c, "focusedusermarkednotecolor",       0, "4");
CLASS_ATTR_ORDER            (c, "focusedusersilentnotecolor",       0, "5");
CLASS_ATTR_ORDER            (c, "focusedlivenotecolor",             0, "6");
CLASS_ATTR_ORDER            (c, "focusedliveplayednotecolor",       0, "7");
CLASS_ATTR_ORDER            (c, "focusedlistennotecolor",           0, "8");
CLASS_ATTR_ORDER            (c, "focuseduserzonecolor",             0, "9");
CLASS_ATTR_ORDER            (c, "focusedlivezonecolor",             0, "10");
CLASS_ATTR_ORDER            (c, "focusedselectedzonecolor",         0, "11");
CLASS_ATTR_ORDER            (c, "unfocusedtextcolor",               0, "12");
CLASS_ATTR_ORDER            (c, "unfocusedbordercolor",             0, "13");
CLASS_ATTR_ORDER            (c, "unfocusedusernotecolor",           0, "14");
CLASS_ATTR_ORDER            (c, "unfocuseduserselectednotecolor",   0, "15");
CLASS_ATTR_ORDER            (c, "unfocusedlivenotecolor",           0, "16");
CLASS_ATTR_ORDER            (c, "unfocusedliveplayednotecolor",     0, "17");
CLASS_ATTR_ORDER            (c, "unfocusedlistennotecolor",         0, "18");
CLASS_ATTR_ORDER            (c, "unfocusedzonecolor",               0, "19");
CLASS_ATTR_ORDER            (c, "selectedtextcolor",                0, "20");
CLASS_ATTR_ORDER            (c, "popuptextcolor",                   0, "21");
CLASS_ATTR_ORDER            (c, "popubgcolor",                      0, "22");
CLASS_ATTR_ORDER            (c, "popuphlttextcolor",                0, "23");
CLASS_ATTR_ORDER            (c, "popuhltbgcolor",                   0, "24");

tll_thirtySecond            [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16A");
tll_thirtySecond            [1] = jgraphics_image_surface_create_from_resource (moduleRef, "32B");
tll_thirtySecond            [2] = jgraphics_image_surface_create_from_resource (moduleRef, "32C");

tll_sixteenth               [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16A");
tll_sixteenth               [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16B");
tll_sixteenth               [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16C");
tll_sixteenthTriplet        [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16TA");
tll_sixteenthTriplet        [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16TB");
tll_sixteenthTriplet        [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16TC");
tll_sixteenthDotted         [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16DA");
tll_sixteenthDotted         [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16DB");
tll_sixteenthDotted         [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16DC");

tll_eighth                  [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8A");
tll_eighth                  [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8B");
tll_eighth                  [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8C");
tll_eighthTriplet           [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8TA");
tll_eighthTriplet           [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8TB");
tll_eighthTriplet           [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8TC");
tll_eighthDotted            [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8DA");
tll_eighthDotted            [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8DB");
tll_eighthDotted            [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8DC");

tll_quarter                 [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4A");
tll_quarter                 [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4B");
tll_quarter                 [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4C");
tll_quarterTriplet          [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4TA");
tll_quarterTriplet          [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4TB");
tll_quarterTriplet          [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4TC");
tll_quarterDotted           [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4DA");
tll_quarterDotted           [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4DB");
tll_quarterDotted           [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4DC");

tll_half                    [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2A");
tll_half                    [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2B");
tll_half                    [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2C");
tll_halfTriplet             [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2TA");
tll_halfTriplet             [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2TB");
tll_halfTriplet             [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2TC");
tll_halfDotted              [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2DA");
tll_halfDotted              [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2DB");
tll_halfDotted              [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2DC");

tll_whole                   [0] = jgraphics_image_surface_create_from_resource (moduleRef, "1A");
tll_whole                   [1] = jgraphics_image_surface_create_from_resource (moduleRef, "1B");
tll_whole                   [2] = jgraphics_image_surface_create_from_resource (moduleRef, "1C");

tll_clipboard = pizGrowingArrayNew (1);

class_register (CLASS_BOX, c);

tll_class = c;

return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new (t_symbol *s, long argc, t_atom *argv)
{
    t_tralala           *x = NULL;
    t_dictionary        *d = NULL;
    long                boxflags;

    /*post ("%ld", calcoffset (t_tralala, channel));
    post ("%ld", calcoffset (t_tralala, arrayMutex));
    post ("%ld", calcoffset (t_tralala, popupFontName));
    post ("%ld", calcoffset (t_tralala, patternCustom));
    post ("%ld", calcoffset (t_tralala, mousePitchValue));
    post ("%ld", calcoffset (t_tralala, playedNotesCopy));
    post ("%ld", calcoffset (t_tralala, previous));
    post ("%ld", calcoffset (t_tralala, textLayers));*/

    if (d = object_dictionaryarg (argc, argv))
        {
            if (x = (t_tralala *)object_alloc (tll_class))
                {
                    long i;
                    long initArrayPool[2] = {2, INIT_GROWING_ARRAY_SIZE};
                    
                    boxflags = 0 
                        | JBOX_DRAWFIRSTIN 
                        | JBOX_DRAWINLAST
                        | JBOX_GROWBOTH
                        | JBOX_DRAWBACKGROUND
                        | JBOX_HILITE 
                        ;

                    jbox_new ((t_jbox *)x, boxflags, argc, argv);
                    x->box.b_firstin = (void *)x;
                    
                    x->rightOutlet          = outlet_new (x, NULL);
                    x->middleRightOutlet    = bangout ((t_object *)x);
                    x->middleLeftOutlet     = listout ((t_object *)x);
                    x->leftOutlet           = listout ((t_object *)x);
                    
                    object_obex_store ((void *)x, tll_sym_dumpout, (t_object *)x->rightOutlet);
                    
                    x->paintClock           = clock_new (x, (method)tralala_paintTask);
                    x->learnClock           = clock_new (x, (method)tralala_learnTask);
                    x->focusClock           = clock_new (x, (method)tralala_focusTask);
                    x->notifyClock          = clock_new (x, (method)tralala_notifyTask);
                    
                    x->runClock             = clock_new (x, (method)tralala_runTask);
                    x->goToStartClock       = clock_new (x, (method)tralala_goToStartTask);
                    x->inhibitStartClock    = clock_new (x, (method)tralala_inhibitStartTask);
                    x->inhibitBangClock     = clock_new (x, (method)tralala_inhibitBangTask);
                    
                    systhread_mutex_new (&x->methodMutex,       SYSTHREAD_MUTEX_NORMAL);
                    systhread_mutex_new (&x->algorithmMutex,    SYSTHREAD_MUTEX_NORMAL);
                    systhread_mutex_new (&x->learnMutex,        SYSTHREAD_MUTEX_NORMAL);
                    systhread_mutex_new (&x->arrayMutex,        SYSTHREAD_MUTEX_NORMAL);
                    
                    for (i = 0; i < TEXT_CELL_COUNT; i++) {
                            x->textLayers[i]     = jtextlayout_create ( );
                            x->textIsSelected[i] = false;
                        }
                    
                    jbox_ready ((t_jbox *)x);

                    x->user                     = pizSequenceNew        ( );
                    x->live                     = pizSequenceNew        ( );
                    x->listen                   = pizSequenceNew        ( );
                    x->arrayPool                = pizArrayPoolNew       (2, initArrayPool);
                    x->factorOracle             = pizFactorOracleNew    (0, NULL);
                    x->galoisLattice            = pizGaloisLatticeNew   (0, NULL);
                    x->finiteState              = pizFiniteStateNew     (0, NULL);
                    x->unselectedNotes          = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->selectedNotes            = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->playedNotes              = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->zone                     = pizGrowingArrayNew    (PIZ_SEQUENCE_ZONE_SIZE);
                    x->unselectedNotesCopy      = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->selectedNotesCopy        = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->playedNotesCopy          = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->zoneCopy                 = pizGrowingArrayNew    (PIZ_SEQUENCE_ZONE_SIZE);
                    x->originNotes              = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->result                   = pizGrowingArrayNew    (INIT_GROWING_ARRAY_SIZE);
                    x->valuesToBeLearned        = pizGrowingArrayNew    (LEARN_THRESHOLD_MAXIMUM);
                    x->learnQueue               = pizBoundedQueueNew    (LEARN_QUEUE_SIZE);
                    x->slots                    = pizLinklistNew        ( );
                    x->undo                     = pizLinklistNew        ( );
                    x->redo                     = pizLinklistNew        ( );
            
                    if (x->user && 
                        x->live && 
                        x->listen && 
                        x->arrayPool &&
                        x->factorOracle &&
                        x->galoisLattice &&
                        x->finiteState &&
                        x->unselectedNotes && 
                        x->selectedNotes && 
                        x->playedNotes &&
                        x->zone && 
                        x->unselectedNotesCopy && 
                        x->selectedNotesCopy && 
                        x->playedNotesCopy &&
                        x->zoneCopy && 
                        x->originNotes &&
                        x->result &&
                        x->valuesToBeLearned &&
                        x->learnQueue &&
                        x->slots &&
                        x->undo &&
                        x->redo)
                        {
                            attr_dictionary_process (x, d);

                            x->flags                = FLAG_NONE;
                            x->textMode             = TEXT_MODE_NOTE;
                            x->hitTest              = HIT_NOTHING;
                            x->cursorType           = JMOUSE_CURSOR_ARROW;
                            x->learnCycle           = PIZ_ALGORITHM_NONE;
                            x->learnThreshold       = LEARN_THRESHOLD_MINIMUM;
                            x->cell                 = PIZ_SNAP_NONE;
                            x->dirtyLayer           = (DIRTY_ZONE | DIRTY_NOTES | DIRTY_GRID | DIRTY_CHANGE);
                            
                            pizSequenceSetGrid (x->user, PIZ_EIGHTH_NOTE);
                            
                            pizLinklistSetFlags (x->slots,  PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY);
                            pizLinklistSetFlags (x->undo,   PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY);
                            pizLinklistSetFlags (x->redo,   PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY);
                            
                            if (dictionary_hasentry (d, tll_sym_tralala) && (x->saveSlotsWithPatcher || 
                                x->saveChannelWithPatcher || x->saveValuesWithPatcher))
                                {
                                    t_dictionary *data = NULL;
                                    
                                    dictionary_getdictionary (d, tll_sym_tralala, (t_object **)&data);
                                    
                                    if (data) {
                                            tralala_dataWithDictionary (x, data);
                                        }
                                }
                            
                            if (!pizLinklistCount (x->slots))
                                {
                                    PIZGrowingArray *firstSlot = NULL;

                                    if (firstSlot = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE)) {
                                            pizLinklistAppend (x->slots, firstSlot);
                                        }
                                }
                            
                            tralala_slotRecall (x, x->slotIndex);
                            
                            srand ((unsigned int)time(NULL));
                            
                            clock_fdelay (x->learnClock, LEARN_CLOCK_INTERVAL + CLOCK_RANDOMIZE 
                                * (rand ( ) / (RAND_MAX + 1.0)));
                        }
                    else
                        {
                            object_free (x);
                            x = NULL;
                        }
                }
        }
    
    return x;
}

void tralala_free (t_tralala *x)
{   
    long i;
        
    if (x->paintClock) {
            clock_unset (x->paintClock);
            object_free (x->paintClock);
        }
    
    if (x->learnClock) {
            clock_unset (x->learnClock);
            object_free (x->learnClock);
        }
        
    if (x->runClock) {
            clock_unset (x->runClock);
            object_free (x->runClock);
        }
    
    if (x->goToStartClock) {
            clock_unset (x->goToStartClock);
            object_free (x->goToStartClock);
        }
    
    if (x->inhibitStartClock) {
            clock_unset (x->inhibitStartClock);
            object_free (x->inhibitStartClock);
        }
    
    if (x->inhibitBangClock) {
            clock_unset (x->inhibitBangClock);
            object_free (x->inhibitBangClock);
        }
    
    if (x->focusClock) {
            clock_unset (x->focusClock);
            object_free (x->focusClock);
        }
    
    if (x->notifyClock) {
            clock_unset (x->notifyClock);
            object_free (x->notifyClock);
        }
    
    if (x->algorithmMutex) {
            systhread_mutex_free (x->methodMutex);
        }
        
    if (x->algorithmMutex) {
            systhread_mutex_free (x->algorithmMutex);
        }
        
    if (x->arrayMutex) {
            systhread_mutex_free (x->arrayMutex);
        }
    
    if (x->learnMutex) {
            systhread_mutex_free (x->learnMutex);
        }
    
    for (i = 0; i < TEXT_CELL_COUNT; i++)
        {
            if (x->textLayers[i]) {
                    jtextlayout_destroy (x->textLayers[i]);
                }
        }
    
    pizGrowingArrayFree (x->unselectedNotes);
    pizGrowingArrayFree (x->selectedNotes);
    pizGrowingArrayFree (x->playedNotes);
    pizGrowingArrayFree (x->zone);
    pizGrowingArrayFree (x->unselectedNotesCopy);
    pizGrowingArrayFree (x->selectedNotesCopy);
    pizGrowingArrayFree (x->playedNotesCopy);
    pizGrowingArrayFree (x->zoneCopy);
    pizGrowingArrayFree (x->originNotes);
    pizGrowingArrayFree (x->result);
    pizGrowingArrayFree (x->valuesToBeLearned);
    
    pizLinklistFree         (x->slots);
    pizLinklistFree         (x->undo);
    pizLinklistFree         (x->redo);
    
    pizBoundedQueueFree     (x->learnQueue);
    pizFactorOracleFree     (x->factorOracle);
    pizGaloisLatticeFree    (x->galoisLattice);
    pizFiniteStateFree      (x->finiteState);
    pizArrayPoolFree        (x->arrayPool);
    
    pizSequenceFree         (x->user);
    pizSequenceFree         (x->listen);
    pizSequenceFree         (x->live);
        
    jbox_free ((t_jbox *)x);
}

void tralala_assist (t_tralala *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) 
        { 
            sprintf (s, "Messages");
        } 
    else 
        {   
            switch (a) {
                case 0  : sprintf (s, "(list) Played Notes"); break;    
                case 1  : sprintf (s, "(list) Dumped Notes"); break;
                case 2  : sprintf (s, "(bang) End Of Loop"); break;
                case 3  : sprintf (s, "(anything) Dumpout"); break;
                }
        }
}

void tralala_jsave (t_tralala *x, t_dictionary *d)
{
    t_dictionary *data = NULL;

    if ((x->saveSlotsWithPatcher || x->saveChannelWithPatcher || x->saveValuesWithPatcher) 
        && d && (data = dictionary_new ()))
        {
            tralala_slotStore (x);
            tralala_dataToDictionary (x, data);
            
            dictionary_appenddictionary (d, tll_sym_tralala, (t_object *)data); 
        }
}

t_max_err tralala_setvalueof (t_tralala *x, long argc, t_atom *argv)
{
    if (argc && argv) 
        {
            ARRAY_GET (tempArray);
            
            if (tempArray)
                {
                    long        i;
                    PIZError    err = PIZ_GOOD;
                    
                    for (i = 0; i < argc; i++) {    
                            err |= pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                        }
                    
                    if (!err) {
                            pizSequenceDecodeSlotWithArray (x->user, tempArray);
                            
                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
                            
                            DIRTYSLOTS
                        }
                    
                    ARRAY_RELEASE (tempArray);
                }
        }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_getvalueof (t_tralala *x, long *argc, t_atom **argv)
{
    if (argc && argv) 
        {
            ARRAY_GET (tempArray);
            
            if (tempArray)
                {
                    if (!pizSequenceEncodeSlotToArray (x->user, tempArray))
                        {
                            long size = pizGrowingArrayCount (tempArray);
                            
                            if (*argc && *argv) {
                                ;
                            } else {
                                *argc = size;
                                *argv = (t_atom *)getbytes(sizeof(t_atom) * size);
                            }
                    
                            if (*argv && (*argc >= size))
                                {
                                    long *ptr = pizGrowingArrayPtr (tempArray);
                                    
                                    atom_setlong_array (*argc, *argv, size, ptr);
                                }
                        }
                    
                    ARRAY_RELEASE (tempArray);
                }
        }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err jbox_notify (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (msg == tll_sym_attr_modified) 
        {           
            t_symbol *attr_name = NULL;
            
            attr_name = (t_symbol *)object_method (data, tll_sym_getname);
                        
            if (attr_name) 
                {                   
                    if ((attr_name != tll_sym_patching_rect) && object_attr_usercanset (x, attr_name))
                        {
                            t_tralala *myObject = NULL;
                            
                            if (myObject = (t_tralala *)(jbox_get_object ((t_object *)x)))
                                {
                                    myObject->dirtyLayer |= (DIRTY_ZONE | DIRTY_NOTES);
                                }
                        }
                }           
        }
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_dataToDictionary (t_tralala *x, t_dictionary *d)
{
    if (d) 
        {
            if (ATOMIC_INCREMENT (&x->popupLock) == 1)
                {
                    dictionary_appendlong   (d, tll_sym_version, PIZ_SEQUENCE_VERSION);
                    dictionary_appendlong   (d, tll_sym_sequenceMode, x->sequenceMode);
                    dictionary_appendlong   (d, tll_sym_zoomMode, x->zoomMode);
                    dictionary_appendfloat  (d, tll_sym_windowOffsetX, x->windowOffsetX);
                    dictionary_appendfloat  (d, tll_sym_windowOffsetY, x->windowOffsetY); 
                    
                    if (x->saveSlotsWithPatcher)
                        {
                            long            index = 0;
                            PIZGrowingArray *slot = NULL;
                            PIZGrowingArray *nextSlot = NULL;
                            
                            pizLinklistPtrAtIndex (x->slots, 0, (void **)&slot);
                            
                            while (slot)
                                {
                                    pizLinklistNextByPtr (x->slots, (void *)slot, (void **)&nextSlot);
                                    
                                    t_atom  *storage = NULL;
                                    long    count = pizGrowingArrayCount (slot);
                                    
                                    if (storage = (t_atom *)sysmem_newptrclear (sizeof(t_atom) * count))
                                        {
                                            long *ptr = NULL;
                                            char key[STRING_MAXIMUM_SIZE];
                                            
                                            ptr = pizGrowingArrayPtr (slot);
                                            atom_setlong_array (count, storage, count, ptr);
                                            
                                            snprintf (key, STRING_MAXIMUM_SIZE, "slot %ld", index);
                                            key[STRING_MAXIMUM_SIZE - 1] = 0;
                                            
                                            dictionary_appendatoms (d, gensym (key), count, storage);
                                            
                                            sysmem_freeptr (storage);
                                        }
                                    
                                    slot = nextSlot;
                                    index ++;
                                }
                            
                            dictionary_appendlong (d, tll_sym_slots, index);
                        }   
                        
                    if (x->saveChannelWithPatcher)
                        {
                            dictionary_appendlong (d, tll_sym_channel, x->channel);
                        }
                    
                    if (x->saveValuesWithPatcher)
                        {
                            char alloc1, alloc2;
                            long argc1 = 0;
                            long argc2 = 0;
                            t_atom *argv1 = NULL;
                            t_atom *argv2 = NULL;
                            
                            dictionary_appendlong   (d, tll_sym_tempo, x->tempo);
                            dictionary_appendlong   (d, tll_sym_chance, x->chance);
                            dictionary_appendsym    (d, tll_sym_scaleKey, x->scaleKey);
                            dictionary_appendsym    (d, tll_sym_scaleType, x->scaleType);
                            dictionary_appendsym    (d, tll_sym_patternCell, x->patternCell);
                            dictionary_appendlong   (d, tll_sym_velocity, x->velocity);
                            
                            if ((atom_alloc_array (PIZ_MAGIC_SCALE, &argc1, &argv1, &alloc1)) == MAX_ERR_NONE) {
                                atom_setlong_array (argc1, argv1, PIZ_MAGIC_SCALE, x->scaleCustom);
                                dictionary_appendatoms (d, tll_sym_scaleCustom, argc1, argv1);
                                sysmem_freeptr (argv1);
                            }
                            
                            if ((atom_alloc_array (x->patternSize, &argc2, &argv2, &alloc2)) == MAX_ERR_NONE) {
                                atom_setlong_array (argc2, argv2, x->patternSize, x->patternCustom);
                                dictionary_appendatoms (d, tll_sym_patternCustom, argc2, argv2);
                                sysmem_freeptr (argv2);
                            }
                            
                        }
                }

            ATOMIC_DECREMENT (&x->popupLock);
        }
}

void tralala_dataWithDictionary (t_tralala *x, t_dictionary *d)
{
    if (d) 
        {
            if (ATOMIC_INCREMENT (&x->popupLock) == 1)
                {
                    long sequenceMode = -1;
                    long channel = -1;
                    
                    dictionary_getlong  (d, tll_sym_zoomMode, &x->zoomMode);
                    dictionary_getfloat (d, tll_sym_windowOffsetX, &x->windowOffsetX);
                    dictionary_getfloat (d, tll_sym_windowOffsetY, &x->windowOffsetY);
                    
                    dictionary_getlong  (d, tll_sym_sequenceMode, &sequenceMode);
                    
                    if (x->saveChannelWithPatcher) {
                            dictionary_getlong (d, tll_sym_channel, &channel);
                        }
                        
                    if (x->saveSlotsWithPatcher)
                        {
                            long i;
                            long count = 0;
                            
                            dictionary_getlong (d, tll_sym_slots, &count);
                            
                            for (i = 0; i < count; i++)
                                {
                                    long    argc = 0;
                                    t_atom  *argv = NULL;
                                    char    key[STRING_MAXIMUM_SIZE];
                    
                                    snprintf (key, STRING_MAXIMUM_SIZE, "slot %ld", i);
                                    key[STRING_MAXIMUM_SIZE - 1] = 0;
                                    
                                    dictionary_getatoms (d, gensym (key), &argc, &argv);
                                    
                                    if (argv) 
                                        {
                                            long j;
                                            PIZGrowingArray *slot = NULL;
                                            
                                            if (slot = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE))  
                                                {
                                                    for (j = 0; j < argc; j++) {
                                                        pizGrowingArrayAppend (slot, atom_getlong (argv + j));
                                                    }
                                                    
                                                    pizLinklistAppend (x->slots, slot);
                                                }
                                        }
                                }
                        }
                    
                    if (x->saveValuesWithPatcher)
                        {
                            long        argc1 = 0;
                            long        argc2 = 0;
                            long        velocity = 0;
                            long        chance = -1;
                            t_symbol    *scaleKey = NULL;
                            t_symbol    *scaleType = NULL;
                            t_symbol    *patternCell = NULL;
                            t_atom      *argv1 = NULL;
                            t_atom      *argv2 = NULL;
                            
                            dictionary_getlong      (d, tll_sym_tempo, &x->tempo);
                            dictionary_getlong      (d, tll_sym_chance, &chance);
                            dictionary_getlong      (d, tll_sym_velocity, &velocity);
                            dictionary_getsym       (d, tll_sym_scaleKey, &scaleKey);
                            dictionary_getsym       (d, tll_sym_scaleType, &scaleType);
                            dictionary_getsym       (d, tll_sym_patternCell, &patternCell);
                            dictionary_copyatoms    (d, tll_sym_scaleCustom, &argc1, &argv1);
                            dictionary_copyatoms    (d, tll_sym_patternCustom, &argc2, &argv2);
                            
                            if ((chance > 0) && chance != x->chance) {
                                    object_attr_setlong (x, tll_sym_chance, chance);
                                }
                            if (velocity != x->velocity) {
                                    object_attr_setlong (x, tll_sym_velocity, velocity);
                                }
                            if (scaleKey) {
                                    object_attr_setsym (x, tll_sym_scaleKey, scaleKey);
                                }
                            if (scaleType) {
                                    object_attr_setsym (x, tll_sym_scaleType, scaleType);
                                }
                            if (patternCell) {
                                    object_attr_setsym (x, tll_sym_patternCell, patternCell);
                                }
                            if (argc1) {
                                    object_attr_setvalueof (x, tll_sym_scaleCustom, argc1, argv1);
                                }
                            if (argc2) {
                                    object_attr_setvalueof (x, tll_sym_patternCustom, argc2, argv2);
                                }
                                
                            if (argv1) {
                                    sysmem_freeptr (argv1);
                                }
                            if (argv2) {
                                    sysmem_freeptr (argv2);
                                }
                        }
                    
                    ATOMIC_DECREMENT (&x->popupLock);
                    
                    if ((sequenceMode >= 0) && sequenceMode != x->sequenceMode) {
                            object_attr_setlong (x, tll_sym_sequenceMode, sequenceMode);
                        }
                        
                    if ((channel > 0) && channel != x->channel) {
                            object_attr_setlong (x, tll_sym_channel, channel);
                        }
                }
            else
                {
                    ATOMIC_DECREMENT (&x->popupLock);
                }
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err tralala_setSequenceMode (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            long k = atom_getlong (argv);
                    
            tralala_willChange (x);
                    
            x->sequenceMode = CLAMP (k, SEQUENCE_MODE_USER, SEQUENCE_MODE_LISTEN);

            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE | DIRTY_GRID);
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setChannel (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            if (ATOMIC_INCREMENT (&x->popupLock) == 1)
                {
                    x->channel = atom_getlong (argv);

                    pizSequenceSetChannel (x->user, x->channel);
                    pizSequenceSetChannel (x->live, x->channel);
                    pizSequenceSetChannel (x->listen, x->channel);
                    
                    DIRTYCHANNEL

                }

            ATOMIC_DECREMENT (&x->popupLock);
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setVelocity (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            long tempVelocity = atom_getlong (argv);
            
            if (x->velocity != tempVelocity) {
                    x->velocity = tempVelocity;
                    pizSequenceSetVelocity (x->live, x->velocity);
                }
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setChance (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            long tempChance = atom_getlong (argv);
            
            if (x->chance != tempChance) {
                    x->chance = tempChance;
                    pizSequenceSetChance (x->user, x->chance);
                    pizSequenceSetChance (x->live, x->chance);
                    pizSequenceSetChance (x->listen, x->chance);
                }
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleKey (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            bool     err = false;
            t_symbol *temp = atom_getsym (argv);
            
            if (temp != x->scaleKey)
                {
                    if (temp == tll_sym_c) {
                        x->key = PIZ_KEY_C;
                    } else if (temp == tll_sym_cSharp) {
                        x->key = PIZ_KEY_C_SHARP;
                    } else if (temp == tll_sym_d) {
                        x->key = PIZ_KEY_D;
                    } else if (temp == tll_sym_dSharp) {
                        x->key = PIZ_KEY_D_SHARP;
                    } else if (temp == tll_sym_e) {
                        x->key = PIZ_KEY_E;
                    } else if (temp == tll_sym_f) {
                        x->key = PIZ_KEY_F;
                    } else if (temp == tll_sym_fSharp) {
                        x->key = PIZ_KEY_F_SHARP;
                    } else if (temp == tll_sym_g) {
                        x->key = PIZ_KEY_G;
                    } else if (temp == tll_sym_gSharp) {
                        x->key = PIZ_KEY_G_SHARP;
                    } else if (temp == tll_sym_a) {
                        x->key = PIZ_KEY_A;
                    } else if (temp == tll_sym_aSharp) {
                        x->key = PIZ_KEY_A_SHARP;
                    } else if (temp == tll_sym_b) {
                        x->key = PIZ_KEY_B;
                    } else { err = true; }

                    if (!err)
                        {   
                            x->scaleKey = temp;
                            
                            if (x->type != PIZ_SCALE_CUSTOM)
                                {
                                    pizSequenceSetScale (x->live, x->key, x->type, NULL);
                                }
                            else
                                {
                                    ARRAY_GET (tempArray);
                                    
                                    if (tempArray)
                                        {
                                            long i;
                                            
                                            for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                                                    pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                                }
                                                
                                            pizSequenceSetScale (x->live, x->key, x->type, tempArray);
                                            
                                            ARRAY_RELEASE (tempArray);
                                        }
                                }
                            
                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                        }
                }
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleType (t_tralala *x, t_object *attr, long argc, t_atom *argv) 
{
    if (argc && argv)
        {
            long        size = 0;
            char        *tempString = NULL;
            t_symbol    *temp = NULL;
            bool        err = false;            

            atom_gettext (argc, argv, &size, &tempString, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
            
            if (tempString)
                {
                    temp = gensym (tempString);
                    
                    if (temp != x->scaleType)
                        {
                            if (temp == tll_sym_none) {
                                x->type = PIZ_SCALE_NONE;
                            } else if (temp == tll_sym_custom) {
                                x->type = PIZ_SCALE_CUSTOM;
                            } else if (temp == tll_sym_ionian) {
                                x->type = PIZ_IONIAN;
                            } else if (temp == tll_sym_dorian) {
                                x->type = PIZ_DORIAN;
                            } else if (temp == tll_sym_phrygian) {
                                x->type = PIZ_PHRYGIAN;
                            } else if (temp == tll_sym_lydian) {
                                x->type = PIZ_LYDIAN;
                            } else if (temp == tll_sym_mixolydian) {
                                x->type = PIZ_MIXOLYDIAN;
                            } else if (temp == tll_sym_aeolian) {
                                x->type = PIZ_AEOLIAN;
                            } else if (temp == tll_sym_locrian) {
                                x->type = PIZ_LOCRIAN;
                            } else if (temp == tll_sym_natural) {
                                x->type = PIZ_NATURAL;
                            } else if (temp == tll_sym_harmonic) {
                                x->type = PIZ_HARMONIC;
                            } else if (temp == tll_sym_melodic) {
                                x->type = PIZ_MELODIC;
                            } else if (temp == tll_sym_wholeTone) {
                                x->type = PIZ_WHOLE_TONE;
                            } else if (temp == tll_sym_pentatonicMajor) {
                                x->type = PIZ_PENTATONIC_MAJOR;
                            } else if (temp == tll_sym_pentatonicMinor) {
                                x->type = PIZ_PENTATONIC_MINOR;
                            } else if (temp == tll_sym_octatonicHalfWhole) {
                                x->type = PIZ_OCTATONIC_HALF_WHOLE;
                            } else if (temp == tll_sym_octatonicWholeHalf) {
                                x->type = PIZ_OCTATONIC_WHOLE_HALF;
                            } else if (temp == tll_sym_seventhMajor) {
                                x->type = PIZ_SEVENTH_MAJOR;
                            } else if (temp == tll_sym_seventhDominant) {
                                x->type = PIZ_SEVENTH_DOMINANT;
                            } else if (temp == tll_sym_seventhMinor) {
                                x->type = PIZ_SEVENTH_MINOR;
                            } else if (temp == tll_sym_seventhMajorSixth) {
                                x->type = PIZ_SEVENTH_MAJOR_SIXTH;
                            } else if (temp == tll_sym_seventhMinorSixth) {
                                x->type = PIZ_SEVENTH_MINOR_SIXTH;
                            } else if (temp == tll_sym_seventhHalfDiminished) {
                                x->type = PIZ_SEVENTH_HALF_DIMINISHED;
                            } else if (temp == tll_sym_seventhDiminished) {
                                x->type = PIZ_SEVENTH_DIMINISHED;
                            } else if (temp == tll_sym_seventhSuspended) {
                                x->type = PIZ_SEVENTH_SUSPENDED;
                            } else if (temp == tll_sym_seventhSharpFive) {
                                x->type = PIZ_SEVENTH_SHARP_FIVE;
                            } else if (temp == tll_sym_seventhFlatFive) {
                                x->type = PIZ_SEVENTH_FLAT_FIVE;
                            } else { err = true; }
                            
                            if (!err)
                                {   
                                    x->scaleType = temp;
                                    
                                    if (x->type != PIZ_SCALE_CUSTOM) 
                                        {
                                            pizSequenceSetScale (x->live, x->key, x->type, NULL);
                                        } 
                                    else 
                                        {
                                            ARRAY_GET (tempArray);
                                            
                                            if (tempArray) 
                                                {
                                                    long i;
                                                    
                                                    for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                                                        pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                                    }
                                                    
                                                    pizSequenceSetScale (x->live, x->key, x->type, tempArray);
                                                    
                                                    ARRAY_RELEASE (tempArray);
                                                }
                                        }
                                    
                                    if (LIVE) {
                                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                                        }
                                }
                        }
                    
                    sysmem_freeptr (tempString);
                }
        }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleCustom (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if ((argc == PIZ_MAGIC_SCALE) && argv)
        {   
            atom_getlong_array (argc, argv, PIZ_MAGIC_SCALE, x->scaleCustom);
            
            if (x->type == PIZ_SCALE_CUSTOM)
                {
                    ARRAY_GET (tempArray);
                            
                    if (tempArray)
                        {
                            long i;
                            
                            for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                                    pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                }
                                
                            pizSequenceSetScale (x->live, x->key, x->type, tempArray);
                            
                            ARRAY_RELEASE (tempArray);
                        }
                    
                    if (LIVE) {
                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                        }
                }
        }

    return MAX_ERR_NONE;
}

t_max_err tralala_setPatternCell (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            long        size = 0;
            char        *tempString = NULL;
            t_symbol    *temp = NULL;
            bool        err = false;            

            atom_gettext (argc, argv, &size, &tempString, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
            
            if (tempString)
                {
                    temp = gensym (tempString);
            
                    if (temp != x->patternCell)
                        {
                            if (temp == tll_sym_none) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_SNAP_NONE);
                            } else if (temp == tll_sym_whole) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_WHOLE_NOTE);
                            } else if (temp == tll_sym_half) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_HALF_NOTE);
                            } else if (temp == tll_sym_quarter) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_QUARTER_NOTE);
                            } else if (temp == tll_sym_eighth) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_EIGHTH_NOTE);
                            } else if (temp == tll_sym_sixteenth) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_SIXTEENTH_NOTE);
                            } else if (temp == tll_sym_thirtySecond) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_THIRTY_SECOND_NOTE);
                            } else if (temp == tll_sym_wholeTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_WHOLE_NOTE_TRIPLET);
                            } else if (temp == tll_sym_halfTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_HALF_NOTE_TRIPLET);
                            } else if (temp == tll_sym_quarterTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_QUARTER_NOTE_TRIPLET);
                            } else if (temp == tll_sym_eighthTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_EIGHTH_NOTE_TRIPLET);
                            } else if (temp == tll_sym_sixteenthTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_SIXTEENTH_NOTE_TRIPLET);
                            } else if (temp == tll_sym_thirtySecondTriplet) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_THIRTY_SECOND_NOTE_TRIPLET);
                            } else if (temp == tll_sym_wholeDotted) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_WHOLE_NOTE_DOTTED);
                            } else if (temp == tll_sym_halfDotted) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_HALF_NOTE_DOTTED);
                            } else if (temp == tll_sym_quarterDotted) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_QUARTER_NOTE_DOTTED);
                            } else if (temp == tll_sym_eighthDotted) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_EIGHTH_NOTE_DOTTED);
                            } else if (temp == tll_sym_sixteenthDotted) {
                                pizSequenceSetGrid (x->live, x->cell = PIZ_SIXTEENTH_NOTE_DOTTED);
                            } else { err = true; }
                            
                            if (!err)
                                {
                                    x->patternCell = temp;
                                    
                                    if (pizSequenceApplyPattern (x->live) && LIVE) {
                                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                                        }
                                    
                                    DIRTYLAYER_SET (DIRTY_GRID);
                                }
                        }
                    
                    sysmem_freeptr (tempString);
                }
        }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_setPatternCustom (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argv)
        {   
            ARRAY_GET (tempArray);

            x->patternSize = MIN (argc, PATTERN_MAXIMUM_SIZE);
            
            atom_getlong_array (argc, argv, x->patternSize, x->patternCustom);
                            
            if (tempArray)
                {
                    long i;
                    
                    for (i = 0; i < x->patternSize; i++) {
                            pizGrowingArrayAppend (tempArray, x->patternCustom[i]);
                        }
                        
                    pizSequenceSetPattern (x->live, tempArray);
                    
                    if (x->patternCell != tll_sym_none)
                        {
                            if (pizSequenceApplyPattern (x->live) && LIVE) {
                                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                                }
                        }
                    
                    ARRAY_RELEASE (tempArray);
                }
        }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark PLAY
#pragma mark -

void tralala_play (t_tralala *x)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    x->flags &= ~FLAG_INHIBIT_START;
    
    clock_unset (x->goToStartClock);
    
    if (pizSequenceIsAtEnd (x->live) && !(x->flags & FLAG_IS_LOOPED))
        {
            x->flags &= ~FLAG_IS_RUNNING;
            
            pizSequenceGoToStart (x->live);
            
            if (!(x->flags & FLAG_INHIBIT_BANG)) {
                    outlet_bang (x->middleRightOutlet);
                }
        }
        
    if (!(x->flags & FLAG_IS_RUNNING) && !(x->flags & FLAG_INHIBIT_START))
        {
            pizSequenceGoToStart (x->live);
            x->runIndex = pizSequenceIndex (x->live);
            
            x->flags |= FLAG_IS_RUNNING;
            x->flags &= ~(FLAG_IS_LOOPED | FLAG_IS_PAUSED);

            clock_fdelay (x->runClock, 0.);
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_stop (t_tralala *x)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    clock_unset (x->runClock);
    
    x->flags &= ~(FLAG_IS_LOOPED | FLAG_IS_RUNNING | FLAG_IS_PAUSED);
    x->runIndex = -1;
    
    if (pizSequenceIsAtEnd (x->live)) {
            clock_fdelay (x->goToStartClock, DEFER_CLOCK_INTERVAL);
        }
    
    x->flags |= FLAG_INHIBIT_START;
    
    clock_fdelay (x->inhibitStartClock, DEFER_CLOCK_INTERVAL);
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_loop (t_tralala *x)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    if (!(x->flags & FLAG_IS_RUNNING))
        {
            clock_unset (x->goToStartClock);
                
            pizSequenceGoToStart (x->live);
            x->runIndex = pizSequenceIndex (x->live);
            
            x->flags |= (FLAG_IS_LOOPED | FLAG_IS_RUNNING);
            x->flags &= ~FLAG_IS_PAUSED;

            clock_fdelay (x->runClock, 0.);
        }
    else
        {
            x->flags |= FLAG_IS_LOOPED;
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_unloop (t_tralala *x)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    x->flags &= ~FLAG_IS_LOOPED;
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_pause (t_tralala *x)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    if ((x->flags & FLAG_IS_RUNNING) && !(x->flags & FLAG_IS_PAUSED))
        {
            x->flags |= FLAG_IS_PAUSED;
            x->flags &= ~FLAG_IS_RUNNING;
            
            clock_unset (x->runClock);
        }
    else
        {
            x->flags &= ~FLAG_IS_PAUSED;
            x->flags |= FLAG_IS_RUNNING;
            
            clock_fdelay (x->runClock, 0.);
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_runTask (t_tralala *x)
{
    PIZError err;
    long     temp;

    if ((pizSequenceIsAtEnd (x->live)) && (x->flags & FLAG_IS_LOOPED))
        {
            outlet_bang (x->middleRightOutlet);
            pizSequenceGoToStart (x->live);
        }
        
    temp = pizSequenceIndex (x->live);
    err  = pizSequenceProceedStep (x->live, x->result);
    
    if (!err)
        {
            long i;
            long count;
            
            if (!(x->flags & FLAG_IS_MUTED) && ((count = pizGrowingArrayCount (x->result) / 4)))
                {
                    for (i = 0; i < count; i++) {
                        long duration = (pizGrowingArrayValueAtIndex (x->result, (i * 4) + 2)) 
                                        * (STEPS_PER_MINUTE / (double)x->tempo);
                            
                        atom_setlong (x->playedNote,     pizGrowingArrayValueAtIndex (x->result, i * 4));
                        atom_setlong (x->playedNote + 1, pizGrowingArrayValueAtIndex (x->result, (i * 4) + 1));
                        atom_setlong (x->playedNote + 2, duration);
                        atom_setlong (x->playedNote + 3, pizGrowingArrayValueAtIndex (x->result, (i * 4) + 3));
                        
                        outlet_list  (x->leftOutlet, NULL, 4, x->playedNote);
                    }
                }
                                
            pizGrowingArrayClear (x->result);

            if (x->flags & FLAG_IS_RUNNING) {
                    clock_fdelay (x->runClock, STEPS_PER_MINUTE / (double)x->tempo);
                }
        }
    else
        {
            temp = -1;
            
            x->flags &= ~FLAG_IS_RUNNING;
            x->flags |= FLAG_INHIBIT_BANG;
            
            clock_fdelay (x->goToStartClock, DEFER_CLOCK_INTERVAL);
            clock_fdelay (x->inhibitBangClock, DEFER_CLOCK_INTERVAL);
            
            outlet_bang (x->middleRightOutlet);
        }

    x->runIndex = temp;
}

void tralala_goToStartTask (t_tralala *x)
{
    pizSequenceGoToStart (x->live);
}

void tralala_inhibitStartTask (t_tralala *x)
{
    x->flags &= ~FLAG_INHIBIT_START;
}

void tralala_inhibitBangTask (t_tralala *x)
{
    x->flags &= ~FLAG_INHIBIT_BANG;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark MESSAGES
#pragma mark -

void tralala_copy (t_tralala *x)
{
    PIZError err = PIZ_GOOD;
    
    ARRAY_GET (tempArrayA);
    ARRAY_GET (tempArrayB);
    
    if (tempArrayA && tempArrayB) 
        {
            err |= pizSequenceNotesToArray (x->listen, tempArrayA, tempArrayA);
        }
    else
        {
            err |= PIZ_MEMORY;
        }
    
    if (!err)
        {
            long i;
            long chance = object_attr_getlong (x, tll_sym_chance);
            long count = pizGrowingArrayCount (tempArrayA) / PIZ_SEQUENCE_NOTE_SIZE;
            long *ptr = pizGrowingArrayPtr (tempArrayA);
            
            pizGrowingArrayClear (tempArrayB);

            for (i = (count - 1); i >= 0; i--) {
                if (100 * (rand ( ) / (RAND_MAX + 1.0)) < chance) {
                        pizGrowingArrayAppendPtr (tempArrayB, PIZ_SEQUENCE_NOTE_SIZE, 
                            ptr + (PIZ_SEQUENCE_NOTE_SIZE * i));
                    }
                }
            
            pizSequenceAddNotesWithArray (x->live, tempArrayB, PIZ_SEQUENCE_ADD_FLAG_PATTERN);
        }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
    
    if (LIVE)
        {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
        }
}

void tralala_enter (t_tralala *x)
{
    tralala_setLiveByUser (x);
    
    if (LIVE) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE);
        }
}

void tralala_setLiveByUser (t_tralala *x)
{
    PIZError err = PIZ_GOOD;
    
    ARRAY_GET (tempArrayA);
    ARRAY_GET (tempArrayB);
    
    if (tempArrayA && tempArrayB) 
        {
            err |= pizSequenceNotesToArray (x->user, tempArrayA, tempArrayA);
            pizSequenceZoneToArray (x->user, tempArrayB);
        }
    else
        {
            err |= PIZ_MEMORY;
        }
    
    if (!err)
        {
            pizSequenceAddNotesWithArray (x->live, tempArrayA, 
                PIZ_SEQUENCE_ADD_FLAG_PATTERN | PIZ_SEQUENCE_ADD_FLAG_CLEAR);
            
            pizSequenceSetZoneWithArray (x->live, tempArrayB);
            pizSequenceSetZoneWithArray (x->listen, tempArrayB);
            
            if (pizSequenceApplyAmbitus (x->listen) && LISTEN) {
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                }
        }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_int (t_tralala *x, long n)
{
    if (n >= 0 && n <= PIZ_MAGIC_PITCH)
        {
            systhread_mutex_lock (&x->learnMutex);
            pizBoundedQueueAppend (x->learnQueue, n);
            systhread_mutex_unlock (&x->learnMutex);
        }
}

void tralala_list (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && argv && (atom_gettype (argv) == A_LONG))
        {
            long n = atom_getlong (argv);
            
            if (n >= 0 && n <= PIZ_MAGIC_PITCH)
                {
                    systhread_mutex_lock (&x->learnMutex);
                    pizBoundedQueueAppend (x->learnQueue, n);
                    systhread_mutex_unlock (&x->learnMutex);
                }
        }
}

void tralala_mute (t_tralala *x, long n)
{   
    systhread_mutex_lock (&x->methodMutex);
    
    if (n) 
        {
            x->flags |= FLAG_IS_MUTED;
        } 
    else
        {
            x->flags &= ~FLAG_IS_MUTED;
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_forget (t_tralala *x)
{
    systhread_mutex_lock (&x->algorithmMutex);
    
    pizFactorOracleClear (x->factorOracle);
    pizGaloisLatticeClear (x->galoisLattice);
    pizFiniteStateClear (x->finiteState);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_learnTask (t_tralala *x)
{
    if (x->learnCycle == PIZ_ALGORITHM_NONE)
        {
            long i = x->learnThreshold;
            
            systhread_mutex_lock (&x->learnMutex);
            
            if (pizBoundedQueueCount (x->learnQueue) >= i)
                {
                    long err = PIZ_GOOD;
                    
                    pizGrowingArrayClear (x->valuesToBeLearned);
                    
                    while (i && !err)
                        {
                            if (!(err |= pizBoundedQueuePop (x->learnQueue)))
                                {
                                    err |= pizGrowingArrayAppend 
                                        (x->valuesToBeLearned, pizBoundedQueuePoppedValue (x->learnQueue));
                                }
                                
                            i --;
                        }
                    
                    x->learnCycle = PIZ_FACTOR_ORACLE;
                    x->learnThreshold = LEARN_THRESHOLD_MINIMUM 
                        + ((LEARN_THRESHOLD_RANGE + 1) * (rand ( ) / (RAND_MAX + 1.0)));
                }
                
            systhread_mutex_unlock (&x->learnMutex);
        }
    else 
        {
            long k = pizGrowingArrayCount (x->valuesToBeLearned);
            long *values = pizGrowingArrayPtr (x->valuesToBeLearned);
            
            switch (x->learnCycle) {
                case PIZ_FACTOR_ORACLE  :   systhread_mutex_lock (&x->algorithmMutex);
                                            pizFactorOracleAdd (x->factorOracle, k, values);
                                            systhread_mutex_unlock (&x->algorithmMutex);
                                            x->learnCycle = PIZ_GALOIS_LATTICE; break;
                case PIZ_GALOIS_LATTICE :   systhread_mutex_lock (&x->algorithmMutex);
                                            pizGaloisLatticeAdd (x->galoisLattice, k, values);
                                            systhread_mutex_unlock (&x->algorithmMutex);
                                            x->learnCycle = PIZ_FINITE_STATE; break;
                case PIZ_FINITE_STATE   :   systhread_mutex_lock (&x->algorithmMutex);
                                            pizFiniteStateAdd (x->finiteState, k, values);
                                            systhread_mutex_unlock (&x->algorithmMutex);
                                            x->learnCycle = PIZ_ALGORITHM_NONE; break;
                }
        }
    
    clock_fdelay (x->learnClock, LEARN_CLOCK_INTERVAL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_anything (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    char            alloc;
    long            size = 0;
    t_atom          *atoms = NULL;
    
    if (atom_alloc_array ((argc + 1), &size, &atoms, &alloc) == MAX_ERR_NONE)
        {
            long i;
            
            atom_setsym (atoms, s);
            
            for (i = 0; i < argc; i++) {
                switch (atom_gettype (argv + i)) {
                    case A_SYM  : atom_setsym (atoms + 1 + i, atom_getsym (argv + i)); break;
                    case A_LONG : atom_setlong (atoms + 1 + i, atom_getlong (argv + i)); break;
                    default     : atom_setfloat (atoms + 1 + i, 0.); break;
                    }
            }
            
            tralala_handleMessages (x, tll_sym_live, argc + 1, atoms);
            
            sysmem_freeptr (atoms);
        }
}
    
void tralala_handleMessages (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && argv && atom_gettype (argv) == A_SYM)
        {
            bool        draw = false;
            PIZSequence *sequence = NULL;
            
            if (s == tll_sym_live) {
                sequence = x->live; draw = LIVE;
                }
            else if (s == tll_sym_user) { 
                sequence = x->user; draw = USER;
                }
            else if (s == tll_sym_listen) {
                sequence = x->listen; draw = LISTEN;
                }
            
            if (sequence && (atom_gettype (argv) == A_SYM))
                {
                    if (atom_getsym (argv) == tll_sym_clear) {
                            draw &= tralala_sequenceClear (x, sequence);
                        }
                    else if (atom_getsym (argv) == tll_sym_clean) {
                            draw &= tralala_sequenceClean (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_note) {
                            draw &= tralala_sequenceNote (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_zone) {
                            draw &= tralala_sequenceZone (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_dump) {
                            draw &= tralala_sequenceDump (x, sequence, argc - 1, argv + 1); 
                        }
                    else if (atom_getsym (argv) == tll_sym_rotate) {
                            draw &= tralala_sequenceRotate (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_scramble) {
                            draw &= tralala_sequenceScramble (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_sort) {
                            draw &= tralala_sequenceSort (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_change) {
                            draw &= tralala_sequenceChange (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_set) {
                            draw &= tralala_sequenceSet (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_november) {
                            draw &= tralala_sequenceNovember (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_juliet) {
                            draw &= tralala_sequenceJuliet (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_cycle) {
                            draw &= tralala_sequenceCycle (x, sequence, argc - 1, argv + 1);
                        }
                    else if (atom_getsym (argv) == tll_sym_kill) {
                            draw &= tralala_sequenceKill (x, sequence);
                        }
                    else if (atom_getsym (argv) == tll_sym_zoulou) {
                            draw &= tralala_sequenceZoulou (x, sequence);
                        }
                    else if (atom_getsym (argv) == tll_sym_romeo) {
                            draw &= tralala_sequenceRomeo (x, sequence);
                        }
                    else if (atom_getsym (argv) == tll_sym_uniform) {
                            draw &= tralala_sequenceUniform (x, sequence);
                        }
                    
                    if (draw)
                        {
                            tralala_willChange (x);
                            
                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
                            
                            if (USER) {
                                    DIRTYSLOTS 
                                    DIRTYPATTR
                                    DIRTYUNDO
                                }
                        }   
                }
            
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool tralala_sequenceClear (t_tralala *x, PIZSequence *sequence)
{
    bool draw = (pizSequenceCount (sequence) != 0);
    
    pizSequenceClear (sequence);
    
    return draw;
}


bool tralala_sequenceClean (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long value = 1;
    
    if (argc && atom_gettype (argv) == A_LONG) {
            value = atom_getlong (argv);
        }
        
    draw = pizSequenceClean (sequence, value);
    
    return draw;
}

bool tralala_sequenceNote (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc == 5)
        {
            long i; 
            long err = PIZ_ERROR;
            
            ARRAY_GET (tempArray);
            
            if (tempArray)
                {
                    err = PIZ_GOOD;
                    
                    for (i = 0; i < 5; i++)
                        {
                            if (!(err |= (atom_gettype (argv + i) != A_LONG))){
                                switch (i) {
                                    case 0 : pizGrowingArrayAppend (tempArray, 
                                                (long)(atom_getlong (argv + i) / TICKS_FOR_ONE_STEP)); break;
                                    case 1 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                                                break;
                                    case 2 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                                                break;
                                    case 3 : pizGrowingArrayAppend (tempArray, 
                                                (long)(atom_getlong (argv + i) / TICKS_FOR_ONE_STEP)); break;
                                    case 4 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                                                break;          
                                    }
                                }
                        }
                }
            
            if (!err)
                {
                    long mode = PIZ_SEQUENCE_ADD_FLAG_SNAP;
                    
                    if (sequence == x->user) {
                        pizGrowingArrayAppend (tempArray, true);
                        pizGrowingArrayAppend (tempArray, false);
                    } else {
                        pizGrowingArrayAppend (tempArray, false);
                        pizGrowingArrayAppend (tempArray, false);
                    }
                    
                    if (sequence == x->live) {
                        mode = PIZ_SEQUENCE_ADD_FLAG_PATTERN;
                    } else if (sequence == x->listen) {
                        mode = PIZ_SEQUENCE_ADD_FLAG_AMBITUS;
                    }
                    
                    draw = !(pizSequenceAddNotesWithArray (sequence, tempArray, mode));
                }   
            
            ARRAY_RELEASE (tempArray);
        }
    
    return draw;
}

bool tralala_sequenceZone (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc == PIZ_SEQUENCE_ZONE_SIZE)
        {
            long i; 
            long err = PIZ_ERROR;
            
            ARRAY_GET (tempArray);
            
            if (tempArray)
                {
                    err = PIZ_GOOD;
                    
                    for (i = 0; i < PIZ_SEQUENCE_ZONE_SIZE; i++)
                        {
                            if (!(err |= (atom_gettype (argv + i) != A_LONG))){
                                switch (i) {
                                    case 0 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                                / TICKS_FOR_ONE_STEP)); break; 
                                    case 1 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                                / TICKS_FOR_ONE_STEP)); break;
                                    case 2 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                                    case 3 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                                    }
                                }
                        }
                }
            
            if (!err)
                {
                    if (sequence == x->user)
                        {
                            pizSequenceSetZoneWithArray (sequence, tempArray);
                            draw = true;
                        }
                    else if (sequence == x->live)
                        {
                            pizSequenceSetZoneWithArray (sequence, tempArray);
                            pizSequenceSetZoneWithArray (x->listen, tempArray);
                            
                            if (pizSequenceApplyAmbitus (x->listen) && LISTEN) 
                                {
                                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                                }
                            
                            draw = true;
                        }
                }
            
            ARRAY_RELEASE (tempArray);
        }
    
    return draw;
}   

bool tralala_sequenceDump (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    if ((argc == 1) && (atom_gettype (argv) == A_SYM)) 
        {
            if (atom_getsym (argv) == tll_sym_notes) 
                {   
                    long        i, count;
                    PIZError    err = PIZ_GOOD;
                    
                    ARRAY_GET (tempArray);
    
                    if (tempArray) 
                        {
                            err |= pizSequenceNotesToArray (sequence, tempArray, tempArray);
                        }
                    else
                        {
                            err |= PIZ_MEMORY;
                        }
                    
                    if (!err)
                        {
                            count = pizGrowingArrayCount (tempArray) / PIZ_SEQUENCE_NOTE_SIZE;
                            
                            outlet_anything  (x->rightOutlet, tll_sym_start, 0, NULL);
                            
                            for (i = 0; i < count; i++)
                                {
                                    long position = (pizGrowingArrayValueAtIndex (tempArray, 
                                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION))
                                                    * TICKS_FOR_ONE_STEP;
                                    long pitch    = pizGrowingArrayValueAtIndex (tempArray, 
                                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                                    long velocity = pizGrowingArrayValueAtIndex (tempArray, 
                                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY);
                                    long duration = (pizGrowingArrayValueAtIndex (tempArray, 
                                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION))
                                                    * TICKS_FOR_ONE_STEP;
                                    long channel  = pizGrowingArrayValueAtIndex (tempArray, 
                                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_CHANNEL);
                                        
                                    atom_setlong (x->dumpedNote,        position);
                                    atom_setlong (x->dumpedNote + 1,    pitch);
                                    atom_setlong (x->dumpedNote + 2,    velocity);
                                    atom_setlong (x->dumpedNote + 3,    duration);
                                    atom_setlong (x->dumpedNote + 4,    channel);
                                    
                                    outlet_list  (x->middleLeftOutlet, NULL, 5, x->dumpedNote);
                                }
                            
                            outlet_anything  (x->rightOutlet, tll_sym_end, 0, NULL);
                        }
                    
                    ARRAY_RELEASE (tempArray);
                } 
            else if (atom_getsym (argv) == tll_sym_zone && (sequence != x->listen)) 
                {   
                    t_atom      zone[4];
                    long        start, end, down, up;
                    PIZError    err = PIZ_GOOD;
                    
                    ARRAY_GET (tempArray);
    
                    if (tempArray) 
                        {
                            pizSequenceZoneToArray (sequence, tempArray);
                        }
                    else
                        {
                            err |= PIZ_MEMORY;
                        }
                    
                    if (!err)
                        {
                            start   = pizGrowingArrayValueAtIndex (tempArray, PIZ_SEQUENCE_START) 
                                        * TICKS_FOR_ONE_STEP;
                            end     = pizGrowingArrayValueAtIndex (tempArray, PIZ_SEQUENCE_END) 
                                        * TICKS_FOR_ONE_STEP;
                            down    = pizGrowingArrayValueAtIndex (tempArray, PIZ_SEQUENCE_DOWN);
                            up      = pizGrowingArrayValueAtIndex (tempArray, PIZ_SEQUENCE_UP);

                            atom_setlong (zone,     start);
                            atom_setlong (zone + 1, end);
                            atom_setlong (zone + 2, down);
                            atom_setlong (zone + 3, up);
                            
                            outlet_anything  (x->rightOutlet, tll_sym_zone, 4, zone);
                        }
                    
                    ARRAY_RELEASE (tempArray);
                }
            else if ((atom_getsym (argv) == tll_sym_count))
                {
                    t_atom  a;
                    long    count = pizSequenceCount (sequence);
                    
                    atom_setlong (&a, count);
                    
                    outlet_anything  (x->rightOutlet, tll_sym_count, 1, &a);
                }
        }

    return false;
}

bool tralala_sequenceRotate (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    long        shift = 1;
    PIZSelector selector = PIZ_PITCH;
    bool        draw = false;
    
    if (argc && argv)
        {
            long i;
                
            for (i = 0; i < argc; i++)
                {
                    if (atom_gettype (argv + i) == A_LONG)
                        {
                            shift = atom_getlong (argv + i);
                        }
                    else if (atom_gettype (argv + i) == A_SYM)
                        {
                            if (atom_getsym (argv + i) == tll_sym_velocity) {
                                    selector = PIZ_VELOCITY;
                                }
                            else if (atom_getsym (argv + i) == tll_sym_duration) {
                                    selector = PIZ_DURATION;
                                }
                            else if (atom_getsym (argv + i) == tll_sym_channel) {
                                    selector = PIZ_CHANNEL;
                                }
                        }
                }
        }
    
    draw = pizSequenceRotate (sequence, selector, -(shift));
    
    return draw;
}

bool tralala_sequenceScramble (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool        draw = false;
    PIZSelector selector = PIZ_PITCH;
    
    if (argc && argv)
        {
            if (atom_gettype (argv) == A_SYM)
                {
                    if (atom_getsym (argv) == tll_sym_velocity) {
                            selector = PIZ_VELOCITY;
                        }
                    else if (atom_getsym (argv) == tll_sym_duration) {
                            selector = PIZ_DURATION;
                        }
                    else if (atom_getsym (argv) == tll_sym_channel) {
                            selector = PIZ_CHANNEL;
                        }
                }
        }
    
    draw = pizSequenceScramble (sequence, selector);
    
    return draw;
}

bool tralala_sequenceSort (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    long        i;
    bool        draw = false;
    bool        mode = UP;
    PIZSelector selector = PIZ_PITCH;
    
    if (argc && argv)
        {
            for (i = 0; i < argc; i++)
                {
                    if (atom_gettype (argv + i) == A_SYM)
                        {
                            if (atom_getsym (argv + i) == tll_sym_down) {
                                    mode = DOWN;
                                }
                            if (atom_getsym (argv + i) == tll_sym_velocity) {
                                    selector = PIZ_VELOCITY;
                                }
                            else if (atom_getsym (argv + i) == tll_sym_duration) {
                                    selector = PIZ_DURATION;
                                }
                            else if (atom_getsym (argv + i) == tll_sym_channel) {
                                    selector = PIZ_CHANNEL;
                                }
                        }
                }
        }
    
    draw = pizSequenceSort (sequence, selector, mode);
                    
    return draw;
}

bool tralala_sequenceChange (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if ((argc == 2) && (atom_gettype (argv) == A_SYM) && (atom_gettype (argv + 1) == A_LONG))
        {
            long value = atom_getlong (argv + 1);

            if (atom_getsym (argv) == tll_sym_pitch) {
                    draw = pizSequenceChange (sequence, PIZ_PITCH, value);
                }
            else if (atom_getsym (argv) == tll_sym_velocity) {
                    draw = pizSequenceChange (sequence, PIZ_VELOCITY, value);
                }
            else if (atom_getsym (argv) == tll_sym_duration) {
                    draw = pizSequenceChange (sequence, PIZ_DURATION, (long)(value / TICKS_FOR_ONE_STEP));
                }
            else if (atom_getsym (argv) == tll_sym_channel) {
                    draw = pizSequenceChange (sequence, PIZ_CHANNEL, value);
                }
        }
    else if ((argc == 3) && 
        (atom_gettype (argv) == A_SYM) && 
        (atom_gettype (argv + 1) == A_LONG) &&
        (atom_gettype (argv + 2) == A_LONG))
        {
            long minValue = atom_getlong (argv + 1);
            long maxValue = atom_getlong (argv + 2);

            if (atom_getsym (argv) == tll_sym_pitch) {
                    draw = pizSequenceRandom (sequence, PIZ_PITCH, minValue, maxValue);
                }
            else if (atom_getsym (argv) == tll_sym_velocity) {
                    draw = pizSequenceRandom (sequence, PIZ_VELOCITY, minValue, maxValue);
                }
            else if (atom_getsym (argv) == tll_sym_duration) {
                    draw = pizSequenceRandom (sequence, PIZ_DURATION, 
                                                (long)(minValue / TICKS_FOR_ONE_STEP),
                                                (long)(maxValue / TICKS_FOR_ONE_STEP));
                }
            else if (atom_getsym (argv) == tll_sym_channel) {
                    draw = pizSequenceRandom (sequence, PIZ_CHANNEL, minValue, maxValue);
                }
        }
    
    return draw;
}

bool tralala_sequenceSet (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if ((argc == 2) && (atom_gettype (argv) == A_SYM) && (atom_gettype (argv + 1) == A_LONG))
        {   
            long value = atom_getlong (argv + 1);

            if (atom_getsym (argv) == tll_sym_pitch) {
                    draw = pizSequenceSet (sequence, PIZ_PITCH, value);
                }
            else if (atom_getsym (argv) == tll_sym_velocity) {
                    draw = pizSequenceSet (sequence, PIZ_VELOCITY, value);
                }
            else if (atom_getsym (argv) == tll_sym_duration) {
                    draw = pizSequenceSet (sequence, PIZ_DURATION, (long)(value / TICKS_FOR_ONE_STEP));
                }
            else if (atom_getsym (argv) == tll_sym_channel) {
                    draw = pizSequenceSet (sequence, PIZ_CHANNEL, value);
                }
        }
    
    return draw;
}

bool tralala_sequenceNovember (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long iterate = 1;
    
    if (argc && (atom_gettype (argv) == A_LONG))
        {
            iterate = CLAMP (atom_getlong (argv), 1, NOVEMBER_MAXIMUM_ITERATE);
        }
        
    draw = pizSequenceCellularAutomata (sequence, iterate);

    return draw;
}

bool tralala_sequenceJuliet (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long division = 0;
    long iterate = 1;
    
    if (argc)
        {
            long i;
            
            for (i = 0; i < argc; i++)
                {
                    if (atom_gettype (argv + i) == A_LONG)
                        {
                            iterate = CLAMP (atom_getlong (argv + i), 1, JULIET_MAXIMUM_ITERATE);
                        }
                    else if (atom_gettype (argv + i) == A_SYM)
                        {
                            if (atom_getsym (argv + i) == tll_sym_duple) {
                                    division = 2;
                                }
                            else if (atom_getsym (argv + i) == tll_sym_triple) {
                                    division = 3;
                                }
                        }
                }
        }
        
    draw = pizSequenceGenerator (sequence, iterate, division);

    return draw;
}

bool tralala_sequenceCycle (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc)
        {
            long i;
            
            ARRAY_GET (tempArray);
            
            if (tempArray) 
                {
                    for (i = 0; i < argc; i++) 
                        {
                            if (atom_gettype (argv + i) == A_LONG) 
                                {
                                    pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                                }
                        }
                
                    draw = pizSequenceCycle (sequence, x->key, tempArray);
                    
                    ARRAY_RELEASE (tempArray);
                }
        }

    return draw;
}

bool tralala_sequenceKill (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    draw = pizSequenceKillNotes (sequence);
    
    return draw;
}

bool tralala_sequenceZoulou (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmMutex);

    draw = pizSequenceProceedAlgorithm (sequence, PIZ_FACTOR_ORACLE, (void *)x->factorOracle);
    
    systhread_mutex_unlock (&x->algorithmMutex);
    
    return draw; 
}

bool tralala_sequenceRomeo (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmMutex);
    
    draw = pizSequenceProceedAlgorithm (sequence, PIZ_GALOIS_LATTICE, (void *)x->galoisLattice);
    
    systhread_mutex_unlock (&x->algorithmMutex);
    
    return draw;
}

bool tralala_sequenceUniform (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmMutex);
    
    draw = pizSequenceProceedAlgorithm (sequence, PIZ_FINITE_STATE, (void *)x->finiteState);
    
    systhread_mutex_unlock (&x->algorithmMutex);

    return draw;
}   

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_slot (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (ATOMIC_INCREMENT (&x->popupLock) == 1)
        {
            if (argc && argv) {
                if (atom_gettype (argv) == A_SYM) 
                    {
                        if (atom_getsym (argv) == tll_sym_store) {
                                tralala_slotStore (x);
                            }
                        else if (atom_getsym (argv) == tll_sym_new) {
                                tralala_slotNew (x);
                            }
                        else if (atom_getsym (argv) == tll_sym_newCopy) {
                                tralala_slotNewCopy (x);
                            }
                        else if (atom_getsym (argv) == tll_sym_recall) 
                            {
                                long n = x->slotIndex;
                                
                                if ((argc == 2) && atom_gettype (argv + 1) == A_LONG) { 
                                        n = atom_getlong (argv + 1);
                                    }

                                tralala_slotRecall (x, n);
                            }
                        else if (atom_getsym (argv) == tll_sym_remove) 
                            {
                                long n = x->slotIndex;
                                
                                if ((argc == 2) && atom_gettype (argv + 1) == A_LONG) { 
                                        n = atom_getlong (argv + 1);
                                    }

                                tralala_slotRemove (x, n);
                            }
                        else if (atom_getsym (argv) == tll_sym_swap) 
                            {
                                if ((argc == 3) && 
                                    atom_gettype (argv + 1) == A_LONG &&
                                    atom_gettype (argv + 2) == A_LONG) {
                                        long m = atom_getlong (argv + 1);
                                        long n = atom_getlong (argv + 2);
                                        
                                        tralala_slotSwap (x, m, n);
                                    }   
                            }
                        else if (atom_getsym (argv) == tll_sym_copy) 
                            {
                                if ((argc == 3) && 
                                    atom_gettype (argv + 1) == A_LONG &&
                                    atom_gettype (argv + 2) == A_LONG) {
                                        long m = atom_getlong (argv + 1);
                                        long n = atom_getlong (argv + 2);
                                        
                                        tralala_slotCopy (x, m, n);
                                    }
                            }
                        else if (atom_getsym (argv) == tll_sym_next) 
                            {
                                tralala_slotNext (x);
                            }
                        else if (atom_getsym (argv) == tll_sym_previous) 
                            {
                                tralala_slotPrevious (x);
                            }
                    }
                }       
        }

    ATOMIC_DECREMENT (&x->popupLock);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_slotNew (t_tralala *x)
{
    PIZGrowingArray *newSlot = NULL;
    
    if (newSlot = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE))
        {
            pizLinklistAppend (x->slots, newSlot);
            
            tralala_slotRecall (x, pizLinklistCount (x->slots) - 1);
            
            DIRTYSLOTS
        }
}

void tralala_slotNewCopy (t_tralala *x)
{
    PIZGrowingArray *newSlot = NULL;
    
    if (newSlot = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE))
        {
            PIZGrowingArray *slot = NULL;
            PIZError        err = PIZ_GOOD;
            
            tralala_slotStore (x);
            
            err |= pizLinklistPtrAtIndex (x->slots, x->slotIndex, (void **)&slot);
            
            if (!err) {
                    pizGrowingArrayCopy (newSlot, slot);
                }
        
            pizLinklistAppend (x->slots, newSlot);
            
            tralala_slotRecall (x, pizLinklistCount (x->slots) - 1);
            
            DIRTYSLOTS
        }
}

void tralala_slotRecall (t_tralala *x, long n)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, n, (void **)&slot))
        {
            tralala_willChange (x);
            
            if (x->slotIndex != n)
                {
                    tralala_slotStore (x);
                    tralala_clearUndo (x);
                }
            
            x->slotIndex = n;
            
            pizSequenceDecodeSlotWithArray (x->user, slot);
            
            DIRTYUNDO
            DIRTYPATTR
            
            DIRTYLAYER_SET (DIRTY_GRID);
        }
    
    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
}

void tralala_slotStore (t_tralala *x)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, x->slotIndex, (void **)&slot))
        {
            pizGrowingArrayClear (slot);
            pizSequenceEncodeSlotToArray (x->user, slot);
        }
}

void tralala_slotRemove (t_tralala *x, long n)
{
    PIZError        err = PIZ_GOOD;
    PIZGrowingArray *slot = NULL;
    
    if (!(err |= pizLinklistPtrAtIndex (x->slots, n, (void **)&slot))) 
        {
            err |= pizLinklistRemoveByPtr (x->slots, slot);
            
            DIRTYSLOTS
        }
    
    if (!err) 
        {
            if (!pizLinklistCount (x->slots))
                {
                    tralala_slotNew (x);
                    tralala_clearUndo (x);
                }
            else
                {
                    if (x->slotIndex > n)
                        {
                            x->slotIndex --;
                        }
                    else if (x->slotIndex == n)
                        {
                            if (x->slotIndex == pizLinklistCount (x->slots))
                                {
                                    x->slotIndex --;
                                }
                            
                            tralala_slotRecall (x, x->slotIndex);
                            tralala_clearUndo (x);
                        }   
                }
        }
}

void tralala_slotSwap (t_tralala *x, long m, long n)
{
    PIZError err = PIZ_GOOD;
    
    if ((x->slotIndex == m) || (x->slotIndex == n))
        {
            tralala_slotStore (x);
        }
    
    err |= pizLinklistSwapByIndexes (x->slots, m, n);
    
    if (!err) 
        {
            if (x->slotIndex == m)
                {
                    tralala_slotRecall (x, x->slotIndex);
                    tralala_clearUndo (x);
                }
            else if (x->slotIndex == n)
                {
                    tralala_slotRecall (x, x->slotIndex);
                }
            
            DIRTYSLOTS
        }
}

void tralala_slotCopy (t_tralala *x, long m, long n)
{
    PIZGrowingArray *a = NULL;
    PIZGrowingArray *b = NULL;
    PIZError        err = PIZ_GOOD;
    
    err |= pizLinklistPtrAtIndex (x->slots, m, (void **)&a);
    err |= pizLinklistPtrAtIndex (x->slots, n, (void **)&b);
    
    if (!err) {
            err |= pizGrowingArrayCopy (a, b);
            
            DIRTYSLOTS
        }
    
    if (!err && (x->slotIndex == m))
        {
            tralala_slotRecall (x, x->slotIndex);
        }
}

void tralala_slotNext (t_tralala *x)
{
    if (x->slotIndex < (pizLinklistCount (x->slots) - 1))
        {
            tralala_slotStore (x);
            tralala_slotRecall (x, x->slotIndex + 1);
        }
}

void tralala_slotPrevious (t_tralala *x)
{
    if (x->slotIndex)
        {
            tralala_slotStore (x);
            tralala_slotRecall (x, x->slotIndex - 1);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark HI
#pragma mark -

void tralala_undo (t_tralala *x)
{
    long count;
    
    systhread_mutex_lock (&x->methodMutex);
    
    count = pizLinklistCount (x->undo);
    
    if (count > 1)
        {
            PIZGrowingArray *now = NULL;
            PIZGrowingArray *previous = NULL;
            PIZGrowingArray *next = NULL;
            PIZError        err = PIZ_ERROR;
    
            if (!pizLinklistPtrAtIndex (x->undo, 0, (void **)&now))
                {
                    if (next = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE))
                        {
                            err = PIZ_GOOD;
                            err |= pizGrowingArrayCopy (next, now);
                            
                            if (!err) {
                                    err |= pizLinklistInsert (x->redo, next);
                                }
                                
                            if (err) 
                                {
                                    pizGrowingArrayFree (next);
                                }
                            else 
                                {
                                    err |= pizLinklistRemoveByPtr (x->undo, now);
                                }
                        }
                }
                
            if (!err && !pizLinklistPtrAtIndex (x->undo, 0, (void **)&previous))
                {
                    err |= pizSequenceDecodeUndoWithArray (x->user, previous);
                }
            
            if (!err) {
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
                    
                    DIRTYPATTR
                    DIRTYSLOTS
                }
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_redo (t_tralala *x)
{
    long count;
    
    systhread_mutex_lock (&x->methodMutex);
    
    count = pizLinklistCount (x->redo);
    
    if (count)
        {
            PIZGrowingArray *next = NULL;
            PIZGrowingArray *now = NULL;
            PIZError        err = PIZ_ERROR;
    
            if (!pizLinklistPtrAtIndex (x->redo, 0, (void **)&next))
                {
                    if (now = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE)) 
                        {
                            err = PIZ_GOOD;
                            err |= pizGrowingArrayCopy (now, next);
                            
                            if (!err) {
                                    err |= pizLinklistInsert (x->undo, now);
                                }
                            
                            if (err) 
                                {
                                    pizGrowingArrayFree (now);
                                }
                            else
                                {
                                    pizSequenceDecodeUndoWithArray (x->user, next);
                                    pizLinklistRemoveByPtr (x->redo, next);
                                    
                                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
                            
                                    DIRTYPATTR
                                    DIRTYSLOTS
                                }
                        }
                }
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_addUndo (t_tralala *x)
{
    PIZGrowingArray *newUndo = NULL;
    
    systhread_mutex_lock (&x->methodMutex);
    
    if (newUndo = pizGrowingArrayNew (INIT_GROWING_ARRAY_SIZE)) 
        {
            bool addUndo = true;
                            
            if (!pizSequenceEncodeUndoToArray (x->user, newUndo)) 
                {   
                    if (pizLinklistCount (x->undo)) 
                        {
                            PIZGrowingArray *lastUndo = NULL;
                            
                            if (!pizLinklistPtrAtIndex (x->undo, 0, (void **)&lastUndo)) 
                                {
                                    if (pizSequenceUndoIsEqualToUndo (newUndo, lastUndo)) {
                                            addUndo = false;
                                        } 
                                }
                        }
                        
                    if (addUndo)
                        {
                            pizLinklistInsert   (x->undo, newUndo);
                            pizLinklistClear    (x->redo);
                        }
                    else
                        {
                            pizGrowingArrayFree (newUndo);
                        }
                }
        }
    
    systhread_mutex_unlock (&x->methodMutex);
}

void tralala_clearUndo (t_tralala *x)
{
    systhread_mutex_lock (&x->methodMutex);
    
    pizLinklistClear (x->undo);
    pizLinklistClear (x->redo);
    
    systhread_mutex_unlock (&x->methodMutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_mousedown (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{   
    tralala_setCoordinatesWithPoint (x, &x->coordinates, pt);
            
    if (USER && CMD && !RIGHT && !CAPS)
        {
            if (!pizSequenceAddNoteWithCoordinates (x->user, &x->coordinates, PIZ_SEQUENCE_ADD_FLAG_SNAP)) {
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                    
                    DIRTYSLOTS
                    DIRTYPATTR
                    DIRTYUNDO
                } 
        }
    else if (USER && VIEWTEXT && !RIGHT && !CAPS && 
        (x->hitTest = tralala_hitTextWithPoint (x, patcherview, pt)))
        {
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
    else if (x->hitTest & HIT_LOCK)
        {
            ;
        }
    else if (USER && !CTRL && !RIGHT && CAPS && (x->hitTest = tralala_hitZoneWithPoint (x, pt)))
        {
            x->flags |= FLAG_ZONE_IS_SELECTED;
            
            pizSequenceSetTempZoneByZone (x->user);

            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_CHANGE);
        }
    else if (USER && SHIFT && !RIGHT && !CAPS)
        {
            long k = pizSequenceInvertNoteWithCoordinates (x->user, &x->coordinates);
            
            if (k != -1) {
                    x->hitTest = k;
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                }
        }
    else if (USER && !CAPS && (x->hitTest = pizSequenceSelectNoteWithCoordinates (x->user, &x->coordinates)))
        {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                        
            if (RIGHT) {
                    tralala_popupRightClickMenu (x, pt, MENU_NOTE);
                }
        }
    else if (RIGHT)
        {
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);

            tralala_popupRightClickMenu (x, pt, MENU_SEQUENCE);
        }
    else if (USER)
        {
            pizSequenceUnselectAllNotes (x->user);
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                    
            tralala_unselectAllText (x);
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
}

void tralala_mousedrag (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (!RIGHT && USER && !(x->dirtyLayer & DIRTY_CHANGE))
        {
            long        deltaPitch, deltaPosition;
            PIZError    err = PIZ_GOOD;
            long        selectedText = -1;
    
            if (!(x->flags & FLAG_ORIGIN_IS_SET))
                {
                    x->origin.x     = pt.x;
                    x->origin.y     = pt.y;
                    x->previous.x   = pt.x;
                    x->previous.y   = pt.y;
                    
                    x->mouseVelocityValue = 0;
                    
                    tralala_setCoordinatesWithPoint (x, &x->originCoordinates, x->previous);
                    
                    systhread_mutex_lock        (&x->arrayMutex);
                    err |= pizGrowingArrayCopy  (x->originNotes, x->selectedNotes);
                    systhread_mutex_unlock      (&x->arrayMutex);
                    
                    if (SHIFT) {
                        x->flags |= FLAG_ORIGIN_HAD_SHIFT_KEY;
                    } else {
                        x->flags &= ~FLAG_ORIGIN_HAD_SHIFT_KEY;
                    }
                    
                    x->flags |= FLAG_ORIGIN_IS_SET;
                }
            
            x->point.x = pt.x;
            x->point.y = pt.y;
            
            tralala_setCoordinatesWithPoint (x, &x->coordinates, pt);
                
            deltaPosition   = x->coordinates.position   - x->originCoordinates.position;
            deltaPitch      = x->coordinates.pitch      - x->originCoordinates.pitch;
                
            if (VIEWTEXT && tralala_hasSelectedText (x, &selectedText))
                {
                    long k = (long)(ABS (pt.y - x->previous.y));
                    
                    if ((pt.y - x->previous.y) < -0.5) {
                        switch (selectedText) {
                        case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, k);
                                                   break;
                        case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, 1);
                                                   break;
                        case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, 1);
                                                   break;
                        case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, 1);
                                                   break;
                        }
                    } else if ((pt.y - x->previous.y) > 0.5) {
                        switch (selectedText) {
                        case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, -k);
                                                   break;
                        case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, -1);
                                                   break;
                        case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, -1);
                                                   break;
                        case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, -1);
                                                   break;
                        }
                    }
                    
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                }
            else if (x->hitTest & HIT_LOCK)
                {
                    ;
                }
            else if ((x->hitTest == HIT_NOTE) && !err)
                {
                    if (ALT && !CTRL && 
                        !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED)))
                        {
                            tralala_duplicateSelectedNotes (x);
                            
                            DIRTYLAYER_SET (DIRTY_NOTES);
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_COPYING);
                        }
                    else if (CTRL && !(x->flags & FLAG_HAVE_MOVED))
                        {
                            if (ALT)
                                {
                                    x->textMode = TEXT_MODE_MOUSE_VELOCITY;
                                    
                                    if ((pt.y - x->previous.y) < -0.5)
                                        {
                                            tralala_changeSelectedNotesVelocity (x, UP);
                                            
                                            x->flags |= FLAG_HAVE_CHANGED;
                                            DIRTYLAYER_SET (DIRTY_NOTES);
                                        }
                                    else if ((pt.y - x->previous.y) > 0.5)
                                        {
                                            tralala_changeSelectedNotesVelocity (x, DOWN);
                                            
                                            x->flags |= FLAG_HAVE_CHANGED;
                                            DIRTYLAYER_SET (DIRTY_NOTES);
                                        }
                                }
                            else if (tralala_changeSelectedNotesDuration (x, deltaPosition))
                                {
                                    tralala_testAutoscroll (x, patcherview, pt);
                                    
                                    x->flags |= FLAG_HAVE_CHANGED;
                                    DIRTYLAYER_SET (DIRTY_NOTES);
                                }
                        }
                    else 
                        {
                            if (tralala_moveSelectedNotes (x, deltaPosition, deltaPitch))
                                {
                                    x->flags |= FLAG_HAVE_MOVED;
                                    DIRTYLAYER_SET (DIRTY_NOTES);
                                }
                            
                            if (CMD)
                                {
                                    x->textMode         = TEXT_MODE_MOUSE_PITCH;
                                    x->mousePitchValue  = x->coordinates.pitch;
                                    x->dirtyLayer       |= DIRTY_REFRESH;
                                }
                            else
                                {
                                    if (x->textMode != TEXT_MODE_NOTE)
                                        {
                                            x->textMode = TEXT_MODE_NOTE;
                                            DIRTYLAYER_SET (DIRTY_REFRESH);
                                        }
                                }
                            
                            tralala_testAutoscroll (x, patcherview, pt);
                        }
                }
            else if (x->hitTest == HIT_ZONE)
                {
                    bool draw = pizSequenceMoveTempZoneWithDelta (x->user, deltaPitch, deltaPosition);
                    
                    tralala_testAutoscroll (x, patcherview, pt);
                    
                    if (draw) {
                            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_CHANGE);
                        }
                }
            else if (x->hitTest)
                {
                    PIZCoordinates  c1;
                    PIZCoordinates  c2;
                    double          f = 1.;
                    bool            draw = false;
                                        
                    switch (x->zoomMode) {
                        case ZOOM_MODE_A    : f = 0.5;  break;
                        case ZOOM_MODE_B    : f = 1.;   break;
                        case ZOOM_MODE_C    : f = 2.;   break;
                        }
    
                    c1.position = (long)((x->windowOffsetX + pt.x) / (STEP_PIXELS_SIZE * f));
                    c2.position = (long)((x->windowOffsetX + pt.x) / (STEP_PIXELS_SIZE * f));
                    
                    c1.pitch = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y - 
                                (SEMITONE_PIXELS_SIZE / 2. * f)) / (SEMITONE_PIXELS_SIZE * f))), 0);
                    c2.pitch = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y + 
                                (SEMITONE_PIXELS_SIZE / 2. * f)) / (SEMITONE_PIXELS_SIZE * f))), 0);
                                
                    switch (x->hitTest) {
                    case HIT_START: draw = pizSequenceSetTempZoneWithCoordinates 
                                        (x->user, &x->coordinates, PIZ_SEQUENCE_START); break;
                    case HIT_END  : draw = pizSequenceSetTempZoneWithCoordinates
                                        (x->user, &x->coordinates, PIZ_SEQUENCE_END); break;
                    case HIT_DOWN : draw = pizSequenceSetTempZoneWithCoordinates 
                                        (x->user, &c1, PIZ_SEQUENCE_DOWN); break;
                    case HIT_UP   : draw = pizSequenceSetTempZoneWithCoordinates 
                                        (x->user, &c2, PIZ_SEQUENCE_UP); break;
                    case (HIT_START | HIT_END)  
                                  : draw = pizSequenceSetTempZoneWithCoordinates 
                                        (x->user, &x->coordinates, PIZ_SEQUENCE_END); break;
                    }
                
                    tralala_testAutoscroll (x, patcherview, pt);

                    if (draw) {
                            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_CHANGE);
                        } 
                }
            else if (CAPS)
                {
                    x->windowOffsetX -= pt.x - x->previous.x;
                    x->windowOffsetY -= pt.y - x->previous.y;
                    
                    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
                    
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                } 
            else if (!CMD) 
                {
                    x->flags |= FLAG_IS_LASSO;
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                    
                    if (x->flags & FLAG_ORIGIN_HAD_SHIFT_KEY) {
                        if (pizSequenceSelectNotesWithLasso 
                            (x->user, &x->originCoordinates, &x->coordinates, INVERT))  {
                                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                            }
                        }
                    else if (pizSequenceSelectNotesWithLasso 
                        (x->user, &x->originCoordinates, &x->coordinates, SELECT)) {
                            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                        }
                    
                    tralala_testAutoscroll (x, patcherview, pt);
                    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                }
            
            x->previous.x = pt.x;
            x->previous.y = pt.y;
        } 
    
    if (!USER)
        {
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
        }
}

void tralala_mouseup (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (USER)
        {
            long selectedText;
                
            if (tralala_hasSelectedText (x, &selectedText))
                {
                    tralala_unselectAllText (x);
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                    
                    DIRTYSLOTS 
                    DIRTYPATTR
                    DIRTYUNDO
                }

            if (x->flags & FLAG_ZONE_IS_SELECTED)
                {
                    pizSequenceSetZoneByTempZone (x->user);
                    x->flags &= ~FLAG_ZONE_IS_SELECTED;
                    DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_CHANGE);
                    
                    DIRTYSLOTS 
                    DIRTYPATTR
                    DIRTYUNDO
                }
            
            if (x->flags & (FLAG_HAVE_CHANGED | FLAG_HAVE_MOVED | FLAG_HAVE_BEEN_DUPLICATED))
                {
                    systhread_mutex_lock            (&x->arrayMutex);
                    pizSequenceRemoveSelectedNotes  (x->user);
                    pizSequenceAddNotesWithArray    (x->user, x->selectedNotes, PIZ_SEQUENCE_ADD_FLAG_NONE);
                    systhread_mutex_unlock          (&x->arrayMutex);
                    
                    x->textMode     = TEXT_MODE_NOTE;
                    x->flags        &= ~(FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED);
                    x->dirtyLayer   |= (DIRTY_NOTES | DIRTY_CHANGE);
                    
                    DIRTYSLOTS 
                    DIRTYPATTR
                    DIRTYUNDO
                }
            
            if (x->flags & FLAG_IS_LASSO)
                {
                    x->flags &= ~FLAG_IS_LASSO;
                    
                    pizSequenceInitLasso (x->user);
                    
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                    
                    DIRTYSLOTS 
                    DIRTYPATTR
                }

            x->flags &= ~FLAG_ORIGIN_IS_SET;
            
            tralala_stopAutoscroll (x);
        }
    
    if (!CMD && (x->hitTest != HIT_ZONE) && tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW))
        {
            if (!CAPS) {
                x->textMode = TEXT_MODE_NOTE;
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
        }
    
    x->hitTest = HIT_NOTHING;
}

void tralala_mouseenter (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}

void tralala_mouseleave (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
    
    if (x->textMode != TEXT_MODE_NOTE) {
            x->textMode = TEXT_MODE_NOTE;
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
}

void tralala_mousemove (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{   
    if (x->flags & FLAG_FOCUS)
        {
            tralala_setCoordinatesWithPoint (x, &x->coordinates, pt);
            
            if (CMD && !CAPS)
                {   
                    if (USER)
                        {
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_CROSSHAIR);
                        }
                    else
                        {
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                        }
                    
                    x->textMode = TEXT_MODE_MOUSE_PITCH;
                    x->mousePitchValue = x->coordinates.pitch;
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                }
            else if (USER && CAPS)
                {
                    long isHit = tralala_hitZoneWithPoint (x, pt);
                    
                    if (isHit == HIT_ZONE) 
                        {
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
                        }
                    else if (isHit)
                        {
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_POINTINGHAND);
                        }
                    else
                        {
                            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                        }
                    
                    if (x->textMode != TEXT_MODE_ZONE)
                        {
                            x->textMode = TEXT_MODE_ZONE;
                            DIRTYLAYER_SET (DIRTY_REFRESH);
                        }
                }
            else 
                {
                    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                            
                    if (x->textMode != TEXT_MODE_NOTE) 
                        {
                            x->textMode = TEXT_MODE_NOTE;
                            DIRTYLAYER_SET (DIRTY_REFRESH);
                        }
                }
        }
}

void tralala_mousewheel (t_tralala *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc)
{
    if (ALT)
        {
            t_rect  rect;
            long    k = 0;
            
            jbox_get_rect_for_view ((t_object *)x, view, &rect);    
    
            if (y_inc > 0.)
                {
                    switch (x->zoomMode) {
                        case ZOOM_MODE_A :  x->zoomMode = ZOOM_MODE_B; k = 1; break;
                        case ZOOM_MODE_B :  x->zoomMode = ZOOM_MODE_C; k = 1; break;
                        }
                }
            else
                {
                    switch (x->zoomMode) {
                        case ZOOM_MODE_C :  x->zoomMode = ZOOM_MODE_B; k = 2; break;
                        case ZOOM_MODE_B :  x->zoomMode = ZOOM_MODE_A; k = 2; break;
                        }
                }
            
            if (k == 1)
                {
                    x->windowOffsetX = ((2. * x->windowOffsetX) + rect.width) - (rect.width  / 2.); 
                    x->windowOffsetY = ((2. * x->windowOffsetY) + rect.height) - (rect.height  / 2.); 
                }
            else if (k == 2)
                {
                    x->windowOffsetX = (((2. * x->windowOffsetX) + rect.width) / 4.) - (rect.width / 2.);
                    x->windowOffsetY = (((2. * x->windowOffsetY) + rect.height) / 4.) - (rect.height / 2.);
                }
            
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES);
        }
    else
        {
            x->windowOffsetX -= x_inc * MOUSEWHEEL_FACTOR;
            x->windowOffsetY -= y_inc * MOUSEWHEEL_FACTOR;
            
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
                                    
void tralala_key (t_tralala *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{
    if (SHARP || (keycode == JKEY_SPACEBAR)) {
    
        switch (x->sequenceMode) {
            case SEQUENCE_MODE_USER   : object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_LIVE);
                                        break;
            case SEQUENCE_MODE_LIVE   : object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_USER);
                                        break;
            case SEQUENCE_MODE_LISTEN : object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_USER);
                                        break;
            }
        
    } else if (keycode == JKEY_UPARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
    
        pizSequenceTransposeOctave (x->user, UP);
        
        DIRTYSLOTS 
        DIRTYPATTR
        DIRTYUNDO
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE);
        
    } else if (keycode == JKEY_DOWNARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
    
        pizSequenceTransposeOctave (x->user, DOWN);
        
        DIRTYSLOTS 
        DIRTYPATTR
        DIRTYUNDO
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE);
        
    } else if (keycode == JKEY_ENTER) {
    
        tralala_setLiveByUser (x);
        
        if (LIVE) {
                DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE);
            }
        
    } else if (keycode == JKEY_RIGHTARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
                tralala_slotNext (x);
            } ATOMIC_DECREMENT (&x->popupLock);
        
    } else if (keycode == JKEY_LEFTARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
                tralala_slotPrevious (x);
            } ATOMIC_DECREMENT (&x->popupLock);
        
    } else if (USER && ((keycode == JKEY_DELETE) || (keycode == JKEY_BACKSPACE))) {
    
        pizSequenceRemoveSelectedNotes (x->user);
        
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
        DIRTYSLOTS 
        DIRTYPATTR
        DIRTYUNDO
        
    } else if (USER && CMD && !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED))) {
        if (ALL) 
            {
                pizSequenceSelectAllNotes (x->user);
                
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
            }
        else if (COPY) 
            {
                pizGrowingArrayClear (tll_clipboard);
                tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
                
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
            }
        else if (CUT) 
            {
                pizGrowingArrayClear (tll_clipboard);
                tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
                pizSequenceRemoveSelectedNotes (x->user);
                
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
            }
        else if (PASTE) 
            {
                if (!tll_clipboardError)
                    {
                        long count;
                        
                        if (count = (pizGrowingArrayCount (tll_clipboard) / PIZ_SEQUENCE_NOTE_SIZE))
                            {
                                long i;
                                long offsetPosition, offsetPitch;
                                                            
                                pizSequenceUnselectAllNotes (x->user);
                                
                                if (pizGrowingArrayValueAtIndex (tll_clipboard, PIZ_SEQUENCE_POSITION)
                                     < (PIZ_SEQUENCE_TIMELINE_SIZE / 2)) {
                                    offsetPosition = pizSequenceGrid (x->user);
                                } else {
                                    offsetPosition = -(pizSequenceGrid (x->user));
                                }
                                    
                                if (pizGrowingArrayValueAtIndex (tll_clipboard, PIZ_SEQUENCE_PITCH)
                                    < (PIZ_MAGIC_PITCH / 2)) {
                                    offsetPitch = 1;
                                } else {
                                    offsetPitch = -1;
                                }
                                
                                for (i = 0; i < count; i++) {
                                    long position = pizGrowingArrayValueAtIndex (tll_clipboard, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                                    long pitch = pizGrowingArrayValueAtIndex (tll_clipboard, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                                        
                                    position    += offsetPosition;
                                    pitch       += offsetPitch;
                                    
                                    pizGrowingArraySetValueAtIndex (tll_clipboard, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION, position);
                                    pizGrowingArraySetValueAtIndex (tll_clipboard, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH, pitch);
                                    }
                                
                                pizSequenceAddNotesWithArray 
                                    (x->user, tll_clipboard, PIZ_SEQUENCE_ADD_FLAG_SNAP);
                                    
                                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
                            }
                    }
            }
        else if (UNDO)
            {
                tralala_undo (x);
            }
        else if (REDO)
            {
                tralala_redo (x);
            }
        
        if (ALL || COPY || CUT || PASTE) {
                DIRTYSLOTS
                DIRTYPATTR
                DIRTYUNDO
            }
            
    } else if (keycode >= 49 && keycode <= 55) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1)
            {
                if (!SHIFT)
                    {
                        switch (keycode) {
                            case 55 : pizSequenceSetGrid (x->user, PIZ_SNAP_NONE);          break;
                            case 54 : pizSequenceSetGrid (x->user, PIZ_WHOLE_NOTE);         break;
                            case 53 : pizSequenceSetGrid (x->user, PIZ_HALF_NOTE);          break;
                            case 52 : pizSequenceSetGrid (x->user, PIZ_QUARTER_NOTE);       break;
                            case 51 : pizSequenceSetGrid (x->user, PIZ_EIGHTH_NOTE);        break;
                            case 50 : pizSequenceSetGrid (x->user, PIZ_SIXTEENTH_NOTE);     break;
                            case 49 : pizSequenceSetGrid (x->user, PIZ_THIRTY_SECOND_NOTE); break;
                            }
                        
                        DIRTYLAYER_SET (DIRTY_GRID);
                    }
                else
                    {
                        switch (keycode) {
                            case 55 : pizSequenceSetNoteValue (x->user, PIZ_SNAP_NONE);         break;
                            case 54 : pizSequenceSetNoteValue (x->user, PIZ_WHOLE_NOTE);        break;
                            case 53 : pizSequenceSetNoteValue (x->user, PIZ_HALF_NOTE);         break;
                            case 52 : pizSequenceSetNoteValue (x->user, PIZ_QUARTER_NOTE);      break;
                            case 51 : pizSequenceSetNoteValue (x->user, PIZ_EIGHTH_NOTE);       break;
                            case 50 : pizSequenceSetNoteValue (x->user, PIZ_SIXTEENTH_NOTE);    break;
                            case 49 : pizSequenceSetNoteValue (x->user, PIZ_THIRTY_SECOND_NOTE);break;
                            }
                        
                        DIRTYLAYER_SET (DIRTY_REFRESH);
                    }
            } ATOMIC_DECREMENT (&x->popupLock);
            
    } else if (TERNARY) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            
            PIZSnapValue old, new = PIZ_SNAP_NONE;
            
            if (!SHIFT) {
                old = pizSequenceGrid (x->user);
            } else {
                old = pizSequenceNoteValue (x->user);
            }
                
            switch (old) {
            case PIZ_WHOLE_NOTE_DOTTED          :   new = PIZ_WHOLE_NOTE_TRIPLET;           break;
            case PIZ_WHOLE_NOTE                 :   new = PIZ_WHOLE_NOTE_TRIPLET;           break;
            case PIZ_WHOLE_NOTE_TRIPLET         :   new = PIZ_WHOLE_NOTE;                   break;
            case PIZ_HALF_NOTE_DOTTED           :   new = PIZ_HALF_NOTE_TRIPLET;            break;
            case PIZ_HALF_NOTE                  :   new = PIZ_HALF_NOTE_TRIPLET;            break;
            case PIZ_HALF_NOTE_TRIPLET          :   new = PIZ_HALF_NOTE;                    break;
            case PIZ_QUARTER_NOTE_DOTTED        :   new = PIZ_QUARTER_NOTE_TRIPLET;         break;
            case PIZ_QUARTER_NOTE               :   new = PIZ_QUARTER_NOTE_TRIPLET;         break;
            case PIZ_QUARTER_NOTE_TRIPLET       :   new = PIZ_QUARTER_NOTE;                 break;
            case PIZ_EIGHTH_NOTE_DOTTED         :   new = PIZ_EIGHTH_NOTE_TRIPLET;          break;
            case PIZ_EIGHTH_NOTE                :   new = PIZ_EIGHTH_NOTE_TRIPLET;          break;
            case PIZ_EIGHTH_NOTE_TRIPLET        :   new = PIZ_EIGHTH_NOTE;                  break;
            case PIZ_SIXTEENTH_NOTE_DOTTED      :   new = PIZ_SIXTEENTH_NOTE_TRIPLET;       break;
            case PIZ_SIXTEENTH_NOTE             :   new = PIZ_SIXTEENTH_NOTE_TRIPLET;       break;
            case PIZ_SIXTEENTH_NOTE_TRIPLET     :   new = PIZ_SIXTEENTH_NOTE;               break;
            case PIZ_THIRTY_SECOND_NOTE         :   new = PIZ_THIRTY_SECOND_NOTE_TRIPLET;   break;
            case PIZ_THIRTY_SECOND_NOTE_TRIPLET :   new = PIZ_THIRTY_SECOND_NOTE;           break;
            case PIZ_SNAP_NONE                  :   new = PIZ_SNAP_NONE;                    break;
            }
            
            if (!SHIFT) {
                pizSequenceSetGrid (x->user, new);
                DIRTYLAYER_SET (DIRTY_GRID);
            } else {
                pizSequenceSetNoteValue (x->user, new);
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
        } ATOMIC_DECREMENT (&x->popupLock);
        
    } else if (DOTTED) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            
            PIZSnapValue old, new = PIZ_SNAP_NONE;
            
            if (!SHIFT) {
                old = pizSequenceGrid (x->user);
            } else {
                old = pizSequenceNoteValue (x->user);
            }
                
            switch (old) {
            case PIZ_WHOLE_NOTE_DOTTED          :   new = PIZ_WHOLE_NOTE;                   break;
            case PIZ_WHOLE_NOTE                 :   new = PIZ_WHOLE_NOTE_DOTTED;            break;
            case PIZ_WHOLE_NOTE_TRIPLET         :   new = PIZ_WHOLE_NOTE_DOTTED;            break;
            case PIZ_HALF_NOTE_DOTTED           :   new = PIZ_HALF_NOTE;                    break;
            case PIZ_HALF_NOTE                  :   new = PIZ_HALF_NOTE_DOTTED;             break;
            case PIZ_HALF_NOTE_TRIPLET          :   new = PIZ_HALF_NOTE_DOTTED;             break;
            case PIZ_QUARTER_NOTE_DOTTED        :   new = PIZ_QUARTER_NOTE;                 break;
            case PIZ_QUARTER_NOTE               :   new = PIZ_QUARTER_NOTE_DOTTED;          break;
            case PIZ_QUARTER_NOTE_TRIPLET       :   new = PIZ_QUARTER_NOTE_DOTTED;          break;
            case PIZ_EIGHTH_NOTE_DOTTED         :   new = PIZ_EIGHTH_NOTE;                  break;
            case PIZ_EIGHTH_NOTE                :   new = PIZ_EIGHTH_NOTE_DOTTED;           break;
            case PIZ_EIGHTH_NOTE_TRIPLET        :   new = PIZ_EIGHTH_NOTE_DOTTED;           break;
            case PIZ_SIXTEENTH_NOTE_DOTTED      :   new = PIZ_SIXTEENTH_NOTE;               break;
            case PIZ_SIXTEENTH_NOTE             :   new = PIZ_SIXTEENTH_NOTE_DOTTED;        break;
            case PIZ_SIXTEENTH_NOTE_TRIPLET     :   new = PIZ_SIXTEENTH_NOTE_DOTTED;        break;
            case PIZ_THIRTY_SECOND_NOTE         :   new = PIZ_THIRTY_SECOND_NOTE;           break;
            case PIZ_THIRTY_SECOND_NOTE_TRIPLET :   new = PIZ_THIRTY_SECOND_NOTE_TRIPLET;   break;
            case PIZ_SNAP_NONE                  :   new = PIZ_SNAP_NONE;                    break;
            }
            
            if (!SHIFT) {
                pizSequenceSetGrid (x->user, new);
                DIRTYLAYER_SET (DIRTY_GRID);
            } else {
                pizSequenceSetNoteValue (x->user, new);
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
        } ATOMIC_DECREMENT (&x->popupLock);
        
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_popupRightClickMenu (t_tralala *x, t_pt pt, long menuMode)
{
    t_pt            locate;
    t_jpopupmenu    *popup;
    t_jpopupmenu    *snapPopup;
    t_jpopupmenu    *snapTripletPopup;
    t_jpopupmenu    *snapDottedPopup;
    t_jpopupmenu    *noteValuePopup;
    t_jpopupmenu    *noteValueTripletPopup;
    t_jpopupmenu    *noteValueDottedPopup;
    t_jpopupmenu    *velocityPopup;
    t_jpopupmenu    *noteChannelPopup;
    t_jpopupmenu    *sequenceChannelPopup;
    t_jpopupmenu    *slotsPopup;
    t_jfont         *font;
    long            returnedPopupValue = 0;
    int             mouseX, mouseY;
    long            channel;
    PIZSnapValue    snap;

    jmouse_getposition_global (&mouseX, &mouseY);
    
    locate.x = (double)mouseX;
    locate.y = (double)mouseY;
    
    font = jfont_create (x->popupFontName->s_name, (x->popupFontFace & (1<<1)), 
        (x->popupFontFace & (1<<0)), x->popupFontSize); 
    
    popup                   = jpopupmenu_create ( );
    
    snapPopup               = jpopupmenu_create ( );
    snapTripletPopup        = jpopupmenu_create ( );
    snapDottedPopup         = jpopupmenu_create ( );
    noteValuePopup          = jpopupmenu_create ( );
    noteValueTripletPopup   = jpopupmenu_create ( );
    noteValueDottedPopup    = jpopupmenu_create ( );
    velocityPopup           = jpopupmenu_create ( );
    noteChannelPopup        = jpopupmenu_create ( );
    sequenceChannelPopup    = jpopupmenu_create ( );
    slotsPopup              = jpopupmenu_create ( );
    
    jpopupmenu_setcolors    (popup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
                                
    jpopupmenu_setcolors    (snapPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (snapTripletPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (snapDottedPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (noteValuePopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (noteValueTripletPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (noteValueDottedPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
                                
    jpopupmenu_setcolors    (velocityPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (noteChannelPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (sequenceChannelPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    jpopupmenu_setcolors    (slotsPopup, x->popupTextColor, x->popupBackgroundColor, 
                                x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor);
    
    jpopupmenu_setfont      (popup, font);  
    jpopupmenu_setfont      (snapPopup, font);
    jpopupmenu_setfont      (snapTripletPopup, font);
    jpopupmenu_setfont      (snapDottedPopup, font);
    jpopupmenu_setfont      (noteValuePopup, font);
    jpopupmenu_setfont      (noteValueTripletPopup, font);
    jpopupmenu_setfont      (noteValueDottedPopup, font);
    jpopupmenu_setfont      (velocityPopup, font);
    jpopupmenu_setfont      (noteChannelPopup, font);                   
    jpopupmenu_setfont      (sequenceChannelPopup, font);
    jpopupmenu_setfont      (slotsPopup, font);
    
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
        snap    = pizSequenceGrid (x->user);
        channel = pizSequenceChannel (x->user);

        jpopupmenu_additem      (snapPopup, 11, "Whole", 
                                NULL, (snap == PIZ_WHOLE_NOTE), 0, NULL);
        jpopupmenu_additem      (snapPopup, 12, "Half", 
                                NULL, (snap == PIZ_HALF_NOTE), 0, NULL);
        jpopupmenu_additem      (snapPopup, 13, "Quarter", 
                                NULL, (snap == PIZ_QUARTER_NOTE), 0, NULL);
        jpopupmenu_additem      (snapPopup, 14, "Eighth", 
                                NULL, (snap == PIZ_EIGHTH_NOTE), 0, NULL);
        jpopupmenu_additem      (snapPopup, 15, "Sixteenth", 
                                NULL, (snap == PIZ_SIXTEENTH_NOTE), 0, NULL);
        jpopupmenu_additem      (snapPopup, 16, "Thirty-second", 
                                NULL, (snap == PIZ_THIRTY_SECOND_NOTE), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 17, "Whole", 
                                NULL, (snap == PIZ_WHOLE_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 18, "Half", 
                                NULL, (snap == PIZ_HALF_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 19, "Quarter", 
                                NULL, (snap == PIZ_QUARTER_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 20, "Eighth", 
                                NULL, (snap == PIZ_EIGHTH_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 21, "Sixteenth", 
                                NULL, (snap == PIZ_SIXTEENTH_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapTripletPopup, 22, "Thirty-second", 
                                NULL, (snap == PIZ_THIRTY_SECOND_NOTE_TRIPLET), 0, NULL);
        jpopupmenu_additem      (snapDottedPopup, 23, "Whole", 
                                NULL, (snap == PIZ_WHOLE_NOTE_DOTTED), 0, NULL);
        jpopupmenu_additem      (snapDottedPopup, 24, "Half", 
                                NULL, (snap == PIZ_HALF_NOTE_DOTTED), 0, NULL);
        jpopupmenu_additem      (snapDottedPopup, 25, "Quarter", 
                                NULL, (snap == PIZ_QUARTER_NOTE_DOTTED), 0, NULL);
        jpopupmenu_additem      (snapDottedPopup, 26, "Eighth", 
                                NULL, (snap == PIZ_EIGHTH_NOTE_DOTTED), 0, NULL);
        jpopupmenu_additem      (snapDottedPopup, 27, "Sixteenth", 
                                NULL, (snap == PIZ_SIXTEENTH_NOTE_DOTTED), 0, NULL);
                                
        jpopupmenu_addseperator (snapPopup);
        jpopupmenu_addsubmenu   (snapPopup, "Triplet", snapTripletPopup, 0);
        jpopupmenu_addsubmenu   (snapPopup, "Dotted ", snapDottedPopup, 0);
        jpopupmenu_addseperator (snapPopup);
        jpopupmenu_additem      (snapPopup, 10, "None", 
                                NULL, (snap == PIZ_SNAP_NONE), 0, NULL);
                                                    
        if (menuMode == MENU_SEQUENCE) {
                jpopupmenu_additem      (sequenceChannelPopup, 501, "1", NULL, (channel == 1), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 502, "2", NULL, (channel == 2), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 503, "3", NULL, (channel == 3), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 504, "4", NULL, (channel == 4), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 505, "5", NULL, (channel == 5), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 506, "6", NULL, (channel == 6), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 507, "7", NULL, (channel == 7), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 508, "8", NULL, (channel == 8), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 509, "9", NULL, (channel == 9), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 510, "10", NULL, (channel == 10), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 511, "11", NULL, (channel == 11), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 512, "12", NULL, (channel == 12), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 513, "13", NULL, (channel == 13), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 514, "14", NULL, (channel == 14), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 515, "15", NULL, (channel == 15), 0, NULL);
                jpopupmenu_additem      (sequenceChannelPopup, 516, "16", NULL, (channel == 16), 0, NULL);
            
                jpopupmenu_additem      (popup, 100,    "User",     NULL, USER, 0, NULL);
                jpopupmenu_additem      (popup, 101,    "Live",     NULL, LIVE, 0, NULL);
                jpopupmenu_additem      (popup, 102,    "Listen",   NULL, LISTEN, 0, NULL);
                jpopupmenu_addseperator (popup);
        
                if (USER) 
                    {
                        long            i;
                        PIZSnapValue    value = pizSequenceNoteValue (x->user);
                        
                        jpopupmenu_additem      (noteValuePopup, 301, "Whole", 
                                                NULL, (value == PIZ_WHOLE_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValuePopup, 302, "Half", 
                                                NULL, (value == PIZ_HALF_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValuePopup, 303, "Quarter", 
                                                NULL, (value == PIZ_QUARTER_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValuePopup, 304, "Eighth", 
                                                NULL, (value == PIZ_EIGHTH_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValuePopup, 305, "Sixteenth", 
                                                NULL, (value == PIZ_SIXTEENTH_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValuePopup, 306, "Thirty-second", 
                                                NULL, (value == PIZ_THIRTY_SECOND_NOTE), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 307, "Whole", 
                                                NULL, (value == PIZ_WHOLE_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 308, "Half", 
                                                NULL, (value == PIZ_HALF_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 309, "Quarter", 
                                                NULL, (value == PIZ_QUARTER_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 310, "Eighth", 
                                                NULL, (value == PIZ_EIGHTH_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 311, "Sixteenth", 
                                                NULL, (value == PIZ_SIXTEENTH_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueTripletPopup, 312, "Thirty-second", 
                                                NULL, (value == PIZ_THIRTY_SECOND_NOTE_TRIPLET), 0, NULL);
                        jpopupmenu_additem      (noteValueDottedPopup, 313, "Whole", 
                                                NULL, (value == PIZ_WHOLE_NOTE_DOTTED), 0, NULL);
                        jpopupmenu_additem      (noteValueDottedPopup, 314, "Half", 
                                                NULL, (value == PIZ_HALF_NOTE_DOTTED), 0, NULL);
                        jpopupmenu_additem      (noteValueDottedPopup, 315, "Quarter", 
                                                NULL, (value == PIZ_QUARTER_NOTE_DOTTED), 0, NULL);
                        jpopupmenu_additem      (noteValueDottedPopup, 316, "Eighth", 
                                                NULL, (value == PIZ_EIGHTH_NOTE_DOTTED), 0, NULL);
                        jpopupmenu_additem      (noteValueDottedPopup, 317, "Sixteenth", 
                                                NULL, (value == PIZ_SIXTEENTH_NOTE_DOTTED), 0, NULL);
                                                                        
                        jpopupmenu_addseperator (noteValuePopup);
                        jpopupmenu_addsubmenu   (noteValuePopup, "Triplet ", noteValueTripletPopup, 0);
                        jpopupmenu_addsubmenu   (noteValuePopup, "Dotted ", noteValueDottedPopup, 0);
                        jpopupmenu_addseperator (noteValuePopup);
                        jpopupmenu_additem      (noteValuePopup, 300, "Automatic", 
                                                NULL, (value == PIZ_SNAP_NONE), 0, NULL);
                                
                        jpopupmenu_additem      (slotsPopup, 1000, "New", NULL, 0, 0, NULL);
                        jpopupmenu_additem      (slotsPopup, 1001, "Clone", NULL, 0, 0, NULL);
                        jpopupmenu_addseperator (slotsPopup);
                        
                        for (i = 0; i < pizLinklistCount (x->slots); i++) {
                            char text[STRING_MAXIMUM_SIZE];
                            snprintf (text, STRING_MAXIMUM_SIZE, "Slot %ld", i);
                            text[STRING_MAXIMUM_SIZE - 1] = 0;
                            jpopupmenu_additem (slotsPopup, 5000 + i, text, NULL, (i == x->slotIndex), 0, NULL);
                        }
                        
                        jpopupmenu_addseperator (slotsPopup);
                        jpopupmenu_additem      (slotsPopup, 1002, "Remove ", NULL, 0, 0, NULL);
                        
                        jpopupmenu_addseperator (popup);
                        jpopupmenu_addsubmenu   (popup, "Slots", slotsPopup, 0);
                        jpopupmenu_addseperator (popup);
                        jpopupmenu_addsubmenu   (popup, "Snap", snapPopup, 0);
                        jpopupmenu_addsubmenu   (popup, "Value", noteValuePopup, 0);
                    }
        
                jpopupmenu_addseperator (popup);
                jpopupmenu_addsubmenu   (popup, "Channel    ", sequenceChannelPopup, 0);
            } 
        else if (menuMode == MENU_NOTE) 
            {
                long count;
                long velocity = -1;
                long channel = -1;

                systhread_mutex_lock (&x->arrayMutex);
                
                count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
                
                if (count == 1) {
                        velocity = pizGrowingArrayValueAtIndex (x->selectedNotes, PIZ_SEQUENCE_VELOCITY);
                        channel = pizGrowingArrayValueAtIndex (x->selectedNotes, PIZ_SEQUENCE_CHANNEL);
                        
                        velocity = CLAMP ((long)((velocity + 4) / 8) * 8, 0, 127);
                    }

                systhread_mutex_unlock (&x->arrayMutex);
                
                jpopupmenu_additem (velocityPopup, 50, "0",     NULL, (velocity == 0), 0, NULL);
                jpopupmenu_additem (velocityPopup, 51, "8",     NULL, (velocity == 8), 0, NULL);
                jpopupmenu_additem (velocityPopup, 52, "16",    NULL, (velocity == 16), 0, NULL);
                jpopupmenu_additem (velocityPopup, 53, "24",    NULL, (velocity == 24), 0, NULL);
                jpopupmenu_additem (velocityPopup, 54, "32",    NULL, (velocity == 32), 0, NULL);
                jpopupmenu_additem (velocityPopup, 55, "40",    NULL, (velocity == 40), 0, NULL);
                jpopupmenu_additem (velocityPopup, 56, "48",    NULL, (velocity == 48), 0, NULL);
                jpopupmenu_additem (velocityPopup, 57, "56",    NULL, (velocity == 56), 0, NULL);
                jpopupmenu_additem (velocityPopup, 58, "64",    NULL, (velocity == 64), 0, NULL);
                jpopupmenu_additem (velocityPopup, 59, "72",    NULL, (velocity == 72), 0, NULL);
                jpopupmenu_additem (velocityPopup, 60, "80",    NULL, (velocity == 80), 0, NULL);
                jpopupmenu_additem (velocityPopup, 61, "88",    NULL, (velocity == 88), 0, NULL);
                jpopupmenu_additem (velocityPopup, 62, "96",    NULL, (velocity == 96), 0, NULL);
                jpopupmenu_additem (velocityPopup, 63, "104",   NULL, (velocity == 104), 0, NULL);
                jpopupmenu_additem (velocityPopup, 64, "112",   NULL, (velocity == 112), 0, NULL);
                jpopupmenu_additem (velocityPopup, 65, "120",   NULL, (velocity == 120), 0, NULL);
                jpopupmenu_additem (velocityPopup, 66, "127",   NULL, (velocity == 127), 0, NULL);
        
                jpopupmenu_additem (noteChannelPopup, 200, "0",    NULL, (channel == 0), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 201, "1",    NULL, (channel == 1), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 202, "2",    NULL, (channel == 2), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 203, "3",    NULL, (channel == 3), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 204, "4",    NULL, (channel == 4), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 205, "5",    NULL, (channel == 5), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 206, "6",    NULL, (channel == 6), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 207, "7",    NULL, (channel == 7), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 208, "8",    NULL, (channel == 8), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 209, "9",    NULL, (channel == 9), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 210, "10",   NULL, (channel == 10), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 211, "11",   NULL, (channel == 11), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 212, "12",   NULL, (channel == 12), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 213, "13",   NULL, (channel == 13), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 214, "14",   NULL, (channel == 14), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 215, "15",   NULL, (channel == 15), 0, NULL);
                jpopupmenu_additem (noteChannelPopup, 216, "16",   NULL, (channel == 16), 0, NULL);

                jpopupmenu_addsubmenu   (popup, "Velocity   ", velocityPopup, 0);
                jpopupmenu_addsubmenu   (popup, "Channel",  noteChannelPopup, 0);
            }
    
        returnedPopupValue = jpopupmenu_popup (popup, locate, 0);
        
            if (returnedPopupValue == 1000) {
                tralala_slotNew (x); 
            } else if (returnedPopupValue == 1001) {
                tralala_slotNewCopy (x); 
            } else if (returnedPopupValue == 1002) {
                tralala_slotRemove (x, x->slotIndex); 
            } else if (returnedPopupValue >= 5000) {
                tralala_slotRecall (x, returnedPopupValue - 5000);
            }
    
        } ATOMIC_DECREMENT (&x->popupLock);
            
    switch (returnedPopupValue) {
        case 0      :   break;
        case 10     :   pizSequenceSetGrid  (x->user, PIZ_SNAP_NONE);                    break;
        case 11     :   pizSequenceSetGrid  (x->user, PIZ_WHOLE_NOTE);                   break;
        case 12     :   pizSequenceSetGrid  (x->user, PIZ_HALF_NOTE);                    break;
        case 13     :   pizSequenceSetGrid  (x->user, PIZ_QUARTER_NOTE);                 break;
        case 14     :   pizSequenceSetGrid  (x->user, PIZ_EIGHTH_NOTE);                  break;
        case 15     :   pizSequenceSetGrid  (x->user, PIZ_SIXTEENTH_NOTE);               break;
        case 16     :   pizSequenceSetGrid  (x->user, PIZ_THIRTY_SECOND_NOTE);           break;
        case 17     :   pizSequenceSetGrid  (x->user, PIZ_WHOLE_NOTE_TRIPLET);           break;
        case 18     :   pizSequenceSetGrid  (x->user, PIZ_HALF_NOTE_TRIPLET);            break;
        case 19     :   pizSequenceSetGrid  (x->user, PIZ_QUARTER_NOTE_TRIPLET);         break;
        case 20     :   pizSequenceSetGrid  (x->user, PIZ_EIGHTH_NOTE_TRIPLET);          break;
        case 21     :   pizSequenceSetGrid  (x->user, PIZ_SIXTEENTH_NOTE_TRIPLET);       break;
        case 22     :   pizSequenceSetGrid  (x->user, PIZ_THIRTY_SECOND_NOTE_TRIPLET);   break;
        case 23     :   pizSequenceSetGrid  (x->user, PIZ_WHOLE_NOTE_DOTTED);            break;
        case 24     :   pizSequenceSetGrid  (x->user, PIZ_HALF_NOTE_DOTTED);             break;
        case 25     :   pizSequenceSetGrid  (x->user, PIZ_QUARTER_NOTE_DOTTED);          break;
        case 26     :   pizSequenceSetGrid  (x->user, PIZ_EIGHTH_NOTE_DOTTED);           break;
        case 27     :   pizSequenceSetGrid  (x->user, PIZ_SIXTEENTH_NOTE_DOTTED);        break;
        case 50     :   tralala_setSelectedNotesVelocity (x, 0);    x->flags |= FLAG_HAVE_CHANGED; break;
        case 51     :   tralala_setSelectedNotesVelocity (x, 8);    x->flags |= FLAG_HAVE_CHANGED; break;
        case 52     :   tralala_setSelectedNotesVelocity (x, 16);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 53     :   tralala_setSelectedNotesVelocity (x, 24);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 54     :   tralala_setSelectedNotesVelocity (x, 32);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 55     :   tralala_setSelectedNotesVelocity (x, 40);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 56     :   tralala_setSelectedNotesVelocity (x, 48);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 57     :   tralala_setSelectedNotesVelocity (x, 56);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 58     :   tralala_setSelectedNotesVelocity (x, 64);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 59     :   tralala_setSelectedNotesVelocity (x, 72);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 60     :   tralala_setSelectedNotesVelocity (x, 80);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 61     :   tralala_setSelectedNotesVelocity (x, 88);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 62     :   tralala_setSelectedNotesVelocity (x, 96);   x->flags |= FLAG_HAVE_CHANGED; break;
        case 63     :   tralala_setSelectedNotesVelocity (x, 104);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 64     :   tralala_setSelectedNotesVelocity (x, 112);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 65     :   tralala_setSelectedNotesVelocity (x, 120);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 66     :   tralala_setSelectedNotesVelocity (x, 127);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 100    :   object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_USER);      break;
        case 101    :   object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_LIVE);      break;
        case 102    :   object_attr_setlong (x, tll_sym_sequenceMode, SEQUENCE_MODE_LISTEN);    break;
        case 200    :   tralala_setSelectedNotesChannel (x, 0);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 201    :   tralala_setSelectedNotesChannel (x, 1);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 202    :   tralala_setSelectedNotesChannel (x, 2);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 203    :   tralala_setSelectedNotesChannel (x, 3);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 204    :   tralala_setSelectedNotesChannel (x, 4);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 205    :   tralala_setSelectedNotesChannel (x, 5);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 206    :   tralala_setSelectedNotesChannel (x, 6);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 207    :   tralala_setSelectedNotesChannel (x, 7);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 208    :   tralala_setSelectedNotesChannel (x, 8);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 209    :   tralala_setSelectedNotesChannel (x, 9);  x->flags |= FLAG_HAVE_CHANGED; break;
        case 210    :   tralala_setSelectedNotesChannel (x, 10); x->flags |= FLAG_HAVE_CHANGED; break;
        case 211    :   tralala_setSelectedNotesChannel (x, 11); x->flags |= FLAG_HAVE_CHANGED; break;
        case 212    :   tralala_setSelectedNotesChannel (x, 12); x->flags |= FLAG_HAVE_CHANGED; break;
        case 213    :   tralala_setSelectedNotesChannel (x, 13); x->flags |= FLAG_HAVE_CHANGED; break;
        case 214    :   tralala_setSelectedNotesChannel (x, 14); x->flags |= FLAG_HAVE_CHANGED; break;
        case 215    :   tralala_setSelectedNotesChannel (x, 15); x->flags |= FLAG_HAVE_CHANGED; break;
        case 216    :   tralala_setSelectedNotesChannel (x, 16); x->flags |= FLAG_HAVE_CHANGED; break;
        case 300    :   pizSequenceSetNoteValue (x->user, PIZ_SNAP_NONE);                   break;
        case 301    :   pizSequenceSetNoteValue (x->user, PIZ_WHOLE_NOTE);                  break;
        case 302    :   pizSequenceSetNoteValue (x->user, PIZ_HALF_NOTE);                   break;
        case 303    :   pizSequenceSetNoteValue (x->user, PIZ_QUARTER_NOTE);                break;
        case 304    :   pizSequenceSetNoteValue (x->user, PIZ_EIGHTH_NOTE);                 break;
        case 305    :   pizSequenceSetNoteValue (x->user, PIZ_SIXTEENTH_NOTE);              break;
        case 306    :   pizSequenceSetNoteValue (x->user, PIZ_THIRTY_SECOND_NOTE);          break;
        case 307    :   pizSequenceSetNoteValue (x->user, PIZ_WHOLE_NOTE_TRIPLET);          break;
        case 308    :   pizSequenceSetNoteValue (x->user, PIZ_HALF_NOTE_TRIPLET);           break;
        case 309    :   pizSequenceSetNoteValue (x->user, PIZ_QUARTER_NOTE_TRIPLET);        break;
        case 310    :   pizSequenceSetNoteValue (x->user, PIZ_EIGHTH_NOTE_TRIPLET);         break;
        case 311    :   pizSequenceSetNoteValue (x->user, PIZ_SIXTEENTH_NOTE_TRIPLET);      break;
        case 312    :   pizSequenceSetNoteValue (x->user, PIZ_THIRTY_SECOND_NOTE_TRIPLET);  break;
        case 313    :   pizSequenceSetNoteValue (x->user, PIZ_WHOLE_NOTE_DOTTED);           break;
        case 314    :   pizSequenceSetNoteValue (x->user, PIZ_HALF_NOTE_DOTTED);            break;
        case 315    :   pizSequenceSetNoteValue (x->user, PIZ_QUARTER_NOTE_DOTTED);         break;
        case 316    :   pizSequenceSetNoteValue (x->user, PIZ_EIGHTH_NOTE_DOTTED);          break;
        case 317    :   pizSequenceSetNoteValue (x->user, PIZ_SIXTEENTH_NOTE_DOTTED);       break;
        case 501    :   object_attr_setlong (x, tll_sym_channel, 1);  break;
        case 502    :   object_attr_setlong (x, tll_sym_channel, 2);  break;
        case 503    :   object_attr_setlong (x, tll_sym_channel, 3);  break;
        case 504    :   object_attr_setlong (x, tll_sym_channel, 4);  break;
        case 505    :   object_attr_setlong (x, tll_sym_channel, 5);  break;
        case 506    :   object_attr_setlong (x, tll_sym_channel, 6);  break;
        case 507    :   object_attr_setlong (x, tll_sym_channel, 7);  break;
        case 508    :   object_attr_setlong (x, tll_sym_channel, 8);  break;
        case 509    :   object_attr_setlong (x, tll_sym_channel, 9);  break;
        case 510    :   object_attr_setlong (x, tll_sym_channel, 10); break;
        case 511    :   object_attr_setlong (x, tll_sym_channel, 11); break;
        case 512    :   object_attr_setlong (x, tll_sym_channel, 12); break;
        case 513    :   object_attr_setlong (x, tll_sym_channel, 13); break;
        case 514    :   object_attr_setlong (x, tll_sym_channel, 14); break;
        case 515    :   object_attr_setlong (x, tll_sym_channel, 15); break;
        case 516    :   object_attr_setlong (x, tll_sym_channel, 16); break;
        }

    if (x->flags & FLAG_HAVE_CHANGED)
        {
            systhread_mutex_lock            (&x->arrayMutex);
            pizSequenceRemoveSelectedNotes  (x->user);
            pizSequenceAddNotesWithArray    (x->user, x->selectedNotes, PIZ_SEQUENCE_ADD_FLAG_NONE);
            systhread_mutex_unlock          (&x->arrayMutex);
                
            x->flags &= ~FLAG_HAVE_CHANGED;
                
            DIRTYLAYER_SET (DIRTY_NOTES);
            
            DIRTYUNDO
        }
    
    if (returnedPopupValue >= 10 && returnedPopupValue <= 27) {
            DIRTYLAYER_SET (DIRTY_GRID);
        }
    
    if (returnedPopupValue >= 300 && returnedPopupValue <= 317) {
            DIRTYLAYER_SET (DIRTY_GRID);
        }
        
    jfont_destroy (font);

    jpopupmenu_destroy (popup);
    jpopupmenu_destroy (snapPopup);
    jpopupmenu_destroy (snapTripletPopup);
    jpopupmenu_destroy (snapDottedPopup);
    jpopupmenu_destroy (noteValuePopup);
    jpopupmenu_destroy (noteValueTripletPopup);
    jpopupmenu_destroy (noteValueDottedPopup);
    jpopupmenu_destroy (velocityPopup);
    jpopupmenu_destroy (noteChannelPopup);
    jpopupmenu_destroy (sequenceChannelPopup);
    jpopupmenu_destroy (slotsPopup);

    DIRTYSLOTS 
    DIRTYPATTR
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark PAINT
#pragma mark -

void tralala_paintTask (t_tralala *x) 
{   
    PIZError    err = PIZ_GOOD;
    ulong       dirty = x->dirtyLayer;
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) {
    
    DIRTYLAYER_UNSET (~(DIRTY_GRID | DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE | DIRTY_REFRESH | DIRTY_PLAYED));
    
    systhread_mutex_lock (&x->arrayMutex);
    
    if (LIVE && !(dirty & DIRTY_CHANGE) && ((x->flags & FLAG_IS_RUNNING) || (x->runIndex == -1))) 
        {
            if (tralala_hitNotesByRunIndex (x)) {           
                    dirty |= DIRTY_PLAYED;
                }
        }
        
    if (dirty)
        {   
            if (dirty & DIRTY_LOCATE_LEFT) {
                    x->windowOffsetX    -= AUTOSCROLL_STEP;
                    x->origin.x         += AUTOSCROLL_STEP; 
                }
            
            if (dirty & DIRTY_LOCATE_RIGHT) {
                    x->windowOffsetX    += AUTOSCROLL_STEP;
                    x->origin.x         -= AUTOSCROLL_STEP;
                }
            
            if (dirty & DIRTY_LOCATE_DOWN) {
                    x->windowOffsetY    += AUTOSCROLL_STEP;
                    x->origin.y         -= AUTOSCROLL_STEP;
                }
            
            if (dirty & DIRTY_LOCATE_UP) {
                    x->windowOffsetY    -= AUTOSCROLL_STEP;
                    x->origin.y         += AUTOSCROLL_STEP;
                }
                
            if (dirty & DIRTY_GRID) {   
                    jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_gridLayer);
                }
                
            if (dirty & DIRTY_ZONE)
                {
                    if (dirty & DIRTY_CHANGE)
                        {
                            PIZSequence *sequence = NULL;
                        
                            switch (x->sequenceMode) {
                                    case SEQUENCE_MODE_USER     : sequence = x->user;   break;
                                    case SEQUENCE_MODE_LIVE     : sequence = x->live;   break;
                                    case SEQUENCE_MODE_LISTEN   : err = PIZ_ERROR;      break;
                                }
                        
                            if (sequence)
                                {
                                    pizGrowingArrayClear (x->zone);
                                    
                                    if (x->flags & FLAG_ZONE_IS_SELECTED) {
                                        err = pizSequenceTempZoneToArray (sequence, x->zone);
                                    } else {
                                        err = pizSequenceZoneToArray (sequence, x->zone);
                                    }
                                }
                        }
                    
                    if (!err) {
                            jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_zoneLayer);
                        }
                }
                
            if (dirty & DIRTY_NOTES)
                {
                    if (dirty & DIRTY_CHANGE) {
                        pizGrowingArrayClear (x->unselectedNotes);
                        pizGrowingArrayClear (x->selectedNotes);
                            
                        if (USER) {
                            err = pizSequenceNotesToArray (x->user, x->unselectedNotes, x->selectedNotes);
                        } else if (LIVE) {
                            err = pizSequenceNotesToArray (x->live, x->unselectedNotes, x->unselectedNotes);
                        } else if (LISTEN) {
                            err = pizSequenceNotesToArray (x->listen, x->unselectedNotes, x->unselectedNotes);
                        }
                        
                        if (LIVE && ((x->flags & FLAG_IS_RUNNING) || (x->runIndex == -1))) {    
                                tralala_hitNotesByRunIndex (x);
                                dirty |= DIRTY_PLAYED;
                            }
                        }
                                                    
                    if (!err) {
                            jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_notesLayer);
                        }
                }
            
            if (dirty & DIRTY_PLAYED) {
                    jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_playedNotesLayer);
                }
            
            err = PIZ_GOOD;
            
            err |= pizGrowingArrayCopy (x->unselectedNotesCopy, x->unselectedNotes);
            err |= pizGrowingArrayCopy (x->selectedNotesCopy, x->selectedNotes);
            err |= pizGrowingArrayCopy (x->playedNotesCopy, x->playedNotes);
            err |= pizGrowingArrayCopy (x->zoneCopy, x->zone);
            
            if (!err) {
                    jbox_redraw ((t_jbox *)x);
                }
        }
    
    systhread_mutex_unlock  (&x->arrayMutex);
           
    } ATOMIC_DECREMENT (&x->paintLock);
    
    clock_fdelay (x->paintClock, PAINT_CLOCK_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
}

void tralala_focusTask (t_tralala *x)
{
    x->flags |= FLAG_FOCUS;
    
    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
}

void tralala_notifyTask (t_tralala *x)
{
    object_notify (x, tll_sym_modified, NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paint (t_tralala *x, t_object *patcherview)
{   
    if (!(x->flags & FLAG_INIT_PAINT_CLOCK)) {
        clock_fdelay (x->paintClock, PAINT_CLOCK_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
        x->flags |= FLAG_INIT_PAINT_CLOCK;
    }
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) 
        {
            tralala_paintGrid (x, patcherview);
            
            if (!LISTEN) {
                    tralala_paintZone (x, patcherview);
                }
            
            tralala_paintNotes (x, patcherview);
            
            if (LIVE && !(x->flags & FLAG_IS_MUTED)) {
                    tralala_paintPlayedNotes (x, patcherview);
                }

            if (x->viewText) {
                    tralala_paintText (x, patcherview);
                }
            
            if (x->flags & FLAG_IS_LASSO) {
                    tralala_paintLasso (x, patcherview);
                }
            
            ATOMIC_DECREMENT (&x->paintLock);
        }
    else
        {
            DIRTYLAYER_SET (DIRTY_REFRESH);
            
            ATOMIC_DECREMENT (&x->paintLock);
        }
}

void tralala_getdrawparams (t_tralala *x, t_object *patcherview, t_jboxdrawparams *params)
{
    jrgba_copy (&params->d_boxfillcolor, &x->backgroundColor);

    if (x->flags & FLAG_FOCUS)
        {
            jrgba_copy (&params->d_bordercolor, &x->focusedBorderColor);
        }
    else
        {
            jrgba_copy (&params->d_bordercolor, &x->unfocusedBorderColor);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_focusgained (t_tralala *x, t_object *patcherview)
{
    clock_fdelay (x->focusClock, FOCUS_CLOCK_INTERVAL);
}

void tralala_focuslost (t_tralala *x, t_object *patcherview)
{
    clock_unset (x->focusClock);
    
    x->flags &= ~FLAG_FOCUS;
    
    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
    
    if (USER)
        {
            if (x->flags & FLAG_ZONE_IS_SELECTED)
                {
                    pizSequenceSetZoneByTempZone (x->user);
                    
                    x->flags &= ~FLAG_ZONE_IS_SELECTED;
                    DIRTYLAYER_SET (DIRTY_CHANGE);
                }
            
            if (x->flags & FLAG_IS_LASSO)
                {
                    x->flags &= ~FLAG_IS_LASSO;
                    DIRTYLAYER_SET (DIRTY_REFRESH);
                }
        
            tralala_stopAutoscroll (x);
            
            x->hitTest  = HIT_NOTHING;
        }
    
    if (x->textMode != TEXT_MODE_NOTE)
        {
            x->textMode = TEXT_MODE_NOTE;
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }

    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
}

void tralala_patcherview_vis (t_tralala *x, t_object *patcherview)
{
    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
    
    clock_fdelay (x->paintClock, 0.);
}

void tralala_patcherview_invis (t_tralala *x, t_object *patcherview)
{
    clock_unset  (x->paintClock);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintText (t_tralala *x, t_object *patcherview)
{
    long                            i;
    bool                            draw = false;
    double                          k = TEXT_CELL_SPACE;
    t_jgraphics                     *g = NULL;
    t_jfont                         *font = NULL;
    t_symbol                        *fontName = NULL;
    t_rect                          rect;
    long                            fontSlant, fontWeight, numlines;
    double                          fontSize, textHeight;
    t_atom                          color[4];
    char                            textCell[STRING_MAXIMUM_SIZE];
    t_jrgba                         backgroundTextColor;
    t_jgraphics_textlayout_flags    flags = (t_jgraphics_textlayout_flags)
                                    (JGRAPHICS_TEXTLAYOUT_NOWRAP | JGRAPHICS_TEXTLAYOUT_USEELLIPSIS); 
    
    snprintf (textCell, STRING_MAXIMUM_SIZE, "Error");
    
    g = (t_jgraphics *)patcherview_get_jgraphics (patcherview);
    jbox_get_rect_for_view ((t_object *)x, patcherview, &rect); 
    
    fontName    = jbox_get_fontname (((t_object *)x));
    fontWeight  = jbox_get_font_weight ((t_object *)x);
    fontSlant   = jbox_get_font_slant ((t_object *)x);
    fontSize    = jbox_get_fontsize ((t_object *)x);
    
    font = jfont_create (fontName->s_name, fontSlant, fontWeight, fontSize); 

    jrgba_to_atoms  (&x->backgroundColor, color);
    atom_setfloat   (color + 3, TEXT_BACKGROUND_ALPHA);
    atoms_to_jrgba  (4, color, &backgroundTextColor);
    
    if (x->textMode == TEXT_MODE_NOTE && USER)
        {
            if (pizSequenceHasMarkedNote (x->user))
                {
                    long textValues[TEXT_CELL_COUNT];           
                    
                    textValues[TEXT_ORDER_VELOCITY] = pizSequenceMarkedNoteValue (x->user, PIZ_VELOCITY);
                    textValues[TEXT_ORDER_DURATION] = pizSequenceMarkedNoteValue (x->user, PIZ_DURATION);
                    textValues[TEXT_ORDER_CHANNEL]  = pizSequenceMarkedNoteValue (x->user, PIZ_CHANNEL);
                    textValues[TEXT_ORDER_PITCH]    = pizSequenceMarkedNoteValue (x->user, PIZ_PITCH);
                    
                    x->textPosition[0] = TEXT_CELL_SPACE;
                    
                    for (i = 0; i < TEXT_CELL_COUNT; i++)
                        {
                            switch (i)  {
                            case TEXT_ORDER_PITCH       : tralala_setStringWithLong (textCell, textValues[i],
                                                            FORMAT_MODE_NOTENAME); break;
                            case TEXT_ORDER_DURATION    : tralala_setStringWithLong (textCell, textValues[i], 
                                                            FORMAT_MODE_TICKS); break;
                            default                     : tralala_setStringWithLong (textCell, textValues[i], 
                                                            FORMAT_MODE_LONG); break;
                            }
                            
                            jtextlayout_set (x->textLayers[i], textCell, font, x->textPosition[i], 
                                (rect.height - (fontSize + TEXT_CELL_SPACE)), (rect.width - k), fontSize, 
                                (t_jgraphics_text_justification) (JGRAPHICS_TEXT_JUSTIFICATION_LEFT), flags);
                            
                            jtextlayout_measure (x->textLayers[i], 0, -1, 1, &x->textWidth[i], 
                                &textHeight, &numlines);
                            
                            k += (x->textWidth[i] + TEXT_CELL_SPACE);
                            
                            if (i < (TEXT_CELL_COUNT - 1)) {
                            x->textPosition[i + 1] = x->textPosition[i] + (x->textWidth[i] + TEXT_CELL_SPACE);
                            }
                            
                            if (x->flags & FLAG_FOCUS)
                                {
                                    if (x->textIsSelected[i]) 
                                        {
                                            jtextlayout_settextcolor (x->textLayers[i], &x->selectedTextColor);
                                        }
                                    else
                                        {
                                            jtextlayout_settextcolor (x->textLayers[i], &x->focusedTextColor);
                                        }
                                }
                            else
                                {
                                    jtextlayout_settextcolor (x->textLayers[i], &x->unfocusedTextColor);
                                }
                        }
                    
                    jgraphics_set_source_jrgba (g, &backgroundTextColor);

                    jgraphics_rectangle_fill_fast (g, 0., (rect.height - (fontSize + TEXT_CELL_SPACE)), 
                        k, textHeight);
                
                    for (i = 0; i < TEXT_CELL_COUNT; i++) {
                            jtextlayout_draw (x->textLayers[i], g);
                        }
                }
            else
                {
                    char         temp[STRING_MAXIMUM_SIZE];
                    PIZSnapValue grid = pizSequenceGrid (x->user);
                    PIZSnapValue value = pizSequenceNoteValue (x->user);
                    
                    snprintf (textCell, STRING_MAXIMUM_SIZE, "Slot %ld", x->slotIndex);
                    textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    
                    tralala_setStringWithLong (temp, grid, FORMAT_MODE_GRID);
                    
                    snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / %s", textCell, temp);
                    textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    
                    if (value != PIZ_SNAP_NONE) {
                            tralala_setStringWithLong (temp, value, FORMAT_MODE_GRID);
                    
                            snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / %s", textCell, temp);
                            textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                        }
                    
                    draw = true;
                }
        }
    else 
        {
            if (x->textMode == TEXT_MODE_MOUSE_PITCH)
                {
                    tralala_setStringWithLong (textCell, x->mousePitchValue, FORMAT_MODE_NOTENAME);
                    
                    draw = true;
                }
            else if (x->textMode == TEXT_MODE_MOUSE_VELOCITY)
                {
                    char textVelocity [STRING_MAXIMUM_SIZE];
                    
                    tralala_setStringWithLong (textVelocity, x->mouseVelocityValue, FORMAT_MODE_LONG);
                    
                    if (x->mouseVelocityValue >= 0) 
                        {
                            snprintf (textCell, STRING_MAXIMUM_SIZE, "Velocity : +%s", textVelocity);
                        } 
                    else 
                        {
                            snprintf (textCell, STRING_MAXIMUM_SIZE, "Velocity : %s", textVelocity);
                        }
                    
                    textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    
                    draw = true;
                }
            else if (x->textMode == TEXT_MODE_ZONE && pizGrowingArrayCount (x->zoneCopy))
                {
                    char         textDown[STRING_MAXIMUM_SIZE];
                    char         textUp [STRING_MAXIMUM_SIZE];
                    long         s      = -1;
                    long         start  = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_START);
                    long         end    = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_END);
                    long         down   = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_DOWN);
                    long         up     = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_UP);
                    PIZSnapValue grid   = pizSequenceGrid (x->user);
                            
                    tralala_setStringWithLong (textDown, down, FORMAT_MODE_NOTENAME);
                    tralala_setStringWithLong (textUp, up, FORMAT_MODE_NOTENAME);
                    
                    if (grid == PIZ_QUARTER_NOTE || grid == PIZ_EIGHTH_NOTE || grid == PIZ_SIXTEENTH_NOTE) 
                        {
                            long temp = (end - start) / grid;
                            
                            if ((temp * grid) == (end - start)) 
                                {
                                    s = temp;
                                }
                            
                            if ((grid == PIZ_EIGHTH_NOTE && s == 8) || (grid == PIZ_SIXTEENTH_NOTE && s == 16))
                                {
                                    s = 4;
                                    grid = PIZ_QUARTER_NOTE;
                                }
                            
                            if ((grid == PIZ_QUARTER_NOTE && s == 6) || (grid == PIZ_SIXTEENTH_NOTE && s == 24))
                                {
                                    s = 12;
                                    grid = PIZ_EIGHTH_NOTE;
                                }
                        }
                    
                    if (s != -1) 
                        {
                            if (grid == PIZ_QUARTER_NOTE) {
                                snprintf (textCell, STRING_MAXIMUM_SIZE, "%s %s : %ld/4", textDown, textUp, s);
                            } else if (grid == PIZ_EIGHTH_NOTE) {
                                snprintf (textCell, STRING_MAXIMUM_SIZE, "%s %s : %ld/8", textDown, textUp, s);
                            } else if (grid == PIZ_SIXTEENTH_NOTE) {
                                snprintf (textCell, STRING_MAXIMUM_SIZE, "%s %s : %ld/16", textDown, textUp, s);
                            }
                        } 
                    else 
                        {
                            snprintf (textCell, STRING_MAXIMUM_SIZE, "%s %s", textDown, textUp);
                        }
                    
                    textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    
                    draw = true;
                }
            else if (LIVE)
                {
                    snprintf (textCell, STRING_MAXIMUM_SIZE, "%s", x->scaleKey->s_name);
                    textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    
                    if (x->scaleType != tll_sym_none) {
                        snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / %s", textCell, x->scaleType->s_name);
                        textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    }
                    
                    if (x->patternCell != tll_sym_none) {
                        snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / %s", textCell, x->patternCell->s_name);
                        textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    }
                                
                    if (x->patternSize) {
                        snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / pattern", textCell);
                        textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    }
                    
                    if (x->velocity > 0) {
                        snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / +%ld", textCell, x->velocity);
                        textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    } else if (x->velocity < 0) {
                        snprintf (textCell, STRING_MAXIMUM_SIZE, "%s / %ld", textCell, x->velocity);
                        textCell[STRING_MAXIMUM_SIZE - 1] = 0;
                    }
                            
                    draw = true;
                }
        }
    
    if (draw) {
        jtextlayout_set (x->textLayers[0], textCell, font, k, 
            (rect.height - (fontSize + TEXT_CELL_SPACE)), (rect.width - k), fontSize, 
            (t_jgraphics_text_justification) (JGRAPHICS_TEXT_JUSTIFICATION_LEFT), flags);
        
        jtextlayout_measure (x->textLayers[0], 0, -1, 1, &x->textWidth[0], &textHeight, &numlines);
        
        x->textPosition[0] = TEXT_CELL_SPACE;
        k += x->textWidth[0] + TEXT_CELL_SPACE;
        
        if (x->flags & FLAG_FOCUS)
            {
                jtextlayout_settextcolor (x->textLayers[0], &x->focusedTextColor); 
            }
        else
            {
                jtextlayout_settextcolor (x->textLayers[0], &x->unfocusedTextColor);
            }
        
        jgraphics_set_source_jrgba      (g, &backgroundTextColor);
        jgraphics_rectangle_fill_fast   (g, 0., (rect.height - (fontSize + TEXT_CELL_SPACE)), k, textHeight);

        jtextlayout_draw (x->textLayers[0], g);
    }
            
    jfont_destroy (font);
}
            
void tralala_paintGrid (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
        
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_gridLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * STEP_PIXELS_SIZE * f, (PIZ_MAGIC_PITCH + 1) * SEMITONE_PIXELS_SIZE * f);

    if (g) 
        {
            long            i;
            t_rect          srcRect, destRect;
            double          imageWidth, imageHeight, gridWidth, gridHeight;
            t_jrgba         gridColor;
            t_jsurface      *background = NULL;
            long            z = x->zoomMode;
            PIZSnapValue    grid = PIZ_SNAP_NONE;
            
            if (LIVE) {
                    grid = x->cell;
                } 
            
            if (grid == PIZ_SNAP_NONE) {
                    grid = pizSequenceGrid (x->user);
                }
            
            switch (grid) {
                case PIZ_WHOLE_NOTE_DOTTED          :   background = tll_half                   [z]; break;
                case PIZ_WHOLE_NOTE                 :   background = tll_whole                  [z]; break;
                case PIZ_WHOLE_NOTE_TRIPLET         :   background = tll_halfTriplet            [z]; break;
                case PIZ_HALF_NOTE_DOTTED           :   background = tll_halfDotted             [z]; break;
                case PIZ_HALF_NOTE                  :   background = tll_half                   [z]; break;
                case PIZ_HALF_NOTE_TRIPLET          :   background = tll_halfTriplet            [z]; break;
                case PIZ_QUARTER_NOTE_DOTTED        :   background = tll_quarterDotted          [z]; break;
                case PIZ_QUARTER_NOTE               :   background = tll_quarter                [z]; break;
                case PIZ_QUARTER_NOTE_TRIPLET       :   background = tll_quarterTriplet         [z]; break;
                case PIZ_EIGHTH_NOTE_DOTTED         :   background = tll_eighthDotted           [z]; break;
                case PIZ_EIGHTH_NOTE                :   background = tll_eighth                 [z]; break;
                case PIZ_EIGHTH_NOTE_TRIPLET        :   background = tll_eighthTriplet          [z]; break;
                case PIZ_SIXTEENTH_NOTE_DOTTED      :   background = tll_sixteenthDotted        [z]; break;
                case PIZ_SIXTEENTH_NOTE             :   background = tll_sixteenth              [z]; break;
                case PIZ_SIXTEENTH_NOTE_TRIPLET     :   background = tll_sixteenthTriplet       [z]; break;
                case PIZ_THIRTY_SECOND_NOTE         :   background = tll_thirtySecond           [z]; break;
                case PIZ_THIRTY_SECOND_NOTE_TRIPLET :   background = tll_sixteenthTriplet       [z]; break;
                case PIZ_SNAP_NONE                  :   background = tll_eighth                 [z]; break;
            }
                
            imageWidth  = jgraphics_image_surface_get_width (background);
            imageHeight = jgraphics_image_surface_get_height (background);
            
            gridWidth   = PIZ_SEQUENCE_TIMELINE_SIZE * STEP_PIXELS_SIZE * f;
            gridHeight  = (PIZ_MAGIC_PITCH + 1) * SEMITONE_PIXELS_SIZE * f;
            
            srcRect.x       = 0.;
            srcRect.y       = 0.;
            srcRect.width   = imageWidth;
            srcRect.height  = imageHeight;
            
            for (i = 0; i < ((PIZ_MAGIC_PITCH + 1) / JSURFACE_MOSAIC_PITCH_SIZE); i ++)
                {
                    long j;
                    
                    for (j = 0; j < (PIZ_SEQUENCE_TIMELINE_SIZE / JSURFACE_MOSAIC_STEP_SIZE); j++)
                        {
                            destRect.x      = j * imageWidth;
                            destRect.y      = i * imageHeight;
                            destRect.width  = imageWidth;
                            destRect.height = imageHeight;
                    
                            jgraphics_image_surface_draw (g, background, srcRect, destRect);
                        }
                }
        
            jrgba_set (&gridColor, 0.87, 0.87, 0.87, 1.);
            jgraphics_set_source_jrgba (g, &gridColor);
            
            jgraphics_rectangle_draw_fast (g, 0., 0., gridWidth, gridHeight, 1.);
            
            jbox_end_layer ((t_object*)x, patcherview, tll_sym_gridLayer);
        }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_gridLayer, -x->windowOffsetX, -x->windowOffsetY);
}

void tralala_paintNotes (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_notesLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * STEP_PIXELS_SIZE * f, (PIZ_MAGIC_PITCH + 1) * SEMITONE_PIXELS_SIZE * f);
    
    if (g) 
        {
            long    i, notesCount; 
            t_jrgba color1, color2, color3;
            long    markedNotePosition  = -1;
            long    markedNotePitch     = -1;
            long    markedNoteVelocity  = -1;
            long    markedNoteDuration  = -1;
            
            if (!(x->flags & FLAG_FOCUS))
                {
                    switch (x->sequenceMode) {
                        case SEQUENCE_MODE_USER   : jrgba_copy (&color3, &x->unfocusedUserSelectedNoteColor);
                                                    jrgba_copy (&color2, &x->unfocusedUserSelectedNoteColor);
                                                    jrgba_copy (&color1, &x->unfocusedUserNoteColor);
                                                    break;
                        case SEQUENCE_MODE_LIVE   : jrgba_copy (&color1, &x->unfocusedLiveNoteColor);
                                                    break;
                        case SEQUENCE_MODE_LISTEN : jrgba_copy (&color1, &x->unfocusedListenNoteColor);
                                                    break;
                        }
                }
            else 
                {
                    switch (x->sequenceMode) {
                        case SEQUENCE_MODE_USER   : jrgba_copy (&color3, &x->focusedUserMarkedNoteColor);
                                                    jrgba_copy (&color2, &x->focusedUserSelectedNoteColor);
                                                    break;
                        case SEQUENCE_MODE_LIVE   : jrgba_copy (&color2, &x->focusedLivePlayedNoteColor);
                                                    jrgba_copy (&color1, &x->focusedLiveNoteColor);
                                                    break;
                        case SEQUENCE_MODE_LISTEN : jrgba_copy (&color1, &x->focusedListenNoteColor);
                                                    break;
                        }
                }

            notesCount = pizGrowingArrayCount (x->unselectedNotesCopy) / PIZ_SEQUENCE_NOTE_SIZE;
            
            if ((x->flags & FLAG_FOCUS) && USER)
                {
                    for (i = 0; i < notesCount; i++)
                        {
                            long position   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                            long pitch      = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                            long velocity   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY);
                            long duration   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
                            
                            tralala_paintNoteCandycane (x, g, position, pitch, velocity, duration);
                        }
                }
            else
                {
                    for (i = 0; i < notesCount; i++)
                        {
                            long position   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                            long pitch      = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                            long velocity   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY);
                            long duration   = pizGrowingArrayValueAtIndex (x->unselectedNotesCopy, 
                                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
                                                        
                            tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color1);
                        }
                }
            
            notesCount = pizGrowingArrayCount (x->selectedNotesCopy) / PIZ_SEQUENCE_NOTE_SIZE;
            
            for (i = 0; i < notesCount; i++)
                {
                    long position   = pizGrowingArrayValueAtIndex (x->selectedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                    long pitch      = pizGrowingArrayValueAtIndex (x->selectedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                    long velocity   = pizGrowingArrayValueAtIndex (x->selectedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY);
                    long duration   = pizGrowingArrayValueAtIndex (x->selectedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
                    long isMarked   = pizGrowingArrayValueAtIndex (x->selectedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_IS_MARKED);
                    
                    if (!isMarked)
                        {
                            tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color2);
                        }
                    else
                        {
                            markedNotePosition  = position;
                            markedNotePitch     = pitch;
                            markedNoteVelocity  = velocity;
                            markedNoteDuration  = duration;
                        }
                }
            
            if (markedNotePosition != -1) {
                    tralala_paintNoteWithColor (x, g, markedNotePosition, markedNotePitch, markedNoteVelocity,
                        markedNoteDuration, &color3);
                }
        
            jbox_end_layer ((t_object*)x, patcherview, tll_sym_notesLayer);
        }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_notesLayer, -x->windowOffsetX, -x->windowOffsetY);
}

void tralala_paintPlayedNotes (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * STEP_PIXELS_SIZE * f, (PIZ_MAGIC_PITCH + 1) * SEMITONE_PIXELS_SIZE * f);
    
    if (g) 
        {
            long    i, notesCount; 
            t_jrgba color;
                
            if (x->flags & FLAG_FOCUS)
                {
                    jrgba_copy (&color, &x->focusedLivePlayedNoteColor);
                }
            else 
                {
                    jrgba_copy (&color, &x->unfocusedLivePlayedNoteColor);
                }

            notesCount = pizGrowingArrayCount (x->playedNotesCopy) / PIZ_SEQUENCE_NOTE_SIZE;
            
            for (i = 0; i < notesCount; i++)
                {
                    long position   = pizGrowingArrayValueAtIndex (x->playedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                    long pitch      = pizGrowingArrayValueAtIndex (x->playedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                    long velocity   = pizGrowingArrayValueAtIndex (x->playedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY);
                    long duration   = pizGrowingArrayValueAtIndex (x->playedNotesCopy, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
                    
                    tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color);
                }
            
            jbox_end_layer ((t_object*)x, patcherview, tll_sym_playedNotesLayer);
        }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, -x->windowOffsetX, 
        -x->windowOffsetY);
}

void tralala_paintNoteCandycane (t_tralala *x, t_jgraphics *g, long position, long pitch,
    long velocity, long duration)
{
    long            noteTone;
    t_rect          noteRect;
    PIZCoordinates  coordinates;
    t_atom          temp[4];
    t_jrgba         color;
    double          alpha = 0.75;
    
    coordinates.position    = position;
    coordinates.pitch       = pitch;
    
    tralala_setRectWithCoordinatesAndDuration (x, &noteRect, &coordinates, duration);
    
    noteTone = pitch % 12;
    
    switch (noteTone) {
        case 0  : jrgba_to_atoms (&x->cNoteColor, temp);        break;
        case 1  : jrgba_to_atoms (&x->cSharpNoteColor, temp);   break;
        case 2  : jrgba_to_atoms (&x->dNoteColor, temp);        break;
        case 3  : jrgba_to_atoms (&x->dSharpNoteColor, temp);   break;
        case 4  : jrgba_to_atoms (&x->eNoteColor, temp);        break;
        case 5  : jrgba_to_atoms (&x->fNoteColor, temp);        break;
        case 6  : jrgba_to_atoms (&x->fSharpNoteColor, temp);   break;
        case 7  : jrgba_to_atoms (&x->gNoteColor, temp);        break;
        case 8  : jrgba_to_atoms (&x->gSharpNoteColor, temp);   break;
        case 9  : jrgba_to_atoms (&x->aNoteColor, temp);        break;
        case 10 : jrgba_to_atoms (&x->aSharpNoteColor, temp);   break;
        case 11 : jrgba_to_atoms (&x->bNoteColor, temp);        break;
        } 

    alpha = (double)(velocity + VELOCITY_PAINT_OFFSET) / (double)PIZ_MAGIC_VELOCITY;
    
    if (alpha > 0.005)
        {
            atom_setfloat   (temp + 3, CLAMP (alpha, 0.25, 1.));
            atoms_to_jrgba  (4, temp, &color);
        }
    else
        {
            jrgba_copy (&color, &x->focusedUserSilentNoteColor);
        }
    
    jgraphics_set_source_jrgba (g, &color);
    jgraphics_rectangle_fill_fast (g, noteRect.x, noteRect.y, noteRect.width, noteRect.height);
}                                                       
                                                            
void tralala_paintNoteWithColor (t_tralala *x, t_jgraphics *g, long position, long pitch, long velocity, 
        long duration, t_jrgba *color)
{
    t_rect          noteRect;
    PIZCoordinates  coordinates;
    t_jrgba         noteColor;
    t_atom          temp[4];
    double          alpha;
    
    coordinates.position    = position;
    coordinates.pitch       = pitch;
    
    tralala_setRectWithCoordinatesAndDuration (x, &noteRect, &coordinates, duration);
    
    alpha = (double)velocity / (double)PIZ_MAGIC_VELOCITY;
    
    jrgba_to_atoms  (color, temp);
    atom_setfloat   (temp + 3, CLAMP (alpha, 0.25, 1.));
    atoms_to_jrgba  (4, temp, &noteColor);
        
    jgraphics_set_source_jrgba (g, &noteColor);
    jgraphics_rectangle_fill_fast (g, noteRect.x, noteRect.y, noteRect.width, noteRect.height);
}

void tralala_paintZone (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;

    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * STEP_PIXELS_SIZE * f, (PIZ_MAGIC_PITCH + 1) * SEMITONE_PIXELS_SIZE * f);
    
    if (g) 
        {
            if (pizGrowingArrayCount (x->zoneCopy))
                {
                    t_rect  zoneRect;
                    long    start   = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_START);
                    long    end     = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_END);
                    long    down    = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_DOWN);
                    long    up      = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_SEQUENCE_UP);
                    
                    tralala_setRectWithZoneValues (x, &zoneRect, start, end, down, up);
                                
                    if (!(x->flags & FLAG_FOCUS))
                        {
                            jgraphics_set_source_jrgba (g, &x->unfocusedZoneColor);
                        }
                    else if (x->flags & FLAG_ZONE_IS_SELECTED)
                        {
                            jgraphics_set_source_jrgba (g, &x->focusedSelectedZoneColor);
                        }
                    else if (USER)
                        {
                            jgraphics_set_source_jrgba (g, &x->focusedUserZoneColor);
                        }
                    else 
                        {
                            jgraphics_set_source_jrgba (g, &x->focusedLiveZoneColor);
                        }

                    jgraphics_rectangle_fill_fast (g, zoneRect.x, zoneRect.y, zoneRect.width, zoneRect.height);
                }
                
            jbox_end_layer ((t_object*)x, patcherview, tll_sym_zoneLayer);
        }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, -x->windowOffsetX, -x->windowOffsetY);
}

void tralala_paintLasso (t_tralala *x, t_object *patcherview)
{
    double a, b, u, v, width, height;
    
    t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics (patcherview);

    a       = MIN (x->origin.x, x->point.x);
    b       = MIN (x->origin.y, x->point.y);
    u       = MAX (x->origin.x, x->point.x);
    v       = MAX (x->origin.y, x->point.y);
    width   = u - a;
    height  = v - b;
            
    jgraphics_set_source_jrgba (g, &x->lassoColor);
            
    jgraphics_rectangle_fill_fast (g, a, b, width, height);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark LIBRARY
#pragma mark -

bool tralala_moveSelectedNotes (t_tralala *x, long deltaPosition, long deltaPitch)
{
    long i;
    long count;
    bool moved = false;
    long grid = pizSequenceGrid (x->user);
        
    systhread_mutex_lock (&x->arrayMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++)
        {
            long previousPosition   = pizGrowingArrayValueAtIndex (x->selectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
            long previousPitch      = pizGrowingArrayValueAtIndex (x->selectedNotes,
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
            long position           = pizGrowingArrayValueAtIndex (x->originNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
            long pitch              = pizGrowingArrayValueAtIndex (x->originNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
            long duration           = pizGrowingArrayValueAtIndex (x->originNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
            
            pitch       += deltaPitch;
            position    += deltaPosition;
            
            position = MAX ((long)((position / (double)grid) + 0.5) * grid, 0);
            
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION,
                                            CLAMP (position, 0, PIZ_SEQUENCE_TIMELINE_SIZE - duration));
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH,
                                            CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
            
            if ((previousPosition != position) || (previousPitch != pitch)) {
                    moved = true;
                }
        }
    
    systhread_mutex_unlock (&x->arrayMutex);
    
    return moved;
}

bool tralala_changeSelectedNotesDuration (t_tralala *x, long deltaPosition)
{
    long i;
    long count;
    bool changed = false;
    long grid = pizSequenceGrid (x->user);
                
    systhread_mutex_lock (&x->arrayMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++)
        {
            long previousDuration   = pizGrowingArrayValueAtIndex (x->selectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
            long duration           = pizGrowingArrayValueAtIndex (x->originNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
            long position           = pizGrowingArrayValueAtIndex (x->originNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
            long maximum            = MIN (PIZ_SEQUENCE_MAXIMUM_DURATION, 
                                        (PIZ_SEQUENCE_TIMELINE_SIZE - position));
                                        
            duration += deltaPosition;
            duration = MAX ((long)((duration / (double)grid) + 0.5) * grid, 0);
            
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION,
                                            CLAMP (duration, grid, maximum));
            
            if (duration != previousDuration) {
                    changed = true;
                }
        }
    
    systhread_mutex_unlock (&x->arrayMutex);
    
    return changed;
}

void tralala_duplicateSelectedNotes (t_tralala *x)
{
    systhread_mutex_lock        (&x->arrayMutex);
    pizGrowingArrayAppendArray  (x->unselectedNotes, x->selectedNotes);
    systhread_mutex_unlock      (&x->arrayMutex);
            
    pizSequenceUnselectAllNotes (x->user);
            
    x->flags |= FLAG_HAVE_BEEN_DUPLICATED;
}

void tralala_changeSelectedNotesVelocity (t_tralala *x, bool decrement)
{
    long count;
    
    systhread_mutex_lock (&x->arrayMutex);
    
    if (count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE)
        {
            long i, step;
            long temp, k = 0;
            long originVelocity = pizGrowingArrayValueAtIndex (x->originNotes, PIZ_SEQUENCE_VELOCITY);
            long velocity       = pizGrowingArrayValueAtIndex (x->selectedNotes, PIZ_SEQUENCE_VELOCITY);
            
            step = (long)((ABS (velocity - originVelocity)) / 5) + 1;
            
            for (i = 0; i < count; i++) 
                {
                    if (decrement) 
                        {
                            temp =  pizGrowingArrayValueAtIndex (x->selectedNotes, 
                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY) - step;
                        }
                    else
                        {
                            temp = PIZ_MAGIC_VELOCITY - (pizGrowingArrayValueAtIndex (x->selectedNotes,
                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY) + step);
                        }
                    
                    if (temp < k) {
                            k = temp;
                        }
                }
            
            step += k;
            
            if (step > 0)
                {
                    if (decrement) {
                            step = -step;
                        }
                        
                    for (i = 0; i < count; i++)
                        { 
                            temp = pizGrowingArrayValueAtIndex (x->selectedNotes, 
                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY) + step;
                            pizGrowingArraySetValueAtIndex (x->selectedNotes,  
                                (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY, temp);
                        }
                    
                    x->mouseVelocityValue = velocity + step - originVelocity;
                }
        }
    
    systhread_mutex_unlock (&x->arrayMutex);
}

void tralala_setSelectedNotesVelocity (t_tralala *x, long velocity)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->arrayMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY,
            CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY));
    }
    
    systhread_mutex_unlock (&x->arrayMutex);
}

void tralala_setSelectedNotesChannel (t_tralala *x, long channel)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->arrayMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_CHANNEL,
            CLAMP (channel, 0, PIZ_MAGIC_CHANNEL));
    }
    
    systhread_mutex_unlock (&x->arrayMutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long tralala_hitZoneWithPoint (t_tralala *x, t_pt pt)
{
    t_rect  zoneRect;
    bool    a, b, c, d, e, f;
    long    start, end, down, up;
    long    hitTest = HIT_NOTHING;

    systhread_mutex_lock (&x->arrayMutex);

    start   = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_START);
    end     = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_END);
    down    = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_DOWN);
    up      = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_UP);

    systhread_mutex_unlock  (&x->arrayMutex);
                        
    tralala_setRectWithZoneValues (x, &zoneRect, start, end, down, up);
    
    zoneRect.x -= x->windowOffsetX;
    zoneRect.y -= x->windowOffsetY;
    
    a = ((ABS (pt.x - zoneRect.x)) < HIT_ZONE_RANGE);
    b = ((ABS (pt.x - (zoneRect.x + zoneRect.width))) < HIT_ZONE_RANGE);
    c = ((pt.x > (zoneRect.x + (HIT_ZONE_RANGE / 2.))) 
            && (pt.x < (zoneRect.x + zoneRect.width - (HIT_ZONE_RANGE / 2.))));
    
    d = ((ABS (pt.y - (zoneRect.y + zoneRect.height))) < HIT_ZONE_RANGE);
    e = ((ABS (pt.y - zoneRect.y)) < HIT_ZONE_RANGE);
    f = ((pt.y > (zoneRect.y + (HIT_ZONE_RANGE / 2.))) 
            && (pt.y < (zoneRect.y + zoneRect.height - (HIT_ZONE_RANGE / 2.))));
                        
    if (a && f) {
            hitTest |= HIT_START;
        }
    if (b && f) {
            hitTest |= HIT_END;
        }
    if (d && c) {
            hitTest |= HIT_DOWN;
        }
    if (e && c) {
            hitTest |= HIT_UP;
        }
    if (c && f) {
            hitTest |= HIT_ZONE;
        }
    
    return hitTest;
}

long tralala_hitTextWithPoint (t_tralala *x, t_object *patcherview, t_pt pt)
{
    long    isHit = HIT_NOTHING;
    long    i, numlines;
    double  textWidth, textHeight, fontSize, h1, h2; 
    t_rect  rect;
            
    jbox_get_rect_for_view ((t_object *)x, patcherview, &rect);
    fontSize = jbox_get_fontsize ((t_object *)x);
    jtextlayout_measure (x->textLayers[0], 0, -1, 1, &textWidth, &textHeight, &numlines);

    h1 = rect.height - (TEXT_CELL_SPACE + fontSize);
    h2 = h1 + textHeight;
    
    if ((pt.y > h1) && (pt.y < h2)) {
        if (pizSequenceHasMarkedNote (x->user)) {
            if ((pt.x > 0) && (pt.x < (x->textPosition[TEXT_CELL_COUNT - 1] 
                + x->textWidth[TEXT_CELL_COUNT - 1] + TEXT_CELL_SPACE))) {
                    isHit = HIT_LOCK;
                }
        
            for (i = 0; i < TEXT_CELL_COUNT; i++) {
                if ((pt.x > x->textPosition[i]) && (pt.x < (x->textPosition[i] + x->textWidth[i]))) {
                        tralala_unselectAllText (x);
                        x->textIsSelected[i] = true;
                        isHit = HIT_TEXT;
                    }
            }
        } else if ((pt.x > x->textPosition[0]) && (pt.x < (x->textPosition[0] + x->textWidth[0]))) {
            isHit = HIT_LOCK;
        }
    }

    return isHit;
}

bool tralala_hitNotesByRunIndex (t_tralala *x)
{
    bool needToDraw = false;

    pizGrowingArrayClear (x->playedNotes);

    if (x->runIndex == -1)
        {
            x->runIndex = 0;
            needToDraw  = true;
        }
    else 
        {
            long i, count, start, end, down, up;
            long err = PIZ_GOOD;
            
            start   = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_START);
            end     = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_END);
            down    = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_DOWN);
            up      = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_UP);
            
            count = pizGrowingArrayCount (x->unselectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
            
            for (i = 0; i < count; i++)
                {
                    long position   = pizGrowingArrayValueAtIndex (x->unselectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION);
                    long pitch      = pizGrowingArrayValueAtIndex (x->unselectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH);
                    long duration   = pizGrowingArrayValueAtIndex (x->unselectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION);
                    long isMarked   = pizGrowingArrayValueAtIndex (x->unselectedNotes, 
                                        (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_IS_MARKED);
                                        
                    if ((position >= start) && 
                        (pitch >= down) &&
                        (pitch <= up) &&
                        (x->runIndex >= position) && 
                        (x->runIndex < (position + duration)))
                        {
                            err |= pizGrowingArrayAppend (x->playedNotes, position);
                            err |= pizGrowingArrayAppend (x->playedNotes, pitch);
                            err |= pizGrowingArrayAppend (x->playedNotes, PIZ_MAGIC_VELOCITY);
                            err |= pizGrowingArrayAppend (x->playedNotes, duration);
                            err |= pizGrowingArrayAppend (x->playedNotes, false);
                            err |= pizGrowingArrayAppend (x->playedNotes, false);
                            err |= pizGrowingArrayAppend (x->playedNotes, false);
                            
                            if (!isMarked) {
                                    pizGrowingArraySetValueAtIndex (x->unselectedNotes, 
                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_IS_MARKED, true);
                                    
                                    needToDraw = true;
                                }
                        }
                    else if (isMarked)
                        {
                            pizGrowingArraySetValueAtIndex (x->unselectedNotes, 
                                    (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_IS_MARKED, false);
                                    
                            needToDraw = true;
                        }
                }
        }
        
    return needToDraw;
}

bool tralala_setCursorType (t_tralala *x, t_object *patcherview, t_jmouse_cursortype type)
{
    bool cursorSet = false;
    
    if (type != x->cursorType)
        {
            x->cursorType = type;
            
            jmouse_setcursor (patcherview, (t_object *)x, type);
            
            cursorSet = true;
        }
    
    return cursorSet;
}
                                                        
bool tralala_setCoordinatesWithPoint (t_tralala *x, PIZCoordinates *coordinates, t_pt pt)
{
    double  f = 1.;
    bool    inside = true;
    
    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    coordinates->position   = (long)((x->windowOffsetX + pt.x) / (STEP_PIXELS_SIZE * f));
    coordinates->pitch      = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y) / 
                                (SEMITONE_PIXELS_SIZE * f))), 0);
    
    if ((coordinates->pitch < 0) || (coordinates->pitch > PIZ_MAGIC_PITCH) || 
        (coordinates->position < 0) || (coordinates->position > (PIZ_SEQUENCE_TIMELINE_SIZE - 1)))
        {
            inside = false;
        }
        
    return inside;
}

void tralala_setRectWithZoneValues (t_tralala *x, t_rect *zoneRect, long start, long end, long down, long up)
{
    double  x1, x2, y1, y2;
    double  f = 1.;

    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    if (end < start) {
        long k  = start;
        start   = end;
        end     = k;
        }
        
    if (down > up) {
        long k  = up;
        up      = down;
        down    = k;
        }
    
    x1 = start * STEP_PIXELS_SIZE  * f;
    x2 = end * STEP_PIXELS_SIZE  * f;
    y1 = (PIZ_MAGIC_PITCH - up) * SEMITONE_PIXELS_SIZE * f;
    y2 = ((PIZ_MAGIC_PITCH + 1) - down) * SEMITONE_PIXELS_SIZE * f;
    
    zoneRect->x         = x1;
    zoneRect->y         = y1;
    zoneRect->width     = x2 - x1;
    zoneRect->height    = y2 - y1;
}

void tralala_setRectWithCoordinatesAndDuration (t_tralala *x, t_rect *noteRect, PIZCoordinates *c, long d)
{
    double  x1, x2, y1, y2;
    double  f = 1.;

    switch (x->zoomMode) {
        case ZOOM_MODE_A    : f = 0.5;  break;
        case ZOOM_MODE_B    : f = 1.;   break;
        case ZOOM_MODE_C    : f = 2.;   break;
        }
    
    x1 = (c->position * STEP_PIXELS_SIZE  * f);
    x2 = x1 + (d * STEP_PIXELS_SIZE * f);
    y1 = ((PIZ_MAGIC_PITCH - c->pitch) * SEMITONE_PIXELS_SIZE * f);
    y2 = y1 + (SEMITONE_PIXELS_SIZE * f);
    
    noteRect->x         = x1;
    noteRect->y         = y1;
    noteRect->width     = x2 - x1;
    noteRect->height    = y2 - y1;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_setStringWithLong (char *string, long longToBeFormatted, long formatMode)
{
    if (formatMode == FORMAT_MODE_LONG)
        {
            snprintf (string, STRING_MAXIMUM_SIZE, "%ld", longToBeFormatted);
        }
    else if (formatMode == FORMAT_MODE_NOTENAME)
        {
            long m, n;
            
            m = longToBeFormatted % 12;
            n = (long)(longToBeFormatted / 12.) - 2;
                        
            switch (m) {
                case 0  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "C", n);  break;
                case 1  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "C#", n); break;
                case 2  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "D", n);  break;
                case 3  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "D#", n); break;
                case 4  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "E", n);  break;
                case 5  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "F", n);  break;
                case 6  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "F#", n); break;
                case 7  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "G", n);  break;
                case 8  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "G#", n); break;
                case 9  : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "A", n);  break;
                case 10 : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "A#", n); break;
                case 11 : snprintf (string, STRING_MAXIMUM_SIZE, "%s%ld", "B", n);  break;
                }
        }
    else if (formatMode == FORMAT_MODE_TICKS)
        {
            snprintf (string, STRING_MAXIMUM_SIZE, "%ld", longToBeFormatted * TICKS_FOR_ONE_STEP);
        }
    else if (formatMode == FORMAT_MODE_GRID) {
        switch (longToBeFormatted) {
        case PIZ_WHOLE_NOTE_DOTTED          :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_wholeDotted->s_name); break;
        case PIZ_WHOLE_NOTE                 :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_whole->s_name); break;
        case PIZ_WHOLE_NOTE_TRIPLET         :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_wholeTriplet->s_name); break;
        case PIZ_HALF_NOTE_DOTTED           :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_halfDotted->s_name); break;
        case PIZ_HALF_NOTE                  :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_half->s_name); break;
        case PIZ_HALF_NOTE_TRIPLET          :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_halfTriplet->s_name); break;
        case PIZ_QUARTER_NOTE_DOTTED        :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_quarterDotted->s_name); break;
        case PIZ_QUARTER_NOTE               :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_quarter->s_name); break;
        case PIZ_QUARTER_NOTE_TRIPLET       :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_quarterTriplet->s_name); break;
        case PIZ_EIGHTH_NOTE_DOTTED         :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_eighthDotted->s_name); break;
        case PIZ_EIGHTH_NOTE                    :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_eighth->s_name); break;
        case PIZ_EIGHTH_NOTE_TRIPLET            :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_eighthTriplet->s_name); break;
        case PIZ_SIXTEENTH_NOTE_DOTTED      :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_sixteenthDotted->s_name); break;
        case PIZ_SIXTEENTH_NOTE             :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_sixteenth->s_name); break;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_sixteenthTriplet->s_name); break;
        case PIZ_THIRTY_SECOND_NOTE         :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_thirtySecond->s_name); break;
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_thirtySecondTriplet->s_name); break;
        case PIZ_SNAP_NONE                  :
            snprintf (string, STRING_MAXIMUM_SIZE, "%s", tll_sym_none->s_name); break;
        }
    }
    
    string[STRING_MAXIMUM_SIZE - 1] = 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_unselectAllText (t_tralala *x)
{
    long i;
    
    for (i = 0; i < TEXT_CELL_COUNT; i++) {
            x->textIsSelected[i] = false;
        }
}

bool tralala_hasSelectedText (t_tralala *x, long *selectedText)
{
    long i;
    bool isSelectedText = false;
    
    for (i = 0; i < TEXT_CELL_COUNT; i++) {
        if (x->textIsSelected[i]) {
                isSelectedText = true;
                *selectedText = i;
            }
    }
    
    return isSelectedText;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_willChange (t_tralala *x)
{
    if (USER)
        {
            x->hitTest  = HIT_NOTHING;
            x->textMode = TEXT_MODE_NOTE;
            x->flags    &= ~(FLAG_HAVE_CHANGED 
                            | FLAG_HAVE_MOVED 
                            | FLAG_HAVE_BEEN_DUPLICATED
                            | FLAG_ZONE_IS_SELECTED
                            | FLAG_ORIGIN_IS_SET
                            | FLAG_IS_LASSO);
            
            x->mouseVelocityValue = 0;
            
            tralala_unselectAllText (x);
            pizSequenceInitLasso (x->user);
            tralala_stopAutoscroll (x);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_testAutoscroll (t_tralala *x, t_object *patcherview, t_pt pt)
{
    t_rect  rect;
    bool    stop = true;

    jbox_get_rect_for_view ((t_object *)x, patcherview, &rect); 
    
    if (pt.x < AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_LEFT);
            stop = false;
        }
        
    if ((rect.width - pt.x) < AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_RIGHT);
            stop = false;
        }
        
    if ((rect.height - pt.y) < AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_DOWN);
            stop = false;
        }
        
    if (pt.y < AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_UP);
            stop = false;
        }
        
    if (stop) {
            tralala_stopAutoscroll (x);
        }
}

void tralala_stopAutoscroll (t_tralala *x)
{
    DIRTYLAYER_UNSET (~(DIRTY_LOCATE_LEFT | DIRTY_LOCATE_RIGHT | DIRTY_LOCATE_DOWN | DIRTY_LOCATE_UP));
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
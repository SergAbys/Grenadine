/*
 *  tralala.c
 *
 *  Created : 25/06/11.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 22/01/12.
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
#pragma mark -

t_jsurface              *tll_thirtySecond       [3];
t_jsurface              *tll_sixteenth          [3];
t_jsurface              *tll_sixteenthTriplet   [3];
t_jsurface              *tll_sixteenthDotted    [3];
t_jsurface              *tll_eighth             [3];
t_jsurface              *tll_eighthTriplet      [3];
t_jsurface              *tll_eighthDotted       [3];
t_jsurface              *tll_quarter            [3];
t_jsurface              *tll_quarterTriplet     [3];
t_jsurface              *tll_quarterDotted      [3];
t_jsurface              *tll_half               [3];
t_jsurface              *tll_halfTriplet        [3];
t_jsurface              *tll_halfDotted         [3];
t_jsurface              *tll_whole              [3];

PIZGrowingArray         *tll_clipboard;

tralalaSymbolsTableA    tll_symbolsA;
tralalaSymbolsTableB    tll_symbolsB;

static t_class          *tll_class;

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
CLASS_ATTR_FILTER_CLIP     (c, "channel",          1, PIZ_SEQUENCE_MIDI_CHANNEL);
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

CLASS_ATTR_LONG_ARRAY      (c, "scalecustom",   0, t_tralala, scaleCustom, PIZ_SEQUENCE_SCALE_SIZE);
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
    post ("%ld", calcoffset (t_tralala, paintMutex));
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
                    systhread_mutex_new (&x->paintMutex,        SYSTHREAD_MUTEX_NORMAL);
                    
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
        
    if (x->paintMutex) {
            systhread_mutex_free (x->paintMutex);
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
                            
                            if (x->type != PIZ_TYPE_CUSTOM)
                                {
                                    pizSequenceSetScale (x->live, x->key, x->type);
                                }
                            else
                                {
                                    ARRAY_GET (tempArray);
                                    
                                    if (tempArray)
                                        {
                                            long i;
                                            
                                            for (i = 0; i < PIZ_SEQUENCE_SCALE_SIZE; i++) {
                                                    pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                                }
                                                
                                            pizSequenceSetCustomScaleWithArray (x->live, x->key, tempArray);
                                            
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
                                x->type = PIZ_TYPE_NONE;
                            } else if (temp == tll_sym_custom) {
                                x->type = PIZ_TYPE_CUSTOM;
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
                                    
                                    if (x->type != PIZ_TYPE_CUSTOM) 
                                        {
                                            pizSequenceSetScale (x->live, x->key, x->type);
                                        } 
                                    else 
                                        {
                                            ARRAY_GET (tempArray);
                                            
                                            if (tempArray) 
                                                {
                                                    long i;
                                                    
                                                    for (i = 0; i < PIZ_SEQUENCE_SCALE_SIZE; i++) {
                                                        pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                                    }
                                                    
                                                    pizSequenceSetCustomScaleWithArray (x->live, x->key, 
                                                        tempArray);
                                                    
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
    if ((argc == PIZ_SEQUENCE_SCALE_SIZE) && argv)
        {   
            atom_getlong_array (argc, argv, PIZ_SEQUENCE_SCALE_SIZE, x->scaleCustom);
            
            if (x->type == PIZ_TYPE_CUSTOM)
                {
                    ARRAY_GET (tempArray);
                            
                    if (tempArray)
                        {
                            long i;
                            
                            for (i = 0; i < PIZ_SEQUENCE_SCALE_SIZE; i++) {
                                    pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                                }
                                
                            pizSequenceSetCustomScaleWithArray (x->live, x->key, tempArray);
                            
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
                        
                    pizSequenceSetCustomPatternWithArray (x->live, tempArray);
                    
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
            long mode = PIZ_SEQUENCE_ADD_MODE_PATTERN;
            long chance = object_attr_getlong (x, tll_sym_chance);
            long count = pizGrowingArrayCount (tempArrayA) / PIZ_SEQUENCE_NOTE_SIZE;
            long *ptr = pizGrowingArrayPtr (tempArrayA);

            for (i = (count - 1); i >= 0; i--) {
                if (100 * (rand ( ) / (RAND_MAX + 1.0)) < chance) {
                        pizGrowingArrayClear (tempArrayB);
                        pizGrowingArrayAppendPtr (tempArrayB, PIZ_SEQUENCE_NOTE_SIZE, 
                            ptr + (PIZ_SEQUENCE_NOTE_SIZE * i));
                        pizSequenceAddNotesWithArray (x->live, tempArrayB, mode);
                    }
            }
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
                PIZ_SEQUENCE_ADD_MODE_PATTERN | PIZ_SEQUENCE_ADD_MODE_CLEAR);
            
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
    if (n >= 0 && n <= PIZ_SEQUENCE_MIDI_NOTE)
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
            
            if (n >= 0 && n <= PIZ_SEQUENCE_MIDI_NOTE)
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
                    long mode = PIZ_SEQUENCE_ADD_MODE_SNAP;
                    
                    if (sequence == x->user) {
                        pizGrowingArrayAppend (tempArray, true);
                        pizGrowingArrayAppend (tempArray, false);
                    } else {
                        pizGrowingArrayAppend (tempArray, false);
                        pizGrowingArrayAppend (tempArray, false);
                    }
                    
                    if (sequence == x->live) {
                        mode = PIZ_SEQUENCE_ADD_MODE_PATTERN;
                    } else if (sequence == x->listen) {
                        mode = PIZ_SEQUENCE_ADD_MODE_AMBITUS;
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
                            
                            if ((atom_alloc_array (PIZ_SEQUENCE_SCALE_SIZE, &argc1, &argv1, &alloc1)) 
                                == MAX_ERR_NONE) {
                                atom_setlong_array (argc1, argv1, PIZ_SEQUENCE_SCALE_SIZE, x->scaleCustom);
                                dictionary_appendatoms (d, tll_sym_scaleCustom, argc1, argv1);
                                sysmem_freeptr (argv1);
                            }
                            
                            if ((atom_alloc_array (x->patternSize, &argc2, &argv2, &alloc2)) 
                                == MAX_ERR_NONE) {
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
// -----------------------------------------------------------------------------------------------------------:x
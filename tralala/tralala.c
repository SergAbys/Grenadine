/*
 *  tralala.c
 *
 *  Created : 25/06/11.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 25/02/12.
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

tralalaSymbolsTableA    tll_symbolsA;
tralalaSymbolsTableB    tll_symbolsB;

PIZError                tll_clipboardError = PIZ_GOOD;
PIZGrowingArray         *tll_clipboard = NULL;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class *tll_class = NULL;

int main (void *moduleRef) 
{   
//
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
class_addmethod (c, (method)tralala_list,                   "list",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_slot,                   "slot",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handle,                 "user",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handle,                 "live",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_handle,                 "listen",               A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceClear,          "clear",                A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceKill,           "kill",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceZoulou,         "zoulou",               A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceRomeo,          "romeo",                A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceUniform,        "uniform",              A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceClean,          "clean",                A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceNote,           "note",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceZone,           "zone",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceDump,           "dump",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceRotate,         "rotate",               A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceScramble,       "scramble",             A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceSort,           "sort",                 A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceChange,         "change",               A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceSet,            "set",                  A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceNovember,       "november",             A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceJuliet,         "juliet",               A_GIMME, 0);
class_addmethod (c, (method)tralala_sequenceCycle,          "cycle",                A_GIMME, 0);
class_addmethod (c, (method)tralala_int,                    "int",                  A_LONG, 0);
class_addmethod (c, (method)tralala_mute,                   "mute",                 A_LONG, 0);
class_addmethod (c, (method)tralala_play,                   "bang",                 0);
class_addmethod (c, (method)tralala_play,                   "play",                 0);
class_addmethod (c, (method)tralala_stop,                   "stop",                 0);
class_addmethod (c, (method)tralala_loop,                   "loop",                 0);
class_addmethod (c, (method)tralala_unloop,                 "unloop",               0);
class_addmethod (c, (method)tralala_pause,                  "pause",                0);
class_addmethod (c, (method)tralala_enter,                  "enter",                0);
class_addmethod (c, (method)tralala_copy,                   "copy",                 0);
class_addmethod (c, (method)tralala_forget,                 "forget",               0);

CLASS_ATTR_DEFAULT   (c, "patching_rect",    0, DEFAULT_PATCHING_RECT);
CLASS_ATTR_DEFAULT   (c, "fontname",         0, DEFAULT_FONTNAME);
CLASS_ATTR_DEFAULT   (c, "fontsize",         0, DEFAULT_FONTSIZE);
CLASS_ATTR_DEFAULT   (c, "fontface",         0, DEFAULT_FONTFACE);
CLASS_ATTR_MIN       (c, "patching_size",    0, SIZE_PATCH_MIN);
CLASS_ATTR_INVISIBLE (c, "textcolor", 0);

CLASS_STICKY_ATTR       (c, "category", 0, "Hint");

CLASS_ATTR_LONG            (c, "embedslots",       0, t_tralala, saveSlotsWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedslots",       0, "onoff", "Save Slots With Patcher");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedslots",       0, "1"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedslots",       0, 1);

CLASS_ATTR_LONG            (c, "embedchannel",     0, t_tralala, saveChannelWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedchannel",     0, "onoff", "Save Channel With Patcher");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedchannel",     0, "1"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedchannel",     0, 1);

CLASS_ATTR_LONG            (c, "embedvalues",      0, t_tralala, saveValuesWithPatcher);
CLASS_ATTR_STYLE_LABEL     (c, "embedvalues",      0, "onoff", "Save Values With Patcher");
CLASS_ATTR_DEFAULT_SAVE    (c, "embedvalues",      0, "0"); 
CLASS_ATTR_FILTER_CLIP     (c, "embedvalues",      0, 1);

CLASS_ATTR_ORDER           (c, "embedslots",       0, "1");
CLASS_ATTR_ORDER           (c, "embedvalues",      0, "2");
CLASS_ATTR_ORDER           (c, "embedchannel",     0, "3");

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Midi");

CLASS_ATTR_LONG            (c, "channel",          0, t_tralala, channel);
CLASS_ATTR_DEFAULT         (c, "channel",          0, DEFAULT_CHANNEL);
CLASS_ATTR_LABEL           (c, "channel",          0, "Channel");
CLASS_ATTR_FILTER_CLIP     (c, "channel",          1, PIZ_MAGIC_CHANNEL);
CLASS_ATTR_ACCESSORS       (c, "channel",          NULL, tralala_setChannel);

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Appearance");

CLASS_ATTR_LONG             (c, "viewtext",         0, t_tralala, viewText);
CLASS_ATTR_DEFAULT_SAVE     (c, "viewtext",         0, DEFAULT_VIEW_TEXT);
CLASS_ATTR_LABEL            (c, "viewtext",         0, "View Text");
CLASS_ATTR_STYLE            (c, "viewtext",         0, "onoff");
CLASS_ATTR_FILTER_CLIP      (c, "viewtext",         0, 1);

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Value");

CLASS_ATTR_LONG            (c, "tempo",             0, t_tralala, tempo);
CLASS_ATTR_DEFAULT         (c, "tempo",             0, DEFAULT_TEMPO);
CLASS_ATTR_LABEL           (c, "tempo",             0, "Tempo");
CLASS_ATTR_FILTER_CLIP     (c, "tempo",             TIME_TEMPO_MIN, TIME_TEMPO_MAX);

CLASS_ATTR_LONG            (c, "velocity",          0, t_tralala, velocity);
CLASS_ATTR_DEFAULT         (c, "velocity",          0, "0");
CLASS_ATTR_LABEL           (c, "velocity",          0, "Velocity");
CLASS_ATTR_ACCESSORS       (c, "velocity",          NULL, tralala_setVelocity);

CLASS_ATTR_SYM             (c, "scalekey",          0, t_tralala, scaleKey);
CLASS_ATTR_ENUM            (c, "scalekey",          0, "C C# D D# E F F# G G# A A# B");
CLASS_ATTR_DEFAULT         (c, "scalekey",          0, DEFAULT_SCALE_KEY);
CLASS_ATTR_ACCESSORS       (c, "scalekey",          NULL, tralala_setScaleKey);
CLASS_ATTR_LABEL           (c, "scalekey",          0, "Scale Key");

CLASS_ATTR_SYM             (c, "scaletype",         0, t_tralala, scaleType);
CLASS_ATTR_ENUM            (c, "scaletype",         0, LIST_SCALE_TYPE);
CLASS_ATTR_DEFAULT         (c, "scaletype",         0, DEFAULT_SCALE_TYPE);
CLASS_ATTR_ACCESSORS       (c, "scaletype",         NULL, tralala_setScaleType);
CLASS_ATTR_LABEL           (c, "scaletype",         0, "Scale Type");

CLASS_ATTR_LONG_ARRAY      (c, "scalecustom",       0, t_tralala, scaleCustom, PIZ_MAGIC_SCALE);
CLASS_ATTR_DEFAULT         (c, "scalecustom",       0, DEFAULT_SCALE_CUSTOM);
CLASS_ATTR_ACCESSORS       (c, "scalecustom",       NULL, tralala_setScaleCustom);
CLASS_ATTR_LABEL           (c, "scalecustom",       0, "Scale Custom");

CLASS_ATTR_SYM             (c, "patterncell",       0, t_tralala, patternCell);
CLASS_ATTR_ENUM            (c, "patterncell",       0, LIST_PATTERN_CELL);
CLASS_ATTR_DEFAULT         (c, "patterncell",       0, DEFAULT_PATTERN_CELL);
CLASS_ATTR_ACCESSORS       (c, "patterncell",       NULL, tralala_setPatternCell);
CLASS_ATTR_LABEL           (c, "patterncell",       0, "Pattern Cell");

CLASS_ATTR_LONG_VARSIZE    (c, "patterncustom",     0, t_tralala, patternCustom, patternSize, SIZE_PATTERN_MAX);
CLASS_ATTR_DEFAULT         (c, "patterncustom",     0, DEFAULT_PATTERN_CUSTOM);
CLASS_ATTR_LABEL           (c, "patterncustom",     0, "Pattern Custom");
CLASS_ATTR_ACCESSORS       (c, "patterncustom",     NULL, tralala_setPatternCustom);

CLASS_ATTR_LONG            (c, "chance",            0, t_tralala, chance);
CLASS_ATTR_DEFAULT         (c, "chance",            0, DEFAULT_CHANCE);
CLASS_ATTR_LABEL           (c, "chance",            0, "Chance");
CLASS_ATTR_FILTER_CLIP     (c, "chance",            0, 100);
CLASS_ATTR_ACCESSORS       (c, "chance",            NULL, tralala_setChance);

CLASS_ATTR_ORDER            (c, "tempo",            0, "201");
CLASS_ATTR_ORDER            (c, "chance",           0, "202");
CLASS_ATTR_ORDER            (c, "velocity",         0, "203");
CLASS_ATTR_ORDER            (c, "scalekey",         0, "204");
CLASS_ATTR_ORDER            (c, "scaletype",        0, "205");
CLASS_ATTR_ORDER            (c, "scalecustom",      0, "206");
CLASS_ATTR_ORDER            (c, "patterncell",      0, "207");
CLASS_ATTR_ORDER            (c, "patterncustom",    0, "208");

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Window");

CLASS_ATTR_DOUBLE           (c, "windowoffsetx",    0, t_tralala, offsetX);
CLASS_ATTR_DEFAULT          (c, "windowoffsetx",    0, DEFAULT_WINDOW_OFFSET_X);
CLASS_ATTR_LABEL            (c, "windowoffsetx",    0, "Window X Offset");

CLASS_ATTR_DOUBLE           (c, "windowoffsety",    0, t_tralala, offsetY);
CLASS_ATTR_DEFAULT          (c, "windowoffsety",    0, DEFAULT_WINDOW_OFFSET_Y);
CLASS_ATTR_LABEL            (c, "windowoffsety",    0, "Window Y Offset");

CLASS_ATTR_LONG             (c, "sequencemode",     0, t_tralala, sequenceMode);
CLASS_ATTR_DEFAULT          (c, "sequencemode",     0, DEFAULT_MODE_SEQUENCE);
CLASS_ATTR_ENUMINDEX        (c, "sequencemode",     0, "user live");
CLASS_ATTR_LABEL            (c, "sequencemode",     0, "Sequence Mode");
CLASS_ATTR_ACCESSORS        (c, "sequencemode",     NULL, tralala_setSequenceMode);

CLASS_ATTR_LONG             (c, "zoommode",         0, t_tralala, zoomMode);
CLASS_ATTR_DEFAULT          (c, "zoommode",         0, DEFAULT_MODE_ZOOM);
CLASS_ATTR_LABEL            (c, "zoommode",         0, "Zoom Mode");
CLASS_ATTR_FILTER_CLIP      (c, "zoommode",         0, 2);

CLASS_ATTR_ORDER            (c, "sequencemode",     0, "101");
CLASS_ATTR_ORDER            (c, "zoommode",         0, "102");
CLASS_ATTR_ORDER            (c, "windowoffsetx",    0, "103");
CLASS_ATTR_ORDER            (c, "windowoffsety",    0, "104");

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Color");

CLASS_ATTR_RGBA             (c, "backgroundcolor",      0, t_tralala, backgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "backgroundcolor",      0, DEFAULT_BACKGROUND_COLOR);
CLASS_ATTR_INVISIBLE        (c, "backgroundcolor",      0);

CLASS_ATTR_RGBA             (c, "lassocolor",           0, t_tralala, lassoColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "lassocolor",           0, DEFAULT_LASSO_COLOR);
CLASS_ATTR_INVISIBLE        (c, "lassocolor",           0);

CLASS_ATTR_RGBA             (c, "unfocusedtextcolor",            0, t_tralala, unfocusedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedtextcolor",            0, DEFAULT_UNFOCUSED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedtextcolor",            0, "rgba", "Unfocused Text");
CLASS_ATTR_RGBA             (c, "unfocusedbordercolor",          0, t_tralala, unfocusedBorderColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedbordercolor",          0, DEFAULT_UNFOCUSED_BORDER_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedbordercolor",          0, "rgba", "Unfocused Border");
CLASS_ATTR_RGBA             (c, "focusedtextcolor",              0, t_tralala, focusedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedtextcolor",              0, DEFAULT_FOCUSED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedtextcolor",              0, "rgba", "Focused Text");
CLASS_ATTR_RGBA             (c, "focusedbordercolor",            0, t_tralala, focusedBorderColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedbordercolor",            0, DEFAULT_FOCUSED_BORDER_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedbordercolor",            0, "rgba", "Focused Border");
CLASS_ATTR_RGBA             (c, "selectedtextcolor",             0, t_tralala, selectedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "selectedtextcolor",             0, DEFAULT_SELECTED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "selectedtextcolor",             0, "rgba", "Selected Text");

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

CLASS_ATTR_RGBA             (c, "unfocusedlivenotecolor",        0, t_tralala, unfocusedLiveNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedlivenotecolor",        0, DEFAULT_UNFOCUSED_LIVE_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedlivenotecolor",        0, "rgba", "Unfocused Live Note");
CLASS_ATTR_RGBA             (c, "unfocusedliveplayednotecolor",  0, t_tralala, unfocusedLivePlayedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "unfocusedliveplayednotecolor",  0, DEFAULT_UNFOCUSED_LIVE_PLAYED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "unfocusedliveplayednotecolor",  0, "rgba", "Unfocused Live Played Note");
CLASS_ATTR_RGBA             (c, "focusedlivenotecolor",          0, t_tralala, focusedLiveNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedlivenotecolor",          0, DEFAULT_FOCUSED_LIVE_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedlivenotecolor",          0, "rgba", "Focused Live Note");
CLASS_ATTR_RGBA             (c, "focusedliveplayednotecolor",    0, t_tralala, focusedLivePlayedNoteColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "focusedliveplayednotecolor",    0, DEFAULT_FOCUSED_LIVE_PLAYED_NOTE_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "focusedliveplayednotecolor",    0, "rgba", "Focused Live Played Note");

CLASS_ATTR_RGBA             (c, "popuptextcolor",           0, t_tralala, popupTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuptextcolor",           0, DEFAULT_POPUP_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuptextcolor",           0, "rgba", "Popup Text");
CLASS_ATTR_RGBA             (c, "popubgcolor",              0, t_tralala, popupBackgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popubgcolor",              0, DEFAULT_POPUP_BACKGROUND_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popubgcolor",              0, "rgba", "Popup Background");
CLASS_ATTR_RGBA             (c, "popuphlttextcolor",        0, t_tralala, popupHighlightedTextColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuphlttextcolor",        0, DEFAULT_POPUP_HIGHLIGHTED_TEXT_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuphlttextcolor",        0, "rgba", "Popup Highlighted Text");
CLASS_ATTR_RGBA             (c, "popuhltbgcolor",           0, t_tralala, popupHighlightedBackgroundColor);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popuhltbgcolor",           0, DEFAULT_POPUP_HIGHLIGHTED_BACKGROUND_COLOR);
CLASS_ATTR_STYLE_LABEL      (c, "popuhltbgcolor",           0, "rgba", "Popup Highlighted Background");

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

CLASS_ATTR_ORDER            (c, "focusedtextcolor",                 0, "1");
CLASS_ATTR_ORDER            (c, "focusedbordercolor",               0, "2");
CLASS_ATTR_ORDER            (c, "focuseduserselectednotecolor",     0, "3");
CLASS_ATTR_ORDER            (c, "focusedusermarkednotecolor",       0, "4");
CLASS_ATTR_ORDER            (c, "focusedusersilentnotecolor",       0, "5");
CLASS_ATTR_ORDER            (c, "focusedlivenotecolor",             0, "6");
CLASS_ATTR_ORDER            (c, "focusedliveplayednotecolor",       0, "7");
CLASS_ATTR_ORDER            (c, "focuseduserzonecolor",             0, "8");
CLASS_ATTR_ORDER            (c, "focusedlivezonecolor",             0, "9");
CLASS_ATTR_ORDER            (c, "focusedselectedzonecolor",         0, "10");
CLASS_ATTR_ORDER            (c, "unfocusedtextcolor",               0, "11");
CLASS_ATTR_ORDER            (c, "unfocusedbordercolor",             0, "12");
CLASS_ATTR_ORDER            (c, "unfocusedusernotecolor",           0, "13");
CLASS_ATTR_ORDER            (c, "unfocuseduserselectednotecolor",   0, "14");
CLASS_ATTR_ORDER            (c, "unfocusedlivenotecolor",           0, "15");
CLASS_ATTR_ORDER            (c, "unfocusedliveplayednotecolor",     0, "16");
CLASS_ATTR_ORDER            (c, "unfocusedzonecolor",               0, "17");
CLASS_ATTR_ORDER            (c, "selectedtextcolor",                0, "18");
CLASS_ATTR_ORDER            (c, "popuptextcolor",                   0, "19");
CLASS_ATTR_ORDER            (c, "popubgcolor",                      0, "20");
CLASS_ATTR_ORDER            (c, "popuphlttextcolor",                0, "21");
CLASS_ATTR_ORDER            (c, "popuhltbgcolor",                   0, "22");

CLASS_STICKY_ATTR_CLEAR (c, "category");
CLASS_STICKY_ATTR       (c, "category", 0, "Font");

CLASS_ATTR_SYM              (c, "popupfontname",    0, t_tralala, popupFontName);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontname",    0, DEFAULT_POPUP_FONTNAME);
CLASS_ATTR_STYLE_LABEL      (c, "popupfontname",    0, "font", "Popup Font Name");
CLASS_ATTR_DOUBLE           (c, "popupfontsize",    0, t_tralala, popupFontSize);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontsize",    0, DEFAULT_POPUP_FONTSIZE);
CLASS_ATTR_ENUM             (c, "popupfontsize",    0, LIST_FONTSIZE);
CLASS_ATTR_LABEL            (c, "popupfontsize",    0, "Popup Font Size");
CLASS_ATTR_LONG             (c, "popupfontface",    0, t_tralala, popupFontFace);
CLASS_ATTR_DEFAULTNAME_SAVE (c, "popupfontface",    0, DEFAULT_POPUP_FONTFACE);
CLASS_ATTR_ENUMINDEX        (c, "popupfontface",    0, LIST_FONTSTYLE);
CLASS_ATTR_LABEL            (c, "popupfontface",    0, "Popup Font Style");

CLASS_STICKY_ATTR_CLEAR (c, "category");

tll_thirtySecond        [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16A");
tll_thirtySecond        [1] = jgraphics_image_surface_create_from_resource (moduleRef, "32B");
tll_thirtySecond        [2] = jgraphics_image_surface_create_from_resource (moduleRef, "32C");

tll_sixteenth           [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16A");
tll_sixteenth           [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16B");
tll_sixteenth           [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16C");
tll_sixteenthTriplet    [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16TA");
tll_sixteenthTriplet    [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16TB");
tll_sixteenthTriplet    [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16TC");
tll_sixteenthDotted     [0] = jgraphics_image_surface_create_from_resource (moduleRef, "16DA");
tll_sixteenthDotted     [1] = jgraphics_image_surface_create_from_resource (moduleRef, "16DB");
tll_sixteenthDotted     [2] = jgraphics_image_surface_create_from_resource (moduleRef, "16DC");

tll_eighth              [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8A");
tll_eighth              [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8B");
tll_eighth              [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8C");
tll_eighthTriplet       [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8TA");
tll_eighthTriplet       [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8TB");
tll_eighthTriplet       [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8TC");
tll_eighthDotted        [0] = jgraphics_image_surface_create_from_resource (moduleRef, "8DA");
tll_eighthDotted        [1] = jgraphics_image_surface_create_from_resource (moduleRef, "8DB");
tll_eighthDotted        [2] = jgraphics_image_surface_create_from_resource (moduleRef, "8DC");

tll_quarter             [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4A");
tll_quarter             [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4B");
tll_quarter             [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4C");
tll_quarterTriplet      [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4TA");
tll_quarterTriplet      [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4TB");
tll_quarterTriplet      [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4TC");
tll_quarterDotted       [0] = jgraphics_image_surface_create_from_resource (moduleRef, "4DA");
tll_quarterDotted       [1] = jgraphics_image_surface_create_from_resource (moduleRef, "4DB");
tll_quarterDotted       [2] = jgraphics_image_surface_create_from_resource (moduleRef, "4DC");

tll_half                [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2A");
tll_half                [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2B");
tll_half                [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2C");
tll_halfTriplet         [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2TA");
tll_halfTriplet         [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2TB");
tll_halfTriplet         [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2TC");
tll_halfDotted          [0] = jgraphics_image_surface_create_from_resource (moduleRef, "2DA");
tll_halfDotted          [1] = jgraphics_image_surface_create_from_resource (moduleRef, "2DB");
tll_halfDotted          [2] = jgraphics_image_surface_create_from_resource (moduleRef, "2DC");

tll_whole               [0] = jgraphics_image_surface_create_from_resource (moduleRef, "1A");
tll_whole               [1] = jgraphics_image_surface_create_from_resource (moduleRef, "1B");
tll_whole               [2] = jgraphics_image_surface_create_from_resource (moduleRef, "1C");

tll_clipboard = pizGrowingArrayNew (0);

class_register (CLASS_BOX, c);

tll_class = c;

return MAX_ERR_NONE;
//
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new (t_symbol *s, long argc, t_atom *argv)
{
    t_tralala       *x = NULL;
    t_dictionary    *d = NULL;

    if (d = object_dictionaryarg (argc, argv)) {
        if (x = (t_tralala *)object_alloc (tll_class)) {
            long i, k, boxflags;
            long initArrayPool[2] = {2, SIZE_GROWING_ARRAY};
            
            boxflags = 0 
                | JBOX_DRAWFIRSTIN 
                | JBOX_DRAWINLAST
                | JBOX_GROWBOTH
                | JBOX_DRAWBACKGROUND
                | JBOX_HILITE 
                ;

            jbox_new ((t_jbox *)x, boxflags, argc, argv);
            x->box.b_firstin = (void *)x;
            
            x->rightOutlet       = outlet_new (x, NULL);
            x->middleRightOutlet = bangout ((t_object *)x);
            x->middleLeftOutlet  = listout ((t_object *)x);
            x->leftOutlet        = listout ((t_object *)x);
            
            object_obex_store ((void *)x, tll_sym_dumpout, (t_object *)x->rightOutlet);
            
            x->paintClock        = clock_new (x, (method)tralala_paintTask);
            x->learnClock        = clock_new (x, (method)tralala_learnTask);
            x->focusClock        = clock_new (x, (method)tralala_focusTask);
            x->notifyClock       = clock_new (x, (method)tralala_notifyTask);
            x->runClock          = clock_new (x, (method)tralala_runTask);
            x->goToStartClock    = clock_new (x, (method)tralala_goToStartTask);
            x->inhibitStartClock = clock_new (x, (method)tralala_inhibitStartTask);
            x->inhibitBangClock  = clock_new (x, (method)tralala_inhibitBangTask);
            
            systhread_mutex_new (&x->methodsMutex,      SYSTHREAD_MUTEX_NORMAL);
            systhread_mutex_new (&x->algorithmsMutex,   SYSTHREAD_MUTEX_NORMAL);
            systhread_mutex_new (&x->learnMutex,        SYSTHREAD_MUTEX_NORMAL);
            systhread_mutex_new (&x->arraysMutex,       SYSTHREAD_MUTEX_NORMAL);
            
            for (i = 0; i < TEXT_CELL_COUNT; i++) {
                x->textLayers[i]     = jtextlayout_create ( );
                x->textIsSelected[i] = false;
            }
            
            jbox_ready ((t_jbox *)x);

            x->user                 = pizSequenceNew        ( );
            x->live                 = pizSequenceNew        ( );
            x->listen               = pizSequenceNew        ( );
            x->arrayPool            = pizArrayPoolNew       (2, initArrayPool);
            x->factorOracle         = pizFactorOracleNew    (0, NULL);
            x->galoisLattice        = pizGaloisLatticeNew   (0, NULL);
            x->finiteState          = pizFiniteStateNew     (0, NULL);
            x->unselected           = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->selected             = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->played               = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->zone                 = pizGrowingArrayNew    (PIZ_DATA_ZONE_SIZE);
            x->unselectedCopy       = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->selectedCopy         = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->playedCopy           = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->zoneCopy             = pizGrowingArrayNew    (PIZ_DATA_ZONE_SIZE);
            x->origin               = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->result               = pizGrowingArrayNew    (SIZE_GROWING_ARRAY);
            x->valuesToBeLearned    = pizGrowingArrayNew    (SIZE_LEARN_ARRAY);
            x->learnQueue           = pizBoundedQueueNew    (SIZE_LEARN_QUEUE);
            x->slots                = pizLinklistNew        ( );
    
            if (x->user && 
                x->live && 
                x->listen && 
                x->arrayPool &&
                x->factorOracle &&
                x->galoisLattice &&
                x->finiteState &&
                x->unselected && 
                x->selected && 
                x->played &&
                x->zone && 
                x->unselectedCopy && 
                x->selectedCopy && 
                x->playedCopy &&
                x->zoneCopy && 
                x->origin &&
                x->result &&
                x->valuesToBeLearned &&
                x->learnQueue &&
                x->slots) {
                
                t_dictionary    *dictionary = NULL;
                PIZGrowingArray *firstSlot = NULL;
                
                attr_dictionary_process (x, d);

                x->flags            = FLAG_INIT_PAINT_CLOCK;
                x->textMode         = MODE_TEXT_NOTE;
                x->hitTest          = HIT_NOTHING;
                x->cursorType       = JMOUSE_CURSOR_ARROW;
                x->learnCycle       = PIZ_ALGORITHM_NONE;
                x->learnThreshold   = SIZE_LEARN_MIN;
                x->cell             = PIZ_NOTE_NONE;
                x->noteValue        = PIZ_NOTE_NONE;
                x->dirtyLayer       = (DIRTY_ZONE | DIRTY_NOTES | DIRTY_GRID | DIRTY_SEQUENCE);
                
                pizSequenceSetGrid  (x->user, x->grid = PIZ_EIGHTH_NOTE);
                pizSequenceSetCell  (x->user, PIZ_EIGHTH_NOTE);
                
                pizLinklistSetFlags (x->slots, PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY);
                
                k = (x->saveSlotsWithPatcher || x->saveChannelWithPatcher || x->saveValuesWithPatcher);
                
                if (k && dictionary_hasentry (d, tll_sym_tralala)) {
                    dictionary_getdictionary (d, tll_sym_tralala, (t_object **)&dictionary);
                    
                    if (dictionary) {
                        tralala_dataWithDictionary (x, dictionary);
                    }
                }
                
                if (!pizLinklistCount (x->slots)) {
                    if (firstSlot = pizGrowingArrayNew (SIZE_GROWING_ARRAY)) {
                        pizLinklistAppend (x->slots, firstSlot);
                    }
                }
                
                tralala_slotRecall (x, x->slotIndex);
                
                srand ((unsigned int)time(NULL));
                
                clock_fdelay (x->learnClock, CLOCK_LEARN_INTERVAL);
                
            } else {
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
    
    if (x->learnMutex) {
        systhread_mutex_free (x->learnMutex);
    }
    if (x->arraysMutex) {
        systhread_mutex_free (x->arraysMutex);
    }
    if (x->methodsMutex) {
        systhread_mutex_free (x->methodsMutex);
    }
    if (x->algorithmsMutex) {
        systhread_mutex_free (x->algorithmsMutex);
    }
    
    for (i = 0; i < TEXT_CELL_COUNT; i++) {
        if (x->textLayers[i]) {
            jtextlayout_destroy (x->textLayers[i]);
        }
    }
    
    pizGrowingArrayFree  (x->zone);
    pizGrowingArrayFree  (x->played);
    pizGrowingArrayFree  (x->selected);
    pizGrowingArrayFree  (x->unselected);
    pizGrowingArrayFree  (x->zoneCopy);
    pizGrowingArrayFree  (x->playedCopy);
    pizGrowingArrayFree  (x->selectedCopy);
    pizGrowingArrayFree  (x->unselectedCopy);
    pizGrowingArrayFree  (x->result);
    pizGrowingArrayFree  (x->origin);
    pizGrowingArrayFree  (x->valuesToBeLearned);
    
    pizFactorOracleFree  (x->factorOracle);
    pizGaloisLatticeFree (x->galoisLattice);
    pizFiniteStateFree   (x->finiteState);
    
    pizLinklistFree      (x->slots);    
    pizBoundedQueueFree  (x->learnQueue);
    pizArrayPoolFree     (x->arrayPool);
    
    pizSequenceFree      (x->user);
    pizSequenceFree      (x->listen);
    pizSequenceFree      (x->live);
        
    jbox_free ((t_jbox *)x);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_assist (t_tralala *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET)  { 
        sprintf (s, "Messages");
    } else {   
        switch (a) {
            case 0  : sprintf (s, "(list) Played Notes");   break;    
            case 1  : sprintf (s, "(list) Dumped Notes");   break;
            case 2  : sprintf (s, "(bang) End Of Loop");    break;
            case 3  : sprintf (s, "(anything) Dumpout");    break;
        }
    }
}

void tralala_jsave (t_tralala *x, t_dictionary *d)
{
    t_dictionary *dictionary = NULL;
    long k = (x->saveSlotsWithPatcher || x->saveChannelWithPatcher || x->saveValuesWithPatcher);
    
    if (k && d && (dictionary = dictionary_new ())) {
        tralala_slotStore (x);
        tralala_dataToDictionary (x, dictionary);
        dictionary_appenddictionary (d, tll_sym_tralala, (t_object *)dictionary); 
    }
}

t_max_err tralala_setvalueof (t_tralala *x, long argc, t_atom *argv)
{
    if (argc && argv) {
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            long     i;
            PIZError err = PIZ_GOOD;
            
            for (i = 0; i < argc; i++) {    
                err |= pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
            }
            
            if (!err) {
                pizSequenceDecodeWithArray (x->user, tempArray);
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_SEQUENCE);
                DIRTYSLOTS
            }
        }
        
        ARRAY_RELEASE (tempArray);
    }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_getvalueof (t_tralala *x, long *argc, t_atom **argv)
{
    if (argc && argv) {
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            if (!pizSequenceEncodeToArray (x->user, tempArray)) {
                long size = pizGrowingArrayCount (tempArray);
                
                if (*argc && *argv) {
                    ;
                } else {
                    *argc = size;
                    *argv = (t_atom *)getbytes(sizeof(t_atom) * size);
                }
        
                if (*argv && (*argc >= size)) {
                    long *ptr = pizGrowingArrayPtr (tempArray);
                    atom_setlong_array (*argc, *argv, size, ptr);
                }
            }
        }
        
        ARRAY_RELEASE (tempArray);
    }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err jbox_notify (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (msg == tll_sym_attr_modified) {  
        t_tralala *myObject = NULL;
        t_symbol *attr_name = NULL;
        
        attr_name = (t_symbol *)object_method (data, tll_sym_getname);
                    
        if (attr_name) {                   
            if ((attr_name != tll_sym_patching_rect) && object_attr_usercanset (x, attr_name)) {
                if (myObject = (t_tralala *)(jbox_get_object ((t_object *)x))) {
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
    if (d) {
    //
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
    //
    dictionary_appendlong   (d, tll_sym_sequenceMode, x->sequenceMode);
    dictionary_appendlong   (d, tll_sym_zoomMode, x->zoomMode);
    dictionary_appendfloat  (d, tll_sym_windowOffsetX, x->offsetX);
    dictionary_appendfloat  (d, tll_sym_windowOffsetY, x->offsetY); 
    
    dictionary_appendlong   (d, tll_sym_version, PIZ_SEQUENCE_VERSION_MAJOR);
    
    if (x->saveChannelWithPatcher) {
        dictionary_appendlong (d, tll_sym_channel, x->channel);
    }
    
    if (x->saveSlotsWithPatcher) {
        long            index = 0;
        PIZGrowingArray *slot = NULL;
        PIZGrowingArray *nextSlot = NULL;
        
        pizLinklistPtrAtIndex (x->slots, 0, (void **)&slot);
        
        while (slot) {
            long    count;
            char    key[SIZE_STRING_MAX];
            t_atom  *storage = NULL;
            long    *ptr = NULL;
            
            pizLinklistNextByPtr (x->slots, (void *)slot, (void **)&nextSlot);
            count = pizGrowingArrayCount (slot);
            
            if (storage = (t_atom *)sysmem_newptrclear (sizeof(t_atom) * count)) {
                ptr = pizGrowingArrayPtr (slot);
                atom_setlong_array (count, storage, count, ptr);
                snprintf (key, SIZE_STRING_MAX, "slot %ld", index);
                key[SIZE_STRING_MAX - 1] = 0;
                dictionary_appendatoms (d, gensym (key), count, storage);
                sysmem_freeptr (storage);
            }
            
            slot = nextSlot;
            index ++;
        }
        
        dictionary_appendlong (d, tll_sym_slots, index);
    }   
        
    if (x->saveValuesWithPatcher) {
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
    //    
    } ATOMIC_DECREMENT (&x->popupLock);  
    //    
    } 
}

void tralala_dataWithDictionary (t_tralala *x, t_dictionary *d)
{
    if (d) {
    //
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
    //
    long tempA = 0;
    long tempB = 0;
    
    dictionary_getlong  (d, tll_sym_sequenceMode, &tempA);
    dictionary_getlong  (d, tll_sym_zoomMode, &x->zoomMode);
    dictionary_getfloat (d, tll_sym_windowOffsetX, &x->offsetX);
    dictionary_getfloat (d, tll_sym_windowOffsetY, &x->offsetY);
    
    if (x->saveChannelWithPatcher) {
        dictionary_getlong (d, tll_sym_channel, &tempB);
    }
        
    if (x->saveSlotsWithPatcher) {
        long            i, j;
        long            count = 0;
        long            argc = 0;
        t_atom          *argv = NULL;
        char            key[SIZE_STRING_MAX];
        PIZGrowingArray *slot = NULL;
        
        dictionary_getlong (d, tll_sym_slots, &count);
        
        for (i = 0; i < count; i++) {
            snprintf (key, SIZE_STRING_MAX, "slot %ld", i);
            key[SIZE_STRING_MAX - 1] = 0;
            dictionary_getatoms (d, gensym (key), &argc, &argv);
            
            if (argv) {
                if (slot = pizGrowingArrayNew (SIZE_GROWING_ARRAY)) {
                    for (j = 0; j < argc; j++) {
                        pizGrowingArrayAppend (slot, atom_getlong (argv + j));
                    }
                    pizLinklistAppend (x->slots, slot);
                }
            }
        }
    }
    
    if (x->saveValuesWithPatcher) {
        long     argc1, argc2, chance, velocity;
        t_atom   *argv1, *argv2;
        t_symbol *scaleKey, *scaleType, *patternCell;
        
        argc1 = argc2 = chance = velocity = 0;
        argv1 = argv2 = NULL;
        scaleKey = scaleType = patternCell = NULL;
        
        dictionary_getlong      (d, tll_sym_tempo, &x->tempo);
        dictionary_getlong      (d, tll_sym_chance, &chance);
        dictionary_getlong      (d, tll_sym_velocity, &velocity);
        dictionary_getsym       (d, tll_sym_scaleKey, &scaleKey);
        dictionary_getsym       (d, tll_sym_scaleType, &scaleType);
        dictionary_getsym       (d, tll_sym_patternCell, &patternCell);
        dictionary_copyatoms    (d, tll_sym_scaleCustom, &argc1, &argv1);
        dictionary_copyatoms    (d, tll_sym_patternCustom, &argc2, &argv2);
        
        object_attr_setlong     (x, tll_sym_chance, chance);
        object_attr_setlong     (x, tll_sym_velocity, velocity);
        object_attr_setsym      (x, tll_sym_scaleKey, scaleKey);
        object_attr_setsym      (x, tll_sym_scaleType, scaleType);
        object_attr_setsym      (x, tll_sym_patternCell, patternCell);
        object_attr_setvalueof  (x, tll_sym_scaleCustom, argc1, argv1);
        object_attr_setvalueof  (x, tll_sym_patternCustom, argc2, argv2);

        if (argv1) {
            sysmem_freeptr (argv1);
        }
        if (argv2) {
            sysmem_freeptr (argv2);
        }
    }
    
    ATOMIC_DECREMENT (&x->popupLock);
    
    object_attr_setlong (x, tll_sym_channel, tempB);
    object_attr_setlong (x, tll_sym_sequenceMode, tempA);

    //        
    } else {
        ATOMIC_DECREMENT (&x->popupLock);
    }
    //    
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err tralala_setSequenceMode (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->sequenceMode = CLAMP (atom_getlong (argv), MODE_SEQUENCE_USER, MODE_SEQUENCE_LIVE);
        
        if (USER) {
            tralala_willChange (x);
        }
        
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_SEQUENCE | DIRTY_GRID);
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setChannel (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
    
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            x->channel = atom_getlong (argv);
            
            pizSequenceSetChannel (x->user,   x->channel);
            pizSequenceSetChannel (x->live,   x->channel);
            pizSequenceSetChannel (x->listen, x->channel);
            
            DIRTYCHANNEL
        } 
        
        ATOMIC_DECREMENT (&x->popupLock);
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setVelocity (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        pizSequenceSetVelocity (x->live, atom_getlong (argv));
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setChance (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->chance = atom_getlong (argv);
        
        pizSequenceSetChance (x->user,   x->chance);
        pizSequenceSetChance (x->live,   x->chance);
        pizSequenceSetChance (x->listen, x->chance);
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleKey (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        long     i;
        bool     err = false;
        t_symbol *s = atom_getsym (argv);
        
        if (s != x->scaleKey) {
        //
        if (s == tll_sym_c)             { x->key = PIZ_KEY_C; } 
        else if (s == tll_sym_cSharp)   { x->key = PIZ_KEY_C_SHARP; } 
        else if (s == tll_sym_d)        { x->key = PIZ_KEY_D; } 
        else if (s == tll_sym_dSharp)   { x->key = PIZ_KEY_D_SHARP; } 
        else if (s == tll_sym_e)        { x->key = PIZ_KEY_E; } 
        else if (s == tll_sym_f)        { x->key = PIZ_KEY_F; } 
        else if (s == tll_sym_fSharp)   { x->key = PIZ_KEY_F_SHARP; }
        else if (s == tll_sym_g)        { x->key = PIZ_KEY_G; } 
        else if (s == tll_sym_gSharp)   { x->key = PIZ_KEY_G_SHARP; } 
        else if (s == tll_sym_a)        { x->key = PIZ_KEY_A; } 
        else if (s == tll_sym_aSharp)   { x->key = PIZ_KEY_A_SHARP; } 
        else if (s == tll_sym_b)        { x->key = PIZ_KEY_B; } 
        else { err = true; }

        if (!err) {   
            x->scaleKey = s;
            
            if (x->type == PIZ_SCALE_CUSTOM) {
            
                ARRAY_GET (tempArray);
                
                if (tempArray) {
                    for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                        pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                    }
                    pizSequenceSetScale (x->live, x->key, x->type, tempArray);                        
                }
                
                ARRAY_RELEASE (tempArray);
                
            } else {
                pizSequenceSetScale (x->live, x->key, x->type, NULL);
            }
            
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
        // 
        }
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleType (t_tralala *x, t_object *attr, long argc, t_atom *argv) 
{
    if (argc && argv) {
        long        size = 0;
        char        *tempString = NULL;
        t_symbol    *s = NULL;
        bool        err = false;            

        atom_gettext (argc, argv, &size, &tempString, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (tempString) {
            s = gensym (tempString);
            
            if (s != x->scaleType) {
            //
            if (s == tll_sym_none)                          { x->type = PIZ_SCALE_NONE; } 
            else if (s == tll_sym_custom)                   { x->type = PIZ_SCALE_CUSTOM; } 
            else if (s == tll_sym_ionian)                   { x->type = PIZ_IONIAN; } 
            else if (s == tll_sym_dorian)                   { x->type = PIZ_DORIAN; } 
            else if (s == tll_sym_phrygian)                 { x->type = PIZ_PHRYGIAN; } 
            else if (s == tll_sym_lydian)                   { x->type = PIZ_LYDIAN; } 
            else if (s == tll_sym_mixolydian)               { x->type = PIZ_MIXOLYDIAN; } 
            else if (s == tll_sym_aeolian)                  { x->type = PIZ_AEOLIAN; } 
            else if (s == tll_sym_locrian)                  { x->type = PIZ_LOCRIAN; }
            else if (s == tll_sym_natural)                  { x->type = PIZ_NATURAL; } 
            else if (s == tll_sym_harmonic)                 { x->type = PIZ_HARMONIC; } 
            else if (s == tll_sym_melodic)                  { x->type = PIZ_MELODIC; } 
            else if (s == tll_sym_wholeTone)                { x->type = PIZ_WHOLE_TONE; } 
            else if (s == tll_sym_pentatonicMajor)          { x->type = PIZ_PENTATONIC_MAJOR; } 
            else if (s == tll_sym_pentatonicMinor)          { x->type = PIZ_PENTATONIC_MINOR; } 
            else if (s == tll_sym_octatonicHalfWhole)       { x->type = PIZ_OCTATONIC_HALF_WHOLE; } 
            else if (s == tll_sym_octatonicWholeHalf)       { x->type = PIZ_OCTATONIC_WHOLE_HALF; } 
            else if (s == tll_sym_seventhMajor)             { x->type = PIZ_SEVENTH_MAJOR; } 
            else if (s == tll_sym_seventhDominant)          { x->type = PIZ_SEVENTH_DOMINANT; } 
            else if (s == tll_sym_seventhMinor)             { x->type = PIZ_SEVENTH_MINOR; } 
            else if (s == tll_sym_seventhMajorSixth)        { x->type = PIZ_SEVENTH_MAJOR_SIXTH; } 
            else if (s == tll_sym_seventhMinorSixth)        { x->type = PIZ_SEVENTH_MINOR_SIXTH; }
            else if (s == tll_sym_seventhHalfDiminished)    { x->type = PIZ_SEVENTH_HALF_DIMINISHED; } 
            else if (s == tll_sym_seventhDiminished)        { x->type = PIZ_SEVENTH_DIMINISHED; } 
            else if (s == tll_sym_seventhSuspended)         { x->type = PIZ_SEVENTH_SUSPENDED; } 
            else if (s == tll_sym_seventhSharpFive)         { x->type = PIZ_SEVENTH_SHARP_FIVE; } 
            else if (s == tll_sym_seventhFlatFive)          { x->type = PIZ_SEVENTH_FLAT_FIVE; } 
            else { err = true; }
            
            if (!err) {   
                x->scaleType = s;
                
                if (x->type != PIZ_SCALE_CUSTOM)  {
                    pizSequenceSetScale (x->live, x->key, x->type, NULL);
                } else {
                    ARRAY_GET (tempArray);
                    
                    if (tempArray) {
                        long i;
                        
                        for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                            pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                        }
                        
                        pizSequenceSetScale (x->live, x->key, x->type, tempArray);
                    }
                    
                    ARRAY_RELEASE (tempArray);
                }
                
                if (LIVE) {
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
                }
            }
            //
            }
            
            sysmem_freeptr (tempString);
        }
    }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_setScaleCustom (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if ((argc == PIZ_MAGIC_SCALE) && argv) {   
        atom_getlong_array (argc, argv, PIZ_MAGIC_SCALE, x->scaleCustom);
        
        if (x->type == PIZ_SCALE_CUSTOM) {
            ARRAY_GET (tempArray);
                    
            if (tempArray) {
                long i;
                
                for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
                    pizGrowingArrayAppend (tempArray, x->scaleCustom[i]);
                }
                    
                pizSequenceSetScale (x->live, x->key, x->type, tempArray);
            }
            ARRAY_RELEASE (tempArray);
            
            if (LIVE) {
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            }
        }
    }

    return MAX_ERR_NONE;
}

t_max_err tralala_setPatternCell (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        long        size = 0;
        char        *tempString = NULL;
        t_symbol    *s = NULL;
        PIZError    err = PIZ_GOOD;            

        atom_gettext (argc, argv, &size, &tempString, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (tempString) {
            s = gensym (tempString);
    
            if (s != x->patternCell) {
            //
            if (s == tll_sym_none)                      { x->cell = PIZ_NOTE_NONE; } 
            else if (s == tll_sym_whole)                { x->cell = PIZ_WHOLE_NOTE; } 
            else if (s == tll_sym_half)                 { x->cell = PIZ_HALF_NOTE; } 
            else if (s == tll_sym_quarter)              { x->cell = PIZ_QUARTER_NOTE; } 
            else if (s == tll_sym_eighth)               { x->cell = PIZ_EIGHTH_NOTE; } 
            else if (s == tll_sym_sixteenth)            { x->cell = PIZ_SIXTEENTH_NOTE; } 
            else if (s == tll_sym_thirtySecond)         { x->cell = PIZ_THIRTY_SECOND_NOTE; } 
            else if (s == tll_sym_wholeTriplet)         { x->cell = PIZ_WHOLE_NOTE_TRIPLET; } 
            else if (s == tll_sym_halfTriplet)          { x->cell = PIZ_HALF_NOTE_TRIPLET; } 
            else if (s == tll_sym_quarterTriplet)       { x->cell = PIZ_QUARTER_NOTE_TRIPLET; }
            else if (s == tll_sym_eighthTriplet)        { x->cell = PIZ_EIGHTH_NOTE_TRIPLET; }
            else if (s == tll_sym_sixteenthTriplet)     { x->cell = PIZ_SIXTEENTH_NOTE_TRIPLET; }
            else if (s == tll_sym_thirtySecondTriplet)  { x->cell = PIZ_THIRTY_SECOND_NOTE_TRIPLET; } 
            else if (s == tll_sym_wholeDotted)          { x->cell = PIZ_WHOLE_NOTE_DOTTED; } 
            else if (s == tll_sym_halfDotted)           { x->cell = PIZ_HALF_NOTE_DOTTED; } 
            else if (s == tll_sym_quarterDotted)        { x->cell = PIZ_QUARTER_NOTE_DOTTED; } 
            else if (s == tll_sym_eighthDotted)         { x->cell = PIZ_EIGHTH_NOTE_DOTTED; } 
            else if (s == tll_sym_sixteenthDotted)      { x->cell = PIZ_SIXTEENTH_NOTE_DOTTED; } 
            else { err = PIZ_ERROR; }
                
            if (!err) {
                pizSequenceSetGrid (x->live,    x->cell);
                pizSequenceSetCell (x->live,    x->cell);
                pizSequenceSetGrid (x->listen,  x->cell);
                pizSequenceSetCell (x->listen,  x->cell);
                
                x->patternCell = s;
                
                DIRTYLAYER_SET (DIRTY_GRID);
            }
            //    
            }
            
            sysmem_freeptr (tempString);
        }
    }
    
    return MAX_ERR_NONE;
}

t_max_err tralala_setPatternCustom (t_tralala *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {  
        long i;
        
        ARRAY_GET (tempArray);

        x->patternSize = MIN (argc, SIZE_PATTERN_MAX);
        atom_getlong_array (argc, argv, x->patternSize, x->patternCustom);
                        
        if (tempArray) {
            for (i = 0; i < x->patternSize; i++) {
                pizGrowingArrayAppend (tempArray, x->patternCustom[i]);
            }
                
            pizSequenceSetPattern (x->live, tempArray);
        }
        
        ARRAY_RELEASE (tempArray);
    }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_play (t_tralala *x)
{   
    METHODSLOCK
    
    x->flags &= ~FLAG_INHIBIT_START;
    clock_unset (x->goToStartClock);
    
    if (pizSequenceIsAtEnd (x->live) && !(x->flags & FLAG_IS_LOOPED)) {
        x->flags &= ~FLAG_IS_RUNNING;
        pizSequenceGoToStart (x->live);
        
        if (!(x->flags & FLAG_INHIBIT_BANG)) {
            outlet_bang (x->middleRightOutlet);
        }
    }
        
    if (!(x->flags & FLAG_IS_RUNNING) && !(x->flags & FLAG_INHIBIT_START)) {
        pizSequenceGoToStart (x->live);
        x->runIndex = pizSequenceIndex (x->live);
        
        x->flags |= FLAG_IS_RUNNING;
        x->flags &= ~(FLAG_IS_LOOPED | FLAG_IS_PAUSED);

        clock_fdelay (x->runClock, 0.);
    }
    
    METHODSUNLOCK
}

void tralala_stop (t_tralala *x)
{   
    METHODSLOCK
    
    clock_unset (x->runClock);
    x->flags &= ~(FLAG_IS_LOOPED | FLAG_IS_RUNNING | FLAG_IS_PAUSED);
    x->runIndex = -1;
    
    if (pizSequenceIsAtEnd (x->live)) {
        clock_fdelay (x->goToStartClock, CLOCK_DEFER_INTERVAL);
    }
    
    x->flags |= FLAG_INHIBIT_START;
    clock_fdelay (x->inhibitStartClock, CLOCK_DEFER_INTERVAL);
    
    METHODSUNLOCK
}

void tralala_loop (t_tralala *x)
{   
    METHODSLOCK
    
    if (!(x->flags & FLAG_IS_RUNNING)) {
        clock_unset (x->goToStartClock);
            
        pizSequenceGoToStart (x->live);
        x->runIndex = pizSequenceIndex (x->live);
        
        x->flags |= (FLAG_IS_LOOPED | FLAG_IS_RUNNING);
        x->flags &= ~FLAG_IS_PAUSED;

        clock_fdelay (x->runClock, 0.);
        
    } else {
        x->flags |= FLAG_IS_LOOPED;
    }
    
    METHODSUNLOCK
}

void tralala_unloop (t_tralala *x)
{   
    METHODSLOCK
    
    x->flags &= ~FLAG_IS_LOOPED;
    
    METHODSUNLOCK
}

void tralala_pause (t_tralala *x)
{   
    METHODSLOCK
    
    if ((x->flags & FLAG_IS_RUNNING) && !(x->flags & FLAG_IS_PAUSED)) {
        x->flags |= FLAG_IS_PAUSED;
        x->flags &= ~FLAG_IS_RUNNING;
        
        clock_unset (x->runClock);
        
    } else {
        x->flags &= ~FLAG_IS_PAUSED;
        x->flags |= FLAG_IS_RUNNING;
            
        clock_fdelay (x->runClock, 0.);
    }
    
    METHODSUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_runTask (t_tralala *x)
{
    PIZError err;
    long     temp;

    if ((pizSequenceIsAtEnd (x->live)) && (x->flags & FLAG_IS_LOOPED)) {
        outlet_bang (x->middleRightOutlet);
        pizSequenceGoToStart (x->live);
    }
        
    temp = pizSequenceIndex (x->live);
    err  = pizSequenceProceedStep (x->live, x->result);
    
    if (!err) {
        double  k;
        long    i, count;
        long    pitch, velocity, duration, channel;
        
        if (!(x->flags & FLAG_IS_MUTED) && (count = pizGrowingArrayCount (x->result))) {
        
            for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
                pitch    = pizGrowingArrayValueAtIndex (x->result, i + PIZ_DATA_PITCH);
                velocity = pizGrowingArrayValueAtIndex (x->result, i + PIZ_DATA_VELOCITY);
                channel  = pizGrowingArrayValueAtIndex (x->result, i + PIZ_DATA_CHANNEL);
                
                k = (TIME_STEPS_PER_MINUTE / (double)x->tempo);
                duration = (long)((pizGrowingArrayValueAtIndex (x->result, i + PIZ_DATA_DURATION)) * k);
                    
                atom_setlong (x->playedNote + 0, pitch);
                atom_setlong (x->playedNote + 1, velocity);
                atom_setlong (x->playedNote + 2, duration);
                atom_setlong (x->playedNote + 3, channel);
                
                outlet_list  (x->leftOutlet, NULL, 4, x->playedNote);
            }
        }
        
        if (x->flags & FLAG_IS_RUNNING) {
            clock_fdelay (x->runClock, TIME_STEPS_PER_MINUTE / (double)x->tempo);
        }
        
    } else {
        temp = -1;
        
        x->flags &= ~FLAG_IS_RUNNING;
        x->flags |= FLAG_INHIBIT_BANG;
        
        clock_fdelay (x->goToStartClock, CLOCK_DEFER_INTERVAL);
        clock_fdelay (x->inhibitBangClock, CLOCK_DEFER_INTERVAL);
        
        outlet_bang (x->middleRightOutlet);
    }
    
    pizGrowingArrayClear (x->result);

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
// -----------------------------------------------------------------------------------------------------------:x
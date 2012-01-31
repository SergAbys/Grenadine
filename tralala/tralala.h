/*
 *  tralala.h
 *
 *  Created : 04/07/11.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 31/01/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef PIZ_TRALALA_H
#define PIZ_TRALALA_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "ext_dictionary.h"
#include "ext_systhread.h"
#include "ext_atomic.h"
#include "commonsyms.h"

#include "pizSequence.h"
#include "pizInterface.h"
#include "pizTransform.h"
#include "pizArchive.h"
#include "tralalaSymbols.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define STEPS_PER_MINUTE                    2500.
#define TICKS_FOR_ONE_STEP                  20
#define STEP_PIXELS_SIZE                    2.
#define SEMITONE_PIXELS_SIZE                24.
#define JSURFACE_MOSAIC_PITCH_SIZE          16
#define JSURFACE_MOSAIC_STEP_SIZE           288

#define DEFER_CLOCK_INTERVAL                0.
#define PAINT_CLOCK_INTERVAL                31.
#define LEARN_CLOCK_INTERVAL                37.
#define FOCUS_CLOCK_INTERVAL                53.
#define NOTIFY_CLOCK_INTERVAL               487.

#define CLOCK_RANDOMIZE                     10.

#define MOUSEWHEEL_FACTOR                   100.
#define AUTOSCROLL_RANGE                    20.
#define AUTOSCROLL_STEP                     10.
#define HIT_ZONE_RANGE                      4.
#define VELOCITY_PAINT_OFFSET               20.

#define TEMPO_MINIMUM                       20
#define TEMPO_MAXIMUM                       300

#define INIT_GROWING_ARRAY_SIZE             28

#define LEARN_QUEUE_SIZE                    256
#define STRING_MAXIMUM_SIZE                 75
#define PATTERN_MAXIMUM_SIZE                26
#define NOVEMBER_MAXIMUM_ITERATE            10
#define JULIET_MAXIMUM_ITERATE              10

#define LEARN_THRESHOLD_MINIMUM             4
#define LEARN_THRESHOLD_RANGE               4
#define LEARN_THRESHOLD_MAXIMUM             8

#define PATCHING_SIZE_MINIMUM               "100. 100."

#define FONTSIZE_LIST                       "8 9 10 11 12 13 14 16 18 20 24 30 36 48 64 72"
#define FONTSTYLE_LIST                      "regular bold italic \"bold italic\""
#define SCALE_TYPE_LIST                     "none custom ionian dorian phrygian lydian mixolydian       \
                                            aeolian locrian natural harmonic melodic \"whole tone\"     \
                                            \"pentatonic major\" \"pentatonic minor\"                   \
                                            \"octatonic half/whole\" \"octatonic whole/half\"           \
                                            \"7th major\" \"7th minor\"                                 \
                                            \"7th dominant\" \"7th major sixth\"                        \
                                            \"7th minor sixth\" \"7th half-diminished\"                 \
                                            \"7th diminished\" \"7th dominant suspended\"               \
                                            \"7th dominant sharp five\"                                 \
                                            \"7th dominant flat five\""                             

#define PATTERN_CELL_LIST                   "none whole half quarter eighth sixteenth thirty-second     \
                                            \"whole triplet\" \"half triplet\"                          \
                                            \"quarter triplet\" \"eighth triplet\"                      \
                                            \"sixteenth triplet\" \"thirty-second triplet\"             \
                                            \"whole dotted\" \"half dotted\"                            \
                                            \"quarter dotted\" \"eighth dotted\"                        \
                                            \"sixteenth dotted\""

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TEXT_CELL_COUNT                     4
#define TEXT_CELL_SPACE                     7.
#define TEXT_BACKGROUND_ALPHA               0.60
#define TEXT_ORDER_VELOCITY                 0
#define TEXT_ORDER_DURATION                 1
#define TEXT_ORDER_CHANNEL                  2
#define TEXT_ORDER_PITCH                    3

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TEXT_MODE_NOTE                      0
#define TEXT_MODE_ZONE                      1
#define TEXT_MODE_MOUSE_PITCH               2
#define TEXT_MODE_MOUSE_VELOCITY            3

#define SEQUENCE_MODE_USER                  0
#define SEQUENCE_MODE_LIVE                  1
#define SEQUENCE_MODE_LISTEN                2

#define ZOOM_MODE_A                         0
#define ZOOM_MODE_B                         1
#define ZOOM_MODE_C                         2

#define FORMAT_MODE_LONG                    0
#define FORMAT_MODE_NOTENAME                1
#define FORMAT_MODE_TICKS                   2
#define FORMAT_MODE_GRID                    3

#define MENU_SEQUENCE                       0
#define MENU_NOTE                           1

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define HIT_NOTHING                         (0L)
#define HIT_NOTE                            (1<<0)
#define HIT_START                           (1<<1)
#define HIT_END                             (1<<2)
#define HIT_DOWN                            (1<<3)
#define HIT_UP                              (1<<4)
#define HIT_ZONE                            (1<<5)
#define HIT_TEXT                            (1<<6)
#define HIT_LOCK                            (1<<7)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DIRTY_NONE                          (0L)
#define DIRTY_REFRESH                       (1<<0)
#define DIRTY_CHANGE                        (1<<1)
#define DIRTY_GRID                          (1<<2)
#define DIRTY_NOTES                         (1<<3)
#define DIRTY_ZONE                          (1<<4)  
#define DIRTY_PLAYED                        (1<<5)
#define DIRTY_LOCATE_LEFT                   (1<<6)
#define DIRTY_LOCATE_RIGHT                  (1<<7)
#define DIRTY_LOCATE_DOWN                   (1<<8)
#define DIRTY_LOCATE_UP                     (1<<9)

#define FLAG_NONE                           (0L)
#define FLAG_FOCUS                          (1<<0)
#define FLAG_HAVE_MOVED                     (1<<1)
#define FLAG_HAVE_CHANGED                   (1<<2)
#define FLAG_HAVE_BEEN_DUPLICATED           (1<<3)
#define FLAG_ZONE_IS_SELECTED               (1<<4)
#define FLAG_ORIGIN_IS_SET                  (1<<5)
#define FLAG_INHIBIT_START                  (1<<6)
#define FLAG_INHIBIT_BANG                   (1<<7)
#define FLAG_IS_LASSO                       (1<<8)
#define FLAG_IS_LOOPED                      (1<<9)
#define FLAG_IS_RUNNING                     (1<<10)
#define FLAG_IS_PAUSED                      (1<<11)
#define FLAG_IS_MUTED                       (1<<12)
#define FLAG_INIT_PAINT_CLOCK               (1<<13)
#define FLAG_ORIGIN_HAD_SHIFT_KEY           (1<<14)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DEFAULT_WINDOW_OFFSET_X                     "-37."
#define DEFAULT_WINDOW_OFFSET_Y                     "658."

#define DEFAULT_FONTNAME                            "Verdana"
#define DEFAULT_FONTSIZE                            "20."
#define DEFAULT_FONTFACE                            "1"
#define DEFAULT_POPUP_FONTNAME                      "Arial"
#define DEFAULT_POPUP_FONTSIZE                      "14."
#define DEFAULT_POPUP_FONTFACE                      "0"
#define DEFAULT_PATCHING_RECT                       "0. 0. 250. 250."

#define DEFAULT_SEQUENCE_MODE                       "0"
#define DEFAULT_ZOOM_MODE                           "0"
#define DEFAULT_VIEW_TEXT                           "1"

#define DEFAULT_TEMPO                               "120"
#define DEFAULT_CHANNEL                             "1"
#define DEFAULT_SCALE_KEY                           "C"
#define DEFAULT_SCALE_TYPE                          "none"
#define DEFAULT_SCALE_CUSTOM                        "0 0 0 0 0 0 0 0 0 0 0 0"
#define DEFAULT_PATTERN_CELL                        "none"
#define DEFAULT_PATTERN_CUSTOM                      ""
#define DEFAULT_CHANCE                              "100"

#define DEFAULT_BACKGROUND_COLOR                    "1.00 1.00 1.00 1."

#define DEFAULT_UNFOCUSED_BORDER_COLOR              "0.83 0.74 0.84 0."
#define DEFAULT_UNFOCUSED_TEXT_COLOR                "0.83 0.74 0.84 1."
#define DEFAULT_FOCUSED_BORDER_COLOR                "0.36 0.36 0.36 0."
#define DEFAULT_FOCUSED_TEXT_COLOR                  "0.15 0.15 0.15 1."
#define DEFAULT_SELECTED_TEXT_COLOR                 "1.00 0.75 0.00 1."
#define DEFAULT_POPUP_TEXT_COLOR                    "0.00 0.00 0.00 1."
#define DEFAULT_POPUP_BACKGROUND_COLOR              "1.00 1.00 1.00 1."
#define DEFAULT_POPUP_HIGHLIGHTED_TEXT_COLOR        "1.00 1.00 1.00 1."
#define DEFAULT_POPUP_HIGHLIGHTED_BACKGROUND_COLOR  "0.39 0.28 0.38 1."

#define DEFAULT_LASSO_COLOR                         "0.00 0.62 0.93 0.1"

#define DEFAULT_FOCUSED_USER_ZONE_COLOR             "0.79 1.00 0.70 0.15"
#define DEFAULT_FOCUSED_LIVE_ZONE_COLOR             "0.62 0.55 0.83 0.25"
#define DEFAULT_FOCUSED_SELECTED_ZONE_COLOR         "0.62 0.55 0.83 0.15"
#define DEFAULT_UNFOCUSED_ZONE_COLOR                "0.90 0.90 0.90 0.10"

#define DEFAULT_C_NOTE_COLOR                        "0.83 0.74 0.84 1."
#define DEFAULT_C_SHARP_NOTE_COLOR                  "0.62 0.55 0.83 1."
#define DEFAULT_D_NOTE_COLOR                        "0.83 0.36 0.54 1."
#define DEFAULT_D_SHARP_NOTE_COLOR                  "0.50 0.49 0.99 1."
#define DEFAULT_E_NOTE_COLOR                        "0.93 0.36 0.72 1."
#define DEFAULT_F_NOTE_COLOR                        "0.80 0.90 0.47 1."
#define DEFAULT_F_SHARP_NOTE_COLOR                  "0.84 0.74 0.43 1."
#define DEFAULT_G_NOTE_COLOR                        "1.00 0.79 0.37 1."
#define DEFAULT_G_SHARP_NOTE_COLOR                  "1.00 0.46 0.46 1."
#define DEFAULT_A_NOTE_COLOR                        "0.88 0.86 0.40 1."
#define DEFAULT_A_SHARP_NOTE_COLOR                  "0.67 0.80 0.57 1."
#define DEFAULT_B_NOTE_COLOR                        "0.67 0.83 0.30 1."

#define DEFAULT_UNFOCUSED_USER_NOTE_COLOR               "0.83 0.74 0.84 1."
#define DEFAULT_UNFOCUSED_USER_SELECTED_NOTE_COLOR      "0.58 0.40 0.53 1."
#define DEFAULT_FOCUSED_USER_MARKED_NOTE_COLOR          "1.00 0.00 0.00 1."
#define DEFAULT_FOCUSED_USER_SELECTED_NOTE_COLOR        "0.15 0.15 0.15 1."
#define DEFAULT_FOCUSED_USER_SILENT_NOTE_COLOR          "0.40 0.40 0.40 0.1"

#define DEFAULT_UNFOCUSED_LIVE_NOTE_COLOR               "0.69 0.55 0.71 1."
#define DEFAULT_UNFOCUSED_LIVE_PLAYED_NOTE_COLOR        "0.49 0.42 0.51 1."
#define DEFAULT_FOCUSED_LIVE_NOTE_COLOR                 "0.42 0.44 0.52 1."
#define DEFAULT_FOCUSED_LIVE_PLAYED_NOTE_COLOR          "1.00 0.60 0.00 1."

#define DEFAULT_UNFOCUSED_LISTEN_NOTE_COLOR             "0.94 0.88 0.94 1."
#define DEFAULT_FOCUSED_LISTEN_NOTE_COLOR               "1.00 0.59 0.42 1."

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define VIEWTEXT        (x->viewText == 1)
#define USER            (x->sequenceMode == 0)
#define LIVE            (x->sequenceMode == 1)
#define LISTEN          (x->sequenceMode == 2)
#define CMD             (modifiers & eCommandKey)
#define SHIFT           (modifiers & eShiftKey)
#define CTRL            (modifiers & eControlKey)
#define ALT             (modifiers & eAltKey)
#define CAPS            (modifiers & eCapsLock)
#define RIGHT           (modifiers & eRightButton)
#define SHARP           (keycode == 167)
#define ALL             (keycode == 97)
#define CUT             (keycode == 120)
#define COPY            (keycode == 99)
#define PASTE           (keycode == 118)
#define TERNARY         (keycode == 116)
#define DOTTED          (keycode == 100)
#define UNDO            (!(modifiers & eShiftKey) && (keycode == 122))
#define REDO            ((modifiers & eShiftKey) && (keycode == 122)) 
#define SELECT          0
#define INVERT          1
#define UP              0
#define DOWN            1

#define DIRTYSLOTS      if (x->saveSlotsWithPatcher) {                                      \
                                jpatcher_set_dirty (jbox_get_patcher ((t_object *)x), 1);   \
                            }   
#define DIRTYPATTR      clock_fdelay (x->notifyClock, NOTIFY_CLOCK_INTERVAL); 
#define DIRTYCHANNEL    if (x->saveChannelWithPatcher) {                                    \
                                jpatcher_set_dirty (jbox_get_patcher ((t_object *)x), 1);   \
                            }   
#define DIRTYUNDO       tralala_addUndo (x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define ARRAY_GET(name)         PIZGrowingArray *(name) = pizArrayPoolGetArray (x->arrayPool)
#define ARRAY_RELEASE(name)     pizArrayPoolReleaseArray (x->arrayPool, (name))

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef MAC_VERSION

    typedef volatile uint32_t t_uint32_atomic;
    
    #define DIRTYLAYER_SET(mask)        OSAtomicOr32((uint32_t)(mask), (uint32_t *)&x->dirtyLayer)  
    #define DIRTYLAYER_UNSET(mask)      OSAtomicAnd32((uint32_t)(mask), (uint32_t *)&x->dirtyLayer)
    
#else
    
    typedef volatile unsigned long t_uint32_atomic;
    
    #define DIRTYLAYER_SET(mask)        x->dirtyLayer |= (mask)
    #define DIRTYLAYER_UNSET(mask)      x->dirtyLayer &= (mask)
    
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
typedef struct _tralala {
    t_jbox              box;
    t_atom              playedNote[4];
    t_atom              dumpedNote[5];
    PIZSequence         *user;
    PIZSequence         *live;
    PIZSequence         *listen;
    PIZGrowingArray     *result;
    PIZArrayPool        *arrayPool;
    long                flags;
    long                runIndex;
    long                tempo;
    long                chance; 
    long                velocity;                                       //
    long                channel; 
    long                slotIndex;  
    long                learnCycle;
    long                learnThreshold;
    t_uint32_atomic     dirtyLayer;
    PIZLinklist         *slots;
    PIZLinklist         *undo;
    PIZLinklist         *redo;
    PIZGrowingArray     *valuesToBeLearned;
    PIZBoundedQueue     *learnQueue;
    PIZFactorOracle     *factorOracle;      
    PIZGaloisLattice    *galoisLattice;
    PIZFiniteState      *finiteState; 
    t_systhread_mutex   methodMutex;    
    t_systhread_mutex   algorithmMutex;
    t_systhread_mutex   learnMutex;                                     //
    t_systhread_mutex   arrayMutex;
    void                *paintClock;
    void                *learnClock;
    void                *focusClock;
    void                *notifyClock;
    void                *runClock;
    void                *goToStartClock;
    void                *inhibitStartClock;
    void                *inhibitBangClock;
    void                *leftOutlet;
    void                *middleLeftOutlet;
    void                *middleRightOutlet;
    void                *rightOutlet;
    t_symbol            *patternCell;
    t_symbol            *scaleKey;
    t_symbol            *scaleType;                                     //
    t_symbol            *popupFontName;
    double              popupFontSize;
    long                popupFontFace;
    long                scaleCustom     [PIZ_SEQUENCE_SCALE_SIZE];      //
    long                patternCustom   [PATTERN_MAXIMUM_SIZE]; 
    long                patternSize;
    PIZSnapValue        cell;
    PIZScaleKey         key;
    PIZScaleType        type;
    t_int32_atomic      popupLock;
    t_int32_atomic      paintLock;                                      //
    long                mousePitchValue;
    long                mouseVelocityValue;
    long                zoomMode;
    long                sequenceMode;
    long                textMode;
    long                viewText;
    long                hitTest;
    long                saveSlotsWithPatcher;
    long                saveChannelWithPatcher;
    long                saveValuesWithPatcher;  
    PIZGrowingArray     *unselectedNotes;
    PIZGrowingArray     *selectedNotes;
    PIZGrowingArray     *playedNotes;
    PIZGrowingArray     *zone;
    PIZGrowingArray     *unselectedNotesCopy;
    PIZGrowingArray     *selectedNotesCopy;                             //
    PIZGrowingArray     *playedNotesCopy;
    PIZGrowingArray     *zoneCopy;
    PIZGrowingArray     *originNotes;
    PIZCoordinates      originCoordinates;
    PIZCoordinates      coordinates;    
    double              windowOffsetX;
    double              windowOffsetY;
    t_jmouse_cursortype cursorType;
    t_pt                origin;
    t_pt                previous;                                       //
    t_pt                point;
    double              textPosition    [TEXT_CELL_COUNT];              ////
    double              textWidth       [TEXT_CELL_COUNT];
    bool                textIsSelected  [TEXT_CELL_COUNT];
    t_jtextlayout       *textLayers     [TEXT_CELL_COUNT];
    t_jrgba             backgroundColor;
    t_jrgba             unfocusedBorderColor;
    t_jrgba             unfocusedTextColor;
    t_jrgba             focusedBorderColor;
    t_jrgba             focusedTextColor;
    t_jrgba             selectedTextColor;
    t_jrgba             popupTextColor;
    t_jrgba             popupBackgroundColor;
    t_jrgba             popupHighlightedTextColor;
    t_jrgba             popupHighlightedBackgroundColor;
    t_jrgba             focusedUserZoneColor;
    t_jrgba             focusedLiveZoneColor;
    t_jrgba             focusedSelectedZoneColor;
    t_jrgba             unfocusedZoneColor;
    t_jrgba             lassoColor;
    t_jrgba             cNoteColor;
    t_jrgba             cSharpNoteColor;
    t_jrgba             dNoteColor;
    t_jrgba             dSharpNoteColor;
    t_jrgba             eNoteColor;
    t_jrgba             fNoteColor;
    t_jrgba             fSharpNoteColor;
    t_jrgba             gNoteColor;
    t_jrgba             gSharpNoteColor;
    t_jrgba             aNoteColor;
    t_jrgba             aSharpNoteColor;
    t_jrgba             bNoteColor;
    t_jrgba             unfocusedUserNoteColor;
    t_jrgba             unfocusedUserSelectedNoteColor;
    t_jrgba             focusedUserSelectedNoteColor;
    t_jrgba             focusedUserMarkedNoteColor;
    t_jrgba             focusedUserSilentNoteColor;
    t_jrgba             unfocusedLiveNoteColor;
    t_jrgba             unfocusedLivePlayedNoteColor;
    t_jrgba             focusedLiveNoteColor;
    t_jrgba             focusedLivePlayedNoteColor;
    t_jrgba             unfocusedListenNoteColor;
    t_jrgba             focusedListenNoteColor;
    } t_tralala;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void            *tralala_new                    (t_symbol *s, long argc, t_atom *argv);
void            tralala_free                    (t_tralala *x);
void            tralala_assist                  (t_tralala *x, void *b, long m, long a, char *s);
void            tralala_jsave                   (t_tralala *x, t_dictionary *d);
t_max_err       tralala_setvalueof              (t_tralala *x, long argc, t_atom *argv);
t_max_err       tralala_getvalueof              (t_tralala *x, long *argc, t_atom **argv);

PIZ_LOCAL void  tralala_dataToDictionary        (t_tralala *x, t_dictionary *d);
PIZ_LOCAL void  tralala_dataWithDictionary      (t_tralala *x, t_dictionary *d);

t_max_err       tralala_setSequenceMode         (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setChannel              (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setVelocity             (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setChance               (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setScaleKey             (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setScaleType            (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setScaleCustom          (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setPatternCell          (t_tralala *x, t_object *attr, long argc, t_atom *argv);
t_max_err       tralala_setPatternCustom        (t_tralala *x, t_object *attr, long argc, t_atom *argv);

void            tralala_play                    (t_tralala *x);
void            tralala_stop                    (t_tralala *x);
void            tralala_loop                    (t_tralala *x);
void            tralala_unloop                  (t_tralala *x);
void            tralala_pause                   (t_tralala *x);

void            tralala_runTask                 (t_tralala *x);
void            tralala_goToStartTask           (t_tralala *x);
void            tralala_inhibitStartTask        (t_tralala *x);
void            tralala_inhibitBangTask         (t_tralala *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void            tralala_copy                    (t_tralala *x);
void            tralala_enter                   (t_tralala *x);

PIZ_LOCAL void  tralala_setLiveByUser           (t_tralala *x);

void            tralala_int                     (t_tralala *x, long n);
void            tralala_list                    (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void            tralala_mute                    (t_tralala *x, long n);
void            tralala_forget                  (t_tralala *x);

void            tralala_learnTask               (t_tralala *x);

void            tralala_slot                    (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void            tralala_anything                (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void            tralala_handleMessages          (t_tralala *x, t_symbol *s, long argc, t_atom *argv);

PIZ_LOCAL bool  tralala_sequenceClear           (t_tralala *x, PIZSequence *sequence);
PIZ_LOCAL bool  tralala_sequenceClean           (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceNote            (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceZone            (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceDump            (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceRotate          (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceScramble        (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceSort            (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceChange          (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceSet             (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceNovember        (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceJuliet          (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceCycle           (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv);
PIZ_LOCAL bool  tralala_sequenceKill            (t_tralala *x, PIZSequence *sequence);
PIZ_LOCAL bool  tralala_sequenceZoulou          (t_tralala *x, PIZSequence *sequence);
PIZ_LOCAL bool  tralala_sequenceRomeo           (t_tralala *x, PIZSequence *sequence);
PIZ_LOCAL bool  tralala_sequenceUniform         (t_tralala *x, PIZSequence *sequence);

PIZ_LOCAL void  tralala_slotNew                 (t_tralala *x);
PIZ_LOCAL void  tralala_slotNewCopy             (t_tralala *x);

PIZ_LOCAL void  tralala_slotRecall              (t_tralala *x, long n);
PIZ_LOCAL void  tralala_slotStore               (t_tralala *x);
PIZ_LOCAL void  tralala_slotRemove              (t_tralala *x, long n);
PIZ_LOCAL void  tralala_slotSwap                (t_tralala *x, long m, long n);
PIZ_LOCAL void  tralala_slotCopy                (t_tralala *x, long m, long n);

PIZ_LOCAL void  tralala_slotNext                (t_tralala *x);
PIZ_LOCAL void  tralala_slotPrevious            (t_tralala *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void  tralala_undo                    (t_tralala *x);
PIZ_LOCAL void  tralala_redo                    (t_tralala *x);
PIZ_LOCAL void  tralala_addUndo                 (t_tralala *x);
PIZ_LOCAL void  tralala_clearUndo               (t_tralala *x);

void            tralala_mousedown               (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);
void            tralala_mousedrag               (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);
void            tralala_mouseup                 (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);
void            tralala_mouseenter              (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);
void            tralala_mouseleave              (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);
void            tralala_mousemove               (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers);

void            tralala_mousewheel              (t_tralala *x, t_object *view, t_pt pt, long modifiers, 
                                                double x_inc, double y_inc);

void            tralala_key                     (t_tralala *x, t_object *patcherview, 
                                                long keycode, long modifiers, long textcharacter);

PIZ_LOCAL void  tralala_popupRightClickMenu     (t_tralala *x, t_pt pt, long menuMode);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void            tralala_paintTask               (t_tralala *x);
void            tralala_focusTask               (t_tralala *x);
void            tralala_notifyTask              (t_tralala *x);

void            tralala_paint                   (t_tralala *x, t_object *patcherview);
void            tralala_getdrawparams           (t_tralala *x, t_object *patcherview, t_jboxdrawparams *params);

void            tralala_focusgained             (t_tralala *x, t_object *patcherview);
void            tralala_focuslost               (t_tralala *x, t_object *patcherview);
void            tralala_patcherview_vis         (t_tralala *x, t_object *patcherview);
void            tralala_patcherview_invis       (t_tralala *x, t_object *patcherview);

PIZ_LOCAL void  tralala_paintText               (t_tralala *x, t_object *patcherview);
PIZ_LOCAL void  tralala_paintGrid               (t_tralala *x, t_object *patcherview);
PIZ_LOCAL void  tralala_paintNotes              (t_tralala *x, t_object *patcherview);
PIZ_LOCAL void  tralala_paintPlayedNotes        (t_tralala *x, t_object *patcherview);

PIZ_LOCAL void  tralala_paintNoteCandycane      (t_tralala *x, 
                                                t_jgraphics *g,
                                                long position,
                                                long pitch,
                                                long velocity,
                                                long duration); 

PIZ_LOCAL void  tralala_paintNoteWithColor      (t_tralala *x, 
                                                t_jgraphics *g,
                                                long position,
                                                long pitch,
                                                long velocity,
                                                long duration,
                                                t_jrgba *color);

PIZ_LOCAL void  tralala_paintZone               (t_tralala *x, t_object *patcherview);
PIZ_LOCAL void  tralala_paintLasso              (t_tralala *x, t_object *patcherview);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL bool  tralala_moveSelectedNotes                   (t_tralala *x, long deltaPosition, long deltaPitch);
PIZ_LOCAL bool  tralala_changeSelectedNotesDuration         (t_tralala *x, long deltaPosition);
PIZ_LOCAL void  tralala_duplicateSelectedNotes              (t_tralala *x);
PIZ_LOCAL void  tralala_changeSelectedNotesVelocity         (t_tralala *x, bool decrement);
PIZ_LOCAL void  tralala_setSelectedNotesVelocity            (t_tralala *x, long velocity);
PIZ_LOCAL void  tralala_setSelectedNotesChannel             (t_tralala *x, long channel);

PIZ_LOCAL long  tralala_hitZoneWithPoint                    (t_tralala *x, t_pt pt);
PIZ_LOCAL long  tralala_hitTextWithPoint                    (t_tralala *x, t_object *patcherview, t_pt pt);
PIZ_LOCAL bool  tralala_hitNotesByRunIndex                  (t_tralala *x);

PIZ_LOCAL bool  tralala_setCursorType                       (t_tralala *x, t_object *patcherview, 
                                                            t_jmouse_cursortype type);

PIZ_LOCAL bool  tralala_setCoordinatesWithPoint             (t_tralala *x, 
                                                            PIZCoordinates *coordinates, 
                                                            t_pt pt);

PIZ_LOCAL void  tralala_setRectWithZoneValues               (t_tralala *x, 
                                                            t_rect *zoneRect, 
                                                            long start, 
                                                            long end, 
                                                            long down, 
                                                            long up);

PIZ_LOCAL void  tralala_setRectWithCoordinatesAndDuration   (t_tralala *x, 
                                                            t_rect *noteRect, 
                                                            PIZCoordinates *coordinates, 
                                                            long duration); 

PIZ_LOCAL void  tralala_setStringWithLong                   (char *string, 
                                                            long longToBeFormatted, 
                                                            long formatMode);

PIZ_LOCAL void  tralala_unselectAllText                     (t_tralala *x);
PIZ_LOCAL bool  tralala_hasSelectedText                     (t_tralala *x, long *selectedText);

PIZ_LOCAL void  tralala_willChange                          (t_tralala *x);

PIZ_LOCAL void  tralala_testAutoscroll                      (t_tralala *x, t_object *patcherview, t_pt pt);
PIZ_LOCAL void  tralala_stopAutoscroll                      (t_tralala *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
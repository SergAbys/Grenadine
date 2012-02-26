/*
 *  tralalaHI.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 26/02/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define CMD         (modifiers & eCommandKey)
#define SHIFT       (modifiers & eShiftKey)
#define CTRL        (modifiers & eControlKey)
#define ALT         (modifiers & eAltKey)
#define MAJ         (modifiers & eCapsLock)
#define RIGHT       (modifiers & eRightButton)
#define SHARP       (keycode == 167)
#define ALL         (keycode == 97)
#define CUT         (keycode == 120)
#define COPY        (keycode == 99)
#define PASTE       (keycode == 118)
#define TERNARY     (keycode == 116)
#define DOTTED      (keycode == 100)

#define DELTA       0.5
#define HIT_BOTH    (HIT_START | HIT_END)

#define MAIN        0
#define SNAP        1
#define SNAP_T      2
#define SNAP_D      3
#define NOTE        4
#define NOTE_T      5
#define NOTE_D      6
#define VELOCITY    7
#define CHANNEL_N   8
#define CHANNEL_S   9
#define SLOTS       10
#define POP_SIZE    11

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern PIZGrowingArray      *tll_clipboard;
extern PIZError             tll_clipboardError;
extern tralalaSymbolsTableA tll_symbolsA;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_notifyTask (t_tralala *x)
{
    object_notify (x, tll_sym_modified, NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_mousedown (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{  
    tralala_setCoordinates (x, &x->coordinates, pt);
            
    if (USER && CMD && !RIGHT && !MAJ) {
        pizSequenceAddNoteWithCoordinates (x->user, &x->coordinates, PIZ_ADD_FLAG_SNAP); 
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE); 
        DIRTYPATTR DIRTYSLOTS 
        
    } else if (USER && VIEWTEXT && !RIGHT && !MAJ && (x->hitTest = tralala_hitText (x, patcherview, pt))) {
        DIRTYLAYER_SET (DIRTY_REFRESH);
        
    } else if (x->hitTest & HIT_LOCKED) {
        ;
        
    } else if (USER && !CTRL && !RIGHT && MAJ && (x->hitTest = tralala_hitZone (x, pt))) {
        x->flags |= FLAG_ZONE_IS_SELECTED;
        pizSequenceInitTempZone (x->user);
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
        
    } else if (USER && SHIFT && !RIGHT && !MAJ) {
        long k = pizSequenceInvertNoteWithCoordinates (x->user, &x->coordinates);
        if (k != -1) {
            x->hitTest = k;
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
        
    } else if (USER && !MAJ && (x->hitTest = pizSequenceSelectNoteWithCoordinates (x->user, &x->coordinates))) {
        if (RIGHT) {
            tralala_popupRightClickMenu (x, pt, MODE_MENU_NOTE);
        }
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        
    } else if (RIGHT) {
        tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
        tralala_popupRightClickMenu (x, pt, MODE_MENU_SEQUENCE);
        
    } else if (USER) {
        pizSequenceUnselectAllNotes (x->user);
        tralala_unselectAllText (x);
        DIRTYLAYER_SET (DIRTY_REFRESH | DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_mousedrag (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (!RIGHT && USER && !(x->dirtyLayer & DIRTY_SEQUENCE)) {
    //
    long     deltaPitch, deltaPosition;
    long     selectedText = -1;
    PIZError err = PIZ_GOOD;

    if (!(x->flags & FLAG_ORIGIN_IS_SET)) {
        x->originPoint.x   = pt.x;
        x->originPoint.y   = pt.y;
        x->previousPoint.x = pt.x;
        x->previousPoint.y = pt.y;
        
        x->mouseVelocityValue = 0;
        
        tralala_setCoordinates (x, &x->originCoordinates, x->previousPoint);
        
        ARRAYSLOCK
        
        err |= pizGrowingArrayCopy (x->origin, x->selected);
        
        ARRAYSUNLOCK
        
        if (SHIFT) {
            x->flags |= FLAG_ORIGIN_HAD_SHIFT_KEY;
        } else {
            x->flags &= ~FLAG_ORIGIN_HAD_SHIFT_KEY;
        }
        
        x->flags |= FLAG_ORIGIN_IS_SET;
    }
    
    x->point.x = pt.x;
    x->point.y = pt.y;
    
    tralala_setCoordinates (x, &x->coordinates, pt);
        
    deltaPosition = x->coordinates.position - x->originCoordinates.position;
    deltaPitch    = x->coordinates.pitch    - x->originCoordinates.pitch;
        
    if (VIEWTEXT && tralala_hasSelectedText (x, &selectedText)) {
        long k = (long)(ABS (pt.y - x->previousPoint.y));
        
        if ((pt.y - x->previousPoint.y) < -DELTA) {
            switch (selectedText) {
                case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, k); break;
                case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, 1); break;
                case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, 1);  break;
                case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, 1);    break;
            }
        } else if ((pt.y - x->previousPoint.y) > DELTA) {
            switch (selectedText) {
                case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, -k); break;
                case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, -1); break;
                case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, -1);  break;
                case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, -1);    break;
            }
        }
        
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        
    } else if (x->hitTest & HIT_LOCKED) {
        ;
        
    } else if ((x->hitTest == HIT_NOTE) && !err) {
    
        if (ALT && !CTRL && !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED))) {
            tralala_duplicateSelectedNotes (x);
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_COPYING);
            DIRTYLAYER_SET (DIRTY_NOTES);
            
        } else if (CTRL && !(x->flags & FLAG_HAVE_MOVED)) {
        
            if (ALT) {
                x->textMode = MODE_TEXT_MOUSE_VELOCITY;
                
                if ((pt.y - x->previousPoint.y) < -DELTA) {
                    if (tralala_changeSelectedNotesVelocity (x, UP)) {
                        x->flags |= FLAG_HAVE_CHANGED; DIRTYLAYER_SET (DIRTY_NOTES);
                    }
                } else if ((pt.y - x->previousPoint.y) > DELTA) {
                    if (tralala_changeSelectedNotesVelocity (x, DOWN)) {
                        x->flags |= FLAG_HAVE_CHANGED; DIRTYLAYER_SET (DIRTY_NOTES);
                    }
                }
                
            } else if (tralala_changeSelectedNotesDuration (x, deltaPosition)) {
                tralala_testAutoscroll (x, patcherview, pt);
                x->flags |= FLAG_HAVE_CHANGED; DIRTYLAYER_SET (DIRTY_NOTES);
            }
            
        } else {
        
            if (tralala_moveSelectedNotes (x, deltaPosition, deltaPitch)) {
                x->flags |= FLAG_HAVE_MOVED; DIRTYLAYER_SET (DIRTY_NOTES);
            }
            
            if (CMD) {
                x->textMode = MODE_TEXT_MOUSE_PITCH;
                x->mousePitchValue = x->coordinates.pitch;
            } else {
                x->textMode = MODE_TEXT_NOTE;
            }
            
            tralala_testAutoscroll (x, patcherview, pt);
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
        
    } else if (x->hitTest == HIT_ZONE) {
        tralala_testAutoscroll (x, patcherview, pt);
        if (pizSequenceMoveTempZone (x->user, deltaPitch, deltaPosition)) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
        }
        
    } else if (x->hitTest) {
        PIZCoordinates  c1, c2;
        double          a, b, f = 1.;
        bool            draw = false;
                            
        switch (x->zoomMode) {
            case MODE_ZOOM_A : f = 0.5;  break;
            case MODE_ZOOM_B : f = 1.;   break;
            case MODE_ZOOM_C : f = 2.;   break;
        }
        
        b = (GUI_PIXELS_PER_SEMITONE * f);
        a = (GUI_PIXELS_PER_SEMITONE / 2. * f);
        
        c1.position = (long)((x->offsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
        c2.position = (long)((x->offsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
        c1.pitch    = PIZ_MAGIC_PITCH - MAX (((long)((x->offsetY + pt.y - a) / b)), 0);
        c2.pitch    = PIZ_MAGIC_PITCH - MAX (((long)((x->offsetY + pt.y + a) / b)), 0);
                    
        switch (x->hitTest) {
        case HIT_START  : draw = pizSequenceResizeTempZone (x->user, &x->coordinates, PIZ_DATA_START);  break;
        case HIT_END    : draw = pizSequenceResizeTempZone (x->user, &x->coordinates, PIZ_DATA_END);    break;
        case HIT_DOWN   : draw = pizSequenceResizeTempZone (x->user, &c1, PIZ_DATA_DOWN);               break;
        case HIT_UP     : draw = pizSequenceResizeTempZone (x->user, &c2, PIZ_DATA_UP);                 break;
        case HIT_BOTH   : draw = pizSequenceResizeTempZone (x->user, &x->coordinates, PIZ_DATA_END);    break;
        }
    
        tralala_testAutoscroll (x, patcherview, pt);

        if (draw) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
        } 
        
    } else if (MAJ) {
        x->offsetX -= pt.x - x->previousPoint.x;
        x->offsetY -= pt.y - x->previousPoint.y;
        
        tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
        DIRTYLAYER_SET (DIRTY_REFRESH);
        
    } else if (!CMD)  {
        x->flags |= FLAG_IS_LASSO;
        
        if (x->flags & FLAG_ORIGIN_HAD_SHIFT_KEY) {
            if (pizSequenceDragLasso (x->user, &x->originCoordinates, &x->coordinates, INVERT)) {
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            }
        } else if (pizSequenceDragLasso (x->user, &x->originCoordinates, &x->coordinates, SELECT)) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
        
        tralala_testAutoscroll (x, patcherview, pt);
        tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
        DIRTYLAYER_SET (DIRTY_REFRESH);
    }
    
    x->previousPoint.x = pt.x;
    x->previousPoint.y = pt.y;
    //
    } 
    
    if (!USER) {
        tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
    }
}

void tralala_mouseup (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (USER) {
        long selectedText;
            
        if (tralala_hasSelectedText (x, &selectedText)) {
            tralala_unselectAllText (x);
            DIRTYLAYER_SET (DIRTY_REFRESH); 
        }

        if (x->flags & FLAG_ZONE_IS_SELECTED) {
            pizSequencePutTempZone (x->user);
            x->flags &= ~FLAG_ZONE_IS_SELECTED;
            
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
            DIRTYPATTR DIRTYSLOTS
        }
        
        if (x->flags & (FLAG_HAVE_CHANGED | FLAG_HAVE_MOVED | FLAG_HAVE_BEEN_DUPLICATED)) {
            
            ARRAYSLOCK
            
            pizSequenceRemoveSelectedNotes (x->user);
            pizSequenceAddNotesWithArray   (x->user, x->selected, PIZ_ADD_FLAG_NONE);
            
            ARRAYSUNLOCK
            
            x->textMode = MODE_TEXT_NOTE;
            x->flags    &= ~(FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED);
            
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            DIRTYPATTR DIRTYSLOTS
        }
        
        if (x->flags & FLAG_IS_LASSO) {
            x->flags &= ~FLAG_IS_LASSO;
            pizSequenceInitLasso (x->user);
            
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }

        x->flags &= ~FLAG_ORIGIN_IS_SET;
        tralala_stopAutoscroll (x);
    }
    
    if (!CMD && (x->hitTest != HIT_ZONE)) {
        if (tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW) && !MAJ) {
            x->textMode = MODE_TEXT_NOTE;
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
    x->textMode = MODE_TEXT_NOTE;
    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
    DIRTYLAYER_SET (DIRTY_REFRESH);
}

void tralala_mousemove (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{   
    if (x->flags & FLAG_FOCUS) {
        long isHit;
        
        tralala_setCoordinates (x, &x->coordinates, pt);
        
        if (CMD && !MAJ) {  
            if (USER) {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_CROSSHAIR);
            } else {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
            }
            
            x->textMode = MODE_TEXT_MOUSE_PITCH;
            x->mousePitchValue = x->coordinates.pitch;
            
            DIRTYLAYER_SET (DIRTY_REFRESH);
            
        } else if (USER && MAJ) {
            if ((isHit = tralala_hitZone (x, pt)) == HIT_ZONE) {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
            } else if (isHit) {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_POINTINGHAND);
            } else {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
            }
            
            if (x->textMode != MODE_TEXT_ZONE) {
                x->textMode = MODE_TEXT_ZONE;
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
            
        } else {
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                        
            if (x->textMode != MODE_TEXT_NOTE)  {
                x->textMode = MODE_TEXT_NOTE;
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
        }  
    }
}

void tralala_mousewheel (t_tralala *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc)
{
    if (ALT) {
        t_rect  rect;
        bool    k = false;
        
        jbox_get_rect_for_view ((t_object *)x, view, &rect);    

        if (y_inc > 0.) {
            switch (x->zoomMode) {
                case MODE_ZOOM_A :  x->zoomMode = MODE_ZOOM_B; k = true; break;
                case MODE_ZOOM_B :  x->zoomMode = MODE_ZOOM_C; k = true; break;
            }
            
            if (k) {
                x->offsetX = ((2. * x->offsetX) + rect.width) - (rect.width  / 2.); 
                x->offsetY = ((2. * x->offsetY) + rect.height) - (rect.height  / 2.);
            }
            
        } else {
            switch (x->zoomMode) {
                case MODE_ZOOM_C :  x->zoomMode = MODE_ZOOM_B; k = true; break;
                case MODE_ZOOM_B :  x->zoomMode = MODE_ZOOM_A; k = true; break;
            }
            
            if (k) {
                x->offsetX = (((2. * x->offsetX) + rect.width) / 4.) - (rect.width / 2.);
                x->offsetY = (((2. * x->offsetY) + rect.height) / 4.) - (rect.height / 2.);
            }
        }
        
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES);
        
    } else {
        x->offsetX -= x_inc * GUI_MOUSEWHEEL_FACTOR;
        x->offsetY -= y_inc * GUI_MOUSEWHEEL_FACTOR;
            
        DIRTYLAYER_SET (DIRTY_REFRESH);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
                                    
void tralala_key (t_tralala *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{
    bool dirty = false;
    
    if (SHARP || (keycode == JKEY_SPACEBAR)) {
        switch (x->sequenceMode) {
            case MODE_SEQUENCE_USER : object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_LIVE); break;
            case MODE_SEQUENCE_LIVE : object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_USER); break;
        }
        
    } else if (keycode == JKEY_UPARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
        pizSequenceTranspose (x->user, PIZ_MAGIC_SCALE);
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_SEQUENCE);
        dirty = true;
        
    } else if (keycode == JKEY_DOWNARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
        pizSequenceTranspose (x->user, -PIZ_MAGIC_SCALE);
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_SEQUENCE);
        dirty = true;
        
    } else if (keycode == JKEY_ENTER) {
        tralala_setLiveByUser (x);
        
        if (LIVE) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_SEQUENCE);
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
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        dirty = true;
        
    } else if (USER && CMD && !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED))) {
        if (ALL) {
            pizSequenceSelectAllNotes (x->user);
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            
        } else if (COPY)  {
            pizGrowingArrayClear (tll_clipboard);
            tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            
        } else if (CUT)  {
            pizGrowingArrayClear (tll_clipboard);
            tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
            pizSequenceRemoveSelectedNotes (x->user);
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            dirty = true;
            
        } else if (PASTE) {
            if (!tll_clipboardError && tralala_pasteFromClipboard (x)) {
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
                dirty = true;
            }
        }

    } else if (keycode >= 49 && keycode <= 55) {
    //
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
        PIZNoteValue k = PIZ_NOTE_NONE;
        
        switch (keycode) {
            case 55 : k = PIZ_NOTE_NONE;            break;
            case 54 : k = PIZ_WHOLE_NOTE;           break;
            case 53 : k = PIZ_HALF_NOTE;            break;
            case 52 : k = PIZ_QUARTER_NOTE;         break;
            case 51 : k = PIZ_EIGHTH_NOTE;          break;
            case 50 : k = PIZ_SIXTEENTH_NOTE;       break;
            case 49 : k = PIZ_THIRTY_SECOND_NOTE;   break;
        }
        
        if (SHIFT) {
            if (x->noteValue != k) {
                pizSequenceSetNoteValue (x->user, x->noteValue = k); 
                dirty = true;
                DIRTYLAYER_SET (DIRTY_REFRESH);
            }
        } else {
            if (x->grid != k) {
                pizSequenceSetGrid (x->user, x->grid = k); 
                pizSequenceSetCell (x->user, k);
                dirty = true;
                DIRTYLAYER_SET (DIRTY_GRID);
            }
        }
            
    } ATOMIC_DECREMENT (&x->popupLock);
    //
    } else if (TERNARY || DOTTED) {
    //
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
    
        PIZNoteValue old, new = PIZ_NOTE_NONE;
        
        if (!SHIFT) {
            old = x->grid;
        } else {
            old = x->noteValue;
        }
         
        if (TERNARY) {
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
                case PIZ_NOTE_NONE                  :   new = PIZ_NOTE_NONE;                    break;
            }
        } else {
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
                case PIZ_NOTE_NONE                  :   new = PIZ_NOTE_NONE;                    break;
            }
        }
        
        if (!SHIFT) {
            pizSequenceSetGrid (x->user, x->grid = new);
            pizSequenceSetCell (x->user, new);
            dirty = true;
            DIRTYLAYER_SET (DIRTY_GRID);
        } else {
            pizSequenceSetNoteValue (x->user, x->noteValue = new);
            dirty = true;
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
        
    } ATOMIC_DECREMENT (&x->popupLock);
    //    
    }
    
    if (dirty) {
        DIRTYPATTR DIRTYSLOTS
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_popupRightClickMenu (t_tralala *x, t_pt pt, long menuMode)
{
    t_pt            locate;
    t_jfont         *font;
    t_jpopupmenu    *pop[POP_SIZE];
    int             mouseX, mouseY;
    long            i, t, popup = 0;  
    bool            dirty = false;

    jmouse_getposition_global (&mouseX, &mouseY);
    
    locate.x = (double)mouseX;
    locate.y = (double)mouseY;
    
    font = jfont_create (x->popupFontName->s_name, 
                        (x->popupFontFace & (1<<1)), (x->popupFontFace & (1<<0)), 
                        x->popupFontSize); 
    
    for (i = 0; i < POP_SIZE; i++) {
        pop[i] = jpopupmenu_create ( );
        
        jpopupmenu_setcolors (pop[i], 
                             x->popupTextColor, x->popupBackgroundColor, 
                             x->popupHighlightedTextColor, x->popupHighlightedBackgroundColor); 
                             
        jpopupmenu_setfont   (pop[i], font); 
    }
    
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
    //
    if (menuMode == MODE_MENU_SEQUENCE) {
    //
    t = x->grid;
    
    jpopupmenu_additem (pop[SNAP_T], 17, "Whole",         NULL, (t == PIZ_WHOLE_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[SNAP_T], 18, "Half",          NULL, (t == PIZ_HALF_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[SNAP_T], 19, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[SNAP_T], 20, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[SNAP_T], 21, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[SNAP_T], 22, "Thirty-second", NULL, (t == PIZ_THIRTY_SECOND_NOTE_TRIPLET), 0, NULL);
    
    jpopupmenu_additem (pop[SNAP_D], 23, "Whole",         NULL, (t == PIZ_WHOLE_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[SNAP_D], 24, "Half",          NULL, (t == PIZ_HALF_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[SNAP_D], 25, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[SNAP_D], 26, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[SNAP_D], 27, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE_DOTTED), 0, NULL);
    
    jpopupmenu_additem      (pop[SNAP], 11, "Whole",         NULL, (t == PIZ_WHOLE_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[SNAP], 12, "Half",          NULL, (t == PIZ_HALF_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[SNAP], 13, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[SNAP], 14, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[SNAP], 15, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[SNAP], 16, "Thirty-second", NULL, (t == PIZ_THIRTY_SECOND_NOTE), 0, NULL);
    
    jpopupmenu_addseperator (pop[SNAP]);
    jpopupmenu_addsubmenu   (pop[SNAP],     "Triplet",       pop[SNAP_T], 0);
    jpopupmenu_addsubmenu   (pop[SNAP],     "Dotted ",       pop[SNAP_D], 0);
    jpopupmenu_addseperator (pop[SNAP]);
    jpopupmenu_additem      (pop[SNAP], 10, "None",          NULL, (t == PIZ_NOTE_NONE), 0, NULL);
    
    t = x->channel;
    
    jpopupmenu_additem (pop[CHANNEL_S], 301, "1",   NULL, (t == 1), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 302, "2",   NULL, (t == 2), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 303, "3",   NULL, (t == 3), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 304, "4",   NULL, (t == 4), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 305, "5",   NULL, (t == 5), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 306, "6",   NULL, (t == 6), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 307, "7",   NULL, (t == 7), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 308, "8",   NULL, (t == 8), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 309, "9",   NULL, (t == 9), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 310, "10",  NULL, (t == 10), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 311, "11",  NULL, (t == 11), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 312, "12",  NULL, (t == 12), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 313, "13",  NULL, (t == 13), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 314, "14",  NULL, (t == 14), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 315, "15",  NULL, (t == 15), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_S], 316, "16",  NULL, (t == 16), 0, NULL);
    //
    }
    
    if ((menuMode == MODE_MENU_SEQUENCE) && USER) {
    //
    
    jpopupmenu_additem      (pop[SLOTS],    500, "New", NULL, 0, 0, NULL);
    jpopupmenu_additem      (pop[SLOTS],    501, "Clone", NULL, 0, 0, NULL);
    jpopupmenu_addseperator (pop[SLOTS]);
    
    for (i = 0; i < pizLinklistCount (x->slots); i++) {
        char text[SIZE_STRING_MAX];
        snprintf (text, SIZE_STRING_MAX, "Slot %ld", i); 
        text[SIZE_STRING_MAX - 1] = 0;
        jpopupmenu_additem (pop[SLOTS],     503 + i, text, NULL, (i == x->slotIndex), 0, NULL);
    }
    
    jpopupmenu_addseperator (pop[SLOTS]);
    jpopupmenu_additem      (pop[SLOTS],    502, "Remove ", NULL, 0, 0, NULL);
    
    t = x->noteValue;
    
    jpopupmenu_additem (pop[NOTE_T], 87, "Whole",         NULL, (t == PIZ_WHOLE_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[NOTE_T], 88, "Half",          NULL, (t == PIZ_HALF_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[NOTE_T], 89, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[NOTE_T], 90, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[NOTE_T], 91, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE_TRIPLET), 0, NULL);
    jpopupmenu_additem (pop[NOTE_T], 92, "Thirty-second", NULL, (t == PIZ_THIRTY_SECOND_NOTE_TRIPLET), 0, NULL);
    
    jpopupmenu_additem (pop[NOTE_D], 93, "Whole",         NULL, (t == PIZ_WHOLE_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[NOTE_D], 94, "Half",          NULL, (t == PIZ_HALF_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[NOTE_D], 95, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[NOTE_D], 96, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE_DOTTED), 0, NULL);
    jpopupmenu_additem (pop[NOTE_D], 97, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE_DOTTED), 0, NULL);

    jpopupmenu_additem      (pop[NOTE], 81, "Whole",         NULL, (t == PIZ_WHOLE_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[NOTE], 82, "Half",          NULL, (t == PIZ_HALF_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[NOTE], 83, "Quarter",       NULL, (t == PIZ_QUARTER_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[NOTE], 84, "Eighth",        NULL, (t == PIZ_EIGHTH_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[NOTE], 85, "Sixteenth",     NULL, (t == PIZ_SIXTEENTH_NOTE), 0, NULL);
    jpopupmenu_additem      (pop[NOTE], 86, "Thirty-second", NULL, (t == PIZ_THIRTY_SECOND_NOTE), 0, NULL);
    
    jpopupmenu_addseperator (pop[NOTE]);
    jpopupmenu_addsubmenu   (pop[NOTE],     "Triplet ",      pop[NOTE_T], 0);
    jpopupmenu_addsubmenu   (pop[NOTE],     "Dotted ",       pop[NOTE_D], 0);
    jpopupmenu_addseperator (pop[NOTE]);
    jpopupmenu_additem      (pop[NOTE], 80, "Automatic",     NULL, (t == PIZ_NOTE_NONE), 0, NULL);

    //
    }
    
    if (menuMode == MODE_MENU_NOTE) {
    //
    long velocity = -1;
    long channel = -1;

    ARRAYSLOCK
    
    if (pizGrowingArrayCount (x->selected) == PIZ_DATA_NOTE_SIZE) {
        channel  = pizGrowingArrayValueAtIndex (x->selected, PIZ_DATA_CHANNEL);
        velocity = pizGrowingArrayValueAtIndex (x->selected, PIZ_DATA_VELOCITY);
        velocity = CLAMP ((long)((velocity + 4) / 8) * 8, 0, 127);
    }

    ARRAYSUNLOCK
    
    jpopupmenu_additem (pop[VELOCITY], 50, "0",     NULL, (velocity == 0), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 51, "8",     NULL, (velocity == 8), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 52, "16",    NULL, (velocity == 16), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 53, "24",    NULL, (velocity == 24), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 54, "32",    NULL, (velocity == 32), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 55, "40",    NULL, (velocity == 40), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 56, "48",    NULL, (velocity == 48), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 57, "56",    NULL, (velocity == 56), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 58, "64",    NULL, (velocity == 64), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 59, "72",    NULL, (velocity == 72), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 60, "80",    NULL, (velocity == 80), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 61, "88",    NULL, (velocity == 88), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 62, "96",    NULL, (velocity == 96), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 63, "104",   NULL, (velocity == 104), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 64, "112",   NULL, (velocity == 112), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 65, "120",   NULL, (velocity == 120), 0, NULL);
    jpopupmenu_additem (pop[VELOCITY], 66, "127",   NULL, (velocity == 127), 0, NULL);

    jpopupmenu_additem (pop[CHANNEL_N], 200, "0",   NULL, (channel == 0), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 201, "1",   NULL, (channel == 1), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 202, "2",   NULL, (channel == 2), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 203, "3",   NULL, (channel == 3), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 204, "4",   NULL, (channel == 4), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 205, "5",   NULL, (channel == 5), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 206, "6",   NULL, (channel == 6), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 207, "7",   NULL, (channel == 7), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 208, "8",   NULL, (channel == 8), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 209, "9",   NULL, (channel == 9), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 210, "10",  NULL, (channel == 10), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 211, "11",  NULL, (channel == 11), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 212, "12",  NULL, (channel == 12), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 213, "13",  NULL, (channel == 13), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 214, "14",  NULL, (channel == 14), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 215, "15",  NULL, (channel == 15), 0, NULL);
    jpopupmenu_additem (pop[CHANNEL_N], 216, "16",  NULL, (channel == 16), 0, NULL);
    //
    }
    
    if (menuMode == MODE_MENU_SEQUENCE) {
        jpopupmenu_additem      (pop[MAIN], 100,    "User",         NULL, USER, 0, NULL);
        jpopupmenu_additem      (pop[MAIN], 101,    "Live",         NULL, LIVE, 0, NULL);
        jpopupmenu_addseperator (pop[MAIN]);
        
        if (USER) {
            jpopupmenu_addseperator (pop[MAIN]);
            jpopupmenu_addsubmenu   (pop[MAIN],     "Slots",        pop[SLOTS], 0);
            jpopupmenu_addseperator (pop[MAIN]);
            jpopupmenu_addsubmenu   (pop[MAIN],     "Snap",         pop[SNAP], 0);
            jpopupmenu_addsubmenu   (pop[MAIN],     "Value",        pop[NOTE], 0);
        }
        
        jpopupmenu_addseperator (pop[MAIN]);
        jpopupmenu_addsubmenu   (pop[MAIN],         "Channel    ",  pop[CHANNEL_S], 0);
    } else {
        jpopupmenu_addsubmenu   (pop[MAIN],         "Velocity",     pop[VELOCITY], 0);
        jpopupmenu_addsubmenu   (pop[MAIN],         "Channel    ",  pop[CHANNEL_N], 0);
    }

    popup = jpopupmenu_popup (pop[MAIN], locate, 0);
    //
    } ATOMIC_DECREMENT (&x->popupLock);
    
    if (popup == 500) {
        tralala_slotNew (x); 
    } else if (popup == 501) {
        tralala_slotNewCopy (x);
    } else if (popup == 502) {
        tralala_slotRemove (x, x->slotIndex);
    } else if (popup >= 503) {
        tralala_slotRecall (x, popup - 503); 
    } else if ((popup >= 301) && (popup <= 316)) {
        object_attr_setlong (x, tll_sym_channel, popup - 300); 
    } else if ((popup >= 200) && (popup <= 216)) {
        tralala_setSelectedNotesChannel (x, popup - 200);  
        x->flags |= FLAG_HAVE_CHANGED;
    } else if ((popup >= 50) && (popup <= 66)) {
        tralala_setSelectedNotesVelocity (x, CLAMP (((popup - 50) * 8), 0, PIZ_MAGIC_VELOCITY));    
        x->flags |= FLAG_HAVE_CHANGED; 
    } else {
    //
    switch (popup) {
        case 10     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_NOTE_NONE);                    break;
        case 11     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_WHOLE_NOTE);                   break;
        case 12     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_HALF_NOTE);                    break;
        case 13     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_QUARTER_NOTE);                 break;
        case 14     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_EIGHTH_NOTE);                  break;
        case 15     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_SIXTEENTH_NOTE);               break;
        case 16     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_THIRTY_SECOND_NOTE);           break;
        case 17     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_WHOLE_NOTE_TRIPLET);           break;
        case 18     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_HALF_NOTE_TRIPLET);            break;
        case 19     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_QUARTER_NOTE_TRIPLET);         break;
        case 20     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_EIGHTH_NOTE_TRIPLET);          break;
        case 21     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_SIXTEENTH_NOTE_TRIPLET);       break;
        case 22     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_THIRTY_SECOND_NOTE_TRIPLET);   break;
        case 23     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_WHOLE_NOTE_DOTTED);            break;
        case 24     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_HALF_NOTE_DOTTED);             break;
        case 25     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_QUARTER_NOTE_DOTTED);          break;
        case 26     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_EIGHTH_NOTE_DOTTED);           break;
        case 27     :   pizSequenceSetGrid  (x->user, x->grid = PIZ_SIXTEENTH_NOTE_DOTTED);        break;
        case 80     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_NOTE_NONE);                  break;
        case 81     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_WHOLE_NOTE);                 break;
        case 82     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_HALF_NOTE);                  break;
        case 83     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_QUARTER_NOTE);               break;
        case 84     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_EIGHTH_NOTE);                break;
        case 85     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_SIXTEENTH_NOTE);             break;
        case 86     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_THIRTY_SECOND_NOTE);         break;
        case 87     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_WHOLE_NOTE_TRIPLET);         break;
        case 88     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_HALF_NOTE_TRIPLET);          break;
        case 89     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_QUARTER_NOTE_TRIPLET);       break;
        case 90     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_EIGHTH_NOTE_TRIPLET);        break;
        case 91     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_SIXTEENTH_NOTE_TRIPLET);     break;
        case 92     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_THIRTY_SECOND_NOTE_TRIPLET); break;
        case 93     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_WHOLE_NOTE_DOTTED);          break;
        case 94     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_HALF_NOTE_DOTTED);           break;
        case 95     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_QUARTER_NOTE_DOTTED);        break;
        case 96     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_EIGHTH_NOTE_DOTTED);         break;
        case 97     :   pizSequenceSetNoteValue (x->user, x->noteValue = PIZ_SIXTEENTH_NOTE_DOTTED);      break;
        case 100    :   object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_USER); break;
        case 101    :   object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_LIVE); break;
    }
    //
    }
    if (x->flags & FLAG_HAVE_CHANGED) {
       
        ARRAYSLOCK
        
        pizSequenceRemoveSelectedNotes  (x->user);
        pizSequenceAddNotesWithArray    (x->user, x->selected, PIZ_ADD_FLAG_NONE);
        
        ARRAYSUNLOCK
            
        x->flags &= ~FLAG_HAVE_CHANGED;
        dirty = true;    
        DIRTYLAYER_SET (DIRTY_NOTES);
    }
    
    if (popup >= 10 && popup <= 27) {
        pizSequenceSetCell (x->user, x->grid);
        dirty = true;
        DIRTYLAYER_SET (DIRTY_GRID);
    }
    
    if (popup >= 80 && popup <= 97) {
        dirty = true;
        DIRTYLAYER_SET (DIRTY_GRID);
    }

    jfont_destroy (font);

    for (i = 0; i < POP_SIZE; i++) {
        jpopupmenu_destroy (pop[i]);
    }
    
    if (dirty) {
        DIRTYPATTR DIRTYSLOTS
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
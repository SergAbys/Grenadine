/*
 *  tralalaHI.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 20/02/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern PIZGrowingArray          *tll_clipboard;
extern PIZError                 tll_clipboardError;
extern tralalaSymbolsTableA     tll_symbolsA;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralala_mousedown (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{   
    tralala_setCoordinatesWithPoint (x, &x->coordinates, pt);
            
    if (USER && CMD && !RIGHT && !CAPS) {
        if (!pizSequenceAddNoteWithCoordinates (x->user, &x->coordinates, PIZ_ADD_FLAG_SNAP)) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
                
            DIRTYSLOTS
            DIRTYPATTR
        } 
    } else if (USER && VIEWTEXT && !RIGHT && !CAPS && 
        (x->hitTest = tralala_hitTextWithPoint (x, patcherview, pt))) {
            DIRTYLAYER_SET (DIRTY_TEXT);
    } else if (x->hitTest & HIT_LOCKED) {
        ;
    } else if (USER && !CTRL && !RIGHT && CAPS && (x->hitTest = tralala_hitZoneWithPoint (x, pt))) {
        x->flags |= FLAG_ZONE_IS_SELECTED;
            
        pizSequenceInitTempZone (x->user);

        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_LOAD);
    } else if (USER && SHIFT && !RIGHT && !CAPS) {
        long k = pizSequenceInvertNoteWithCoordinates (x->user, &x->coordinates);
            
        if (k != -1) {
            x->hitTest = k;
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        }
    } else if (USER && !CAPS && 
        (x->hitTest = pizSequenceSelectNoteWithCoordinates (x->user, &x->coordinates))) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
                        
        if (RIGHT) {
            tralala_popupRightClickMenu (x, pt, MODE_MENU_NOTE);
        }
    } else if (RIGHT) {
        tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
        tralala_popupRightClickMenu (x, pt, MODE_MENU_SEQUENCE);
    } else if (USER) {
        pizSequenceUnselectAllNotes (x->user);
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
                    
        tralala_unselectAllText (x);
        DIRTYLAYER_SET (DIRTY_TEXT);
    }
}

void tralala_mousedrag (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if (!RIGHT && USER && !(x->dirtyLayer & DIRTY_LOAD)) {
        long        deltaPitch, deltaPosition;
        PIZError    err = PIZ_GOOD;
        long        selectedText = -1;

        if (!(x->flags & FLAG_ORIGIN_IS_SET)) {
            x->originPoint.x     = pt.x;
            x->originPoint.y     = pt.y;
            x->previousPoint.x   = pt.x;
            x->previousPoint.y   = pt.y;
            
            x->mouseVelocityValue = 0;
            
            tralala_setCoordinatesWithPoint (x, &x->originCoordinates, x->previousPoint);
            
            systhread_mutex_lock        (&x->arraysMutex);
            err |= pizGrowingArrayCopy  (x->origin, x->selected);
            systhread_mutex_unlock      (&x->arraysMutex);
            
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
            
        if (VIEWTEXT && tralala_hasSelectedText (x, &selectedText)) {
            long k = (long)(ABS (pt.y - x->previousPoint.y));
            
            if ((pt.y - x->previousPoint.y) < -0.5) {
                switch (selectedText) {
                case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, k); break;
                case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, 1); break;
                case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, 1);  break;
                case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, 1);    break;
                }
            } else if ((pt.y - x->previousPoint.y) > 0.5) {
                switch (selectedText) {
                case TEXT_ORDER_VELOCITY : pizSequenceChangeMarkedNoteValue (x->user, PIZ_VELOCITY, -k); break;
                case TEXT_ORDER_DURATION : pizSequenceChangeMarkedNoteValue (x->user, PIZ_DURATION, -1); break;
                case TEXT_ORDER_CHANNEL  : pizSequenceChangeMarkedNoteValue (x->user, PIZ_CHANNEL, -1);  break;
                case TEXT_ORDER_PITCH    : pizSequenceChangeMarkedNoteValue (x->user, PIZ_PITCH, -1);    break;
                }
            }
            
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        } else if (x->hitTest & HIT_LOCKED) {
            ;
        } else if ((x->hitTest == HIT_NOTE) && !err) {
            if (ALT && !CTRL && !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED)))
            {
                tralala_duplicateSelectedNotes (x);
                
                DIRTYLAYER_SET (DIRTY_NOTES);
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_COPYING);
            } else if (CTRL && !(x->flags & FLAG_HAVE_MOVED)) {
                if (ALT) {
                    x->textMode = MODE_TEXT_MOUSE_VELOCITY;
                    
                    if ((pt.y - x->previousPoint.y) < -0.5) {
                        if (tralala_changeSelectedNotesVelocity (x, UP)) {
                            x->flags |= FLAG_HAVE_CHANGED;
                            DIRTYLAYER_SET (DIRTY_NOTES);
                        }
                    } else if ((pt.y - x->previousPoint.y) > 0.5) {
                        if (tralala_changeSelectedNotesVelocity (x, DOWN)) {
                            x->flags |= FLAG_HAVE_CHANGED;
                            DIRTYLAYER_SET (DIRTY_NOTES);
                        }
                    }
                } else if (tralala_changeSelectedNotesDuration (x, deltaPosition)) {
                    tralala_testAutoscroll (x, patcherview, pt);
                        
                    x->flags |= FLAG_HAVE_CHANGED;
                    DIRTYLAYER_SET (DIRTY_NOTES);
                }
            } else {
                if (tralala_moveSelectedNotes (x, deltaPosition, deltaPitch)) {
                    x->flags |= FLAG_HAVE_MOVED;
                    DIRTYLAYER_SET (DIRTY_NOTES);
                }
                
                if (CMD) {
                    x->textMode         = MODE_TEXT_MOUSE_PITCH;
                    x->mousePitchValue  = x->coordinates.pitch;
                    x->dirtyLayer       |= DIRTY_TEXT;
                } else {
                    if (x->textMode != MODE_TEXT_NOTE) {
                        x->textMode = MODE_TEXT_NOTE;
                        DIRTYLAYER_SET (DIRTY_TEXT);
                    }
                }
                
                tralala_testAutoscroll (x, patcherview, pt);
            }
        } else if (x->hitTest == HIT_ZONE) {
            bool draw = pizSequenceMoveTempZone (x->user, deltaPitch, deltaPosition);
                
            tralala_testAutoscroll (x, patcherview, pt);
                
            if (draw) {
                DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_LOAD);
            }
        } else if (x->hitTest) {
            PIZCoordinates  c1;
            PIZCoordinates  c2;
            double          f = 1.;
            bool            draw = false;
                                
            switch (x->zoomMode) {
                case MODE_ZOOM_A    : f = 0.5;  break;
                case MODE_ZOOM_B    : f = 1.;   break;
                case MODE_ZOOM_C    : f = 2.;   break;
            }

            c1.position = (long)((x->windowOffsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
            c2.position = (long)((x->windowOffsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
            
            c1.pitch = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y 
                - (GUI_PIXELS_PER_SEMITONE / 2. * f)) / (GUI_PIXELS_PER_SEMITONE * f))), 0);
            c2.pitch = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y 
                + (GUI_PIXELS_PER_SEMITONE / 2. * f)) / (GUI_PIXELS_PER_SEMITONE * f))), 0);
                        
            switch (x->hitTest) {
                case HIT_START              : draw = pizSequenceResizeTempZone 
                                                (x->user, &x->coordinates, PIZ_DATA_START); break;
                case HIT_END                : draw = pizSequenceResizeTempZone
                                                (x->user, &x->coordinates, PIZ_DATA_END); break;
                case HIT_DOWN               : draw = pizSequenceResizeTempZone 
                                                (x->user, &c1, PIZ_DATA_DOWN); break;
                case HIT_UP                 : draw = pizSequenceResizeTempZone 
                                                (x->user, &c2, PIZ_DATA_UP); break;
                case (HIT_START | HIT_END)  : draw = pizSequenceResizeTempZone 
                                                (x->user, &x->coordinates, PIZ_DATA_END); break;
                }
        
            tralala_testAutoscroll (x, patcherview, pt);

            if (draw) {
                DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_LOAD);
            } 
        } else if (CAPS) {
            x->windowOffsetX -= pt.x - x->previousPoint.x;
            x->windowOffsetY -= pt.y - x->previousPoint.y;
                
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
                
            DIRTYLAYER_SET (DIRTY_TEXT);
        } else if (!CMD)  {
            x->flags |= FLAG_IS_LASSO;
            DIRTYLAYER_SET (DIRTY_TEXT);
            
            if (x->flags & FLAG_ORIGIN_HAD_SHIFT_KEY) {
                if (pizSequenceDragLasso (x->user, &x->originCoordinates, &x->coordinates, INVERT)) {
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
                }
            } else if 
                (pizSequenceDragLasso (x->user, &x->originCoordinates, &x->coordinates, SELECT)) {
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
            }
            
            tralala_testAutoscroll (x, patcherview, pt);
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
        }
        
        x->previousPoint.x = pt.x;
        x->previousPoint.y = pt.y;
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
            DIRTYLAYER_SET (DIRTY_TEXT);
            
            DIRTYSLOTS 
            DIRTYPATTR
        }

        if (x->flags & FLAG_ZONE_IS_SELECTED) {
            pizSequencePutTempZone (x->user);
            x->flags &= ~FLAG_ZONE_IS_SELECTED;
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_LOAD);
            
            DIRTYSLOTS 
            DIRTYPATTR
        }
        
        if (x->flags & (FLAG_HAVE_CHANGED | FLAG_HAVE_MOVED | FLAG_HAVE_BEEN_DUPLICATED)) {
            systhread_mutex_lock            (&x->arraysMutex);
            pizSequenceRemoveSelectedNotes  (x->user);
            pizSequenceAddNotesWithArray    (x->user, x->selected, PIZ_ADD_FLAG_NONE);
            systhread_mutex_unlock          (&x->arraysMutex);
            
            x->textMode     = MODE_TEXT_NOTE;
            x->flags        &= ~(FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED);
            x->dirtyLayer   |= (DIRTY_NOTES | DIRTY_LOAD);
            
            DIRTYSLOTS 
            DIRTYPATTR
        }
        
        if (x->flags & FLAG_IS_LASSO) {
            x->flags &= ~FLAG_IS_LASSO;
            
            pizSequenceInitLasso (x->user);
            
            DIRTYLAYER_SET (DIRTY_TEXT);
            
            DIRTYSLOTS 
            DIRTYPATTR
        }

        x->flags &= ~FLAG_ORIGIN_IS_SET;
        
        tralala_stopAutoscroll (x);
    }
    
    if (!CMD && (x->hitTest != HIT_ZONE) && tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW)) {
        if (!CAPS) {
            x->textMode = MODE_TEXT_NOTE;
            DIRTYLAYER_SET (DIRTY_TEXT);
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
    
    if (x->textMode != MODE_TEXT_NOTE) {
        x->textMode = MODE_TEXT_NOTE;
        DIRTYLAYER_SET (DIRTY_TEXT);
    }
}

void tralala_mousemove (t_tralala *x, t_object *patcherview, t_pt pt, long modifiers)
{   
    if (x->flags & FLAG_FOCUS) {
        tralala_setCoordinatesWithPoint (x, &x->coordinates, pt);
        
        if (CMD && !CAPS) {   
            if (USER) {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_CROSSHAIR);
            } else {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
            }
            
            x->textMode = MODE_TEXT_MOUSE_PITCH;
            x->mousePitchValue = x->coordinates.pitch;
            DIRTYLAYER_SET (DIRTY_TEXT);
        } else if (USER && CAPS) {
            long isHit = tralala_hitZoneWithPoint (x, pt);
            
            if (isHit == HIT_ZONE)  {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_DRAGGINGHAND);
            } else if (isHit) {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_POINTINGHAND);
            } else {
                tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
            }
            
            if (x->textMode != MODE_TEXT_ZONE) {
                x->textMode = MODE_TEXT_ZONE;
                DIRTYLAYER_SET (DIRTY_TEXT);
            }
        } else {
            tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
                        
            if (x->textMode != MODE_TEXT_NOTE)  {
                x->textMode = MODE_TEXT_NOTE;
                DIRTYLAYER_SET (DIRTY_TEXT);
            }
        }
    }
}

void tralala_mousewheel (t_tralala *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc)
{
    if (ALT) {
        t_rect  rect;
        long    k = 0;
        
        jbox_get_rect_for_view ((t_object *)x, view, &rect);    

        if (y_inc > 0.) {
            switch (x->zoomMode) {
                case MODE_ZOOM_A :  x->zoomMode = MODE_ZOOM_B; k = 1; break;
                case MODE_ZOOM_B :  x->zoomMode = MODE_ZOOM_C; k = 1; break;
            }
        } else {
            switch (x->zoomMode) {
                case MODE_ZOOM_C :  x->zoomMode = MODE_ZOOM_B; k = 2; break;
                case MODE_ZOOM_B :  x->zoomMode = MODE_ZOOM_A; k = 2; break;
            }
        }
        
        if (k == 1) {
            x->windowOffsetX = ((2. * x->windowOffsetX) + rect.width) - (rect.width  / 2.); 
            x->windowOffsetY = ((2. * x->windowOffsetY) + rect.height) - (rect.height  / 2.); 
        } else if (k == 2) {
            x->windowOffsetX = (((2. * x->windowOffsetX) + rect.width) / 4.) - (rect.width / 2.);
            x->windowOffsetY = (((2. * x->windowOffsetY) + rect.height) / 4.) - (rect.height / 2.);
        }
        
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES);
    } else {
        x->windowOffsetX -= x_inc * GUI_MOUSEWHEEL_FACTOR;
        x->windowOffsetY -= y_inc * GUI_MOUSEWHEEL_FACTOR;
            
        DIRTYLAYER_SET (DIRTY_TEXT);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
                                    
void tralala_key (t_tralala *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{
    if (SHARP || (keycode == JKEY_SPACEBAR)) {
        switch (x->sequenceMode) {
        case MODE_SEQUENCE_USER   : object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_LIVE); break;
        case MODE_SEQUENCE_LIVE   : object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_USER); break;
        }
    } else if (keycode == JKEY_UPARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
        pizSequenceTranspose (x->user, PIZ_MAGIC_SCALE);
        
        DIRTYSLOTS 
        DIRTYPATTR
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_LOAD);
    } else if (keycode == JKEY_DOWNARROW && USER && !(x->flags & FLAG_ZONE_IS_SELECTED)) {
        pizSequenceTranspose (x->user, -PIZ_MAGIC_SCALE);
        
        DIRTYSLOTS 
        DIRTYPATTR
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_LOAD);
    } else if (keycode == JKEY_ENTER) {
        tralala_setLiveByUser (x);
        
        if (LIVE) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_LOAD);
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
        
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        DIRTYSLOTS 
        DIRTYPATTR
    } else if (USER && CMD && !(x->flags & (FLAG_HAVE_MOVED | FLAG_HAVE_CHANGED | FLAG_HAVE_BEEN_DUPLICATED))) {
        if (ALL) {
            pizSequenceSelectAllNotes (x->user);
                
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        } else if (COPY)  {
            pizGrowingArrayClear (tll_clipboard);
            tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
                
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        } else if (CUT)  {
            pizGrowingArrayClear (tll_clipboard);
            tll_clipboardError = pizSequenceNotesToArray (x->user, NULL, tll_clipboard);
            pizSequenceRemoveSelectedNotes (x->user);
                
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
        } else if (PASTE) {
            if (!tll_clipboardError) {
                long count;
                
                if (count = (pizGrowingArrayCount (tll_clipboard) / PIZ_DATA_NOTE_SIZE)) {
                    long i;
                    long offsetPosition, offsetPitch;
                                                
                    pizSequenceUnselectAllNotes (x->user);
                    
                    if (pizGrowingArrayValueAtIndex (tll_clipboard, PIZ_DATA_POSITION) 
                        < (PIZ_SEQUENCE_TIMELINE_SIZE / 2)) {
                        offsetPosition = pizSequenceGrid (x->user);
                    } else {
                        offsetPosition = -(pizSequenceGrid (x->user));
                    }
                        
                    if (pizGrowingArrayValueAtIndex (tll_clipboard, PIZ_DATA_PITCH) 
                        < (PIZ_MAGIC_PITCH / 2)) {
                        offsetPitch = 1;
                    } else {
                        offsetPitch = -1;
                    }
                    
                    for (i = 0; i < count; i++) {
                        long position = pizGrowingArrayValueAtIndex (tll_clipboard, (PIZ_DATA_NOTE_SIZE * i)
                            + PIZ_DATA_POSITION);
                        long pitch = pizGrowingArrayValueAtIndex (tll_clipboard, (PIZ_DATA_NOTE_SIZE * i) 
                            + PIZ_DATA_PITCH);
                            
                        position    += offsetPosition;
                        pitch       += offsetPitch;
                        
                        pizGrowingArraySetValueAtIndex (tll_clipboard, (PIZ_DATA_NOTE_SIZE * i) 
                            + PIZ_DATA_POSITION, position);
                        pizGrowingArraySetValueAtIndex (tll_clipboard, (PIZ_DATA_NOTE_SIZE * i) 
                            + PIZ_DATA_PITCH, pitch);
                        }
                    
                    pizSequenceAddNotesWithArray (x->user, tll_clipboard, PIZ_ADD_FLAG_SNAP);
                        
                    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_LOAD);
                }
            }
        }
        
        if (ALL || COPY || CUT || PASTE) {
            DIRTYSLOTS
            DIRTYPATTR
        }
            
    } else if (keycode >= 49 && keycode <= 55) {
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            if (!SHIFT) {
                switch (keycode) {
                    case 55 : pizSequenceSetGrid (x->user, PIZ_NOTE_NONE);          break;
                    case 54 : pizSequenceSetGrid (x->user, PIZ_WHOLE_NOTE);         break;
                    case 53 : pizSequenceSetGrid (x->user, PIZ_HALF_NOTE);          break;
                    case 52 : pizSequenceSetGrid (x->user, PIZ_QUARTER_NOTE);       break;
                    case 51 : pizSequenceSetGrid (x->user, PIZ_EIGHTH_NOTE);        break;
                    case 50 : pizSequenceSetGrid (x->user, PIZ_SIXTEENTH_NOTE);     break;
                    case 49 : pizSequenceSetGrid (x->user, PIZ_THIRTY_SECOND_NOTE); break;
                    }
                
                DIRTYLAYER_SET (DIRTY_GRID);
            } else {
                switch (keycode) {
                    case 55 : pizSequenceSetNoteValue (x->user, PIZ_NOTE_NONE);         break;
                    case 54 : pizSequenceSetNoteValue (x->user, PIZ_WHOLE_NOTE);        break;
                    case 53 : pizSequenceSetNoteValue (x->user, PIZ_HALF_NOTE);         break;
                    case 52 : pizSequenceSetNoteValue (x->user, PIZ_QUARTER_NOTE);      break;
                    case 51 : pizSequenceSetNoteValue (x->user, PIZ_EIGHTH_NOTE);       break;
                    case 50 : pizSequenceSetNoteValue (x->user, PIZ_SIXTEENTH_NOTE);    break;
                    case 49 : pizSequenceSetNoteValue (x->user, PIZ_THIRTY_SECOND_NOTE);break;
                    }
                
                DIRTYLAYER_SET (DIRTY_TEXT);
            }
        } ATOMIC_DECREMENT (&x->popupLock);
    } else if (TERNARY) {
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            
            PIZNoteValue old, new = PIZ_NOTE_NONE;
            
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
                case PIZ_NOTE_NONE                  :   new = PIZ_NOTE_NONE;                    break;
            }
            
            if (!SHIFT) {
                pizSequenceSetGrid (x->user, new);
                DIRTYLAYER_SET (DIRTY_GRID);
            } else {
                pizSequenceSetNoteValue (x->user, new);
                DIRTYLAYER_SET (DIRTY_TEXT);
            }
        } ATOMIC_DECREMENT (&x->popupLock);
    } else if (DOTTED) {
        if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
            PIZNoteValue old, new = PIZ_NOTE_NONE;
            
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
                case PIZ_NOTE_NONE                  :   new = PIZ_NOTE_NONE;                    break;
            }
            
            if (!SHIFT) {
                pizSequenceSetGrid (x->user, new);
                DIRTYLAYER_SET (DIRTY_GRID);
            } else {
                pizSequenceSetNoteValue (x->user, new);
                DIRTYLAYER_SET (DIRTY_TEXT);
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
    PIZNoteValue    snap;

    jmouse_getposition_global (&mouseX, &mouseY);
    
    locate.x = (double)mouseX;
    locate.y = (double)mouseY;
    
    font = jfont_create (x->popupFontName->s_name, (x->popupFontFace & (1<<1)), (x->popupFontFace & (1<<0)), 
        x->popupFontSize); 
    
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
                                NULL, (snap == PIZ_NOTE_NONE), 0, NULL);
                                                    
        if (menuMode == MODE_MENU_SEQUENCE) {
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
            jpopupmenu_addseperator (popup);
    
            if (USER) {
                long            i;
                PIZNoteValue    value = pizSequenceNoteValue (x->user);
                
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
                                        NULL, (value == PIZ_NOTE_NONE), 0, NULL);
                        
                jpopupmenu_additem      (slotsPopup, 1000, "New", NULL, 0, 0, NULL);
                jpopupmenu_additem      (slotsPopup, 1001, "Clone", NULL, 0, 0, NULL);
                jpopupmenu_addseperator (slotsPopup);
                
                for (i = 0; i < pizLinklistCount (x->slots); i++) {
                    char text[SIZE_STRING_MAX];
                    snprintf (text, SIZE_STRING_MAX, "Slot %ld", i);
                    text[SIZE_STRING_MAX - 1] = 0;
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
        } else if (menuMode == MODE_MENU_NOTE) {
            long count;
            long velocity = -1;
            long channel = -1;

            systhread_mutex_lock (&x->arraysMutex);
            
            count = pizGrowingArrayCount (x->selected) / PIZ_DATA_NOTE_SIZE;
            
            if (count == 1) {
                velocity = pizGrowingArrayValueAtIndex (x->selected, PIZ_DATA_VELOCITY);
                channel = pizGrowingArrayValueAtIndex (x->selected, PIZ_DATA_CHANNEL);
                
                velocity = CLAMP ((long)((velocity + 4) / 8) * 8, 0, 127);
            }

            systhread_mutex_unlock (&x->arraysMutex);
            
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
        case 10     :   pizSequenceSetGrid  (x->user, PIZ_NOTE_NONE);                    break;
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
        case 100    :   object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_USER);      break;
        case 101    :   object_attr_setlong (x, tll_sym_sequenceMode, MODE_SEQUENCE_LIVE);      break;
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
        case 300    :   pizSequenceSetNoteValue (x->user, PIZ_NOTE_NONE);                   break;
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

    if (x->flags & FLAG_HAVE_CHANGED) {
        systhread_mutex_lock            (&x->arraysMutex);
        pizSequenceRemoveSelectedNotes  (x->user);
        pizSequenceAddNotesWithArray    (x->user, x->selected, PIZ_ADD_FLAG_NONE);
        systhread_mutex_unlock          (&x->arraysMutex);
            
        x->flags &= ~FLAG_HAVE_CHANGED;
            
        DIRTYLAYER_SET (DIRTY_NOTES);
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
// -----------------------------------------------------------------------------------------------------------:x
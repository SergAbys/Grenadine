/*
 *  tralalaGUI.c
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

extern t_jsurface           *tll_thirtySecond       [3];
extern t_jsurface           *tll_sixteenth          [3];
extern t_jsurface           *tll_sixteenthTriplet   [3];
extern t_jsurface           *tll_sixteenthDotted    [3];
extern t_jsurface           *tll_eighth             [3];
extern t_jsurface           *tll_eighthTriplet      [3];
extern t_jsurface           *tll_eighthDotted       [3];
extern t_jsurface           *tll_quarter            [3];
extern t_jsurface           *tll_quarterTriplet     [3];
extern t_jsurface           *tll_quarterDotted      [3];
extern t_jsurface           *tll_half               [3];
extern t_jsurface           *tll_halfTriplet        [3];
extern t_jsurface           *tll_halfDotted         [3];
extern t_jsurface           *tll_whole              [3];

extern PIZGrowingArray      *tll_clipboard;

extern tralalaSymbolsTableA tll_symbolsA;
extern tralalaSymbolsTableB tll_symbolsB;

static PIZError tll_clipboardError = PIZ_GOOD;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

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
            if (!pizSequenceAddNoteWithCoordinates (x->user, &x->coordinates, PIZ_SEQUENCE_ADD_MODE_SNAP)) {
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
                    
                    systhread_mutex_lock        (&x->paintMutex);
                    err |= pizGrowingArrayCopy  (x->originNotes, x->selectedNotes);
                    systhread_mutex_unlock      (&x->paintMutex);
                    
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
                    
                    c1.pitch = PIZ_SEQUENCE_MIDI_NOTE - ((long)floor ((x->windowOffsetY + pt.y - 
                                (SEMITONE_PIXELS_SIZE / 2. * f)) / (SEMITONE_PIXELS_SIZE * f)));
                    c2.pitch = PIZ_SEQUENCE_MIDI_NOTE - ((long)floor ((x->windowOffsetY + pt.y + 
                                (SEMITONE_PIXELS_SIZE / 2. * f)) / (SEMITONE_PIXELS_SIZE * f)));
                                
                    switch (x->hitTest) {
                    case HIT_START: draw = pizSequenceSetTempZoneStartWithCoordinates 
                                      (x->user, &x->coordinates); break;
                    case HIT_END  : draw = pizSequenceSetTempZoneEndWithCoordinates
                                      (x->user, &x->coordinates); break;
                    case HIT_DOWN : draw = pizSequenceSetTempZoneDownWithCoordinates (x->user, &c1); break;
                    case HIT_UP   : draw = pizSequenceSetTempZoneUpWithCoordinates (x->user, &c2); break;
                    case (HIT_START | HIT_END)  
                                  : draw = pizSequenceSetTempZoneEndWithCoordinates (x->user, &x->coordinates);
                                      break;
                    }
                
                    tralala_testAutoscroll (x, patcherview, pt);

                    if (draw) {
                            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_CHANGE);
                        } 
                }
            else if (CAPS)
                {
                    x->windowOffsetX = round (x->windowOffsetX - (pt.x - x->previous.x));
                    x->windowOffsetY = round (x->windowOffsetY - (pt.y - x->previous.y));
                    
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
                    systhread_mutex_lock            (&x->paintMutex);
                    pizSequenceRemoveSelectedNotes  (x->user);
                    pizSequenceAddNotesWithArray    (x->user, x->selectedNotes, PIZ_SEQUENCE_ADD_MODE_NONE);
                    systhread_mutex_unlock          (&x->paintMutex);
                    
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
                    
                    x->windowOffsetX = round (x->windowOffsetX);
                    x->windowOffsetY = round (x->windowOffsetY);
                }
            else if (k == 2)
                {
                    x->windowOffsetX = (((2. * x->windowOffsetX) + rect.width) / 4.) - (rect.width / 2.);
                    x->windowOffsetY = (((2. * x->windowOffsetY) + rect.height) / 4.) - (rect.height / 2.);
                    
                    x->windowOffsetX = round (x->windowOffsetX);
                    x->windowOffsetY = round (x->windowOffsetY);
                }
            
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES);
        }
    else
        {
            x->windowOffsetX = round (x->windowOffsetX - (x_inc * MOUSEWHEEL_FACTOR));
            x->windowOffsetY = round (x->windowOffsetY - (y_inc * MOUSEWHEEL_FACTOR));
            
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
                                    < (PIZ_SEQUENCE_MIDI_NOTE / 2)) {
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
                                    (x->user, tll_clipboard, PIZ_SEQUENCE_ADD_MODE_SNAP);
                                    
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

void tralala_paint (t_tralala *x, t_object *patcherview)
{   
    PIZError err = PIZ_GOOD;
    
    if (!(x->flags & FLAG_INIT_PAINT_CLOCK)) {
        clock_fdelay (x->paintClock, PAINT_CLOCK_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
        x->flags |= FLAG_INIT_PAINT_CLOCK;
    }
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) 
        {
            systhread_mutex_lock (&x->paintMutex);
            
            err |= pizGrowingArrayCopy (x->unselectedNotesCopy, x->unselectedNotes);
            err |= pizGrowingArrayCopy (x->selectedNotesCopy, x->selectedNotes);
            err |= pizGrowingArrayCopy (x->playedNotesCopy, x->playedNotes);
            err |= pizGrowingArrayCopy (x->zoneCopy, x->zone);
            
            systhread_mutex_unlock  (&x->paintMutex);
            
            tralala_paintGrid (x, patcherview);
            
            if (!err && !LISTEN) {
                    tralala_paintZone (x, patcherview);
                }
            
            if (!err) {
                    tralala_paintNotes (x, patcherview);
                }
            
            if (!err && LIVE && !(x->flags & FLAG_IS_MUTED)) {
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

void tralala_paintTask (t_tralala *x) 
{   
    ulong dirty = x->dirtyLayer;
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) {
    
    DIRTYLAYER_UNSET (~(DIRTY_GRID | DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE | DIRTY_REFRESH | DIRTY_PLAYED));
                                
    if (LIVE && !(dirty & DIRTY_CHANGE) && ((x->flags & FLAG_IS_RUNNING) || (x->runIndex == -1)))
        {
            systhread_mutex_lock (&x->paintMutex);
            
            if (tralala_hitNotesByRunIndex (x)) {           
                    dirty |= DIRTY_PLAYED;
                }
            
            systhread_mutex_unlock (&x->paintMutex);
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
                    PIZError err = PIZ_GOOD;
                    
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
                                    systhread_mutex_lock (&x->paintMutex);
                                    
                                    pizGrowingArrayClear (x->zone);
                                    
                                    if (x->flags & FLAG_ZONE_IS_SELECTED)
                                        {
                                            err = pizSequenceTempZoneToArray (sequence, x->zone);
                                        }
                                    else
                                        {
                                            err = pizSequenceZoneToArray (sequence, x->zone);
                                        }
                                            
                                    systhread_mutex_unlock (&x->paintMutex);
                                }
                        }
                    
                    if (!err) {
                            jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_zoneLayer);
                        }
                }
                
            if (dirty & DIRTY_NOTES)
                {
                    PIZError err = PIZ_GOOD;
                    
                    if (dirty & DIRTY_CHANGE) {
                        systhread_mutex_lock (&x->paintMutex);
                        
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
                        
                        systhread_mutex_unlock (&x->paintMutex);
                    }
                                                    
                    if (!err) {
                            jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_notesLayer);
                        }
                }
            
            if (dirty & DIRTY_PLAYED) {
                    jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_playedNotesLayer);
                }
                
            jbox_redraw ((t_jbox *)x);
        }
        
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

bool tralala_moveSelectedNotes (t_tralala *x, long deltaPosition, long deltaPitch)
{
    long i;
    long count;
    bool moved = false;
    long grid = pizSequenceGrid (x->user);
        
    systhread_mutex_lock (&x->paintMutex);

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
            
            position = (long)(round (position / (double)grid)) * grid;
            
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_POSITION,
                                            CLAMP (position, 0, PIZ_SEQUENCE_TIMELINE_SIZE - duration));
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_PITCH,
                                            CLAMP (pitch, 0, PIZ_SEQUENCE_MIDI_NOTE));
            
            if ((previousPosition != position) || (previousPitch != pitch))
                {
                    moved = true;
                }
        }
    
    systhread_mutex_unlock (&x->paintMutex);
    
    return moved;
}

bool tralala_changeSelectedNotesDuration (t_tralala *x, long deltaPosition)
{
    long i;
    long count;
    bool changed = false;
    long grid = pizSequenceGrid (x->user);
                
    systhread_mutex_lock (&x->paintMutex);

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
            duration = (long)(round (duration / (double)grid)) * grid;
            
            pizGrowingArraySetValueAtIndex (x->selectedNotes, 
                                            (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_DURATION,
                                            CLAMP (duration, grid, maximum));
            
            if (duration != previousDuration)
                {
                    changed = true;
                }
        }
    
    systhread_mutex_unlock (&x->paintMutex);
    
    return changed;
}

void tralala_duplicateSelectedNotes (t_tralala *x)
{
    systhread_mutex_lock        (&x->paintMutex);
    pizGrowingArrayAppendArray  (x->unselectedNotes, x->selectedNotes);
    systhread_mutex_unlock      (&x->paintMutex);
            
    pizSequenceUnselectAllNotes (x->user);
            
    x->flags |= FLAG_HAVE_BEEN_DUPLICATED;
}

void tralala_changeSelectedNotesVelocity (t_tralala *x, bool decrement)
{
    long count;
    
    systhread_mutex_lock (&x->paintMutex);
    
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
                            temp = PIZ_SEQUENCE_MIDI_VELOCITY - (pizGrowingArrayValueAtIndex (x->selectedNotes,
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
    
    systhread_mutex_unlock (&x->paintMutex);
}

void tralala_setSelectedNotesVelocity (t_tralala *x, long velocity)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->paintMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY,
            CLAMP (velocity, 0, PIZ_SEQUENCE_MIDI_VELOCITY));
    }
    
    systhread_mutex_unlock (&x->paintMutex);
}

void tralala_setSelectedNotesChannel (t_tralala *x, long channel)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->paintMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_CHANNEL,
            CLAMP (channel, 0, PIZ_SEQUENCE_MIDI_CHANNEL));
    }
    
    systhread_mutex_unlock (&x->paintMutex);
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

    systhread_mutex_lock (&x->paintMutex);

    start   = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_START);
    end     = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_END);
    down    = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_DOWN);
    up      = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_UP);

    systhread_mutex_unlock  (&x->paintMutex);
                        
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
                            err |= pizGrowingArrayAppend (x->playedNotes, PIZ_SEQUENCE_MIDI_VELOCITY);
                            err |= pizGrowingArrayAppend (x->playedNotes, duration);
                            err |= pizGrowingArrayAppend (x->playedNotes, PIZ_SEQUENCE_NOTE_CHANNEL_NONE);
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
    coordinates->pitch      = PIZ_SEQUENCE_MIDI_NOTE - ((long)floor ((x->windowOffsetY + pt.y) / 
                                (SEMITONE_PIXELS_SIZE * f)));
    
    if ((coordinates->pitch < 0) || (coordinates->pitch > PIZ_SEQUENCE_MIDI_NOTE) || 
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
    y1 = (PIZ_SEQUENCE_MIDI_NOTE - up) * SEMITONE_PIXELS_SIZE * f;
    y2 = ((PIZ_SEQUENCE_MIDI_NOTE + 1) - down) * SEMITONE_PIXELS_SIZE * f;
    
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
    y1 = ((PIZ_SEQUENCE_MIDI_NOTE - c->pitch) * SEMITONE_PIXELS_SIZE * f);
    y2 = y1 + (SEMITONE_PIXELS_SIZE * f);
    
    noteRect->x         = x1;
    noteRect->y         = y1;
    noteRect->width     = x2 - x1;
    noteRect->height    = y2 - y1;
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
                        jpopupmenu_additem      (slotsPopup, 1001, "New Copy", NULL, 0, 0, NULL);
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
        else if (menuMode == MENU_NOTE) {
                jpopupmenu_additem (velocityPopup, 50, "0", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 51, "8", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 52, "16", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 53, "24", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 54, "32", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 55, "40", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 56, "48", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 57, "56", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 58, "64", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 59, "72", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 60, "80", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 61, "88", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 62, "96", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 63, "104", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 64, "112", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 65, "120", NULL, 0, 0, NULL);
                jpopupmenu_additem (velocityPopup, 66, "127", NULL, 0, 0, NULL);
        
                jpopupmenu_additem      (noteChannelPopup, 200, "0", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 201, "1", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 202, "2", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 203, "3", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 204, "4", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 205, "5", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 206, "6", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 207, "7", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 208, "8", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 209, "9", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 210, "10", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 211, "11", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 212, "12", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 213, "13", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 214, "14", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 215, "15", NULL, 0, 0, NULL);
                jpopupmenu_additem      (noteChannelPopup, 216, "16", NULL, 0, 0, NULL);

                jpopupmenu_addsubmenu   (popup, "Snap", snapPopup, 0);
                jpopupmenu_addseperator (popup);
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
            systhread_mutex_lock            (&x->paintMutex);
            pizSequenceRemoveSelectedNotes  (x->user);
            pizSequenceAddNotesWithArray    (x->user, x->selectedNotes, PIZ_SEQUENCE_ADD_MODE_NONE);
            systhread_mutex_unlock          (&x->paintMutex);
                
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
        * STEP_PIXELS_SIZE * f, (PIZ_SEQUENCE_MIDI_NOTE + 1) * SEMITONE_PIXELS_SIZE * f);

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
            gridHeight  = (PIZ_SEQUENCE_MIDI_NOTE + 1) * SEMITONE_PIXELS_SIZE * f;
            
            srcRect.x       = 0.;
            srcRect.y       = 0.;
            srcRect.width   = imageWidth;
            srcRect.height  = imageHeight;
            
            for (i = 0; i < ((PIZ_SEQUENCE_MIDI_NOTE + 1) / JSURFACE_MOSAIC_PITCH_SIZE); i ++)
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
        
            jrgba_set (&gridColor, 0.85, 0.85, 0.85, 1.);
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
        * STEP_PIXELS_SIZE * f, (PIZ_SEQUENCE_MIDI_NOTE + 1) * SEMITONE_PIXELS_SIZE * f);
    
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
        * STEP_PIXELS_SIZE * f, (PIZ_SEQUENCE_MIDI_NOTE + 1) * SEMITONE_PIXELS_SIZE * f);
    
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
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, 
        -x->windowOffsetX, -x->windowOffsetY);
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

    alpha = (double)(velocity + VELOCITY_PAINT_OFFSET) / (double)PIZ_SEQUENCE_MIDI_VELOCITY;
    
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
    
    alpha = (double)velocity / (double)PIZ_SEQUENCE_MIDI_VELOCITY;
    
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
        * STEP_PIXELS_SIZE * f, (PIZ_SEQUENCE_MIDI_NOTE + 1) * SEMITONE_PIXELS_SIZE * f);
    
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
// -----------------------------------------------------------------------------------------------------------:x
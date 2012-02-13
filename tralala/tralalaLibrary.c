/*
 *  tralalaLibrary.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 13/02/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern tralalaSymbolsTableB tll_symbolsB;

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
        
    systhread_mutex_lock (&x->arraysMutex);

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
    
    systhread_mutex_unlock (&x->arraysMutex);
    
    return moved;
}

bool tralala_changeSelectedNotesDuration (t_tralala *x, long deltaPosition)
{
    long i;
    long count;
    bool changed = false;
    long grid = pizSequenceGrid (x->user);
                
    systhread_mutex_lock (&x->arraysMutex);

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
    
    systhread_mutex_unlock (&x->arraysMutex);
    
    return changed;
}

void tralala_duplicateSelectedNotes (t_tralala *x)
{
    systhread_mutex_lock        (&x->arraysMutex);
    pizGrowingArrayAppendArray  (x->unselectedNotes, x->selectedNotes);
    systhread_mutex_unlock      (&x->arraysMutex);
            
    pizSequenceUnselectAllNotes (x->user);
            
    x->flags |= FLAG_HAVE_BEEN_DUPLICATED;
}

void tralala_changeSelectedNotesVelocity (t_tralala *x, bool decrement)
{
    long count;
    
    systhread_mutex_lock (&x->arraysMutex);
    
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
    
    systhread_mutex_unlock (&x->arraysMutex);
}

void tralala_setSelectedNotesVelocity (t_tralala *x, long velocity)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->arraysMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_VELOCITY,
            CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY));
    }
    
    systhread_mutex_unlock (&x->arraysMutex);
}

void tralala_setSelectedNotesChannel (t_tralala *x, long channel)
{
    long i;
    long count;
    
    systhread_mutex_lock (&x->arraysMutex);

    count = pizGrowingArrayCount (x->selectedNotes) / PIZ_SEQUENCE_NOTE_SIZE;
    
    for (i = 0; i < count; i++) {
        pizGrowingArraySetValueAtIndex (x->selectedNotes, (PIZ_SEQUENCE_NOTE_SIZE * i) + PIZ_SEQUENCE_CHANNEL,
            CLAMP (channel, 0, PIZ_MAGIC_CHANNEL));
    }
    
    systhread_mutex_unlock (&x->arraysMutex);
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

    systhread_mutex_lock (&x->arraysMutex);

    start   = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_START);
    end     = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_END);
    down    = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_DOWN);
    up      = pizGrowingArrayValueAtIndex (x->zone, PIZ_SEQUENCE_UP);

    systhread_mutex_unlock  (&x->arraysMutex);
                        
    tralala_setRectWithZoneValues (x, &zoneRect, start, end, down, up);
    
    zoneRect.x -= x->windowOffsetX;
    zoneRect.y -= x->windowOffsetY;
    
    a = ((ABS (pt.x - zoneRect.x)) < GUI_HIT_ZONE_RANGE);
    b = ((ABS (pt.x - (zoneRect.x + zoneRect.width))) < GUI_HIT_ZONE_RANGE);
    c = ((pt.x > (zoneRect.x + (GUI_HIT_ZONE_RANGE / 2.))) 
            && (pt.x < (zoneRect.x + zoneRect.width - (GUI_HIT_ZONE_RANGE / 2.))));
    
    d = ((ABS (pt.y - (zoneRect.y + zoneRect.height))) < GUI_HIT_ZONE_RANGE);
    e = ((ABS (pt.y - zoneRect.y)) < GUI_HIT_ZONE_RANGE);
    f = ((pt.y > (zoneRect.y + (GUI_HIT_ZONE_RANGE / 2.))) 
            && (pt.y < (zoneRect.y + zoneRect.height - (GUI_HIT_ZONE_RANGE / 2.))));
                        
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

    h1 = rect.height - (GUI_TEXT_SPACE + fontSize);
    h2 = h1 + textHeight;
    
    if ((pt.y > h1) && (pt.y < h2)) {
        if (pizSequenceHasMarkedNote (x->user)) {
            if ((pt.x > 0) && (pt.x < (x->textPosition[TEXT_CELL_COUNT - 1] 
                + x->textWidth[TEXT_CELL_COUNT - 1] + GUI_TEXT_SPACE))) {
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
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
        }
    
    coordinates->position   = (long)((x->windowOffsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
    coordinates->pitch      = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y) / 
                                (GUI_PIXELS_PER_SEMITONE * f))), 0);
    
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
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
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
    
    x1 = start * GUI_PIXELS_PER_STEP  * f;
    x2 = end * GUI_PIXELS_PER_STEP  * f;
    y1 = (PIZ_MAGIC_PITCH - up) * GUI_PIXELS_PER_SEMITONE * f;
    y2 = ((PIZ_MAGIC_PITCH + 1) - down) * GUI_PIXELS_PER_SEMITONE * f;
    
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
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
        }
    
    x1 = (c->position * GUI_PIXELS_PER_STEP  * f);
    x2 = x1 + (d * GUI_PIXELS_PER_STEP * f);
    y1 = ((PIZ_MAGIC_PITCH - c->pitch) * GUI_PIXELS_PER_SEMITONE * f);
    y2 = y1 + (GUI_PIXELS_PER_SEMITONE * f);
    
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
    if (formatMode == MODE_FORMAT_LONG)
        {
            snprintf (string, SIZE_STRING_MAX, "%ld", longToBeFormatted);
        }
    else if (formatMode == MODE_FORMAT_NOTENAME)
        {
            long m, n;
            
            m = longToBeFormatted % 12;
            n = (long)(longToBeFormatted / 12.) - 2;
                        
            switch (m) {
                case 0  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "C", n);  break;
                case 1  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "C#", n); break;
                case 2  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "D", n);  break;
                case 3  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "D#", n); break;
                case 4  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "E", n);  break;
                case 5  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "F", n);  break;
                case 6  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "F#", n); break;
                case 7  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "G", n);  break;
                case 8  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "G#", n); break;
                case 9  : snprintf (string, SIZE_STRING_MAX, "%s%ld", "A", n);  break;
                case 10 : snprintf (string, SIZE_STRING_MAX, "%s%ld", "A#", n); break;
                case 11 : snprintf (string, SIZE_STRING_MAX, "%s%ld", "B", n);  break;
                }
        }
    else if (formatMode == MODE_FORMAT_TICKS)
        {
            snprintf (string, SIZE_STRING_MAX, "%ld", longToBeFormatted * TIME_TICKS_PER_STEP);
        }
    else if (formatMode == MODE_FORMAT_GRID) {
        switch (longToBeFormatted) {
        case PIZ_WHOLE_NOTE_DOTTED          :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_wholeDotted->s_name); break;
        case PIZ_WHOLE_NOTE                 :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_whole->s_name); break;
        case PIZ_WHOLE_NOTE_TRIPLET         :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_wholeTriplet->s_name); break;
        case PIZ_HALF_NOTE_DOTTED           :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_halfDotted->s_name); break;
        case PIZ_HALF_NOTE                  :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_half->s_name); break;
        case PIZ_HALF_NOTE_TRIPLET          :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_halfTriplet->s_name); break;
        case PIZ_QUARTER_NOTE_DOTTED        :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_quarterDotted->s_name); break;
        case PIZ_QUARTER_NOTE               :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_quarter->s_name); break;
        case PIZ_QUARTER_NOTE_TRIPLET       :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_quarterTriplet->s_name); break;
        case PIZ_EIGHTH_NOTE_DOTTED         :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_eighthDotted->s_name); break;
        case PIZ_EIGHTH_NOTE                    :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_eighth->s_name); break;
        case PIZ_EIGHTH_NOTE_TRIPLET            :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_eighthTriplet->s_name); break;
        case PIZ_SIXTEENTH_NOTE_DOTTED      :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_sixteenthDotted->s_name); break;
        case PIZ_SIXTEENTH_NOTE             :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_sixteenth->s_name); break;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_sixteenthTriplet->s_name); break;
        case PIZ_THIRTY_SECOND_NOTE         :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_thirtySecond->s_name); break;
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_thirtySecondTriplet->s_name); break;
        case PIZ_SNAP_NONE                  :
            snprintf (string, SIZE_STRING_MAX, "%s", tll_sym_none->s_name); break;
        }
    }
    
    string[SIZE_STRING_MAX - 1] = 0;
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
            x->textMode = MODE_TEXT_NOTE;
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
    
    if (pt.x < GUI_AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_LEFT);
            stop = false;
        }
        
    if ((rect.width - pt.x) < GUI_AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_RIGHT);
            stop = false;
        }
        
    if ((rect.height - pt.y) < GUI_AUTOSCROLL_RANGE) {
            DIRTYLAYER_SET (DIRTY_LOCATE_DOWN);
            stop = false;
        }
        
    if (pt.y < GUI_AUTOSCROLL_RANGE) {
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
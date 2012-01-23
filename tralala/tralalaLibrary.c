/*
 *  tralalaLibrary.c
 *
 *  Created : 25/06/11.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 23/01/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern tralalaSymbolsTableA     tll_symbolsA;
extern tralalaSymbolsTableB     tll_symbolsB;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

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
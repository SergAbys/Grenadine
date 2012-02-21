/*
 *  tralalaLibrary.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 21/02/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern tralalaSymbolsTableB tll_symbolsB;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

bool tralala_moveSelectedNotes (t_tralala *x, long deltaPosition, long deltaPitch)
{
    long i, count, grid = pizSequenceGrid (x->user);
    bool haveChanged = false;

    ARRAYSLOCK
    
    count = pizGrowingArrayCount (x->selected);
    
    for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
        long position     = pizGrowingArrayValueAtIndex (x->origin,   i + PIZ_DATA_POSITION);
        long pitch        = pizGrowingArrayValueAtIndex (x->origin,   i + PIZ_DATA_PITCH);
        long duration     = pizGrowingArrayValueAtIndex (x->origin,   i + PIZ_DATA_DURATION);
        long tempPosition = pizGrowingArrayValueAtIndex (x->selected, i + PIZ_DATA_POSITION);
        long tempPitch    = pizGrowingArrayValueAtIndex (x->selected, i + PIZ_DATA_PITCH);
        
        pitch    += deltaPitch;
        position += deltaPosition;
        
        position = MAX ((long)((position / (double)grid) + 0.5) * grid, 0);
        
        position = CLAMP (position, 0, PIZ_SEQUENCE_TIMELINE_SIZE - duration);
        pitch    = CLAMP (pitch, 0, PIZ_MAGIC_PITCH);
        
        pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_POSITION, position);
        pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_PITCH, pitch);
        
        if ((tempPosition != position) || (tempPitch != pitch)) {
            haveChanged = true;
        }
    }
    
    ARRAYSUNLOCK
    
    return haveChanged;
}

bool tralala_changeSelectedNotesDuration (t_tralala *x, long deltaPosition)
{
    long i, count, grid = pizSequenceGrid (x->user);
    bool haveChanged = false;
                
    ARRAYSLOCK

    count = pizGrowingArrayCount (x->selected);
    
    for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
        long maximum;
        long duration = pizGrowingArrayValueAtIndex (x->origin,   i + PIZ_DATA_DURATION);
        long position = pizGrowingArrayValueAtIndex (x->origin,   i + PIZ_DATA_POSITION);
        long temp     = pizGrowingArrayValueAtIndex (x->selected, i + PIZ_DATA_DURATION);
        
        maximum  = MIN (PIZ_SEQUENCE_MAXIMUM_DURATION, (PIZ_SEQUENCE_TIMELINE_SIZE - position));
                                    
        duration += deltaPosition;
        
        duration = MAX ((long)((duration / (double)grid) + 0.5) * grid, 0);
        duration = CLAMP (duration, grid, maximum);
        
        pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_DURATION, duration);
        
        if (duration != temp) {
            haveChanged = true;
        }
    }
    
    ARRAYSUNLOCK
    
    return haveChanged;
}

bool tralala_changeSelectedNotesVelocity (t_tralala *x, bool decrement)
{
    long count;
    bool haveChanged = false;
    
    ARRAYSLOCK
    
    if (count = pizGrowingArrayCount (x->selected)) {
        long i, step;
        long k = 0;
        long temp     = pizGrowingArrayValueAtIndex (x->origin,   PIZ_DATA_VELOCITY);
        long velocity = pizGrowingArrayValueAtIndex (x->selected, PIZ_DATA_VELOCITY);
        
        step = (long)((ABS (velocity - temp)) / 5.) + 1;
        
        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            long m = pizGrowingArrayValueAtIndex (x->selected, i + PIZ_DATA_VELOCITY);
            
            if (decrement) {
                m = m - step;
            } else {
                m = PIZ_MAGIC_VELOCITY - (m + step);
            }
            
            if (m < k) {
                k = m;
            }
        }
        
        step += k;
        
        if (step > 0) {
            if (decrement) {
                step = -step;
            }
                
            for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) { 
                long m = pizGrowingArrayValueAtIndex (x->selected, i + PIZ_DATA_VELOCITY) + step;
                pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_VELOCITY, m);
            }
            
            x->mouseVelocityValue = velocity - temp + step;
            haveChanged = true;
        }
    }
    
    ARRAYSUNLOCK
    
    return haveChanged;
}

void tralala_duplicateSelectedNotes (t_tralala *x)
{
    ARRAYSLOCK
    
    pizGrowingArrayAppendArray (x->unselected, x->selected);
    
    ARRAYSUNLOCK
    
    pizSequenceUnselectAllNotes (x->user);
    x->flags |= FLAG_HAVE_BEEN_DUPLICATED;
}

void tralala_setSelectedNotesVelocity (t_tralala *x, long velocity)
{
    long i, count;
    
    velocity = CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY);
    
    ARRAYSLOCK

    count = pizGrowingArrayCount (x->selected);
    
    for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
        pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_VELOCITY, velocity);
    }
    
    ARRAYSUNLOCK
}

void tralala_setSelectedNotesChannel (t_tralala *x, long channel)
{
    long i, count;
    
    channel = CLAMP (channel, 0, PIZ_MAGIC_CHANNEL);
    
    ARRAYSLOCK

    count = pizGrowingArrayCount (x->selected);
    
    for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
        pizGrowingArraySetValueAtIndex (x->selected, i + PIZ_DATA_CHANNEL, channel);
    }
    
    ARRAYSUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long tralala_hitZoneWithPoint (t_tralala *x, t_pt pt)
{
    t_rect  zoneRect;
    bool    a, b, c, d, e, f;
    long    start, end, down, up, k;
    long    hitTest = HIT_NOTHING;

    ARRAYSLOCK

    start = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_START);
    end   = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_END);
    down  = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_DOWN);
    up    = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_UP);

    ARRAYSUNLOCK
                        
    tralala_setRectWithZoneValues (x, &zoneRect, start, end, down, up);
    
    zoneRect.x -= x->windowOffsetX;
    zoneRect.y -= x->windowOffsetY;
    
    k = GUI_HIT_ZONE_RANGE;
    
    a = ((ABS (pt.x - zoneRect.x)) < k);
    b = ((ABS (pt.x - (zoneRect.x + zoneRect.width))) < k);
    c = ((pt.x > (zoneRect.x + (k / 2.))) && (pt.x < (zoneRect.x + zoneRect.width - (k / 2.))));
    d = ((ABS (pt.y - (zoneRect.y + zoneRect.height))) < k);
    e = ((ABS (pt.y - zoneRect.y)) < k);
    f = ((pt.y > (zoneRect.y + (k / 2.))) && (pt.y < (zoneRect.y + zoneRect.height - (k / 2.))));
                        
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
    long    i, numlines;
    double  textWidth, textHeight, fontSize, h1, h2, end; 
    t_rect  rect;
    long    isHit = HIT_NOTHING;
            
    jbox_get_rect_for_view ((t_object *)x, patcherview, &rect);
    fontSize = jbox_get_fontsize ((t_object *)x);
    jtextlayout_measure (x->textLayers[0], 0, -1, 1, &textWidth, &textHeight, &numlines);

    h1  = rect.height - (GUI_TEXT_SPACE + fontSize);
    h2  = h1 + textHeight;
    end = (x->textPosition[TEXT_CELL_COUNT - 1] + x->textWidth[TEXT_CELL_COUNT - 1] + GUI_TEXT_SPACE);
    
    if ((pt.y > h1) && (pt.y < h2)) {
        if (pizSequenceHasMarkedNote (x->user)) {
            if ((pt.x > 0) && (pt.x < end)) {
                isHit = HIT_LOCKED;
            }
        
            for (i = 0; i < TEXT_CELL_COUNT; i++) {
                if ((pt.x > x->textPosition[i]) && (pt.x < (x->textPosition[i] + x->textWidth[i]))) {
                    tralala_unselectAllText (x);
                    x->textIsSelected[i] = true;
                    isHit = HIT_TEXT;
                }
            }
        } else if ((pt.x > x->textPosition[0]) && (pt.x < (x->textPosition[0] + x->textWidth[0]))) {
            isHit = HIT_LOCKED;
        }   
    }

    return isHit;
}

bool tralala_hitNotesByRunIndex (t_tralala *x)
{
    bool haveChanged = false;

    pizGrowingArrayClear (x->played);

    if (x->runIndex == -1) {
        x->runIndex = 0;
        haveChanged = true;
    } else {
        long i, count, start, end, down, up;
        long err = PIZ_GOOD;
        
        start   = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_START);
        end     = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_END);
        down    = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_DOWN);
        up      = pizGrowingArrayValueAtIndex (x->zone, PIZ_DATA_UP);
        
        count = pizGrowingArrayCount (x->unselected);
        
        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            long err2       = PIZ_GOOD;
            long position   = pizGrowingArrayValueAtIndex (x->unselected, i + PIZ_DATA_POSITION);
            long pitch      = pizGrowingArrayValueAtIndex (x->unselected, i + PIZ_DATA_PITCH);
            long duration   = pizGrowingArrayValueAtIndex (x->unselected, i + PIZ_DATA_DURATION);
            long isMarked   = pizGrowingArrayValueAtIndex (x->unselected, i + PIZ_DATA_IS_MARKED);
            
            err2 |= (position < start);
            err2 |= (pitch < down);
            err2 |= (pitch > up);
            err2 |= (x->runIndex < position);
            err2 |= (x->runIndex > (position + duration));
            
            if (!err2) {
                err |= pizGrowingArrayAppend (x->played, position);
                err |= pizGrowingArrayAppend (x->played, pitch);
                err |= pizGrowingArrayAppend (x->played, PIZ_MAGIC_VELOCITY);
                err |= pizGrowingArrayAppend (x->played, duration);
                err |= pizGrowingArrayAppend (x->played, PIZ_MAGIC_CHANNEL);
                err |= pizGrowingArrayAppend (x->played, false);
                err |= pizGrowingArrayAppend (x->played, false);
                
                if (!isMarked) {
                    pizGrowingArraySetValueAtIndex (x->unselected, i + PIZ_DATA_IS_MARKED, true);
                    haveChanged = true;
                }
            } else if (isMarked){
                pizGrowingArraySetValueAtIndex (x->unselected, i + PIZ_DATA_IS_MARKED, false);
                haveChanged = true;
            }
        }
    }
        
    return haveChanged;
}

bool tralala_setCursorType (t_tralala *x, t_object *patcherview, t_jmouse_cursortype type)
{
    bool cursorSet = false;
    
    if (type != x->cursorType) {
        x->cursorType = type;
        jmouse_setcursor (patcherview, (t_object *)x, type);
        cursorSet = true;
    }
    
    return cursorSet;
}
                                                        
bool tralala_setCoordinatesWithPoint (t_tralala *x, PIZCoordinates *coordinates, t_pt pt)
{
    long    m, n;
    double  f = 1.;
    bool    inside = true;
    
    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5; break;
        case MODE_ZOOM_B : f = 1.;  break;
        case MODE_ZOOM_C : f = 2.;  break;
    }
    
    m = (long)((x->windowOffsetX + pt.x) / (GUI_PIXELS_PER_STEP * f));
    n = PIZ_MAGIC_PITCH - MAX (((long)((x->windowOffsetY + pt.y) / (GUI_PIXELS_PER_SEMITONE * f))), 0);
    
    coordinates->position = m;
    coordinates->pitch    = n;
    
    if ((n < 0) || (n > PIZ_MAGIC_PITCH) || (m < 0) || (m > (PIZ_SEQUENCE_TIMELINE_SIZE - 1))) {
        inside = false;
    }
        
    return inside;
}

void tralala_setRectWithZoneValues (t_tralala *x, t_rect *zoneRect, long start, long end, long down, long up)
{
    double  x1, x2, y1, y2;
    double  f = 1.;

    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5; break;
        case MODE_ZOOM_B : f = 1.;  break;
        case MODE_ZOOM_C : f = 2.;  break;
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
    
    zoneRect->x      = x1;
    zoneRect->y      = y1;
    zoneRect->width  = x2 - x1;
    zoneRect->height = y2 - y1;
}

void tralala_setRectWithCoordinatesAndDuration (t_tralala *x, t_rect *noteRect, PIZCoordinates *c, long d)
{
    double  x1, x2, y1, y2;
    double  f = 1.;

    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5; break;
        case MODE_ZOOM_B : f = 1.;  break;
        case MODE_ZOOM_C : f = 2.;  break;
    }
    
    x1 = (c->position * GUI_PIXELS_PER_STEP  * f);
    x2 = x1 + (d * GUI_PIXELS_PER_STEP * f);
    y1 = ((PIZ_MAGIC_PITCH - c->pitch) * GUI_PIXELS_PER_SEMITONE * f);
    y2 = y1 + (GUI_PIXELS_PER_SEMITONE * f);
    
    noteRect->x      = x1;
    noteRect->y      = y1;
    noteRect->width  = x2 - x1;
    noteRect->height = y2 - y1;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_setStringWithLong (char *string, long value, long formatMode)
{
    if (formatMode == MODE_FORMAT_LONG) {
        snprintf (string, SIZE_STRING_MAX, "%ld", value);
        
    } else if (formatMode == MODE_FORMAT_NOTENAME) {
        long m = value % 12;
        long n = (long)(value / 12.) - 2;
                    
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
        
    } else if (formatMode == MODE_FORMAT_TICKS) {
        snprintf (string, SIZE_STRING_MAX, "%ld", value * TIME_TICKS_PER_STEP);
        
    } else if (formatMode == MODE_FORMAT_GRID) {
        char *s = "Error";
        
        switch (value) {
            case PIZ_WHOLE_NOTE_DOTTED          : s = tll_sym_wholeDotted->s_name;          break;
            case PIZ_WHOLE_NOTE                 : s = tll_sym_whole->s_name;                break;
            case PIZ_WHOLE_NOTE_TRIPLET         : s = tll_sym_wholeTriplet->s_name;         break;
            case PIZ_HALF_NOTE_DOTTED           : s = tll_sym_halfDotted->s_name;           break;
            case PIZ_HALF_NOTE                  : s = tll_sym_half->s_name;                 break;
            case PIZ_HALF_NOTE_TRIPLET          : s = tll_sym_halfTriplet->s_name;          break;
            case PIZ_QUARTER_NOTE_DOTTED        : s = tll_sym_quarterDotted->s_name;        break;
            case PIZ_QUARTER_NOTE               : s = tll_sym_quarter->s_name;              break;
            case PIZ_QUARTER_NOTE_TRIPLET       : s = tll_sym_quarterTriplet->s_name;       break;
            case PIZ_EIGHTH_NOTE_DOTTED         : s = tll_sym_eighthDotted->s_name;         break;
            case PIZ_EIGHTH_NOTE                : s = tll_sym_eighth->s_name;               break;
            case PIZ_EIGHTH_NOTE_TRIPLET        : s = tll_sym_eighthTriplet->s_name;        break;
            case PIZ_SIXTEENTH_NOTE_DOTTED      : s = tll_sym_sixteenthDotted->s_name;      break;
            case PIZ_SIXTEENTH_NOTE             : s = tll_sym_sixteenth->s_name;            break;
            case PIZ_SIXTEENTH_NOTE_TRIPLET     : s = tll_sym_sixteenthTriplet->s_name;     break;
            case PIZ_THIRTY_SECOND_NOTE         : s = tll_sym_thirtySecond->s_name;         break;
            case PIZ_THIRTY_SECOND_NOTE_TRIPLET : s = tll_sym_thirtySecondTriplet->s_name;  break;
            case PIZ_NOTE_NONE                  : s = tll_sym_none->s_name;                 break;
        }  
        
        snprintf (string, SIZE_STRING_MAX, "%s", s); 
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

bool tralala_hasSelectedText (t_tralala *x, long *result)
{
    long i;
    bool isSelected = false;
    
    for (i = 0; i < TEXT_CELL_COUNT; i++) {
        if (x->textIsSelected[i]) {
            isSelected = true;
            *result = i;
        }
    }
    
    return isSelected;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_willChange (t_tralala *x)
{
    if (USER) {
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
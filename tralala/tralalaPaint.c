/*
 *  tralalaPaint.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 07/03/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define STRINGSAFE  textCell[SIZE_STRING_MAX - 1] = 0;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern tralalaSymbolsTableA tll_symbolsA;
extern tralalaSymbolsTableB tll_symbolsB;

extern t_jsurface *tll_thirtySecond       [3];
extern t_jsurface *tll_sixteenth          [3];
extern t_jsurface *tll_sixteenthTriplet   [3];
extern t_jsurface *tll_sixteenthDotted    [3];
extern t_jsurface *tll_eighth             [3];
extern t_jsurface *tll_eighthTriplet      [3];
extern t_jsurface *tll_eighthDotted       [3];
extern t_jsurface *tll_quarter            [3];
extern t_jsurface *tll_quarterTriplet     [3];
extern t_jsurface *tll_quarterDotted      [3];
extern t_jsurface *tll_half               [3];
extern t_jsurface *tll_halfTriplet        [3];
extern t_jsurface *tll_halfDotted         [3];
extern t_jsurface *tll_whole              [3];

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralala_paintTask (t_tralala *x) 
{       
    PIZError    err = PIZ_GOOD;
    PIZSequence *sequence = NULL;
    ulong       dirty;
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) { 
    //
    dirty = x->dirtyLayer; 
    DIRTYLAYER_UNSET (~(DIRTY_SEQUENCE | DIRTY_REFRESH | DIRTY_GRID | DIRTY_ZONE | DIRTY_NOTES | DIRTY_PLAYED));
        
    if (LIVE && !(dirty & DIRTY_SEQUENCE) && ((x->flags & FLAG_IS_RUNNING) || (x->runIndex == -1))) {
        ARRAYSLOCK

        if (tralala_hitNotesByRunIndex (x)) {           
            dirty |= DIRTY_PLAYED;
        }
        
        ARRAYSUNLOCK
    }
    
    if (dirty) {
    //
    ARRAYSLOCK
    
    if (USER) {
        sequence = x->user;
    } else {
        sequence = x->live;
    }
    
    if (dirty & DIRTY_GRID) {   
        jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_gridLayer);
    }
        
    if (dirty & DIRTY_ZONE) {
        if (dirty & DIRTY_SEQUENCE) {
        
            pizGrowingArrayClear (x->zone);
            
            if (x->flags & FLAG_ZONE_IS_SELECTED) {
                pizSequenceTempZoneToArray (sequence, x->zone);
            } else {
                pizSequenceZoneToArray (sequence, x->zone);
            }
        }
        
        jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_zoneLayer);
    }
        
    if (dirty & DIRTY_NOTES) {
        if (dirty & DIRTY_SEQUENCE) {
        
            if (USER) {  
                if (x->isMarkedNote = pizSequenceHasMarkedNote (x->user)) {
                    x->markedPitch      = pizSequenceMarkedNoteValue (x->user, PIZ_PITCH);
                    x->markedVelocity   = pizSequenceMarkedNoteValue (x->user, PIZ_VELOCITY);
                    x->markedDuration   = pizSequenceMarkedNoteValue (x->user, PIZ_DURATION);
                    x->markedChannel    = pizSequenceMarkedNoteValue (x->user, PIZ_CHANNEL);
                }
            }
        
            pizGrowingArrayClear (x->selected);
            pizGrowingArrayClear (x->unselected);
            
            if (USER) {
                err = pizSequenceNotesToArray (sequence, x->unselected, x->selected);
            } else {
                err = pizSequenceNotesToArray (sequence, x->unselected, x->unselected);
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
    
    err |= pizGrowingArrayCopy (x->zoneTemp, x->zone);
    err |= pizGrowingArrayCopy (x->playedTemp, x->played);
    err |= pizGrowingArrayCopy (x->selectedTemp, x->selected);
    err |= pizGrowingArrayCopy (x->unselectedTemp, x->unselected);
    
    ARRAYSUNLOCK
        
    if (dirty & DIRTY_LOCATE_LEFT) {
        x->offsetX          -= GUI_AUTOSCROLL_STEP;
        x->originPoint.x    += GUI_AUTOSCROLL_STEP; 
    }
    if (dirty & DIRTY_LOCATE_RIGHT) {
        x->offsetX          += GUI_AUTOSCROLL_STEP;
        x->originPoint.x    -= GUI_AUTOSCROLL_STEP;
    }
    if (dirty & DIRTY_LOCATE_DOWN) {
        x->offsetY          += GUI_AUTOSCROLL_STEP;
        x->originPoint.y    -= GUI_AUTOSCROLL_STEP;
    }
    if (dirty & DIRTY_LOCATE_UP) {
        x->offsetY          -= GUI_AUTOSCROLL_STEP;
        x->originPoint.y    += GUI_AUTOSCROLL_STEP;
    }
    
    if (!err) {
        jbox_redraw ((t_jbox *)x);
    }
    
    // 
    }
    //      
    } ATOMIC_DECREMENT (&x->paintLock);
    
    clock_fdelay (x->paintClock, CLOCK_PAINT_INTERVAL + CLOCK_RANDOMIZE * RANDOM);
}

bool tralala_hitNotesByRunIndex (t_tralala *x)
{
    bool haveChanged = 0;

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

void tralala_focusTask (t_tralala *x)
{
    x->flags |= FLAG_FOCUS;
    
    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paint (t_tralala *x, t_object *patcherview)
{   
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) {
        pizGrowingArrayCopy (x->zonePaint, x->zoneTemp);
        pizGrowingArrayCopy (x->playedPaint, x->playedTemp);
        pizGrowingArrayCopy (x->selectedPaint, x->selectedTemp);
        pizGrowingArrayCopy (x->unselectedPaint, x->unselectedTemp);
        
    } else {
        DIRTYLAYER_SET (DIRTY_SEQUENCE | DIRTY_GRID | DIRTY_ZONE | DIRTY_NOTES | DIRTY_PLAYED);
        jbox_redraw ((t_jbox *)x);
    }
    
    ATOMIC_DECREMENT (&x->paintLock);
    
    tralala_paintGrid   (x, patcherview);
    tralala_paintZone   (x, patcherview);
    tralala_paintNotes  (x, patcherview);
    
    if (LIVE && !(x->flags & FLAG_IS_MUTED)) {
        tralala_paintPlayed (x, patcherview);
    }

    if (x->viewText) {
        tralala_paintText (x, patcherview);
    }
    
    if (x->flags & FLAG_IS_LASSO) {
        tralala_paintLasso (x, patcherview);
    }
    
    if (x->flags & FLAG_INIT_PAINT_CLOCK) {
        x->flags &= ~FLAG_INIT_PAINT_CLOCK;
        clock_fdelay (x->paintClock, CLOCK_PAINT_INTERVAL + CLOCK_RANDOMIZE * RANDOM);
    }
}

void tralala_getdrawparams (t_tralala *x, t_object *patcherview, t_jboxdrawparams *params)
{
    jrgba_copy (&params->d_boxfillcolor, &x->backgroundColor);

    if (x->flags & FLAG_FOCUS) {
        jrgba_copy (&params->d_bordercolor, &x->focusedBorderColor);
    } else {
        jrgba_copy (&params->d_bordercolor, &x->unfocusedBorderColor);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_focusgained (t_tralala *x, t_object *patcherview)
{
    clock_fdelay (x->focusClock, CLOCK_FOCUS_INTERVAL);
}

void tralala_focuslost (t_tralala *x, t_object *patcherview)
{
    clock_unset (x->focusClock);
    
    if (USER) {
        tralala_willChange (x);
    }
    
    x->flags &= ~FLAG_FOCUS;
    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);

    DIRTYLAYER_SET (DIRTY_REFRESH | DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
}

void tralala_patcherview_vis (t_tralala *x, t_object *patcherview)
{
    DIRTYLAYER_SET (DIRTY_REFRESH | DIRTY_NOTES | DIRTY_ZONE | DIRTY_PLAYED);
    
    clock_fdelay (x->paintClock, 0.);
}

void tralala_patcherview_invis (t_tralala *x, t_object *patcherview)
{
    clock_unset (x->paintClock);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintText (t_tralala *x, t_object *patcherview)
{
    long                            i, t;
    long                            down, up;
    bool                            draw = false;
    double                          k = GUI_TEXT_SPACE;
    t_jgraphics                     *g = NULL;
    t_jfont                         *font = NULL;
    t_symbol                        *fontName = NULL;
    t_rect                          rect;
    long                            fontSlant, fontWeight, numlines;
    double                          fontSize, textHeight;
    t_atom                          color[4];
    char                            textCell[SIZE_STRING_MAX];
    char                            temp[SIZE_STRING_MAX];
    long                            textValues[TEXT_CELL_COUNT]; 
    t_jrgba                         backgroundTextColor;
    t_jgraphics_textlayout_flags    flags;
    t_jgraphics_text_justification  justification;
    
    flags = (t_jgraphics_textlayout_flags)(JGRAPHICS_TEXTLAYOUT_NOWRAP | JGRAPHICS_TEXTLAYOUT_USEELLIPSIS); 
    justification = (t_jgraphics_text_justification) (JGRAPHICS_TEXT_JUSTIFICATION_LEFT);
    
    snprintf (textCell, SIZE_STRING_MAX, "Error");
    
    g = (t_jgraphics *)patcherview_get_jgraphics (patcherview);
    jbox_get_rect_for_view ((t_object *)x, patcherview, &rect); 
    
    fontName    = jbox_get_fontname (((t_object *)x));
    fontWeight  = jbox_get_font_weight ((t_object *)x);
    fontSlant   = jbox_get_font_slant ((t_object *)x);
    fontSize    = jbox_get_fontsize ((t_object *)x);
    
    font = jfont_create (fontName->s_name, fontSlant, fontWeight, fontSize); 

    jrgba_to_atoms  (&x->backgroundColor, color);
    atom_setfloat   (color + 3, GUI_TEXT_BACKGROUND_ALPHA);
    atoms_to_jrgba  (4, color, &backgroundTextColor);
    
    if (x->textMode == MODE_TEXT_NOTE && USER && x->isMarkedNote) {  
        textValues[TEXT_CELL_VELOCITY] = x->markedVelocity;
        textValues[TEXT_CELL_DURATION] = x->markedDuration;
        textValues[TEXT_CELL_CHANNEL]  = x->markedChannel;
        textValues[TEXT_CELL_PITCH]    = x->markedPitch;
        
        x->textPosition[0] = GUI_TEXT_SPACE;
        
        for (i = 0; i < TEXT_CELL_COUNT; i++) {
            switch (i)  {
                case    TEXT_CELL_PITCH    : t = MODE_FORMAT_NOTENAME;  break;
                case    TEXT_CELL_DURATION : t = MODE_FORMAT_TICKS;     break;
                default                    : t = MODE_FORMAT_LONG;      break;
            }
            
            tralala_setString (textCell, textValues[i], t);
                                            
            jtextlayout_set (x->textLayers[i], 
                            textCell, 
                            font, 
                            x->textPosition[i], 
                            (rect.height - (fontSize + GUI_TEXT_SPACE)), 
                            (rect.width - k), 
                            fontSize, 
                            justification, flags);
            
            jtextlayout_measure (x->textLayers[i], 0, -1, 1, &x->textWidth[i], &textHeight, &numlines);
            
            k += (x->textWidth[i] + GUI_TEXT_SPACE);
            
            if (i < (TEXT_CELL_COUNT - 1)) {
                x->textPosition[i + 1] = x->textPosition[i] + (x->textWidth[i] + GUI_TEXT_SPACE);
            }
            
            if (x->flags & FLAG_FOCUS) {
                if (x->textIsSelected[i]) {
                    jtextlayout_settextcolor (x->textLayers[i], &x->selectedTextColor);
                } else {
                    jtextlayout_settextcolor (x->textLayers[i], &x->focusedTextColor);
                }
            } else {
                jtextlayout_settextcolor (x->textLayers[i], &x->unfocusedTextColor);
            }
        }
        
        jgraphics_set_source_jrgba (g, &backgroundTextColor);
        jgraphics_rectangle_fill_fast (g, 0., (rect.height - (fontSize + GUI_TEXT_SPACE)), k, textHeight);
    
        for (i = 0; i < TEXT_CELL_COUNT; i++) {
            jtextlayout_draw (x->textLayers[i], g);
        }
        
    } else if (x->textMode == MODE_TEXT_NOTE && USER) {
        draw = true;
        tralala_setString (temp, x->grid, MODE_FORMAT_GRID);
        
        snprintf (textCell, SIZE_STRING_MAX, "Slot %ld", x->slotIndex); STRINGSAFE
        snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, temp); STRINGSAFE
        
        if (x->noteValue != PIZ_NOTE_NONE) {
            tralala_setString (temp, x->noteValue, MODE_FORMAT_GRID);
            snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, temp); STRINGSAFE
        }
        
    } else if (x->textMode == MODE_TEXT_MOUSE_PITCH) {
        draw = true;
        tralala_setString (textCell, x->mousePitchValue, MODE_FORMAT_NOTENAME);
        
    } else if (x->textMode == MODE_TEXT_MOUSE_VELOCITY) {
        draw = true;
        tralala_setString (temp, x->mouseVelocityValue, MODE_FORMAT_LONG);
        
        if (x->mouseVelocityValue >= 0) {
            snprintf (textCell, SIZE_STRING_MAX, "Velocity : +%s", temp); STRINGSAFE
        } else {
            snprintf (textCell, SIZE_STRING_MAX, "Velocity : %s", temp); STRINGSAFE
        }
        
    } else if (x->textMode == MODE_TEXT_ZONE && pizGrowingArrayCount (x->zonePaint)) {
        draw = true;
        down = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_DOWN);
        up   = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_UP);
                
        tralala_setString (textCell, down, MODE_FORMAT_NOTENAME);
        tralala_setString (temp, up, MODE_FORMAT_NOTENAME);
        
        snprintf (textCell, SIZE_STRING_MAX, "%s %s", textCell, temp); STRINGSAFE
        
    } else if (LIVE) {
        draw = true;
        
        snprintf (textCell, SIZE_STRING_MAX, "%s", x->scaleKey->s_name); STRINGSAFE
        
        if (x->scaleType != tll_sym_none) {
            snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, x->scaleType->s_name); STRINGSAFE
        }
        if (x->patternCell != tll_sym_none) {
            snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, x->patternCell->s_name); STRINGSAFE
        }
        if (x->patternSize) {
            snprintf (textCell, SIZE_STRING_MAX, "%s / pattern", textCell); STRINGSAFE
        }
        
        if (x->velocity > 0) {
            snprintf (textCell, SIZE_STRING_MAX, "%s / +%ld", textCell, x->velocity); STRINGSAFE
        } else if (x->velocity < 0) {
            snprintf (textCell, SIZE_STRING_MAX, "%s / %ld", textCell, x->velocity); STRINGSAFE
        }
    }
    
    if (draw) {
        jtextlayout_set (x->textLayers[0], 
                        textCell, 
                        font, 
                        k, 
                        (rect.height - (fontSize + GUI_TEXT_SPACE)), 
                        (rect.width - k), 
                        fontSize, 
                        justification, flags);
        
        jtextlayout_measure (x->textLayers[0], 0, -1, 1, &x->textWidth[0], &textHeight, &numlines);
        
        x->textPosition[0] = GUI_TEXT_SPACE;
        k += x->textWidth[0] + GUI_TEXT_SPACE;
        
        if (x->flags & FLAG_FOCUS) {
            jtextlayout_settextcolor (x->textLayers[0], &x->focusedTextColor); 
        } else {
            jtextlayout_settextcolor (x->textLayers[0], &x->unfocusedTextColor);
        }
        
        jgraphics_set_source_jrgba    (g, &backgroundTextColor);
        jgraphics_rectangle_fill_fast (g, 0., (rect.height - (fontSize + GUI_TEXT_SPACE)), k, textHeight);

        jtextlayout_draw (x->textLayers[0], g);
    }
            
    jfont_destroy (font);
}
            
void tralala_paintGrid (t_tralala *x, t_object *patcherview)
{
    double      gridWidth, gridHeight, f = 1.;
    long        z = x->zoomMode;
    t_jgraphics *g = NULL;
            
    switch (z) {
        case MODE_ZOOM_A : f = 0.5;  break;
        case MODE_ZOOM_B : f = 1.;   break;
        case MODE_ZOOM_C : f = 2.;   break;
    }
    
    gridWidth   = f * (PIZ_DEFAULT_TIMELINE_SIZE * GUI_PIXELS_PER_STEP);
    gridHeight  = f * ((PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE);

    if (g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_gridLayer, gridWidth, gridHeight)) {
        long            i, j;
        t_rect          srcRect, destRect;
        double          imageWidth, imageHeight;
        t_jrgba         gridColor;
        t_jsurface      *background = NULL;
        PIZNoteValue    grid;
        
        if (USER) {
            grid = x->grid;       
        } else {
            grid = x->cell;
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
            case PIZ_NOTE_NONE                  :   background = tll_eighth                 [z]; break;
        }
            
        imageWidth  = jgraphics_image_surface_get_width (background);
        imageHeight = jgraphics_image_surface_get_height (background);
        
        srcRect.x       = 0.;
        srcRect.y       = 0.;
        srcRect.width   = imageWidth;
        srcRect.height  = imageHeight;
        
        for (i = 0; i < ((PIZ_MAGIC_PITCH + 1) / GUI_JSURFACE_SEMITONES); i ++) {
            for (j = 0; j < (PIZ_DEFAULT_TIMELINE_SIZE / GUI_JSURFACE_STEPS); j++) {
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
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_gridLayer, -x->offsetX, -x->offsetY);
}

void tralala_paintNotes (t_tralala *x, t_object *patcherview)
{
    double      gridWidth, gridHeight, f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5;  break;
        case MODE_ZOOM_B : f = 1.;   break;
        case MODE_ZOOM_C : f = 2.;   break;
    }
    
    gridWidth   = f * (PIZ_DEFAULT_TIMELINE_SIZE * GUI_PIXELS_PER_STEP);
    gridHeight  = f * ((PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE);
    
    if (g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_notesLayer, gridWidth, gridHeight)) {
        long    i, count; 
        long    markedPosition  = -1;
        long    markedPitch     = -1;
        long    markedVelocity  = -1;
        long    markedDuration  = -1;
        t_jrgba color1, color2, color3;
        
        if (x->flags & FLAG_FOCUS) {
            if (USER) {
                jrgba_copy (&color3, &x->focusedUserMarkedNoteColor);
                jrgba_copy (&color2, &x->focusedUserSelectedNoteColor);
            } else {
                jrgba_copy (&color2, &x->focusedLivePlayedNoteColor);
                jrgba_copy (&color1, &x->focusedLiveNoteColor); 
            }
        } else {
           if (USER) {
                jrgba_copy (&color3, &x->unfocusedUserSelectedNoteColor);
                jrgba_copy (&color2, &x->unfocusedUserSelectedNoteColor);
                jrgba_copy (&color1, &x->unfocusedUserNoteColor);
            } else {
                jrgba_copy (&color1, &x->unfocusedLiveNoteColor);
            }
        }

        count = pizGrowingArrayCount (x->unselectedPaint);
        
        if ((x->flags & FLAG_FOCUS) && USER) {
            for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
                long position   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_POSITION);
                long pitch      = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_PITCH);
                long velocity   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_VELOCITY);
                long duration   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_DURATION);
                
                tralala_noteWithColor (x, g, position, pitch, velocity, duration, NULL);
            }
        } else {
            for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
                long position   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_POSITION);
                long pitch      = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_PITCH);
                long velocity   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_VELOCITY);
                long duration   = pizGrowingArrayValueAtIndex (x->unselectedPaint, i + PIZ_DATA_DURATION);
                                            
                tralala_noteWithColor (x, g, position, pitch, velocity, duration, &color1);
            }
        }
        
        count = pizGrowingArrayCount (x->selectedPaint);
        
        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            long position   = pizGrowingArrayValueAtIndex (x->selectedPaint, i + PIZ_DATA_POSITION);
            long pitch      = pizGrowingArrayValueAtIndex (x->selectedPaint, i + PIZ_DATA_PITCH);
            long velocity   = pizGrowingArrayValueAtIndex (x->selectedPaint, i + PIZ_DATA_VELOCITY);
            long duration   = pizGrowingArrayValueAtIndex (x->selectedPaint, i + PIZ_DATA_DURATION);
            long isMarked   = pizGrowingArrayValueAtIndex (x->selectedPaint, i + PIZ_DATA_IS_MARKED);
            
            if (isMarked) {
                markedPosition  = position;
                markedPitch     = pitch;
                markedVelocity  = velocity;
                markedDuration  = duration;
            } else {
                tralala_noteWithColor (x, g, position, pitch, velocity, duration, &color2);
            }
        }
        
        if (markedPosition != -1) {
            tralala_noteWithColor (x, g, markedPosition, markedPitch, markedVelocity, markedDuration, &color3);
        }
    
        jbox_end_layer ((t_object*)x, patcherview, tll_sym_notesLayer);
    }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_notesLayer, -x->offsetX, -x->offsetY);
}

void tralala_paintPlayed (t_tralala *x, t_object *patcherview)
{
    double      gridWidth, gridHeight, f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5;  break;
        case MODE_ZOOM_B : f = 1.;   break;
        case MODE_ZOOM_C : f = 2.;   break;
    }
    
    gridWidth   = f * (PIZ_DEFAULT_TIMELINE_SIZE * GUI_PIXELS_PER_STEP);
    gridHeight  = f * ((PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE);

    if (g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, gridWidth, gridHeight)) {
        long    i, count; 
        t_jrgba color;
            
        if (x->flags & FLAG_FOCUS) {
            jrgba_copy (&color, &x->focusedLivePlayedNoteColor);
        } else  {
            jrgba_copy (&color, &x->unfocusedLivePlayedNoteColor);
        }

        count = pizGrowingArrayCount (x->playedPaint);
        
        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            long position   = pizGrowingArrayValueAtIndex (x->playedPaint, i + PIZ_DATA_POSITION);
            long pitch      = pizGrowingArrayValueAtIndex (x->playedPaint, i + PIZ_DATA_PITCH);
            long velocity   = pizGrowingArrayValueAtIndex (x->playedPaint, i + PIZ_DATA_VELOCITY);
            long duration   = pizGrowingArrayValueAtIndex (x->playedPaint, i + PIZ_DATA_DURATION);
            
            tralala_noteWithColor (x, g, position, pitch, velocity, duration, &color);
        }
        
        jbox_end_layer ((t_object*)x, patcherview, tll_sym_playedNotesLayer);
    }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, -x->offsetX, -x->offsetY);
}                                                      
                                                        
void tralala_paintZone (t_tralala *x, t_object *patcherview)
{
    double      gridWidth, gridHeight, f = 1.;
    t_jgraphics *g = NULL;

    switch (x->zoomMode) {
        case MODE_ZOOM_A : f = 0.5;  break;
        case MODE_ZOOM_B : f = 1.;   break;
        case MODE_ZOOM_C : f = 2.;   break;
    }
    
    gridWidth   = f * (PIZ_DEFAULT_TIMELINE_SIZE * GUI_PIXELS_PER_STEP);
    gridHeight  = f * ((PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE);
    
    if (g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, gridWidth, gridHeight)) {
        if (pizGrowingArrayCount (x->zonePaint)) {
            long    start   = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_START);
            long    end     = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_END);
            long    down    = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_DOWN);
            long    up      = pizGrowingArrayValueAtIndex (x->zonePaint, PIZ_DATA_UP);
            t_rect  zoneRect;
                        
            tralala_setRectWithZoneValues (x, &zoneRect, start, end, down, up);
                        
            if (!(x->flags & FLAG_FOCUS)) {
                jgraphics_set_source_jrgba (g, &x->unfocusedZoneColor);
            } else if (x->flags & FLAG_ZONE_IS_SELECTED) {
                jgraphics_set_source_jrgba (g, &x->focusedSelectedZoneColor);
            } else if (USER) {
                jgraphics_set_source_jrgba (g, &x->focusedUserZoneColor);
            } else {
                jgraphics_set_source_jrgba (g, &x->focusedLiveZoneColor);
            }

            jgraphics_rectangle_fill_fast (g, zoneRect.x, zoneRect.y, zoneRect.width, zoneRect.height);
        }
            
        jbox_end_layer ((t_object*)x, patcherview, tll_sym_zoneLayer);
    }
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, -x->offsetX, -x->offsetY);
}

void tralala_paintLasso (t_tralala *x, t_object *patcherview)
{
    double a, b, u, v, width, height;
    
    t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics (patcherview);

    a       = MIN (x->originPoint.x, x->point.x);
    b       = MIN (x->originPoint.y, x->point.y);
    u       = MAX (x->originPoint.x, x->point.x);
    v       = MAX (x->originPoint.y, x->point.y);
    width   = u - a;
    height  = v - b;
            
    jgraphics_set_source_jrgba (g, &x->lassoColor);
            
    jgraphics_rectangle_fill_fast (g, a, b, width, height);
}

void tralala_noteWithColor (t_tralala *x, t_jgraphics *g, long p, long pitch, long v, long d, t_jrgba *color)
{
    t_rect          noteRect;
    PIZCoordinates  coordinates;
    t_jrgba         noteColor;
    t_atom          temp[4];
    
    coordinates.position = p;
    coordinates.pitch    = pitch;
    
    tralala_setRectWithCoordinatesAndDuration (x, &noteRect, &coordinates, d);
    
    if (color) {
        jrgba_to_atoms (color, temp);
        atom_setfloat  (temp + 3, CLAMP ((v / (double)PIZ_MAGIC_VELOCITY), 0.25, 1.));
        atoms_to_jrgba (4, temp, &noteColor);
    } else {
        if (v == 0) {
            jrgba_copy (&noteColor, &x->focusedUserSilentNoteColor);
        } else {
            double alpha = (v + GUI_VELOCITY_OFFSET) / (double)PIZ_MAGIC_VELOCITY;
    
            switch (pitch % PIZ_MAGIC_SCALE) {
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
    
            atom_setfloat   (temp + 3, CLAMP (alpha, 0.25, 1.));
            atoms_to_jrgba  (4, temp, &noteColor);
        }
    }    
        
    jgraphics_set_source_jrgba (g, &noteColor);
    jgraphics_rectangle_fill_fast (g, noteRect.x, noteRect.y, noteRect.width, noteRect.height);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
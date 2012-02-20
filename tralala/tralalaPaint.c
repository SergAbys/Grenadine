/*
 *  tralalaPaint.c
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
    ulong       dirty = x->dirtyLayer;
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) {
    
    DIRTYLAYER_UNSET (~(DIRTY_GRID | DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE | DIRTY_REFRESH | DIRTY_PLAYED));
    
    systhread_mutex_lock (&x->arraysMutex);
    
    if (LIVE && !(dirty & DIRTY_CHANGE) && ((x->flags & FLAG_IS_RUNNING) || (x->runIndex == -1))) {
        if (tralala_hitNotesByRunIndex (x)) {           
            dirty |= DIRTY_PLAYED;
        }
    }
        
    if (dirty) {   
        if (dirty & DIRTY_LOCATE_LEFT) {
            x->windowOffsetX    -= GUI_AUTOSCROLL_STEP;
            x->originPoint.x    += GUI_AUTOSCROLL_STEP; 
        }
        
        if (dirty & DIRTY_LOCATE_RIGHT) {
            x->windowOffsetX    += GUI_AUTOSCROLL_STEP;
            x->originPoint.x    -= GUI_AUTOSCROLL_STEP;
        }
        
        if (dirty & DIRTY_LOCATE_DOWN) {
            x->windowOffsetY    += GUI_AUTOSCROLL_STEP;
            x->originPoint.y    -= GUI_AUTOSCROLL_STEP;
        }
        
        if (dirty & DIRTY_LOCATE_UP) {
            x->windowOffsetY    -= GUI_AUTOSCROLL_STEP;
            x->originPoint.y    += GUI_AUTOSCROLL_STEP;
        }
            
        if (dirty & DIRTY_GRID) {   
            jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_gridLayer);
        }
            
        if (dirty & DIRTY_ZONE) {
            if (dirty & DIRTY_CHANGE) {
                PIZSequence *sequence = NULL;
            
                switch (x->sequenceMode) {
                    case MODE_SEQUENCE_USER     : sequence = x->user;   break;
                    case MODE_SEQUENCE_LIVE     : sequence = x->live;   break;
                }
            
                if (sequence) {
                    pizGrowingArrayClear (x->zone);
                    
                    if (x->flags & FLAG_ZONE_IS_SELECTED) {
                        err = pizSequenceTempZoneToArray (sequence, x->zone);
                    } else {
                        err = pizSequenceZoneToArray (sequence, x->zone);
                    }
                }
            }
            
            if (!err) {
                jbox_invalidate_layer ((t_object*)x, NULL, tll_sym_zoneLayer);
            }
        }
            
        if (dirty & DIRTY_NOTES) {
            if (dirty & DIRTY_CHANGE) {
                pizGrowingArrayClear (x->unselected);
                pizGrowingArrayClear (x->selected);
                    
                if (USER) {
                    err = pizSequenceNotesToArray (x->user, x->unselected, x->selected);
                } else {
                    err = pizSequenceNotesToArray (x->live, x->unselected, x->unselected);
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
        
        err |= pizGrowingArrayCopy (x->zoneCopy, x->zone);
        err |= pizGrowingArrayCopy (x->playedCopy, x->played);
        err |= pizGrowingArrayCopy (x->selectedCopy, x->selected);
        err |= pizGrowingArrayCopy (x->unselectedCopy, x->unselected);
        
        if (!err) {
            jbox_redraw ((t_jbox *)x);
        }
    }
    
    systhread_mutex_unlock  (&x->arraysMutex);
           
    } ATOMIC_DECREMENT (&x->paintLock);
    
    clock_fdelay (x->paintClock, CLOCK_PAINT_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
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

void tralala_paint (t_tralala *x, t_object *patcherview)
{   
    if (!(x->flags & FLAG_INIT_PAINT_CLOCK)) {
        clock_fdelay (x->paintClock, CLOCK_PAINT_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
        x->flags |= FLAG_INIT_PAINT_CLOCK;
    }
    
    if (ATOMIC_INCREMENT (&x->paintLock) == 1) {
        tralala_paintGrid (x, patcherview);
        tralala_paintZone (x, patcherview);
        tralala_paintNotes (x, patcherview);
        
        if (LIVE && !(x->flags & FLAG_IS_MUTED)) {
            tralala_paintPlayedNotes (x, patcherview);
        }

        if (x->viewText) {
            tralala_paintText (x, patcherview);
        }
        
        if (x->flags & FLAG_IS_LASSO) {
            tralala_paintLasso (x, patcherview);
        }
        
        ATOMIC_DECREMENT (&x->paintLock);
    } else {
        DIRTYLAYER_SET (DIRTY_REFRESH);
            
        ATOMIC_DECREMENT (&x->paintLock);
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
    
    x->flags &= ~FLAG_FOCUS;
    
    tralala_setCursorType (x, patcherview, JMOUSE_CURSOR_ARROW);
    
    if (USER) {
        if (x->flags & FLAG_ZONE_IS_SELECTED) {
            pizSequencePutTempZone (x->user);
                
            x->flags &= ~FLAG_ZONE_IS_SELECTED;
            DIRTYLAYER_SET (DIRTY_CHANGE);
        }
        
        if (x->flags & FLAG_IS_LASSO) {
            x->flags &= ~FLAG_IS_LASSO;
            DIRTYLAYER_SET (DIRTY_REFRESH);
        }
    
        tralala_stopAutoscroll (x);
        
        x->hitTest = HIT_NOTHING;
    }
    
    if (x->textMode != MODE_TEXT_NOTE) {
        x->textMode = MODE_TEXT_NOTE;
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

void tralala_paintText (t_tralala *x, t_object *patcherview)
{
    long                            i;
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
    t_jrgba                         backgroundTextColor;
    t_jgraphics_textlayout_flags    flags = (t_jgraphics_textlayout_flags)
                                    (JGRAPHICS_TEXTLAYOUT_NOWRAP | JGRAPHICS_TEXTLAYOUT_USEELLIPSIS); 
    
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
    
    if (x->textMode == MODE_TEXT_NOTE && USER) {
        if (pizSequenceHasMarkedNote (x->user)) {
            long textValues[TEXT_CELL_COUNT];           
            
            textValues[TEXT_ORDER_VELOCITY] = pizSequenceMarkedNoteValue (x->user, PIZ_VELOCITY);
            textValues[TEXT_ORDER_DURATION] = pizSequenceMarkedNoteValue (x->user, PIZ_DURATION);
            textValues[TEXT_ORDER_CHANNEL]  = pizSequenceMarkedNoteValue (x->user, PIZ_CHANNEL);
            textValues[TEXT_ORDER_PITCH]    = pizSequenceMarkedNoteValue (x->user, PIZ_PITCH);
            
            x->textPosition[0] = GUI_TEXT_SPACE;
            
            for (i = 0; i < TEXT_CELL_COUNT; i++) {
                switch (i)  {
                case TEXT_ORDER_PITCH       : tralala_setStringWithLong (textCell, textValues[i], 
                                                MODE_FORMAT_NOTENAME); break;
                case TEXT_ORDER_DURATION    : tralala_setStringWithLong (textCell, textValues[i], 
                                                MODE_FORMAT_TICKS); break;
                default                     : tralala_setStringWithLong (textCell, textValues[i], 
                                                MODE_FORMAT_LONG); break;
                }
                
                jtextlayout_set (x->textLayers[i], textCell, font, x->textPosition[i], 
                    (rect.height - (fontSize + GUI_TEXT_SPACE)), (rect.width - k), fontSize, 
                    (t_jgraphics_text_justification) (JGRAPHICS_TEXT_JUSTIFICATION_LEFT), flags);
                
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

            jgraphics_rectangle_fill_fast (g, 0., (rect.height - (fontSize + GUI_TEXT_SPACE)), 
                k, textHeight);
        
            for (i = 0; i < TEXT_CELL_COUNT; i++) {
                jtextlayout_draw (x->textLayers[i], g);
            }
        } else {
            char         temp[SIZE_STRING_MAX];
            PIZNoteValue grid = pizSequenceGrid (x->user);
            PIZNoteValue value = pizSequenceNoteValue (x->user);
            
            snprintf (textCell, SIZE_STRING_MAX, "Slot %ld", x->slotIndex);
            textCell[SIZE_STRING_MAX - 1] = 0;
            
            tralala_setStringWithLong (temp, grid, MODE_FORMAT_GRID);
            
            snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, temp);
            textCell[SIZE_STRING_MAX - 1] = 0;
            
            if (value != PIZ_NOTE_NONE) {
                tralala_setStringWithLong (temp, value, MODE_FORMAT_GRID);
            
                snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, temp);
                textCell[SIZE_STRING_MAX - 1] = 0;
            }
            
            draw = true;
        }
    } else {
        if (x->textMode == MODE_TEXT_MOUSE_PITCH) {
            tralala_setStringWithLong (textCell, x->mousePitchValue, MODE_FORMAT_NOTENAME);
            draw = true;
        }
        else if (x->textMode == MODE_TEXT_MOUSE_VELOCITY) {
            char textVelocity [SIZE_STRING_MAX];
            
            tralala_setStringWithLong (textVelocity, x->mouseVelocityValue, MODE_FORMAT_LONG);
            
            if (x->mouseVelocityValue >= 0) {
                snprintf (textCell, SIZE_STRING_MAX, "Velocity : +%s", textVelocity);
            } else {
                snprintf (textCell, SIZE_STRING_MAX, "Velocity : %s", textVelocity);
            }
            
            textCell[SIZE_STRING_MAX - 1] = 0;
            
            draw = true;
        } else if (x->textMode == MODE_TEXT_ZONE && pizGrowingArrayCount (x->zoneCopy)) {
            char         textDown[SIZE_STRING_MAX];
            char         textUp [SIZE_STRING_MAX];
            long         s      = -1;
            long         start  = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_START);
            long         end    = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_END);
            long         down   = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_DOWN);
            long         up     = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_UP);
            PIZNoteValue grid   = pizSequenceGrid (x->user);
                    
            tralala_setStringWithLong (textDown, down, MODE_FORMAT_NOTENAME);
            tralala_setStringWithLong (textUp, up, MODE_FORMAT_NOTENAME);
            
            if (grid == PIZ_QUARTER_NOTE || grid == PIZ_EIGHTH_NOTE || grid == PIZ_SIXTEENTH_NOTE) {
                long temp = (end - start) / grid;
                
                if ((temp * grid) == (end - start)) {
                    s = temp;
                }
                
                if ((grid == PIZ_EIGHTH_NOTE && s == 8) || (grid == PIZ_SIXTEENTH_NOTE && s == 16)) {
                    s = 4;
                    grid = PIZ_QUARTER_NOTE;
                }
                
                if ((grid == PIZ_QUARTER_NOTE && s == 6) || (grid == PIZ_SIXTEENTH_NOTE && s == 24)) {
                    s = 12;
                    grid = PIZ_EIGHTH_NOTE;
                }
            }
            
            if (s != -1) {
                if (grid == PIZ_QUARTER_NOTE) {
                    snprintf (textCell, SIZE_STRING_MAX, "%s %s : %ld/4", textDown, textUp, s);
                } else if (grid == PIZ_EIGHTH_NOTE) {
                    snprintf (textCell, SIZE_STRING_MAX, "%s %s : %ld/8", textDown, textUp, s);
                } else if (grid == PIZ_SIXTEENTH_NOTE) {
                    snprintf (textCell, SIZE_STRING_MAX, "%s %s : %ld/16", textDown, textUp, s);
                }
            } else {
                snprintf (textCell, SIZE_STRING_MAX, "%s %s", textDown, textUp);
            }
            
            textCell[SIZE_STRING_MAX - 1] = 0;
            
            draw = true;
        } else if (LIVE) {
            snprintf (textCell, SIZE_STRING_MAX, "%s", x->scaleKey->s_name);
            textCell[SIZE_STRING_MAX - 1] = 0;
            
            if (x->scaleType != tll_sym_none) {
                snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, x->scaleType->s_name);
                textCell[SIZE_STRING_MAX - 1] = 0;
            }
            
            if (x->patternCell != tll_sym_none) {
                snprintf (textCell, SIZE_STRING_MAX, "%s / %s", textCell, x->patternCell->s_name);
                textCell[SIZE_STRING_MAX - 1] = 0;
            }
                        
            if (x->patternSize) {
                snprintf (textCell, SIZE_STRING_MAX, "%s / pattern", textCell);
                textCell[SIZE_STRING_MAX - 1] = 0;
            }
            
            if (x->velocity > 0) {
                snprintf (textCell, SIZE_STRING_MAX, "%s / +%ld", textCell, x->velocity);
                textCell[SIZE_STRING_MAX - 1] = 0;
            } else if (x->velocity < 0) {
                snprintf (textCell, SIZE_STRING_MAX, "%s / %ld", textCell, x->velocity);
                textCell[SIZE_STRING_MAX - 1] = 0;
            }
                    
            draw = true;
        }
    }
    
    if (draw) {
        jtextlayout_set (x->textLayers[0], textCell, font, k, 
            (rect.height - (fontSize + GUI_TEXT_SPACE)), (rect.width - k), fontSize, 
            (t_jgraphics_text_justification) (JGRAPHICS_TEXT_JUSTIFICATION_LEFT), flags);
        
        jtextlayout_measure (x->textLayers[0], 0, -1, 1, &x->textWidth[0], &textHeight, &numlines);
        
        x->textPosition[0] = GUI_TEXT_SPACE;
        k += x->textWidth[0] + GUI_TEXT_SPACE;
        
        if (x->flags & FLAG_FOCUS) {
            jtextlayout_settextcolor (x->textLayers[0], &x->focusedTextColor); 
        } else {
            jtextlayout_settextcolor (x->textLayers[0], &x->unfocusedTextColor);
        }
        
        jgraphics_set_source_jrgba      (g, &backgroundTextColor);
        jgraphics_rectangle_fill_fast   (g, 0., (rect.height - (fontSize + GUI_TEXT_SPACE)), k, textHeight);

        jtextlayout_draw (x->textLayers[0], g);
    }
            
    jfont_destroy (font);
}
            
void tralala_paintGrid (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
    }
        
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_gridLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * GUI_PIXELS_PER_STEP * f, (PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE * f);

    if (g) {
        long            i;
        t_rect          srcRect, destRect;
        double          imageWidth, imageHeight, gridWidth, gridHeight;
        t_jrgba         gridColor;
        t_jsurface      *background = NULL;
        long            z = x->zoomMode;
        PIZNoteValue    grid = PIZ_NOTE_NONE;
        
        if (LIVE) {
            grid = x->cell;
        } 
        
        if (grid == PIZ_NOTE_NONE) {
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
            case PIZ_NOTE_NONE                  :   background = tll_eighth                 [z]; break;
        }
            
        imageWidth  = jgraphics_image_surface_get_width (background);
        imageHeight = jgraphics_image_surface_get_height (background);
        
        gridWidth   = PIZ_SEQUENCE_TIMELINE_SIZE * GUI_PIXELS_PER_STEP * f;
        gridHeight  = (PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE * f;
        
        srcRect.x       = 0.;
        srcRect.y       = 0.;
        srcRect.width   = imageWidth;
        srcRect.height  = imageHeight;
        
        for (i = 0; i < ((PIZ_MAGIC_PITCH + 1) / GUI_JSURFACE_SEMITONES); i ++) {
            long j;
            
            for (j = 0; j < (PIZ_SEQUENCE_TIMELINE_SIZE / GUI_JSURFACE_STEPS); j++) {
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
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_gridLayer, -x->windowOffsetX, -x->windowOffsetY);
}

void tralala_paintNotes (t_tralala *x, t_object *patcherview)
{
    double      f = 1.;
    t_jgraphics *g = NULL;
    
    switch (x->zoomMode) {
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
    }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_notesLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * GUI_PIXELS_PER_STEP * f, (PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE * f);
    
    if (g) {
        long    i, notesCount; 
        t_jrgba color1, color2, color3;
        long    markedNotePosition  = -1;
        long    markedNotePitch     = -1;
        long    markedNoteVelocity  = -1;
        long    markedNoteDuration  = -1;
        
        if (!(x->flags & FLAG_FOCUS)) {
            switch (x->sequenceMode) {
                case MODE_SEQUENCE_USER   : jrgba_copy (&color3, &x->unfocusedUserSelectedNoteColor);
                                            jrgba_copy (&color2, &x->unfocusedUserSelectedNoteColor);
                                            jrgba_copy (&color1, &x->unfocusedUserNoteColor); break;
                case MODE_SEQUENCE_LIVE   : jrgba_copy (&color1, &x->unfocusedLiveNoteColor); break;
            }
        } else {
            switch (x->sequenceMode) {
                case MODE_SEQUENCE_USER   : jrgba_copy (&color3, &x->focusedUserMarkedNoteColor);
                                            jrgba_copy (&color2, &x->focusedUserSelectedNoteColor); break;
                case MODE_SEQUENCE_LIVE   : jrgba_copy (&color2, &x->focusedLivePlayedNoteColor);
                                            jrgba_copy (&color1, &x->focusedLiveNoteColor); break;
            }
        }

        notesCount = pizGrowingArrayCount (x->unselectedCopy) / PIZ_DATA_NOTE_SIZE;
        
        if ((x->flags & FLAG_FOCUS) && USER) {
            for (i = 0; i < notesCount; i++) {
                long position   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_POSITION);
                long pitch      = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_PITCH);
                long velocity   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_VELOCITY);
                long duration   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_DURATION);
                
                tralala_paintNoteCandycane (x, g, position, pitch, velocity, duration);
            }
        } else {
            for (i = 0; i < notesCount; i++) {
                long position   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_POSITION);
                long pitch      = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_PITCH);
                long velocity   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_VELOCITY);
                long duration   = pizGrowingArrayValueAtIndex (x->unselectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_DURATION);
                                            
                tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color1);
            }
        }
        
        notesCount = pizGrowingArrayCount (x->selectedCopy) / PIZ_DATA_NOTE_SIZE;
        
        for (i = 0; i < notesCount; i++) {
            long position   = pizGrowingArrayValueAtIndex (x->selectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_POSITION);
            long pitch      = pizGrowingArrayValueAtIndex (x->selectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_PITCH);
            long velocity   = pizGrowingArrayValueAtIndex (x->selectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_VELOCITY);
            long duration   = pizGrowingArrayValueAtIndex (x->selectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_DURATION);
            long isMarked   = pizGrowingArrayValueAtIndex (x->selectedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_IS_MARKED);
            
            if (!isMarked) {
                tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color2);
            } else {
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
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
    }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * GUI_PIXELS_PER_STEP * f, (PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE * f);
    
    if (g) {
        long    i, notesCount; 
        t_jrgba color;
            
        if (x->flags & FLAG_FOCUS) {
            jrgba_copy (&color, &x->focusedLivePlayedNoteColor);
        } else  {
            jrgba_copy (&color, &x->unfocusedLivePlayedNoteColor);
        }

        notesCount = pizGrowingArrayCount (x->playedCopy) / PIZ_DATA_NOTE_SIZE;
        
        for (i = 0; i < notesCount; i++) {
            long position   = pizGrowingArrayValueAtIndex (x->playedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_POSITION);
            long pitch      = pizGrowingArrayValueAtIndex (x->playedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_PITCH);
            long velocity   = pizGrowingArrayValueAtIndex (x->playedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_VELOCITY);
            long duration   = pizGrowingArrayValueAtIndex (x->playedCopy, (PIZ_DATA_NOTE_SIZE * i) 
                                + PIZ_DATA_DURATION);
            
            tralala_paintNoteWithColor (x, g, position, pitch, velocity, duration, &color);
        }
        
        jbox_end_layer ((t_object*)x, patcherview, tll_sym_playedNotesLayer);
    }
        
  jbox_paint_layer ((t_object *)x, patcherview, tll_sym_playedNotesLayer, -x->windowOffsetX, -x->windowOffsetY);
}

void tralala_paintNoteCandycane (t_tralala *x, t_jgraphics *g, long position, long pitch, long velocity, 
long duration)
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

    alpha = (double)(velocity + GUI_VELOCITY_OFFSET) / (double)PIZ_MAGIC_VELOCITY;
    
    if (alpha > 0.005) {
        atom_setfloat   (temp + 3, CLAMP (alpha, 0.25, 1.));
        atoms_to_jrgba  (4, temp, &color);
    } else {
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
    
    alpha = (double)velocity / (double)PIZ_MAGIC_VELOCITY;
    
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
        case MODE_ZOOM_A    : f = 0.5;  break;
        case MODE_ZOOM_B    : f = 1.;   break;
        case MODE_ZOOM_C    : f = 2.;   break;
    }
    
    g = jbox_start_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, PIZ_SEQUENCE_TIMELINE_SIZE 
        * GUI_PIXELS_PER_STEP * f, (PIZ_MAGIC_PITCH + 1) * GUI_PIXELS_PER_SEMITONE * f);
    
    if (g) {
        if (pizGrowingArrayCount (x->zoneCopy)) {
            t_rect  zoneRect;
            long    start   = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_START);
            long    end     = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_END);
            long    down    = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_DOWN);
            long    up      = pizGrowingArrayValueAtIndex (x->zoneCopy, PIZ_DATA_UP);
            
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
        
    jbox_paint_layer ((t_object *)x, patcherview, tll_sym_zoneLayer, -x->windowOffsetX, -x->windowOffsetY);
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

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
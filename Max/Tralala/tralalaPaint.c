/*
 *  tralalaPaint.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaPaint.h"
#include "tralalaMouse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_paintRun                 (t_tll *x, t_object *pv);
PIZ_LOCAL void tralala_paintLasso               (t_tll *x, t_object *pv);
PIZ_LOCAL void tralala_paintCurrent             (t_tll *x, t_object *pv);
PIZ_LOCAL void tralala_paintBackground          (t_tll *x, t_object *pv);

PIZ_LOCAL void tralala_paintCurrentText         (t_tll *x, t_object *pv, char *string);
PIZ_LOCAL void tralala_paintCurrentZone         (t_tll *x, t_object *pv, long argc, t_atom *argv, long status);
PIZ_LOCAL void tralala_paintCurrentNotes        (t_tll *x, t_object *pv, t_atomarray **notes);

PIZ_LOCAL void tralala_paintStrncatZone         (char *dst, long argc, t_atom *argv, long status);
PIZ_LOCAL void tralala_paintStrncatNote         (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_paintStrncatAttribute    (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_paintPitchAsString       (char *dst, long k, long size);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_STRING_SIZE 128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_paint(t_tll *x, t_object *pv)
{
    tralala_paintBackground(x, pv);
    tralala_paintCurrent(x, pv);
    tralala_paintRun(x, pv);
    tralala_paintLasso(x, pv);
}

void tralala_params(t_tll *x, t_object *pv, t_jboxdrawparams *params)
{
    if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
        jrgba_copy(&params->d_bordercolor, &x->border);
        jrgba_copy(&params->d_boxfillcolor, &x->background);
    } else {
        jrgba_copy(&params->d_bordercolor, &x->uBorder);
        jrgba_copy(&params->d_boxfillcolor, &x->uBackground);
    }
}

void tralala_focusgained(t_tll *x, t_object *pv)
{
	TLL_FLAG_SET(TLL_FLAG_FOCUS)
    
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_RUN);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_BACKGROUND);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    
    jbox_redraw((t_jbox *)x);
}

void tralala_focuslost(t_tll *x, t_object *pv)
{
	TLL_FLAG_UNSET(TLL_FLAG_FOCUS)
    
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_RUN);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_BACKGROUND);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    
    jbox_redraw((t_jbox *)x);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err tralala_notify (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_symbol *name = NULL;
    
    if (msg == TLL_SYM_ATTR_MODIFIED && (name = (t_symbol *)object_method(data, TLL_SYM_GETNAME))) {
    //
    if ((name == TLL_SYM_COLOR) || (name == TLL_SYM_UCOLOR)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_BACKGROUND);
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
        
    } else if ((name == TLL_SYM_HCOLOR1) || (name == TLL_SYM_HCOLOR2)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
        
    } else if (name == TLL_SYM_LASSOCOLOR) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
    }
    
    jbox_redraw((t_jbox *)x);
    //
    }
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_paintRun(t_tll *x, t_object *pv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    PIZEvent *event = NULL;
    PIZEvent *nextEvent = NULL;
    
    pizArrayClear(x->temp);
    
    TLL_RUN_LOCK
    
    pizLinklistPtrAtIndex(x->run, 0, (void **)&event);
    
    while (event) {
    //
    long argc;
    long *argv = NULL;

    pizLinklistNextWithPtr(x->run, (void *)event, (void **)&nextEvent);
    
    pizEventData(event, &argc, &argv);
    
    pizArrayAppend(x->temp, argv[0]);
    pizArrayAppend(x->temp, argv[1]);
    pizArrayAppend(x->temp, argv[2]);
    pizArrayAppend(x->temp, argv[3]);
        
    event = nextEvent;
    //
    }
    
    TLL_RUN_UNLOCK
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_RUN, w, h)) {
    //
    long j;
    long *note = pizArrayPtr(x->temp);
    
    for (j = 0; j < pizArrayCount(x->temp); j += 4) {
        t_rect r;
        r.x = TLL_POSITION_TO_X(note[j]);
        r.y = TLL_PITCH_TO_Y_UP(note[j + 1]); 
        r.width  = TLL_POSITION_TO_X(note[j] + note[j + 3]) - r.x; 
        r.height = TLL_PITCH_TO_Y_DOWN(note[j + 1]) - r.y;
    
        if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
            jgraphics_set_source_jrgba(g, &x->hColor3);
        } else {
            jgraphics_set_source_jrgba(g, &x->hColor4);
        }
        
        jgraphics_rectangle_fill_fast(g, r.x, r.y, r.width, r.height);
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_RUN);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_RUN, -x->offsetX, -x->offsetY);
}

void tralala_paintLasso(t_tll *x, t_object *pv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;

    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_LASSO, w, h)) {
        t_rect r;
        
        r.x = MIN(x->origin.x, x->cursor.x);
        r.y = MIN(x->origin.y, x->cursor.y);
        r.width = MAX(x->origin.x, x->cursor.x) - r.x;
        r.height = MAX(x->origin.y, x->cursor.y) - r.y;
        
        if (TLL_FLAG_TRUE(TLL_FLAG_LASSO)) {
            jgraphics_set_source_jrgba(g, &x->lasso);
            jgraphics_rectangle_fill_fast(g, TLL_X_OFFSET(r.x), TLL_Y_OFFSET(r.y), r.width, r.height);
        }
        
        jbox_end_layer((t_object*)x, pv, TLL_SYM_LASSO);
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_LASSO, -x->offsetX, -x->offsetY);
}

void tralala_paintCurrent(t_tll *x, t_object *pv)
{
    long zoneStatus = 0;
    long i, argc, n = 0;
    t_atom *argv = NULL;
    t_symbol *mark = NULL;
    t_symbol **keys = NULL;
    t_atomarray *notes[3];
    PIZError err = PIZ_GOOD;
    char string[TLL_STRING_SIZE] = "";
    
    t_symbol *s[ ] = { 
        TLL_SYM_BPM, 
        TLL_SYM_CHANCE, 
        TLL_SYM_VELOCITY, 
        TLL_SYM_CHANNEL, 
        TLL_SYM_CHORD,
        TLL_SYM_CELL, 
        TLL_SYM_VALUE, 
        TLL_SYM_SCALE, 
        };

    err |= !(notes[0] = atomarray_new(0, NULL));
    err |= !(notes[1] = atomarray_new(0, NULL));
    err |= !(notes[2] = atomarray_new(0, NULL));
    
    TLL_LOCK
    
    dictionary_getlong(x->status, TLL_SYM_ZONE, &zoneStatus);
        
    for (i = 0; i < 9; i++) {
        if (!(dictionary_getatoms(x->current, s[i], &argc, &argv)) && (argc > 1)) {
            tralala_paintStrncatAttribute(string, argc, argv);
        }
    }
    
    strncat_zero(string, "\n", TLL_STRING_SIZE);
    
    if (dictionary_hasentry(x->status, TLL_SYM_ZONE)) {
        if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
            tralala_paintStrncatZone(string, argc, argv, zoneStatus);
        } 
    } else if (!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) 
        && (!(dictionary_getatoms(x->current, mark, &argc, &argv)))) {
        tralala_paintStrncatNote(string, argc, argv);
    }
    
    if (!err && !(dictionary_getkeys(x->current, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long k, status = 0; 
    t_symbol *key = (*(keys + i));
    
    if (!(dictionary_getatoms(x->current, key, &argc, &argv)) && (atom_getsym(argv) == TLL_SYM_NOTE)) { 
        if (k = dictionary_hasentry(x->status, key)) {
           dictionary_getlong(x->status, key, &status);
        }
        if (!k || (status == TLL_LASSO_UNSELECTED)) {
            atomarray_appendatoms(notes[0], argc, argv);
        } else if (key != mark) {
            atomarray_appendatoms(notes[1], argc, argv);
        } else {
            atomarray_appendatoms(notes[2], argc, argv);
        }
    }
    //
    }

    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    err |= (dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv)) != MAX_ERR_NONE;
    
    TLL_UNLOCK 
    
    if (!err) {
        if (!zoneStatus) {
            tralala_paintCurrentZone(x, pv, argc, argv, zoneStatus);
            tralala_paintCurrentNotes(x, pv, notes);
        } else {
            tralala_paintCurrentNotes(x, pv, notes);
            tralala_paintCurrentZone(x, pv, argc, argv, zoneStatus);
        }
        
        if (x->viewText) {
            tralala_paintCurrentText(x, pv, string);
        }
    }
    
    object_free(notes[0]);
    object_free(notes[1]);
    object_free(notes[2]);
}


void tralala_paintBackground(t_tll *x, t_object *pv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;

    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, w, h)) {
        if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
            jgraphics_set_source_jrgba(g, &x->color);
        } else {
            jgraphics_set_source_jrgba(g, &x->uColor);
        }
        
        jgraphics_rectangle_draw_fast(g, 0., 0., w, h, 1.);
        jbox_end_layer((t_object*)x, pv, TLL_SYM_BACKGROUND);
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintCurrentText(t_tll *x, t_object *pv, char *string)
{
    t_rect r;
    t_jgraphics *g = NULL; 
    t_jfont *font = NULL;
    t_jgraphics_text_justification justification = JGRAPHICS_TEXT_JUSTIFICATION_RIGHT;
    
    jbox_get_rect_for_view((t_object *)x, pv, &r);
    
    g = (t_jgraphics *)patcherview_get_jgraphics(pv);
    font = jfont_create((jbox_get_fontname(((t_object *)x)))->s_name, 
                (jbox_get_font_slant((t_object *)x)), 
                (jbox_get_font_weight((t_object *)x)), 
                (jbox_get_fontsize((t_object *)x))); 
    
    jtextlayout_set(x->layer, string, font, 5., 5., r.width - 10., r.height - 5., justification, 0L);
    
    if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
        jtextlayout_settextcolor(x->layer, &x->text);
    } else {
        jtextlayout_settextcolor(x->layer, &x->uText);
    }
    
    jtextlayout_draw(x->layer, g);
    
    jfont_destroy(font);
}

void tralala_paintCurrentZone(t_tll *x, t_object *pv, long argc, t_atom *argv, long status)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_ZONE, w, h)) {
    //
    t_rect r;
    long zone[4];
    
    atom_getlong_array(argc - 1, argv + 1, 4, zone);
    
    r.x = TLL_POSITION_TO_X(zone[0]);
    r.y = TLL_PITCH_TO_Y_UP(zone[3]); 
    r.width = TLL_POSITION_TO_X(zone[1]) - r.x;
    r.height = TLL_PITCH_TO_Y_DOWN(zone[2]) - r.y;

    if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
        if (status) {
            jgraphics_set_source_jrgba(g, &x->hColor1);
        } else  {
            jgraphics_set_source_jrgba(g, &x->color);
        }
    } else {
        jgraphics_set_source_jrgba(g, &x->uColor);
    }
    
    jgraphics_rectangle_draw_fast(g, r.x, r.y, r.width, r.height, 1.);
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_ZONE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_ZONE, -x->offsetX, -x->offsetY);
}

void tralala_paintCurrentNotes(t_tll *x, t_object *pv, t_atomarray **notes)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_NOTE, w, h)) {
    //
    long i, argc;
    t_atom *argv = NULL;
    
    for (i = 0; i < 3; i++) {
    //
    if (!atomarray_getatoms(notes[i], &argc, &argv)) {
    //
    long j;
    for (j = 0; j < atomarray_getsize(notes[i]); j += 6) {
        t_rect r;
        long note[4];
        
        atom_getlong_array(4, argv + 1 + j, 4, note);
        
        r.x = TLL_POSITION_TO_X(note[0]);
        r.y = TLL_PITCH_TO_Y_UP(note[1]); 
        r.width  = TLL_POSITION_TO_X(note[0] + note[3]) - r.x; 
        r.height = TLL_PITCH_TO_Y_DOWN(note[1]) - r.y;
    
        if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
            if (i == 0 ) {
                jgraphics_set_source_jrgba(g, &x->color);
            } else if (i == 1) {
                jgraphics_set_source_jrgba(g, &x->hColor1);
            } else {
                jgraphics_set_source_jrgba(g, &x->hColor2);
            }
            
        } else {
            jgraphics_set_source_jrgba(g, &x->uColor);
        }
        
        jgraphics_rectangle_fill_fast(g, r.x, r.y, r.width, r.height);
    }
    //
    }
    //
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_NOTE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_NOTE, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintStrncatZone(char *dst, long argc, t_atom *argv, long status)
{
    char a[4];
    char b[4];
    char temp[32];
  
    tralala_paintPitchAsString(a, atom_getlong(argv + 3), 4);
    tralala_paintPitchAsString(b, atom_getlong(argv + 4), 4);
        
    if (status >= TLL_SELECTED_START) {
    //
    switch (status) {
        case TLL_SELECTED_START : 
            strncat_zero(dst, "Start : ", TLL_STRING_SIZE); 
            snprintf_zero(temp, 32, "%ld\n", atom_getlong(argv + 1));
            break;
            
        case TLL_SELECTED_END : 
            strncat_zero(dst, "End : ", TLL_STRING_SIZE); 
            snprintf_zero(temp, 32, "%ld\n", atom_getlong(argv + 2));
            break;
            
        case TLL_SELECTED_DOWN : 
            strncat_zero(dst, "Down : ", TLL_STRING_SIZE); 
            snprintf_zero(temp, 32, "%s\n", a);
            break;
            
        case TLL_SELECTED_UP : 
            strncat_zero(dst, "Up : ", TLL_STRING_SIZE); 
            snprintf_zero(temp, 32, "%s\n", b);
            break;
    } 
    //
    } else {
        snprintf_zero(temp, 32, "%ld %ld %s %s\n", atom_getlong(argv + 1), atom_getlong(argv + 2), a, b);
    }
    
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_paintStrncatNote(char *dst, long argc, t_atom *argv)
{
    char a[4];
    char temp[32];
    long p = atom_getlong(argv + 1);
    long v = atom_getlong(argv + 3);
    long d = atom_getlong(argv + 4);
    long c = atom_getlong(argv + 5);
    
    tralala_paintPitchAsString(a, atom_getlong(argv + 2), 4);
    snprintf_zero(temp, 32, "%ld %s %ld %ld %ld\n", p, a, v, d, c);
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_paintStrncatAttribute(char *dst, long argc, t_atom *argv)
{
    long k = 0;
    char *p = NULL;
    
    if (!(atom_gettext(argc - 1, argv + 1, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE))) {
        strncat_zero(dst, p, TLL_STRING_SIZE);
        strncat_zero(dst, "\n", TLL_STRING_SIZE);
        sysmem_freeptr(p);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintPitchAsString(char *dst, long k, long size)
{
    long m = k % 12;
    long n = ((long)(k / 12.)) - 2;
    
    switch (m) {
        case 0  : snprintf_zero(dst, size, "%s%ld", "C",  n); break;
        case 1  : snprintf_zero(dst, size, "%s%ld", "C#", n); break;
        case 2  : snprintf_zero(dst, size, "%s%ld", "D",  n); break;
        case 3  : snprintf_zero(dst, size, "%s%ld", "D#", n); break;
        case 4  : snprintf_zero(dst, size, "%s%ld", "E",  n); break;
        case 5  : snprintf_zero(dst, size, "%s%ld", "F",  n); break;
        case 6  : snprintf_zero(dst, size, "%s%ld", "F#", n); break;
        case 7  : snprintf_zero(dst, size, "%s%ld", "G",  n); break;
        case 8  : snprintf_zero(dst, size, "%s%ld", "G#", n); break;
        case 9  : snprintf_zero(dst, size, "%s%ld", "A",  n); break;
        case 10 : snprintf_zero(dst, size, "%s%ld", "A#", n); break;
        case 11 : snprintf_zero(dst, size, "%s%ld", "B",  n); break;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
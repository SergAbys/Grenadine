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

PIZ_STATIC void tralala_paintRun                (t_tll *x, t_object *pv);
PIZ_STATIC void tralala_paintLasso              (t_tll *x, t_object *pv);
PIZ_STATIC void tralala_paintCurrent            (t_tll *x, t_object *pv);
PIZ_STATIC void tralala_paintBackground         (t_tll *x, t_object *pv);

PIZ_STATIC void tralala_paintCurrentNote        (t_tll *x, t_object *pv);
PIZ_STATIC void tralala_paintCurrentText        (t_tll *x, t_object *pv, char *string);
PIZ_STATIC void tralala_paintCurrentZone        (t_tll *x, t_object *pv, long argc, t_atom *argv, long status);

PIZ_STATIC void tralala_paintStrncatZone        (char *dst, long argc, t_atom *argv, long status);
PIZ_STATIC void tralala_paintStrncatNote        (char *dst, long argc, t_atom *argv);
PIZ_STATIC void tralala_paintStrncatPanel       (char *dst, long argc, t_atom *argv);
PIZ_STATIC void tralala_paintPitchAsString      (char *dst, long k, long size);

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
    if (TLL_FLAG_TRUE(TLL_DIRTY_RUN)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_RUN);
        TLL_FLAG_UNSET(TLL_DIRTY_RUN);
    }
    
    if (TLL_FLAG_TRUE(TLL_DIRTY_ZONE)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
        TLL_FLAG_UNSET(TLL_DIRTY_ZONE);
    }
    
    if (TLL_FLAG_TRUE(TLL_DIRTY_NOTE)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
        TLL_FLAG_UNSET(TLL_DIRTY_NOTE);
    }
    
    if (TLL_FLAG_TRUE(TLL_DIRTY_LASSO)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
        TLL_FLAG_UNSET(TLL_DIRTY_LASSO);
    }
    
    if (TLL_FLAG_TRUE(TLL_DIRTY_BACKGROUND)) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_BACKGROUND);
        TLL_FLAG_UNSET(TLL_DIRTY_BACKGROUND);
    }
    
    tralala_paintBackground(x, pv);
    tralala_paintCurrent(x, pv);
    tralala_paintRun(x, pv);
    tralala_paintLasso(x, pv);
}

void tralala_params(t_tll *x, t_object *pv, t_jboxdrawparams *params)
{
    if (TLL_FLAG_TRUE(TLL_FLAG_FOCUS)) {
        jrgba_copy(&params->d_bordercolor, &x->border);
    } else {
        jrgba_copy(&params->d_bordercolor, &x->uBorder);
    }
    
    jrgba_copy(&params->d_boxfillcolor, &x->background);
}

void tralala_focusGained(t_tll *x, t_object *pv)
{
    TLL_FLAG_SET(TLL_FLAG_FOCUS);
}

void tralala_focusLost(t_tll *x, t_object *pv)
{
    TLL_FLAG_UNSET(TLL_FLAG_FOCUS);
    
    tralala_mouseUnselectAll(x);
    TLL_FLAG_SET(TLL_DIRTY_ZONE | TLL_DIRTY_NOTE);
    jbox_redraw((t_jbox *)x);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err tralala_notify (t_jbox *jbox, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_symbol *name = NULL;
    t_tll *x = (t_tll *)jbox;
    
    if (msg == TLL_SYM_ATTR_MODIFIED && (name = (t_symbol *)object_method(data, TLL_SYM_GETNAME))) {
    //
    if (name == TLL_SYM_COLOR) {
        TLL_FLAG_SET(TLL_DIRTY_BACKGROUND | TLL_DIRTY_ZONE | TLL_DIRTY_NOTE);
        
    } else if ((name == TLL_SYM_HCOLOR1) || (name == TLL_SYM_HCOLOR2)) {
        TLL_FLAG_SET(TLL_DIRTY_ZONE | TLL_DIRTY_NOTE);
        
    } else if (name == TLL_SYM_LASSOCOLOR) {
        TLL_FLAG_SET(TLL_DIRTY_LASSO);
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
    
    pizArrayClear(x->temp[0]);
    
    TLL_DAEMON_LOCK
    
    pizLinklistPtrAtIndex(x->daemon, 0, (void **)&event);
    
    while (event) {
    //
    long argc;
    long *argv = NULL;

    pizLinklistNextWithPtr(x->daemon, (void *)event, (void **)&nextEvent);
    
    pizEventData(event, &argc, &argv);
    
    pizArrayAppend(x->temp[0], argv[0]);
    pizArrayAppend(x->temp[0], argv[1]);
    pizArrayAppend(x->temp[0], argv[2]);
    pizArrayAppend(x->temp[0], argv[3]);
        
    event = nextEvent;
    //
    }
    
    TLL_DAEMON_UNLOCK
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_RUN, w, h)) {
    //
    long j;
    long *note = pizArrayPtr(x->temp[0]);
    
    for (j = 0; j < pizArrayCount(x->temp[0]); j += 4) {
        t_rect r;
        r.x = TLL_POSITION_TO_X(note[j]);
        r.y = TLL_PITCH_TO_Y_UP(note[j + 1]); 
        r.width  = TLL_POSITION_TO_X(note[j] + note[j + 3]) - r.x; 
        r.height = TLL_PITCH_TO_Y_DOWN(note[j + 1]) - r.y;
    
        jgraphics_set_source_jrgba(g, &x->hColor3);
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
        TLL_SYM_SCALE
        };
    
    pizArrayClear(x->temp[0]);
    pizArrayClear(x->temp[1]);
    pizArrayClear(x->temp[2]);
    
    TLL_DATA_LOCK
    
    dictionary_getlong(x->status, TLL_SYM_ZONE, &zoneStatus);
    
    for (i = 0; i < 9; i++) {
        if (!(dictionary_getatoms(x->current, s[i], &argc, &argv)) && (argc > 1)) {
            tralala_paintStrncatPanel(string, argc, argv);
        }
    }
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_MUTE, &argc, &argv)) 
        && (argc > 1)
        && (atom_gettype(argv + 1) == A_LONG)
        && (atom_getlong(argv + 1) == 1)) {
        strncat_zero(string, "•\n", TLL_STRING_SIZE);
    } else {
        strncat_zero(string, "\n", TLL_STRING_SIZE);
    }
    
    if (dictionary_hasentry(x->status, TLL_SYM_ZONE)) {
        if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
            tralala_paintStrncatZone(string, argc, argv, zoneStatus);
        } 
    } else if (!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) 
        && (!(dictionary_getatoms(x->current, mark, &argc, &argv)))) {
        tralala_paintStrncatNote(string, argc, argv);
    }
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long k, j, p, status = 0; 
    t_symbol *key = (*(keys + i));
    
    if (!(dictionary_getatoms(x->current, key, &argc, &argv)) 
        && (atom_getsym(argv) == TLL_SYM_NOTE)
        && (argc == 6)) { 
        if (k = dictionary_hasentry(x->status, key)) {
           dictionary_getlong(x->status, key, &status);
        }
        if (!k || (status == TLL_LASSO_UNSELECTED)) {
            p = 0;
        } else if (key != mark) {
            p = 1;
        } else {
            p = 2;
        }
        for (j = 1; j < 5; j++) {
            pizArrayAppend(x->temp[p], atom_getlong(argv + j));
        }
    }
    //
    }

    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    err |= (dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv)) != MAX_ERR_NONE;
    
    TLL_DATA_UNLOCK 
    
    if (!err) {
        if (!zoneStatus) {
            tralala_paintCurrentZone(x, pv, argc, argv, zoneStatus);
            tralala_paintCurrentNote(x, pv);
        } else {
            tralala_paintCurrentNote(x, pv);
            tralala_paintCurrentZone(x, pv, argc, argv, zoneStatus);
        }
        
        if (x->viewText) {
            tralala_paintCurrentText(x, pv, string);
        }
    }
}

void tralala_paintBackground(t_tll *x, t_object *pv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;

    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, w, h)) {
        jgraphics_set_source_jrgba(g, &x->color);
        jgraphics_rectangle_draw_fast(g, 0., 0., w, h, 1.);
        jbox_end_layer((t_object*)x, pv, TLL_SYM_BACKGROUND);
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintCurrentNote(t_tll *x, t_object *pv)
{  
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_NOTE, w, h)) {
    //
    long i;
    for (i = 0; i < 3; i++) {
    //
    long j;
    long *note = pizArrayPtr(x->temp[i]);
    
    for (j = 0; j < pizArrayCount(x->temp[i]); j += 4) {
        t_rect r;
        r.x = TLL_POSITION_TO_X(note[j + 0]);
        r.y = TLL_PITCH_TO_Y_UP(note[j + 1]); 
        r.width  = TLL_POSITION_TO_X(note[j + 0] + note[j + 3]) - r.x; 
        r.height = TLL_PITCH_TO_Y_DOWN(note[j + 1]) - r.y;
    
        if (i == 0 ) {
            jgraphics_set_source_jrgba(g, &x->color);
        } else if (i == 1) {
            jgraphics_set_source_jrgba(g, &x->hColor1);
        } else {
            jgraphics_set_source_jrgba(g, &x->hColor2);
        }

        
        jgraphics_rectangle_fill_fast(g, r.x, r.y, r.width, r.height);
    }
    //
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_NOTE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_NOTE, -x->offsetX, -x->offsetY);
}

void tralala_paintCurrentText(t_tll *x, t_object *pv, char *string)
{
    t_rect r;
    t_jgraphics *g = NULL; 
    t_jfont *font = NULL;
    t_jgraphics_textlayout_flags flags = (t_jgraphics_textlayout_flags) (0);
    t_jgraphics_text_justification justification = JGRAPHICS_TEXT_JUSTIFICATION_RIGHT;
    
    jbox_get_rect_for_view((t_object *)x, pv, &r);
    
    g = (t_jgraphics *)patcherview_get_jgraphics(pv);
    font = jfont_create((jbox_get_fontname(((t_object *)x)))->s_name, 
                (t_jgraphics_font_slant)(jbox_get_font_slant((t_object *)x)), 
                (t_jgraphics_font_weight)(jbox_get_font_weight((t_object *)x)), 
                (jbox_get_fontsize((t_object *)x))); 
    
    jtextlayout_set(x->layer, string, font, 5., 5., r.width - 10., r.height - 5., justification, flags);

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

    if (status) {
        jgraphics_set_source_jrgba(g, &x->hColor2);
    } else  {
        jgraphics_set_source_jrgba(g, &x->color);
    }
      
    jgraphics_rectangle_draw_fast(g, r.x, r.y, r.width, r.height, 1.);
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_ZONE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_ZONE, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintStrncatZone(char *dst, long argc, t_atom *argv, long status)
{
    char a[5];
    char b[5];
    char temp[32];
  
    tralala_paintPitchAsString(a, atom_getlong(argv + 3), 5);
    tralala_paintPitchAsString(b, atom_getlong(argv + 4), 5);
        
    if (status >= TLL_SELECTED_START) {
    //
    switch (status) {
        case TLL_SELECTED_START : snprintf_zero(temp, 32, "Start : %ld\n", atom_getlong(argv + 1)); break;
        case TLL_SELECTED_END   : snprintf_zero(temp, 32, "End : %ld\n", atom_getlong(argv + 2)); break;
        case TLL_SELECTED_DOWN  : snprintf_zero(temp, 32, "Down : %s\n", a); break;
        case TLL_SELECTED_UP    : snprintf_zero(temp, 32, "Up : %s\n", b); break;
    } 
    //
    } else {
        snprintf_zero(temp, 32, "%ld %ld %s %s\n", atom_getlong(argv + 1), atom_getlong(argv + 2), a, b);
    }
    
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_paintStrncatNote(char *dst, long argc, t_atom *argv)
{
    char a[5];
    char temp[32];
    long p = atom_getlong(argv + 1);
    long v = atom_getlong(argv + 3);
    long d = atom_getlong(argv + 4);
    long c = atom_getlong(argv + 5);
    
    tralala_paintPitchAsString(a, atom_getlong(argv + 2), 5);
    snprintf_zero(temp, 32, "%ld %s %ld %ld %ld\n", p, a, v, d, c);
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_paintStrncatPanel(char *dst, long argc, t_atom *argv)
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
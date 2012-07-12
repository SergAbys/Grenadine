/*
 *  tralalaPaint.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaPaint.h"
#include "tralalaUser.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_paintBackground      (t_tll *x, t_object *pv);
PIZ_LOCAL void tralala_paintDictionary      (t_tll *x, t_object *pv);

PIZ_LOCAL void tralala_paintText            (t_tll *x, t_object *pv, char *string);
PIZ_LOCAL void tralala_paintZone            (t_tll *x, t_object *pv, long argc, t_atom *argv, long status);
PIZ_LOCAL void tralala_paintNote            (t_tll *x, t_object *pv, t_atomarray **notes);

PIZ_LOCAL void tralala_strncatZone          (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_strncatNote          (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_strncatAttribute     (char *dst, long argc, t_atom *argv);

PIZ_LOCAL void tralala_pitchAsString        (char *s, long k, long size);

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
    post("!");
    tralala_paintBackground(x, pv);
    tralala_paintDictionary(x, pv);
}

void tralala_params(t_tll *x, t_object *pv, t_jboxdrawparams *params)
{
    jrgba_copy(&params->d_boxfillcolor, &x->background);
    jrgba_copy(&params->d_bordercolor, &x->border);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err tralala_notify (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_symbol *name = NULL;
    
    if (msg == TLL_SYM_ATTR_MODIFIED && (name = (t_symbol *)object_method(data, TLL_SYM_GETNAME))) {
    //
    if (name == TLL_SYM_COLOR) {
        TLL_DIRTY_BACKGROUND
        TLL_DIRTY_ZONE
        TLL_DIRTY_NOTE
        
    } else if ((name == TLL_SYM_HCOLOR1) && (name == TLL_SYM_HCOLOR2)) {
        TLL_DIRTY_ZONE
        TLL_DIRTY_NOTE
    }
    
    jbox_redraw(x);
    //
    }
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

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

void tralala_paintDictionary(t_tll *x, t_object *pv)
{
    long status = 0;
    long i, argc, n = 0;
    t_atom *argv = NULL;
    t_symbol *last = NULL;
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
        TLL_SYM_PATTERN 
        };

    err |= !(notes[0] = atomarray_new(0, NULL));
    err |= !(notes[1] = atomarray_new(0, NULL));
    err |= !(notes[2] = atomarray_new(0, NULL));
    
    TLL_LOCK
            
    for (i = 0; i < 9; i++) {
        if (!(dictionary_getatoms(x->current, s[i], &argc, &argv))) {
            tralala_strncatAttribute(string, argc, argv);
        }
    }
    
    if (dictionary_hasentry(x->status, TLL_SYM_ZONE)) {
        if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
            tralala_strncatZone(string, argc, argv);
        } 
    }
    
    if (!(dictionary_getsym(x->status, TLL_SYM_LAST, &last)) 
        && (!(dictionary_getatoms(x->current, last, &argc, &argv)))) {
        tralala_strncatNote(string, argc, argv);
    }
    
    if (!err && !(dictionary_getkeys(x->current, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
        if (!(dictionary_getatoms(x->current, (*(keys + i)), &argc, &argv))) { 
        //
        if ((atom_getsym(argv) == TLL_SYM_NOTE)) {
            if (!(dictionary_hasentry(x->status, (*(keys + i))))) {
                atomarray_appendatoms(notes[0], argc, argv);
            } else if ((*(keys + i)) != last) {
                atomarray_appendatoms(notes[1], argc, argv);
            } else {
                atomarray_appendatoms(notes[2], argc, argv);
            }
        }
        //
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    dictionary_getlong(x->status, TLL_SYM_ZONE, &status);
    err |= (dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv)) != MAX_ERR_NONE;
    
    TLL_UNLOCK 
    
    if (!err) {
        tralala_paintNote(x, pv, notes);
        tralala_paintZone(x, pv, argc, argv, status);
        
        if (x->viewText) {
            tralala_paintText(x, pv, string);
        }
    }
    
    object_free(notes[0]);
    object_free(notes[1]);
    object_free(notes[2]);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintText(t_tll *x, t_object *pv, char *string)
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
    jtextlayout_settextcolor(x->layer, &x->text);    
    jtextlayout_draw(x->layer, g);
    
    jfont_destroy(font);
}

void tralala_paintZone(t_tll *x, t_object *pv, long argc, t_atom *argv, long status)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_ZONE, w, h)) {
    //
    t_rect r; 
    long zone[4];
    
    atom_getlong_array(argc - 1, argv + 1, 4, zone);
    
    if (status == TLL_SELECTED) {
        jgraphics_set_source_jrgba(g, &x->hcolor2);
    } else {
        jgraphics_set_source_jrgba(g, &x->color);
    }
    
    r.x = POSITION_TO_X(zone[0]);
    r.y = PITCH_TO_Y_UP(zone[3]); 
    r.width  = POSITION_TO_X(zone[1]) - r.x; 
    r.height = PITCH_TO_Y_DOWN(zone[2]) - r.y;
        
    jgraphics_rectangle_draw_fast(g, r.x, r.y, r.width, r.height, 1.);
    
    if (status > TLL_SELECTED) {
        switch (status) {
            case TLL_SELECTED_START : r.x = POSITION_TO_X(zone[0]); r.width = 3.; break ;
            case TLL_SELECTED_END   : r.x = POSITION_TO_X(zone[1]) - 3.; r.width = 3.; break ;
            case TLL_SELECTED_DOWN  : r.y = PITCH_TO_Y_DOWN(zone[2]) - 3.; r.height = 3.; break;
            case TLL_SELECTED_UP    : r.height = 3.; break;
        }
        
        jgraphics_set_source_jrgba(g, &x->hcolor2);
        jgraphics_rectangle_draw_fast(g, r.x, r.y, r.width, r.height, 1.);
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_ZONE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_ZONE, -x->offsetX, -x->offsetY);
}

void tralala_paintNote(t_tll *x, t_object *pv, t_atomarray **notes)
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
        long note[5];
        
        atom_getlong_array(5, argv + 1 + j, 5, note);
        
        r.x = POSITION_TO_X(note[0]);
        r.y = PITCH_TO_Y_UP(note[1]); 
        r.width  = POSITION_TO_X(note[0] + note[3]) - r.x; 
        r.height = PITCH_TO_Y_DOWN(note[1]) - r.y;
    
        if (i == 0) {
            jgraphics_set_source_jrgba(g, &x->color);
        } else if (i == 1) {
            jgraphics_set_source_jrgba(g, &x->hcolor1);
        } else {
            jgraphics_set_source_jrgba(g, &x->hcolor2);
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

void tralala_strncatZone(char *dst, long argc, t_atom *argv)
{
    char a[4];
    char b[4];
    char temp[32];
    
    tralala_pitchAsString(a, atom_getlong(argv + 3), 4);
    tralala_pitchAsString(b, atom_getlong(argv + 4), 4);
    snprintf_zero(temp, 32, "%ld %ld %s %s\n", atom_getlong(argv + 1), atom_getlong(argv + 2), a, b);
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_strncatNote(char *dst, long argc, t_atom *argv)
{
    char a[4];
    char temp[32];
    long p = atom_getlong(argv + 1);
    long v = atom_getlong(argv + 3);
    long d = atom_getlong(argv + 4);
    long c = atom_getlong(argv + 5);
    
    tralala_pitchAsString(a, atom_getlong(argv + 2), 4);
    snprintf_zero(temp, 32, "%ld %s %ld %ld %ld\n", p, a, v, d, c);
    strncat_zero(dst, temp, TLL_STRING_SIZE);
}

void tralala_strncatAttribute(char *dst, long argc, t_atom *argv)
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

void tralala_pitchAsString(char *s, long k, long size)
{
    long m = k % 12;
    long n = ((long)(k / 12.)) - 2;
    
    switch (m) {
        case 0  : snprintf_zero(s, size, "%s%ld", "C",  n); break;
        case 1  : snprintf_zero(s, size, "%s%ld", "C#", n); break;
        case 2  : snprintf_zero(s, size, "%s%ld", "D",  n); break;
        case 3  : snprintf_zero(s, size, "%s%ld", "D#", n); break;
        case 4  : snprintf_zero(s, size, "%s%ld", "E",  n); break;
        case 5  : snprintf_zero(s, size, "%s%ld", "F",  n); break;
        case 6  : snprintf_zero(s, size, "%s%ld", "F#", n); break;
        case 7  : snprintf_zero(s, size, "%s%ld", "G",  n); break;
        case 8  : snprintf_zero(s, size, "%s%ld", "G#", n); break;
        case 9  : snprintf_zero(s, size, "%s%ld", "A",  n); break;
        case 10 : snprintf_zero(s, size, "%s%ld", "A#", n); break;
        case 11 : snprintf_zero(s, size, "%s%ld", "B",  n); break;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
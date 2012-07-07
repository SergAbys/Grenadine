/*
 *  tralalaPaint.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaPaint.h"
#include "tralalaParse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_PIXELS_PER_STEP     2.
#define TLL_PIXELS_PER_SEMITONE 24.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralala_paintBackground        (t_tll *x, t_object *pv);
void tralala_paintDictionary        (t_tll *x, t_object *pv);

void tralala_paintDictionaryZone    (t_tll *x, t_object *pv, t_symbol *key, long argc, t_atom *argv);
void tralala_paintDictionaryNote    (t_tll *x, t_object *pv, t_symbol *key, long argc, t_atom *argv);

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

void tralala_getdrawparams(t_tll *x, t_object *pv, t_jboxdrawparams *params)
{
    jrgba_copy(&params->d_boxfillcolor, &x->background);
    jrgba_copy(&params->d_bordercolor, &x->border);
}

void tralala_key(t_tll *x, t_object *pv, long keycode, long m, long textcharacter)
{
	post("Key : %ld %ld %ld", keycode, m, textcharacter);
}

void tralala_mousewheel(t_tll *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc)
{
    long h = object_attr_getlong(x, TLL_SYM_XOFFSET) - (x_inc * 100);
    long v = object_attr_getlong(x, TLL_SYM_YOFFSET) - (y_inc * 100);
        
    object_attr_setlong(x, TLL_SYM_XOFFSET, h);
    object_attr_setlong(x, TLL_SYM_YOFFSET, v);
}


t_max_err jbox_notify (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_symbol *name = NULL;
    
    if (msg == TLL_SYM_ATTR_MODIFIED && (name = (t_symbol *)object_method(data, TLL_SYM_GETNAME))) {
        if ((name != TLL_SYM_PATCHING_RECT) && object_attr_usercanset((t_object *)x, name)) {
            if (name == TLL_SYM_COLOR) {
                jbox_invalidate_layer((t_object*)x, NULL, TLL_SYM_BACKGROUND);
            }
            
            jbox_redraw(x);
        }
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
    long i, n = 0;
    t_symbol **keys = NULL;
    t_dictionary *d = x->current;
    
    if (!(dictionary_getkeys(d, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long k;
    t_atom *data = NULL;
    
    if (!dictionary_getatoms(d, (*(keys + i)), &k, &data)) {
        if (atom_gettype(data) == A_SYM) {
            if (atom_getsym(data) == TLL_SYM_NOTE) {
                tralala_paintDictionaryNote(x, pv, *(keys + i), k - 1, data + 1);
            } else if (atom_getsym(data) == TLL_SYM_ZONE) {
                tralala_paintDictionaryZone(x, pv, *(keys + i), k - 1, data + 1);
            }
        }
    }
    //
    }
    
    dictionary_freekeys(d, n, keys);
    //
    }
}

void tralala_paintDictionaryZone(t_tll *x, t_object *pv, t_symbol *key, long argc, t_atom *argv)
{
    post("%s", key->s_name);
}

void tralala_paintDictionaryNote(t_tll *x, t_object *pv, t_symbol *key, long argc, t_atom *argv)
{
    post("%s", key->s_name);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
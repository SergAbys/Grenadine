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

#define TLL_PIXELS_PER_STEP         2.
#define TLL_PIXELS_PER_SEMITONE     24.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralala_paintBackground    (t_tll *x, t_object *patcherview);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -
    
void tralala_paint(t_tll *x, t_object *patcherview)
{
    tralala_paintBackground(x, patcherview);
}

void tralala_getdrawparams(t_tll *x, t_object *patcherview, t_jboxdrawparams *params)
{
    jrgba_copy(&params->d_boxfillcolor, &x->background);
    jrgba_copy(&params->d_bordercolor, &x->border);
}

void tralala_key(t_tll *x, t_object *patcherview, long keycode, long m, long textcharacter)
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

void tralala_paintBackground(t_tll *x, t_object *patcherview)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;

    if (g = jbox_start_layer((t_object *)x, patcherview, TLL_SYM_BACKGROUND, w, h)) {
        jgraphics_set_source_jrgba(g, &x->color);
        jgraphics_rectangle_draw_fast(g, 0., 0., w, h, 1.);
        jbox_end_layer((t_object*)x, patcherview, TLL_SYM_BACKGROUND);
    }
        
    jbox_paint_layer((t_object *)x, patcherview, TLL_SYM_BACKGROUND, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
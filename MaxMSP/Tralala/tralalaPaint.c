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

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

//jbox_invalidate_layer((t_object*)x, NULL, TLL_SYM_NOTE);
//jbox_redraw((t_jbox *)x);
    
void tralala_paint(t_tll *x, t_object *patcherview)
{
    ;
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
    long h = object_attr_getlong(x, TLL_SYM_X_OFFSET) - (x_inc * 100);
    long v = object_attr_getlong(x, TLL_SYM_Y_OFFSET) - (y_inc * 100);
    
    object_attr_setlong(x, TLL_SYM_X_OFFSET, h);
    object_attr_setlong(x, TLL_SYM_Y_OFFSET, v);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
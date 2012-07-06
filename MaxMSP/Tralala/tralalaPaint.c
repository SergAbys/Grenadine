/*
 *  tralalaPaint.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

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
	//post("Key : %ld %ld %ld", keycode, m, textcharacter);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 *  tralalaPaint.h
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef TLL_PAINT_H
#define TLL_PAINT_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"
#include "tralalaParse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_PIXELS_PER_STEP         1.
#define TLL_PIXELS_PER_SEMITONE     12.

#define TLL_DIRTY_ZONE              jbox_invalidate_layer((t_object*)x, NULL, TLL_SYM_ZONE);
#define TLL_DIRTY_NOTE              jbox_invalidate_layer((t_object*)x, NULL, TLL_SYM_NOTE);
#define TLL_DIRTY_BACKGROUND        jbox_invalidate_layer((t_object*)x, NULL, TLL_SYM_BACKGROUND);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define POSITION_TO_X(a)            ((a) * TLL_PIXELS_PER_STEP)
#define PITCH_TO_Y_UP(a)            ((PIZ_MAGIC_PITCH - (a)) * TLL_PIXELS_PER_SEMITONE)
#define PITCH_TO_Y_DOWN(a)          ((PIZ_MAGIC_PITCH - (a) + 1) * TLL_PIXELS_PER_SEMITONE)
#define Y_TO_PITCH(a)               (PIZ_MAGIC_PITCH - ((long)((x->offsetY + (a)) / TLL_PIXELS_PER_SEMITONE)))
#define X_TO_POSITION(a)            ((long)((x->offsetX + (a)) / TLL_PIXELS_PER_STEP))

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paint          (t_tll *x, t_object *pv);
void tralala_params         (t_tll *x, t_object *pv, t_jboxdrawparams *params);

t_max_err tralala_notify    (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_PAINT_H
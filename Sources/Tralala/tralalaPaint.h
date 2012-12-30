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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_POSITION_TO_X(a)        ((a) * TLL_PIXELS_PER_STEP)
#define TLL_PITCH_TO_Y_UP(a)        ((PIZ_MAGIC_PITCH - (a)) * TLL_PIXELS_PER_SEMITONE)
#define TLL_PITCH_TO_Y_DOWN(a)      ((PIZ_MAGIC_PITCH - (a) + 1) * TLL_PIXELS_PER_SEMITONE)
#define TLL_Y_TO_PITCH(a)           (PIZ_MAGIC_PITCH - ((long)((x->offsetY + (a)) / TLL_PIXELS_PER_SEMITONE)))
#define TLL_X_TO_POSITION(a)        ((long)((x->offsetX + (a)) / TLL_PIXELS_PER_STEP))
#define TLL_Y_OFFSET(a)             ((a) + x->offsetY)
#define TLL_X_OFFSET(a)             ((a) + x->offsetX)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void        tralala_paint           (t_tll *x, t_object *pv);
void        tralala_params          (t_tll *x, t_object *pv, t_jboxdrawparams *params);
void        tralala_focusGained     (t_tll *x, t_object *pv);
void        tralala_focusLost       (t_tll *x, t_object *pv);
t_max_err   tralala_notify          (t_jbox *jbox, t_symbol *s, t_symbol *msg, void *sender, void *data);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_PAINT_H
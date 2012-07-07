/*
 *  tralala.h
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef TLL_TRALALA_H
#define TLL_TRALALA_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "ext_atomic.h"
#include "jpatcher_api.h"
#include "jgraphics.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tll {
    t_jbox          box;						
    t_atom          played[4];
    t_atom          dumped[5];
    t_atom          link;
    t_jrgba         text;
    t_jrgba         color;
    t_jrgba         border;
    t_jrgba         background;
    PIZTime         time;
    long            identifier;
    long            offsetX;
    long            offsetY;
    t_dictionary    *data;
    t_dictionary    *current;
    t_dictionary    *selected;
    PIZAgent        *agent;
    void            *left;
    void            *middleLeft;
    void            *middleRight;
    void            *right;
    } t_tll;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new           (t_symbol *s, long argc, t_atom *argv);

void tralala_init           (t_tll *x, t_symbol *s, short argc, t_atom *argv);
void tralala_free           (t_tll *x);

void tralala_assist         (t_tll *x, void *b, long m, long a, char *s);
void tralala_jsave          (t_tll *x, t_dictionary *d);
void tralala_paint          (t_tll *x, t_object *pv);
void tralala_params         (t_tll *x, t_object *pv, t_jboxdrawparams *params);
void tralala_key            (t_tll *x, t_object *pv, long keycode, long m, long textcharacter);
void tralala_mousewheel     (t_tll *x, t_object *view, t_pt pt, long modifiers, double x_inc, double y_inc);

t_max_err tralala_notify    (t_jbox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void tralala_store          (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_restore        (t_tll *x, t_symbol *s, long argc, t_atom *argv);

void tralala_callback       (void *ptr, PIZEvent *event);

void tralala_play           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_loop           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_stop           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_unloop         (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_list           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything       (t_tll *x, t_symbol *s, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_H
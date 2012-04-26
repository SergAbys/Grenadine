/*
 *  tralala.h
 *
 *  Created : 17/05/12.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  April 22, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef PIZ_TRALALA_H
#define PIZ_TRALALA_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _tralala {
	t_object    ob;
    PIZAgent    *agent;
	void        *outlet;
	} t_tralala;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *tralala_new       (t_symbol *s, long argc, t_atom *argv);
void tralala_free       (t_tralala *x);
void tralala_assist     (t_tralala *x, void *b, long m, long a, char *s);

void tralala_notify     (void *x, PIZEvent *event);

void tralala_bang       (t_tralala *x);
void tralala_play       (t_tralala *x);
void tralala_stop       (t_tralala *x);
void tralala_loop       (t_tralala *x);
void tralala_unloop     (t_tralala *x);
void tralala_bpm        (t_tralala *x, long n);

void tralala_note       (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_clear      (t_tralala *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_TRALALA_H
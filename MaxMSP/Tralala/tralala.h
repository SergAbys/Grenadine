/*
 *  tralala.h
 *
 *  nicolas.danet@free.fr
 *
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
    t_atom      a[4];
    PIZAgent    *agent;
	void        *leftOutlet;
    void        *rightOutlet;
	} t_tralala;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *tralala_new               (t_symbol *s, long argc, t_atom *argv);

void tralala_free               (t_tralala *x);
void tralala_assist             (t_tralala *x, void *b, long m, long a, char *s);
void tralala_notify             (void *ptr, PIZEvent *event);

void tralala_bang               (t_tralala *x);
void tralala_play               (t_tralala *x);
void tralala_stop               (t_tralala *x);
void tralala_loop               (t_tralala *x);
void tralala_unloop             (t_tralala *x);

void tralala_bpm                (t_tralala *x, long n);
void tralala_chance             (t_tralala *x, long n);
void tralala_velocity           (t_tralala *x, long n);
void tralala_channel            (t_tralala *x, long n);
void tralala_chord              (t_tralala *x, long n);
void tralala_cell               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_scale              (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_pattern            (t_tralala *x, t_symbol *s, long argc, t_atom *argv);

void tralala_note               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_clear              (t_tralala *x);
void tralala_clean              (t_tralala *x, long n);
void tralala_transpose          (t_tralala *x, long n);
void tralala_rotate             (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_scramble           (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_sort               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_change             (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_fill               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_kill               (t_tralala *x);
void tralala_cycle              (t_tralala *x, t_symbol *s, long argc, t_atom *argv);

void tralala_int                (t_tralala *x, long n);
void tralala_zoulou             (t_tralala *x);
void tralala_romeo              (t_tralala *x);
void tralala_november           (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_list               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
                                
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_TRALALA_H
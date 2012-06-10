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
#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tralalaTable {
    t_symbol *sym_note;
    t_symbol *sym_clear;
    t_symbol *sym_tralala;
    t_symbol *sym_current;
    t_symbol *sym_restore;
    } t_tralalaTable;

typedef struct _tralala {
    t_object        ob;
    t_atom          played[4];
    t_atom          dumped[5];
    t_object        *patcher;
    t_dictionary    *data;
    PIZAgent        *agent;
    void            *left;
    void            *middleLeft;
    void            *middleRight;
    void            *right;
    } t_tralala;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new                   (t_symbol *s, long argc, t_atom *argv);

void tralala_init                   (t_tralala *x, t_symbol *s, short argc, t_atom *argv);
void tralala_free                   (t_tralala *x);
void tralala_assist                 (t_tralala *x, void *b, long m, long a, char *s);
void tralala_dictionary             (t_tralala *x, t_dictionary *d);

void tralala_dblclick               (t_tralala *x);
void tralala_notify                 (void *ptr, PIZEvent *event);

void tralala_bang                   (t_tralala *x);
void tralala_play                   (t_tralala *x);
void tralala_stop                   (t_tralala *x);
void tralala_loop                   (t_tralala *x);
void tralala_unloop                 (t_tralala *x);

void tralala_list                   (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_H
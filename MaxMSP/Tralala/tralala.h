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
#include "ext_strings.h"
#include "ext_systhread.h"
#include "jpatcher_api.h"
#include "jgraphics.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_LOCK    systhread_mutex_lock (&x->mutex); 
#define TLL_UNLOCK  systhread_mutex_unlock (&x->mutex); 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tll {
    t_jbox              box;
    t_systhread_mutex   mutex;
    t_atom              played[4];
    t_atom              dumped[5];
    t_atom              link;
    t_jrgba             text;
    t_jrgba             color;
    t_jrgba             hcolor1;
    t_jrgba             hcolor2;
    t_jrgba             border;
    t_jrgba             background;
    PIZTime             time;
    long                identifier;
    long                offsetX;
    long                offsetY;
    long                viewText;
    t_pt                cursor;        
    t_jtextlayout       *layer; 
    t_dictionary        *data;
    t_dictionary        *current;
    t_dictionary        *status;
    PIZAgent            *agent;
    void                *left;
    void                *middleLeft;
    void                *middleRight;
    void                *right;
    } t_tll;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new       (t_symbol *s, long argc, t_atom *argv);

void tralala_init       (t_tll *x, t_symbol *s, short argc, t_atom *argv);
void tralala_free       (t_tll *x);
void tralala_assist     (t_tll *x, void *b, long m, long a, char *s);
void tralala_jsave      (t_tll *x, t_dictionary *d);

void tralala_store      (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_recall     (t_tll *x, t_symbol *s, long argc, t_atom *argv);

void tralala_callback   (void *ptr, PIZEvent *event);

void tralala_play       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_loop       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_stop       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_unloop     (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_list       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything   (t_tll *x, t_symbol *s, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_H
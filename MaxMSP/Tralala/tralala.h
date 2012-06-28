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
#include "tralalaTime.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_FLAG_NONE   0UL
#define TLL_FLAG_INIT   1UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define SEND(code)  PIZEvent *event = NULL;                             \
                    if (event = pizEventNew(code)) {                    \
                        PIZTime time;                                   \
                        pizTimeSet(&time);                              \
                        pizEventSetTime(event, &time);                  \
                        pizEventSetIdentifier(event, x->identifier);    \
                        pizAgentAddEvent(x->agent, event);              \
                    }
                    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tll {
    t_object        ob;	
    ulong           flags;					
    t_atom          played[4];
    t_atom          dumped[5];
    long            identifier;
    t_time          time;
    t_dictionary    *data;
    t_dictionary    *current;
    t_dictionary    *restore;
    PIZAgent        *agent;
    void            *left;
    void            *middleLeft;
    void            *middleRight;
    void            *right;
    } t_tll;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new       (t_symbol *s, long argc, t_atom *argv);

void tralala_init       (t_tll *x, t_symbol *s, short argc, t_atom *argv);
void tralala_free       (t_tll *x);
void tralala_assist     (t_tll *x, void *b, long m, long a, char *s);
void tralala_jsave      (t_tll *x, t_dictionary *d);

void tralala_dblclick   (t_tll *x);

void tralala_callback   (void *ptr, PIZEvent *event);

void tralala_bang       (t_tll *x);
void tralala_play       (t_tll *x);
void tralala_stop       (t_tll *x);
void tralala_loop       (t_tll *x);
void tralala_unloop     (t_tll *x);

void tralala_list       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything   (t_tll *x, t_symbol *s, long argc, t_atom *argv);
     
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_H
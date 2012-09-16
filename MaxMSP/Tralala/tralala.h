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

#include "pizAlgorithms.h"  

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_CLOCK_FOCUS         83.
#define TLL_CLOCK_DAEMON_WORK   31.
#define TLL_CLOCK_DAEMON_IDLE   3947. 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_FLAG_NONE           0UL
#define TLL_FLAG_GRAB           (1UL << 0)
#define TLL_FLAG_COPY           (1UL << 1)
#define TLL_FLAG_LASSO          (1UL << 2)
#define TLL_FLAG_FOCUS          (1UL << 3)
#define TLL_FLAG_SHIFT          (1UL << 4)
#define TLL_FLAG_DAEMON         (1UL << 5)

#define TLL_DIRTY_RUN           (1UL << 6)
#define TLL_DIRTY_ZONE          (1UL << 7)
#define TLL_DIRTY_NOTE          (1UL << 8)
#define TLL_DIRTY_LASSO         (1UL << 9)
#define TLL_DIRTY_BACKGROUND    (1UL << 10) 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#ifdef __MACH__

typedef uint32_t t_uint32_atomic;

#endif // __MACH__

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#ifdef __MACH__

#define TLL_FLAG_SET(mask)      OSAtomicOr32Barrier((mask), &x->flags); 
#define TLL_FLAG_UNSET(mask)    OSAtomicAnd32Barrier((~(mask)), &x->flags);
#define TLL_FLAG_TRUE(mask)     (x->flags & (mask)) 
#define TLL_FLAG_FALSE(mask)    !(x->flags & (mask))
    
#endif // __MACH__

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_RUN_LOCK        systhread_mutex_lock(x->runMutex); 
#define TLL_RUN_UNLOCK      systhread_mutex_unlock(x->runMutex); 

#define TLL_DATA_LOCK       systhread_mutex_lock(x->dataMutex); 
#define TLL_DATA_UNLOCK     systhread_mutex_unlock(x->dataMutex); 

#define TLL_DAEMON_LOCK     systhread_mutex_lock(x->daemonMutex); 
#define TLL_DAEMON_UNLOCK   systhread_mutex_unlock(x->daemonMutex); 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tll {
    t_jbox              box;
    t_uint32_atomic     flags;
    t_systhread_mutex   runMutex;
    t_systhread_mutex   dataMutex;
    t_systhread_mutex   daemonMutex;
    t_atom              info[4];
    t_atom              played[4];
    t_atom              dumped[5];
    t_atom              link;
    PIZTime             time;
    long                identifier;
    long                offsetX;
    long                offsetY;
    long                viewText;
    t_pt                cursor;  
    t_pt                origin;
    t_jrgba             lasso;
    t_jrgba             color;
    t_jrgba             uColor;
    t_jrgba             hColor1;
    t_jrgba             hColor2;
    t_jrgba             hColor3;
    t_jrgba             hColor4;
    t_jrgba             hColor5;
    t_jrgba             text;
    t_jrgba             uText;
    t_jrgba             border;
    t_jrgba             uBorder;
    t_jrgba             background;
    t_jrgba             uBackground;
    t_symbol            *name;
    t_jtextlayout       *layer; 
    t_dictionary        *data;
    t_dictionary        *current;
    t_dictionary        *status;
    PIZAgent            *agent;
    PIZLinklist         *run;
    PIZLinklist         *runCopy;
    PIZLinklist         *daemon;
    PIZArray            *temp[3];
    void                *runClock;
    void                *daemonClock;
    void                *gainedClock;
    void                *lostClock;
    void                *left;
    void                *middleLeft;
    void                *middle;
    void                *middleRight;
    void                *right;
    } t_tll;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new           (t_symbol *s, long argc, t_atom *argv);

void tralala_init           (t_tll *x, t_symbol *s, short argc, t_atom *argv);
void tralala_free           (t_tll *x);
void tralala_assist         (t_tll *x, void *b, long m, long a, char *s);
void tralala_jsave          (t_tll *x, t_dictionary *d);

void tralala_store          (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_recall         (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_remove         (t_tll *x, t_symbol *s, long argc, t_atom *argv);

void tralala_callback       (void *ptr, PIZEvent *event);
void tralala_runTask        (t_tll *x);
void tralala_daemonTask     (t_tll *x);

void tralala_play           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_loop           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_stop           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_unloop         (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_list           (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything       (t_tll *x, t_symbol *s, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_H
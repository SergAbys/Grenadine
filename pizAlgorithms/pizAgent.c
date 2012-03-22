/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	March 22, 2012.
 */
 
/*
 *  Copyright (c) 2012, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <errno.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define EXIT (x->flags & PIZ_FLAG_EXIT)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZAgent *pizAgentNew (void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc (sizeof(PIZAgent))) {
    //
    long err = PIZ_GOOD;
    
    x->flags        = PIZ_FLAG_WAKED | PIZ_FLAG_CHANGED;  
    x->tempo        = PIZ_DEFAULT_TEMPO;  
    x->runIn        = pizLinklistNew ( );
    x->runOut       = pizLinklistNew ( );
    x->graphicIn    = pizLinklistNew ( );
    x->graphicOut   = pizLinklistNew ( );
    x->sequence     = pizSequenceNew (0);
    x->eventLoopErr = PIZ_ERROR;
    
    if (!(x->runIn && 
        x->runOut && 
        x->graphicIn && 
        x->graphicOut && 
        x->sequence)) {
        err |= PIZ_MEMORY;
    }
    
    err |= pthread_mutex_init (&x->eventMutex, NULL);
    err |= pthread_cond_init  (&x->eventCondition, NULL);
    err |= pthread_attr_init  (&x->attr);
    
    if (!err) {
        pthread_attr_setscope        (&x->attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setinheritsched (&x->attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setdetachstate  (&x->attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setschedpolicy  (&x->attr, SCHED_OTHER);
        
        x->eventLoopErr = pthread_create (&x->eventLoop, &x->attr, pizAgentEventLoop, (void *)x); 
        err |= x->eventLoopErr;
    }
        
    if (err) {
        pizAgentFree (x);
        x = NULL;
    }
    //
    }
    
    return x;
}

void pizAgentFree (PIZAgent *x)
{ 
    if (x) {
    //
    if (!x->eventLoopErr) {
        PIZLOCKEVENT
        x->flags |= PIZ_FLAG_EXIT;
        PIZUNLOCKEVENT
    
        pthread_cond_signal (&x->eventCondition);
        pthread_join (x->eventLoop, NULL); 
    }
    
    pthread_attr_destroy  (&x->attr);
    pthread_mutex_destroy (&x->eventMutex);
    pthread_cond_destroy  (&x->eventCondition);
    
    pizLinklistFree (x->runIn);
    pizLinklistFree (x->graphicIn);
    pizSequenceFree (x->sequence);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = agent;  
    
    while (!EXIT) { 
    //
    
    PIZLOCKEVENT
    
    while (!pizAgentEventLoopCondition (x)) {
        pthread_cond_wait (&x->eventCondition, &x->eventMutex);
        post ("Waked");
        x->flags |= PIZ_FLAG_WAKED;
                
        if (EXIT) {
            break;
        } 
    }
    
    PIZUNLOCKEVENT
        
    if (!EXIT) {
    //
    pizAgentEventLoopInit (x);
     
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopProceedRunEvent (x)) {
            break;
        } 
    }
    
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopProceedGraphicEvent (x)) {
            break;
        } 
    }
    
    pizAgentEventLoopSleep (x); 
    //
    }
    //    
    }
    
    pthread_exit (NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE bool pizAgentEventLoopCondition (PIZAgent *x)
{
    bool condition = false;
    
    condition = pizLinklistCount (x->runIn) ||
                pizLinklistCount (x->graphicIn) ||
                (x->flags & PIZ_FLAG_PLAYED);
    
    return condition;
}

PIZ_INLINE void pizAgentEventLoopInit (PIZAgent *x)
{
    if (x->flags & PIZ_FLAG_WAKED) {
        pizTimeSet (&x->grainStart);
    } else {
        pizTimeCopy (&x->grainStart, &x->grainEnd);
    }
    
    if (x->flags & PIZ_FLAG_CHANGED) {
        x->grainSize     = (PIZNano)(PIZ_BPM_CONSTANT / x->tempo);
        x->grainWorkSize = (PIZNano)(PIZ_BPM_CONSTANT * PIZ_WORK_TIME_RATIO / x->tempo);
        x->flags &= ~PIZ_FLAG_CHANGED;
    }
    
    pizTimeCopy (&x->grainEnd, &x->grainStart);
    pizTimeAddNano (&x->grainEnd, &x->grainSize);
    x->flags &= ~PIZ_FLAG_WAKED;
}

PIZ_INLINE bool pizAgentEventLoopIsWorkTime (PIZAgent *x)
{
    bool    isWorkTime = false;
    PIZTime now;
    PIZNano elapsed;
    
    pizTimeSet (&now);
    
    if (!pizTimeElapsedNano (&x->grainStart, &now, &elapsed)) {
        if (elapsed < x->grainWorkSize) {
            isWorkTime = true;
        }
    }
    
    return isWorkTime;
}

PIZ_INLINE void pizAgentEventLoopSleep (PIZAgent *x)
{
    PIZTime          now;
    PIZNano          ns;
    struct timespec  t0, t1;
    struct timespec* ptrA = &t0;
    struct timespec* ptrB = &t1;
    struct timespec* temp = NULL;

    PIZError err = PIZ_GOOD;
    
    pizTimeSet (&now);
    err = pizTimeElapsedNano (&now, &x->grainEnd, &ns);
    
    while (err) {
        pizTimeAddNano (&x->grainEnd, &x->grainSize); 
        err = pizTimeElapsedNano (&now, &x->grainEnd, &ns);
    }
    
    pizTimespecWithNano (ptrA, &ns);
    
    while ((nanosleep (ptrA, ptrB) == -1) && (errno == EINTR)) {
        temp = ptrA;
        ptrA = ptrB;
        ptrB = temp;
        post ("Interrupted");
    }
} 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentEventLoopProceedRunEvent (PIZAgent *x) 
{
    long            err = PIZ_ERROR;
    PIZEvent        *event = NULL;
    PIZAgentMethod  f = NULL;
            
    PIZLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (x->runIn, 0, (void *)&event)) {
        pizLinklistChuckByPtr (x->runIn, event);
        post ("Chucked");
        
        switch (event->name) {
            case PIZ_PLAY : f = pizAgentMethodPlay; break;
            case PIZ_STOP : f = pizAgentMethodStop; break;
        }
    }
    
    PIZUNLOCKEVENT
    
    if (f) {
        (*f)(x, event);
    }
    
    pizEventFree (event);
    
    PIZLOCKEVENT
    
    if (pizLinklistCount (x->runIn)) {
        err = PIZ_GOOD;
    }
    
    PIZUNLOCKEVENT
    
    return err;
}

PIZError pizAgentEventLoopProceedGraphicEvent (PIZAgent *x)
{
    long err = PIZ_ERROR;
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentAppendEvent (PIZAgent *x, PIZEvent *event)
{
    if (event) {
        if (event->type == PIZ_RUN_EVENT) {
            PIZLOCKEVENT
            pizLinklistAppend (x->runIn, event);
            PIZUNLOCKEVENT
        } else if (event->type == PIZ_GRAPHIC_EVENT) {
            PIZLOCKEVENT
            pizLinklistAppend (x->graphicIn, event);
            PIZUNLOCKEVENT
        }
        
        pthread_cond_signal (&x->eventCondition);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentMethodPlay (PIZAgent *x, PIZEvent *event)
{
    x->flags |= PIZ_FLAG_PLAYED; 
}

void pizAgentMethodStop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_FLAG_PLAYED; 
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	March 21, 2012.
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
#define INIT (x->flags & PIZ_FLAG_INIT)
#define PLAY (x->flags & PIZ_FLAG_PLAY)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZAgent *pizAgentNew (void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc (sizeof(PIZAgent))) {
    //
    long err = PIZ_GOOD;
    x->eventLoopErr = PIZ_ERROR;
    
    x->flags         = PIZ_FLAG_INIT;  
    x->tempo         = PIZ_DEFAULT_TEMPO;  
    x->grainSize     = (PIZNano)(PIZ_BPM_CONSTANT / x->tempo);
    x->grainWorkSize = (PIZNano)(PIZ_BPM_CONSTANT / x->tempo * PIZ_WORK_TIME_RATIO);
        
    x->runIn      = pizLinklistNew ( );
    x->runOut     = pizLinklistNew ( );
    x->graphicIn  = pizLinklistNew ( );
    x->graphicOut = pizLinklistNew ( );
    
    if (!(x->runIn && x->runOut && x->graphicIn && x->graphicOut)) {
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
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = agent;   
    
    while (!EXIT) { post ("####");
    //
    PIZLOCKEVENT
    
    while (!pizAgentEventLoopCondition (x)) {
        pthread_cond_wait (&x->eventCondition, &x->eventMutex);
        post ("Wake Up");
        x->flags |= PIZ_FLAG_INIT;
                
        if (EXIT) {
            break;
        } 
    }
    
    PIZUNLOCKEVENT
    
    if (!EXIT && !pizAgentEventLoopInit (x)) {
    //
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

bool pizAgentEventLoopCondition (PIZAgent *x)
{
    bool condition = false;
    
    condition = pizLinklistCount (x->runIn) ||
                pizLinklistCount (x->graphicIn) ||
                PLAY;
    
    return condition;
}

PIZError pizAgentEventLoopInit (PIZAgent *x)
{
    long err = PIZ_GOOD;
    
    if (INIT) {
        err |= pizTimeSet (&x->grainStart);
    } else {
        pizTimeCopy (&x->grainStart, &x->grainEnd);
    }
    
    if (!err) {
        pizTimeCopy (&x->grainEnd, &x->grainStart);
        pizTimeAddNano (&x->grainEnd, &x->grainSize);
        x->flags &= ~PIZ_FLAG_INIT;
    }
    
    return err;
}

bool pizAgentEventLoopIsWorkTime (PIZAgent *x)
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

void pizAgentEventLoopSleep (PIZAgent *x)
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
    }
} 

PIZError pizAgentEventLoopProceedRunEvent (PIZAgent *x) 
{
    long            err = PIZ_ERROR;
    PIZEvent        *event = NULL;
    PIZAgentMethod  f = NULL;
            
    PIZLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (x->runIn, 0, (void *)&event)) {
        pizLinklistChuckByPtr (x->runIn, event);
        post ("Chuck Event");
        
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
    x->flags |= PIZ_FLAG_PLAY; 
}

void pizAgentMethodStop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_FLAG_PLAY; 
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
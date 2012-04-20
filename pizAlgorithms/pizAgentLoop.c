/*
 * \file	pizAgentLoop.c
 * \author	Jean Sapristi
 * \date	April 20, 2012.
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

#include "pizAgentLoop.h"
#include "pizAgentMethod.h"
#include "pizSequenceRun.h"
#include "pizSequenceTransform.h"
#include "pizSequenceGraphic.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <errno.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_EXIT (x->flags & PIZ_AGENT_FLAG_EXIT)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_OBJECT_NONE         0
#define PIZ_OBJECT_AGENT        1
#define PIZ_OBJECT_SEQUENCE     2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = (PIZAgent *)agent;  
    
    while (!PIZ_EXIT) { 
    //
    
    PIZ_AGENT_LOCK_EVENT
    
    while (!(pizAgentEventLoopIsCondition (x))) {
        pthread_cond_wait (&x->eventCondition, &x->eventLock);
        x->flags |= PIZ_AGENT_FLAG_WAKED;
        if (PIZ_EXIT) { 
            break; 
        } 
    }
    
    PIZ_AGENT_UNLOCK_EVENT
        
    if (!PIZ_EXIT) {
    //
    
    pizAgentEventLoopInit (x);
     
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoEvent (x, x->runInQueue)) {
            break;
        } 
    }
    
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoEvent (x, x->transformQueue)) {
            break;
        } 
    }
    
    if (x->flags & PIZ_AGENT_FLAG_PLAYING) {
        if (pizAgentEventLoopIsWorkTime (x)) {
            pizAgentEventLoopDoStep (x, 0);
        } else {
            pizAgentEventLoopDoStep (x, 1);
        }
    }
    
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoEvent (x, x->graphicInQueue)) {
            pizAgentEventLoopDoRefresh (x);
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

void *pizAgentNotificationLoop (void *agent)
{
    PIZAgent *x = (PIZAgent *)agent;  
    
    while (!PIZ_EXIT) { 
    //
    
    PIZ_AGENT_LOCK_NOTIFICATION
    
    while (!(pizLinklistCount (x->notifyQueue))) {
        pthread_cond_wait (&x->notificationCondition, &x->notificationLock);
        if (PIZ_EXIT) {
            break;
        } 
    }
    
    PIZ_AGENT_UNLOCK_NOTIFICATION
        
    if (!PIZ_EXIT) {
        pizAgentNotificationLoopNotify (x);
    }
    //    
    }
    
    pthread_exit (NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentEventLoopDoEvent (PIZAgent *x, PIZLinklist *queue) 
{
    long            k; 
    PIZError        err = PIZ_GOOD;
    PIZEvent        *event = NULL;
    PIZMethod       f = NULL;
    PIZMethodError  g = NULL;

            
    PIZ_AGENT_LOCK_EVENT
    
    if (!(pizLinklistPtrAtIndex (queue, 0, (void **)&event))) {
        pizLinklistChuckByPtr (queue, event);
    }
    
    if (!(pizLinklistCount (queue))) {
        err = PIZ_ERROR;
    }
    
    PIZ_AGENT_UNLOCK_EVENT
    
    DEBUGEVENT
    
    if (event && (k = pizAgentEventLoopGetMethod (event, &f, &g))) {
    //
    void *ptr = NULL;
    
    if (k == PIZ_OBJECT_AGENT) {
        ptr = x;
    } else {
        ptr = x->sequence;
    }
        
    if (f) {
        (*f)(ptr, event);
    } else {
        if ((*g)(ptr, event) == PIZ_MEMORY) {
            PIZ_AGENT_MEMORY
        }
    }

    pizEventFree (event);
    //
    }
    
    return err;
}

void pizAgentEventLoopDoStep (PIZAgent *x, bool blank)
{   
    bool     k = false;
    long     count = 0;
    PIZError err = PIZ_GOOD; 
    
    do {
    //
    
    if (!blank) {
        PIZ_AGENT_LOCK_GETTER
        
        pizLinklistClear (x->runOutQueue);
        err = pizSequenceProceedStep (x->sequence, x->runOutQueue, x->bpm);
        count = pizLinklistCount (x->runOutQueue);
        
        PIZ_AGENT_UNLOCK_GETTER
    } else {
        err = pizSequenceProceedStep (x->sequence, NULL, x->bpm);
    }
    
    if (err == PIZ_GOOD) {
        if (count) {
            PIZEvent *event = NULL;
            if (event = pizEventNewWithTime (PIZ_EVENT_RUN_READY, &x->grainStart)) {
            
                PIZ_AGENT_LOCK_NOTIFICATION
                PIZ_AGENT_QUEUE(x->notifyQueue)
                pthread_cond_signal (&x->notificationCondition);
                PIZ_AGENT_UNLOCK_NOTIFICATION
            }
        }
        
        if (pizSequenceIsAtEnd (x->sequence)) {
            pizAgentEventLoopDoStepLast (x);
        }
        
        k = false;  
        
    } else if (err == PIZ_ERROR) {
        if (x->flags & PIZ_AGENT_FLAG_LOOPED) {
            k = true;
        } else {
            k = false;
            x->flags &= ~PIZ_AGENT_FLAG_PLAYING;
        }
        pizSequenceGoToStart (x->sequence);
        pizAgentEventLoopDoStepEnd (x);
  
    } else if (err == PIZ_MEMORY) { 
        PIZ_AGENT_MEMORY 
    }

    //
    } while (k);
}

void pizAgentEventLoopDoRefresh (PIZAgent *x)
{
    PIZEvent *event = NULL;
    PIZError err = PIZ_ERROR;
    long     count = 0;
    
    PIZ_AGENT_LOCK_GETTER
    
    if (!(pizLinklistCount (x->graphicOutQueue))) {
        err = pizSequenceGetGraphicEvents (x->sequence, x->graphicOutQueue);
        count = pizLinklistCount (x->graphicOutQueue);
    }
    
    PIZ_AGENT_UNLOCK_GETTER
    
    if (!err) {
        if (count && (event = pizEventNewWithTime (PIZ_EVENT_GRAPHIC_READY, &x->grainStart))) {
            
            PIZ_AGENT_LOCK_NOTIFICATION
            PIZ_AGENT_QUEUE(x->notifyQueue)
            pthread_cond_signal (&x->notificationCondition);
            PIZ_AGENT_UNLOCK_NOTIFICATION
        }
        
    } else if (err == PIZ_MEMORY) {
        PIZ_AGENT_MEMORY
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopDoStepEnd (PIZAgent *x)
{
    PIZEvent *event = NULL;

    if (event = pizEventNewWithTime (PIZ_EVENT_END, &x->grainStart)) {
    
        PIZ_AGENT_LOCK_NOTIFICATION
        PIZ_AGENT_QUEUE(x->notifyQueue)
        pthread_cond_signal (&x->notificationCondition);
        PIZ_AGENT_UNLOCK_NOTIFICATION
    }
}

void pizAgentEventLoopDoStepLast (PIZAgent *x)
{
    PIZEvent *event = NULL;
    
    if (event = pizEventNewWithTime (PIZ_EVENT_LAST, &x->grainStart)) {
    
        PIZ_AGENT_LOCK_NOTIFICATION
        PIZ_AGENT_QUEUE(x->notifyQueue)
        pthread_cond_signal (&x->notificationCondition);
        PIZ_AGENT_UNLOCK_NOTIFICATION
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopInit (PIZAgent *x)
{   
    if (x->flags & PIZ_AGENT_FLAG_WAKED) {
        pizTimeSet (&x->grainStart);
        x->flags &= ~PIZ_AGENT_FLAG_WAKED;
    } else {
        pizTimeCopy (&x->grainStart, &x->grainEnd);
    }
    
    pizTimeSetNano (&x->grainSize, PIZ_AGENT_CONSTANT_BPM / x->bpm);
    pizTimeCopy    (&x->grainEnd, &x->grainStart);
    pizTimeAddNano (&x->grainEnd, &x->grainSize);
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
        pizTimeAddNano (&x->grainStart, &x->grainSize);
        pizTimeAddNano (&x->grainEnd, &x->grainSize); 
        
        if (x->flags & PIZ_AGENT_FLAG_PLAYING) {
            pizAgentEventLoopDoStep (x, 1);
        } 
        
        err = pizTimeElapsedNano (&now, &x->grainEnd, &ns);
    }
    
    pizTimespecWithNano (ptrA, &ns);
    
    while ((nanosleep (ptrA, ptrB) == -1) && (errno == EINTR)) {
        temp = ptrA;
        ptrA = ptrB;
        ptrB = temp;
    }
}

bool pizAgentEventLoopIsCondition (PIZAgent *x)
{
    bool condition = false;
    
    if ((x->flags & PIZ_AGENT_FLAG_PLAYING)   ||
        pizLinklistCount (x->transformQueue) ||
        pizLinklistCount (x->runInQueue)     ||
        pizLinklistCount (x->graphicInQueue)) {
        condition = true;
    }
    
    return condition;
}

bool pizAgentEventLoopIsWorkTime (PIZAgent *x)
{
    bool    isWorkTime = false;
    PIZTime now;
    PIZNano elapsed;
    PIZNano timeOut;
    
    pizTimeSet     (&now);
    pizTimeSetNano (&timeOut, PIZ_AGENT_CONSTANT_WORK_RATIO / x->bpm);
    
    if (!(pizTimeElapsedNano (&x->grainStart, &now, &elapsed))) {
        if (elapsed < timeOut) {
            isWorkTime = true;
        }
    }
    
    return isWorkTime;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizAgentEventLoopGetMethod (const PIZEvent *event, PIZMethod *f, PIZMethodError *g)
{
    long k = PIZ_OBJECT_NONE;
    
    switch (event->identifier) {
        case PIZ_EVENT_PLAY     : *f = pizAgentPlay;        k = PIZ_OBJECT_AGENT; break;
        case PIZ_EVENT_STOP     : *f = pizAgentStop;        k = PIZ_OBJECT_AGENT; break;
        case PIZ_EVENT_LOOP     : *f = pizAgentLoop;        k = PIZ_OBJECT_AGENT; break;
        case PIZ_EVENT_UNLOOP   : *f = pizAgentUnloop;      k = PIZ_OBJECT_AGENT; break;
        case PIZ_EVENT_BPM      : *f = pizAgentBPM;         k = PIZ_OBJECT_AGENT; break;
        case PIZ_EVENT_ADD      : *f = pizSequenceAdd;      k = PIZ_OBJECT_SEQUENCE; break;
        case PIZ_EVENT_CLEAR    : *f = pizSequenceClear;    k = PIZ_OBJECT_SEQUENCE; break;  
    }
    
    return k;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentNotificationLoopNotify (PIZAgent *x)
{
    PIZEvent *event = NULL;
            
    PIZ_AGENT_LOCK_NOTIFICATION
    
    if (!(pizLinklistPtrAtIndex (x->notifyQueue, 0, (void **)&event))) {
        pizLinklistChuckByPtr (x->notifyQueue, event);
    }
    
    PIZ_AGENT_UNLOCK_NOTIFICATION   
           
    DEBUGEVENT    
        
    pizEventFree (event);
} 

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
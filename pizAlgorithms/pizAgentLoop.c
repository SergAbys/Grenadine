/*
 * \file	pizAgentLoop.c
 * \author	Jean Sapristi
 * \date	April 5, 2012.
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <errno.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define EXIT (x->flags & PIZ_AGENT_FLAG_EXIT)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = (PIZAgent *)agent;  
    
    while (!EXIT) { 
    //
    PIZAGENTLOCKEVENT
    
    while (!pizAgentEventLoopCondition (x)) {
        pthread_cond_wait (&x->eventCondition, &x->eventLock);
        x->flags |= PIZ_AGENT_FLAG_WAKED;
        
        if (EXIT) {
            break;
        } 
    }
    
    PIZAGENTUNLOCKEVENT
        
    if (!EXIT) {
    //
    
    pizAgentEventLoopInit (x);
     
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoEvent (x, x->runInQueue)) {
            break;
        } 
    }
    
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoEvent (x, x->mainQueue)) {
            break;
        } 
    }
    
    if (x->flags & PIZ_AGENT_FLAG_PLAYED) {
        if (pizAgentEventLoopIsWorkTime (x)) {
            pizAgentEventLoopDoStep (x, 0);
        } else {
            pizAgentEventLoopDoStep (x, 1);
        }
    }
    
    if (x->flags & PIZ_AGENT_FLAG_GUI) {
        while (pizAgentEventLoopIsWorkTime (x)) {
            if (pizAgentEventLoopDoEvent (x, x->graphicInQueue)) {
                pizAgentEventLoopDoRefresh (x);
                break;
            } 
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
    
    while (!EXIT) { 
    //
    
    PIZAGENTLOCKNOTIFICATION
    
    while (!(pizLinklistCount (x->notifyQueue))) {
        pthread_cond_wait (&x->notificationCondition, &x->notificationLock);
                        
        if (EXIT) {
            break;
        } 
    }
    
    PIZAGENTUNLOCKNOTIFICATION
        
    if (!EXIT) {
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
    long            err = PIZ_ERROR;
    PIZEvent        *event = NULL;
    PIZAgentMethod  f = NULL;
            
    PIZAGENTLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (queue, 0, (void **)&event)) {
        pizLinklistChuckByPtr (queue, event);
    }
    
    PIZAGENTUNLOCKEVENT
    
    DEBUGEVENT
    
    if (event) {
        pizAgentEventLoopGetMethod (event, &f);
    
        if (f) {
            (*f)(x, event);
        }
    
        pizEventFree (event);
    }
    
    PIZAGENTLOCKEVENT
    
    if (pizLinklistCount (queue)) {
        err = PIZ_GOOD;
    }
    
    PIZAGENTUNLOCKEVENT
    
    return err;
}

void pizAgentEventLoopDoStep (PIZAgent *x, bool blank)
{
    bool     k = false;
    PIZError err = PIZ_GOOD; 

    do {
    //
    
    if (!blank) {
        pizArrayClear (x->tempArray);
        err = pizSequenceProceedStep (x->sequence, x->tempArray);
    } else {
        err = pizSequenceProceedStep (x->sequence, NULL);
    }
    
    if (err == PIZ_GOOD) {
    //
    if (!blank) {
    //
    long     i;
    long     *ptr = NULL;
    PIZEvent *event = NULL;
        
    PIZAGENTLOCKGETTER 
        
    pizLinklistClear (x->runOutQueue);
    ptr = pizArrayPtr (x->tempArray);
    
    for (i = 0; i < pizArrayCount (x->tempArray); i += PIZ_DATA_NOTE_SIZE) {
    //
    if (event = pizEventNewWithArray (PIZ_EVENT_RUN, PIZ_EVENT_NOTE_PLAYED, PIZ_DATA_NOTE_SIZE, (ptr + i), 0)) {
        PIZAGENTQUEUE(x->runOutQueue)
    }
    //
    }
    
    if (pizLinklistCount (x->runOutQueue)) {
    //
    if (event = pizEventNewWithTime (PIZ_EVENT_NOTIFICATION, PIZ_EVENT_RUN_READY, &x->grainStart)) {
        PIZAGENTLOCKNOTIFICATION
        PIZAGENTQUEUE(x->notifyQueue)
        PIZAGENTUNLOCKNOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
    }
    //
    }
        
    PIZAGENTUNLOCKGETTER
    //
    }
    
    if (pizSequenceIsAtEnd (x->sequence)) {
        pizAgentEventLoopDoStepLast (x);
    }
    
    k = false;
    //    
    } else if (err == PIZ_ERROR) {
        if (x->flags & PIZ_AGENT_FLAG_LOOPED) {
            k = true;
        } else {
            k = false;
            x->flags &= ~PIZ_AGENT_FLAG_PLAYED;
        }
        pizSequenceGoToStart (x->sequence);
        pizAgentEventLoopDoStepEnd (x);
        
    } else {
        MEMORY_ERROR;
    }
    //
    } while (k);
}

void pizAgentEventLoopDoRefresh (PIZAgent *x)
{
    PIZEvent *event = NULL;
    PIZError err = PIZ_GOOD;
    
    PIZAGENTLOCKGETTER
    
    pizLinklistClear (x->graphicOutQueue);
    
    if (err = pizSequenceGetGraphicEvents (x->sequence, x->graphicOutQueue)) {
        MEMORY_ERROR
    }
    
    if (pizLinklistCount (x->graphicOutQueue)) {
    //
    if (event = pizEventNewWithTime (PIZ_EVENT_NOTIFICATION, PIZ_EVENT_GUI_READY, &x->grainStart)) {
        PIZAGENTLOCKNOTIFICATION
        PIZAGENTQUEUE(x->notifyQueue)
        PIZAGENTUNLOCKNOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
    }
    //
    }
    
    PIZAGENTUNLOCKGETTER
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopDoStepEnd (PIZAgent *x)
{
    PIZEvent *event = NULL;

    if (event = pizEventNewWithTime (PIZ_EVENT_NOTIFICATION, PIZ_EVENT_END, &x->grainStart)) {
        PIZAGENTLOCKNOTIFICATION
        PIZAGENTQUEUE(x->notifyQueue)
        PIZAGENTUNLOCKNOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
    }
}

void pizAgentEventLoopDoStepLast (PIZAgent *x)
{
    PIZEvent *event = NULL;
    
    if (event = pizEventNewWithTime (PIZ_EVENT_NOTIFICATION, PIZ_EVENT_LAST, &x->grainStart)) {
        PIZAGENTLOCKNOTIFICATION
        PIZAGENTQUEUE(x->notifyQueue)
        PIZAGENTUNLOCKNOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizAgentEventLoopCondition (PIZAgent *x)
{
    bool condition = false;
    
    if ((x->flags & PIZ_AGENT_FLAG_PLAYED) ||
        pizLinklistCount (x->mainQueue)    ||
        pizLinklistCount (x->runInQueue)   ||
        pizLinklistCount (x->graphicInQueue)) {
        condition = true;
    }
    
    return condition;
}

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

bool pizAgentEventLoopIsWorkTime (PIZAgent *x)
{
    bool    isWorkTime = false;
    PIZTime now;
    PIZNano elapsed;
    PIZNano timeOut;
    
    pizTimeSet     (&now);
    pizTimeSetNano (&timeOut, PIZ_AGENT_CONSTANT_WORK_RATIO / x->bpm);
    
    if (!pizTimeElapsedNano (&x->grainStart, &now, &elapsed)) {
        if (elapsed < timeOut) {
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
        pizTimeAddNano (&x->grainStart, &x->grainSize);
        pizTimeAddNano (&x->grainEnd, &x->grainSize); 
        
        if (x->flags & PIZ_AGENT_FLAG_PLAYED) {
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

void pizAgentEventLoopGetMethod (const PIZEvent *x, PIZAgentMethod *f)
{
    switch (x->identifier) {
        case PIZ_EVENT_PLAY     : *f = pizAgentMethodPlay;       break;
        case PIZ_EVENT_STOP     : *f = pizAgentMethodStop;       break;
        case PIZ_EVENT_LOOP     : *f = pizAgentMethodLoop;       break;
        case PIZ_EVENT_UNLOOP   : *f = pizAgentMethodUnloop;     break; 
        case PIZ_EVENT_BPM      : *f = pizAgentMethodBPM;        break;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentNotificationLoopNotify (PIZAgent *x)
{
    PIZEvent *event = NULL;
            
    PIZAGENTLOCKNOTIFICATION
    
    if (!pizLinklistPtrAtIndex (x->notifyQueue, 0, (void **)&event)) {
        pizLinklistChuckByPtr (x->notifyQueue, event);
    }
    
    PIZAGENTUNLOCKNOTIFICATION   
           
    DEBUGEVENT    
        
    pizEventFree (event);
} 

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
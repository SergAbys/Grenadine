/*
 * \file	pizAgentLoop.c
 * \author	Jean Sapristi
 * \date	March 25, 2012.
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
#pragma mark -

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = agent;  
    
    while (!EXIT) { 
    //
    PIZLOCKEVENT
    
    while (!pizAgentEventLoopCondition (x)) {
        pthread_cond_wait (&x->eventCondition, &x->eventLock);
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
        if (pizAgentEventLoopDoEvent (x, x->runIn)) {
            break;
        } 
    }
    
    if (x->flags & PIZ_FLAG_PLAYED) {
        if (pizAgentEventLoopIsWorkTime (x)) {
            pizAgentEventLoopDoStep (x);
        } else {
            pizAgentEventLoopDoBlank (x);
        }
    }
    
    if (x->flags & PIZ_FLAG_GUI) {
        while (pizAgentEventLoopIsWorkTime (x)) {
            if (pizAgentEventLoopDoEvent (x, x->graphicIn)) {
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentEventLoopDoEvent (PIZAgent *x, PIZLinklist *queue) 
{
    long            err = PIZ_ERROR;
    PIZEvent        *event = NULL;
    PIZAgentMethod  f = NULL;
            
    PIZLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (queue, 0, (void *)&event)) {
        pizLinklistChuckByPtr (queue, event);
    }
    
    PIZUNLOCKEVENT
    
    if (event) {
        switch (event->name) {
            case PIZ_PLAY           : f = pizAgentMethodPlay;       break;
            case PIZ_STOP           : f = pizAgentMethodStop;       break;
            case PIZ_LOOP           : f = pizAgentMethodLoop;       break;
            case PIZ_UNLOOP         : f = pizAgentMethodUnloop;     break; 
            case PIZ_BPM            : f = pizAgentMethodBPM;        break;
            case PIZ_ENABLE_GUI     : f = pizAgentMethodEnableGUI;  break;
            case PIZ_DISABLE_GUI    : f = pizAgentMethodDisableGUI; break;
        }
    }
    
    if (f) {
        (*f)(x, event);
    }
    
    pizEventFree (event);
    
    PIZLOCKEVENT
    
    if (pizLinklistCount (queue)) {
        err = PIZ_GOOD;
    }
    
    PIZUNLOCKEVENT
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopDoRefresh (PIZAgent *x)
{
    PIZEvent *event = NULL;
    
    PIZLOCKGETTER
    
    pizLinklistClear (x->graphicOut);
    pizSequenceAppendGraphicEvents (x->sequence, x->graphicOut);
    
    if (pizLinklistCount (x->graphicOut)) {
        if (event = pizEventNewWithTime (PIZ_NOTIFICATION, PIZ_GRAPHIC_READY, &x->grainStart)) {
            PIZLOCKNOTIFICATION
            if (pizLinklistAppend (x->notificationQueue, event)) {
                pizEventFree (event);
            }
            PIZUNLOCKNOTIFICATION
            pthread_cond_signal (&x->notificationCondition);
        }
    }
    
    PIZUNLOCKGETTER
}

void pizAgentEventLoopDoStep (PIZAgent *x)
{
    long err = PIZ_GOOD; 

    pizGrowingArrayClear (x->tempArray);
    err = pizSequenceProceedStep (x->sequence, x->tempArray);
    
    if (err == PIZ_GOOD) {
    //
    long     i;
    long     *ptr = NULL;
    PIZEvent *event = NULL;
        
    PIZLOCKGETTER 
        
    pizLinklistClear (x->runOut);
    ptr = pizGrowingArrayPtr (x->tempArray);
    
    for (i = 0; i < pizGrowingArrayCount (x->tempArray); i += PIZ_DATA_NOTE_SIZE) {
        if (event = pizEventNewWithArray (PIZ_RUN, PIZ_NOTE_PLAYED, PIZ_DATA_NOTE_SIZE, (ptr + i), 0)) {
            if (pizLinklistAppend (x->runOut, event)) {
                pizEventFree (event);
            }
        }
    }
    
    if (pizLinklistCount (x->runOut)) {
        if (event = pizEventNewWithTime (PIZ_NOTIFICATION, PIZ_RUN_READY, &x->grainStart)) {
            PIZLOCKNOTIFICATION
            if (pizLinklistAppend (x->notificationQueue, event)) {
                pizEventFree (event);
            }
            PIZUNLOCKNOTIFICATION
            pthread_cond_signal (&x->notificationCondition);
        }
    }
        
    PIZUNLOCKGETTER
    //    
    } else if (err == PIZ_ERROR) {
        pizAgentEventLoopNotifyEnd (x);
    }
}

void pizAgentEventLoopDoBlank (PIZAgent *x)
{
    long err = pizSequenceProceedStep (x->sequence, NULL);
    
    if (err == PIZ_ERROR) {
        pizAgentEventLoopNotifyEnd (x);
    }
}

void pizAgentEventLoopNotifyEnd (PIZAgent *x)
{
    PIZEvent *event = NULL;
        
    if (event = pizEventNewWithTime (PIZ_NOTIFICATION, PIZ_END, &x->grainStart)) {
        PIZLOCKNOTIFICATION
        if (pizLinklistAppend (x->notificationQueue, event)) {
            pizEventFree (event);
        }
        PIZUNLOCKNOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
    }
    
    x->flags &= ~PIZ_FLAG_PLAYED;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE bool pizAgentEventLoopCondition (PIZAgent *x)
{
    bool condition = false;
    
    if ((x->flags & PIZ_FLAG_PLAYED) ||
        pizLinklistCount (x->runIn) ||
        pizLinklistCount (x->graphicIn)) {
        condition = true;
    }
    
    return condition;
}

PIZ_INLINE void pizAgentEventLoopInit (PIZAgent *x)
{
    if (x->flags & PIZ_FLAG_WAKED) {
        pizTimeSet (&x->grainStart);
        x->flags &= ~PIZ_FLAG_WAKED;
    } else {
        pizTimeCopy (&x->grainStart, &x->grainEnd);
    }
    
    pizTimeSetNano (&x->grainSize, PIZ_CONSTANT_BPM / x->bpm);
    pizTimeSetNano (&x->grainWorkSize, PIZ_CONSTANT_WORK_RATIO / x->bpm);
    
    pizTimeCopy    (&x->grainEnd, &x->grainStart);
    pizTimeAddNano (&x->grainEnd, &x->grainSize);
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
        pizTimeAddNano (&x->grainStart, &x->grainSize);
        pizTimeAddNano (&x->grainEnd, &x->grainSize); 
        
        if (x->flags & PIZ_FLAG_PLAYED) {
            pizAgentEventLoopDoBlank (x);
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL void *pizAgentNotificationLoop (void *agent)
{
    PIZAgent *x = agent;  
    
    while (!EXIT) { 
    //
    
    PIZLOCKNOTIFICATION
    
    while (!(pizLinklistCount (x->notificationQueue))) {
        pthread_cond_wait (&x->notificationCondition, &x->notificationLock);
                        
        if (EXIT) {
            break;
        } 
    }
    
    PIZUNLOCKNOTIFICATION
        
    if (!EXIT) {
        pizAgentNotificationLoopProceed (x);
    }
    //    
    }
    
    pthread_exit (NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentNotificationLoopProceed (PIZAgent *x)
{
    PIZEvent *event = NULL;
            
    PIZLOCKNOTIFICATION
    
    if (!pizLinklistPtrAtIndex (x->notificationQueue, 0, (void *)&event)) {
        pizLinklistChuckByPtr (x->notificationQueue, event);
    }
    
    PIZUNLOCKNOTIFICATION
    
    // 
    
    pizEventFree (event);
} 

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
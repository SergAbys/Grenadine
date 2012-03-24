/*
 * \file	pizAgentLoop.c
 * \author	Jean Sapristi
 * \date	March 24, 2012.
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
        post ("Waked / %s", __FUNCTION__);
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
        if (pizAgentEventLoopProceedRun (x)) {
            break;
        } 
    }
    
    if (x->flags & PIZ_FLAG_PLAYED) {
        if (pizAgentEventLoopIsWorkTime (x)) {
            pizAgentEventLoopProceedStep (x);
        } else {
            pizAgentEventLoopProceedBlank (x);
        }
    }
    
    if (x->flags & PIZ_FLAG_GUI) {
        while (pizAgentEventLoopIsWorkTime (x)) {
            if (pizAgentEventLoopProceedGraphic (x)) {
                pizAgentEventLoopProceedGraphicUpdate (x);
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

PIZError pizAgentEventLoopProceedRun (PIZAgent *x) 
{
    long            err = PIZ_ERROR;
    PIZEvent        *event = NULL;
    PIZAgentMethod  f = NULL;
            
    PIZLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (x->runIn, 0, (void *)&event)) {
        pizLinklistChuckByPtr (x->runIn, event);
        post ("Chucked / %s", __FUNCTION__);
        
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

PIZError pizAgentEventLoopProceedGraphic (PIZAgent *x)
{
    long err = PIZ_ERROR;
        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopProceedGraphicUpdate (PIZAgent *x)
{
    ;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopProceedStep (PIZAgent *x)
{
    long err = PIZ_GOOD; 

    pizGrowingArrayClear (x->tempArray);
    err = pizSequenceProceedStep (x->sequence, x->tempArray);
    
    if (err == PIZ_GOOD) {
        if (!PIZTRYLOCKQUERY) {
            pizLinklistClear (x->runOut);
            
            /*for (i = 0; i < pizGrowingArrayCount (x->tempArray); i += PIZ_DATA_NOTE_SIZE) {
            
            }*/
            
            if (pizLinklistCount (x->runOut)) {
                PIZLOCKNOTIFICATION
            
                PIZUNLOCKNOTIFICATION
            }
                
            PIZUNLOCKQUERY
        }
    } else if (err == PIZ_ERROR) {
        pizAgentEventLoopProceedEnd (x);
    }
}

void pizAgentEventLoopProceedBlank (PIZAgent *x)
{
    long err = pizSequenceProceedStep (x->sequence, NULL);
    
    if (err == PIZ_ERROR) {
        pizAgentEventLoopProceedEnd (x);
    }
}

void pizAgentEventLoopProceedEnd (PIZAgent *x)
{
    PIZEvent *notification = NULL;
        
    if (notification = pizEventNew ( )) {
        notification->type = PIZ_NOTIFICATION;
        notification->name = PIZ_END;
        pizTimeCopy (&notification->data.time, &x->grainStart);   
        
        PIZLOCKNOTIFICATION
        if (pizLinklistAppend (x->notificationQueue, notification)) {
            pizEventFree (notification);
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
        pizTimeAddNano (&x->grainStart, &x->grainSize);
        pizTimeAddNano (&x->grainEnd, &x->grainSize); 
        
        if (x->flags & PIZ_FLAG_PLAYED) {
            pizAgentEventLoopProceedBlank (x);
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
        post ("Waked / %s", __FUNCTION__);
                
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
        post ("Chucked / %s", __FUNCTION__);
    }
    
    PIZUNLOCKNOTIFICATION
    
    pizEventFree (event);
} 

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	April 1, 2012.
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
#include "pizAgentLoop.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_BPM     120

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZAgent *pizAgentNew (void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc (sizeof(PIZAgent))) {
    //
    long     err = PIZ_GOOD;
    PIZEvent *event = NULL;
    
    x->flags                = PIZ_AGENT_FLAG_GUI;  
    x->bpm                  = PIZ_DEFAULT_BPM;  
    x->runInQueue           = pizLinklistNew ( );
    x->runOutQueue          = pizLinklistNew ( );
    x->graphicInQueue       = pizLinklistNew ( );
    x->graphicOutQueue      = pizLinklistNew ( );
    x->mainQueue            = pizLinklistNew ( );
    x->notifyQueue          = pizLinklistNew ( );
    x->sequence             = pizSequenceNew (0);
    x->tempArray            = pizGrowingArrayNew (0);
    x->err1                 = PIZ_ERROR;
    x->err2                 = PIZ_ERROR;
    
    if (!(x->runInQueue &&  
        x->runOutQueue && 
        x->graphicInQueue &&
        x->graphicOutQueue &&
        x->mainQueue &&
        x->notifyQueue && 
        x->sequence &&
        x->tempArray)) {
        
        err |= PIZ_MEMORY;
    }
    
    err |= pthread_mutex_init (&x->eventLock, NULL);
    err |= pthread_mutex_init (&x->notificationLock, NULL);
    err |= pthread_mutex_init (&x->getterLock, NULL);
    
    err |= pthread_cond_init  (&x->eventCondition, NULL);
    err |= pthread_cond_init  (&x->notificationCondition, NULL);
    
    err |= pthread_attr_init  (&x->attr);
    
    if (!err) {
        pthread_attr_setscope        (&x->attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setinheritsched (&x->attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setdetachstate  (&x->attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setschedpolicy  (&x->attr, SCHED_OTHER);
        
        x->err1 = pthread_create (&x->eventLoop, &x->attr, pizAgentEventLoop, (void *)x); 
        err |= x->err1;
        
        x->err2 = pthread_create (&x->notificationLoop, &x->attr, pizAgentNotificationLoop, (void *)x); 
        err |= x->err2;
    }
    
    if (err) {
        pizAgentFree (x);
        x = NULL;
    } else if (event = pizEventNew (PIZ_EVENT_RUN, PIZ_EVENT_INIT)) {
        pizAgentAddEvent (x, event);
    }
    //
    }
    
    return x;
}

void pizAgentFree (PIZAgent *x)
{ 
    if (x) {
    //
    if (!x->err1) {
        PIZAGENTLOCKEVENT
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZAGENTUNLOCKEVENT
    
        pthread_cond_signal (&x->eventCondition);
        pthread_join (x->eventLoop, NULL); 
    }
    
    if (!x->err2) {
        PIZAGENTLOCKNOTIFICATION
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZAGENTUNLOCKNOTIFICATION
    
        pthread_cond_signal (&x->notificationCondition);
        pthread_join (x->notificationLoop, NULL); 
    }
    
    pthread_attr_destroy  (&x->attr);
    
    pthread_mutex_destroy (&x->eventLock);
    pthread_mutex_destroy (&x->notificationLock);
    pthread_mutex_destroy (&x->getterLock);
    
    pthread_cond_destroy  (&x->eventCondition);
    pthread_cond_destroy  (&x->notificationCondition);
    
    pizLinklistFree (x->runInQueue);
    pizLinklistFree (x->runOutQueue);
    pizLinklistFree (x->graphicInQueue);
    pizLinklistFree (x->graphicOutQueue);
    pizLinklistFree (x->mainQueue);
    pizLinklistFree (x->notifyQueue);
    
    pizSequenceFree     (x->sequence);
    pizGrowingArrayFree (x->tempArray);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentAddEvent (PIZAgent *x, PIZEvent *event)
{
    if (event) {
        PIZLinklist *queue = NULL;
        
        switch (event->type) {
            case PIZ_EVENT_RUN            : queue = x->runInQueue; break;
            case PIZ_EVENT_TRANSFORMATION : queue = x->mainQueue; break;
            case PIZ_EVENT_GRAPHIC        : if (x->flags & PIZ_AGENT_FLAG_GUI) { 
                                                queue = x->graphicInQueue; 
                                            } break;
        }
        
        if (queue) {
            PIZAGENTLOCKEVENT
            if (pizLinklistAppend (queue, event)) {
                pizEventFree (event);
            }
            PIZAGENTUNLOCKEVENT
            pthread_cond_signal (&x->eventCondition);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
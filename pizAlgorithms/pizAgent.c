/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	April 9, 2012.
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
    
    x->flags                = PIZ_AGENT_FLAG_NONE;  
    x->bpm                  = PIZ_DEFAULT_BPM;  
    x->runInQueue           = pizLinklistNew ( );
    x->runOutQueue          = pizLinklistNew ( );
    x->graphicInQueue       = pizLinklistNew ( );
    x->graphicOutQueue      = pizLinklistNew ( );
    x->transformQueue       = pizLinklistNew ( );
    x->notifyQueue          = pizLinklistNew ( );
    x->sequence             = pizSequenceNew      (0);
    x->factorOracle         = pizFactorOracleNew  (0, NULL);
    x->galoisLattice        = pizGaloisLatticeNew (0, NULL);
    x->err1                 = PIZ_ERROR;
    x->err2                 = PIZ_ERROR;
    
    if (!(x->runInQueue     &&  
        x->runOutQueue      && 
        x->graphicInQueue   &&
        x->graphicOutQueue  &&
        x->transformQueue   &&
        x->notifyQueue      && 
        x->sequence         &&
        x->factorOracle     &&
        x->galoisLattice)) {
        
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
    } else if (event = pizEventNewRun (PIZ_EVENT_INIT)) {
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
        PIZAGENTLOCK_EVENT
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZAGENTUNLOCK_EVENT
    
        pthread_cond_signal (&x->eventCondition);
        pthread_join (x->eventLoop, NULL); 
    }
    
    if (!x->err2) {
        PIZAGENTLOCK_NOTIFICATION
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZAGENTUNLOCK_NOTIFICATION
    
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
    pizLinklistFree (x->transformQueue);
    pizLinklistFree (x->notifyQueue);
    
    pizSequenceFree (x->sequence);
    
    pizFactorOracleFree   (x->factorOracle);
    pizGaloisLatticeFree  (x->galoisLattice);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentAddEvent (PIZAgent *x, PIZEvent *event)
{
    PIZLinklist *queue = NULL;
    
    switch (event->type) {
        case PIZ_EVENT_RUN       : queue = x->runInQueue; break;
        case PIZ_EVENT_TRANSFORM : queue = x->transformQueue; break;
        case PIZ_EVENT_GRAPHIC   : queue = x->graphicInQueue; break;
    }
    
    if (queue) {
        PIZAGENTLOCK_EVENT
        PIZAGENTQUEUE(queue)
        PIZAGENTUNLOCK_EVENT
        
        pthread_cond_signal (&x->eventCondition);
    }
}

PIZError pizAgentGetEvent (PIZAgent *x, PIZEventType type, PIZEvent **eventPtr)
{
    PIZError err = PIZ_ERROR;
    PIZLinklist *queue = NULL;
        
    switch (type) {
        case PIZ_EVENT_RUN     : queue = x->runOutQueue; break;
        case PIZ_EVENT_GRAPHIC : queue = x->graphicOutQueue; break;
    }
    
    if (queue) {
        err = PIZ_GOOD;
        
        PIZAGENTLOCK_GETTER
        
        /*
        if (!(err |= pizLinklistPtrAtIndex (queue, 0, (void **)eventPtr))) {
            pizLinklistChuckByPtr (queue, (*eventPtr));
        }*/
        
        PIZAGENTUNLOCK_GETTER
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
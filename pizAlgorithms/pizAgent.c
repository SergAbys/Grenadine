/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	May 5, 2012.
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

#define PIZ_DEFAULT_BPM 120

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZAgent *pizAgentNew (void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc (sizeof(PIZAgent))) {
    //
    long err = PIZ_GOOD;
    
    x->bpm              = PIZ_DEFAULT_BPM;  
    x->flags            = PIZ_AGENT_FLAG_INIT; 
    x->run              = pizLinklistNew ( );
    x->graphic          = pizLinklistNew ( );
    x->transform        = pizLinklistNew ( );
    x->notification     = pizLinklistNew ( );    
    x->observer         = pizLinklistNew ( );
    x->sequence         = pizSequenceNew      (0);
    x->factorOracle     = pizFactorOracleNew  (0, NULL);
    x->galoisLattice    = pizGaloisLatticeNew (0, NULL);
    x->err1             = PIZ_ERROR;
    x->err2             = PIZ_ERROR;
    
    if (!(x->run        &&  
        x->graphic      &&
        x->transform    &&
        x->notification && 
        x->observer     &&
        x->sequence     &&
        x->factorOracle &&
        x->galoisLattice)) {
        
        err |= PIZ_MEMORY;
    }
    
    err |= pthread_mutex_init (&x->eventLock, NULL);
    err |= pthread_mutex_init (&x->notificationLock, NULL);
    err |= pthread_mutex_init (&x->observerLock, NULL);
    
    err |= pthread_cond_init  (&x->eventCondition, NULL);
    err |= pthread_cond_init  (&x->notificationCondition, NULL);
    
    err |= pthread_attr_init  (&x->attr);
    
    if (!err) {
    //

    pthread_attr_setscope        (&x->attr, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setdetachstate  (&x->attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy  (&x->attr, SCHED_OTHER);
    
    x->err1 = (pthread_create (&x->eventLoop, &x->attr, pizAgentEventLoop, (void *)x) != 0); 
    err |= x->err1;
    
    x->err2 = (pthread_create (&x->notificationLoop, &x->attr, pizAgentNotificationLoop, (void *)x) != 0); 
    err |= x->err2;
    //
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
    if (!x->err1) {
        PIZ_AGENT_LOCK_EVENT
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZ_AGENT_UNLOCK_EVENT
        pthread_cond_signal (&x->eventCondition);
        
        pthread_join (x->eventLoop, NULL); 
    }
    
    if (!x->err2) {
        PIZ_AGENT_LOCK_NOTIFICATION
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZ_AGENT_UNLOCK_NOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
        
        pthread_join (x->notificationLoop, NULL); 
    }
    
    pthread_attr_destroy  (&x->attr);
    
    pthread_mutex_destroy (&x->eventLock);
    pthread_mutex_destroy (&x->notificationLock);
    pthread_mutex_destroy (&x->observerLock);
    
    pthread_cond_destroy  (&x->eventCondition);
    pthread_cond_destroy  (&x->notificationCondition);
    
    pizLinklistFree (x->run);
    pizLinklistFree (x->graphic);
    pizLinklistFree (x->transform);
    pizLinklistFree (x->notification);
    pizLinklistFree (x->observer);
    
    pizSequenceFree (x->sequence);
    
    pizFactorOracleFree   (x->factorOracle);
    pizGaloisLatticeFree  (x->galoisLattice);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentAttach (PIZAgent *x, void *observer, PIZMethod f)
{
    PIZError err = PIZ_ERROR;
    
    if (observer && f) {
    //
    PIZObserver *newObserver = NULL;
    
    if (newObserver = (PIZObserver *)malloc (sizeof(PIZObserver))) {
    //
    err = PIZ_GOOD;
    
    newObserver->observer = observer;
    newObserver->notify = f;
    
    PIZ_AGENT_LOCK_OBSERVER
    
    err = pizLinklistAppend (x->observer, newObserver);
        
    PIZ_AGENT_UNLOCK_OBSERVER
    
    //
    } else {
        err = PIZ_MEMORY;
    }
    
    if (err == PIZ_MEMORY) {
        PIZ_AGENT_MEMORY
    }
    //
    }
    
    return err;
}

PIZError pizAgentDetach (PIZAgent *x, void *observer)
{
    PIZError    err = PIZ_ERROR;
    PIZObserver *ptr = NULL;
    PIZObserver *nextPtr = NULL;
    
    PIZ_AGENT_LOCK_OBSERVER
    
    pizLinklistPtrAtIndex (x->observer, 0, (void **)&ptr);
    
    while (ptr) {
        pizLinklistNextByPtr (x->observer, (void *)ptr, (void **)&nextPtr);
        
        if (ptr->observer == observer) {
            err = pizLinklistRemoveByPtr (x->observer, (void *)ptr);
        }
        
        ptr = nextPtr;
    }
    
    PIZ_AGENT_UNLOCK_OBSERVER
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentAddEvent (PIZAgent *x, PIZEvent *event)
{
    PIZLinklist *q = NULL;
    
    switch (event->type) {
        case PIZ_EVENT_RUN       : q = x->run;         break;
        case PIZ_EVENT_TRANSFORM : q = x->transform;   break;
        case PIZ_EVENT_ATTRIBUTE : q = x->transform;   break;
        case PIZ_EVENT_GRAPHIC   : q = x->graphic;     break;
    }
    
    if (q) {
        PIZ_AGENT_LOCK_EVENT
        PIZ_AGENT_QUEUE (q, event)
        PIZ_AGENT_UNLOCK_EVENT
        pthread_cond_signal (&x->eventCondition);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
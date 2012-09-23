/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
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
#pragma mark -
#pragma mark ---
#pragma mark -

PIZAgent *pizAgentNew(void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc(sizeof(PIZAgent))) {
    //
    long err = PIZ_GOOD;
    
    x->flags        = PIZ_AGENT_FLAG_INIT; 
    x->run          = pizLinklistNew( );
    x->low          = pizLinklistNew( );
    x->high         = pizLinklistNew( );
    x->sequence     = pizSequenceNew(x);
    x->learn        = pizArrayNew(0);
    x->oracle       = pizFactorOracleNew(0, NULL);
    x->lattice      = pizGaloisLatticeNew(0, NULL);
    x->observer     = NULL;
    x->callback     = NULL;
    x->error        = PIZ_ERROR;
    x->seed         = PIZ_SEED;
    
    if (!(x->run    &&  
        x->low      &&
        x->high     &&
        x->sequence &&
        x->learn    &&
        x->oracle   &&
        x->lattice )) { err |= PIZ_MEMORY; }
    
    err |= pthread_attr_init(&x->attribute);
    err |= pthread_cond_init(&x->condition, NULL);
    err |= pthread_mutex_init(&x->eventLock, NULL);
    err |= pthread_mutex_init(&x->observerLock, NULL);
    
    if (!err) {
    //
    pthread_attr_setscope(&x->attribute, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setdetachstate(&x->attribute, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy(&x->attribute, SCHED_OTHER);
    
    x->error = (pthread_create(&x->eventLoop, &x->attribute, pizAgentEventLoop, (void *)x) != 0); 
    err |= x->error;
    //
    }
    
    if (err) {
        pizAgentFree(x);
        x = NULL;
    }
    //
    }
    
    return x;
}

void pizAgentFree(PIZAgent *x)
{ 
    if (x) {
    //
    if (!x->error) {
        PIZ_AGENT_LOCK_EVENT
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZ_AGENT_UNLOCK_EVENT
        
        pthread_cond_signal(&x->condition);
        pthread_join(x->eventLoop, NULL); 
    }
    
    pthread_attr_destroy(&x->attribute);
    pthread_cond_destroy(&x->condition);
    pthread_mutex_destroy(&x->eventLock);
    pthread_mutex_destroy(&x->observerLock);
    
    pizArrayFree(x->learn);
    
    pizFactorOracleFree(x->oracle);
    pizGaloisLatticeFree(x->lattice);
    
    pizLinklistFree(x->run);
    pizLinklistFree(x->low);
    pizLinklistFree(x->high);

    pizSequenceFree(x->sequence);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentAttach(PIZAgent *x, void *observer, PIZCallback callback)
{
    PIZError err = PIZ_ERROR;
    
    if (observer && callback) {
    //
    PIZ_AGENT_LOCK_OBSERVER
    
    x->observer = observer;
    x->callback = callback;
    
    PIZ_AGENT_UNLOCK_OBSERVER

    err = PIZ_GOOD;
    //
    }
    
    return err;
}

PIZError pizAgentDetach(PIZAgent *x, void *observer)
{
    PIZ_AGENT_LOCK_OBSERVER
        
    x->observer = NULL;
    x->callback = NULL;
        
    PIZ_AGENT_UNLOCK_OBSERVER
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentDoEvent(PIZAgent *x, PIZEvent *event)
{
    PIZLinklist *q = NULL;
        
    switch (event->type) {
        case PIZ_EVENT_RUN  : q = x->run;  break;
        case PIZ_EVENT_LOW  : q = x->low;  break;
        case PIZ_EVENT_HIGH : q = x->high; break;
    }
    
    if (q) {
        PIZError err = PIZ_GOOD;

        PIZ_AGENT_LOCK_EVENT
        err = pizLinklistAppend(q, event);
        PIZ_AGENT_UNLOCK_EVENT
        
        pthread_cond_signal(&x->condition);
        
        if (err) {
            pizEventFree(event);
        }
        
        if (err & PIZ_MEMORY) {
            PIZ_AGENT_BAD_MEMORY
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	March 16, 2012.
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZAgent *pizAgentNew (void)
{
    PIZAgent *x = NULL;
    
    if (x = (PIZAgent *)malloc (sizeof(PIZAgent))) {
    //
    long err = PIZ_GOOD;
    
    x->runQueue = pizLinklistNew ( );
    
    err |= pthread_mutex_init (&x->eventMutex, NULL);
    err |= pthread_cond_init  (&x->eventCondition, NULL);
    err |= pthread_attr_init  (&x->attr);
    
    if (!err) {
        pthread_attr_setdetachstate (&x->attr, PTHREAD_CREATE_JOINABLE);
        x->eventLoopErr = pthread_create (&x->eventLoop, &x->attr, pizAgentEventLoop, (void *)x); 
        err |= x->eventLoopErr;
    }
        
    if (!err && x->runQueue) {
        x->flags    = PIZ_AGENT_FLAG_NONE;  
        x->tempo    = PIZ_DEFAULT_TEMPO;  
        x->quantum  = PIZ_BPM_CONSTANT / (double)x->tempo;
    } else {
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
        x->flags |= PIZ_AGENT_FLAG_EXIT;
        PIZUNLOCKEVENT
    
        pthread_cond_signal (&x->eventCondition);
        pthread_join (x->eventLoop, NULL); 
    }
    
    pthread_attr_destroy  (&x->attr);
    pthread_mutex_destroy (&x->eventMutex);
    pthread_cond_destroy  (&x->eventCondition);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *pizAgentEventLoop (void *agent) 
{
    PIZAgent *x = agent;
    
    while (!EXIT) {
    //
    PIZLOCKEVENT
    
    while (!(pizLinklistCount (x->runQueue))) {
        pthread_cond_wait (&x->eventCondition, &x->eventMutex); 
                
        if (EXIT) {
            break;
        } 
    }
    
    PIZUNLOCKEVENT
    
    if (!EXIT) {
        PIZEvent *event = NULL;
        
        PIZLOCKEVENT
        if (!pizLinklistPtrAtIndex (x->runQueue, 0, (void *)&event)) {
            post ("Dequeue / %s", __FUNCTION__);
            pizLinklistRemoveByPtr (x->runQueue, event);
        }
        PIZUNLOCKEVENT
    }
    
    if (!EXIT) {
        sleep (1);
    }
    //    
    }
    
    pthread_exit (NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizAgentAppendEvent (PIZAgent *x, PIZEvent *event)
{
    if (event) {
        PIZLOCKEVENT
        pizLinklistAppend (x->runQueue, event);
        PIZUNLOCKEVENT
        pthread_cond_signal (&x->eventCondition);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
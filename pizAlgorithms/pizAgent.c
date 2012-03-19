/*
 * \file	pizAgent.c
 * \author	Jean Sapristi
 * \date	March 19, 2012.
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

#define EXIT (x->flags & PIZ_FLAG_EXIT)
#define INIT (x->flags & PIZ_FLAG_INIT)

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
        
    x->runQueue = pizLinklistNew ( );
    
    if (!x->runQueue) {
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
    
    pizLinklistFree (x->runQueue);
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
        x->flags |= PIZ_FLAG_INIT;
                
        if (EXIT) {
            break;
        } 
    }
    
    PIZUNLOCKEVENT
    
    if (!EXIT && !pizAgentEventLoopInit (x)) {
    //
    while (pizAgentEventLoopIsWorkTime (x)) {
        if (pizAgentEventLoopDoRunEvent (x)) {
            break;
        } 
    }
    
    pizAgentEventLoopSleep (x);
    
    sleep (1);
    //
    }
    //    
    }
    
    pthread_exit (NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

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
        pizTimeIncrement (&x->grainEnd, &x->grainSize);
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
    
    if (!pizTimeElapsed (&x->grainStart, &now, &elapsed)) {
        if (elapsed < x->grainWorkSize) {
            isWorkTime = true;
        }
    }
    
    return isWorkTime;
}

void pizAgentEventLoopSleep (PIZAgent *x)
{
    PIZTime  now;
    PIZNano  ns;
    PIZError err = PIZ_GOOD;
    
    pizTimeSet (&now);
    err = pizTimeElapsed (&now, &x->grainEnd, &ns);
    
    while (err) {
        pizTimeIncrement (&x->grainEnd, &x->grainSize); 
        err = pizTimeElapsed (&now, &x->grainEnd, &ns);
    }
    
    //############### nanosleep;
}

PIZError pizAgentEventLoopDoRunEvent (PIZAgent *x) 
{
    long     err = PIZ_ERROR;
    PIZEvent *event = NULL;
            
    PIZLOCKEVENT
    
    if (!pizLinklistPtrAtIndex (x->runQueue, 0, (void *)&event)) {
        pizLinklistRemoveByPtr (x->runQueue, event);
    }
    
    if (pizLinklistCount (x->runQueue)) {
        err = PIZ_GOOD;
    }
    
    PIZUNLOCKEVENT
    
    return err;
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
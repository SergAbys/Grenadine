/*
 * \file	pizAgentLoop.c
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

#include "pizAgentLoop.h"
#include "pizAgentMethods.h"
#include "pizSequenceRun.h"
#include "pizSequenceUser.h"
#include "pizSequenceMethods.h"
#include "pizSequenceAttributes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <errno.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const PIZMethodError pizEventMethods[ ]  = { NULL,
                                                    pizAgentInit,
                                                    pizAgentPlay,
                                                    pizAgentStop,
                                                    pizAgentLoop,
                                                    pizAgentUnloop,
                                                    //
                                                    pizAgentLearn,
                                                    pizAgentForget,
                                                    //
                                                    pizAgentSetBpm,
                                                    pizSequenceSetChance,
                                                    pizSequenceSetVelocity,
                                                    pizSequenceSetChannel,
                                                    pizSequenceSetChord,
                                                    pizSequenceSetCell,
                                                    pizSequenceSetValue,
                                                    pizSequenceSetScale,
                                                    pizSequenceSetMute,
                                                    pizSequenceSetZone,
                                                    //
                                                    pizSequenceNote,
                                                    pizSequenceClear,
                                                    pizSequenceClean,
                                                    pizSequenceTranspose,
                                                    pizSequenceRotate,
                                                    pizSequenceScramble,
                                                    pizSequenceSort,
                                                    pizSequenceChange,
                                                    pizSequenceFill,
                                                    pizSequenceKill,
                                                    pizSequenceCycle,
                                                    pizSequencePattern,
                                                    pizSequenceAlgorithm,
                                                    pizSequenceAlgorithm,
                                                    pizSequenceJuliet,
                                                    //
                                                    pizSequenceDump,
                                                    pizSequenceStatistics,
                                                    pizSequenceAttributes,
                                                    //
                                                    pizSequenceCellUp,
                                                    pizSequenceCellDown,
                                                    pizSequenceCellRight,
                                                    pizSequenceCellLeft,
                                                    pizSequenceValueUp,
                                                    pizSequenceValueDown,
                                                    pizSequenceValueRight,
                                                    pizSequenceValueLeft,
                                                    pizSequenceNoteDelete,
                                                    pizSequenceNoteIncrement,
                                                    pizSequenceNoteDecrement,
                                                    pizSequenceNoteForward,
                                                    pizSequenceNoteBackward,
                                                    pizSequenceZoneIncrement,
                                                    pizSequenceZoneDecrement,
                                                    pizSequenceChannelIncrement,
                                                    pizSequenceChannelDecrement
                                                    };             
                                                                                                    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_EXIT (x->flags & PIZ_AGENT_FLAG_EXIT)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL PIZError  pizAgentEventLoopDoEvent        (PIZAgent *x, PIZLinklist *q);
PIZ_LOCAL void      pizAgentEventLoopDoStep         (PIZAgent *x, bool blank);
PIZ_LOCAL void      pizAgentEventLoopDoRefresh      (PIZAgent *x);

PIZ_LOCAL void      pizAgentEventLoopInit           (PIZAgent *x);
PIZ_LOCAL void      pizAgentEventLoopSleep          (PIZAgent *x);
PIZ_LOCAL bool      pizAgentEventLoopIsCondition    (PIZAgent *x);
PIZ_LOCAL bool      pizAgentEventLoopIsWorkTime     (PIZAgent *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void *pizAgentEventLoop(void *agent) 
{
    PIZAgent *x = (PIZAgent *)agent;  
    
    while (!PIZ_EXIT) { 
    //
    
    PIZ_AGENT_LOCK_EVENT
    
    while (!(pizAgentEventLoopIsCondition(x))) {
        pthread_cond_wait(&x->condition, &x->eventLock);
        x->flags |= PIZ_AGENT_FLAG_INIT;
        if (PIZ_EXIT) { 
            break; 
        } 
    }
    
    PIZ_AGENT_UNLOCK_EVENT
        
    if (!PIZ_EXIT) {
    //
    pizAgentEventLoopInit(x);
     
    while (pizAgentEventLoopIsWorkTime(x)) {
        if (pizAgentEventLoopDoEvent(x, x->run)) {
            break;
        }
    }
    
    while (pizAgentEventLoopIsWorkTime(x)) {
        if (pizAgentEventLoopDoEvent(x, x->high)) {
            break;
        } 
    }
    
    if (x->flags & PIZ_AGENT_FLAG_RUNNING) {
        if (pizAgentEventLoopIsWorkTime(x)) {
            pizAgentEventLoopDoStep(x, 0);
        } else {
            pizAgentEventLoopDoStep(x, 1);
        }
    }

    while (pizAgentEventLoopIsWorkTime(x)) {
        if (pizAgentEventLoopDoEvent(x, x->low)) {
            pizAgentEventLoopDoRefresh(x);
            break;
        } 
    }
    
    pizAgentEventLoopSleep(x); 
    //    
    }
    //
    }

    pthread_exit(NULL);
}

PIZError pizAgentNotify(PIZAgent *x, PIZEventCode n, long ac, long *av)
{
    PIZEvent *notification = NULL;

    if (notification = pizEventNew(n)) {
    //
    pizEventSetData(notification, ac, av);
    pizEventSetTime(notification, &x->grainStart);
 
    PIZ_AGENT_LOCK_OBSERVER
        
    if (x->observer && x->callback) {
        (*x->callback)(x->observer, notification);
    }
    
    PIZ_AGENT_UNLOCK_OBSERVER
    
    return PIZ_GOOD;
    //
    }
    
    return PIZ_MEMORY;
} 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizAgentEventLoopDoEvent(PIZAgent *x, PIZLinklist *q) 
{
    void *o = NULL;
    PIZEvent *event = NULL;
    PIZError err = PIZ_GOOD;
    PIZMethodError f = NULL;
    PIZEventCode code = PIZ_MESSAGE_NONE;
    
    PIZ_AGENT_LOCK_EVENT
    
    if (!(pizLinklistPtrAtIndex(q, 0, (void **)&event))) {
        pizLinklistChuckWithPtr(q, event);
    }
    
    if (!(pizLinklistCount(q))) {
        err = PIZ_ERROR;
    }
    
    PIZ_AGENT_UNLOCK_EVENT
                  
    if (event) {
    //
    pizEventCode(event, &code);
    
    if (code < PIZ_MESSAGE_CHANCE) {
        o = x;
    } else {
        o = x->sequence;
    }
    
    if (o && (f = pizEventMethods[code]) && ((*f)(o, event) & PIZ_MEMORY)) {
        PIZ_AGENT_BAD_MEMORY
    }
        
    pizEventFree(event);
    //
    }
    
    return err;
}

void pizAgentEventLoopDoStep(PIZAgent *x, bool blank)
{   
    bool k = false;
    PIZError err1 = PIZ_GOOD; 
    PIZError err2 = PIZ_GOOD; 
    
    do {
    //
    if (!blank) {
        err1 = pizSequenceStep(x->sequence);
    } else {
        err1 = pizSequenceStepBlank(x->sequence); 
    }
    
    if (err1 == PIZ_GOOD) {
        if (pizSequenceIsAtEnd(x->sequence)) {
            err2 |= pizAgentNotify(x, PIZ_EVENT_WILL_END, 0, NULL);
        }
        k = false;  
        
    } else if (err1 == PIZ_ERROR) {
        if (x->flags & (PIZ_AGENT_FLAG_LOOPED | PIZ_AGENT_FLAG_REPLAY)) {
            k = true;
            x->flags &= ~PIZ_AGENT_FLAG_REPLAY;
        } else {
            k = false;
            x->flags &= ~PIZ_AGENT_FLAG_RUNNING;
        }
        
        pizSequenceJumpToStart(x->sequence);
        err2 |= pizAgentNotify(x, PIZ_EVENT_END, 0, NULL);
    } 
    
    if ((err1 | err2) & PIZ_MEMORY) { 
        PIZ_AGENT_BAD_MEMORY 
    }
    //
    } while (k);
}

void pizAgentEventLoopDoRefresh(PIZAgent *x)
{
    if (pizSequenceRefresh(x->sequence) & PIZ_MEMORY) {
        PIZ_AGENT_BAD_MEMORY
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentEventLoopInit(PIZAgent *x)
{   
    if (x->flags & PIZ_AGENT_FLAG_INIT) {
        pizTimeSet(&x->grainStart);
        x->flags &= ~PIZ_AGENT_FLAG_INIT;
        
    } else {
        pizTimeCopy(&x->grainStart, &x->grainEnd);
    }
    
    pizNanoSet(&x->grainSize, PIZ_AGENT_CONSTANT_BPM_NS / pizSequenceBpm(x->sequence));
    pizTimeCopy(&x->grainEnd, &x->grainStart);
    pizTimeAddNano(&x->grainEnd, &x->grainSize);
}

void pizAgentEventLoopSleep(PIZAgent *x)
{
    if (x->flags & PIZ_AGENT_FLAG_RUNNING) {
    //
    PIZNano ns;
    PIZTime now;
    struct timespec t0, t1;
    struct timespec *ptrA = &t0;
    struct timespec *ptrB = &t1;
    struct timespec *temp = NULL;
    
    PIZError err = PIZ_GOOD;
    
    pizTimeSet(&now);
    err = pizTimeElapsedNano(&now, &x->grainEnd, &ns);
    
    while (err) {
        pizTimeAddNano(&x->grainStart, &x->grainSize);
        pizTimeAddNano(&x->grainEnd, &x->grainSize); 
        
        pizAgentEventLoopDoStep(x, 1);
        
        if (!(x->flags & PIZ_AGENT_FLAG_RUNNING)) {
            x->flags |= PIZ_AGENT_FLAG_INIT;
            return;
        }
        
        err = pizTimeElapsedNano(&now, &x->grainEnd, &ns);
    }
    
    pizTimespecWithNano(ptrA, &ns);
    
    while ((nanosleep(ptrA, ptrB) == -1) && (errno == EINTR)) {
        temp = ptrA;
        ptrA = ptrB;
        ptrB = temp;
    }
    //
    } else {
        x->flags |= PIZ_AGENT_FLAG_INIT;
    }
}

bool pizAgentEventLoopIsCondition(PIZAgent *x)
{
    if ((x->flags & PIZ_AGENT_FLAG_RUNNING)
        || pizLinklistCount(x->run)  
        || pizLinklistCount(x->low)  
        || pizLinklistCount(x->high)) {
        return true;
    }
    
    return false;
}

bool pizAgentEventLoopIsWorkTime(PIZAgent *x)
{
    PIZTime now;
    PIZNano elapsed;
    PIZNano timeOut;
    bool isWorkTime = false;
    
    pizTimeSet(&now);
    pizNanoSet(&timeOut, PIZ_AGENT_CONSTANT_RATIO_WORK / pizSequenceBpm(x->sequence));
    
    if (!(pizTimeElapsedNano(&x->grainStart, &now, &elapsed))) {
        if (elapsed < timeOut) {
            isWorkTime = true;
        }
    }
    
    return isWorkTime;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
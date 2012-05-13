/**
 * \file	pizAgent.h
 * \author	Jean Sapristi
 * \date	May 12, 2012.
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

#ifndef PIZ_AGENT_H
#define PIZ_AGENT_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizEvent.h"
#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <pthread.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_CONSTANT_BPM              25.E8
#define PIZ_AGENT_CONSTANT_WORK_RATIO       25.E8 * 0.75

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_FLAG_NONE                 0UL
#define PIZ_AGENT_FLAG_EXIT                 1UL
#define PIZ_AGENT_FLAG_INIT                 2UL
#define PIZ_AGENT_FLAG_LOOPED               4UL
#define PIZ_AGENT_FLAG_REPLAY               8UL
#define PIZ_AGENT_FLAG_RUNNING              16UL
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_LOCK_EVENT                pthread_mutex_lock      (&x->eventLock);
#define PIZ_AGENT_UNLOCK_EVENT              pthread_mutex_unlock    (&x->eventLock);

#define PIZ_AGENT_LOCK_NOTIFICATION         pthread_mutex_lock      (&x->notificationLock);
#define PIZ_AGENT_UNLOCK_NOTIFICATION       pthread_mutex_unlock    (&x->notificationLock);

#define PIZ_AGENT_LOCK_OBSERVER             pthread_mutex_lock      (&x->observerLock);
#define PIZ_AGENT_UNLOCK_OBSERVER           pthread_mutex_unlock    (&x->observerLock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_MEMORY                    ;
#define PIZ_AGENT_QUEUE(queue, event)       if (pizLinklistAppend ((queue), (event))) {         \
                                                pizEventFree ((event));                         \
                                                PIZ_AGENT_MEMORY                                \
                                            }
                                        
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZAgent {
    long                bpm;
    ulong               flags;
    PIZNano             grainSize;
    PIZTime             grainStart;
    PIZTime             grainEnd;
    PIZLinklist         *run;
    PIZLinklist         *graphic;
    PIZLinklist         *transform;
    PIZLinklist         *notification;
    PIZSequence         *sequence;
    void                *observer;
    PIZMethod           notify;
    PIZFactorOracle     *factorOracle;
    PIZGaloisLattice    *galoisLattice;
    pthread_attr_t      attr;
    pthread_cond_t      eventCondition;
    pthread_cond_t      notificationCondition;
    pthread_mutex_t     eventLock;
    pthread_mutex_t     notificationLock;
    pthread_mutex_t     observerLock;
    pthread_t           eventLoop;
    pthread_t           notificationLoop;
    PIZError            err1;
    PIZError            err2;
    } PIZAgent;  

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZAgent    *pizAgentNew        (void);

void        pizAgentFree        (PIZAgent *x);
PIZError    pizAgentAttach      (PIZAgent *x, void *observer, PIZMethod f); 
PIZError    pizAgentDetach      (PIZAgent *x, void *observer);
void        pizAgentAddEvent    (PIZAgent *x, PIZEvent *event);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_AGENT_H
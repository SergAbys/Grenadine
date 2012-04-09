/**
 * \file	pizAgent.h
 * \author	Jean Sapristi
 * \date	April 5, 2012.
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
#include "pizFactorOracle.h"
#include "pizFiniteState.h"
#include "pizGaloisLattice.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <pthread.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_CONSTANT_BPM          25.E8
#define PIZ_AGENT_CONSTANT_WORK_RATIO   25.E8 * 0.75

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_FLAG_NONE             0UL
#define PIZ_AGENT_FLAG_GUI              1UL
#define PIZ_AGENT_FLAG_EXIT             2UL
#define PIZ_AGENT_FLAG_WAKED            4UL
#define PIZ_AGENT_FLAG_PLAYED           8UL
#define PIZ_AGENT_FLAG_LOOPED           16UL
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZAGENTLOCK_EVENT               pthread_mutex_lock      (&x->eventLock);
#define PIZAGENTUNLOCK_EVENT             pthread_mutex_unlock    (&x->eventLock);

#define PIZAGENTLOCK_NOTIFICATION        pthread_mutex_lock      (&x->notificationLock);
#define PIZAGENTUNLOCK_NOTIFICATION      pthread_mutex_unlock    (&x->notificationLock);

#define PIZAGENTLOCK_GETTER              pthread_mutex_lock      (&x->getterLock);
#define PIZAGENTUNLOCK_GETTER            pthread_mutex_unlock    (&x->getterLock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZAGENTMEMORY

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZAGENTQUEUE(queue)            if (pizLinklistAppend ((queue), event)) {       \
                                            pizEventFree (event);                       \
                                            PIZAGENTMEMORY                              \
                                        }
            
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#define DEBUGEVENT                      if (event) {                                                    \
                                            post ("%s / %s", pizEventGetName (event), __FUNCTION__);    \
                                        }

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZAgent {
    ulong               flags;
    long                bpm;
    PIZNano             grainSize;
    PIZTime             grainStart;
    PIZTime             grainEnd;
    PIZLinklist         *runInQueue;
    PIZLinklist         *runOutQueue;
    PIZLinklist         *graphicInQueue;
    PIZLinklist         *graphicOutQueue;
    PIZLinklist         *mainQueue;
    PIZLinklist         *notifyQueue;
    PIZSequence         *sequence;
    PIZFactorOracle     *factorOracle;
    PIZGaloisLattice    *galoisLattice;
    pthread_attr_t      attr;
    pthread_cond_t      eventCondition;
    pthread_cond_t      notificationCondition;
    pthread_mutex_t     eventLock;
    pthread_mutex_t     notificationLock;
    pthread_mutex_t     getterLock;
    pthread_t           eventLoop;
    pthread_t           notificationLoop;
    long                err1;
    long                err2;
    } PIZAgent;  

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef void (*PIZAgentMethod)(PIZAgent *x, PIZEvent *event);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZAgent *pizAgentNew     (void);
void     pizAgentFree     (PIZAgent *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void     pizAgentAddEvent (PIZAgent *x, PIZEvent *event);
PIZError pizAgentGetEvent (PIZAgent *x, PIZEventType type, PIZEvent **eventPtr);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_AGENT_H
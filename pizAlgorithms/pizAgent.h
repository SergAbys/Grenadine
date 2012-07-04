/**
 * \file	pizAgent.h
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

#define PIZ_AGENT_CONSTANT_BPM_NS       25.E8
#define PIZ_AGENT_CONSTANT_BPM_MS       25.E2
#define PIZ_AGENT_CONSTANT_RATIO_WORK   25.E8 * 0.75
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_FLAG_NONE             0UL
#define PIZ_AGENT_FLAG_EXIT             1UL
#define PIZ_AGENT_FLAG_INIT             2UL
#define PIZ_AGENT_FLAG_LOOPED           4UL
#define PIZ_AGENT_FLAG_REPLAY           8UL
#define PIZ_AGENT_FLAG_RUNNING          16UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_LOCK_EVENT            pthread_mutex_lock(&x->eventLock);
#define PIZ_AGENT_UNLOCK_EVENT          pthread_mutex_unlock(&x->eventLock);

#define PIZ_AGENT_LOCK_OBSERVER         pthread_mutex_lock(&x->observerLock);
#define PIZ_AGENT_UNLOCK_OBSERVER       pthread_mutex_unlock(&x->observerLock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_AGENT_MEMORY

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _PIZAgent {
    long                identifier;
    long                bpm;
    ulong               flags;
    PIZNano             grainSize;
    PIZTime             grainStart;
    PIZTime             grainEnd;
    PIZLinklist         *run;
    PIZLinklist         *low;
    PIZLinklist         *high;
    PIZSequence         *sequence;
    PIZArray            *buffer;
    PIZFactorOracle     *factorOracle;
    PIZGaloisLattice    *galoisLattice;
    void                *observer;
    PIZMethod           notify;
    pthread_attr_t      attr;
    pthread_cond_t      condition;
    pthread_mutex_t     eventLock;
    pthread_mutex_t     observerLock;
    pthread_t           eventLoop;
    PIZError            error;
    uint                seed;
    } PIZAgent;  

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_START_C_LINKAGE

PIZAgent    *pizAgentNew        (long identifier);

void        pizAgentFree        (PIZAgent *x);
PIZError    pizAgentAttach      (PIZAgent *x, void *observer, PIZMethod f); 
PIZError    pizAgentDetach      (PIZAgent *x, void *observer);
void        pizAgentAddEvent    (PIZAgent *x, PIZEvent *event);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_AGENT_H
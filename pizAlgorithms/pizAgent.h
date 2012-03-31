/**
 * \file	pizAgent.h
 * \author	Jean Sapristi
 * \date	March 31, 2012.
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

#define PIZ_AGENT_CONSTANT_BPM          25.E8
#define PIZ_AGENT_CONSTANT_WORK_RATIO   25.E8 * 0.75

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZAGENTLOCKEVENT               pthread_mutex_lock      (&x->eventLock);
#define PIZAGENTUNLOCKEVENT             pthread_mutex_unlock    (&x->eventLock);

#define PIZAGENTLOCKNOTIFICATION        pthread_mutex_lock      (&x->notificationLock);
#define PIZAGENTUNLOCKNOTIFICATION      pthread_mutex_unlock    (&x->notificationLock);

#define PIZAGENTLOCKGETTER              pthread_mutex_lock      (&x->getterLock);
#define PIZAGENTUNLOCKGETTER            pthread_mutex_unlock    (&x->getterLock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DEBUGEVENT                      if (event) {                                                        \
                                            post ("%s / %s", pizEventNameAsString (event), __FUNCTION__);   \
                                        }
                                        
#define DEBUGTIME                       PIZTime ttt;                                                        \
                                        pizTimeSet (&ttt);                                                  \
                                        post ("%llu / %s", ttt, __FUNCTION__);                              \

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZAgentFlag {
    PIZ_FLAG_NONE       = 0,
    PIZ_FLAG_GUI        = 1,
    PIZ_FLAG_EXIT       = 2,
    PIZ_FLAG_WAKED      = 4,
    PIZ_FLAG_PLAYED     = 8,
    PIZ_FLAG_LOOPED     = 16
    } PIZAgentFlag;

typedef struct _PIZAgent {
    long                flags;
    long                bpm;
    PIZNano             grainSize;
    PIZNano             grainWorkSize;
    PIZTime             grainStart;
    PIZTime             grainEnd;
    PIZLinklist         *runIn;
    PIZLinklist         *graphicIn;
    PIZLinklist         *transformIn;
    PIZLinklist         *runOut;
    PIZLinklist         *graphicOut;
    PIZLinklist         *notificationOut;
    PIZSequence         *sequence;
    PIZGrowingArray     *tempArray;
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

PIZAgent *pizAgentNew       (void);
void     pizAgentFree       (PIZAgent *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void     pizAgentAddEvent   (PIZAgent *x, PIZEvent *event);
PIZError pizAgentGetEvent   (PIZAgent *x, PIZEventType, PIZEvent **eventPtr);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_AGENT_H
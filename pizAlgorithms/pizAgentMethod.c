/*
 * \file	pizAgentMethod.c
 * \author	Jean Sapristi
 * \date	May 1, 2012.
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

#include "pizAgentMethod.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_MAXIMUM_BPM     300
#define PIZ_MINIMUM_BPM     40

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentPlay (PIZAgent *x, PIZEvent *event)
{
    if (x->flags & PIZ_AGENT_FLAG_RUNNING) {
        x->flags |= PIZ_AGENT_FLAG_REPLAY;
    } else {
        pizSequenceGoToStart (x->sequence);
        x->flags |= PIZ_AGENT_FLAG_RUNNING; 
    }
}

void pizAgentStop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~(PIZ_AGENT_FLAG_LOOPED | PIZ_AGENT_FLAG_REPLAY | PIZ_AGENT_FLAG_RUNNING);
}

void pizAgentLoop (PIZAgent *x, PIZEvent *event)
{
    x->flags |= PIZ_AGENT_FLAG_LOOPED;
    pizAgentPlay (x, event);
}

void pizAgentUnloop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_AGENT_FLAG_LOOPED;
}

void pizAgentBPM (PIZAgent *x, PIZEvent *event)
{
    long     value;
    PIZEvent *notification = NULL;
    
    pizEventValue (event, &value);
    value = CLAMP (value, PIZ_MINIMUM_BPM, PIZ_MAXIMUM_BPM);
    
    if (x->bpm != value) {
    //
    x->bpm = value;
    
    if (notification = pizEventWithValue (PIZ_EVENT_BPM_CHANGED, value)) {
    
        PIZ_AGENT_LOCK_NOTIFICATION
        PIZ_AGENT_QUEUE (x->notification, notification)
        PIZ_AGENT_UNLOCK_NOTIFICATION
        pthread_cond_signal (&x->notificationCondition);
        
    } else {
        PIZ_AGENT_MEMORY
    }
        
    pizTimeSetNano (&x->grainSize, PIZ_AGENT_CONSTANT_BPM / x->bpm);    
    pizTimeCopy    (&x->grainEnd, &x->grainStart);
    pizTimeAddNano (&x->grainEnd, &x->grainSize);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
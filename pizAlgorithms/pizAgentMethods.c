/*
 * \file	pizAgentMethods.c
 * \author	Jean Sapristi
 * \date	May 4, 2012.
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

#include "pizAgentMethods.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_MINIMUM_BPM     40
#define PIZ_MAXIMUM_BPM     300

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizAgentInit (PIZAgent *x, PIZEvent *event)
{
    return PIZ_GOOD;
}

PIZError pizAgentPlay (PIZAgent *x, PIZEvent *event)
{
    if (x->flags & PIZ_AGENT_FLAG_RUNNING) {
        x->flags |= PIZ_AGENT_FLAG_REPLAY;
    } else {
        pizSequenceGoToStart (x->sequence);
        x->flags |= PIZ_AGENT_FLAG_RUNNING; 
    }
    
    return PIZ_GOOD;
}

PIZError pizAgentStop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~(PIZ_AGENT_FLAG_LOOPED | PIZ_AGENT_FLAG_REPLAY | PIZ_AGENT_FLAG_RUNNING);
    
    return PIZ_GOOD;
}

PIZError pizAgentLoop (PIZAgent *x, PIZEvent *event)
{
    x->flags |= PIZ_AGENT_FLAG_LOOPED;
    
    return (pizAgentPlay (x, event));
}

PIZError pizAgentUnloop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_AGENT_FLAG_LOOPED;
    
    return PIZ_GOOD;
}

PIZError pizAgentBPM (PIZAgent *x, PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
    //
    if ((value >= PIZ_MINIMUM_BPM) && (value <= PIZ_MAXIMUM_BPM) && (value != x->bpm)) {
        x->bpm = value;
        pizAgentAddNotification (x, PIZ_EVENT_CHANGED_BPM, -1, 1, &value);
            
        pizTimeSetNano (&x->grainSize, PIZ_AGENT_CONSTANT_BPM_NS / x->bpm);    
        pizTimeCopy    (&x->grainEnd, &x->grainStart);
        pizTimeAddNano (&x->grainEnd, &x->grainSize);
    }
    //
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
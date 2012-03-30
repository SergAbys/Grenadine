/*
 * \file	pizAgentMethod.c
 * \author	Jean Sapristi
 * \date	March 30, 2012.
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

#define PIZ_MAXIMUM_BPM   300
#define PIZ_MINIMUM_BPM   40

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizAgentMethodPlay (PIZAgent *x, PIZEvent *event)
{
    if (!(x->flags & PIZ_FLAG_PLAYED)) {
        pizSequenceGoToStart (x->sequence);
        x->flags |= PIZ_FLAG_PLAYED; 
        
        if (event->data.time) {
            pizTimeCopy    (&x->grainStart, &event->data.time);
            pizTimeCopy    (&x->grainEnd, &x->grainStart);
            pizTimeAddNano (&x->grainEnd, &x->grainSize);
        } 
    } 
}

void pizAgentMethodStop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_FLAG_PLAYED; 
    x->flags &= ~PIZ_FLAG_LOOPED;
}

void pizAgentMethodLoop (PIZAgent *x, PIZEvent *event)
{
    x->flags |= PIZ_FLAG_LOOPED;
    pizAgentMethodPlay (x, event);
}

void pizAgentMethodUnloop (PIZAgent *x, PIZEvent *event)
{
    x->flags &= ~PIZ_FLAG_LOOPED;
}

void pizAgentMethodBPM (PIZAgent *x, PIZEvent *event)
{
    x->bpm = CLAMP (event->data.values[0], PIZ_MINIMUM_BPM, PIZ_MAXIMUM_BPM);
        
    pizTimeSetNano (&x->grainSize, PIZ_AGENT_CONSTANT_BPM / x->bpm);
    pizTimeSetNano (&x->grainWorkSize, PIZ_AGENT_CONSTANT_WORK_RATIO / x->bpm);
    
    pizTimeCopy    (&x->grainEnd, &x->grainStart);
    pizTimeAddNano (&x->grainEnd, &x->grainSize);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
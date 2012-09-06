/*
 * \file	pizAgentRun.c
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

#include "pizAgentMethods.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_CONSTANT_LEARN 5

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizAgentInit(PIZAgent *x, const PIZEvent *event)
{
    pizSequenceInit(x->sequence);
    
    return PIZ_GOOD;
}

PIZError pizAgentPlay(PIZAgent *x, const PIZEvent *event)
{
    if (x->flags & PIZ_AGENT_FLAG_RUNNING) {
        x->flags |= PIZ_AGENT_FLAG_REPLAY;
        
    } else {
        PIZTime time;
        pizEventTime(event, &time);
        
        if (!(pizTimeIsZero(&time))) {
            pizTimeCopy(&x->grainStart, &time);
            pizTimeCopy(&x->grainEnd, &x->grainStart);
            pizTimeAddNano(&x->grainEnd, &x->grainSize);
        }
    
        pizSequenceJumpToStart(x->sequence);
        x->flags |= PIZ_AGENT_FLAG_RUNNING; 
    }
    
    return PIZ_GOOD;
}

PIZError pizAgentStop(PIZAgent *x, const PIZEvent *event)
{
    x->flags &= ~(PIZ_AGENT_FLAG_LOOPED | PIZ_AGENT_FLAG_REPLAY | PIZ_AGENT_FLAG_RUNNING);
    
    return PIZ_GOOD;
}

PIZError pizAgentLoop(PIZAgent *x, const PIZEvent *event)
{
    x->flags |= PIZ_AGENT_FLAG_LOOPED;
    
    return (pizAgentPlay(x, event));
}

PIZError pizAgentUnloop(PIZAgent *x, const PIZEvent *event)
{
    x->flags &= ~PIZ_AGENT_FLAG_LOOPED;
    
    return PIZ_GOOD;
}

PIZError pizAgentForget(PIZAgent *x, const PIZEvent *event)
{
    pizFactorOracleClear(x->oracle);
    pizGaloisLatticeClear(x->lattice);
        
    return PIZ_GOOD;
}

PIZError pizAgentLearn(PIZAgent *x, const PIZEvent *event)
{   
    long argc;
    long *argv = NULL;
    PIZError err = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv))) {
    //
    long h = (100 * (rand_r(&x->seed) / (RAND_MAX + 1.0)));
    
    err |= pizArrayAppend(x->learn, argv[0]);
    
    if (h < (pizArrayCount(x->learn) * PIZ_CONSTANT_LEARN)) {
        err |= pizFactorOracleAdd(x->oracle, pizArrayCount(x->learn), pizArrayPtr(x->learn));
        err |= pizGaloisLatticeAdd(x->lattice, pizArrayCount(x->learn), pizArrayPtr(x->learn));
        pizArrayClear(x->learn);
    }
    //
    }
        
    return err;
}

PIZError pizAgentDump(PIZAgent *x, const PIZEvent *event)
{
    return pizSequenceDump(x->sequence);
}

PIZError pizAgentStatistics(PIZAgent *x, const PIZEvent *event)
{
    return pizSequenceStatistics(x->sequence);
}

PIZError pizAgentBpm(PIZAgent *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
    //
    if ((argv[0] >= PIZ_SEQUENCE_MINIMUM_BPM) && (argv[0] <= PIZ_SEQUENCE_MAXIMUM_BPM)) {
        pizSequenceSetBpm(x->sequence, event);
            
        pizNanoSet(&x->grainSize, PIZ_AGENT_CONSTANT_BPM_NS / pizSequenceBpm(x->sequence));    
        pizTimeCopy(&x->grainEnd, &x->grainStart);
        pizTimeAddNano(&x->grainEnd, &x->grainSize);
    }
    //
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file    pizSequenceRun.c
 * \author  Jean Sapristi
 */
 
/*
 *  Copyright (c) 2011, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *  This software is governed by the CeCILL-C license under French law and
 *  abiding by the rules of distribution of free software. You can use, 
 *  modify and/or redistribute the software under the terms of the CeCILL-C
 *  license as circulated by CEA, CNRS and INRIA at the following URL
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and rights to copy,
 *  modify and redistribute granted by the license, users are provided only
 *  with a limited warranty  and the software's author, the holder of the
 *  economic rights, and the successive licensors have only limited
 *  liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated
 *  with loading, using, modifying and/or developing or reproducing the
 *  software by the user in light of its specific status of free software,
 *  that may mean that it is complicated to manipulate, and that also
 *  therefore means that it is reserved for developers and experienced
 *  professionals having in-depth computer knowledge. Users are therefore
 *  encouraged to load and test the software's suitability as regards their
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and, more generally, to use and operate it in the 
 *  same conditions as regards security. 
 *  
 *  The fact that you are presently reading this means that you have had
 *  knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizAgent.h"
#include "pizAgentLoop.h"
#include "pizSequenceRun.h"
#include "pizSequenceMethods.h"
#include "pizSequenceEach.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_STATIC ulong pizSequenceStepMask(PIZSequence *x, long n, long count);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

#ifndef PIZ_EXTERN_INLINE

bool pizSequenceIsAtEnd(PIZSequence *x)
{
    return (x->index >= x->end);
}

void pizSequenceJumpToStart(PIZSequence *x)
{
    x->index = x->start;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceUpdate(PIZSequence *x)
{
    long i;
    PIZNote *note = NULL;
    PIZError err = PIZ_GOOD; 
    
    if (x->flags) {
    //
        
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_BPM) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_BPM, 1, &x->bpm);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_CHANCE) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_CHANCE, 1, &x->chance);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_VELOCITY) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_VELOCITY, 1, &x->velocity);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_CHANNEL) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_CHANNEL, 1, &x->channel);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_CHORD) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_CHORD, 1, &x->chord);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_CELL) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_CELL, 1, &x->cell);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_VALUE) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_VALUE, 1, &x->value);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_SCALE) {
        long a[ ] = { x->key, x->type };
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_SCALE, 2, a);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_MUTE) {
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_MUTE, 1, &x->mute);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTIFY_ZONE) {
        long a[ ] = { x->start, x->end, x->down, x->up };
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_ZONE, 4, a);
    }
    
    x->flags = PIZ_SEQUENCE_FLAG_NONE;
    //
    }
    
    if (pizItemsetCount(&x->removed)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (pizItemsetIsSetAtIndex(&x->removed, i)) {
            long a[ ] = { 0, 0, 0, 0, 0, i, x->bpm };
            err |= pizAgentNotify(x->agent, PIZ_NOTIFY_REMOVED, 7, a);
        } 
    }
    
    pizItemsetClear(&x->removed);
    //
    }
    
    if (pizItemsetCount(&x->addedLow) || pizItemsetCount(&x->addedHigh)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        bool low = pizItemsetIsSetAtIndex(&x->addedLow, i);
        bool high = pizItemsetIsSetAtIndex(&x->addedHigh, i);
        
        if ((low || high) && (note = x->lookup[i])) {

            long a[ ] = { note->position, 
                          note->values[PIZ_VALUE_PITCH],
                          note->values[PIZ_VALUE_VELOCITY],
                          note->values[PIZ_VALUE_DURATION], 
                          note->values[PIZ_VALUE_CHANNEL],
                          note->tag,
                          x->bpm,
                          low };
            
            err |= pizAgentNotify(x->agent, PIZ_NOTIFY_ADDED, 8, a);
            
            pizItemsetUnsetAtIndex(&x->changed, i);
        } 
    }

    pizItemsetClear(&x->addedLow);
    pizItemsetClear(&x->addedHigh);
    //
    }
    
    if (pizItemsetCount(&x->changed)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (pizItemsetIsSetAtIndex(&x->changed, i) && (note = x->lookup[i])) {

            long a[ ] = { note->position,
                          note->values[PIZ_VALUE_PITCH],
                          note->values[PIZ_VALUE_VELOCITY],
                          note->values[PIZ_VALUE_DURATION], 
                          note->values[PIZ_VALUE_CHANNEL],
                          note->tag,
                          x->bpm };
            
            err |= pizAgentNotify(x->agent, PIZ_NOTIFY_CHANGED, 7, a);
        } 
    }
    
    pizItemsetClear(&x->changed);
    //
    }
    
    return err;
}

PIZError pizSequenceStep(PIZSequence *x)
{
    PIZError err = PIZ_ERROR;
    
    if (x->index < x->start) {
        x->index = x->start;
    }
        
    if (x->index < x->end) {
    //
    long count;
        
    err = PIZ_GOOD;
    
    if (x->timeline[x->index] && (count = pizLinklistCount(x->timeline[x->index]))) {
        long i = 0;
        ulong mask = ~0UL;
        PIZNote *note = NULL;
        PIZNote *nextNote = NULL;
        long scale = pizArrayCount(x->scale);
        
        if (x->chord && (x->chord < count)) {
            mask = pizSequenceStepMask(x, x->chord, count);
        }
                
        pizLinklistPtrAtIndex(x->timeline[x->index], 0, (void **)&note);
        
        while (note) {
        //
        long pitch    = note->values[PIZ_VALUE_PITCH];
        long velocity = note->values[PIZ_VALUE_VELOCITY];
        long channel  = note->values[PIZ_VALUE_CHANNEL];
        
        pizLinklistNextWithPtr(x->timeline[x->index], (void *)note, (void **)&nextNote);
        
        if (scale) {
            pitch += pizArrayAtIndex(x->scale, pitch % scale);
        }

        if (x->mute) {
            velocity = 0;
        }
        
        if (velocity) {
            velocity += x->velocity;
        } 
            
        if (!channel) {
            channel = x->channel;
        }
            
        if ((i < PIZ_MAGIC_ULONG) && (mask & (1UL << i)) && (pitch >= x->down) && (pitch <= x->up)) {
        //
        long a[ ] = { note->position, 
                      CLAMP(pitch, 0, PIZ_MAGIC_PITCH),
                      CLAMP(velocity, 0, PIZ_MAGIC_VELOCITY),
                      note->values[PIZ_VALUE_DURATION], 
                      channel,
                      note->tag,
                      x->bpm };
        
        err |= pizAgentNotify(x->agent, PIZ_NOTIFY_PLAYED, 7, a);
        //
        }
        
        note = nextNote;
        i++;
        //
        }
    }
    
    x->index ++;
    //    
    }
    
    return err;
}

PIZError pizSequenceStepBlank(PIZSequence *x)
{
    PIZError err = PIZ_ERROR;
    
    if (x->index < x->start) {
        x->index = x->start;
    }
        
    if (x->index < x->end) {
        err = PIZ_GOOD;
        x->index ++;    
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

ulong pizSequenceStepMask(PIZSequence *x, long n, long count)
{
    long i;
    ulong mask = (1UL << n) - 1UL;
    
    for (i = (count - 1); i > 0; i--)  {
        long j = (i + 1) * (rand_r(&x->seed) / (RAND_MAX + 1.0));
        
        if (j != i) {
            ulong t = ((mask >> i) ^ (mask >> j)) & 1UL; 
            mask ^= (t << i) | (t << j);
        }
    }
        
    return mask;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
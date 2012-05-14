/*
 * \file    pizSequenceRun.c
 * \author  Jean Sapristi
 * \date    May 14, 2012.
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

#include "pizSequenceRun.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CONSTANT_DURATION   2500.
#define PIZ_UNTAG               pizItemsetUnsetAtIndex 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL PIZError pizSequenceAddNotification (PIZLinklist *q, PIZEventCode n, long tag, long ac, long *av);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceIsAtEnd (PIZSequence *x)
{
    return (x->index >= x->end);
}

void pizSequenceGoToStart (PIZSequence *x)
{
    x->index = x->start;
}

PIZError pizSequenceStep (PIZSequence *x, PIZLinklist *q, long bpm)
{
    PIZError err = PIZ_ERROR;
    
    if (x->index < x->start) {
        x->index = x->start;
    }
        
    if (x->index < x->end) {
    //
    err = PIZ_GOOD;
    
    if (q) {
    //
    long count;
    
    if (x->timeline[x->index] && (count = pizLinklistCount (x->timeline[x->index]))) {
        long    i = 0;
        long    h = -1;
        long    scale = pizArrayCount (x->scale);
        PIZNote *note = NULL;
        PIZNote *nextNote = NULL;
        
        if (!x->chord) { 
             h = (long)(count * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        }
        
        pizLinklistPtrAtIndex (x->timeline[x->index], 0, (void **)&note);
        
        while (note) {
        //
        long pitch, velocity, channel, duration;
        
        pizLinklistNextByPtr (x->timeline[x->index], (void *)note, (void **)&nextNote);
        
        pitch    = note->values[PIZ_VALUE_PITCH];
        velocity = note->values[PIZ_VALUE_VELOCITY];
        duration = note->values[PIZ_VALUE_DURATION];
        channel  = note->values[PIZ_VALUE_CHANNEL];
        
        if (scale) {
            pitch += pizArrayValueAtIndex (x->scale, pitch % scale);
        }

        if (velocity) {
            velocity += x->velocity;
        } 
            
        if (!channel) {
            channel = x->channel;
        }
            
        if ((x->chord || (i == h)) && (pitch >= x->down) && (pitch <= x->up)) {
        //
        long a[ ] = { note->position, 
                      CLAMP (pitch, 0, PIZ_MAGIC_PITCH),
                      CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY),
                      (long)(duration * (PIZ_CONSTANT_DURATION / bpm)), 
                      channel };
        
        err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_PLAYED, note->tag, 5, a);
        //
        }
        
        note = nextNote;
        i++;
        //
        }
    }
    //
    }
    
    x->index ++;
    //    
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceRefresh (PIZSequence *x, PIZLinklist *q)
{
    long     i;
    PIZError err = PIZ_GOOD;
    PIZNote  *note = NULL; 
    
    if (x->flags) {
    //
    if (x->flags & PIZ_SEQUENCE_FLAG_ZONE) {
        long a[ ] = { x->start, x->end, x->down, x->up };
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_ZONE,       -1, 4, a);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_CHANCE) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_CHANCE,     -1, 1, &x->chance);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_VELOCITY) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_VELOCITY,   -1, 1, &x->velocity);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_CHANNEL) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_CHANNEL,    -1, 1, &x->channel);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_CHORD) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_CHORD,      -1, 1, &x->chord);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_CELL) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_CELL,       -1, 1, &x->cell);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_NOTE_VALUE) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_NOTE_VALUE, -1, 1, &x->noteValue);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_SCALE) {
        long a[ ] = { x->key, x->type };
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_SCALE,      -1, 2, a);
    }
    if (x->flags & PIZ_SEQUENCE_FLAG_PATTERN) {
        long argc  = pizArrayCount (x->pattern);
        long *argv = pizArrayPtr (x->pattern);
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANGED_PATTERN,    -1, argc, argv);
    }
    
    x->flags = PIZ_SEQUENCE_FLAG_NONE;
    //
    }
    
    if (pizItemsetCount (&x->removedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (pizItemsetIsSetAtIndex (&x->removedNotes, i)) { 
            err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_REMOVED, i, 0, NULL);
        } 
    }
    
    pizItemsetClear (&x->removedNotes);
    //
    }
    
    if (pizItemsetCount (&x->addedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (pizItemsetIsSetAtIndex (&x->addedNotes, i)) {
            if (!(pizHashTablePtrByKey (x->lookup, i, (void **)&note))) {
            
                long a[ ] = { note->position, 
                              note->values[PIZ_VALUE_PITCH],
                              note->values[PIZ_VALUE_VELOCITY],
                              note->values[PIZ_VALUE_DURATION], 
                              note->values[PIZ_VALUE_CHANNEL] };
                
                err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_ADDED, i, 5, a);
            }
            PIZ_UNTAG (&x->changedNotes, i);
        } 
    }

    pizItemsetClear (&x->addedNotes);
    //
    }
    
    if (pizItemsetCount (&x->changedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (pizItemsetIsSetAtIndex (&x->changedNotes, i)) {
            if (!(pizHashTablePtrByKey (x->lookup, i, (void **)&note))) {
            
                long a[ ] = { note->position,
                              note->values[PIZ_VALUE_PITCH],
                              note->values[PIZ_VALUE_VELOCITY],
                              note->values[PIZ_VALUE_DURATION], 
                              note->values[PIZ_VALUE_CHANNEL] };
                
                pizSequenceAddNotification (q, PIZ_EVENT_NOTE_CHANGED, i, 5, a);
            }
        } 
    }
    
    pizItemsetClear (&x->changedNotes);
    //
    }
        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceAddNotification (PIZLinklist *q, PIZEventCode n, long tag, long ac, long *av)
{
    PIZEvent *notification = NULL;
    PIZError err = PIZ_GOOD;
    
    if (notification = pizEventNew (n, tag, ac, av)) {
        if (err |= pizLinklistAppend (q, notification)) {       
            pizEventFree (notification);  
        }
    } else {
        err |= PIZ_MEMORY;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
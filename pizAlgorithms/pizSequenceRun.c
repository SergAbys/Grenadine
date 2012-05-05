/*
 * \file    pizSequenceRun.c
 * \author  Jean Sapristi
 * \date    May 5, 2012.
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

#define PIZ_CONSTANT_DURATION 2500.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_UNTAG pizItemset128UnsetAtIndex 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZError pizSequenceAddNotification (PIZLinklist *q, PIZEventName n, long tag, long ac, long *av);

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

PIZError pizSequenceProceedStep (PIZSequence *x, PIZLinklist *q, long bpm)
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
    if (x->timeline[x->index] && pizLinklistCount (x->timeline[x->index])) {
        long    scale;
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        scale = pizArrayCount (x->scale);
        
        pizLinklistPtrAtIndex (x->timeline[x->index], 0, (void **)&note);
        
        while (note) {
        //
        long pitch, velocity, channel, duration;
        
        pizLinklistNextByPtr (x->timeline[x->index], (void *)note, (void **)&nextNote);
        
        pitch    = note->midi[PIZ_MIDI_PITCH];
        velocity = note->midi[PIZ_MIDI_VELOCITY];
        duration = note->midi[PIZ_MIDI_DURATION];
        channel  = note->midi[PIZ_MIDI_CHANNEL];
        
        if (scale) {
            pitch += pizArrayValueAtIndex (x->scale, pitch % scale);
        }

        if (velocity) {
            velocity += x->velocity;
        } 
            
        if (!channel) {
            channel = x->channel;
        }
            
        if ((pitch >= x->down) && (pitch <= x->up)) {
        //
        long a[ ] = { note->position,
                      CLAMP (pitch, 0, PIZ_MAGIC_PITCH),
                      CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY),
                      (long)(duration * (PIZ_CONSTANT_DURATION / bpm)), 
                      channel };
        
        err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_PLAYED, note->tag, PIZ_SEQUENCE_NOTE_SIZE, a);
        //
        }
        
        note = nextNote;
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

PIZError pizSequenceNotifications (PIZSequence *x, PIZLinklist *q)
{
    long     i;
    PIZError err = PIZ_GOOD;
    PIZNote  *note = NULL; 
    
    if (x->flags) {
    //
    if (x->flags & PIZ_SEQUENCE_FLAG_ZONE) {
        long a[ ] = { x->start, x->end, x->down, x->up };
        err |= pizSequenceAddNotification (q, PIZ_EVENT_ZONE_CHANGED, -1, PIZ_SEQUENCE_ZONE_SIZE, a);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_CHANCE) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANCE_CHANGED, -1, 1, &x->chance);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_VELOCITY) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_VELOCITY_CHANGED, -1, 1, &x->velocity);
    }
    
    if (x->flags & PIZ_SEQUENCE_FLAG_CHANNEL) {
        err |= pizSequenceAddNotification (q, PIZ_EVENT_CHANNEL_CHANGED, -1, 1, &x->channel);
    }
    
    x->flags = PIZ_SEQUENCE_FLAG_NONE;
    //
    }
    
    if (pizItemset128Count (&x->removedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->removedNotes, i)) { 
            err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_REMOVED, i, 0, NULL);
        } 
    }
    
    pizItemset128Clear (&x->removedNotes);
    //
    }
    
    if (pizItemset128Count (&x->addedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->addedNotes, i)) {
            if (!(pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note))) {
            
                long a[ ] = { note->position,
                              note->midi[PIZ_MIDI_PITCH],
                              note->midi[PIZ_MIDI_VELOCITY],
                              note->midi[PIZ_MIDI_DURATION], 
                              note->midi[PIZ_MIDI_CHANNEL] };
                
                err |= pizSequenceAddNotification (q, PIZ_EVENT_NOTE_ADDED, i, PIZ_SEQUENCE_NOTE_SIZE, a);
            }
            PIZ_UNTAG (&x->changedNotes, i);
        } 
    }

    pizItemset128Clear (&x->addedNotes);
    //
    }
    
    if (pizItemset128Count (&x->changedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->changedNotes, i)) {
            if (!(pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note))) {
            
                long a[ ] = { note->position,
                              note->midi[PIZ_MIDI_PITCH],
                              note->midi[PIZ_MIDI_VELOCITY],
                              note->midi[PIZ_MIDI_DURATION], 
                              note->midi[PIZ_MIDI_CHANNEL] };
                
                pizSequenceAddNotification (q, PIZ_EVENT_NOTE_CHANGED, i, PIZ_SEQUENCE_NOTE_SIZE, a);
            }
        } 
    }
    
    pizItemset128Clear (&x->changedNotes);
    //
    }
        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceAddNotification (PIZLinklist *q, PIZEventName n, long tag, long ac, long *av)
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
/*
 * \file    pizSequenceRun.c
 * \author  Jean Sapristi
 * \date    April 20, 2012.
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
#include "pizEvent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CONSTANT_DURATION   2500.

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

PIZError pizSequenceProceedStep (PIZSequence *x, PIZLinklist *queue, long bpm)
{
    PIZError err = PIZ_ERROR;
    
    if (x->index < x->start) {
        x->index = x->start;
    }
        
    if (x->index < x->end) {
    //
    err = PIZ_GOOD;
    
    if (queue) {
    //
    if (x->timeline[x->index] && pizLinklistCount (x->timeline[x->index])) {
        long    scale;
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        scale = pizArrayCount (x->scale);
        
        pizLinklistPtrAtIndex (x->timeline[x->index], 0, (void **)&note);
        
        while (note) {
            long     pitch, 
                     velocity, 
                     channel, 
                     duration;
            PIZEvent *event = NULL;
            
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

                long argv[ ] = { note->position,
                                 CLAMP (pitch, 0, PIZ_MAGIC_PITCH),
                                 CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY),
                                 (long)(duration * (PIZ_CONSTANT_DURATION / bpm)), 
                                 channel };
                         
                if (event = pizEventNewWithNote (PIZ_EVENT_NOTE_PLAYED, argv, note->tag)) {
                    if (err |= pizLinklistAppend (queue, event)) {       
                        pizEventFree (event);  
                    }
                } else {
                    err |= PIZ_MEMORY;
                }
            }
            
            note = nextNote;
        }
    }
    
    x->index ++;
    //
    }
    //    
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
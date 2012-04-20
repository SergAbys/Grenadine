/*
 * \file	pizSequenceLibrary.c
 * \author	Jean Sapristi
 * \date	April 16, 2012.
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

#include "pizSequenceLibrary.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_TAG     pizItemset128SetAtIndex 
#define PIZ_UNTAG   pizItemset128UnsetAtIndex 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceFunAll (PIZSequence *x, PIZMethod f, const PIZEvent *event)
{
    long i;
    
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            (*f)(x, note, event);
            
            note = nextNote;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceRemoveNote (PIZSequence *x, PIZNote *note, const PIZEvent *event) 
{
    long p = note->position;
    long tag = note->tag;
    
    pizBoundedHashTableRemoveByKey (x->lookup, tag, note);
    pizBoundedStackPush (x->ticketMachine, tag);
    pizLinklistRemoveByPtr (x->timeline[p], (void *)note);
    x->count --; 
    
    PIZ_TAG   (&x->removedNotes, tag);
    PIZ_UNTAG (&x->addedNotes, tag);
    PIZ_UNTAG (&x->changedNotes, tag);
}

void pizSequenceFillTempHash (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{   /*
    long key, scale, offset = 0;
    
    if (scale = pizArrayCount (x->scale)) {
        offset = pizArrayValueAtIndex (x->scale, note->midi[PIZ_MIDI_PITCH] % scale);
    }
    
    key = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1));
    key += note->midi[PIZ_MIDI_PITCH] + offset;
    
    x->tempError |= pizBoundedHashTableAdd (x->tempHash, key, (void *)note);*/
}

void pizSequenceFillTempNotes (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    x->tempNotes1[x->tempIndex] = note;
    x->tempIndex ++;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZNote *pizSequenceNewNote (PIZSequence *x, long *argv, ulong flags)
{
    PIZNote *newNote = NULL;
    long    err      = PIZ_GOOD;
    long    position = argv[PIZ_DATA_POSITION];
    long    pitch    = argv[PIZ_DATA_PITCH];
    long    velocity = argv[PIZ_DATA_VELOCITY];
    long    duration = argv[PIZ_DATA_DURATION]; 
    long    channel  = argv[PIZ_DATA_CHANNEL]; 
    
    if (flags & PIZ_SEQUENCE_FLAG_SNAP) {
        position = pizSequenceSnapPositionToPattern (x, position);
    } 
    
    if (flags & PIZ_SEQUENCE_FLAG_AMBITUS) {
        pitch = pizSequenceMovePitchToAmbitus (x, pitch);
    }
                
    err |= (position < 0);
    err |= (position > (x->timelineSize - 1));
    err |= (x->count >= PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (flags & PIZ_SEQUENCE_FLAG_CLIP) {
        err |= (pitch > x->up);
        err |= (pitch < x->down);
        err |= (position < x->start);
        err |= (position >= x->end);
    }
        
    if (!err) {
    //
    err |= pizBoundedStackPop (x->ticketMachine);
    
    if (!err && (newNote = (PIZNote *)malloc (sizeof(PIZNote)))) {
        newNote->tag                     = pizBoundedStackPoppedValue (x->ticketMachine);
        newNote->position                = position;
        newNote->midi[PIZ_MIDI_PITCH]    = CLAMP (pitch,    0, PIZ_MAGIC_PITCH);
        newNote->midi[PIZ_MIDI_VELOCITY] = CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY);
        newNote->midi[PIZ_MIDI_DURATION] = CLAMP (duration, 0, PIZ_SEQUENCE_MAXIMUM_DURATION);
        newNote->midi[PIZ_MIDI_CHANNEL]  = CLAMP (channel,  0, PIZ_MAGIC_CHANNEL);
    
        if (!(x->timeline[newNote->position])) {
            if (!(x->timeline[newNote->position] = pizLinklistNew ( ))) {
                err |= PIZ_MEMORY;
            }
        }
        
        err |= pizBoundedHashTableAdd (x->lookup, newNote->tag, newNote);
                                
        if (!err && !(pizLinklistInsert (x->timeline[newNote->position], (void *)newNote))) {
            x->count ++; 
            PIZ_TAG   (&x->addedNotes, newNote->tag);
            PIZ_UNTAG (&x->changedNotes, newNote->tag);
            
        } else {
            pizBoundedHashTableRemoveByKey (x->lookup, newNote->tag, newNote);
            pizBoundedStackPush (x->ticketMachine, newNote->tag);
            free (newNote);
            newNote = NULL;
        }
    }
    //    
    }
        
    return newNote;
}   
/*
PIZError pizSequenceMoveNote (PIZSequence *x, PIZNote *note, long newPosition)
{
    long     position = note->position;
    PIZError err = PIZ_GOOD; 
    
    if (position != newPosition) {
    //
    if (!x->timeline[newPosition]) {
        if (!(x->timeline[newPosition] = pizLinklistNew ( ))) {
            err |= PIZ_MEMORY;
        }
    }
    
    if (!err) {
        if (!(err |= pizLinklistChuckByPtr (x->timeline[position], (void *)note))) {            
            if (!(err |= pizLinklistInsert (x->timeline[newPosition], (void *)note))) {
                note->position = newPosition;
                PIZ_TAG (&x->changedNotes, note->tag);
            } else {
                pizSequenceRemoveNote (x, note, NULL);
            }
        } 
    }
    //
    }
    
    return err;
}*/

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceMakeMap (PIZSequence *x)
{
    long i;
            
    pizArrayClear (x->map);
                            
    for (i = 0; i < x->timelineSize; i++) {
        if (x->timeline[i] && pizLinklistCount (x->timeline[i])) {
            pizArrayAppend (x->map, i);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceMovePitchToAmbitus (PIZSequence *x, long pitch)
{
    if (pitch < x->down) {
        while ((pitch < x->down) && (pitch < PIZ_MAGIC_PITCH)) {
            pitch += PIZ_MAGIC_SCALE;
        }
    } else if (pitch > x->up) {
        while ((pitch > x->up) && (pitch > 0)) {
            pitch -= PIZ_MAGIC_SCALE;
        }
    }
    
    return (CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
}

long pizSequenceSnapPositionToPattern (PIZSequence *x, long position)
{
    long s = pizArrayCount (x->pattern); 
    long j = (long)(position / (double)x->cell);
    
    position = j * x->cell;
    
    if (s) {
        long k = (long)(x->start / (double)x->cell);
        
        while (k > position) {
            k -= s;
        }
        
        position += pizArrayValueAtIndex (x->pattern, (j - k) % s) * x->cell;
    } 

    return position;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
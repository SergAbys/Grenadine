/*
 * \file	pizSequenceLibrary.c
 * \author	Jean Sapristi
 * \date	April 12, 2012.
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
    
    pizItemset128SetAtIndex   (&x->removedNotes, tag);
    pizItemset128UnsetAtIndex (&x->addedNotes, tag);
    pizItemset128UnsetAtIndex (&x->changedNotes, tag);
}

void pizSequenceTestIsPlayed (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    bool isHit;
    
    isHit = (x->index >= note->position);
    isHit = isHit && (x->index < (note->position + note->midi[PIZ_MIDI_DURATION]));
    
    if (isHit && !note->isPlayed) {
        note->isPlayed = true;
        pizItemset128SetAtIndex (&x->changedNotes, note->tag);
    } else if (!isHit && note->isPlayed) {
        note->isPlayed = false;
        pizItemset128SetAtIndex (&x->changedNotes, note->tag);
    }
}

void pizSequenceFillTempHash (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    long key, scale, offset = 0;
    
    if (scale = pizArrayCount (x->scale)) {
        offset = pizArrayValueAtIndex (x->scale, note->midi[PIZ_MIDI_PITCH] % scale);
    }
    
    key = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1));
    key += note->midi[PIZ_MIDI_PITCH] + offset;
    
    x->tempError |= pizBoundedHashTableAdd (x->tempHash, key, (void *)note);
}

void pizSequenceFillTempNotes (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    x->tempNotes1[x->tempIndex] = note;
    x->tempIndex ++;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceFillNotes (PIZSequence *x, PIZMidiSelector selector, bool reverse)
{
    long i, t;
    
    if (selector == PIZ_MIDI_DURATION) {
    //
    for (i = 0; i < x->tempIndex; i++) {
        if (!reverse) {
            t = MIN (x->tempValues[i], x->timelineSize - x->tempNotes1[i]->position);
        } else {
            t = MIN (x->tempValues[(x->tempIndex - 1) - i], x->timelineSize - x->tempNotes1[i]->position);
        }
        
        if (x->tempNotes1[i]->midi[selector] != t) {
            x->tempNotes1[i]->midi[selector] = t;
            pizItemset128SetAtIndex (&x->changedNotes, x->tempNotes1[i]->tag);
        }
    }
    //
    } else {
    //
    for (i = 0; i < x->tempIndex; i++) {
        if (!reverse) {
            t = x->tempValues[i];
        } else {
            t = x->tempValues[(x->tempIndex - 1) - i];
        }
        
        if (x->tempNotes1[i]->midi[selector] != t) {
            x->tempNotes1[i]->midi[selector] = t;
            pizItemset128SetAtIndex (&x->changedNotes, x->tempNotes1[i]->tag);
        }
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZNote *pizSequenceNewNote (PIZSequence *x, long *argv, ulong flags)
{
    PIZNote *newNote    = NULL;
    long    err         = PIZ_GOOD;
    long    position    = argv[PIZ_DATA_POSITION];
    long    pitch       = CLAMP (argv[PIZ_DATA_PITCH],    0, PIZ_MAGIC_PITCH);
    long    velocity    = CLAMP (argv[PIZ_DATA_VELOCITY], 0, PIZ_MAGIC_VELOCITY);
    long    duration    = CLAMP (argv[PIZ_DATA_DURATION], 1, PIZ_SEQUENCE_MAXIMUM_DURATION); 
    long    channel     = CLAMP (argv[PIZ_DATA_CHANNEL],  0, PIZ_MAGIC_CHANNEL); 
    long    isSelected  = argv[PIZ_DATA_IS_SELECTED];
    long    isPlayed    = argv[PIZ_DATA_IS_PLAYED];
    
    if (flags & PIZ_SEQUENCE_FLAG_SNAP) {
        position = MAX (((long)(position / (double)x->grid)) * x->grid, 0);
    }
    
    if (flags & PIZ_SEQUENCE_FLAG_PATTERN) {
        position = pizSequenceSnapPositionToPattern (x, position);
    } 
    
    if (flags & PIZ_SEQUENCE_FLAG_AMBITUS) {
        pitch = pizSequenceMovePitchToAmbitus (x, pitch);
    }
        
    if (flags & PIZ_SEQUENCE_FLAG_UNSELECT) {
        isSelected = false;
    }
                
    err |= (position < 0);
    err |= (position > (x->timelineSize - MAX (duration, 1)));
    err |= (x->count >= PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (flags & PIZ_SEQUENCE_FLAG_CLIP) {
        err |= (position < x->start);
        err |= (position >= x->end);
        err |= (pitch < x->down);
        err |= (pitch > x->up);
    }
        
    if (!err) {
    //
    err |= pizBoundedStackPop (x->ticketMachine);
    
    if (!err && (newNote = (PIZNote *)malloc (sizeof(PIZNote)))) {
        newNote->tag                     = pizBoundedStackPoppedValue (x->ticketMachine);
        newNote->position                = position;
        newNote->midi[PIZ_MIDI_PITCH]    = pitch;
        newNote->midi[PIZ_MIDI_VELOCITY] = velocity;
        newNote->midi[PIZ_MIDI_DURATION] = duration;
        newNote->midi[PIZ_MIDI_CHANNEL]  = channel;
        newNote->isSelected              = isSelected;
        newNote->isPlayed                = isPlayed;
    
        if (!(x->timeline[newNote->position])) {
            if (!(x->timeline[newNote->position] = pizLinklistNew ( ))) {
                err |= PIZ_MEMORY;
            }
        }
        
        err |= pizBoundedHashTableAdd (x->lookup, newNote->tag, newNote);
                                
        if (!err && !(pizLinklistInsert (x->timeline[newNote->position], (void *)newNote))) {
            x->count ++; 
            pizItemset128SetAtIndex (&x->addedNotes, newNote->tag);
            pizItemset128UnsetAtIndex (&x->changedNotes, newNote->tag);
            
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
                pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            } else {
                pizSequenceRemoveNote (x, note, NULL);
            }
        } 
    }
    //
    }
    
    return err;
}

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
    long scale, offset = 0;
    
    if (scale = pizArrayCount (x->scale)) {
        offset = pizArrayValueAtIndex (x->scale, pitch % scale);
    }

    pitch += offset;
            
    if (pitch < x->down) {
        while ((pitch < x->down) && (pitch < PIZ_MAGIC_PITCH)) {
            pitch += PIZ_MAGIC_SCALE;
        }
    } else if (pitch > x->up) {
        while ((pitch > x->up) && (pitch > 0)) {
            pitch -= PIZ_MAGIC_SCALE;
        }
    }
            
    pitch -= offset;
    
    return (CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
}

long pizSequenceSnapPositionToPattern (PIZSequence *x, long position)
{
    long patternSize = pizArrayCount (x->pattern);  
        
    if (patternSize) {
        if (x->cell != PIZ_NOTE_VALUE_NONE) {
            long j = MAX ((long)(position / (double)x->cell), 0);
            long k = j % patternSize;
        
            position = j * x->cell;
            position += pizArrayValueAtIndex (x->pattern, k) * x->cell;
        }
    }

    return position;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
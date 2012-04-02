/*
 * \file	pizSequenceLibrary.c
 * \author	Jean Sapristi
 * \date	April 2, 2012.
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

PIZNote *pizSequenceNewNote (PIZSequence *x, long *values, long flags)
{
    PIZNote *newNote    = NULL;
    long    err         = PIZ_GOOD;
    long    position    = values[PIZ_DATA_POSITION];
    long    pitch       = CLAMP (values[PIZ_DATA_PITCH],    0, PIZ_MAGIC_PITCH);
    long    velocity    = CLAMP (values[PIZ_DATA_VELOCITY], 0, PIZ_MAGIC_VELOCITY);
    long    duration    = CLAMP (values[PIZ_DATA_DURATION], 1, PIZ_SEQUENCE_MAXIMUM_DURATION); 
    long    channel     = CLAMP (values[PIZ_DATA_CHANNEL],  0, PIZ_MAGIC_CHANNEL); 
    long    isSelected  = values[PIZ_DATA_IS_SELECTED];
    long    isMarked    = values[PIZ_DATA_IS_MARKED];
    
    if (flags & PIZ_SEQUENCE_ADD_FLAG_SNAP) {
        position = MAX (((long)(position / (double)x->grid)) * x->grid, 0);
    }
    
    if (flags & PIZ_SEQUENCE_ADD_FLAG_PATTERN) {
        long patternSize = pizGrowingArrayCount (x->pattern);  
        if (patternSize) {
            position = pizSequenceSnapPositionToPattern (x, position, patternSize);
        }
    } 
    
    if (flags & PIZ_SEQUENCE_ADD_FLAG_AMBITUS) {
        pitch = pizSequenceMovePitchToAmbitus (x, pitch);
    }
        
    if (flags & PIZ_SEQUENCE_ADD_FLAG_UNSELECT) {
        isSelected = false;
        isMarked = false;
    }
                
    err |= (position < 0);
    err |= (position > (x->timelineSize - MAX (duration, 1)));
    err |= (x->count >= PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (flags & PIZ_SEQUENCE_ADD_FLAG_CLIP) {
        err |= (position < x->start);
        err |= (position >= x->end);
        err |= (pitch < x->down);
        err |= (pitch > x->up);
    }
        
    if (!err) {
    //
    err |= pizBoundedStackPop (x->ticketMachine);
    
    if (!err && (newNote = (PIZNote *)malloc (sizeof(PIZNote)))) {
        newNote->flags              = PIZ_SEQUENCE_NOTE_FLAG_NONE;
        newNote->data[PIZ_PITCH]    = pitch;
        newNote->data[PIZ_VELOCITY] = velocity;
        newNote->data[PIZ_DURATION] = duration;
        newNote->data[PIZ_CHANNEL]  = channel;
        newNote->isSelected         = isSelected;
        newNote->position           = position;
        newNote->tag                = pizBoundedStackPoppedValue (x->ticketMachine);
    
        if (!x->timeline[newNote->position]) {
            if (!(x->timeline[newNote->position] = pizLinklistNew ( ))) {
                err |= PIZ_MEMORY;
            }
        }
        
        err |= pizBoundedHashTableAdd (x->lookup, newNote->tag, newNote);
                                
        if (!err && !(pizLinklistInsert (x->timeline[newNote->position], (void *)newNote))) {
            if (isMarked) {
                x->markedNote = newNote;
            }
            x->count ++; 
            pizItemset128SetAtIndex (&x->addedNotes, newNote->tag);
            pizItemset128UnsetAtIndex (&x->changedNotes, newNote->tag);
        } else {
            pizBoundedStackPush (x->ticketMachine, newNote->tag);
            free (newNote);
            newNote = NULL;
        }
    }
    //    
    }
        
    return newNote;
}   

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceRemoveNote (PIZSequence *x, PIZNote *note) 
{
    long p = note->position;
    long tag = note->tag;
    
    pizBoundedHashTableRemoveByKeyAndPtr (x->lookup, tag, note);
    
    pizBoundedStackPush (x->ticketMachine, tag);
    
    if (note == x->markedNote) {
        x->markedNote = NULL;
    }

    pizLinklistRemoveByPtr (x->timeline[p], (void *)note);
    
    x->count --; 
    pizItemset128SetAtIndex   (&x->removedNotes, tag);
    pizItemset128UnsetAtIndex (&x->addedNotes, tag);
    pizItemset128UnsetAtIndex (&x->changedNotes, tag);
}

void pizSequenceRemoveAllNotes (PIZSequence *x)
{
    if (x->count) {
    //
    long i;
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            pizSequenceRemoveNote (x, note);
            note = nextNote;
        }
    }
    
    pizGrowingArrayClear (x->map);
    //    
    }
}

void pizSequenceMoveNote (PIZSequence *x, PIZNote *note, long newPosition)
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
        if (!pizLinklistChuckByPtr (x->timeline[position], (void *)note)) {            
            if (!pizLinklistInsert (x->timeline[newPosition], (void *)note)) {
                note->position = newPosition;
                pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            }
        } 
    }
    //
    }
}

void pizSequenceMakeMap (PIZSequence *x)
{
    long i;
            
    pizGrowingArrayClear (x->map);
                            
    for (i = 0; i < x->timelineSize; i++) {
        if (x->timeline[i] && pizLinklistCount (x->timeline[i])) {
            pizGrowingArrayAppend (x->map, i);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceMovePitchToAmbitus (PIZSequence *x, long pitch)
{
    long scale, offset = 0;
    
    if (scale = pizGrowingArrayCount (x->scale)) {
        offset = pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
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

long pizSequenceSnapPositionToPattern (PIZSequence *x, long toSnapped, long patternSize)
{
    if (x->cell != PIZ_NOTE_NONE) {
        long j = MAX ((long)(toSnapped / (double)x->cell), 0);
        long k = j % patternSize;
    
        toSnapped = j * x->cell;
        toSnapped += pizGrowingArrayValueAtIndex (x->pattern, k) * x->cell;
    }

    return toSnapped;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequencePickUpNotes (PIZSequence *x)
{
    long i, k = 0;
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                
            x->notes1[k] = note;
            k ++;
                
            note = nextNote;
        }
    }
    
    return k;
}

bool pizSequenceFillValues (PIZSequence *x, PIZSelector selector, long k, bool reverse)
{
    long i, temp;
    bool haveChanged = false;
    
    if (selector == PIZ_DURATION) {
        for (i = 0; i < k; i++) {
            if (!reverse) {
                temp = MIN (x->values1[i], (x->timelineSize - x->notes1[i]->position));
            } else {
                temp = MIN (x->values1[(k - 1) - i], (x->timelineSize - x->notes1[i]->position));
            }
            
            if (x->notes1[i]->data[PIZ_DURATION] != temp) {
                x->notes1[i]->data[PIZ_DURATION] = temp;
                pizItemset128SetAtIndex (&x->changedNotes, x->notes1[i]->tag);
                haveChanged = true;
            }
        }
    } else {
        for (i = 0; i < k; i++) {
            if (!reverse) {
                temp = x->values1[i];
            } else {
                temp = x->values1[(k - 1) - i];
            }
            
            if (x->notes1[i]->data[selector] != temp) {
                x->notes1[i]->data[selector] = temp;
                pizItemset128SetAtIndex (&x->changedNotes, x->notes1[i]->tag);
                haveChanged = true;
            }
        }
    }
    
    return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
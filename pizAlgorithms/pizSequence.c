/*
 * \file    pizSequence.c
 * \author  Jean Sapristi
 * \date    April 1, 2012.
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

#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_START           0
#define PIZ_DEFAULT_END             96
#define PIZ_DEFAULT_DOWN            60
#define PIZ_DEFAULT_UP              71
#define PIZ_DEFAULT_CHANCE          100
#define PIZ_DEFAULT_CHANNEL         1
#define PIZ_DEFAULT_VELOCITY        90

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long piz_modes[ ] = 
{   0,-1, 0, 1, 0, 0, 1, 0,-1, 0, 1, 0,     // Ionian
    0,-1, 0, 0,-1, 0, 1, 0,-1, 0, 0,-1,     // Dorian
    0, 0,-1, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Phrygian
    0,-1, 0, 1, 0, 1, 0, 0,-1, 0, 1, 0,     // Lydian
    0,-1, 0, 1, 0, 0, 1, 0,-1, 0, 0,-1,     // Mixolydian
    0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Aeolian
    0, 0,-1, 0,-1, 0, 0,-1, 0,-1, 0,-1,     // Locrian
    0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Natural
    0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 1, 0,     // Harmonic
    0,-1, 0, 0,-1, 0, 1, 0,-1, 0, 1, 0,     // Melodic
    0,-1, 0,-1, 0,-1, 0, 1, 0,-1, 0, 1,     // Whole tone
    0,-1, 0,-1, 0,-1, 1, 0,-1, 0,-1, 1,     // Pentatonic major
    0,-1, 1, 0, 1, 0,-1, 0,-1, 1, 0,-1,     // Pentatonic minor
    0, 0,-1, 0, 0, 1, 0, 0, 1, 0, 0,-1,     // Octatonic half whole
    0,-1, 0, 0, 1, 0, 0,-1, 0, 0, 1, 0,     // Octatonic whole half
    0,-1,-2, 1, 0,-1, 1, 0,-1,-2, 1, 0,     // Seventh major
    0,-1,-2, 1, 0,-1, 1, 0,-1, 1, 0,-1,     // Seventh dominant
    0,-1, 1, 0,-1,-2, 1, 0,-1, 1, 0,-1,     // Seventh minor
    0,-1,-2, 1, 0,-1, 1, 0, 1, 0,-1,-2,     // Seventh major sixth
    0,-1, 1, 0,-1,-2, 1, 0, 1, 0,-1,-2,     // Seventh minor sixth
    0,-1, 1, 0,-1, 1, 0,-1,-2, 1, 0,-1,     // Seventh half diminished
    0,-1, 1, 0,-1, 1, 0,-1, 1, 0,-1,-2,     // Seventh diminished
    0,-1,-2, 2, 1, 0, 1, 0,-1, 1, 0,-1,     // Seventh suspended
    0,-1,-2, 1, 0,-1, 2, 1, 0,-1, 0,-1,     // Seventh sharp five
    0,-1,-2, 1, 0, 1, 0,-1,-2, 1, 0,-1   }; // Seventh flat five

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZSequence *pizSequenceNew (long size)
{
    PIZSequence *x = NULL;
    
    if (x = (PIZSequence *)malloc (sizeof(PIZSequence))) {
    //
    long argv1[2] = { 0, PIZ_SEQUENCE_MAXIMUM_NOTES };
    long argv2[2] = { PIZ_SEQUENCE_LOOKUP_SIZE, PIZ_SEQUENCE_MAXIMUM_NOTES };
    
    if (size > 0) {
        x->timelineSize = size;
    } else {
        x->timelineSize = PIZ_SEQUENCE_DEFAULT_TIMELINE;
    }
    
    x->map           = pizGrowingArrayNew (PIZ_SEQUENCE_MAXIMUM_NOTES);
    x->scale         = pizGrowingArrayNew (PIZ_MAGIC_SCALE);
    x->pattern       = pizGrowingArrayNew (0);
    x->values1       = (long *)malloc (sizeof(long) * PIZ_SEQUENCE_TEMP_SIZE);
    x->values2       = (long *)malloc (sizeof(long) * PIZ_SEQUENCE_TEMP_SIZE);
    x->notes1        = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_TEMP_SIZE);
    x->notes2        = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_TEMP_SIZE);
    x->hashTable     = pizBoundedHashTableNew (2, argv1);
    x->lookup        = pizBoundedHashTableNew (2, argv2);
    x->ticketMachine = pizBoundedStackNew (PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (x->map && 
        x->scale &&
        x->pattern &&
        x->values1 &&
        x->values2 &&
        x->notes1 &&
        x->notes2 &&
        x->hashTable &&
        x->lookup &&
        x->ticketMachine &&
        (x->timeline = (PIZLinklist **)calloc (x->timelineSize, sizeof(PIZLinklist **)))) {
        long i;
        
        for (i = (PIZ_SEQUENCE_MAXIMUM_NOTES - 1); i >= 0; i--) {
            pizBoundedStackPush (x->ticketMachine, i);
        }
        
        x->markedNote = NULL;
        
        pizItemset128Clear (&x->addedNotes);
        pizItemset128Clear (&x->removedNotes);
        pizItemset128Clear (&x->changedNotes);
        
        x->changedZone = true;
        
        x->seed = (unsigned int)time(NULL);
                        
        pthread_mutex_init (&x->lock, NULL);
        
        x->start            = PIZ_DEFAULT_START;
        x->end              = PIZ_DEFAULT_END;
        x->down             = PIZ_DEFAULT_DOWN;
        x->up               = PIZ_DEFAULT_UP;
        x->count            = 0;
        x->index            = 0;
        x->chance           = PIZ_DEFAULT_CHANCE;
        x->channel          = PIZ_DEFAULT_CHANNEL;
        x->velocity         = 0;
        x->cell             = PIZ_NOTE_NONE;
        x->grid             = PIZ_NOTE_NONE;
        x->noteValue        = PIZ_NOTE_NONE;
            
    } else {
        pizSequenceFree (x);
        x = NULL;
    }
    //
    }
    
    return x;
}

void pizSequenceFree (PIZSequence *x)
{
    if (x) {
        PIZSEQUENCELOCK
        PIZSEQUENCEUNLOCK
        
        if (x->timeline) {
            long i;
            
            for (i = 0; i < x->timelineSize; i++) {
                pizLinklistFree (x->timeline[i]);
            }
                
            free (x->timeline);
            x->timeline = NULL;
        }
        
        pizGrowingArrayFree (x->map);
        pizGrowingArrayFree (x->scale);
        pizGrowingArrayFree (x->pattern);
        
        pizBoundedHashTableFree (x->hashTable);
        pizBoundedHashTableFree (x->lookup);
        pizBoundedStackFree (x->ticketMachine);
        
        if (x->values1) {
            free (x->values1);
        }
        if (x->values2) {
            free (x->values2);
        }
        if (x->notes1) {
            free (x->notes1);
        }
        if (x->notes2) {
            free (x->notes2);
        }
        
        pthread_mutex_destroy (&x->lock);
        
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceCount (PIZSequence *x)
{   
    long k; 
    
    PIZSEQUENCELOCK
    k = x->count;
    PIZSEQUENCEUNLOCK
    
    return k;
}   

long pizSequenceChance (PIZSequence *x)
{
    long k;
    
    PIZSEQUENCELOCK
    k = x->chance;
    PIZSEQUENCEUNLOCK
    
    return k;
}

long pizSequenceVelocity (PIZSequence *x)
{
    long k;
    
    PIZSEQUENCELOCK
    k = x->velocity;
    PIZSEQUENCEUNLOCK
    
    return k;
}

long pizSequenceChannel (PIZSequence *x)
{
    long k;
    
    PIZSEQUENCELOCK
    k = x->channel;
    PIZSEQUENCEUNLOCK
    
    return k;
}

PIZNoteValue pizSequenceCell (PIZSequence *x)
{
    PIZNoteValue k;
    
    PIZSEQUENCELOCK
    k = x->cell;
    PIZSEQUENCEUNLOCK
    
    return k;
}

PIZNoteValue pizSequenceGrid (PIZSequence *x)
{
    PIZNoteValue k;
    
    PIZSEQUENCELOCK
    k = x->grid;
    PIZSEQUENCEUNLOCK
    
    return k;
}

PIZNoteValue pizSequenceNoteValue (PIZSequence *x)
{
    PIZNoteValue k;
    
    PIZSEQUENCELOCK
    k = x->noteValue;
    PIZSEQUENCEUNLOCK
    
    return k;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceSetChance (PIZSequence *x, long value)
{
    PIZSEQUENCELOCK
    x->chance = CLAMP (value, 0, 100);
    PIZSEQUENCEUNLOCK
}

void pizSequenceSetVelocity (PIZSequence *x, long value)
{
    PIZSEQUENCELOCK
    x->velocity = value;
    PIZSEQUENCEUNLOCK
}

void pizSequenceSetChannel (PIZSequence *x, long channel)
{
    PIZSEQUENCELOCK
    x->channel = CLAMP (channel, 1, PIZ_MAGIC_CHANNEL);
    PIZSEQUENCEUNLOCK
}

void pizSequenceSetCell (PIZSequence *x, PIZNoteValue snapValue)
{
    PIZSEQUENCELOCK
    x->cell = snapValue;
    PIZSEQUENCEUNLOCK
}

void pizSequenceSetGrid (PIZSequence *x, PIZNoteValue snapValue)
{
    PIZSEQUENCELOCK
    x->grid = snapValue;
    PIZSEQUENCEUNLOCK
}

void pizSequenceSetNoteValue (PIZSequence *x, PIZNoteValue noteValue)
{
    PIZSEQUENCELOCK
    x->noteValue = noteValue;
    PIZSEQUENCEUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizSequenceSetScale (PIZSequence *x, PIZScaleKey key, PIZScaleType type, const PIZGrowingArray *a)
{
    long     *ptr = NULL;
    PIZError err = PIZ_GOOD;

    PIZSEQUENCELOCK
    
    pizGrowingArrayClear (x->scale);
    
    if (type == PIZ_SCALE_CUSTOM) {
        if (a && (pizGrowingArrayCount (a) == PIZ_MAGIC_SCALE)) { 
            ptr = pizGrowingArrayPtr (a);
        } else {
            err = PIZ_ERROR;
        }
    } else if (type != PIZ_SCALE_NONE) {
        ptr = piz_modes + (type * PIZ_MAGIC_SCALE); 
    }
    
    if (ptr) {
        long i;
        for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
            pizGrowingArrayAppend (x->scale, *(ptr + ((PIZ_MAGIC_SCALE - key + i) % PIZ_MAGIC_SCALE)));
        }
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}   

PIZError pizSequenceSetPattern (PIZSequence *x, const PIZGrowingArray *a)
{
    PIZError err = PIZ_ERROR;
     
    PIZSEQUENCELOCK
    
    if (a) {
        err = PIZ_GOOD;
        err |= pizGrowingArrayCopy (x->pattern, a);
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceIndex (PIZSequence *x)
{
    long k;
    
    PIZSEQUENCELOCK
    k = x->index;
    PIZSEQUENCEUNLOCK
    
    return k;
}

bool pizSequenceIsAtEnd (PIZSequence *x)
{
    bool k = false;
    
    PIZSEQUENCELOCK
    
    if (x->index >= x->end) {
        k = true;
    }
    
    PIZSEQUENCEUNLOCK
    
    return k;
}

void pizSequenceGoToStart (PIZSequence *x)
{
    PIZSEQUENCELOCK
    
    x->index = x->start;
    
    PIZSEQUENCEUNLOCK
}

PIZError pizSequenceProceedStep (PIZSequence *x, PIZGrowingArray *a)
{
    PIZError err = PIZ_ERROR;
    
    PIZSEQUENCELOCK
    
    if (x->index < x->start) {
        x->index = x->start;
    }
        
    if (x->index < x->end) {
    //
    err = PIZ_GOOD;
                
    if (a && (x->timeline[x->index]) && (pizLinklistCount (x->timeline[x->index]))) {
        long    scale;
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        scale = pizGrowingArrayCount (x->scale);
        
        pizLinklistPtrAtIndex (x->timeline[x->index], 0, (void **)&note);
        
        while (note) {
            long pitch;
            
            pizLinklistNextByPtr (x->timeline[x->index], (void *)note, (void **)&nextNote);
            
            pitch = note->data[PIZ_PITCH];
            
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
            
            if ((pitch >= x->down) && (pitch <= x->up)) {
                long velocity       = note->data[PIZ_VELOCITY];
                long noteChannel    = note->data[PIZ_CHANNEL];
                        
                if (velocity) {
                    velocity += x->velocity;
                } 
                
                if (noteChannel == PIZ_SEQUENCE_CHANNEL_NONE) {
                    noteChannel = x->channel;
                }
                  
                err |= pizGrowingArrayAppend (a, note->position);      
                err |= pizGrowingArrayAppend (a, CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
                err |= pizGrowingArrayAppend (a, CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY));
                err |= pizGrowingArrayAppend (a, note->data[PIZ_DURATION]);
                err |= pizGrowingArrayAppend (a, noteChannel);
                err |= pizGrowingArrayAppend (a, note->isSelected);
                err |= pizGrowingArrayAppend (a, (note == x->markedNote));
            }
            
            note = nextNote;
        }
    }
    
    x->index ++;
    //    
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
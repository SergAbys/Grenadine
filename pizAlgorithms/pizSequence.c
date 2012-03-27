/*
 * \file    pizSequence.c
 * \author  Jean Sapristi
 * \date    March 26, 2012.
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
    long k;
    
    PIZSEQUENCELOCK
    k = x->cell;
    PIZSEQUENCEUNLOCK
    
    return k;
}

PIZNoteValue pizSequenceGrid (PIZSequence *x)
{
    long k;
    
    PIZSEQUENCELOCK
    k = x->grid;
    PIZSEQUENCEUNLOCK
    
    return k;
}

PIZNoteValue pizSequenceNoteValue (PIZSequence *x)
{
    long k;
    
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
    long err = PIZ_GOOD;
    long *ptr = NULL;

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
    long err = PIZ_ERROR;
     
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

bool pizSequenceHasMarkedNote (PIZSequence *x)
{
    bool k;
    
    PIZSEQUENCELOCK
    k = (x->markedNote != NULL);
    PIZSEQUENCEUNLOCK;
    
    return k;
}

long pizSequenceMarkedNoteValue (PIZSequence *x, PIZSelector selector)
{
    long k = -1;
    
    PIZSEQUENCELOCK
    
    if (x->markedNote) {
        if (selector == PIZ_CHANNEL) {
            if (x->markedNote->data[PIZ_CHANNEL] == PIZ_SEQUENCE_CHANNEL_NONE) {
                k = x->channel;
            } else {
                k = x->markedNote->data[PIZ_CHANNEL];
            }
        } else {
            k = x->markedNote->data[selector];
        }
    }
        
    PIZSEQUENCEUNLOCK
    
    return k;
}

void pizSequenceChangeMarkedNoteValue (PIZSequence *x, PIZSelector selector, long value)
{
    PIZSEQUENCELOCK
    
    if (x->markedNote)  {
        long temp;
        
        if (selector == PIZ_DURATION) {
            long err = PIZ_GOOD;
            
            temp = x->markedNote->data[PIZ_DURATION];
            
            if (value > 0) {
                temp += x->grid;
            } else {
                temp -= x->grid;
            }
            
            err |= ((x->markedNote->position + temp) > x->timelineSize);
            err |= (temp > PIZ_SEQUENCE_MAXIMUM_DURATION);
            err |= (temp <= 0);
            
            if (!err) {
                x->markedNote->data[PIZ_DURATION] = temp;
                PIZ_SEQUENCE_TAG (x->markedNote->tag);
            }
        } else {
            temp = x->markedNote->data[selector];
            temp += value;
            
            switch (selector) {
                case PIZ_PITCH    : temp = CLAMP (temp, 0, PIZ_MAGIC_PITCH);    break;
                case PIZ_VELOCITY : temp = CLAMP (temp, 0, PIZ_MAGIC_VELOCITY); break;
                case PIZ_CHANNEL  : temp = CLAMP (temp, 0, PIZ_MAGIC_CHANNEL);  break;
            }
            
            if (x->markedNote->data[selector] != temp) {
                x->markedNote->data[selector] = temp;
                PIZ_SEQUENCE_TAG (x->markedNote->tag);
            }
        }
    }
        
    PIZSEQUENCEUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceClear (PIZSequence *x)
{
    PIZSEQUENCELOCK
    
    pizSequenceRemoveAllNotes (x);
    
    PIZSEQUENCEUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceNotesToArray (PIZSequence *x, PIZGrowingArray *a, PIZGrowingArray *b)
{
    long i, scale;
    long err = PIZ_GOOD;
    
    PIZSEQUENCELOCK
    
    scale = pizGrowingArrayCount (x->scale);
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long pitch;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            pitch = note->data[PIZ_PITCH];
            
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
                        
            if (note->isSelected && b) {
                err |= pizGrowingArrayAppend (b, note->position);
                err |= pizGrowingArrayAppend (b, CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
                err |= pizGrowingArrayAppend (b, note->data[PIZ_VELOCITY]);
                err |= pizGrowingArrayAppend (b, note->data[PIZ_DURATION]);
                err |= pizGrowingArrayAppend (b, note->data[PIZ_CHANNEL]);
                err |= pizGrowingArrayAppend (b, note->isSelected);
                err |= pizGrowingArrayAppend (b, (note == x->markedNote));
            } else if (a) {
                err |= pizGrowingArrayAppend (a, note->position);
                err |= pizGrowingArrayAppend (a, CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
                err |= pizGrowingArrayAppend (a, note->data[PIZ_VELOCITY]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_DURATION]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_CHANNEL]);
                err |= pizGrowingArrayAppend (a, note->isSelected);
                err |= pizGrowingArrayAppend (a, (note == x->markedNote));
            }

            note = nextNote;
        }
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

PIZError pizSequenceAddNotesWithArray (PIZSequence *x, const PIZGrowingArray *a, long flags)
{
    bool haveChanged = false;
    long err = PIZ_ERROR;
    
    PIZSEQUENCELOCK
    
    if (flags & PIZ_ADD_FLAG_CLEAR) {
        pizSequenceRemoveAllNotes (x);
    }
        
    if (a) {
        long i, count;
        long *ptr = NULL;
        
        err = PIZ_GOOD;
        
        count   = pizGrowingArrayCount (a) / PIZ_DATA_NOTE_SIZE;
        ptr     = pizGrowingArrayPtr (a);
        
        for (i = (count - 1); i >= 0; i--) {
            if (!(pizSequenceAddNote (x, ptr + (i * PIZ_DATA_NOTE_SIZE), flags))) {
                err |= PIZ_ERROR;
            } else {
                haveChanged = true;
            }
        }
        
        if (haveChanged) {
            pizSequenceMakeMap (x);
        }
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceZoneToArray (PIZSequence *x, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;

    PIZSEQUENCELOCK
    
    if (a) {
        err = PIZ_GOOD;
        
        err |= pizGrowingArrayAppend (a, x->start);
        err |= pizGrowingArrayAppend (a, x->end);
        err |= pizGrowingArrayAppend (a, x->down);
        err |= pizGrowingArrayAppend (a, x->up);
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

PIZError pizSequenceSetZoneWithArray (PIZSequence *x, const PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZSEQUENCELOCK
    
    if (a && (pizGrowingArrayCount (a) == PIZ_DATA_ZONE_SIZE)) {
        long tempStart  = pizGrowingArrayValueAtIndex (a, PIZ_DATA_START);
        long tempEnd    = pizGrowingArrayValueAtIndex (a, PIZ_DATA_END);
        long tempDown   = pizGrowingArrayValueAtIndex (a, PIZ_DATA_DOWN);
        long tempUp     = pizGrowingArrayValueAtIndex (a, PIZ_DATA_UP);
        
        tempStart   = CLAMP (tempStart, 0, x->timelineSize);
        tempEnd     = CLAMP (tempEnd,   0, x->timelineSize);
        tempDown    = CLAMP (tempDown,  0, PIZ_MAGIC_PITCH);
        tempUp      = CLAMP (tempUp,    0, PIZ_MAGIC_PITCH);

        if (tempStart != tempEnd) {
            if (tempEnd < tempStart) {
                long k      = tempEnd;
                tempEnd     = tempStart;
                tempStart   = k;
            }
            
            if (tempUp < tempDown) {
                long k      = tempUp;
                tempUp      = tempDown;
                tempDown    = k;
            }
            
            x->start = tempStart;
            x->end   = tempEnd;
            x->down  = tempDown;
            x->up    = tempUp;
            
            x->changedZone = true;
            
            err = PIZ_GOOD;
        } 
    }
    
    PIZSEQUENCEUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceTranspose (PIZSequence *x, long n)
{
    long i, a, b;
        
    PIZSEQUENCELOCK
    
    a = CLAMP (x->down + n, 0, PIZ_MAGIC_PITCH);
    b = CLAMP (x->up + n, 0, PIZ_MAGIC_PITCH);
    
    if (x->down != a) {
        x->down = a;
        x->changedZone = true;
    }
    
    if (x->up != b) {
        x->up = b;
        x->changedZone = true;
    }
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long temp, p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            temp = CLAMP (note->data[PIZ_PITCH] + n, 0, PIZ_MAGIC_PITCH);
            if (note->data[PIZ_PITCH] != temp) {
                note->data[PIZ_PITCH] = temp;
                PIZ_SEQUENCE_TAG (note->tag);
            }
            
            note = nextNote;
        }
    }
    
    PIZSEQUENCEUNLOCK
}

bool pizSequenceClean (PIZSequence *x, long value)
{
    long i, scale, v;
    long index = 0;
    bool haveChanged = false;

    PIZSEQUENCELOCK
        
    scale = pizGrowingArrayCount (x->scale);
    v = CLAMP (value, 0, PIZ_MAGIC_PITCH);
    
    for (i = 0; i < (PIZ_MAGIC_PITCH + 1); i++) {
        x->values1[i] = 0;
    }
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long j, start, end, m, n, pitch;
            bool death = false;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            pitch = note->data[PIZ_PITCH];
                    
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
            
            start   = pitch - v;
            end     = start + (2 * v);
            
            m = CLAMP (start, 0, PIZ_MAGIC_PITCH);
            n = CLAMP (end, 0, PIZ_MAGIC_PITCH);
            
            for (j = m; j <= n; j++) {
                if (x->values1[j] == (p + 1)) {
                    death = true;
                }
            }
            
            if (death) {
                x->notes1[index] = note;
                index ++;
            } else {
                x->values1[pitch] = (p + 1);
            }
            
            note = nextNote;
        }
    }
    
    if (index) {
        for (i = 0; i < index; i++) {
            pizSequenceRemoveNote (x, x->notes1[i]);
        }
        
        haveChanged = true;
    }
    
    PIZSEQUENCEUNLOCK
    
    return haveChanged;
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
    long err = PIZ_ERROR;
    
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
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZNote *pizSequenceAddNote (PIZSequence *x, long *values, long flags)
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
    
    if (flags & PIZ_ADD_FLAG_SNAP) {
        position = MAX (((long)(position / (double)x->grid)) * x->grid, 0);
    }
    
    if (flags & PIZ_ADD_FLAG_PATTERN) {
        long patternSize = pizGrowingArrayCount (x->pattern);  
        if (patternSize) {
            position = pizSequenceSnapPositionToPattern (x, position, patternSize);
        }
    } 
    
    if (flags & PIZ_ADD_FLAG_AMBITUS) {
        pitch = pizSequenceMovePitchToAmbitus (x, pitch);
    }
        
    if (flags & PIZ_ADD_FLAG_UNSELECT) {
        isSelected = false;
        isMarked = false;
    }
                
    err |= (position < 0);
    err |= (position > (x->timelineSize - MAX (duration, 1)));
    err |= (x->count >= PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (flags & PIZ_ADD_FLAG_CLIP) {
        err |= (position < x->start);
        err |= (position >= x->end);
        err |= (pitch < x->down);
        err |= (pitch > x->up);
    }
        
    if (!err) {
    //
    err |= pizBoundedStackPop (x->ticketMachine);
    
    if (!err && (newNote = (PIZNote *)malloc (sizeof(PIZNote)))) {
        newNote->flags              = PIZ_NOTE_FLAG_NONE;
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
    long err = PIZ_GOOD; 
    long position = note->position;
    
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
                PIZ_SEQUENCE_TAG (note->tag);
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
// -----------------------------------------------------------------------------------------------------------:x
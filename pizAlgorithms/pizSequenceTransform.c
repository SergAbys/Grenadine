/*
 * \file    pizSequenceTransform.c
 * \author  Jean Sapristi
 * \date    April 9, 2012.
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

#include "pizSequenceTransform.h"
#include "pizSequenceLibrary.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <math.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_NOVEMBER_H_SIZE         6
#define PIZ_JULIET_MAXIMUM_OFFSET   6
#define PIZ_MAXIMUM_LOOP            20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CEIL(a,b) (((a)%(b))==0?(a)/(b):(a)/(b)+1)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const long piz_neighbors[ ]  = { -256, 
                                        -130, 
                                        -129, 
                                        -128, 
                                        -127, 
                                        -126, 
                                         126, 
                                         127, 
                                         128, 
                                         129, 
                                         130, 
                                         256, 
                                          -2, 
                                          -1, 
                                           1, 
                                           2  };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_NEIGHBORS_BIRTH_SIZE    12
#define PIZ_NEIGHBORS_DEATH_SIZE    16

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const long piz_divisions[ ]  = { 2, 3, 4, 5, 7, 11 };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DIVISIONS_SIZE          6

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const double piz_distribution2[ ]  = { 0.75, 1. };
static const double piz_distribution3[ ]  = { 0.68, 0.85, 1. };
static const double piz_distribution4[ ]  = { 0.63, 0.75, 0.87, 1. };
static const double piz_distribution5[ ]  = { 0.60, 0.70, 0.80, 0.90, 1. };
static const double piz_distribution7[ ]  = { 0.56, 0.63, 0.70, 0.77, 0.84, 0.91, 1. };
static const double piz_distribution11[ ] = { 0.54, 0.59, 0.63, 0.68, 0.72, 0.77, 0.81, 0.86, 0.9, 0.95, 1. };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceAlgorithm (PIZSequence *x, PIZAlgorithm *algorithm)
{
    long                k;
    PIZError            err = PIZ_ERROR;
    PIZAlgorithmMethod  count = NULL;
    PIZAlgorithmMethod  proceed = NULL;
    
    k = pizSequencePickUpNotes (x);

    count = algorithm->count;
    proceed = algorithm->proceed;
    
    if ((*count)(algorithm)) {
        err = (*proceed)(algorithm, k, x->tempValues);
    }
    
    if (!err) {
        long i, h;  
        
        for (i = 0; i < k; i++) {
            x->tempValues[i] = pizSequenceMovePitchToAmbitus (x, x->tempValues[i]);
        }
        
        for (i = 0; i < k; i++) {        
            h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            if (h >= x->chance) {
                x->tempValues[i] = x->tempNotes1[i]->midi[PIZ_MIDI_PITCH];
            } 
        }
        
        pizSequenceFillValues (x, PIZ_MIDI_PITCH, k, 0);
    }
    
    return err;
}

PIZError pizSequenceNovember (PIZSequence *x, long iterate)
{
    PIZError err1 = PIZ_ERROR;
                                                    
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long i, start, end, mapCount, scale;
    long k = 0;
    long loop = 0;
    bool haveChanged = false;
    
    err1 = PIZ_GOOD;
    
    pizBoundedHashTableClear (x->tempHash);

    start       = PIZ_CEIL (x->start, x->cell);
    end         = PIZ_CEIL (x->end, x->cell);
    mapCount    = pizArrayCount (x->map);
    scale       = pizArrayCount (x->scale);
    
    for (i = 0; i < mapCount; i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long key, offset = 0;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (scale) {
                offset = pizArrayValueAtIndex (x->scale, note->midi[PIZ_MIDI_PITCH] % scale);
            }
            
            key = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1));
            key += note->midi[PIZ_MIDI_PITCH] + offset;
            
            err1 |= pizBoundedHashTableAdd (x->tempHash, key, (void *)note);
            
            note = nextNote;
        }
    }

    while (!err1 && (k < iterate) && (loop < PIZ_MAXIMUM_LOOP)) {
    //
    if (x->count && (mapCount = pizArrayCount (x->map))) {
        long    j, here, previous, next, pitch, hCenter;
        long    q = -1;
        long    p = -1;
        long    hPat[PIZ_NOVEMBER_H_SIZE] = { -1, -1, -1, -1, -1, -1 };
        long    offset = 0;
        long    neighbors = 0;
        long    err2 = PIZ_GOOD;
        long    patternSize = pizArrayCount (x->pattern);
        bool    death = false;
        PIZNote *note = NULL;

        while (q == -1) {
            p = pizArrayValueAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
            if (pizLinklistCount (x->timeline[p])) {
                q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
            } 
        }                           

        pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note);
        
        if (scale) {
            offset = pizArrayValueAtIndex (x->scale, note->midi[PIZ_MIDI_PITCH] % scale);
        }
        
        pitch = note->midi[PIZ_MIDI_PITCH] + offset;
        
        hCenter = ((long)(note->position / (double)x->cell)) * (PIZ_MAGIC_PITCH + 1);
        hCenter += note->midi[PIZ_MIDI_PITCH] + offset;
        
        err2 |= (note->position < x->start);
        err2 |= (note->position >= x->end);
        err2 |= (pitch < x->down);
        err2 |= (pitch > x->up);
        
        if (!err2) {
            for (j = 0; j < PIZ_NEIGHBORS_DEATH_SIZE; j++) {
                long key = hCenter + piz_neighbors[j];
                if (pizBoundedHashTableContainsKey (x->tempHash, key)) {
                    neighbors ++;
                }
            }
            
            if (neighbors > 1) {
                death = true;
            }
        }
            
        here     = (long)(note->position / (double)x->cell);
        previous = here - 1;
        next     = here + 1;
        
        if (next >= end) {
            next = start;
        }
        
        if (previous < start) {
            previous = (end - 1);
        }
        
        if (patternSize) {  
            previous += pizArrayValueAtIndex (x->pattern, previous % patternSize);
            next     += pizArrayValueAtIndex (x->pattern, next % patternSize);
        }

        if (previous != here) {
            hPat[1] = (previous * (PIZ_MAGIC_PITCH + 1)) + pitch;
            
            if (scale) {
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[0] = hPat[1] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[2] = hPat[1] + j + n;
                        break;
                    }
                }
            } else {
                hPat[0] = hPat[1] - 1;
                hPat[2] = hPat[1] + 1;
            }
        }
        
        if (next != here) {
            hPat[4] = (next * (PIZ_MAGIC_PITCH + 1)) + pitch;
            
            if (scale) {
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[3] = hPat[4] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[5] = hPat[4] + j + n;
                        break;
                    }
                }
            } else {
                hPat[3] = hPat[4] - 1;
                hPat[5] = hPat[4] + 1;
            }
        }
            
        for (j = 5; j > 0; j--)  {
            long h          = (j + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            long tempHPat   = hPat[h];
            hPat[h]         = hPat[j];
            hPat[j]         = tempHPat;
        }
        
        for (j = 0; j < PIZ_NOVEMBER_H_SIZE; j++) {
            if ((hPat[j] >= 0) &&   
                !(pizBoundedHashTableContainsKey (x->tempHash, hPat[j] - 2)) &&
                !(pizBoundedHashTableContainsKey (x->tempHash, hPat[j] - 1)) &&
                !(pizBoundedHashTableContainsKey (x->tempHash, hPat[j]))     &&
                !(pizBoundedHashTableContainsKey (x->tempHash, hPat[j] + 1)) &&
                !(pizBoundedHashTableContainsKey (x->tempHash, hPat[j] + 2))) {
                long    t;
                PIZNote *ptr = NULL;
                PIZNote *noteToCopy = NULL;
                
                neighbors = 0;
        
                for (t = 0; t < PIZ_NEIGHBORS_BIRTH_SIZE; t++)  {
                    long key = hPat[j] + piz_neighbors[t];
                    if (!(pizBoundedHashTablePtrByKey (x->tempHash, key, (void **)&ptr))) {
                        neighbors ++;
                        noteToCopy = ptr;
                    }
                }
                
                if (neighbors == 1) {
                //
                long values[ ] = {  ((long)(hPat[j] / (double)(PIZ_MAGIC_PITCH + 1))) * x->cell,
                                    hPat[j] % (PIZ_MAGIC_PITCH + 1),
                                    noteToCopy->midi[PIZ_MIDI_VELOCITY], 
                                    noteToCopy->midi[PIZ_MIDI_DURATION],
                                    noteToCopy->midi[PIZ_MIDI_CHANNEL],
                                    false,  
                                    false };
                                    
                PIZNote *newNote = pizSequenceNewNote (x, values, PIZ_SEQUENCE_FLAG_CLIP);
                
                if (newNote) {
                    err1 |= pizBoundedHashTableAdd (x->tempHash, hPat[j], (void *)newNote);
                    haveChanged = true;
                    k ++;
                } 
                
                break;
                //
                }
            }
        }
            
        if (death) {
            pizBoundedHashTableRemoveByKey (x->tempHash, hCenter, (void *)note);
            pizSequenceRemoveNote (x, note);
            haveChanged = true;
        }
        
    } else {
        break;
    }
    
    loop ++;
    //    
    }

    if (haveChanged) {
        pizSequenceMakeMap (x);
    }
    //
    }
            
    return err1;
}

PIZError pizSequenceJuliet (PIZSequence *x, long iterate, long division)
{
    bool     haveChanged = false;
    PIZError err = PIZ_ERROR;
    
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long i, start, end, size;
    long b = 0;
    
    start = PIZ_CEIL (x->start, x->cell);
    end   = PIZ_CEIL (x->end, x->cell); 
    
    if (size = (end - start)) {
        for (i = 0; i < PIZ_DIVISIONS_SIZE; i++) {
            if (!(size % piz_divisions[i])) {
                b = piz_divisions[i];
                if (b == division) {
                    break;
                }
            }
        }
    }
    
    if (b) {
    //
    long k = 0;
    long loop = 0;
    long a = size / b;
    long mapCount = pizArrayCount (x->map);
    long scale = pizArrayCount (x->scale);
     
    err = PIZ_GOOD;

    pizBoundedHashTableClear (x->tempHash);

    for (i = 0; i < mapCount; i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long key, offset = 0;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (scale) {
                offset = pizArrayValueAtIndex (x->scale, note->midi[PIZ_MIDI_PITCH] % scale);
            }
            
            key = (long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1);
            key += note->midi[PIZ_MIDI_PITCH] + offset;
            
            err |= pizBoundedHashTableAdd (x->tempHash, key, (void *)note);
            
            note = nextNote;
        }
    }
    
    while (!err && x->count && (k < iterate) && (loop < PIZ_MAXIMUM_LOOP)) {
    //
    long    j, patternSize, step, newKey, newPosition;
    long    q = -1;
    long    p = -1;
    long    offset = 0;
    double  h = rand_r (&x->seed) / (RAND_MAX + 1.0);
    PIZNote *note1 = NULL;
    PIZNote *note2 = NULL;
    
    const double *distribution = NULL;

    mapCount = pizArrayCount (x->map);

    while (q == -1) {
        p = pizArrayValueAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
        if (pizLinklistCount (x->timeline[p])) {
            q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        } 
    }                           

    pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note1);     
    
    step = MIN (a, PIZ_JULIET_MAXIMUM_OFFSET) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    step *= b;
    
    switch (b) {
        case 2  :   distribution = piz_distribution2;   break;
        case 3  :   distribution = piz_distribution3;   break;
        case 4  :   distribution = piz_distribution4;   break;
        case 5  :   distribution = piz_distribution5;   break;
        case 7  :   distribution = piz_distribution7;   break;
        case 11 :   distribution = piz_distribution11;  break;
    }
    
    for (j = 0; j < (b - 1); j++) {
        if (h < distribution[j]) {
            break;
        }
    }
    
    step += j;
    
    newPosition = ((long)(note1->position / (double)x->cell)) + step;
    
    if (newPosition >= end) {
        newPosition = (newPosition - end) + start;
    }
    
    if (patternSize = pizArrayCount (x->pattern)) {
        newPosition += pizArrayValueAtIndex (x->pattern, newPosition % patternSize);
    }
    
    if (scale) {
        offset = pizArrayValueAtIndex (x->scale, note1->midi[PIZ_MIDI_PITCH] % scale);
    }
                
    newKey = (newPosition * (PIZ_MAGIC_PITCH + 1)) + note1->midi[PIZ_MIDI_PITCH] + offset;
    
    if (!(pizBoundedHashTableContainsKey (x->tempHash, newKey))) {
    //
        long values[ ] = { newPosition * x->cell,
                           note1->midi[PIZ_MIDI_PITCH] + offset,
                           note1->midi[PIZ_MIDI_VELOCITY],
                           note1->midi[PIZ_MIDI_DURATION],
                           note1->midi[PIZ_MIDI_CHANNEL],
                           false, 
                           false };

        note2 = pizSequenceNewNote (x, values, PIZ_SEQUENCE_FLAG_CLIP);
        
        if (note2) {
            err |= pizBoundedHashTableAdd (x->tempHash, newKey, (void *)note2);
            haveChanged = true;
            k ++;
        } 
    //
    } else if (!(pizBoundedHashTablePtrByKey (x->tempHash, newKey, (void **)&note2))) {
        if (note2 != note1) {
            pizBoundedHashTableRemoveByKey (x->tempHash, newKey, (void *)note2);
            pizSequenceRemoveNote (x, note2);
            haveChanged = true;
            k ++;
        }
    }

    loop ++;
    //    
    }
    //
    }
    
    if (haveChanged) {
        pizSequenceMakeMap (x);
    }
    //
    }
                        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceClear (PIZSequence *x)
{
    pizSequenceRemoveAllNotes (x);
}

void pizSequenceTranspose (PIZSequence *x, long n)
{
    long i, a, b;
        
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
    
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long temp, p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            temp = CLAMP (note->midi[PIZ_MIDI_PITCH] + n, 0, PIZ_MAGIC_PITCH);
            if (note->midi[PIZ_MIDI_PITCH] != temp) {
                note->midi[PIZ_MIDI_PITCH] = temp;
                pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            }
            
            note = nextNote;
        }
    }
}

void pizSequenceClean (PIZSequence *x, long value)
{
    long i, scale, v, index = 0;
        
    scale = pizArrayCount (x->scale);
    v = CLAMP (value, 0, PIZ_MAGIC_PITCH);
    
    for (i = 0; i < (PIZ_MAGIC_PITCH + 1); i++) {
        x->tempValues[i] = 0;
    }
    
    for (i = 0; i < pizArrayCount (x->map); i++) { 
    //  
    PIZNote *note = NULL;
    PIZNote *nextNote = NULL;
    
    long p = pizArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
        long j, start, end, m, n, pitch;
        bool death = false;
        
        pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
        
        pitch = note->midi[PIZ_MIDI_PITCH];
                
        if (scale) {
            pitch += pizArrayValueAtIndex (x->scale, pitch % scale);
        }
        
        start = pitch - v;
        end   = start + (2 * v);
        
        m = CLAMP (start, 0, PIZ_MAGIC_PITCH);
        n = CLAMP (end, 0, PIZ_MAGIC_PITCH);
        
        for (j = m; j <= n; j++) {
            if (x->tempValues[j] == (p + 1)) {
                death = true;
            }
        }
        
        if (death) {
            x->tempNotes1[index] = note;
            index ++;
        } else {
            x->tempValues[pitch] = (p + 1);
        }
        
        note = nextNote;
    }
    //
    }
    
    if (index) {
        for (i = 0; i < index; i++) {
            pizSequenceRemoveNote (x, x->tempNotes1[i]);
        }
    }
}

void pizSequenceRotate (PIZSequence *x, PIZMidi selector, long shift)
{
    long i, k = 0;
            
    k = pizSequencePickUpNotes (x);
    
    if (k && shift < 0) {
        shift = k - ((-shift) % k);
    }

    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes1[(i + shift) % k]->midi[selector];
    }
                
    pizSequenceFillValues (x, selector, k, 0);
}

void pizSequenceScramble (PIZSequence *x, PIZMidi selector)
{
    long i, k = 0;
        
    k = pizSequencePickUpNotes (x);
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes1[i]->midi[selector];
    }
        
    for (i = (k - 1); i > 0; i--) {
        long    h = (i + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        PIZNote *temp = NULL;
            
        temp = x->tempNotes1[h];
        x->tempNotes1[h] = x->tempNotes1[i];
        x->tempNotes1[i] = temp;
    }
            
    pizSequenceFillValues (x, selector, k, 0);
}

void pizSequenceSort (PIZSequence *x, PIZMidi selector, bool down)
{
    long i, scale, k = 0;
        
    k = pizSequencePickUpNotes (x);
    scale = pizArrayCount (x->scale);
    
    for (i = 0; i < PIZ_SEQUENCE_INIT_TEMP_SIZE; i++) {
        x->tempValues[i] = 0;
    }
    
    if (selector == PIZ_MIDI_PITCH) {
        for (i = 0; i < k; i++) { 
            long pitch = x->tempNotes1[i]->midi[PIZ_MIDI_PITCH];
                            
            if (scale) {
                pitch += pizArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] ++; 
        }
    } else {
        for (i = 0; i < k; i++) { 
            x->tempValues[x->tempNotes1[i]->midi[selector]] ++; 
        }   
    }
        
    for (i = 1; i < PIZ_SEQUENCE_INIT_TEMP_SIZE; i++) {
        x->tempValues[i] += x->tempValues[i - 1];
    }
                
    if (selector == PIZ_MIDI_PITCH) {
        for (i = (k - 1); i >= 0; i--) {
            long pitch = x->tempNotes1[i]->midi[PIZ_MIDI_PITCH];
                                
            if (scale) {
                pitch += pizArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->tempNotes2[x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] - 1] = x->tempNotes1[i];
            x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] --;
        }
    } else {
        for (i = (k - 1); i >= 0; i--) {    
            x->tempNotes2[x->tempValues[x->tempNotes1[i]->midi[selector]] - 1] = x->tempNotes1[i];
            x->tempValues[x->tempNotes1[i]->midi[selector]] --;
        }
    }
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes2[i]->midi[selector];
    }
                
    pizSequenceFillValues (x, selector, k, down);
}

void pizSequenceChange (PIZSequence *x, PIZMidi selector, long value)
{
    long i;
            
    for (i = 0; i < pizArrayCount (x->map); i++) {  
    // 
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long m, t;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
    //
    switch (selector) {
    //
    case PIZ_MIDI_PITCH    : t = CLAMP (note->midi[PIZ_MIDI_PITCH] + value, 0, PIZ_MAGIC_PITCH); break;
    case PIZ_MIDI_VELOCITY : t = CLAMP (note->midi[PIZ_MIDI_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); break;
    case PIZ_MIDI_DURATION : m = MIN   (x->timelineSize - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                             t = CLAMP (note->midi[PIZ_MIDI_DURATION] + value, 1, m); break;
    case PIZ_MIDI_CHANNEL  : t = CLAMP (note->midi[PIZ_MIDI_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL); break;
    //                            
    }
    
    if (note->midi[selector] != t) {
        note->midi[selector] = t;
        pizItemset128SetAtIndex (&x->changedNotes, note->tag);
    }
    //
    }
    
    note = nextNote;
    //
    }
    //
    }
}

void pizSequenceSet (PIZSequence *x, PIZMidi selector, long value)
{
    long i;
            
    for (i = 0; i < pizArrayCount (x->map); i++) {
    //   
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long m, t;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
        switch (selector) {
        case PIZ_MIDI_PITCH    : t = CLAMP (value, 0, PIZ_MAGIC_PITCH); break;
        case PIZ_MIDI_VELOCITY : t = CLAMP (value, 0, PIZ_MAGIC_VELOCITY); break;
        case PIZ_MIDI_DURATION : m = MIN   (x->timelineSize - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                 t = CLAMP (value, 1, m); break;
        case PIZ_MIDI_CHANNEL  : t = CLAMP (value, 0, PIZ_MAGIC_CHANNEL); break;
        }
        
        if (note->midi[selector] != t) {
            note->midi[selector] = t;
            pizItemset128SetAtIndex (&x->changedNotes, note->tag);
        }
    }
    
    note = nextNote;
    //
    }
    //
    }
}

void pizSequenceRandom (PIZSequence *x, PIZMidi selector, long minValue, long maxValue)
{
    long i, range;
        
    if (minValue > maxValue) {
        long temp = minValue;
        minValue = maxValue;
        maxValue = temp;
    }
    
    range = (maxValue - minValue) + 1;
        
    for (i = 0; i < pizArrayCount (x->map); i++) {
    //   
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long m, t;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
    //
    long value = minValue + (long)(range * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
    
    switch (selector) {
    case PIZ_MIDI_PITCH    : t = CLAMP (note->midi[PIZ_MIDI_PITCH] + value, 0, PIZ_MAGIC_PITCH); break;
    case PIZ_MIDI_VELOCITY : t = CLAMP (note->midi[PIZ_MIDI_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); break;
    case PIZ_MIDI_DURATION : m = MIN   (x->timelineSize - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                             t = CLAMP (note->midi[PIZ_MIDI_DURATION] + value, 1, m); break;
    case PIZ_MIDI_CHANNEL  : t = CLAMP (note->midi[PIZ_MIDI_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL); break;
    }
    
    if (note->midi[selector] != t) {
        note->midi[selector] = t;
        pizItemset128SetAtIndex (&x->changedNotes, note->tag);
    }
    //
    }
    
    note = nextNote;
    //
    }
    //
    }
}

void pizSequenceKill (PIZSequence *x)
{
    long i;
    bool haveChanged = false;
        
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (100 * (rand_r (&x->seed) / (RAND_MAX + 1.0)) < x->chance) {
                pizSequenceRemoveNote (x, note);
                haveChanged = true;
            }
                
            note = nextNote;
        }
    }
    
    if (haveChanged) {
        pizSequenceMakeMap (x);
    }
}

void pizSequenceCycle (PIZSequence *x, PIZScaleKey key, const PIZArray *a)
{
    if (a) {
    //
    long count = pizArrayCount (a);
                
    if (count > 1) {
    //
    long i;
    long t[PIZ_MAGIC_SCALE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    long s[PIZ_MAGIC_SCALE];
    long m, n, o, temp;
    long scale = pizArrayCount (x->scale);
    
    temp = pizArrayValueAtIndex (a, 0);
    m = CLAMP (temp, 0, PIZ_MAGIC_SCALE - 1);
    o = m;
    
    for (i = 1; i < count; i++) {
        temp = pizArrayValueAtIndex (a, i);
        n = CLAMP (temp, 0, PIZ_MAGIC_SCALE - 1);
        t[m] = n - m;
        m = n;
    }
    
    t[m] = o - m;
    
    for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
        s[i] = t[(PIZ_MAGIC_SCALE - key + i) % PIZ_MAGIC_SCALE];
    }
        
    for (i = 0; i < pizArrayCount (x->map); i++) {   
    //
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
        long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        
        pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
        
        if (h < x->chance) {
            long pitch, offset = 0;
            
            pitch = note->midi[PIZ_MIDI_PITCH];
            
            if (scale) {
                offset += pizArrayValueAtIndex (x->scale, pitch % scale);
            }
            
            pitch = CLAMP (pitch + offset, 0, PIZ_MAGIC_PITCH);
            pitch += s[pitch % PIZ_MAGIC_SCALE];
    
            if (pitch != (note->midi[PIZ_MIDI_PITCH] + offset)) {
                note->midi[PIZ_MIDI_PITCH] = CLAMP (pitch, 0, PIZ_MAGIC_PITCH);
                pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            }
        }

        note = nextNote;
    }
    //
    }
    //    
    }
    //    
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
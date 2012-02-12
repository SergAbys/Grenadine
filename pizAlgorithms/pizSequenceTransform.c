/*
 * \file    pizSequenceTransform.c
 * \author  Jean Sapristi
 * \date    26 janvier 2012
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <math.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_H_SIZE                    6
#define PIZ_NEIGHBORHOOD_BIRTH_SIZE   12
#define PIZ_NEIGHBORHOOD_DEATH_SIZE   16
#define PIZ_DIVISIONS_SIZE            6
#define PIZ_MAXIMUM_OFFSET            6
#define PIZ_BREAK_LOOP                20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CEIL(a,b)   (((a)%(b))==0?(a)/(b):(a)/(b)+1)
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long piz_neighbors[ ] = {-256, -130, -129, -128, -127, -126, 126, 127, 128, 129, 130, 256, -2, -1, 1, 2};

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long     piz_divisions[ ]      = {2, 3, 4, 5, 7, 11};
static double   piz_distribution2[ ]  = {0.75, 1.};
static double   piz_distribution3[ ]  = {0.68, 0.85, 1.};
static double   piz_distribution4[ ]  = {0.63, 0.75, 0.87, 1.};
static double   piz_distribution5[ ]  = {0.60, 0.70, 0.80, 0.90, 1.};
static double   piz_distribution7[ ]  = {0.56, 0.63, 0.70, 0.77, 0.84, 0.91, 1.};
static double   piz_distribution11[ ] = {0.54, 0.59, 0.63, 0.68, 0.72, 0.77, 0.81, 0.86, 0.9, 0.95, 1.};

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceProceedAlgorithm (PIZSequence *x, PIZAlgorithm flag, void *algorithm)
{
    long k;
    long err = PIZ_ERROR;
    bool haveChanged = false;
    
    PIZLOCK
    
    pizSequenceCleanMap (x);
    
    k = pizSequencePickUpNotes (x);
    
    if ((flag == PIZ_FACTOR_ORACLE) && pizFactorOracleCount ((PIZFactorOracle *)algorithm)) {
            err = pizFactorOracleProceed ((PIZFactorOracle *)algorithm, k, x->values1);
        }
    else if ((flag == PIZ_GALOIS_LATTICE) && pizGaloisLatticeCount ((PIZGaloisLattice *)algorithm)) {
            err = pizGaloisLatticeProceed ((PIZGaloisLattice *)algorithm, k, x->values1);
        }
    else if ((flag == PIZ_FINITE_STATE) && pizFiniteStateCount ((PIZFiniteState *)algorithm)) {
            err = pizFiniteStateProceed ((PIZFiniteState *)algorithm, k, x->values1);
        }
    
    if (!err) {
        long i, h;  
        
        for (i = 0; i < k; i++) {
                x->values1[i] = pizSequenceMovePitchToAmbitus (x, x->values1[i]);
            }
        
        for (i = 0; i < k; i++) {        
            h = 100 * (rand ( ) / (RAND_MAX + 1.0));
            if (h >= x->chance) {
                x->values1[i] = x->notes1[i]->data[PIZ_PITCH];
            } else {
                haveChanged = true;
            }
        }
        
        pizSequenceFillValues (x, PIZ_PITCH, k, 0);
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceCellularAutomata (PIZSequence *x, long iterate)
{
    bool haveChanged = false;
                                                
    PIZLOCK
    
    if (x->grid != PIZ_SNAP_NONE) {
        long i, start, end, mapCount, scale;
        long k = 0;
        long loop = 0;
        
        pizBoundedHashTableClear (x->hashTable);

        start       = PIZ_CEIL (x->start, x->grid);
        end         = PIZ_CEIL (x->end, x->grid);
        mapCount    = pizGrowingArrayCount (x->map);
        scale       = pizGrowingArrayCount (x->scale);
        
        for (i = 0; i < mapCount; i++) {   
            PIZNote *note       = NULL;
            PIZNote *nextNote   = NULL;
            
            long p = pizGrowingArrayValueAtIndex (x->map, i);
            
            pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
            
            while (note) {
                long key, offset = 0;
                
                pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                
                if (scale) {
                    offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_PITCH] % scale);
                }
                
                key = ((long)(note->position / (double)x->grid) * (PIZ_MIDI_PITCH + 1)) 
                    + (note->data[PIZ_PITCH] + offset);
                
                pizBoundedHashTableAdd (x->hashTable, key, (void *)note);
                
                note = nextNote;
            }
        }
    
        while (k < iterate && loop < PIZ_BREAK_LOOP) {
            if (x->count && (mapCount = pizGrowingArrayCount (x->map))) {
                long    j, here, previous, next, pitch, hCenter;
                long    q = -1;
                long    p = -1;
                long    hPat [PIZ_H_SIZE] = {-1, -1, -1, -1, -1, -1};
                long    offset = 0;
                long    neighbors = 0;
                long    err = PIZ_GOOD;
                long    patternSize = pizGrowingArrayCount (x->pattern);
                bool    death = false;
                PIZNote *note = NULL;
        
                while (q == -1) {
                    p = pizGrowingArrayValueAtIndex (x->map, (mapCount * (rand ( ) / (RAND_MAX + 1.0))));
                    if (pizLinklistCount (x->timeline[p])) {
                        q = pizLinklistCount (x->timeline[p]) * (rand ( ) / (RAND_MAX + 1.0));
                    } 
                }                           

                pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note);
                
                if (scale) {
                    offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_PITCH] % scale);
                }
                
                pitch = note->data[PIZ_PITCH] + offset;
                
                hCenter = (((long)(note->position / (double)x->grid)) * (PIZ_MIDI_PITCH + 1)) 
                    + (note->data[PIZ_PITCH] + offset);
                
                err |= (note->position < x->start);
                err |= (note->position >= x->end);
                err |= (pitch < x->down);
                err |= (pitch > x->up);
                
                if (!err) {
                    for (j = 0; j < PIZ_NEIGHBORHOOD_DEATH_SIZE; j++) {
                        long key = hCenter + piz_neighbors[j];
                        if (pizBoundedHashTableContainsKey (x->hashTable, key)) {
                                neighbors ++;
                            }
                    }
                    
                    if (neighbors > 1) {
                        death = true;
                    }
                }
                    
                here        = (long)(note->position / (double)x->grid);
                previous    = here - 1;
                next        = here + 1;
                
                if (next >= end) {
                    next = start;
                }
                
                if (previous < start) {
                    previous = (end - 1);
                }
                
                if (patternSize) {  
                    previous += pizGrowingArrayValueAtIndex (x->pattern, previous % patternSize);
                    next     += pizGrowingArrayValueAtIndex (x->pattern, next % patternSize);
                }

                if (previous != here) {
                    hPat[1] = (previous * (PIZ_MIDI_PITCH + 1)) + pitch;
                    
                    if (scale) {
                        for (j = 1; j < PIZ_SCALE_SIZE; j++) {
                            long n;
                            long t = pitch - j;
                            long b = CLAMP (t, 0, PIZ_MIDI_PITCH);
                            
                            n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                            
                            if ((b + n) != pitch) {
                                    hPat[0] = hPat[1] - j + n;
                                    break;
                                }
                        }
                        
                        for (j = 1; j < PIZ_SCALE_SIZE; j++) {
                            long n;
                            long t = pitch + j;
                            long b = CLAMP (t, 0, PIZ_MIDI_PITCH);
                            
                            n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                            
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
                    hPat[4] = (next * (PIZ_MIDI_PITCH + 1)) + pitch;
                    
                    if (scale) {
                        for (j = 1; j < PIZ_SCALE_SIZE; j++) {
                            long n;
                            long t = pitch - j;
                            long b = CLAMP (t, 0, PIZ_MIDI_PITCH);
                            
                            n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                            
                            if ((b + n) != pitch) {
                                    hPat[3] = hPat[4] - j + n;
                                    break;
                                }
                        }
                        
                        for (j = 1; j < PIZ_SCALE_SIZE; j++) {
                            long n;
                            long t = pitch + j;
                            long b = CLAMP (t, 0, PIZ_MIDI_PITCH);
                            
                            n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                            
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
                    long h          = (j + 1) * (rand ( ) / (RAND_MAX + 1.0));
                    long tempHPat   = hPat[h];
                    hPat[h]         = hPat[j];
                    hPat[j]         = tempHPat;
                }
                
                for (j = 0; j < PIZ_H_SIZE; j++) {
                    if ((hPat[j] >= 0) && 
                    !pizBoundedHashTableContainsKey (x->hashTable, hPat[j] - 2) &&
                    !pizBoundedHashTableContainsKey (x->hashTable, hPat[j] - 1) &&
                    !pizBoundedHashTableContainsKey (x->hashTable, hPat[j])     &&
                    !pizBoundedHashTableContainsKey (x->hashTable, hPat[j] + 1) &&
                    !pizBoundedHashTableContainsKey (x->hashTable, hPat[j] + 2)) {
                        long    t;
                        PIZNote *ptr = NULL;
                        PIZNote *noteToCopy = NULL;
                        
                        neighbors = 0;
                
                        for (t = 0; t < PIZ_NEIGHBORHOOD_BIRTH_SIZE; t++)  {
                            long key = hPat[j] + piz_neighbors[t];
                            if (!pizBoundedHashTablePtrByKey (x->hashTable, key, (void **)&ptr)) {
                                neighbors ++;
                                noteToCopy = ptr;
                            }
                        }
                        
                        if (neighbors == 1) {
                            long    values[PIZ_SEQUENCE_NOTE_SIZE];
                            PIZNote *newNote = NULL;
                            
                            values[PIZ_SEQUENCE_POSITION] = 
                                ((long)(hPat[j] / (double)(PIZ_MIDI_PITCH + 1))) * x->grid;
                            values[PIZ_SEQUENCE_PITCH]          = hPat[j] % (PIZ_MIDI_PITCH + 1);
                            values[PIZ_SEQUENCE_VELOCITY]       = noteToCopy->data[PIZ_VELOCITY];
                            values[PIZ_SEQUENCE_DURATION]       = noteToCopy->data[PIZ_DURATION];
                            values[PIZ_SEQUENCE_CHANNEL]        = noteToCopy->data[PIZ_CHANNEL];
                            values[PIZ_SEQUENCE_IS_SELECTED]    = false;
                            values[PIZ_SEQUENCE_IS_MARKED]      = false;
                            
                            newNote = pizSequenceAddNote (x, values, PIZ_SEQUENCE_ADD_FLAG_CLIP);
                            
                            if (newNote) {
                                pizBoundedHashTableAdd (x->hashTable, hPat[j], (void *)newNote);
                                haveChanged = true;
                                k ++;
                            }
                            
                            break;
                        }
                    }
                }
                    
                if (death) {
                    if (note == x->markedNote) {
                        x->markedNote = NULL;
                    }
                
                    pizBoundedHashTableRemoveByKeyAndPtr (x->hashTable, hCenter, (void *)note);
                    
                    if (!pizLinklistRemoveByPtr (x->timeline[p], (void *)note)) {
                        x->count --;
                        haveChanged = true;
                    }
                }
            } else {
                break;
            }
            
            loop ++;
        }

        if (haveChanged) {
            PIZMAPDIRTY
            pizSequenceCleanMap (x);
        }
    }
        
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceGenerator (PIZSequence *x, long iterate, long division)
{
    bool haveChanged = false;

    PIZLOCK
    
    if (x->grid != PIZ_SNAP_NONE) {
        long i, start, end, size;
        long b = 0;
        
        start   = PIZ_CEIL (x->start, x->grid);
        end     = PIZ_CEIL (x->end, x->grid); 
        
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
            long k = 0;
            long loop = 0;
            long a = size / b;
            long mapCount = pizGrowingArrayCount (x->map);
            long scale = pizGrowingArrayCount (x->scale);
                                
            pizBoundedHashTableClear (x->hashTable);
    
            for (i = 0; i < mapCount; i++) {   
                PIZNote *note       = NULL;
                PIZNote *nextNote   = NULL;
                
                long p = pizGrowingArrayValueAtIndex (x->map, i);
                
                pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
                
                while (note) {
                    long key;
                    long offset = 0;
                    
                    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                    
                    if (scale) {
                        offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_PITCH] % scale);
                    }
        
                    key = ((long)(note->position / (double)x->grid) * (PIZ_MIDI_PITCH + 1))
                        + note->data[PIZ_PITCH] + offset;
                    
                    pizBoundedHashTableAdd (x->hashTable, key, (void *)note);
                    
                    note = nextNote;
                }
            }
            
            while (x->count && k < iterate && loop < PIZ_BREAK_LOOP) {
                long    j, patternSize, step, newKey, newPosition;
                long    q = -1;
                long    p = -1;
                long    offset = 0;
                double  h = rand ( ) / (RAND_MAX + 1.0);
                double  *distribution = NULL;
                PIZNote *note1 = NULL;
                PIZNote *note2 = NULL;
                
                mapCount = pizGrowingArrayCount (x->map);
        
                while (q == -1) {
                    p = pizGrowingArrayValueAtIndex (x->map, (mapCount * (rand ( ) / (RAND_MAX + 1.0))));
                    if (pizLinklistCount (x->timeline[p])) {
                        q = pizLinklistCount (x->timeline[p]) * (rand ( ) / (RAND_MAX + 1.0));
                    } 
                }                           

                pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note1);     
                
                step = MIN (a, PIZ_MAXIMUM_OFFSET) * (rand ( ) / (RAND_MAX + 1.0));
                step *= b;
                
                switch (b) {
                    case 2  :   distribution = piz_distribution2;     break;
                    case 3  :   distribution = piz_distribution3;     break;
                    case 4  :   distribution = piz_distribution4;     break;
                    case 5  :   distribution = piz_distribution5;     break;
                    case 7  :   distribution = piz_distribution7;     break;
                    case 11 :   distribution = piz_distribution11;    break;
                }
                
                for (j = 0; j < (b - 1); j++) {
                    if (h < distribution[j]) {
                        break;
                    }
                }
                
                step += j;
                
                newPosition = ((long)(note1->position / (double)x->grid)) + step;
                
                if (newPosition >= end) {
                    newPosition = (newPosition - end) + start;
                }
                
                if (patternSize = pizGrowingArrayCount (x->pattern)) {
                    newPosition += pizGrowingArrayValueAtIndex (x->pattern, newPosition % patternSize);
                }
                
                if (scale) {
                    offset = pizGrowingArrayValueAtIndex (x->scale, note1->data[PIZ_PITCH] % scale);
                }
                            
                newKey = (newPosition * (PIZ_MIDI_PITCH + 1)) + note1->data[PIZ_PITCH] + offset;
                
                if (!pizBoundedHashTableContainsKey (x->hashTable, newKey)) {
                    long values[PIZ_SEQUENCE_NOTE_SIZE];
                    
                    newPosition *= x->grid;
                    
                    values[PIZ_SEQUENCE_POSITION]       = newPosition;
                    values[PIZ_SEQUENCE_PITCH]          = note1->data[PIZ_PITCH] + offset;
                    values[PIZ_SEQUENCE_VELOCITY]       = note1->data[PIZ_VELOCITY];
                    values[PIZ_SEQUENCE_DURATION]       = note1->data[PIZ_DURATION];
                    values[PIZ_SEQUENCE_CHANNEL]        = note1->data[PIZ_CHANNEL];
                    values[PIZ_SEQUENCE_IS_SELECTED]    = false;
                    values[PIZ_SEQUENCE_IS_MARKED]      = false;

                    note2 = pizSequenceAddNote (x, values, PIZ_SEQUENCE_ADD_FLAG_CLIP);
                    
                    if (note2) {
                        pizBoundedHashTableAdd (x->hashTable, newKey, (void *)note2);
                        haveChanged = true;
                        k ++;
                    }
                } else if (!(pizBoundedHashTablePtrByKey (x->hashTable, newKey, (void **)&note2)) && 
                    (note2 != note1)) {
                    
                    if (note2 == x->markedNote) {
                        x->markedNote = NULL;
                    }
                    
                    pizBoundedHashTableRemoveByKeyAndPtr (x->hashTable, newKey, (void *)note2);
                    
                    if (!pizLinklistRemoveByPtr (x->timeline[note2->position], (void *)note2)) {
                        x->count --;
                        haveChanged = true;
                        k ++;
                    }
                }

                loop ++;
            }
        }
        
        if (haveChanged) {
            PIZMAPDIRTY
            pizSequenceCleanMap (x);
        }
    }
                    
    PIZUNLOCK
    
    return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceRotate (PIZSequence *x, PIZSelector selector, long shift)
{
    long i, k = 0;
    bool haveChanged;
    
    PIZLOCK
    
    haveChanged = (x->count > 1);
    
    pizSequenceCleanMap (x);
    
    k = pizSequencePickUpNotes (x);
    
    if (k && shift < 0) {
            shift = k - ((-shift) % k);
        }

    for (i = 0; i < k; i++) {
            x->values1[i] = x->notes1[(i + shift) % k]->data[selector];
        }
                
    pizSequenceFillValues (x, selector, k, 0);
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceScramble (PIZSequence *x, PIZSelector selector)
{
    long i, k = 0;
    bool haveChanged;
    
    PIZLOCK
    
    haveChanged = (x->count > 1);
    
    k = pizSequencePickUpNotes (x);
    
    for (i = 0; i < k; i++) {
            x->values1[i] = x->notes1[i]->data[selector];
        }
        
    for (i = (k - 1); i > 0; i--) {
            long    h = (i + 1) * (rand ( ) / (RAND_MAX + 1.0));
            PIZNote *temp = NULL;
            
            temp = x->notes1[h];
            x->notes1[h] = x->notes1[i];
            x->notes1[i] = temp;
        }
            
    pizSequenceFillValues (x, selector, k, 0);
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceSort (PIZSequence *x, PIZSelector selector, long down)
{
    long i, scale;
    long k = 0;
    bool haveChanged;
    
    PIZLOCK
    
    haveChanged = (x->count > 1);
    
    pizSequenceCleanMap (x);
    
    k = pizSequencePickUpNotes (x);
    scale = pizGrowingArrayCount (x->scale);
    
    for (i = 0; i < PIZ_POOL_SIZE; i++) {
        x->values1[i] = 0;
    }
    
    if (selector == PIZ_PITCH) {
        for (i = 0; i < k; i++) { 
            long pitch = x->notes1[i]->data[PIZ_PITCH];
                            
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->values1[CLAMP (pitch, 0, PIZ_MIDI_PITCH)] ++; 
        }
    } else {
        for (i = 0; i < k; i++) { 
            x->values1[x->notes1[i]->data[selector]] ++; 
        }   
    }
        
    for (i = 1; i < PIZ_POOL_SIZE; i++) {
        x->values1[i] += x->values1[i - 1];
    }
                
    if (selector == PIZ_PITCH) {
        for (i = (k - 1); i >= 0; i--) {
            long pitch = x->notes1[i]->data[PIZ_PITCH];
                                
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->notes2[x->values1[CLAMP (pitch, 0, PIZ_MIDI_PITCH)] - 1] = x->notes1[i];
            x->values1[CLAMP (pitch, 0, PIZ_MIDI_PITCH)] --;
        }
    } else {
        for (i = (k - 1); i >= 0; i--) {    
            x->notes2[x->values1[x->notes1[i]->data[selector]] - 1] = x->notes1[i];
            x->values1[x->notes1[i]->data[selector]] --;
        }
    }
    
    for (i = 0; i < k; i++) {
        x->values1[i] = x->notes2[i]->data[selector];
    }
                
    pizSequenceFillValues (x, selector, k, down);
    
    PIZUNLOCK
    
    return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceChange (PIZSequence *x, PIZSelector selector, long value)
{
    long i;
    bool haveChanged = false;
    
    PIZLOCK
        
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long max1, max2;
            long h = 100 * (rand ( ) / (RAND_MAX + 1.0));
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (h < x->chance) {
                switch (selector) {
                    case PIZ_PITCH    : note->data[PIZ_PITCH] = CLAMP (note->data[PIZ_PITCH] + value, 
                                        0, PIZ_MIDI_PITCH); break;
                    case PIZ_VELOCITY : note->data[PIZ_VELOCITY] = CLAMP (note->data[PIZ_VELOCITY] + value, 
                                        0, PIZ_MIDI_VELOCITY); break;
                    case PIZ_DURATION : max1 = PIZ_SEQUENCE_TIMELINE_SIZE - note->position;
                                        max2 = MIN (max1, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                        note->data[PIZ_DURATION] = CLAMP (note->data[PIZ_DURATION] + value, 
                                        1, max2); break;
                    case PIZ_CHANNEL  : note->data[PIZ_CHANNEL] = CLAMP (note->data[PIZ_CHANNEL] + value, 
                                        0, PIZ_MIDI_CHANNEL); break;
                }
                
                haveChanged = true;
            }
            
            note = nextNote;
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceSet (PIZSequence *x, PIZSelector selector, long value)
{
    long i;
    bool haveChanged = false;
        
    PIZLOCK
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long max1, max2;
            long h = 100 * (rand ( ) / (RAND_MAX + 1.0));
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (h < x->chance) {
                switch (selector) {
                    case PIZ_PITCH    : note->data[PIZ_PITCH] = CLAMP (value, 0, PIZ_MIDI_PITCH); break;
                    case PIZ_VELOCITY : note->data[PIZ_VELOCITY] = CLAMP (value, 0, PIZ_MIDI_VELOCITY); break;
                    case PIZ_DURATION : max1 = PIZ_SEQUENCE_TIMELINE_SIZE - note->position;
                                        max2 = MIN (max1, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                        note->data[PIZ_DURATION] = CLAMP (value, 1, max2); break;
                    case PIZ_CHANNEL  : note->data[PIZ_CHANNEL] = CLAMP (value, 0, PIZ_MIDI_CHANNEL); break;
                }
                
                haveChanged = true;
            }
            
            note = nextNote;
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceRandom (PIZSequence *x, PIZSelector selector, long minValue, long maxValue)
{
    long i, range;
    bool haveChanged = false;
    
    PIZLOCK
    
    if (minValue > maxValue) {
        long temp = minValue;
        minValue = maxValue;
        maxValue = temp;
    }
    
    range = (maxValue - minValue) + 1;
        
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long max1, max2;
            long h = 100 * (rand ( ) / (RAND_MAX + 1.0));
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (h < x->chance) {
                long value = minValue + (long)(range * (rand ( ) / (RAND_MAX + 1.0)));
                
                switch (selector) {
                    case PIZ_PITCH    : note->data[PIZ_PITCH] = CLAMP (note->data[PIZ_PITCH] + value, 
                                        0, PIZ_MIDI_PITCH); break;
                    case PIZ_VELOCITY : note->data[PIZ_VELOCITY] = CLAMP (note->data[PIZ_VELOCITY] + value, 
                                        0, PIZ_MIDI_VELOCITY); break;
                    case PIZ_DURATION : max1 = PIZ_SEQUENCE_TIMELINE_SIZE - note->position;
                                        max2 = MIN (max1, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                        note->data[PIZ_DURATION] = CLAMP (note->data[PIZ_DURATION] + value, 
                                        1, max2); break;
                    case PIZ_CHANNEL  : note->data[PIZ_CHANNEL] = CLAMP (note->data[PIZ_CHANNEL] + value, 
                                        0, PIZ_MIDI_CHANNEL); break;
                }
                
                haveChanged = true;
            }
            
            note = nextNote;
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceKillNotes (PIZSequence *x)
{
    long i;
    bool haveChanged = false;
    
    PIZLOCK
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (100 * (rand ( ) / (RAND_MAX + 1.0)) < x->chance) {
                if (note == x->markedNote) {
                    x->markedNote = NULL;
                }
                    
                if (!pizLinklistRemoveByPtr (x->timeline[p], (void *)note)) {
                    x->count --;
                    haveChanged = true;
                }
            }
                
            note = nextNote;
        }
    }
    
    if (haveChanged) {
        PIZMAPDIRTY
        pizSequenceCleanMap (x);
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceCycle (PIZSequence *x, PIZScaleKey key, const PIZGrowingArray *a)
{
    bool haveChanged = false;

    PIZLOCK
    
    if (a) {
        long count = pizGrowingArrayCount (a);
                    
        if (count > 1) {
            long i;
            long t[PIZ_SCALE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            long s[PIZ_SCALE_SIZE];
            long m, n, o, temp;
            long scale = pizGrowingArrayCount (x->scale);
            
            temp = pizGrowingArrayValueAtIndex (a, 0);
            m = CLAMP (temp, 0, PIZ_SCALE_SIZE - 1);
            o = m;
            
            for (i = 1; i < count; i++) {
                temp = pizGrowingArrayValueAtIndex (a, i);
                n = CLAMP (temp, 0, PIZ_SCALE_SIZE - 1);
                t[m] = n - m;
                m = n;
            }
            
            t[m] = o - m;
            
            for (i = 0; i < PIZ_SCALE_SIZE; i++) {
                s[i] = t[(PIZ_SCALE_SIZE - key + i) % PIZ_SCALE_SIZE];
            }
                
            for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
                PIZNote *note       = NULL;
                PIZNote *nextNote   = NULL;
                
                long p = pizGrowingArrayValueAtIndex (x->map, i);
                
                pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
                
                while (note) {
                    long h = 100 * (rand ( ) / (RAND_MAX + 1.0));
                    
                    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                    
                    if (h < x->chance) {
                        long pitch, tempPitch;
                        long offset = 0;
                        
                        pitch = note->data[PIZ_PITCH];
                        
                        if (scale) {
                            offset += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
                        }
                        
                        pitch += offset;
                        
                        tempPitch = CLAMP (pitch, 0, PIZ_MIDI_PITCH);
                        pitch = tempPitch;
                        
                        pitch += s[pitch % PIZ_SCALE_SIZE];
                
                        if (pitch != (note->data[PIZ_PITCH] + offset)) {
                            note->data[PIZ_PITCH] = CLAMP (pitch, 0, PIZ_MIDI_PITCH);
                            haveChanged = true;
                        }
                    }

                    note = nextNote;
                }
            }
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
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

void pizSequenceFillValues (PIZSequence *x, PIZSelector selector, long k, bool reverse)
{
    long i;
    
    if (selector == PIZ_DURATION) {
        if (!reverse) {
            for (i = 0; i < k; i++) {
                x->notes1[i]->data[PIZ_DURATION] = MIN (x->values1[i], PIZ_SEQUENCE_TIMELINE_SIZE 
                    - x->notes1[i]->position);
            }
        } else {
            for (i = 0; i < k; i++) {
                x->notes1[i]->data[PIZ_DURATION] = MIN (x->values1[(k - 1) - i], PIZ_SEQUENCE_TIMELINE_SIZE 
                    - x->notes1[i]->position);
            }
        }
    } else {
        if (!reverse) {
            for (i = 0; i < k; i++) {
                x->notes1[i]->data[selector] = x->values1[i];
            }
        } else {
            for (i = 0; i < k; i++) {
                x->notes1[i]->data[selector] = x->values1[(k - 1) - i];
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
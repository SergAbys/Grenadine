/*
 * \file    pizSequenceTransform.c
 * \author  Jean Sapristi
 * \date    April 3, 2012.
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

#define PIZ_H_SIZE                      6
#define PIZ_NEIGHBORS_BIRTH_SIZE        12
#define PIZ_NEIGHBORS_DEATH_SIZE        16
#define PIZ_DIVISIONS_SIZE              6
#define PIZ_MAXIMUM_OFFSET              6
#define PIZ_BREAK_LOOP                  20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CEIL(a,b) (((a)%(b))==0?(a)/(b):(a)/(b)+1)
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long piz_neighbors[ ]          = {   -256, 
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
                                               2    };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long     piz_divisions[ ]      = { 2, 3, 4, 5, 7, 11 };
static double   piz_distribution2[ ]  = { 0.75, 1. };
static double   piz_distribution3[ ]  = { 0.68, 0.85, 1. };
static double   piz_distribution4[ ]  = { 0.63, 0.75, 0.87, 1. };
static double   piz_distribution5[ ]  = { 0.60, 0.70, 0.80, 0.90, 1. };
static double   piz_distribution7[ ]  = { 0.56, 0.63, 0.70, 0.77, 0.84, 0.91, 1. };
static double   piz_distribution11[ ] = { 0.54, 0.59, 0.63, 0.68, 0.72, 0.77, 0.81, 0.86, 0.9, 0.95, 1. };

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
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long temp, p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            temp = CLAMP (note->data[PIZ_NOTE_PITCH] + n, 0, PIZ_MAGIC_PITCH);
            if (note->data[PIZ_NOTE_PITCH] != temp) {
                note->data[PIZ_NOTE_PITCH] = temp;
                pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            }
            
            note = nextNote;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceClean (PIZSequence *x, long value)
{
    long i, scale, v;
    long index = 0;
    bool haveChanged = false;
        
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
            
            pitch = note->data[PIZ_NOTE_PITCH];
                    
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
    
    return haveChanged;
}

bool pizSequenceAlgorithm (PIZSequence *x, PIZAlgorithm flag, void *algorithm)
{
    long     k;
    bool     haveChanged = false;
    PIZError err = PIZ_ERROR;
        
    k = pizSequencePickUpNotes (x);

    if (flag & PIZ_ALGORITHM_FACTOR_ORACLE) {
        if (pizFactorOracleCount ((PIZFactorOracle *)algorithm)) {
            err = pizFactorOracleProceed ((PIZFactorOracle *)algorithm, k, x->values1);
        }
    } else if (flag & PIZ_ALGORITHM_GALOIS_LATTICE) {
        if (pizGaloisLatticeCount ((PIZGaloisLattice *)algorithm)) {
            err = pizGaloisLatticeProceed ((PIZGaloisLattice *)algorithm, k, x->values1);
        }
    } else if (flag & PIZ_ALGORITHM_FINITE_STATE) {
        if (pizFiniteStateCount ((PIZFiniteState *)algorithm)) {
            err = pizFiniteStateProceed ((PIZFiniteState *)algorithm, k, x->values1);
        }
    }
    
    if (!err) {
        long i, h;  
        
        for (i = 0; i < k; i++) {
            x->values1[i] = pizSequenceMovePitchToAmbitus (x, x->values1[i]);
        }
        
        for (i = 0; i < k; i++) {        
            h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            if (h >= x->chance) {
                x->values1[i] = x->notes1[i]->data[PIZ_NOTE_PITCH];
            } 
        }
        
        haveChanged = pizSequenceFillValues (x, PIZ_NOTE_PITCH, k, 0);
    }
    
    return haveChanged;
}

bool pizSequenceNovember (PIZSequence *x, long iterate)
{
    bool haveChanged = false;
                                                    
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long i, start, end, mapCount, scale;
    long k = 0;
    long loop = 0;
    
    pizBoundedHashTableClear (x->hashTable);

    start       = PIZ_CEIL (x->start, x->cell);
    end         = PIZ_CEIL (x->end, x->cell);
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
                offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_NOTE_PITCH] % scale);
            }
            
            key = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1));
            key += note->data[PIZ_NOTE_PITCH] + offset;
            
            pizBoundedHashTableAdd (x->hashTable, key, (void *)note);
            
            note = nextNote;
        }
    }

    while (k < iterate && loop < PIZ_BREAK_LOOP) {
    //
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
            p = pizGrowingArrayValueAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
            if (pizLinklistCount (x->timeline[p])) {
                q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
            } 
        }                           

        pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note);
        
        if (scale) {
            offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_NOTE_PITCH] % scale);
        }
        
        pitch = note->data[PIZ_NOTE_PITCH] + offset;
        
        hCenter = ((long)(note->position / (double)x->cell)) * (PIZ_MAGIC_PITCH + 1);
        hCenter += note->data[PIZ_NOTE_PITCH] + offset;
        
        err |= (note->position < x->start);
        err |= (note->position >= x->end);
        err |= (pitch < x->down);
        err |= (pitch > x->up);
        
        if (!err) {
            for (j = 0; j < PIZ_NEIGHBORS_DEATH_SIZE; j++) {
                long key = hCenter + piz_neighbors[j];
                if (pizBoundedHashTableContainsKey (x->hashTable, key)) {
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
            previous += pizGrowingArrayValueAtIndex (x->pattern, previous % patternSize);
            next     += pizGrowingArrayValueAtIndex (x->pattern, next % patternSize);
        }

        if (previous != here) {
            hPat[1] = (previous * (PIZ_MAGIC_PITCH + 1)) + pitch;
            
            if (scale) {
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[0] = hPat[1] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
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
            hPat[4] = (next * (PIZ_MAGIC_PITCH + 1)) + pitch;
            
            if (scale) {
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizGrowingArrayValueAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[3] = hPat[4] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n;
                    long t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
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
            long h          = (j + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
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
        
                for (t = 0; t < PIZ_NEIGHBORS_BIRTH_SIZE; t++)  {
                    long key = hPat[j] + piz_neighbors[t];
                    if (!pizBoundedHashTablePtrByKey (x->hashTable, key, (void **)&ptr)) {
                        neighbors ++;
                        noteToCopy = ptr;
                    }
                }
                
                if (neighbors == 1) {
                long    values[PIZ_DATA_NOTE_SIZE];
                PIZNote *newNote = NULL;
                
                values[PIZ_DATA_POSITION]    = ((long)(hPat[j] / (double)(PIZ_MAGIC_PITCH + 1))) * x->cell;
                values[PIZ_DATA_PITCH]       = hPat[j] % (PIZ_MAGIC_PITCH + 1);
                values[PIZ_DATA_VELOCITY]    = noteToCopy->data[PIZ_NOTE_VELOCITY];
                values[PIZ_DATA_DURATION]    = noteToCopy->data[PIZ_NOTE_DURATION];
                values[PIZ_DATA_CHANNEL]     = noteToCopy->data[PIZ_NOTE_CHANNEL];
                values[PIZ_DATA_IS_SELECTED] = false;
                values[PIZ_DATA_IS_MARKED]   = false;
                
                newNote = pizSequenceNewNote (x, values, PIZ_SEQUENCE_ADD_FLAG_CLIP);
                
                if (newNote) {
                    pizBoundedHashTableAdd (x->hashTable, hPat[j], (void *)newNote);
                    haveChanged = true;
                    k ++;
                }
                
                break;
                //
                }
            }
        }
            
        if (death) {
            pizBoundedHashTableRemoveByKeyAndPtr (x->hashTable, hCenter, (void *)note);
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
            
    return haveChanged;
}

bool pizSequenceJuliet (PIZSequence *x, long iterate, long division)
{
    bool haveChanged = false;
    
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long i, start, end, size;
    long b = 0;
    
    start   = PIZ_CEIL (x->start, x->cell);
    end     = PIZ_CEIL (x->end, x->cell); 
    
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
                offset = pizGrowingArrayValueAtIndex (x->scale, note->data[PIZ_NOTE_PITCH] % scale);
            }
            
            key = (long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1);
            key += note->data[PIZ_NOTE_PITCH] + offset;
            
            pizBoundedHashTableAdd (x->hashTable, key, (void *)note);
            
            note = nextNote;
        }
    }
    
    while (x->count && k < iterate && loop < PIZ_BREAK_LOOP) {
    //
    long    j, patternSize, step, newKey, newPosition;
    long    q = -1;
    long    p = -1;
    long    offset = 0;
    double  h = rand_r (&x->seed) / (RAND_MAX + 1.0);
    double  *distribution = NULL;
    PIZNote *note1 = NULL;
    PIZNote *note2 = NULL;
    
    mapCount = pizGrowingArrayCount (x->map);

    while (q == -1) {
        p = pizGrowingArrayValueAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
        if (pizLinklistCount (x->timeline[p])) {
            q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        } 
    }                           

    pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note1);     
    
    step = MIN (a, PIZ_MAXIMUM_OFFSET) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
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
    
    if (patternSize = pizGrowingArrayCount (x->pattern)) {
        newPosition += pizGrowingArrayValueAtIndex (x->pattern, newPosition % patternSize);
    }
    
    if (scale) {
        offset = pizGrowingArrayValueAtIndex (x->scale, note1->data[PIZ_NOTE_PITCH] % scale);
    }
                
    newKey = (newPosition * (PIZ_MAGIC_PITCH + 1)) + note1->data[PIZ_NOTE_PITCH] + offset;
    
    if (!pizBoundedHashTableContainsKey (x->hashTable, newKey)) {
        long values[PIZ_DATA_NOTE_SIZE];
        
        values[PIZ_DATA_POSITION]       = newPosition * x->cell;
        values[PIZ_DATA_PITCH]          = note1->data[PIZ_NOTE_PITCH] + offset;
        values[PIZ_DATA_VELOCITY]       = note1->data[PIZ_NOTE_VELOCITY];
        values[PIZ_DATA_DURATION]       = note1->data[PIZ_NOTE_DURATION];
        values[PIZ_DATA_CHANNEL]        = note1->data[PIZ_NOTE_CHANNEL];
        values[PIZ_DATA_IS_SELECTED]    = false;
        values[PIZ_DATA_IS_MARKED]      = false;

        note2 = pizSequenceNewNote (x, values, PIZ_SEQUENCE_ADD_FLAG_CLIP);
        
        if (note2) {
            pizBoundedHashTableAdd (x->hashTable, newKey, (void *)note2);
            haveChanged = true;
            k ++;
        }
    } else if (!(pizBoundedHashTablePtrByKey (x->hashTable, newKey, (void **)&note2))) {
        if (note2 != note1) {
            pizBoundedHashTableRemoveByKeyAndPtr (x->hashTable, newKey, (void *)note2);
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
                        
    return haveChanged;
}

bool pizSequenceRotate (PIZSequence *x, PIZNoteSelector selector, long shift)
{
    long i, k = 0;
    bool haveChanged;
        
    k = pizSequencePickUpNotes (x);
    
    if (k && shift < 0) {
        shift = k - ((-shift) % k);
    }

    for (i = 0; i < k; i++) {
        x->values1[i] = x->notes1[(i + shift) % k]->data[selector];
    }
                
    haveChanged = pizSequenceFillValues (x, selector, k, 0);
        
    return haveChanged;
}

bool pizSequenceScramble (PIZSequence *x, PIZNoteSelector selector)
{
    long i, k = 0;
    bool haveChanged;
        
    k = pizSequencePickUpNotes (x);
    
    for (i = 0; i < k; i++) {
        x->values1[i] = x->notes1[i]->data[selector];
    }
        
    for (i = (k - 1); i > 0; i--) {
        long    h = (i + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        PIZNote *temp = NULL;
            
        temp = x->notes1[h];
        x->notes1[h] = x->notes1[i];
        x->notes1[i] = temp;
    }
            
    haveChanged = pizSequenceFillValues (x, selector, k, 0);
        
    return haveChanged;
}

bool pizSequenceSort (PIZSequence *x, PIZNoteSelector selector, bool down)
{
    long i, scale;
    long k = 0;
    bool haveChanged;
        
    k = pizSequencePickUpNotes (x);
    scale = pizGrowingArrayCount (x->scale);
    
    for (i = 0; i < PIZ_SEQUENCE_TEMP_ARRAY_SIZE; i++) {
        x->values1[i] = 0;
    }
    
    if (selector == PIZ_NOTE_PITCH) {
        for (i = 0; i < k; i++) { 
            long pitch = x->notes1[i]->data[PIZ_NOTE_PITCH];
                            
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->values1[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] ++; 
        }
    } else {
        for (i = 0; i < k; i++) { 
            x->values1[x->notes1[i]->data[selector]] ++; 
        }   
    }
        
    for (i = 1; i < PIZ_SEQUENCE_TEMP_ARRAY_SIZE; i++) {
        x->values1[i] += x->values1[i - 1];
    }
                
    if (selector == PIZ_NOTE_PITCH) {
        for (i = (k - 1); i >= 0; i--) {
            long pitch = x->notes1[i]->data[PIZ_NOTE_PITCH];
                                
            if (scale) {
                pitch += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
            }
                
            x->notes2[x->values1[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] - 1] = x->notes1[i];
            x->values1[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] --;
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
                
    haveChanged = pizSequenceFillValues (x, selector, k, down);
        
    return haveChanged;
}

bool pizSequenceChange (PIZSequence *x, PIZNoteSelector selector, long value)
{
    long i;
    bool haveChanged = false;
            
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {  
    // 
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizGrowingArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long max, temp;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
    //
    switch (selector) {
        case PIZ_NOTE_PITCH    : temp = CLAMP (note->data[PIZ_NOTE_PITCH] + value, 0, PIZ_MAGIC_PITCH); 
                                 break;
        case PIZ_NOTE_VELOCITY : temp = CLAMP (note->data[PIZ_NOTE_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); 
                                 break;
        case PIZ_NOTE_DURATION : max = x->timelineSize - note->position;
                                 max = MIN (max, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                 temp = CLAMP (note->data[PIZ_NOTE_DURATION] + value, 1, max); 
                                 break;
        case PIZ_NOTE_CHANNEL  : temp = CLAMP (note->data[PIZ_NOTE_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL);
                                 break;
    }
    
    if (note->data[selector] != temp) {
        note->data[selector] = temp;
        pizItemset128SetAtIndex (&x->changedNotes, note->tag);
        haveChanged = true;
    }
    //
    }
    
    note = nextNote;
    //
    }
    //
    }
        
    return haveChanged;
}

bool pizSequenceSet (PIZSequence *x, PIZNoteSelector selector, long value)
{
    long i;
    bool haveChanged = false;
            
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long max, temp;
            long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (h < x->chance) {
                switch (selector) {
                    case PIZ_NOTE_PITCH    : temp = CLAMP (value, 0, PIZ_MAGIC_PITCH); break;
                    case PIZ_NOTE_VELOCITY : temp = CLAMP (value, 0, PIZ_MAGIC_VELOCITY); break;
                    case PIZ_NOTE_DURATION : max = x->timelineSize - note->position;
                                             max = MIN (max, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                             temp = CLAMP (value, 1, max); break;
                    case PIZ_NOTE_CHANNEL  : temp = CLAMP (value, 0, PIZ_MAGIC_CHANNEL); break;
                }
                
                if (note->data[selector] != temp) {
                    note->data[selector] = temp;
                    pizItemset128SetAtIndex (&x->changedNotes, note->tag);
                    haveChanged = true;
                }
            }
            
            note = nextNote;
        }
    }
        
    return haveChanged;
}

bool pizSequenceRandom (PIZSequence *x, PIZNoteSelector selector, long minValue, long maxValue)
{
    long i, range;
    bool haveChanged = false;
        
    if (minValue > maxValue) {
        long temp = minValue;
        minValue = maxValue;
        maxValue = temp;
    }
    
    range = (maxValue - minValue) + 1;
        
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {
    //   
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizGrowingArrayValueAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long max, temp;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
        long value = minValue + (long)(range * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        
        switch (selector) {
        case PIZ_NOTE_PITCH    : temp = CLAMP (note->data[PIZ_NOTE_PITCH] + value, 0, PIZ_MAGIC_PITCH); 
                                 break;
        case PIZ_NOTE_VELOCITY : temp = CLAMP (note->data[PIZ_NOTE_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); 
                                 break;
        case PIZ_NOTE_DURATION : max = x->timelineSize - note->position;
                                 max = MIN (max, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                 temp = CLAMP (note->data[PIZ_NOTE_DURATION] + value, 1, max); 
                                 break;
        case PIZ_NOTE_CHANNEL  : temp = CLAMP (note->data[PIZ_NOTE_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL); 
                                 break;
        }
        
        if (note->data[selector] != temp) {
            note->data[selector] = temp;
            pizItemset128SetAtIndex (&x->changedNotes, note->tag);
            haveChanged = true;
        }
    }
    
    note = nextNote;
    //
    }
    //
    }
        
    return haveChanged;
}

bool pizSequenceKill (PIZSequence *x)
{
    long i;
    bool haveChanged = false;
        
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
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
        
    return haveChanged;
}

bool pizSequenceCycle (PIZSequence *x, PIZScaleKey key, const PIZGrowingArray *a)
{
    bool haveChanged = false;
    
    if (a) {
    //
    long count = pizGrowingArrayCount (a);
                
    if (count > 1) {
    //
    long i;
    long t[PIZ_MAGIC_SCALE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    long s[PIZ_MAGIC_SCALE];
    long m, n, o, temp;
    long scale = pizGrowingArrayCount (x->scale);
    
    temp = pizGrowingArrayValueAtIndex (a, 0);
    m = CLAMP (temp, 0, PIZ_MAGIC_SCALE - 1);
    o = m;
    
    for (i = 1; i < count; i++) {
        temp = pizGrowingArrayValueAtIndex (a, i);
        n = CLAMP (temp, 0, PIZ_MAGIC_SCALE - 1);
        t[m] = n - m;
        m = n;
    }
    
    t[m] = o - m;
    
    for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
        s[i] = t[(PIZ_MAGIC_SCALE - key + i) % PIZ_MAGIC_SCALE];
    }
        
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (h < x->chance) {
                long pitch, offset = 0;
                
                pitch = note->data[PIZ_NOTE_PITCH];
                
                if (scale) {
                    offset += pizGrowingArrayValueAtIndex (x->scale, pitch % scale);
                }
                
                pitch = CLAMP (pitch + offset, 0, PIZ_MAGIC_PITCH);
                pitch += s[pitch % PIZ_MAGIC_SCALE];
        
                if (pitch != (note->data[PIZ_NOTE_PITCH] + offset)) {
                    note->data[PIZ_NOTE_PITCH] = CLAMP (pitch, 0, PIZ_MAGIC_PITCH);
                    pizItemset128SetAtIndex (&x->changedNotes, note->tag);
                    haveChanged = true;
                }
            }

            note = nextNote;
        }
    }
    //    
    }
    //    
    }
        
    return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
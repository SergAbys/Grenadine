/*
 * \file    pizSequenceMethods.c
 * \author  Jean Sapristi
 * \date    May 17, 2012.
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

#include "pizSequenceLibrary.h"
#include "pizSequenceMethods.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <string.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_NOVEMBER_H_SIZE             6
#define PIZ_JULIET_MAXIMUM_OFFSET       6
#define PIZ_MAXIMUM_LOOP                20
#define PIZ_NEIGHBORS_BIRTH_SIZE        12
#define PIZ_NEIGHBORS_DEATH_SIZE        16
#define PIZ_DIVISIONS_SIZE              5

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LONG    pizSequenceNeighbors[ ]     = { -256, 
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

PIZ_LONG    pizSequenceDivisions[ ]     = { 2, 3, 4, 5, 7 };
PIZ_DOUBLE  pizSequenceDistribution2[ ] = { 0.75, 1. };
PIZ_DOUBLE  pizSequenceDistribution3[ ] = { 0.68, 0.85, 1. };
PIZ_DOUBLE  pizSequenceDistribution4[ ] = { 0.63, 0.75, 0.87, 1. };
PIZ_DOUBLE  pizSequenceDistribution5[ ] = { 0.60, 0.70, 0.80, 0.90, 1. };
PIZ_DOUBLE  pizSequenceDistribution7[ ] = { 0.56, 0.63, 0.70, 0.77, 0.84, 0.91, 1. };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CEIL(a,b)                   (((a)%(b))==0?(a)/(b):(a)/(b)+1)

#define PIZ_PICKUP_NOTES                x->tempIndex = 0;                                           \
                                        pizSequenceForEach (x, pizSequenceFillTempNotes, NULL);     \
                                        k = x->tempIndex;
                        
#define PIZ_FILL_NOTES                  pizSequenceFillNotes (x, selector, 0);
#define PIZ_FILL_NOTES_REVERSE          pizSequenceFillNotes (x, selector, 1);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL void pizSequenceFillNotes (PIZSequence *x, long selector, bool reverse);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceNote (PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventPtr (event, &argc, &argv))) {
        long  i;
        ulong flags = PIZ_SEQUENCE_FLAG_SNAP | PIZ_SEQUENCE_FLAG_AMBITUS;
        long  values[ ] = { -1,
                             0, 
                             PIZ_SEQUENCE_DEFAULT_VELOCITY, 
                             x->noteValue, 
                             0 };
        
        for (i = 0; i < MIN (argc, 5); i++) {
            values[i] = argv[i];
        }
        
        if (pizSequenceNewNote (x, event->tag, values, flags)) {
            pizSequenceMakeMap (x);
        }
    } 
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceClear (PIZSequence *x, const PIZEvent *event)
{
    if (x->count) {
        pizSequenceForEach (x, pizSequenceRemoveNote, NULL);
        pizArrayClear (x->map);  
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceTranspose (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        x->down = CLAMP (x->down + value, 0, PIZ_MAGIC_PITCH);
        x->up   = CLAMP (x->up + value, 0, PIZ_MAGIC_PITCH);
        
        x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
        
        pizSequenceForEach (x, pizSequenceChangeNote, event);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceClean (PIZSequence *x, const PIZEvent *event)
{
    long i;
    
    x->tempIndex = 0;
    
    for (i = 0; i < (PIZ_MAGIC_PITCH + 1); i++) {
        x->tempValues[i] = 0;
    }
    
    pizSequenceForEach (x, pizSequenceNearby, event);
    
    if (x->tempIndex) {
        for (i = 0; i < x->tempIndex; i++) {
            pizSequenceRemoveNote (x, x->tempNotes1[i], NULL);
        }
        pizSequenceMakeMap (x);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceRotate (PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
        
    if (!(pizEventPtr (event, &argc, &argv))) {
    //
    long i, k;
    long shift    = argv[0];
    long selector = CLAMP (argv[1], PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
    
    PIZ_PICKUP_NOTES
    
    if (k && (shift < 0)) {
        shift = k - ((-shift) % k);
    }

    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes1[(i + shift) % k]->values[selector];
    }
                
    PIZ_FILL_NOTES
    //
    }
    
    return PIZ_GOOD;
}

//PIZError pizSequenceScramble (PIZSequence *x, PIZMidiSelector selector)
PIZError pizSequenceScramble (PIZSequence *x, const PIZEvent *event)
{/*
    long i, k;
        
    PIZ_PICKUP_NOTES
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes1[i]->values[selector];
    }
        
    for (i = (k - 1); i > 0; i--) {
        long    h = (i + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        PIZNote *temp = NULL;
            
        temp = x->tempNotes1[h];
        x->tempNotes1[h] = x->tempNotes1[i];
        x->tempNotes1[i] = temp;
    }
            
    PIZ_FILL_NOTES*/
    
    return PIZ_GOOD;
}

//PIZError pizSequenceSort (PIZSequence *x, PIZMidiSelector selector, bool down)
PIZError pizSequenceSort (PIZSequence *x, const PIZEvent *event)
{/*
    long i, scale, k;
        
    PIZ_PICKUP_NOTES
    
    scale = pizArrayCount (x->scale);
    
    for (i = 0; i < PIZ_SEQUENCE_INIT_TEMP_SIZE; i++) {
        x->tempValues[i] = 0;
    }
    
    if (selector == PIZ_VALUE_PITCH) {
        for (i = 0; i < k; i++) { 
            long pitch = x->tempNotes1[i]->values[PIZ_VALUE_PITCH];
                            
            if (scale) {
                pitch += pizArrayAtIndex (x->scale, pitch % scale);
            }
                
            x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] ++; 
        }
    } else {
        for (i = 0; i < k; i++) { 
            x->tempValues[x->tempNotes1[i]->values[selector]] ++; 
        }   
    }
        
    for (i = 1; i < PIZ_SEQUENCE_INIT_TEMP_SIZE; i++) {
        x->tempValues[i] += x->tempValues[i - 1];
    }
                
    if (selector == PIZ_VALUE_PITCH) {
        for (i = (k - 1); i >= 0; i--) {
            long pitch = x->tempNotes1[i]->values[PIZ_VALUE_PITCH];
                                
            if (scale) {
                pitch += pizArrayAtIndex (x->scale, pitch % scale);
            }
                
            x->tempNotes2[x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] - 1] = x->tempNotes1[i];
            x->tempValues[CLAMP (pitch, 0, PIZ_MAGIC_PITCH)] --;
        }
    } else {
        for (i = (k - 1); i >= 0; i--) {    
            x->tempNotes2[x->tempValues[x->tempNotes1[i]->values[selector]] - 1] = x->tempNotes1[i];
            x->tempValues[x->tempNotes1[i]->values[selector]] --;
        }
    }
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes2[i]->values[selector];
    }
     
    if (!down) {
        PIZ_FILL_NOTES
    } else {
        PIZ_FILL_NOTES_REVERSE
    }*/
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

//PIZError pizSequenceChange (PIZSequence *x, PIZMidiSelector selector, long value)
PIZError pizSequenceChange (PIZSequence *x, const PIZEvent *event)
{/*
    long i;
            
    for (i = 0; i < pizArrayCount (x->map); i++) {  
    // 
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayAtIndex (x->map, i);
    
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
    case PIZ_VALUE_PITCH    : t = CLAMP (note->values[PIZ_VALUE_PITCH] + value, 0, PIZ_MAGIC_PITCH); break;
    case PIZ_VALUE_VELOCITY : t = CLAMP (note->values[PIZ_VALUE_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); 
                                break;
    case PIZ_VALUE_DURATION : m = MIN   (x->size - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                             t = CLAMP (note->values[PIZ_VALUE_DURATION] + value, 1, m); break;
    case PIZ_VALUE_CHANNEL  : t = CLAMP (note->values[PIZ_VALUE_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL); break;
    //                            
    }
    
    if (note->values[selector] != t) {
        note->values[selector] = t;
        pizItemsetSetAtIndex (&x->changedNotes, note->tag);
    }
    //
    }
    
    note = nextNote;
    //
    }
    //
    }*/
    
    return PIZ_GOOD;
}

//PIZError pizSequenceFill (PIZSequence *x, PIZMidiSelector selector, long value)
PIZError pizSequenceFill (PIZSequence *x, const PIZEvent *event)
{/*
    long i;
            
    for (i = 0; i < pizArrayCount (x->map); i++) {
    //   
    PIZNote *note       = NULL;
    PIZNote *nextNote   = NULL;
    
    long p = pizArrayAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
    //
    long m, t;
    long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    
    pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
    
    if (h < x->chance) {
        switch (selector) {
        case PIZ_VALUE_PITCH    : t = CLAMP (value, 0, PIZ_MAGIC_PITCH); break;
        case PIZ_VALUE_VELOCITY : t = CLAMP (value, 0, PIZ_MAGIC_VELOCITY); break;
        case PIZ_VALUE_DURATION : m = MIN   (x->size - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                                 t = CLAMP (value, 1, m); break;
        case PIZ_VALUE_CHANNEL  : t = CLAMP (value, 0, PIZ_MAGIC_CHANNEL); break;
        }
        
        if (note->values[selector] != t) {
            note->values[selector] = t;
            pizItemsetSetAtIndex (&x->changedNotes, note->tag);
        }
    }
    
    note = nextNote;
    //
    }
    //
    }*/
    
    return PIZ_GOOD;
}

//PIZError pizSequenceRandom (PIZSequence *x, PIZMidiSelector selector, long minValue, long maxValue)
PIZError pizSequenceRandom (PIZSequence *x, const PIZEvent *event)
{/*
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
    
    long p = pizArrayAtIndex (x->map, i);
    
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
    case PIZ_VALUE_PITCH    : t = CLAMP (note->values[PIZ_VALUE_PITCH] + value, 0, PIZ_MAGIC_PITCH); break;
    case PIZ_VALUE_VELOCITY : t = CLAMP (note->values[PIZ_VALUE_VELOCITY] + value, 0, PIZ_MAGIC_VELOCITY); 
                            break;
    case PIZ_VALUE_DURATION : m = MIN   (x->size - note->position, PIZ_SEQUENCE_MAXIMUM_DURATION);
                             t = CLAMP (note->values[PIZ_VALUE_DURATION] + value, 1, m); break;
    case PIZ_VALUE_CHANNEL  : t = CLAMP (note->values[PIZ_VALUE_CHANNEL] + value, 0, PIZ_MAGIC_CHANNEL); break;
    }
    
    if (note->values[selector] != t) {
        note->values[selector] = t;
        pizItemsetSetAtIndex (&x->changedNotes, note->tag);
    }
    //
    }
    
    note = nextNote;
    //
    }
    //
    }*/
    
    return PIZ_GOOD;
}

//PIZError pizSequenceKill (PIZSequence *x)
PIZError pizSequenceKill (PIZSequence *x, const PIZEvent *event)
{/*
    long i;
    bool haveChanged = false;
        
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (100 * (rand_r (&x->seed) / (RAND_MAX + 1.0)) < x->chance) {
                pizSequenceRemoveNote (x, note, NULL);
                haveChanged = true;
            }
                
            note = nextNote;
        }
    }
    
    if (haveChanged) {
        pizSequenceMakeMap (x);
    }*/
    
    return PIZ_GOOD;
}

//PIZError pizSequenceCycle (PIZSequence *x, PIZScaleKey key, const PIZArray *a)
PIZError pizSequenceCycle (PIZSequence *x, const PIZEvent *event)
{/*
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
    
    temp = pizArrayAtIndex (a, 0);
    m = CLAMP (temp, 0, PIZ_MAGIC_SCALE - 1);
    o = m;
    
    for (i = 1; i < count; i++) {
        temp = pizArrayAtIndex (a, i);
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
    
    long p = pizArrayAtIndex (x->map, i);
    
    pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
    
    while (note) {
        long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        
        pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
        
        if (h < x->chance) {
            long pitch, offset = 0;
            
            pitch = note->values[PIZ_VALUE_PITCH];
            
            if (scale) {
                offset += pizArrayAtIndex (x->scale, pitch % scale);
            }
            
            pitch = CLAMP (pitch + offset, 0, PIZ_MAGIC_PITCH);
            pitch += s[pitch % PIZ_MAGIC_SCALE];
    
            if (pitch != (note->values[PIZ_VALUE_PITCH] + offset)) {
                note->values[PIZ_VALUE_PITCH] = CLAMP (pitch, 0, PIZ_MAGIC_PITCH);
                pizItemsetSetAtIndex (&x->changedNotes, note->tag);
            }
        }

        note = nextNote;
    }
    //
    }
    //    
    }
    //    
    }*/
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceLearn (PIZSequence *x, const PIZEvent *event)
{
    return PIZ_GOOD;
}

//PIZError pizSequenceAlgorithm (PIZSequence *x, PIZAlgorithm *algorithm)
PIZError pizSequenceAlgorithm (PIZSequence *x, const PIZEvent *event)
{
    PIZError        err = PIZ_GOOD; /*
    long            k;
    PIZMethodLong   count = NULL;
    PIZMethodError  proceed = NULL;
    PIZMidiSelector selector = PIZ_VALUE_PITCH;
    
    PIZ_PICKUP_NOTES

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
                x->tempValues[i] = x->tempNotes1[i]->values[selector];
            } 
        }
        
        PIZ_FILL_NOTES
    }*/
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

//PIZError pizSequenceNovember (PIZSequence *x, long iterate)
PIZError pizSequenceNovember (PIZSequence *x, const PIZEvent *event)
{
    PIZError err1 = PIZ_GOOD;
    /*                                                
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long k           = 0;
    long loop        = 0;
    long start       = PIZ_CEIL (x->start, x->cell);
    long end         = PIZ_CEIL (x->end, x->cell);
    long scale       = pizArrayCount (x->scale);
    long mapCount    = pizArrayCount (x->map);
    
    bool haveChanged = false;
    
    x->tempError = PIZ_GOOD;
    pizHashTableClear (x->tempHash);
    pizSequenceForEach (x, pizSequenceFillTempHash, NULL);
    err1 = x->tempError;

    while (!err1 && (k < iterate) && (loop < PIZ_MAXIMUM_LOOP)) {
    //
    if (x->count) {
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
            p = pizArrayAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
            if (pizLinklistCount (x->timeline[p])) {
                q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
            } 
        }                           

        pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note);
        
        if (scale) {
            offset = pizArrayAtIndex (x->scale, note->values[PIZ_VALUE_PITCH] % scale);
        }
        
        pitch = note->values[PIZ_VALUE_PITCH] + offset;
        
        hCenter = ((long)(note->position / (double)x->cell)) * (PIZ_MAGIC_PITCH + 1);
        hCenter += note->values[PIZ_VALUE_PITCH] + offset;
        
        err2 |= (note->position < x->start);
        err2 |= (note->position >= x->end);
        err2 |= (pitch < x->down);
        err2 |= (pitch > x->up);
        
        if (!err2) {
            for (j = 0; j < PIZ_NEIGHBORS_DEATH_SIZE; j++) {
                long key = hCenter + pizSequenceNeighbors[j];
                if (pizHashTableContainsKey (x->tempHash, key)) {
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
            previous += pizArrayAtIndex (x->pattern, previous % patternSize);
            next     += pizArrayAtIndex (x->pattern, next % patternSize);
        }

        if (previous != here) {
            hPat[1] = (previous * (PIZ_MAGIC_PITCH + 1)) + pitch;
            
            if (scale) {
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n, t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[0] = hPat[1] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n, t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayAtIndex (x->scale, b % scale);
                    
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
                    long n, t = pitch - j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayAtIndex (x->scale, b % scale);
                    
                    if ((b + n) != pitch) {
                        hPat[3] = hPat[4] - j + n;
                        break;
                    }
                }
                
                for (j = 1; j < PIZ_MAGIC_SCALE; j++) {
                    long n, t = pitch + j;
                    long b = CLAMP (t, 0, PIZ_MAGIC_PITCH);
                    
                    n = pizArrayAtIndex (x->scale, b % scale);
                    
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
                !(pizHashTableContainsKey (x->tempHash, hPat[j] - 2)) &&
                !(pizHashTableContainsKey (x->tempHash, hPat[j] - 1)) &&
                !(pizHashTableContainsKey (x->tempHash, hPat[j]))     &&
                !(pizHashTableContainsKey (x->tempHash, hPat[j] + 1)) &&
                !(pizHashTableContainsKey (x->tempHash, hPat[j] + 2))) {
                long    t;
                PIZNote *ptr = NULL;
                PIZNote *noteToCopy = NULL;
                
                neighbors = 0;
        
                for (t = 0; t < PIZ_NEIGHBORS_BIRTH_SIZE; t++)  {
                    long key = hPat[j] + pizSequenceNeighbors[t];
                    if (!(pizHashTablePtrByKey (x->tempHash, key, (void **)&ptr))) {
                        neighbors ++;
                        noteToCopy = ptr;
                    }
                }
                
                if (neighbors == 1) {
                //
                long values[ ] = { ((long)(hPat[j] / (double)(PIZ_MAGIC_PITCH + 1))) * x->cell,
                                   hPat[j] % (PIZ_MAGIC_PITCH + 1),
                                   noteToCopy->values[PIZ_VALUE_VELOCITY], 
                                   noteToCopy->values[PIZ_VALUE_DURATION],
                                   noteToCopy->values[PIZ_VALUE_CHANNEL] };
                                    
                PIZNote *newNote = pizSequenceNewNote (x, -1, values, PIZ_SEQUENCE_FLAG_CLIP);
                
                if (newNote) {
                    err1 |= pizHashTableAdd (x->tempHash, hPat[j], (void *)newNote);
                    haveChanged = true;
                    k ++;
                } 
                
                break;
                //
                }
            }
        }
            
        if (death) {
            pizHashTableRemove (x->tempHash, hCenter, (void *)note);
            pizSequenceRemoveNote (x, note, NULL);
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
    }*/
    
    return err1;
}

//PIZError pizSequenceJuliet (PIZSequence *x, long iterate, long division)
PIZError pizSequenceJuliet (PIZSequence *x, const PIZEvent *event)
{
    PIZError err = PIZ_GOOD;/*
    bool     haveChanged = false;
    
    if (x->cell != PIZ_NOTE_VALUE_NONE) {
    //
    long i, size;
    long b = 0;
    long start = PIZ_CEIL (x->start, x->cell);
    long end   = PIZ_CEIL (x->end, x->cell); 
    
    if (size = (end - start)) {
        for (i = 0; i < PIZ_DIVISIONS_SIZE; i++) {
            if (!(size % pizSequenceDivisions[i])) {
                b = pizSequenceDivisions[i];
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
     
    x->tempError = PIZ_GOOD;
    pizHashTableClear (x->tempHash);
    pizSequenceForEach (x, pizSequenceFillTempHash, NULL);
    err = x->tempError;
    
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
        p = pizArrayAtIndex (x->map, (long)(mapCount * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
        if (pizLinklistCount (x->timeline[p])) {
            q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        } 
    }                           

    pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note1);     
    
    step = MIN (a, PIZ_JULIET_MAXIMUM_OFFSET) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    step *= b;
    
    switch (b) {
        case 2  :   distribution = pizSequenceDistribution2;   break;
        case 3  :   distribution = pizSequenceDistribution3;   break;
        case 4  :   distribution = pizSequenceDistribution4;   break;
        case 5  :   distribution = pizSequenceDistribution5;   break;
        case 7  :   distribution = pizSequenceDistribution7;   break;
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
        newPosition += pizArrayAtIndex (x->pattern, newPosition % patternSize);
    }
    
    if (scale) {
        offset = pizArrayAtIndex (x->scale, note1->values[PIZ_VALUE_PITCH] % scale);
    }
                
    newKey = (newPosition * (PIZ_MAGIC_PITCH + 1)) + note1->values[PIZ_VALUE_PITCH] + offset;
    
    if (!(pizHashTableContainsKey (x->tempHash, newKey))) {
    //
        long values[ ] = { newPosition * x->cell,
                           note1->values[PIZ_VALUE_PITCH] + offset,
                           note1->values[PIZ_VALUE_VELOCITY],
                           note1->values[PIZ_VALUE_DURATION],
                           note1->values[PIZ_VALUE_CHANNEL] };

        note2 = pizSequenceNewNote (x, -1, values, PIZ_SEQUENCE_FLAG_CLIP);
        
        if (note2) {
            err |= pizHashTableAdd (x->tempHash, newKey, (void *)note2);
            haveChanged = true;
            k ++;
        } 
    //
    } else if (!(pizHashTablePtrByKey (x->tempHash, newKey, (void **)&note2))) {
        if (note2 != note1) {
            pizHashTableRemove (x->tempHash, newKey, (void *)note2);
            pizSequenceRemoveNote (x, note2, NULL);
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
    }*/
                        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceFillNotes (PIZSequence *x, long selector, bool reverse)
{
    long i;
    
    for (i = 0; i < x->tempIndex; i++) {
    //
    long t;

    if (!reverse) {
        t = x->tempValues[i];
    } else {
        t = x->tempValues[(x->tempIndex - 1) - i];
    }
    
    if (x->tempNotes1[i]->values[selector] != t) {
        x->tempNotes1[i]->values[selector] = t;
        pizItemsetSetAtIndex (&x->changedNotes, x->tempNotes1[i]->tag);
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
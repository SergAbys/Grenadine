/*
 * \file    pizSequenceMethods.c
 * \author  Jean Sapristi
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

#include "pizAgent.h"
#include "pizSequenceMethods.h"
#include "pizSequencePrivate.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <string.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SIZE_H          6
#define PIZ_SIZE_BIRTH      12
#define PIZ_SIZE_DEATH      16
#define PIZ_MAXIMUM_LOOP    20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const long   pizSequenceNeighbors[ ] = { -256, 
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

#define PIZ_CEIL(a,b) (((a)%(b))==0?(a)/(b):(a)/(b)+1)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceNote (PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData (event, &argc, &argv))) {
        long  i;
        ulong flags = PIZ_SEQUENCE_FLAG_SNAP | PIZ_SEQUENCE_FLAG_AMBITUS;
        long  values[ ] = { -1,
                            PIZ_SEQUENCE_DEFAULT_PITCH, 
                            PIZ_SEQUENCE_DEFAULT_VELOCITY, 
                            x->value, 
                            0 };
        
        for (i = 0; i < MIN (argc, 5); i++) {
            values[i] = argv[i];
        }
        
        if (pizSequenceNewNote (x, values, flags)) {
            pizSequenceMakeMap (x);
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceZone (PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData (event, &argc, &argv))) {
    //
    long i;
    long v[ ] = { x->start, x->end, x->down, x->up };
    
    for (i = 0; i < MIN (argc, 4); i++) {
        v[i] = argv[i];
    }
    
    v[0] = pizSequenceSnapByCell (x, v[0]);
    v[1] = pizSequenceSnapByCell (x, v[1]);
    
    v[0] = CLAMP (v[0], 0, PIZ_SEQUENCE_SIZE_TIMELINE);
    v[1] = CLAMP (v[1], 0, PIZ_SEQUENCE_SIZE_TIMELINE);
    v[2] = CLAMP (v[2], 0, PIZ_MAGIC_PITCH);
    v[3] = CLAMP (v[3], 0, PIZ_MAGIC_PITCH);
    
    if (v[0] != v[1]) {
    //
    if (v[0] < v[1]) { x->start = v[0]; x->end = v[1]; } else { x->end = v[0]; x->start = v[1]; }
    if (v[2] < v[3]) { x->down = v[2]; x->up = v[3]; } else { x->up = v[2]; x->down = v[3]; }
    //
    }
        
    x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceClear (PIZSequence *x, const PIZEvent *event)
{
    if (x->count) {
        pizSequenceForEach (x, NULL, PIZ_SEQUENCE_FLAG_NONE, pizSequenceEachRemove);
        pizArrayClear (x->map);  
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceClean (PIZSequence *x, const PIZEvent *event)
{
    long i;
    
    for (i = 0; i < (PIZ_MAGIC_PITCH + 1); i++) {
        x->tempValues[i] = 0;
    }
    
    x->tempIndex = 0;
    
    pizSequenceForEach (x, event, PIZ_SEQUENCE_FLAG_NEARBY, pizSequenceEachTempNotes);
    
    if (x->tempIndex) {
        for (i = 0; i < x->tempIndex; i++) {
            pizSequenceEachRemove (x, NULL, PIZ_SEQUENCE_FLAG_NONE, x->tempNotes1[i]);
        }
        pizSequenceMakeMap (x);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceRotate (PIZSequence *x, const PIZEvent *event)
{
    long i, k, selector, shift = 1;
        
    pizEventValue  (event, &shift);
    pizEventOption (event, &selector);
    selector = CLAMP (selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
    
    k = pizSequenceFillTempNotes (x);
    
    if (k && (shift < 0)) {
        shift = k - ((-shift) % k);
    }

    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes1[(i + shift) % k]->values[selector];
    }
                
    pizSequenceSetByTempNotes (x, selector, 0);
    
    return PIZ_GOOD;
}

PIZError pizSequenceScramble (PIZSequence *x, const PIZEvent *event)
{
    long i, k, selector;
    
    pizEventOption (event, &selector);
    selector = CLAMP (selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
        
    k = pizSequenceFillTempNotes (x);
    
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
            
    pizSequenceSetByTempNotes (x, selector, 0);
    
    return PIZ_GOOD;
}

PIZError pizSequenceSort (PIZSequence *x, const PIZEvent *event)
{
    long i, k, selector, down = 0;
    
    pizEventValue (event, &down);
    pizEventOption (event, &selector);
    selector = CLAMP (selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
        
    k = pizSequenceFillTempNotes (x);
    
    for (i = 0; i < PIZ_SEQUENCE_SIZE_TEMP; i++) {
        x->tempValues[i] = 0;
    }
    
    for (i = 0; i < k; i++) { 
        x->tempValues[x->tempNotes1[i]->values[selector]] ++; 
    }   
        
    for (i = 1; i < PIZ_SEQUENCE_SIZE_TEMP; i++) {
        x->tempValues[i] += x->tempValues[i - 1];
    }
                
    for (i = (k - 1); i >= 0; i--) {    
        x->tempNotes2[x->tempValues[x->tempNotes1[i]->values[selector]] - 1] = x->tempNotes1[i];
        x->tempValues[x->tempNotes1[i]->values[selector]] --;
    }
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = x->tempNotes2[i]->values[selector];
    }
     
    if (down) {
        pizSequenceSetByTempNotes (x, selector, 1);
    } else {
        pizSequenceSetByTempNotes (x, selector, 0);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceChange (PIZSequence *x, const PIZEvent *event)
{
    pizSequenceForEach (x, event, PIZ_SEQUENCE_FLAG_RANDOM, pizSequenceEachChange);

    return PIZ_GOOD;
}

PIZError pizSequenceFill (PIZSequence *x, const PIZEvent *event)
{
    pizSequenceForEach (x, event, PIZ_SEQUENCE_FLAG_RANDOM | PIZ_SEQUENCE_FLAG_FILL, pizSequenceEachChange);

    return PIZ_GOOD;
}

PIZError pizSequenceKill (PIZSequence *x, const PIZEvent *event)
{
    long count = x->count;
     
    if (count) {
        pizSequenceForEach (x, NULL, PIZ_SEQUENCE_FLAG_RANDOM, pizSequenceEachRemove);
        
        if (x->count != count) { 
            pizSequenceMakeMap (x); 
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceCycle (PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
        
    if (!(pizEventData (event, &argc, &argv))) {
    //
    ulong mask = 0UL;
    long  key, i, m, n, o, k = 0;
    long  a[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    long  t[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    pizEventOption (event, &key);
    key = CLAMP (key, PIZ_KEY_C, PIZ_KEY_B);
    
    for (i = 0; i < argc; i++) {
        long j = CLAMP (argv[i], 0, PIZ_MAGIC_SCALE - 1);
        if (!((1UL << j) & mask)) {
            mask |= (1UL << j);
            a[k] = j;
            k ++;
        }
    }
    
    if (k > 1) {
    //
    m = a[0];
    o = m;
    
    for (i = 1; i < k; i++) {
        n    = a[i];
        t[m] = n - m;
        m    = n;
    }
    
    t[m] = o - m;
    
    for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
        x->tempValues[i] = t[(PIZ_MAGIC_SCALE - key + i) % PIZ_MAGIC_SCALE];
    }
    
    pizSequenceForEach (x, NULL, PIZ_SEQUENCE_FLAG_RANDOM, pizSequenceEachCycle);
    //
    }
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceAlgorithm (PIZSequence *x, const PIZEvent *event)
{
    long         k;
    PIZError     err = PIZ_GOOD;
    PIZEventCode code;
    
    pizEventCode (event, &code);
    k = pizSequenceFillTempNotes (x);
    
    if (code == PIZ_EVENT_ZOULOU) {
        err = pizFactorOracleProceed (((PIZAgent *)(x->agent))->factorOracle, k, x->tempValues);
    } else {
        err = pizGaloisLatticeProceed (((PIZAgent *)(x->agent))->galoisLattice, k, x->tempValues);
    }
    
    if (!err) {
    //
    long i;  
    
    for (i = 0; i < k; i++) {
        x->tempValues[i] = pizSequenceSnapByAmbitus (x, x->tempValues[i]);
    }
    
    for (i = 0; i < k; i++) {        
        long h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        if (h >= x->chance) {
            x->tempValues[i] = x->tempNotes1[i]->values[PIZ_VALUE_PITCH];
        } 
    }
    
    pizSequenceSetByTempNotes (x, PIZ_VALUE_PITCH, 0);
    //
    }
    
    return err;
}

PIZError pizSequenceJuliet (PIZSequence *x, const PIZEvent *event)
{
    long     iterate = 1;
    PIZError hashErr = PIZ_GOOD;
    
    pizEventValue (event, &iterate); 
    
    if (x->cell != PIZ_NOTE_NONE) {
    //
    long k       = 0;
    long loop    = 0;
    long start   = PIZ_CEIL (x->start, x->cell);
    long end     = PIZ_CEIL (x->end, x->cell);
    long count   = pizArrayCount (x->map);
    
    bool haveChanged = false;
    
    x->tempError = PIZ_GOOD;
    pizHashTableClear  (x->tempHash);
    pizSequenceForEach (x, NULL, PIZ_SEQUENCE_FLAG_NONE, pizSequenceEachTempHash);
    hashErr = x->tempError;
    
    while (!hashErr && (k < iterate) && (loop < PIZ_MAXIMUM_LOOP)) {
    //
    if (x->count) {
    //
    long    j, pitch, here, previous, next, center;
    long    q = -1;
    long    p = -1;
    long    hPat[PIZ_SIZE_H] = { -1, -1, -1, -1, -1, -1 };
    long    neighbors = 0;
    long    err = PIZ_GOOD;
    long    size = pizArrayCount (x->pattern);
    bool    death = false;
    PIZNote *note = NULL;

    while (q == -1) {
        p = pizArrayAtIndex (x->map, (long)(count * (rand_r (&x->seed) / (RAND_MAX + 1.0))));
        if (pizLinklistCount (x->timeline[p])) {
            q = (long)(pizLinklistCount (x->timeline[p]) * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        } 
    }                           

    pizLinklistPtrAtIndex (x->timeline[p], q, (void **)&note);
    
    here     = (long)(note->position / (double)x->cell);
    pitch    = note->values[PIZ_VALUE_PITCH];
    center   = (here * (PIZ_MAGIC_PITCH + 1)) + pitch;
    previous = here - 1;
    next     = here + 1;
    
    if (next >= end) { next = start; }
    if (previous < start) { previous = (end - 1); }
    
    if (size) {  
        previous += pizArrayAtIndex (x->pattern, previous % size);
        next     += pizArrayAtIndex (x->pattern, next % size);
    }

    if (previous != here) {
        hPat[1] = (previous * (PIZ_MAGIC_PITCH + 1)) + pitch;
        hPat[0] = hPat[1] - 1;
        hPat[2] = hPat[1] + 1;
    }
    
    if (next != here) {
        hPat[4] = (next * (PIZ_MAGIC_PITCH + 1)) + pitch;
        hPat[3] = hPat[4] - 1;
        hPat[5] = hPat[4] + 1;
    }
                
    for (j = (PIZ_SIZE_H - 1); j > 0; j--)  {
        long h    = (j + 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
        long temp = hPat[h];
        hPat[h]   = hPat[j];
        hPat[j]   = temp;
    }
    
    err |= (note->position < x->start);
    err |= (note->position >= x->end);
    err |= (pitch < x->down);
    err |= (pitch > x->up);
    
    if (!err) {
    //
    for (j = 0; j < PIZ_SIZE_DEATH; j++) {
        if (pizHashTableContainsKey (x->tempHash, (center + pizSequenceNeighbors[j]))) {
            neighbors ++;
        }
    }
    if (neighbors > 1) {
        death = true;
    }
    //
    }
    
    for (j = 0; j < PIZ_SIZE_H; j++) {
        if ((hPat[j] >= 0) && 
            !(pizHashTableContainsKey (x->tempHash, hPat[j] - 2)) &&
            !(pizHashTableContainsKey (x->tempHash, hPat[j] - 1)) &&
            !(pizHashTableContainsKey (x->tempHash, hPat[j]))     &&
            !(pizHashTableContainsKey (x->tempHash, hPat[j] + 1)) &&
            !(pizHashTableContainsKey (x->tempHash, hPat[j] + 2))) {
            long    t;
            PIZNote *temp = NULL;
            PIZNote *toCopy = NULL;
            PIZNote *newNote = NULL;
            
            neighbors = 0;
            
            for (t = 0; t < PIZ_SIZE_BIRTH; t++) {
            //
            if (!(pizHashTablePtrWithKey (x->tempHash, hPat[j] + pizSequenceNeighbors[t], (void **)&temp))) {
                toCopy = temp;
                neighbors ++;
            }
            //
            }
                        
            if (neighbors == 1) {
            //
            long values[ ] = { ((long)(hPat[j] / (double)(PIZ_MAGIC_PITCH + 1))) * x->cell,
                               hPat[j] % (PIZ_MAGIC_PITCH + 1),
                               toCopy->values[PIZ_VALUE_VELOCITY], 
                               toCopy->values[PIZ_VALUE_DURATION],
                               toCopy->values[PIZ_VALUE_CHANNEL] };
                        
            if (newNote = pizSequenceNewNote (x, values, PIZ_SEQUENCE_FLAG_CLIP)) {
                hashErr |= pizHashTableAdd (x->tempHash, hPat[j], (void *)newNote);
                haveChanged = true;
                k ++;
            }
            
            break;
            //
            }
        } 
    }
        
    if (death) {
        pizHashTableRemove (x->tempHash, center, (void *)note);
        pizSequenceEachRemove (x, NULL, PIZ_SEQUENCE_FLAG_NONE, note);
        haveChanged = true;
    }
    //    
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
    
    return hashErr;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
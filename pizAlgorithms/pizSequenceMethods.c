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
#include "pizSequenceRun.h"
#include "pizSequenceEach.h"
#include "pizSequenceMethods.h"
#include "pizSequenceUtils.h"

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

PIZ_LOCAL PIZNote   *pizSequenceNewNote         (PIZSequence *x, long *argv, ulong flags);
PIZ_LOCAL PIZError  pizSequenceGetTag           (PIZSequence *x, long *ptr);

PIZ_LOCAL long      pizSequenceFillTempNotes    (PIZSequence *x);
PIZ_LOCAL void      pizSequenceWithTempNotes    (PIZSequence *x, long selector, bool reverse);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceNote(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc >= 2)) {
        long i;
        PIZEventType type;
        ulong flags = PIZ_SEQUENCE_FLAG_NONE; 
        long  values[ ] = { -1,
                            PIZ_SEQUENCE_DEFAULT_PITCH, 
                            PIZ_SEQUENCE_DEFAULT_VELOCITY, 
                            x->value, 
                            0 };
        
        for (i = 0; i < MIN(argc, 5); i++) {
            values[i] = argv[i];
        }
        
        pizEventType(event, &type);
        
        if (type == PIZ_EVENT_HIGH) {
            flags |= PIZ_SEQUENCE_FLAG_SNAP | PIZ_SEQUENCE_FLAG_AMBITUS;
            
        } else if (type == PIZ_EVENT_LOW) {
            flags |= PIZ_SEQUENCE_FLAG_SNAP | PIZ_SEQUENCE_FLAG_LOW;
        } 
        
        pizSequenceNewNote(x, values, flags);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceClear(PIZSequence *x, const PIZEvent *event)
{
    if (x->count) {
        pizSequenceForEach(x, pizSequenceEachRemove, NULL, PIZ_SEQUENCE_FLAG_NONE);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceClean(PIZSequence *x, const PIZEvent *event)
{
    long i;
    
    for (i = 0; i < (PIZ_MAGIC_PITCH + 1); i++) {
        x->temp.values[i] = 0;
    }
    
    x->temp.index = 0;
    
    pizSequenceForEach(x, pizSequenceEachFillTempNotes, event, PIZ_SEQUENCE_FLAG_NEARBY);
    
    if (x->temp.index) {
        for (i = 0; i < x->temp.index; i++) {
            pizSequenceEachRemove(x, x->temp.notes1[i], NULL, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceTranspose(PIZSequence *x, const PIZEvent *event)
{
    long argc, down, up;
    long *argv = NULL;
    PIZError err = PIZ_ERROR;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        err = PIZ_GOOD;
        
        down = x->down + argv[1];
        up = x->up + argv[1];
        
        err |= ((down < 0) || (down > PIZ_MAGIC_PITCH));
        err |= ((up < 0) || (up > PIZ_MAGIC_PITCH));
    }
    
    if (!err) {
        x->up = up;
        x->down = down;
        x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
        pizSequenceForEach(x, pizSequenceEachChange, event, PIZ_SEQUENCE_FLAG_NONE);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceRotate(PIZSequence *x, const PIZEvent *event)
{
    long i, k, argc, shift = 1, selector = 0;
    long *argv = NULL;
        
    if (!(pizEventData(event, &argc, &argv))) {
        selector = argv[0];
        if (argc > 1) { shift = argv[1]; }
    }
    
    selector = CLAMP(selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
    
    k = pizSequenceFillTempNotes(x);
    
    if (k && (shift < 0)) {
        shift = k - ((-shift) % k);
    }

    for (i = 0; i < k; i++) {
        x->temp.values[i] = x->temp.notes1[(i + shift) % k]->values[selector];
    }
                
    pizSequenceWithTempNotes(x, selector, 0);
    
    return PIZ_GOOD;
}

PIZError pizSequenceScramble(PIZSequence *x, const PIZEvent *event)
{
    long i, k, argc, selector = 0;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        selector = argv[0];
    }
    
    selector = CLAMP(selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
        
    k = pizSequenceFillTempNotes(x);
    
    for (i = 0; i < k; i++) {
        x->temp.values[i] = x->temp.notes1[i]->values[selector];
    }
        
    for (i = (k - 1); i > 0; i--) {
        long h = (i + 1) * (rand_r(&x->seed) / (RAND_MAX + 1.0));
        PIZNote *temp = NULL;
            
        temp = x->temp.notes1[h];
        x->temp.notes1[h] = x->temp.notes1[i];
        x->temp.notes1[i] = temp;
    }
            
    pizSequenceWithTempNotes(x, selector, 0);
    
    return PIZ_GOOD;
}

PIZError pizSequenceSort(PIZSequence *x, const PIZEvent *event)
{
    long i, k, argc, down = 0, selector = 0;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        selector = argv[0];
        if (argc > 1) { down = argv[1]; }
    }
    
    selector = CLAMP(selector, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
        
    k = pizSequenceFillTempNotes(x);
    
    for (i = 0; i < PIZ_SEQUENCE_SIZE_TEMPORARY; i++) {
        x->temp.values[i] = 0;
    }
    
    for (i = 0; i < k; i++) { 
        x->temp.values[x->temp.notes1[i]->values[selector]] ++; 
    }   
        
    for (i = 1; i < PIZ_SEQUENCE_SIZE_TEMPORARY; i++) {
        x->temp.values[i] += x->temp.values[i - 1];
    }
                
    for (i = (k - 1); i >= 0; i--) {    
        x->temp.notes2[x->temp.values[x->temp.notes1[i]->values[selector]] - 1] = x->temp.notes1[i];
        x->temp.values[x->temp.notes1[i]->values[selector]] --;
    }
    
    for (i = 0; i < k; i++) {
        x->temp.values[i] = x->temp.notes2[i]->values[selector];
    }
     
    if (down) {
        pizSequenceWithTempNotes(x, selector, 1);
    } else {
        pizSequenceWithTempNotes(x, selector, 0);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceChange(PIZSequence *x, const PIZEvent *event)
{
    pizSequenceForEach(x, pizSequenceEachChange, event, PIZ_SEQUENCE_FLAG_RANDOM);

    return PIZ_GOOD;
}

PIZError pizSequenceFill(PIZSequence *x, const PIZEvent *event)
{
    pizSequenceForEach(x, pizSequenceEachChange, event, PIZ_SEQUENCE_FLAG_RANDOM | PIZ_SEQUENCE_FLAG_FILL);

    return PIZ_GOOD;
}

PIZError pizSequenceKill(PIZSequence *x, const PIZEvent *event)
{
    long count = x->count;
     
    if (count) {
        pizSequenceForEach(x, pizSequenceEachRemove, NULL, PIZ_SEQUENCE_FLAG_RANDOM);
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceCycle(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
        
    if ((!(pizEventData(event, &argc, &argv))) && (argc > 2)) {
    //
    ulong mask = 0UL;
    long i, m, n, o, k = 0;
    long a[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    long t[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    long key = CLAMP(argv[0], PIZ_KEY_C, PIZ_KEY_B);
    
    for (i = 1; i < argc; i++) {
        long j = CLAMP(argv[i], 0, PIZ_MAGIC_SCALE - 1);
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
        x->temp.values[i] = t[(PIZ_MAGIC_SCALE - key + i) % PIZ_MAGIC_SCALE];
    }
    
    pizSequenceForEach(x, pizSequenceEachCycle, NULL, PIZ_SEQUENCE_FLAG_RANDOM);
    //
    }
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceAlgorithm(PIZSequence *x, const PIZEvent *event)
{
    long k;
    PIZError err = PIZ_GOOD;
    PIZEventCode code = PIZ_MSG_NONE;
    
    pizEventCode(event, &code);
    k = pizSequenceFillTempNotes(x);
    
    if (code == PIZ_MSG_ZOULOU) {
        err = pizFactorOracleProceed(pizAgentOraclePtr(x->agent), k, x->temp.values);
    } else {
        err = pizGaloisLatticeProceed(pizAgentLatticePtr(x->agent), k, x->temp.values);
    }
    
    if (!err) {
    //
    long i;  
    
    for (i = 0; i < k; i++) {
        x->temp.values[i] = CLAMP((pizSequenceSnapByAmbitus(x, x->temp.values[i])), 0, PIZ_MAGIC_PITCH);
    }
    
    for (i = 0; i < k; i++) {        
        long h = 100 * (rand_r(&x->seed) / (RAND_MAX + 1.0));
        if (h >= x->chance) {
            x->temp.values[i] = x->temp.notes1[i]->values[PIZ_VALUE_PITCH];
        } 
    }
    
    pizSequenceWithTempNotes(x, PIZ_VALUE_PITCH, 0);
    //
    }
    
    return err;
}

PIZError pizSequencePattern(PIZSequence *x, const PIZEvent *event)
{
    long i, k;  

    x->temp.error = PIZ_GOOD;
    
    if (k = pizSequenceFillTempNotes(x)) {
        for (i = 0; i < k; i++) {
            pizSequenceEachMove(x, x->temp.notes1[i], event, PIZ_SEQUENCE_FLAG_PATTERN);
        }
    }

    return x->temp.error;
}

PIZError pizSequenceJuliet(PIZSequence *x, const PIZEvent *event)
{
    long argc, iterate = 1;
    long *argv = NULL;
    PIZError hashError = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv))) {
        iterate = argv[0];
    }
        
    if (x->cell != PIZ_NOTE_NONE) {
    //
    long k = 0;
    long loop = 0;
    long start = PIZ_CEIL(x->start, x->cell);
    long end = PIZ_CEIL(x->end, x->cell);
        
    x->temp.error = PIZ_GOOD;
    pizHashTableClear(x->temp.hash);
    pizSequenceForEach(x, pizSequenceEachFillTempHash, NULL, PIZ_SEQUENCE_FLAG_NONE);
    hashError = x->temp.error;
    
    while (!hashError && (k < iterate) && (loop < PIZ_MAXIMUM_LOOP)) {
    //
    if (x->count) {
    //
    long i, pitch, here, previous, next, center;
    long hPat[PIZ_SIZE_H] = { -1, -1, -1, -1, -1, -1 };
    long neighbors = 0;
    bool death = false;
    PIZNote *note = NULL;
    PIZError err = PIZ_GOOD;
    long j = (long)(x->count * (rand_r(&x->seed) / (RAND_MAX + 1.0)));
    
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (x->lookup[i]) {
            if (j == 0) {
                note = x->lookup[i];
                break;
            }
            j--;
        }
    }
    
    here     = (long)(note->position / (double)x->cell);
    pitch    = note->values[PIZ_VALUE_PITCH];
    center   = (here * (PIZ_MAGIC_PITCH + 1)) + pitch;
    previous = here - 1;
    next     = here + 1;
    
    if (next >= end) { next = start; }
    if (previous < start) { previous = (end - 1); }

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
                
    for (i = (PIZ_SIZE_H - 1); i > 0; i--)  {
        long h    = (i + 1) * (rand_r(&x->seed) / (RAND_MAX + 1.0));
        long temp = hPat[h];
        hPat[h]   = hPat[i];
        hPat[i]   = temp;
    }
    
    err |= (note->position < x->start);
    err |= (note->position >= x->end);
    err |= (pitch < x->down);
    err |= (pitch > x->up);
    
    if (!err) {
    //
    for (i = 0; i < PIZ_SIZE_DEATH; i++) {
        if (pizHashTableContainsKey(x->temp.hash, (center + pizSequenceNeighbors[i]))) {
            neighbors ++;
        }
    }
    if (neighbors > 1) {
        death = true;
    }
    //
    }
    
    for (i = 0; i < PIZ_SIZE_H; i++) {
        if ((hPat[i] >= 0) && 
            !(pizHashTableContainsKey(x->temp.hash, hPat[i] - 2)) &&
            !(pizHashTableContainsKey(x->temp.hash, hPat[i] - 1)) &&
            !(pizHashTableContainsKey(x->temp.hash, hPat[i]))     &&
            !(pizHashTableContainsKey(x->temp.hash, hPat[i] + 1)) &&
            !(pizHashTableContainsKey(x->temp.hash, hPat[i] + 2))) {
            long t;
            PIZNote *temp = NULL;
            PIZNote *toCopy = NULL;
            PIZNote *newNote = NULL;
            
            neighbors = 0;
            
            for (t = 0; t < PIZ_SIZE_BIRTH; t++) {
            //
            if (!(pizHashTablePtrWithKey(x->temp.hash, hPat[i] + pizSequenceNeighbors[t], (void **)&temp))) {
                toCopy = temp;
                neighbors ++;
            }
            //
            }
                        
            if (neighbors == 1) {
            //
            long values[ ] = { ((long)(hPat[i] / (double)(PIZ_MAGIC_PITCH + 1))) * x->cell,
                               hPat[i] % (PIZ_MAGIC_PITCH + 1),
                               toCopy->values[PIZ_VALUE_VELOCITY], 
                               toCopy->values[PIZ_VALUE_DURATION],
                               toCopy->values[PIZ_VALUE_CHANNEL] };
                        
            if (newNote = pizSequenceNewNote(x, values, PIZ_SEQUENCE_FLAG_CLIP)) {
                hashError |= pizHashTableAdd(x->temp.hash, hPat[i], (void *)newNote);
                k ++;
            }
            
            break;
            //
            }
        } 
    }
        
    if (death) {
        pizHashTableRemove(x->temp.hash, center, (void *)note);
        pizSequenceEachRemove(x, note, NULL, PIZ_SEQUENCE_FLAG_NONE);
    }
    //    
    } else {
        break;
    }
    
    loop ++;
    //    
    }
    //
    }
    
    return hashError;
}

PIZError pizSequenceDump(PIZSequence *x, const PIZEvent *event)
{
    x->temp.error = pizAgentNotify(x->agent, PIZ_NOTIFY_WILL_DUMP, 0, NULL);
    
    pizSequenceForEach(x, pizSequenceEachDump, NULL, PIZ_SEQUENCE_FLAG_NONE);
    
    return x->temp.error;
}

PIZError pizSequenceStatistics(PIZSequence *x, const PIZEvent *event)
{
    return pizAgentNotify(x->agent, PIZ_INFORM_COUNT, 1, &x->count);
}

PIZError pizSequenceAttributes(PIZSequence *x, const PIZEvent *event)
{
    PIZError err = PIZ_GOOD;
    long a[ ] = { x->key, x->type };
    long b[ ] = { x->start, x->end, x->down, x->up };
    
    err |= pizAgentNotify(x->agent, PIZ_INFORM_BPM, 1, &x->bpm);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_CHANCE, 1, &x->chance);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_VELOCITY, 1, &x->velocity);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_CHANNEL, 1, &x->channel);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_CHORD, 1, &x->chord);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_CELL, 1, &x->cell);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_VALUE, 1, &x->value);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_SCALE, 2, a);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_MUTE, 1, &x->mute);
    err |= pizAgentNotify(x->agent, PIZ_INFORM_ZONE, 4, b);
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZNote *pizSequenceNewNote(PIZSequence *x, long *argv, ulong flags)
{
    PIZNote *newNote = NULL;
    long err = PIZ_GOOD;
    long k = -1;
    long position = argv[0];
    long pitch    = argv[1];
    long velocity = argv[2];
    long duration = argv[3]; 
    long channel  = argv[4]; 
    
    if (flags & PIZ_SEQUENCE_FLAG_SNAP) {
        position = ((long)((position / (double)(x->cell)))) * x->cell;
    } 
    
    if (flags & PIZ_SEQUENCE_FLAG_AMBITUS) {
        pitch = pizSequenceSnapByAmbitus(x, pitch);
    }
                
    err |= (position < 0);
    err |= (position > (PIZ_SEQUENCE_SIZE_TIMELINE - 1));
    err |= (x->count >= PIZ_SEQUENCE_MAXIMUM_NOTES);
    
    if (flags & PIZ_SEQUENCE_FLAG_CLIP) {
        err |= (pitch > x->up);
        err |= (pitch < x->down);
        err |= (position < x->start);
        err |= (position >= x->end);
    }
    
    if (!err) {
    //
    err |= pizSequenceGetTag(x, &k);
    
    if (!err && (newNote = (PIZNote *)malloc(sizeof(PIZNote)))) {
    //
    newNote->tag                        = k;
    newNote->position                   = position;
    newNote->values[PIZ_VALUE_PITCH]    = CLAMP(pitch,    0, PIZ_MAGIC_PITCH);
    newNote->values[PIZ_VALUE_VELOCITY] = CLAMP(velocity, 0, PIZ_MAGIC_VELOCITY);
    newNote->values[PIZ_VALUE_DURATION] = CLAMP(duration, 1, PIZ_SEQUENCE_MAXIMUM_DURATION);
    newNote->values[PIZ_VALUE_CHANNEL]  = CLAMP(channel,  0, PIZ_MAGIC_CHANNEL);

    if (!(x->timeline[newNote->position])) {
        if (!(x->timeline[newNote->position] = pizLinklistNew( ))) {
            err |= PIZ_MEMORY;
        }
    }
    
    x->lookup[newNote->tag] = newNote;
                            
    if (!err && !(pizLinklistAppend(x->timeline[newNote->position], (void *)newNote))) {
        x->count ++; 
        x->dirty = true;
        
        if (flags & PIZ_SEQUENCE_FLAG_LOW) {
            pizItemsetSetAtIndex(&x->addedLow, newNote->tag);
        } else {
            pizItemsetSetAtIndex(&x->addedHigh, newNote->tag);
        }
        
    } else {
        x->lookup[newNote->tag] = NULL;
        free(newNote);
        newNote = NULL;
    }
    //
    }
    //    
    }
        
    return newNote;
}   

PIZError pizSequenceGetTag(PIZSequence *x, long *ptr)
{
    long i, k = -1;
    PIZError err = PIZ_ERROR;
    
    for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
        if (!x->lookup[i]) {
            k = i;
            break;
        }
    }
    
    if (k != -1) {
        (*ptr) = k;
        err = PIZ_GOOD;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceFillTempNotes(PIZSequence *x)
{
    x->temp.index = 0;
    pizSequenceForEach(x, pizSequenceEachFillTempNotes, NULL, PIZ_SEQUENCE_FLAG_NONE);
    
    return x->temp.index;
}

void pizSequenceWithTempNotes(PIZSequence *x, long selector, bool reverse)
{
    long i;
    
    for (i = 0; i < x->temp.index; i++) {
    //
    long t;

    if (!reverse) {
        t = x->temp.values[i];
    } else {
        t = x->temp.values[(x->temp.index - 1) - i];
    }
    
    if (x->temp.notes1[i]->values[selector] != t) {
        x->temp.notes1[i]->values[selector] = t;
        pizItemsetSetAtIndex(&x->changed, x->temp.notes1[i]->tag);
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
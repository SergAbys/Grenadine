/*
 * \file    pizSequenceEach.c
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
#pragma mark -
#pragma mark ---
#pragma mark -

void pizSequenceForEach(PIZSequence *x, PIZMethod method, const PIZEvent *e, ulong f)
{
    long i;
    
    if (x->dirty) {
    //
    pizArrayClear(x->map);
    
    for (i = 0; i < PIZ_SEQUENCE_SIZE_TIMELINE; i++) {
        if (x->timeline[i] && pizLinklistCount(x->timeline[i])) {
            pizArrayAppend(x->map, i);
        }
    }
    
    x->dirty = false;
    //
    }
    
    for (i = 0; i < pizArrayCount(x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayAtIndex(x->map, i);
        
        pizLinklistPtrAtIndex(x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextWithPtr(x->timeline[p], (void *)note, (void **)&nextNote);
            (*method)(x, note, e, f);
            note = nextNote;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceEachRemove(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag) 
{
    long h = -1;
    long p = note->position;
    long tag = note->tag;
    
    if (flag & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r(&x->seed) / (RAND_MAX + 1.0));
    }
    
    if ((h == -1) || (h < x->chance)) {
    //
    x->lookup[tag] = NULL;
    pizLinklistRemoveWithPtr(x->timeline[p], (void *)note);
    x->count --; 
    x->dirty = true;
    
    pizItemsetSetAtIndex(&x->removed, tag);
    pizItemsetUnsetAtIndex(&x->changed, tag);
    pizItemsetUnsetAtIndex(&x->addedLow, tag);
    pizItemsetUnsetAtIndex(&x->addedHigh, tag);
    //
    }
}

void pizSequenceEachChange(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{
    long argc;
    long *argv = NULL;
        
    if ((!(pizEventData(e, &argc, &argv))) && (argc > 1)) {
    //
    long t, h = -1;
    long v = argv[1];
    long s = CLAMP(argv[0], PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
    
    if (flag & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r(&x->seed) / (RAND_MAX + 1.0));
    }
          
    if ((h == -1) || (h < x->chance)) {
    //
    if (flag & PIZ_SEQUENCE_FLAG_FILL) {
        t = v;
    } else {
        t = note->values[s] + v;
    }
    
    switch (s) {
        case PIZ_VALUE_PITCH : 
            if ((t < 0) || (t > PIZ_MAGIC_PITCH)) {
                t = note->values[s];
            } break;
            
        case PIZ_VALUE_VELOCITY : 
            t = CLAMP(t, 0, PIZ_MAGIC_VELOCITY);
            break;
            
        case PIZ_VALUE_CHANNEL : 
            t = CLAMP(t, 0, PIZ_MAGIC_CHANNEL);
            break;
            
        case PIZ_VALUE_DURATION : 
            t = CLAMP(t, 0, PIZ_SEQUENCE_MAXIMUM_DURATION);
            if (t == 0) { 
                t = note->values[s]; 
            } break;
    }
    
    if (t != note->values[s]) {
        note->values[s] = t;
        pizItemsetSetAtIndex(&x->changed, note->tag);
    }
    //
    }
    //
    }
}
 
void pizSequenceEachCycle(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{
    long t, h = -1;
    
    if (flag & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r(&x->seed) / (RAND_MAX + 1.0));
    }
          
    if ((h == -1) || (h < x->chance)) {
    //
    t = note->values[PIZ_VALUE_PITCH];
    t += x->temp.values[t % PIZ_MAGIC_SCALE];
    t = CLAMP(t, 0, PIZ_MAGIC_PITCH);
    
    if (note->values[PIZ_VALUE_PITCH] != t) {
        note->values[PIZ_VALUE_PITCH] = t;
        pizItemsetSetAtIndex(&x->changed, note->tag);
    }
    //
    }
}

void pizSequenceEachDump(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{
    long a[ ] = { note->position, 
                  note->values[PIZ_VALUE_PITCH],
                  note->values[PIZ_VALUE_VELOCITY],
                  note->values[PIZ_VALUE_DURATION], 
                  note->values[PIZ_VALUE_CHANNEL],
                  note->tag,
                  x->bpm };
                
    x->temp.error |= pizAgentNotify(x->agent, PIZ_NOTIFICATION_DUMPED, 7, a);
}

void pizSequenceEachMove(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{
    long offset = 0;
    long a = note->position;
    long b = -1;
    long argc;
    long *argv = NULL;
    PIZError err = PIZ_GOOD; 
    
    if (flag & PIZ_SEQUENCE_FLAG_BACKWARD) {
        do {
        b = pizSequenceSnapByCell(x, a + offset);
        offset -= x->cell;
        } while (b >= a);
        
    } else if (flag & PIZ_SEQUENCE_FLAG_FORWARD) {
        do {
        b = pizSequenceSnapByCell(x, a + offset);
        offset += x->cell;
        } while (b <= a);
        
    } else if ((flag & PIZ_SEQUENCE_FLAG_PATTERN) && (!(pizEventData(e, &argc, &argv)))) {
        long k = (long)(a / (double)(x->cell));
        k += argv[k % argc];
        b = (MAX(k, 0)) * x->cell;
    }
    
    if ((b >= 0) && (b < (PIZ_SEQUENCE_SIZE_TIMELINE - 1))) {
    //
    if (!x->timeline[b]) {
        if (!(x->timeline[b] = pizLinklistNew ( ))) {
            err |= PIZ_MEMORY;
        }
    }
    
    if (!err && (!(err |= pizLinklistChuckWithPtr(x->timeline[a], (void *)note)))) {            
        if (!(err |= pizLinklistAppend(x->timeline[b], (void *)note))) {
            note->position = b;
            pizItemsetSetAtIndex(&x->changed, note->tag);
            x->dirty = true;
        } else {
            pizSequenceEachRemove(x, note, NULL, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    //
    }
    
    x->temp.error |= err;
}

void pizSequenceEachFillTempHash(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{   
    long k = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1)) + note->values[PIZ_VALUE_PITCH];
    x->temp.error |= pizHashTableAdd(x->temp.hash, k, (void *)note);
}

void pizSequenceEachFillTempNotes(PIZSequence *x, PIZNote *note, const PIZEvent *e, ulong flag)
{
    if (flag & PIZ_SEQUENCE_FLAG_NEARBY) {
    //
    long argc;
    long *argv = NULL;
    
    if (e && (!(pizEventData(e, &argc, &argv)))) {
    //
    bool death = false;
    long j, a, b, value;
    long pitch = note->values[PIZ_VALUE_PITCH];
    
    value = MAX(0, argv[0]);
    
    a = CLAMP((pitch - value), 0, PIZ_MAGIC_PITCH);
    b = CLAMP((pitch + value), 0, PIZ_MAGIC_PITCH);
    
    for (j = a; j <= b; j++) {
        if (x->temp.values[j] == (note->position + 1)) {
            death = true;
        }
    }
    
    if (death) {
        x->temp.notes1[x->temp.index] = note;
        x->temp.index ++;
    } else {
        x->temp.values[pitch] = (note->position + 1);
    }
    //
    }
    //
    
    } else {
        x->temp.notes1[x->temp.index] = note;
        x->temp.index ++;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
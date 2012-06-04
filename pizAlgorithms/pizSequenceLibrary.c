/*
 * \file    pizSequence.c
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
#include "pizSequenceLibrary.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZNote *pizSequenceNewNote (PIZSequence *x, long tag, long *argv, ulong flags)
{
    PIZNote *newNote = NULL;
    long    err      = PIZ_GOOD;
    long    k        = PIZ_NADA;
    long    position = argv[0];
    long    pitch    = argv[1];
    long    velocity = argv[2];
    long    duration = argv[3]; 
    long    channel  = argv[4]; 
    
    if (flags & PIZ_SEQUENCE_FLAG_SNAP) {
        position = pizSequenceToPattern (x, position);
    } 
    
    if (flags & PIZ_SEQUENCE_FLAG_AMBITUS) {
        pitch = pizSequenceToAmbitus (x, pitch);
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
    
    err |= pizSequenceGetTag (x, tag, &k);
    
    if (!err && (newNote = (PIZNote *)malloc (sizeof(PIZNote)))) {
        newNote->tag                        = k;
        newNote->position                   = position;
        newNote->values[PIZ_VALUE_PITCH]    = CLAMP (pitch,    0, PIZ_MAGIC_PITCH);
        newNote->values[PIZ_VALUE_VELOCITY] = CLAMP (velocity, 0, PIZ_MAGIC_VELOCITY);
        newNote->values[PIZ_VALUE_DURATION] = CLAMP (duration, 1, PIZ_SEQUENCE_MAXIMUM_DURATION);
        newNote->values[PIZ_VALUE_CHANNEL]  = CLAMP (channel,  0, PIZ_MAGIC_CHANNEL);
    
        if (!(x->timeline[newNote->position])) {
            if (!(x->timeline[newNote->position] = pizLinklistNew ( ))) {
                err |= PIZ_MEMORY;
            }
        }
        
        err |= pizHashTableAdd (x->lookup, newNote->tag, newNote);
                                
        if (!err && !(pizLinklistAppend (x->timeline[newNote->position], (void *)newNote))) {
            x->count ++; 
            pizItemsetSetAtIndex (&x->addedNotes, newNote->tag);
            
        } else {
            pizHashTableRemove (x->lookup, newNote->tag, newNote);
            pizItemsetUnsetAtIndex (&x->usedNotes, tag);
            free (newNote);
            newNote = NULL;
        }
    }
    //    
    }
        
    return newNote;
}   

PIZError pizSequenceGetTag (PIZSequence *x, long tag, long *ptr)
{
    long     i, k = PIZ_NADA;
    PIZError err = PIZ_ERROR;
    
    if ((tag >= 0) && (tag < PIZ_ITEMSET_SIZE) && !(pizItemsetIsSetAtIndex (&x->usedNotes, tag))) {
        pizItemsetSetAtIndex (&x->usedNotes, tag);
        k = tag;
    } 
    
    if (k == PIZ_NADA) {
        for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
            if (!(pizItemsetIsSetAtIndex (&x->usedNotes, i))) {
                pizItemsetSetAtIndex (&x->usedNotes, i);
                k = i;
                break;
            }
        }
    }
    
    if (k != PIZ_NADA) {
        (*ptr) = k;
        err = PIZ_GOOD;
    }
    
    return err;
}

void pizSequenceMakeMap (PIZSequence *x)
{
    long i;
            
    pizArrayClear (x->map);
                            
    for (i = 0; i < PIZ_SEQUENCE_SIZE_TIMELINE; i++) {
        if (x->timeline[i] && pizLinklistCount (x->timeline[i])) {
            pizArrayAppend (x->map, i);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceAddNotification (PIZSequence *x, PIZEventCode n, long tag, long ac, long *av)
{
    PIZEvent *notification = NULL;
    PIZError err = PIZ_GOOD;
    
    if (notification = pizEventNew (n)) {
    //
    pizEventSetTag (notification, tag);
    pizEventSetData (notification, ac, av);
    
    if (err |= pizLinklistAppend (x->agent->notification, notification)) {       
        pizEventFree (notification);  
    }
    //
    } else {
        err |= PIZ_MEMORY;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceForEach (PIZSequence *x, const PIZEvent *e, ulong f, PIZMethod method)
{
    long i;
    
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            (*method)(x, e, f, note);
            
            note = nextNote;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceEachRemove (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n) 
{
    long h = -1;
    long p = n->position;
    long tag = n->tag;
    
    if (f & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    }
    
    if ((h == -1) || ( h < x->chance)) {
    //
    pizHashTableRemove (x->lookup, tag, n);
    pizItemsetUnsetAtIndex (&x->usedNotes, tag);
    pizLinklistRemoveByPtr (x->timeline[p], (void *)n);
    x->count --; 
    
    pizItemsetSetAtIndex   (&x->removedNotes, tag);
    pizItemsetUnsetAtIndex (&x->addedNotes, tag);
    pizItemsetUnsetAtIndex (&x->changedNotes, tag);
    //
    }
}

void pizSequenceEachChange (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n)
{
    long v, s;
        
    if (!(pizEventValue (e, &v))) {
    //
    long t, h = -1;
 
    pizEventOption (e, &s);
    s = CLAMP (s, PIZ_VALUE_PITCH, PIZ_VALUE_CHANNEL);
    
    if (f & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    }
          
    if ((h == -1) || (h < x->chance)) {
    //
    if (f & PIZ_SEQUENCE_FLAG_FILL) {
        t = v;
    } else {
        t = n->values[s] + v;
    }
    
    switch (s) {
        case PIZ_VALUE_PITCH    : t = CLAMP (t, 0, PIZ_MAGIC_PITCH);               break;
        case PIZ_VALUE_VELOCITY : t = CLAMP (t, 0, PIZ_MAGIC_VELOCITY);            break;
        case PIZ_VALUE_DURATION : t = CLAMP (t, 1, PIZ_SEQUENCE_MAXIMUM_DURATION); break;
        case PIZ_VALUE_CHANNEL  : t = CLAMP (t, 0, PIZ_MAGIC_CHANNEL);             break;
    }
    
    if (n->values[s] != t) {
        n->values[s] = t;
        pizItemsetSetAtIndex (&x->changedNotes, n->tag);
    }
    //
    }
    //
    }
}
 
void pizSequenceEachCycle (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n)
{
    long t, h = -1;
    
    if (f & PIZ_SEQUENCE_FLAG_RANDOM) {
        h = 100 * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    }
          
    if ((h == -1) || (h < x->chance)) {
    //
    t = n->values[PIZ_VALUE_PITCH];
    t += x->tempValues[t % PIZ_MAGIC_SCALE];
    t = CLAMP (t, 0, PIZ_MAGIC_PITCH);
    
    if (n->values[PIZ_VALUE_PITCH] != t) {
        n->values[PIZ_VALUE_PITCH] = t;
        pizItemsetSetAtIndex (&x->changedNotes, n->tag);
    }
    //
    }
}

void pizSequenceEachDump (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n)
{
    long a[ ] = { n->position, 
                  n->values[PIZ_VALUE_PITCH],
                  n->values[PIZ_VALUE_VELOCITY],
                  n->values[PIZ_VALUE_DURATION], 
                  n->values[PIZ_VALUE_CHANNEL] };
                
    x->tempError |= pizSequenceAddNotification (x, PIZ_EVENT_NOTE_DUMPED, n->tag, 5, a);
}

void pizSequenceEachTempHash (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n)
{   
    long key = ((long)(n->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1)) + n->values[PIZ_VALUE_PITCH];
    x->tempError |= pizHashTableAdd (x->tempHash, key, (void *)n);
}

void pizSequenceEachTempNotes (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n)
{
    if (f & PIZ_SEQUENCE_FLAG_NEARBY) {
    //
    long value;
    
    if (e && (!(pizEventValue (e, &value)))) {
    //
    bool death = false;
    long j, a, b;
    long pitch = n->values[PIZ_VALUE_PITCH];
    
    value = MAX (0, value);
    
    a = CLAMP ((pitch - value), 0, PIZ_MAGIC_PITCH);
    b = CLAMP ((pitch + value), 0, PIZ_MAGIC_PITCH);
    
    for (j = a; j <= b; j++) {
        if (x->tempValues[j] == (n->position + 1)) {
            death = true;
        }
    }
    
    if (death) {
        x->tempNotes1[x->tempIndex] = n;
        x->tempIndex ++;
    } else {
        x->tempValues[pitch] = (n->position + 1);
    }
    //
    }
    //
    
    } else {
        x->tempNotes1[x->tempIndex] = n;
        x->tempIndex ++;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceFillTempNotes (PIZSequence *x)
{
    x->tempIndex = 0;
    pizSequenceForEach (x, NULL, PIZ_SEQUENCE_FLAG_NONE, pizSequenceEachTempNotes);
    
    return x->tempIndex;
}

void pizSequenceWithTempNotes (PIZSequence *x, long selector, bool reverse)
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
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceToAmbitus (PIZSequence *x, long pitch)
{
    if (pitch < x->down) {
        while ((pitch < x->down) && (pitch < PIZ_MAGIC_PITCH)) {
            pitch += PIZ_MAGIC_SCALE;
        }
    } else if (pitch > x->up) {
        while ((pitch > x->up) && (pitch > 0)) {
            pitch -= PIZ_MAGIC_SCALE;
        }
    }
    
    return (CLAMP (pitch, 0, PIZ_MAGIC_PITCH));
}

long pizSequenceToPattern (PIZSequence *x, long position)
{
    long s, j = (long)(position / (double)(ABS(x->cell)));
    
    if (s = pizArrayCount (x->pattern)) {
        j += pizArrayAtIndex (x->pattern, j % s);
    }

    return (j * ABS(x->cell));
}

long pizSequenceToCell (PIZSequence *x, long position)
{
    return (((long)((position / (double)(ABS(x->cell))) + 0.5)) * ABS(x->cell));
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
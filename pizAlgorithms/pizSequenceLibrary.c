/*
 * \file	pizSequenceLibrary.c
 * \author	Jean Sapristi
 * \date	May 12, 2012.
 */
 
/*
 *  Copyright (c) 2012, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequenceLibrary.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_TAG     pizItemset128SetAtIndex 
#define PIZ_UNTAG   pizItemset128UnsetAtIndex 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE PIZError pizSequenceGetTag (PIZSequence *x, long tag, long *ptr);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceForEach (PIZSequence *x, PIZMethod f, const PIZEvent *event)
{
    long i;
    
    for (i = 0; i < pizArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            (*f)(x, note, event);
            
            note = nextNote;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceRemoveNote (PIZSequence *x, PIZNote *note, const PIZEvent *event) 
{
    long p = note->position;
    long tag = note->tag;
    
    pizBoundedHashTableRemoveByKey (x->lookup, tag, note);
    pizItemset128UnsetAtIndex (&x->usedNotes, tag);
    pizLinklistRemoveByPtr (x->timeline[p], (void *)note);
    x->count --; 
    
    PIZ_TAG   (&x->removedNotes, tag);
    PIZ_UNTAG (&x->addedNotes, tag);
    PIZ_UNTAG (&x->changedNotes, tag);
}

void pizSequenceTransposeNote (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    long n;
    
    if (!(pizEventValue (event, &n))) {
        long temp = CLAMP (note->values[PIZ_VALUE_PITCH] + n, 0, PIZ_MAGIC_PITCH);
        if (note->values[PIZ_VALUE_PITCH] != temp) {
            note->values[PIZ_VALUE_PITCH] = temp;
            PIZ_TAG (&x->changedNotes, note->tag);
        }
    }
}

void pizSequenceFillTempHash (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{   /*
    long key, scale, offset = 0;
    
    if (scale = pizArrayCount (x->scale)) {
        offset = pizArrayValueAtIndex (x->scale, note->values[PIZ_VALUE_PITCH] % scale);
    }
    
    key = ((long)(note->position / (double)x->cell) * (PIZ_MAGIC_PITCH + 1));
    key += note->values[PIZ_VALUE_PITCH] + offset;
    
    x->tempError |= pizBoundedHashTableAdd (x->tempHash, key, (void *)note);*/
}

void pizSequenceFillTempNotes (PIZSequence *x, PIZNote *note, const PIZEvent *event)
{
    x->tempNotes1[x->tempIndex] = note;
    x->tempIndex ++;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZNote *pizSequenceNewNote (PIZSequence *x, long tag, long *argv, ulong flags)
{
    PIZNote *newNote = NULL;
    long    err      = PIZ_GOOD;
    long    k        = -1;
    long    position = argv[0];
    long    pitch    = argv[1];
    long    velocity = argv[2];
    long    duration = argv[3]; 
    long    channel  = argv[4]; 
    
    if (flags & PIZ_SEQUENCE_FLAG_SNAP) {
        position = pizSequenceSnapPositionToPattern (x, position);
    } 
    
    if (flags & PIZ_SEQUENCE_FLAG_AMBITUS) {
        pitch = pizSequenceMovePitchToAmbitus (x, pitch);
    }
                
    err |= (position < 0);
    err |= (position > (x->size - 1));
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
        newNote->values[PIZ_VALUE_DURATION] = CLAMP (duration, 0, PIZ_SEQUENCE_MAXIMUM_DURATION);
        newNote->values[PIZ_VALUE_CHANNEL]  = CLAMP (channel,  0, PIZ_MAGIC_CHANNEL);
    
        if (!(x->timeline[newNote->position])) {
            if (!(x->timeline[newNote->position] = pizLinklistNew ( ))) {
                err |= PIZ_MEMORY;
            }
        }
        
        err |= pizBoundedHashTableAdd (x->lookup, newNote->tag, newNote);
                                
        if (!err && !(pizLinklistInsert (x->timeline[newNote->position], (void *)newNote))) {
            x->count ++; 
            PIZ_TAG   (&x->addedNotes, newNote->tag);
            PIZ_UNTAG (&x->changedNotes, newNote->tag);
            
        } else {
            pizBoundedHashTableRemoveByKey (x->lookup, newNote->tag, newNote);
            pizItemset128UnsetAtIndex (&x->usedNotes, tag);
            free (newNote);
            newNote = NULL;
        }
    }
    //    
    }
        
    return newNote;
}   

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceMakeMap (PIZSequence *x)
{
    long i;
            
    pizArrayClear (x->map);
                            
    for (i = 0; i < x->size; i++) {
        if (x->timeline[i] && pizLinklistCount (x->timeline[i])) {
            pizArrayAppend (x->map, i);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long pizSequenceMovePitchToAmbitus (PIZSequence *x, long pitch)
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

long pizSequenceSnapPositionToPattern (PIZSequence *x, long position)
{
    long s, j = (long)(position / (double)x->cell);
    
    if (s = pizArrayCount (x->pattern)) {
        j += pizArrayValueAtIndex (x->pattern, j % s);
    }

    return (j * x->cell);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE PIZError pizSequenceGetTag (PIZSequence *x, long tag, long *ptr)
{
    long     i, k = -1;
    PIZError err = PIZ_ERROR;
    
    if ((tag >= 0) && (tag < PIZ_ITEMSET128_SIZE) && !(pizItemset128IsSetAtIndex (&x->usedNotes, tag))) {
        pizItemset128SetAtIndex (&x->usedNotes, tag);
        k = tag;
    } 
    
    if (k == -1) {
        for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
            if (!(pizItemset128IsSetAtIndex (&x->usedNotes, i))) {
                pizItemset128SetAtIndex (&x->usedNotes, i);
                k = i;
                break;
            }
        }
    }
    
    if (k != -1) {
        (*ptr) = k;
        err = PIZ_GOOD;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
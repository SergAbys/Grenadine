/*
 * \file    pizSequenceMaxMSP.c
 * \author  Jean Sapristi
 * \date    31 janvier 2012
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

#include "pizSequenceMaxMSP.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizSequenceEncodeSlotToArray (PIZSequence *x, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    pizSequenceCleanMap (x);
    
    if (a) {
        long i;
        
        err  = PIZ_GOOD;
        err |= pizGrowingArrayAppend (a, PIZ_SEQUENCE_VERSION);
        err |= pizGrowingArrayAppend (a, x->grid);
        err |= pizGrowingArrayAppend (a, x->noteValue);
        err |= pizGrowingArrayAppend (a, x->start);
        err |= pizGrowingArrayAppend (a, x->end);
        err |= pizGrowingArrayAppend (a, x->down);
        err |= pizGrowingArrayAppend (a, x->up);
        err |= pizGrowingArrayAppend (a, x->count);
        
        for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
            PIZNote *note       = NULL;
            PIZNote *nextNote   = NULL;
            
            long p = pizGrowingArrayValueAtIndex (x->map, i);
            
            pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
            
            while (note) {
                pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                
                err |= pizGrowingArrayAppend (a, note->originPosition);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_PITCH]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_VELOCITY]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_DURATION]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_CHANNEL]);
                err |= pizGrowingArrayAppend (a, note->isSelected);
                err |= pizGrowingArrayAppend (a, (note == x->markedNote));
                
                note = nextNote;
            }
        }
    }
    
    PIZUNLOCK
    
    return err;
}

PIZError pizSequenceDecodeSlotWithArray (PIZSequence *x, const PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    if (a) {
        long i, t;
        
        err = PIZ_GOOD;
        pizSequenceClearNotes (x);

        if (t = pizGrowingArrayCount (a)) {
            long k, version, count, grid, noteValue;
            long modeFlags = PIZ_SEQUENCE_ADD_FLAG_UNSELECT;
            long *ptr = pizGrowingArrayPtr (a);
            
            version     = pizGrowingArrayValueAtIndex (a, 0);
            grid        = pizGrowingArrayValueAtIndex (a, 1);
            noteValue   = pizGrowingArrayValueAtIndex (a, 2);
            
            switch (grid) {
                case PIZ_WHOLE_NOTE_DOTTED          : x->grid = PIZ_WHOLE_NOTE_DOTTED;          break;
                case PIZ_WHOLE_NOTE                 : x->grid = PIZ_WHOLE_NOTE;                 break;
                case PIZ_WHOLE_NOTE_TRIPLET         : x->grid = PIZ_WHOLE_NOTE_TRIPLET;         break;
                case PIZ_HALF_NOTE_DOTTED           : x->grid = PIZ_HALF_NOTE_DOTTED;           break;
                case PIZ_HALF_NOTE                  : x->grid = PIZ_HALF_NOTE;                  break;
                case PIZ_HALF_NOTE_TRIPLET          : x->grid = PIZ_HALF_NOTE_TRIPLET;          break;
                case PIZ_QUARTER_NOTE_DOTTED        : x->grid = PIZ_QUARTER_NOTE_DOTTED;        break;
                case PIZ_QUARTER_NOTE               : x->grid = PIZ_QUARTER_NOTE;               break;
                case PIZ_QUARTER_NOTE_TRIPLET       : x->grid = PIZ_QUARTER_NOTE_TRIPLET;       break;
                case PIZ_EIGHTH_NOTE_DOTTED         : x->grid = PIZ_EIGHTH_NOTE_DOTTED;         break;
                case PIZ_EIGHTH_NOTE                : x->grid = PIZ_EIGHTH_NOTE;                break;
                case PIZ_EIGHTH_NOTE_TRIPLET        : x->grid = PIZ_EIGHTH_NOTE_TRIPLET;        break;
                case PIZ_SIXTEENTH_NOTE_DOTTED      : x->grid = PIZ_SIXTEENTH_NOTE_DOTTED;      break;
                case PIZ_SIXTEENTH_NOTE             : x->grid = PIZ_SIXTEENTH_NOTE;             break;
                case PIZ_SIXTEENTH_NOTE_TRIPLET     : x->grid = PIZ_SIXTEENTH_NOTE_TRIPLET;     break;
                case PIZ_THIRTY_SECOND_NOTE         : x->grid = PIZ_THIRTY_SECOND_NOTE;         break;
                case PIZ_THIRTY_SECOND_NOTE_TRIPLET : x->grid = PIZ_THIRTY_SECOND_NOTE_TRIPLET; break;
                case PIZ_SNAP_NONE                  : x->grid = PIZ_SNAP_NONE;                  break;
            }
            
            switch (noteValue) {
                case PIZ_WHOLE_NOTE_DOTTED          : x->noteValue = PIZ_WHOLE_NOTE_DOTTED;         break;
                case PIZ_WHOLE_NOTE                 : x->noteValue = PIZ_WHOLE_NOTE;                break;
                case PIZ_WHOLE_NOTE_TRIPLET         : x->noteValue = PIZ_WHOLE_NOTE_TRIPLET;        break;
                case PIZ_HALF_NOTE_DOTTED           : x->noteValue = PIZ_HALF_NOTE_DOTTED;          break;
                case PIZ_HALF_NOTE                  : x->noteValue = PIZ_HALF_NOTE;                 break;
                case PIZ_HALF_NOTE_TRIPLET          : x->noteValue = PIZ_HALF_NOTE_TRIPLET;         break;
                case PIZ_QUARTER_NOTE_DOTTED        : x->noteValue = PIZ_QUARTER_NOTE_DOTTED;       break;
                case PIZ_QUARTER_NOTE               : x->noteValue = PIZ_QUARTER_NOTE;              break;
                case PIZ_QUARTER_NOTE_TRIPLET       : x->noteValue = PIZ_QUARTER_NOTE_TRIPLET;      break;
                case PIZ_EIGHTH_NOTE_DOTTED         : x->noteValue = PIZ_EIGHTH_NOTE_DOTTED;        break;
                case PIZ_EIGHTH_NOTE                : x->noteValue = PIZ_EIGHTH_NOTE;               break;
                case PIZ_EIGHTH_NOTE_TRIPLET        : x->noteValue = PIZ_EIGHTH_NOTE_TRIPLET;       break;
                case PIZ_SIXTEENTH_NOTE_DOTTED      : x->noteValue = PIZ_SIXTEENTH_NOTE_DOTTED;     break;
                case PIZ_SIXTEENTH_NOTE             : x->noteValue = PIZ_SIXTEENTH_NOTE;            break;
                case PIZ_SIXTEENTH_NOTE_TRIPLET     : x->noteValue = PIZ_SIXTEENTH_NOTE_TRIPLET;    break;
                case PIZ_THIRTY_SECOND_NOTE         : x->noteValue = PIZ_THIRTY_SECOND_NOTE;        break;
                case PIZ_THIRTY_SECOND_NOTE_TRIPLET : x->noteValue = PIZ_THIRTY_SECOND_NOTE_TRIPLET;break;
                case PIZ_SNAP_NONE                  : x->noteValue = PIZ_SNAP_NONE;                 break;
            }

            x->start    = pizGrowingArrayValueAtIndex (a, 3);
            x->end      = pizGrowingArrayValueAtIndex (a, 4);
            x->down     = pizGrowingArrayValueAtIndex (a, 5);
            x->up       = pizGrowingArrayValueAtIndex (a, 6);
            count       = pizGrowingArrayValueAtIndex (a, 7);
            
            k = 8;
            
            for (i = 0; i < count; i++) {
                if (!(pizSequenceAddNote (x, ptr + (i * PIZ_SEQUENCE_NOTE_SIZE) + k, modeFlags))) {
                        err |= PIZ_ERROR;
                }
            }
        }
    }

    PIZUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceEncodeUndoToArray (PIZSequence *x, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    pizSequenceCleanMap (x);
    
    if (a) {
        long i;
        
        err  = PIZ_GOOD;
        err |= pizGrowingArrayAppend (a, PIZ_SEQUENCE_VERSION);
        err |= pizGrowingArrayAppend (a, x->start);
        err |= pizGrowingArrayAppend (a, x->end);
        err |= pizGrowingArrayAppend (a, x->down);
        err |= pizGrowingArrayAppend (a, x->up);
        err |= pizGrowingArrayAppend (a, x->count);
        
        for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
            PIZNote *note       = NULL;
            PIZNote *nextNote   = NULL;
            
            long p = pizGrowingArrayValueAtIndex (x->map, i);
            
            pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
            
            while (note) {
                pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                
                err |= pizGrowingArrayAppend (a, note->originPosition);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_PITCH]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_VELOCITY]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_DURATION]);
                err |= pizGrowingArrayAppend (a, note->data[PIZ_CHANNEL]);
                err |= pizGrowingArrayAppend (a, note->isSelected);
                err |= pizGrowingArrayAppend (a, (note == x->markedNote));
                
                note = nextNote;
            }
        }
    }
    
    PIZUNLOCK
    
    return err;
}

PIZError pizSequenceDecodeUndoWithArray (PIZSequence *x, const PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    if (a) {
        long i, t;
        
        err = PIZ_GOOD;
        pizSequenceClearNotes (x);

        if (t = pizGrowingArrayCount (a)) {
            long k, version, count;
            long *ptr = pizGrowingArrayPtr (a);
            
            version     = pizGrowingArrayValueAtIndex (a, 0);
            x->start    = pizGrowingArrayValueAtIndex (a, 1);
            x->end      = pizGrowingArrayValueAtIndex (a, 2);
            x->down     = pizGrowingArrayValueAtIndex (a, 3);
            x->up       = pizGrowingArrayValueAtIndex (a, 4);
            count       = pizGrowingArrayValueAtIndex (a, 5);
            
            k = 6;
            
            for (i = 0; i < count; i++) {
                if (!(pizSequenceAddNote (x, ptr + (i * PIZ_SEQUENCE_NOTE_SIZE) + k, 
                    PIZ_SEQUENCE_ADD_FLAG_UNSELECT))) {
                        err |= PIZ_ERROR;
                    }
            }
        }
    }

    PIZUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceUndoIsEqualToUndo (const PIZGrowingArray *a, const PIZGrowingArray *b)
{
    bool isEqual = false;
    long count = pizGrowingArrayCount (a);
            
    if (count == pizGrowingArrayCount (b)) {
        long i, n, k;
        long *ptrA = pizGrowingArrayPtr (a);
        long *ptrB = pizGrowingArrayPtr (b);
        bool d = false;
    
        d |= (*(ptrA + 1) != *(ptrB + 1));
        d |= (*(ptrA + 2) != *(ptrB + 2));
        d |= (*(ptrA + 3) != *(ptrB + 3));
        d |= (*(ptrA + 4) != *(ptrB + 4));
        
        n = *(ptrA + 5);
        
        k = 6;
        
        for (i = 0; i < n; i++) {
            d |= (*(ptrA + k + PIZ_SEQUENCE_POSITION)   != *(ptrB + k + PIZ_SEQUENCE_POSITION));
            d |= (*(ptrA + k + PIZ_SEQUENCE_PITCH)      != *(ptrB + k + PIZ_SEQUENCE_PITCH));
            d |= (*(ptrA + k + PIZ_SEQUENCE_VELOCITY)   != *(ptrB + k + PIZ_SEQUENCE_VELOCITY));
            d |= (*(ptrA + k + PIZ_SEQUENCE_DURATION)   != *(ptrB + k + PIZ_SEQUENCE_DURATION));
            d |= (*(ptrA + k + PIZ_SEQUENCE_CHANNEL)    != *(ptrB + k + PIZ_SEQUENCE_CHANNEL));
            
            k += PIZ_SEQUENCE_NOTE_SIZE;
            
            if (d) {
                    break;
                }
        }
        
        isEqual = !d;
    }
        
    return isEqual;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
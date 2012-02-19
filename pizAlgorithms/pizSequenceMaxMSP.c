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

#define PIZ_SLOT_VERSION        0
#define PIZ_SLOT_GRID           1
#define PIZ_SLOT_NOTE_VALUE     2
#define PIZ_SLOT_START          3
#define PIZ_SLOT_END            4
#define PIZ_SLOT_DOWN           5
#define PIZ_SLOT_UP             6
#define PIZ_SLOT_COUNT          7
#define PIZ_SLOT_DATA           8

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizSequenceEncodeToArray (PIZSequence *x, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    if (a) {
        long i;
        
        err  = PIZ_GOOD;
        err |= pizGrowingArrayAppend (a, PIZ_SEQUENCE_VERSION_MAJOR);
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
                
                err |= pizGrowingArrayAppend (a, note->position);
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

PIZError pizSequenceDecodeWithArray (PIZSequence *x, const PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    if (a) {
        long i, t;
        
        err = PIZ_GOOD;
        pizSequenceRemoveAllNotes (x);

        if (t = pizGrowingArrayCount (a)) {
            long k, version, count, grid, noteValue;
            long modeFlags = PIZ_ADD_FLAG_UNSELECT;
            long *ptr = pizGrowingArrayPtr (a);
            
            version     = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_VERSION);
            grid        = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_GRID);
            noteValue   = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_NOTE_VALUE);
            
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
                case PIZ_NOTE_NONE                  : x->grid = PIZ_NOTE_NONE;                  break;
            }
            
            switch (noteValue) {
                case PIZ_WHOLE_NOTE_DOTTED          : x->noteValue = PIZ_WHOLE_NOTE_DOTTED;             break;
                case PIZ_WHOLE_NOTE                 : x->noteValue = PIZ_WHOLE_NOTE;                    break;
                case PIZ_WHOLE_NOTE_TRIPLET         : x->noteValue = PIZ_WHOLE_NOTE_TRIPLET;            break;
                case PIZ_HALF_NOTE_DOTTED           : x->noteValue = PIZ_HALF_NOTE_DOTTED;              break;
                case PIZ_HALF_NOTE                  : x->noteValue = PIZ_HALF_NOTE;                     break;
                case PIZ_HALF_NOTE_TRIPLET          : x->noteValue = PIZ_HALF_NOTE_TRIPLET;             break;
                case PIZ_QUARTER_NOTE_DOTTED        : x->noteValue = PIZ_QUARTER_NOTE_DOTTED;           break;
                case PIZ_QUARTER_NOTE               : x->noteValue = PIZ_QUARTER_NOTE;                  break;
                case PIZ_QUARTER_NOTE_TRIPLET       : x->noteValue = PIZ_QUARTER_NOTE_TRIPLET;          break;
                case PIZ_EIGHTH_NOTE_DOTTED         : x->noteValue = PIZ_EIGHTH_NOTE_DOTTED;            break;
                case PIZ_EIGHTH_NOTE                : x->noteValue = PIZ_EIGHTH_NOTE;                   break;
                case PIZ_EIGHTH_NOTE_TRIPLET        : x->noteValue = PIZ_EIGHTH_NOTE_TRIPLET;           break;
                case PIZ_SIXTEENTH_NOTE_DOTTED      : x->noteValue = PIZ_SIXTEENTH_NOTE_DOTTED;         break;
                case PIZ_SIXTEENTH_NOTE             : x->noteValue = PIZ_SIXTEENTH_NOTE;                break;
                case PIZ_SIXTEENTH_NOTE_TRIPLET     : x->noteValue = PIZ_SIXTEENTH_NOTE_TRIPLET;        break;
                case PIZ_THIRTY_SECOND_NOTE         : x->noteValue = PIZ_THIRTY_SECOND_NOTE;            break;
                case PIZ_THIRTY_SECOND_NOTE_TRIPLET : x->noteValue = PIZ_THIRTY_SECOND_NOTE_TRIPLET;    break;
                case PIZ_NOTE_NONE                  : x->noteValue = PIZ_NOTE_NONE;                     break;
            }

            x->start    = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_START);
            x->end      = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_END);
            x->down     = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_DOWN);
            x->up       = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_UP);
            count       = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_COUNT);
            
            k = PIZ_SLOT_DATA;
            
            for (i = 0; i < count; i++) {
                if (!(pizSequenceAddNote (x, ptr + (i * PIZ_DATA_NOTE_SIZE) + k, modeFlags))) {
                    err |= PIZ_ERROR;
                }
            }
            
            if (count) {
                pizSequenceMakeMap (x);
            }
        }
    }

    PIZUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
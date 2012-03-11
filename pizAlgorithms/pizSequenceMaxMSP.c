/*
 * \file    pizSequenceMaxMSP.c
 * \author  Jean Sapristi
 * \date    March 11, 2012.
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

static long piz_start        = PIZ_DEFAULT_START;
static long piz_end          = PIZ_DEFAULT_END;
static long piz_down         = PIZ_DEFAULT_DOWN;
static long piz_up           = PIZ_DEFAULT_UP;
static long piz_originStart  = PIZ_DEFAULT_START;
static long piz_originDown   = PIZ_DEFAULT_DOWN;
static long piz_originWidth  = PIZ_DEFAULT_END - PIZ_DEFAULT_START;
static long piz_originHeight = PIZ_DEFAULT_UP  - PIZ_DEFAULT_DOWN;
                
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceInitTempZone (PIZSequence *x)
{
    PIZLOCK
    
    piz_start         = x->start;
    piz_end           = x->end;
    piz_down          = x->down;
    piz_up            = x->up;
    piz_originStart   = x->start;
    piz_originDown    = x->down;
    piz_originWidth   = x->end - x->start;
    piz_originHeight  = x->up  - x->down;
    
    PIZUNLOCK
}

void pizSequencePutTempZone (PIZSequence *x)
{
    PIZLOCK
    
    if (!(piz_start == piz_end)) {
        if (piz_end < piz_start) {
            long k    = piz_end;
            piz_end   = piz_start;
            piz_start = k;
        }
        
        if (piz_up < piz_down) {
            long k   = piz_up;
            piz_up   = piz_down;
            piz_down = k;
        }
        
        x->start = piz_start;
        x->end   = piz_end;
        x->down  = piz_down;
        x->up    = piz_up;
        
        x->changedZone = true;
    }
        
    PIZUNLOCK
}

bool pizSequenceResizeTempZone (PIZSequence *x, const PIZCoordinates *c, PIZDataIndex side)
{
    long temp;
    bool haveChanged = false;
    
    PIZLOCK
    
    switch (side) {
    case PIZ_DATA_START : temp = CLAMP (pizSequenceSnapRound (x, c->position), 0, x->timelineSize);
                          if (piz_start != temp) { 
                          piz_start = temp; haveChanged = true; 
                          } break;
    case PIZ_DATA_END   : temp = CLAMP (pizSequenceSnapRound (x, c->position), 0, x->timelineSize); 
                          if (piz_end != temp) { 
                          piz_end = temp; haveChanged = true; 
                          } break;
    case PIZ_DATA_DOWN  : if (c->pitch <= piz_up) { 
                          temp = CLAMP (c->pitch, 0, PIZ_MAGIC_PITCH); 
                          } 
                          if (piz_down != temp) { 
                          piz_down = temp; haveChanged = true; 
                          } break;
    case PIZ_DATA_UP    : if (c->pitch >= piz_down) { 
                          temp = CLAMP (c->pitch, 0, PIZ_MAGIC_PITCH); 
                          } 
                          if (piz_up != temp) { 
                          piz_up = temp; haveChanged = true; 
                          } break;
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceMoveTempZone (PIZSequence *x, long pitch, long position)
{
    bool haveChanged = false;
    long tempStart, tempDown;
    
    PIZLOCK
    
    tempStart = pizSequenceSnapRound (x, piz_originStart + position);
    tempStart = CLAMP (tempStart, 0, (x->timelineSize - piz_originWidth));
    tempDown  = CLAMP (piz_originDown + pitch, 0, (PIZ_MAGIC_PITCH - piz_originHeight));
    
    if ((tempStart != piz_start) || (tempDown != piz_down)) {
        piz_start = tempStart;
        piz_end   = tempStart + piz_originWidth;
        piz_down  = tempDown;
        piz_up    = tempDown + piz_originHeight;
        
        haveChanged = true;
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceTempZoneToArray (PIZSequence *x, PIZGrowingArray *a)
{
    PIZLOCK
    
    if (a) {
        pizGrowingArrayAppend (a, piz_start);
        pizGrowingArrayAppend (a, piz_end);
        pizGrowingArrayAppend (a, piz_down);
        pizGrowingArrayAppend (a, piz_up);
    }
        
    PIZUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceSelectAllNotes (PIZSequence *x)
{
    long i;
    
    PIZLOCK
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            if (!note->isSelected) {
                note->isSelected = true;
                PIZ_TAG (note->tag);
            }
            note = nextNote;
        }
    }
    
    PIZUNLOCK
}

void pizSequenceUnselectAllNotes (PIZSequence *x)
{
    PIZLOCK
    
    pizSequenceUnselectNotes (x);
    
    PIZUNLOCK
}

long pizSequenceSelectNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c)
{
    long i, count;
    long k = 0;

    PIZLOCK
    
    count = pizGrowingArrayCount (x->map);
        
    for (i = (count - 1); i >= 0; i--) {
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note && !k) {
            long err = PIZ_GOOD;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            err |= (c->position < p);
            err |= (c->position > (p + note->data[PIZ_DURATION]));
            err |= (c->pitch != note->data[PIZ_PITCH]);
            
            if (!err) {
            
                if (!note->isSelected) {
                    pizSequenceUnselectNotes (x);
                    if (!note->isSelected) {
                        note->isSelected = true;
                        PIZ_TAG (note->tag);
                    }
                    x->markedNote = note;
                }
                
                k = 1;
            }
            
            note = nextNote;
        }
        
        if (k) { 
            break;
        }
    }
    
    PIZUNLOCK
    
    return k;
}

long pizSequenceInvertNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c)
{
    long i, count;
    long k = -1;
    
    PIZLOCK
    
    count = pizGrowingArrayCount (x->map);
        
    for (i = (count - 1); i >= 0; i--) {
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long err = PIZ_GOOD;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            err |= (c->position < p);
            err |= (c->position > (p + note->data[PIZ_DURATION]));
            err |= (c->pitch != note->data[PIZ_PITCH]);
            
            if (!err) {
                if (note->isSelected) {
                    if (x->markedNote == note) {
                        x->markedNote = NULL;
                    }
                            
                    note->isSelected = false;
                    k = 0;
                } else {
                    note->isSelected = true;
                    k = 1;
                }
                PIZ_TAG (note->tag);
                break;
            }
            
            note = nextNote;
        }
        
        if (k != -1) {
            break;
        }
    }
    
    PIZUNLOCK
    
    return k;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceInitLasso (PIZSequence *x)
{
    long i;
    
    PIZLOCK
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            note->flags &= ~PIZ_NOTE_FLAG_LASSO;
            note = nextNote;
        }
    }
    
    PIZUNLOCK
}

long pizSequenceDragLasso (PIZSequence *x, const PIZCoordinates *m, const PIZCoordinates *n, bool reverse)
{
    long i, a, b, u, v;
    long k = 0;
    
    PIZLOCK
    
    a = MIN (m->position,  n->position);
    b = MIN (m->pitch,     n->pitch);
    u = MAX (m->position,  n->position);
    v = MAX (m->pitch,     n->pitch);
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            long err, end;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            err = PIZ_GOOD;
            end = p + note->data[PIZ_DURATION];
            
            err |= !(note->data[PIZ_PITCH] >= b && note->data[PIZ_PITCH] <= v);
            err |= !((p >= a && p <= u) || ((end >= a) && (end <= u)));
                
            if (!err) {
                if (reverse) {
                    if (!(note->flags & PIZ_NOTE_FLAG_LASSO)) {
                        note->isSelected = !note->isSelected;
                        PIZ_TAG (note->tag);
                        note->flags |= PIZ_NOTE_FLAG_LASSO;
                        k = 1;
                            
                        if (note == x->markedNote) {
                            x->markedNote = NULL;
                        }
                    }
                } else if (!note->isSelected) {
                    note->isSelected = true;
                    PIZ_TAG (note->tag);
                    k = 1;
                }
            } else {
                if (reverse) {
                    if (note->flags & PIZ_NOTE_FLAG_LASSO) {
                        note->isSelected = !note->isSelected;
                        PIZ_TAG (note->tag);
                        note->flags &= ~PIZ_NOTE_FLAG_LASSO;
                        k = 1;
                    }
                } else if (note->isSelected)  {
                    note->isSelected = false;
                    PIZ_TAG (note->tag);
                    k = 1;
                }
            }
            
            note = nextNote;
        }
    }

    PIZUNLOCK
    
    return k;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceRemoveSelectedNotes (PIZSequence *x)
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
            
            if (note->isSelected) {
                pizSequenceRemoveNote (x, note);
                haveChanged = true;
            }
                
            note = nextNote;
        }
    }
    
    if (haveChanged) {
        pizSequenceMakeMap (x);
    }
    
    PIZUNLOCK 
}

void pizSequenceAddNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c, long flags)
{
    long values[PIZ_DATA_NOTE_SIZE];
    
    PIZLOCK
        
    values[PIZ_DATA_POSITION]    = c->position;
    values[PIZ_DATA_PITCH]       = c->pitch;
    values[PIZ_DATA_VELOCITY]    = PIZ_DEFAULT_VELOCITY;
    values[PIZ_DATA_CHANNEL]     = PIZ_CHANNEL_NONE;
    values[PIZ_DATA_IS_SELECTED] = true;
    values[PIZ_DATA_IS_MARKED]   = true;

    if (x->noteValue != PIZ_NOTE_NONE) {
        values[PIZ_DATA_DURATION] = x->noteValue;
    } else {
        values[PIZ_DATA_DURATION] = x->grid;
    }
    
    if (pizSequenceAddNote (x, values, flags)) {
        pizSequenceMakeMap (x);
    }
                    
    PIZUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

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
    //
    long i, t;
    
    err = PIZ_GOOD;
    pizSequenceRemoveAllNotes (x);

    if (t = pizGrowingArrayCount (a)) {
    //
    long k, version, count, grid, noteValue;
    long modeFlags = PIZ_ADD_FLAG_UNSELECT;
    long *ptr = pizGrowingArrayPtr (a);
    
    version     = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_VERSION);
    grid        = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_GRID);
    noteValue   = pizGrowingArrayValueAtIndex (a, PIZ_SLOT_NOTE_VALUE);
    
    switch (grid) {
    case PIZ_WHOLE_NOTE_DOTTED          : x->cell = (x->grid = PIZ_WHOLE_NOTE_DOTTED);          break;
    case PIZ_WHOLE_NOTE                 : x->cell = (x->grid = PIZ_WHOLE_NOTE);                 break;
    case PIZ_WHOLE_NOTE_TRIPLET         : x->cell = (x->grid = PIZ_WHOLE_NOTE_TRIPLET);         break;
    case PIZ_HALF_NOTE_DOTTED           : x->cell = (x->grid = PIZ_HALF_NOTE_DOTTED);           break;
    case PIZ_HALF_NOTE                  : x->cell = (x->grid = PIZ_HALF_NOTE);                  break;
    case PIZ_HALF_NOTE_TRIPLET          : x->cell = (x->grid = PIZ_HALF_NOTE_TRIPLET);          break;
    case PIZ_QUARTER_NOTE_DOTTED        : x->cell = (x->grid = PIZ_QUARTER_NOTE_DOTTED);        break;
    case PIZ_QUARTER_NOTE               : x->cell = (x->grid = PIZ_QUARTER_NOTE);               break;
    case PIZ_QUARTER_NOTE_TRIPLET       : x->cell = (x->grid = PIZ_QUARTER_NOTE_TRIPLET);       break;
    case PIZ_EIGHTH_NOTE_DOTTED         : x->cell = (x->grid = PIZ_EIGHTH_NOTE_DOTTED);         break;
    case PIZ_EIGHTH_NOTE                : x->cell = (x->grid = PIZ_EIGHTH_NOTE);                break;
    case PIZ_EIGHTH_NOTE_TRIPLET        : x->cell = (x->grid = PIZ_EIGHTH_NOTE_TRIPLET);        break;
    case PIZ_SIXTEENTH_NOTE_DOTTED      : x->cell = (x->grid = PIZ_SIXTEENTH_NOTE_DOTTED);      break;
    case PIZ_SIXTEENTH_NOTE             : x->cell = (x->grid = PIZ_SIXTEENTH_NOTE);             break;
    case PIZ_SIXTEENTH_NOTE_TRIPLET     : x->cell = (x->grid = PIZ_SIXTEENTH_NOTE_TRIPLET);     break;
    case PIZ_THIRTY_SECOND_NOTE         : x->cell = (x->grid = PIZ_THIRTY_SECOND_NOTE);         break;
    case PIZ_THIRTY_SECOND_NOTE_TRIPLET : x->cell = (x->grid = PIZ_THIRTY_SECOND_NOTE_TRIPLET); break;
    case PIZ_NOTE_NONE                  : x->cell = (x->grid = PIZ_NOTE_NONE);                  break;
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
    //    
    }
    //    
    }

    PIZUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE void pizSequenceUnselectNotes (PIZSequence *x)
{
    long i;
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            if (note->isSelected) {
                note->isSelected = false;
                PIZ_TAG (note->tag);
            }
            note = nextNote;
        }
    }
    
    x->markedNote = NULL;
}

PIZ_INLINE long pizSequenceSnapRound (PIZSequence *x, long toBeSnapped)
{
    return MAX (((long)(((toBeSnapped / (double)x->grid) + 0.5)) * x->grid), 0);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
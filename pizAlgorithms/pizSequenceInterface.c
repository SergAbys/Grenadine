/*
 * \file    pizSequenceInterface.c
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

#include "pizSequenceInterface.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceHasMarkedNote (PIZSequence *x)
{
    bool k;
    
    PIZLOCK
    k = (x->markedNote != NULL);
    PIZUNLOCK;
    
    return k;
}

long pizSequenceMarkedNoteValue (PIZSequence *x, PIZSelector selector)
{
    long k = -1;
    
    PIZLOCK
    
    if (x->markedNote) {
        if (selector == PIZ_CHANNEL) {
            if (x->markedNote->data[PIZ_CHANNEL] == PIZ_CHANNEL_NONE) {
                k = x->channel;
            } else {
                k = x->markedNote->data[PIZ_CHANNEL];
            }
        } else {
            k = x->markedNote->data[selector];
        }
    }
        
    PIZUNLOCK
    
    return k;
}

void pizSequenceChangeMarkedNoteValue (PIZSequence *x, PIZSelector selector, long value)
{
    PIZLOCK
    
    if (x->markedNote)  {
        if (selector == PIZ_DURATION) {
            long err = PIZ_GOOD;
            long temp = x->markedNote->data[PIZ_DURATION];
            
            if (value > 0) {
                temp += x->grid;
            } else {
                temp -= x->grid;
            }
                
            temp = pizSequenceSnapRound (x, temp);
            
            err |= ((x->markedNote->position + temp) > PIZ_SEQUENCE_TIMELINE_SIZE);
            err |= (temp > PIZ_SEQUENCE_MAXIMUM_DURATION);
            err |= (temp <= 0);
            
            if (!err) {
                x->markedNote->data[PIZ_DURATION] = temp;
            }
        } else {
            x->markedNote->data[selector] += value;
            
            switch (selector) {
            case PIZ_PITCH    : x->markedNote->data[PIZ_PITCH] = CLAMP (x->markedNote->data[PIZ_PITCH], 
                                0, PIZ_MAGIC_PITCH); break;
            case PIZ_VELOCITY : x->markedNote->data[PIZ_VELOCITY] = CLAMP (x->markedNote->data[PIZ_VELOCITY], 
                                0, PIZ_MAGIC_VELOCITY); break;
            case PIZ_CHANNEL  : x->markedNote->data[PIZ_CHANNEL] = CLAMP (x->markedNote->data[PIZ_CHANNEL], 
                                0, PIZ_MAGIC_CHANNEL); break;
            }
        }
    }
        
    PIZUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceSetTempZoneByZone (PIZSequence *x)
{
    PIZLOCK
    
    x->tempStart         = x->start;
    x->tempEnd           = x->end;
    x->tempDown          = x->down;
    x->tempUp            = x->up;
    x->tempOriginStart   = x->start;
    x->tempOriginDown    = x->down;
    x->tempOriginWidth   = (x->end - x->start);
    x->tempOriginHeight  = (x->up - x->down);
    
    PIZUNLOCK
}

bool pizSequenceSetTempZoneWithCoordinates (PIZSequence *x, const PIZCoordinates *coordinates, long side)
{
    long tempValue = -1;
    bool haveChanged = false;
    
    PIZLOCK
    
    switch (side) {
        case PIZ_DATA_START :   tempValue = CLAMP (pizSequenceSnapRound (x, coordinates->position), 
                                    0, PIZ_SEQUENCE_TIMELINE_SIZE); break;
        case PIZ_DATA_END   :   tempValue = CLAMP (pizSequenceSnapRound (x, coordinates->position), 
                                    0, PIZ_SEQUENCE_TIMELINE_SIZE); break;
        case PIZ_DATA_DOWN  :   if (coordinates->pitch <= x->tempUp) {
                                        tempValue = CLAMP (coordinates->pitch, 0, PIZ_MAGIC_PITCH);
                                    } break;
        case PIZ_DATA_UP    :   if (coordinates->pitch >= x->tempDown) {
                                        tempValue = CLAMP (coordinates->pitch, 0, PIZ_MAGIC_PITCH);
                                    } break;
    }
    
    if (tempValue != -1) {
        if ((side == PIZ_DATA_START) && (x->tempStart != tempValue)) {
            x->tempStart = tempValue;
            haveChanged = true;
        } else if ((side == PIZ_DATA_END) && (x->tempEnd != tempValue)) {
            x->tempEnd = tempValue;
            haveChanged = true;
        } else if ((side == PIZ_DATA_DOWN) && (x->tempDown != tempValue)) {
            x->tempDown = tempValue;
            haveChanged = true;
        } else if ((side == PIZ_DATA_UP) && (x->tempUp != tempValue)) {
            x->tempUp = tempValue;
            haveChanged = true;
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceMoveTempZoneWithDelta (PIZSequence *x, long pitch, long position)
{
    bool haveChanged = false;
    long tempStart, tempDown;
    
    PIZLOCK
    
    tempStart = CLAMP (pizSequenceSnapRound (x, x->tempOriginStart + position), 0, 
                    (PIZ_SEQUENCE_TIMELINE_SIZE - x->tempOriginWidth));
    tempDown  = CLAMP (x->tempOriginDown + pitch, 0, (PIZ_MAGIC_PITCH - x->tempOriginHeight));
    
    if ((tempStart != x->tempStart) || (tempDown != x->tempDown)) {
        x->tempStart = tempStart;
        x->tempEnd   = tempStart + x->tempOriginWidth;
        x->tempDown  = tempDown;
        x->tempUp    = tempDown + x->tempOriginHeight;
        
        haveChanged = true;
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

PIZError pizSequenceTempZoneToArray (PIZSequence *x, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;

    PIZLOCK
    
    if (a) {
            err = PIZ_GOOD;
            
            err |= pizGrowingArrayAppend (a, x->tempStart);
            err |= pizGrowingArrayAppend (a, x->tempEnd);
            err |= pizGrowingArrayAppend (a, x->tempDown);
            err |= pizGrowingArrayAppend (a, x->tempUp);
        }
        
    PIZUNLOCK
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceSetZoneByTempZone (PIZSequence *x)
{
    long err = PIZ_ERROR;
    
    PIZLOCK
    
    if (!(x->tempStart == x->tempEnd)) {
    
        if (x->tempEnd < x->tempStart) {
            long k          = x->tempEnd;
            x->tempEnd      = x->tempStart;
            x->tempStart    = k;
        }
        
        if (x->tempUp < x->tempDown) {
            long k      = x->tempUp;
            x->tempUp   = x->tempDown;
            x->tempDown = k;
        }
        
        x->start = x->tempStart;
        x->end   = x->tempEnd;
        x->down  = x->tempDown;
        x->up    = x->tempUp;
        
        err = PIZ_GOOD;
    }
        
    PIZUNLOCK
    
    return err;
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
                note->isSelected = true;
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

long pizSequenceSelectNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *coordinates)
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
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (((coordinates->position >= p) && (coordinates->position <= (p + note->data[PIZ_DURATION]))) 
                && (coordinates->pitch == note->data[PIZ_PITCH])) {
                    if (!note->isSelected) {
                        pizSequenceUnselectNotes (x);
                        note->isSelected = true;
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

long pizSequenceInvertNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *coordinates)
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
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if (((coordinates->position >= p) && (coordinates->position <= (p + note->data[PIZ_DURATION]))) 
                && (coordinates->pitch == note->data[PIZ_PITCH])) {
                
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
                    
                    break;
                }
            
            note = nextNote;
        }
        
        if (k != -1)  {
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

long pizSequenceSelectNotesWithLasso (PIZSequence *x, const PIZCoordinates *m, const PIZCoordinates *n, bool r)
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
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            if ((note->data[PIZ_PITCH] >= b && note->data[PIZ_PITCH] <= v) && ((p >= a && p <= u) || 
                (((p + note->data[PIZ_DURATION]) >= a) && ((p + note->data[PIZ_DURATION]) <= u)))) {
                if (r) {
                    if (!(note->flags & PIZ_NOTE_FLAG_LASSO)) {
                        note->isSelected = !note->isSelected;
                        note->flags |= PIZ_NOTE_FLAG_LASSO;
                        k = 1;
                            
                        if (note == x->markedNote) {
                            x->markedNote = NULL;
                        }
                    }
                } else if (!note->isSelected) {
                    note->isSelected = true;
                    k = 1;
                }
            } else {
                if (r) {
                    if (note->flags & PIZ_NOTE_FLAG_LASSO) {
                        note->isSelected = !note->isSelected;
                        note->flags &= ~PIZ_NOTE_FLAG_LASSO;
                        k = 1;
                    }
                } else if (note->isSelected)  {
                    note->isSelected = false;
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

PIZError pizSequenceRemoveSelectedNotes (PIZSequence *x)
{
    long i;
    long err = PIZ_GOOD;
    
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
            }
                
            note = nextNote;
        }
    }
    
    pizSequenceMakeMap (x);
    
    PIZUNLOCK
    
    return err; 
}

PIZError pizSequenceAddNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *coordinates, long flags)
{
    long err = PIZ_GOOD;
    long values[PIZ_DATA_NOTE_SIZE];
    
    PIZLOCK
        
    values[PIZ_DATA_POSITION]       = coordinates->position;
    values[PIZ_DATA_PITCH]          = coordinates->pitch;
    values[PIZ_DATA_VELOCITY]       = PIZ_DEFAULT_VELOCITY;
    values[PIZ_DATA_CHANNEL]        = PIZ_CHANNEL_NONE;
    values[PIZ_DATA_IS_SELECTED]    = true;
    values[PIZ_DATA_IS_MARKED]      = true;

    if (x->noteValue != PIZ_NOTE_NONE) {
        values[PIZ_DATA_DURATION] = x->noteValue;
    } else {
        values[PIZ_DATA_DURATION] = x->grid;
    }
    
    if (!(pizSequenceAddNote (x, values, flags))) {
        err |= PIZ_ERROR;
    } else {
        pizSequenceMakeMap (x);
    }
                    
    PIZUNLOCK
    
    return err;
}

void pizSequenceTransposeOctave (PIZSequence *x, bool down)
{
    long i, step;
    
    if (down) {
        step = -PIZ_MAGIC_SCALE;
    } else {
        step = PIZ_MAGIC_SCALE;
    }
        
    PIZLOCK
        
    x->down = CLAMP (x->down + step, 0, PIZ_MAGIC_PITCH);
    x->up   = CLAMP (x->up + step, 0, PIZ_MAGIC_PITCH);
    
    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
        
        while (note) {
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);

            note->data[PIZ_PITCH] = CLAMP (note->data[PIZ_PITCH] + step, 0, PIZ_MAGIC_PITCH);
            
            note = nextNote;
        }
    }
    
    PIZUNLOCK
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
            note->isSelected = false;
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
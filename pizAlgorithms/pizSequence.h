/**
 * \file    pizSequence.h
 * \author  Jean Sapristi
 * \date    April 9, 2012.
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

#ifndef PIZ_SEQUENCE_H
#define PIZ_SEQUENCE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_MAXIMUM_NOTES              128   
#define PIZ_SEQUENCE_MAXIMUM_DURATION           96
#define PIZ_SEQUENCE_MAXIMUM_PATTERN            12
                                            
#define PIZ_SEQUENCE_INIT_TEMP_SIZE             128     /* MAX (MAXIMUM_NOTES, MAXIMUM_DURATION) */
#define PIZ_SEQUENCE_INIT_LOOKUP_SIZE           19
#define PIZ_SEQUENCE_DEFAULT_TIMELINE_SIZE      576 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_FLAG_NONE                  0UL
#define PIZ_SEQUENCE_FLAG_SNAP                  1UL
#define PIZ_SEQUENCE_FLAG_PATTERN               2UL
#define PIZ_SEQUENCE_FLAG_AMBITUS               4UL
#define PIZ_SEQUENCE_FLAG_CLIP                  8UL
#define PIZ_SEQUENCE_FLAG_UNSELECT              16UL
#define PIZ_SEQUENCE_FLAG_CLEAR                 32UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_NOTE_SIZE                  7
#define PIZ_SEQUENCE_ZONE_SIZE                  4
  
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
      
enum {
    PIZ_DATA_POSITION       = 0,
    PIZ_DATA_PITCH          = 1,
    PIZ_DATA_VELOCITY       = 2,
    PIZ_DATA_DURATION       = 3,
    PIZ_DATA_CHANNEL        = 4,
    PIZ_DATA_IS_SELECTED    = 5,
    PIZ_DATA_IS_PLAYED      = 6
    };

enum {
    PIZ_DATA_START          = 0,
    PIZ_DATA_END            = 1,
    PIZ_DATA_DOWN           = 2,
    PIZ_DATA_UP             = 3
    };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZNoteValue {
    PIZ_WHOLE_NOTE_DOTTED           = 144,
    PIZ_WHOLE_NOTE                  = 96,
    PIZ_WHOLE_NOTE_TRIPLET          = 64,
    PIZ_HALF_NOTE_DOTTED            = 72,
    PIZ_HALF_NOTE                   = 48,
    PIZ_HALF_NOTE_TRIPLET           = 32,
    PIZ_QUARTER_NOTE_DOTTED         = 36,
    PIZ_QUARTER_NOTE                = 24,
    PIZ_QUARTER_NOTE_TRIPLET        = 16,
    PIZ_EIGHTH_NOTE_DOTTED          = 18,
    PIZ_EIGHTH_NOTE                 = 12,
    PIZ_EIGHTH_NOTE_TRIPLET         = 8,
    PIZ_SIXTEENTH_NOTE_DOTTED       = 9,
    PIZ_SIXTEENTH_NOTE              = 6,
    PIZ_SIXTEENTH_NOTE_TRIPLET      = 4,
    PIZ_THIRTY_SECOND_NOTE          = 3,
    PIZ_THIRTY_SECOND_NOTE_TRIPLET  = 2,
    PIZ_NOTE_VALUE_NONE             = 1
    } PIZNoteValue;

typedef enum _PIZScaleType {
    PIZ_SCALE_CUSTOM                = -2,
    PIZ_SCALE_NONE                  = -1,
    PIZ_IONIAN                      =  0,
    PIZ_DORIAN,
    PIZ_PHRYGIAN,
    PIZ_LYDIAN,
    PIZ_MIXOLYDIAN,
    PIZ_AEOLIAN,
    PIZ_LOCRIAN,
    PIZ_NATURAL,
    PIZ_HARMONIC,
    PIZ_MELODIC,
    PIZ_WHOLE_TONE,
    PIZ_PENTATONIC_MAJOR,
    PIZ_PENTATONIC_MINOR,
    PIZ_OCTATONIC_HALF_WHOLE,
    PIZ_OCTATONIC_WHOLE_HALF,
    PIZ_SEVENTH_MAJOR,
    PIZ_SEVENTH_DOMINANT,
    PIZ_SEVENTH_MINOR,
    PIZ_SEVENTH_MAJOR_SIXTH,
    PIZ_SEVENTH_MINOR_SIXTH,
    PIZ_SEVENTH_HALF_DIMINISHED,
    PIZ_SEVENTH_DIMINISHED,
    PIZ_SEVENTH_SUSPENDED,
    PIZ_SEVENTH_SHARP_FIVE,
    PIZ_SEVENTH_FLAT_FIVE
    } PIZScaleType;

typedef enum _PIZScaleKey {
    PIZ_KEY_C = 0,
    PIZ_KEY_C_SHARP,
    PIZ_KEY_D,
    PIZ_KEY_D_SHARP,
    PIZ_KEY_E,
    PIZ_KEY_F,
    PIZ_KEY_F_SHARP,
    PIZ_KEY_G,
    PIZ_KEY_G_SHARP,
    PIZ_KEY_A,
    PIZ_KEY_A_SHARP,
    PIZ_KEY_B
    } PIZScaleKey;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZMidi {
    PIZ_MIDI_PITCH      = 0,
    PIZ_MIDI_VELOCITY   = 1,
    PIZ_MIDI_DURATION   = 2,
    PIZ_MIDI_CHANNEL    = 3
    } PIZMidi;
    
typedef struct _PIZNote {
    long     tag;
    long     position;
    long     midi[4];
    long     isSelected;
    long     isPlayed;
    } PIZNote;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZSequence {
    long                    *tempValues;
    PIZNote                 **tempNotes1;
    PIZNote                 **tempNotes2;
    PIZBoundedHashTable     *tempHash;
    PIZArray                *map;
    PIZLinklist             **timeline;
    long                    timelineSize;
    PIZBoundedStack         *ticketMachine;
    PIZBoundedHashTable     *lookup;
    PIZArray                *scale;
    PIZArray                *pattern;
    bool                    changedZone;
    PIZItemset128           addedNotes;
    PIZItemset128           removedNotes;
    PIZItemset128           changedNotes;
    long                    start;
    long                    end;
    long                    down;
    long                    up;
    long                    count;
    long                    index;
    long                    chance;
    long                    channel;
    long                    velocity;
    PIZNoteValue            cell;
    PIZNoteValue            grid;
    PIZNoteValue            noteValue;
    unsigned int            seed;
    } PIZSequence;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZSequence     *pizSequenceNew                     (long size);
void            pizSequenceFree                     (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long            pizSequenceCount                    (PIZSequence *x);

long            pizSequenceChance                   (PIZSequence *x);
long            pizSequenceVelocity                 (PIZSequence *x);
long            pizSequenceChannel                  (PIZSequence *x);
PIZNoteValue    pizSequenceCell                     (PIZSequence *x);
PIZNoteValue    pizSequenceGrid                     (PIZSequence *x);
PIZNoteValue    pizSequenceNoteValue                (PIZSequence *x);

void            pizSequenceSetChance                (PIZSequence *x, long value);
void            pizSequenceSetVelocity              (PIZSequence *x, long value);
void            pizSequenceSetChannel               (PIZSequence *x, long channel);
void            pizSequenceSetCell                  (PIZSequence *x, PIZNoteValue snapValue);
void            pizSequenceSetGrid                  (PIZSequence *x, PIZNoteValue snapValue);
void            pizSequenceSetNoteValue             (PIZSequence *x, PIZNoteValue noteValue);

PIZError        pizSequenceSetScale                 (PIZSequence *x, 
                                                    PIZScaleKey key, 
                                                    PIZScaleType type, 
                                                    const PIZArray *a);
                                        
PIZError        pizSequenceSetPattern               (PIZSequence *x, const PIZArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

bool            pizSequenceIsAtEnd                  (PIZSequence *x);
void            pizSequenceGoToStart                (PIZSequence *x);
PIZError        pizSequenceProceedStep              (PIZSequence *x, PIZLinklist *queue); //

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN long pizSequenceCount (PIZSequence *x)
{   
    return x->count;
}   

PIZ_EXTERN long pizSequenceChance (PIZSequence *x)
{
    return x->chance;
}

PIZ_EXTERN long pizSequenceVelocity (PIZSequence *x)
{
    return x->velocity;
}

PIZ_EXTERN long pizSequenceChannel (PIZSequence *x)
{
    return x->channel;
}

PIZ_EXTERN PIZNoteValue pizSequenceCell (PIZSequence *x)
{
    return x->cell;
}

PIZ_EXTERN PIZNoteValue pizSequenceGrid (PIZSequence *x)
{
    return x->grid;
}

PIZ_EXTERN PIZNoteValue pizSequenceNoteValue (PIZSequence *x)
{
    return x->noteValue;
}

PIZ_EXTERN void pizSequenceSetChance (PIZSequence *x, long value)
{
    x->chance = CLAMP (value, 0, 100);
}

PIZ_EXTERN void pizSequenceSetVelocity (PIZSequence *x, long value)
{
    x->velocity = value;
}

PIZ_EXTERN void pizSequenceSetChannel (PIZSequence *x, long channel)
{
    x->channel = CLAMP (channel, 1, PIZ_MAGIC_CHANNEL);
}

PIZ_EXTERN void pizSequenceSetCell (PIZSequence *x, PIZNoteValue snapValue)
{
    x->cell = snapValue;
}

PIZ_EXTERN void pizSequenceSetGrid (PIZSequence *x, PIZNoteValue snapValue)
{
    x->grid = snapValue;
}

PIZ_EXTERN void pizSequenceSetNoteValue (PIZSequence *x, PIZNoteValue noteValue)
{
    x->noteValue = noteValue;
}

PIZ_EXTERN bool pizSequenceIsAtEnd (PIZSequence *x)
{
    return (x->index >= x->end);
}

PIZ_EXTERN void pizSequenceGoToStart (PIZSequence *x)
{
    x->index = x->start;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_H
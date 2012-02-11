/**
 * \file    pizSequence.h
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

#ifndef PIZ_SEQUENCE_H
#define PIZ_SEQUENCE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <pthread.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_TIMELINE_SIZE                  576  
//                                                  576 < 1024

#define PIZ_SEQUENCE_MAXIMUM_NOTES                  128
#define PIZ_SEQUENCE_MAXIMUM_DURATION               96
#define PIZ_SEQUENCE_MIDI_CHANNEL                   16
#define PIZ_SEQUENCE_MIDI_NOTE                      127
#define PIZ_SEQUENCE_MIDI_VELOCITY                  127
//                                          MAX    -----
#define PIZ_SEQUENCE_POOL_SIZE                      128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_SCALE_SIZE                     12

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_VERSION                        1
    
#define PIZ_SEQUENCE_POSITION                       0
#define PIZ_SEQUENCE_PITCH                          1
#define PIZ_SEQUENCE_VELOCITY                       2
#define PIZ_SEQUENCE_DURATION                       3
#define PIZ_SEQUENCE_CHANNEL                        4
#define PIZ_SEQUENCE_IS_SELECTED                    5
#define PIZ_SEQUENCE_IS_MARKED                      6
#define PIZ_SEQUENCE_NOTE_SIZE                      7

#define PIZ_SEQUENCE_START                          0
#define PIZ_SEQUENCE_END                            1
#define PIZ_SEQUENCE_DOWN                           2
#define PIZ_SEQUENCE_UP                             3
#define PIZ_SEQUENCE_ZONE_SIZE                      4

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_NOTE_FLAG_NONE                 (0L)
#define PIZ_SEQUENCE_NOTE_FLAG_LASSO                (1<<0)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_ADD_MODE_NONE                  (0L)
#define PIZ_SEQUENCE_ADD_MODE_SNAP                  (1<<0)
#define PIZ_SEQUENCE_ADD_MODE_PATTERN               (1<<1)
#define PIZ_SEQUENCE_ADD_MODE_AMBITUS               (1<<2)
#define PIZ_SEQUENCE_ADD_MODE_CLIP                  (1<<3)
#define PIZ_SEQUENCE_ADD_MODE_ORIGIN                (1<<4)
#define PIZ_SEQUENCE_ADD_MODE_UNSELECT              (1<<5)
#define PIZ_SEQUENCE_ADD_MODE_CLEAR                 (1<<6)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_START                           0
#define PIZ_DEFAULT_END                             96
#define PIZ_DEFAULT_DOWN                            60
#define PIZ_DEFAULT_UP                              71
#define PIZ_DEFAULT_CHANCE                          100
#define PIZ_DEFAULT_CHANNEL                         1
#define PIZ_DEFAULT_VELOCITY                        90

#define PIZ_CHANNEL_NONE                            0

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZLOCK             pthread_mutex_lock (&x->lock);
#define PIZUNLOCK           pthread_mutex_unlock (&x->lock);
#define PIZMAPFLAG          (pizItemset1024SetAtIndex (&x->mapFlags, 1024));
#define PIZNEEDTOMAKEMAP    (pizItemset1024IsSetAtIndex (&x->mapFlags, 1024))

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZSnapValue {
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
    PIZ_SNAP_NONE                   = 1
    } PIZSnapValue;

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
    
typedef struct  _PIZCoordinates {
    long    position;
    long    pitch;
    } PIZCoordinates;

typedef enum _PIZSelector {
    PIZ_PITCH       = 0,
    PIZ_VELOCITY    = 1,
    PIZ_DURATION    = 2,
    PIZ_CHANNEL     = 3
    } PIZSelector;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZNote {
    long    flags;
    long    data[4];
    long    isSelected;
    long    position;
    long    originPosition;
    } PIZNote;

typedef struct _PIZSequence {
    PIZItemset1024      mapFlags;               //
    pthread_mutex_t     lock;
    long                *values1;
    long                *values2;
    PIZNote             **notes1;
    PIZNote             **notes2;
    PIZBoundedHashTable *hashTable;             //
    PIZLinklist         **timeline;
    PIZGrowingArray     *scale;
    PIZGrowingArray     *pattern;
    PIZNote             *markedNote;
    PIZGrowingArray     *map;
    long                start;
    long                end;
    long                down;
    long                up;
    long                count;
    long                index;
    long                chance;
    long                channel;
    long                velocity;
    PIZSnapValue        grid;
    PIZSnapValue        noteValue;              //
    long                tempStart;
    long                tempEnd;
    long                tempDown;
    long                tempUp;
    long                tempOriginStart;
    long                tempOriginDown;
    long                tempOriginWidth;
    long                tempOriginHeight;
    } PIZSequence;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZSequence  *pizSequenceNew                       (void);
void         pizSequenceFree                       (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long         pizSequenceCount                      (PIZSequence *x);
long         pizSequenceIndex                      (PIZSequence *x);
long         pizSequenceChannel                    (PIZSequence *x);
long         pizSequenceVelocity                   (PIZSequence *x);
PIZSnapValue pizSequenceGrid                       (PIZSequence *x);
PIZSnapValue pizSequenceNoteValue                  (PIZSequence *x);

void         pizSequenceSetChance                  (PIZSequence *x, long value);
void         pizSequenceSetChannel                 (PIZSequence *x, long channel);
void         pizSequenceSetVelocity                (PIZSequence *x, long value);
void         pizSequenceSetGrid                    (PIZSequence *x, PIZSnapValue snapValue);
void         pizSequenceSetNoteValue               (PIZSequence *x, PIZSnapValue noteValue);

PIZError     pizSequenceSetScale (PIZSequence *x, PIZScaleKey key, PIZScaleType type, const PIZGrowingArray *a);
PIZError     pizSequenceSetPattern  (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void         pizSequenceClear                      (PIZSequence *x);

PIZError     pizSequenceSetZoneWithArray           (PIZSequence *x, const PIZGrowingArray *a);

PIZError     pizSequenceAddNotesWithArray          (PIZSequence *x, const PIZGrowingArray *a, long mode);
PIZError     pizSequenceAddNoteWithCoordinates     (PIZSequence *x, const PIZCoordinates *c,  long mode);

PIZError     pizSequenceZoneToArray                (PIZSequence *x, PIZGrowingArray *a);
PIZError     pizSequenceNotesToArray               (PIZSequence *x, 
                                                   PIZGrowingArray *unselectedNotes,
                                                   PIZGrowingArray *selectedNotes);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

bool         pizSequenceClean                      (PIZSequence *x, long value);
bool         pizSequenceApplyPattern               (PIZSequence *x);
bool         pizSequenceApplyAmbitus               (PIZSequence *x);
void         pizSequenceTransposeOctave            (PIZSequence *x, bool down);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

bool         pizSequenceIsAtEnd                    (PIZSequence *x);
void         pizSequenceGoToStart                  (PIZSequence *x);
PIZError     pizSequenceProceedStep                (PIZSequence *x, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError    pizSequenceEncodeSlotToArray        (PIZSequence *x, PIZGrowingArray *a);
PIZError    pizSequenceDecodeSlotWithArray      (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL            PIZNote *pizSequenceAddNote            (PIZSequence *x, long *values, long mode);

PIZ_LOCAL            void pizSequenceMakeMap                (PIZSequence *x);
PIZ_LOCAL            void pizSequenceClearNotes             (PIZSequence *x);
PIZ_LOCAL            long pizSequenceMovePitchToAmbitus     (PIZSequence *x, long pitch);

PIZ_LOCAL PIZ_INLINE long pizSequenceSnapPositionToPattern  (PIZSequence *x, long toSnapped, long patternSize);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
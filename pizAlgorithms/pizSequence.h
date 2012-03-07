/**
 * \file    pizSequence.h
 * \author  Jean Sapristi
 * \date    March 7, 2012.
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

#define PIZ_SEQUENCE_VERSION_MAJOR      1
#define PIZ_SEQUENCE_VERSION_MINOR      0  

#define PIZ_SEQUENCE_MAXIMUM_NOTES      128   
#define PIZ_SEQUENCE_MAXIMUM_DURATION   96
//                                     ----- MAX 
#define PIZ_TEMP_SIZE                   128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_CHANNEL_NONE                0
#define PIZ_NOTE_FLAG_NONE              (0L)
#define PIZ_NOTE_FLAG_LASSO             (1<<0)

#define PIZ_DEFAULT_START               0
#define PIZ_DEFAULT_END                 96
#define PIZ_DEFAULT_DOWN                60
#define PIZ_DEFAULT_UP                  71
#define PIZ_DEFAULT_CHANCE              100
#define PIZ_DEFAULT_CHANNEL             1
#define PIZ_DEFAULT_VELOCITY            90
#define PIZ_DEFAULT_TIMELINE_SIZE       576 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZLOCK     pthread_mutex_lock (&x->lock);
#define PIZUNLOCK   pthread_mutex_unlock (&x->lock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_TAG(n)  pizItemset128SetAtIndex (&x->changedNotes, (n))     

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
    PIZ_NOTE_NONE                   = 1
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

typedef enum _PIZSelector {
    PIZ_PITCH       = 0,
    PIZ_VELOCITY    = 1,
    PIZ_DURATION    = 2,
    PIZ_CHANNEL     = 3
    } PIZSelector;
 
typedef enum _PIZDataIndex {
    PIZ_DATA_POSITION       = 0,
    PIZ_DATA_PITCH,
    PIZ_DATA_VELOCITY,
    PIZ_DATA_DURATION,
    PIZ_DATA_CHANNEL,
    PIZ_DATA_IS_SELECTED,
    PIZ_DATA_IS_MARKED,
    PIZ_DATA_NOTE_SIZE      = 7,
    PIZ_DATA_START          = 0,
    PIZ_DATA_END,
    PIZ_DATA_DOWN,
    PIZ_DATA_UP,
    PIZ_DATA_ZONE_SIZE
    } PIZDataIndex;
 
typedef enum _PIZAddFlag {
    PIZ_ADD_FLAG_NONE       = 0,
    PIZ_ADD_FLAG_SNAP       = 1,
    PIZ_ADD_FLAG_PATTERN    = 2,
    PIZ_ADD_FLAG_AMBITUS    = 4,
    PIZ_ADD_FLAG_CLIP       = 8,
    PIZ_ADD_FLAG_UNSELECT   = 16,
    PIZ_ADD_FLAG_CLEAR      = 32,
    } PIZAddFlag;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
 
typedef struct _PIZNote {
    long    flags;
    long    data[4];
    long    isSelected;
    long    position;
    long    tag;
    } PIZNote;

typedef struct _PIZSequence {
    pthread_mutex_t     lock;
    long                *values1;
    long                *values2;
    PIZNote             **notes1;
    PIZNote             **notes2;
    PIZBoundedHashTable *hashTable;
    PIZLinklist         **timeline;
    PIZGrowingArray     *scale;
    PIZGrowingArray     *pattern;
    PIZNote             *markedNote;
    PIZGrowingArray     *map;
    PIZBoundedStack     *ticketMachine;
    PIZItemset128       addedNotes;
    PIZItemset128       removedNotes;
    PIZItemset128       changedNotes;
    long                changedZone;
    long                timelineSize;
    long                start;
    long                end;
    long                down;
    long                up;
    long                count;
    long                index;
    long                chance;
    long                channel;
    long                velocity;
    PIZNoteValue        cell;
    PIZNoteValue        grid;
    PIZNoteValue        noteValue;
    unsigned int        seed;
    } PIZSequence;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZSequence     *pizSequenceNew                     (long size);
void            pizSequenceFree                     (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long            pizSequenceCount                    (PIZSequence *x);
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
                                                    const PIZGrowingArray *a);
                                        
PIZError        pizSequenceSetPattern               (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

bool            pizSequenceHasMarkedNote            (PIZSequence *x);
long            pizSequenceMarkedNoteValue          (PIZSequence *x, PIZSelector selector);
void            pizSequenceChangeMarkedNoteValue    (PIZSequence *x, PIZSelector selector, long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void            pizSequenceClear                    (PIZSequence *x);
PIZError        pizSequenceNotesToArray             (PIZSequence *x, 
                                                    PIZGrowingArray *unselected, 
                                                    PIZGrowingArray *selected);
                                                
PIZError        pizSequenceAddNotesWithArray        (PIZSequence *x, const PIZGrowingArray *a, long flags);
PIZError        pizSequenceZoneToArray              (PIZSequence *x, PIZGrowingArray *a);
PIZError        pizSequenceSetZoneWithArray         (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void            pizSequenceTranspose                (PIZSequence *x, long n);
bool            pizSequenceClean                    (PIZSequence *x, long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long            pizSequenceIndex                    (PIZSequence *x);
bool            pizSequenceIsAtEnd                  (PIZSequence *x);
void            pizSequenceGoToStart                (PIZSequence *x);
PIZError        pizSequenceProceedStep              (PIZSequence *x, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZNote   *pizSequenceAddNote                 (PIZSequence *x, long *values, long flags);
PIZ_LOCAL PIZError  pizSequenceRemoveNote               (PIZSequence *x, PIZNote *note);
PIZ_LOCAL void      pizSequenceRemoveAllNotes           (PIZSequence *x);
PIZ_LOCAL void      pizSequenceMoveNote                 (PIZSequence *x, PIZNote *note, long newPosition);
PIZ_LOCAL void      pizSequenceMakeMap                  (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL long      pizSequenceMovePitchToAmbitus       (PIZSequence *x, long pitch);
PIZ_LOCAL long      pizSequenceSnapPositionToPattern    (PIZSequence *x, long toSnapped, long patternSize);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_H
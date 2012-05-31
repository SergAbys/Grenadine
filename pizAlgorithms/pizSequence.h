/**
 * \file    pizSequence.h
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

/* 
 *  PIZ_SEQUENCE_MAXIMUM_NOTES   = PIZ_ITEMSET_SIZE 
 *  PIZ_SEQUENCE_SIZE_TEMP       = MAX (MAXIMUM_NOTES, MAXIMUM_DURATION, PIZ_MAGIC_PITCH + 1) 
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef PIZ_SEQUENCE_H
#define PIZ_SEQUENCE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"
#include "pizArray.h"
#include "pizLinklist.h"
#include "pizItemset.h"
#include "pizHashTable.h"
#include "pizFactorOracle.h"
#include "pizGaloisLattice.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_MAXIMUM_NOTES          128
#define PIZ_SEQUENCE_MAXIMUM_DURATION       96
                                        
#define PIZ_SEQUENCE_SIZE_TEMP              128
#define PIZ_SEQUENCE_SIZE_LOOKUP            19
#define PIZ_SEQUENCE_SIZE_TIMELINE          288 

#define PIZ_SEQUENCE_DEFAULT_PITCH          60
#define PIZ_SEQUENCE_DEFAULT_VELOCITY       80

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_FLAG_NONE              0UL
#define PIZ_SEQUENCE_FLAG_SNAP              1UL
#define PIZ_SEQUENCE_FLAG_AMBITUS           2UL
#define PIZ_SEQUENCE_FLAG_CLIP              4UL 

#define PIZ_SEQUENCE_FLAG_ZONE              (1UL << 0)
#define PIZ_SEQUENCE_FLAG_CHANCE            (1UL << 1)
#define PIZ_SEQUENCE_FLAG_VELOCITY          (1UL << 2)
#define PIZ_SEQUENCE_FLAG_CHANNEL           (1UL << 3)
#define PIZ_SEQUENCE_FLAG_CHORD             (1UL << 4)
#define PIZ_SEQUENCE_FLAG_CELL              (1UL << 5)
#define PIZ_SEQUENCE_FLAG_NOTE_VALUE        (1UL << 6)
#define PIZ_SEQUENCE_FLAG_SCALE             (1UL << 8)
#define PIZ_SEQUENCE_FLAG_PATTERN           (1UL << 9) 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
  
struct _PIZAgent;

enum {
    PIZ_THIRTY_SECOND_NOTE_TRIPLET  = 2,
    PIZ_THIRTY_SECOND_NOTE          = 3,
    PIZ_SIXTEENTH_NOTE_TRIPLET      = 4,
    PIZ_SIXTEENTH_NOTE              = 6,
    PIZ_SIXTEENTH_NOTE_DOTTED       = 9,
    PIZ_EIGHTH_NOTE_TRIPLET         = 8,
    PIZ_EIGHTH_NOTE                 = 12,
    PIZ_EIGHTH_NOTE_DOTTED          = 18,
    PIZ_QUARTER_NOTE_TRIPLET        = 16,
    PIZ_QUARTER_NOTE                = 24,
    PIZ_QUARTER_NOTE_DOTTED         = 36,
    PIZ_HALF_NOTE_TRIPLET           = 32,
    PIZ_HALF_NOTE                   = 48,
    PIZ_HALF_NOTE_DOTTED            = 72,
    PIZ_WHOLE_NOTE_TRIPLET          = 64,
    PIZ_WHOLE_NOTE                  = 96,
    PIZ_WHOLE_NOTE_DOTTED           = 144
    };

enum {
    PIZ_IONIAN                      = 0,
    PIZ_DORIAN                      = 1,
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
    };
    
enum {
    PIZ_KEY_C                       = 0,
    PIZ_KEY_C_SHARP                 = 1,
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
    };

enum {
    PIZ_VALUE_PITCH                 = 0,
    PIZ_VALUE_VELOCITY              = 1,
    PIZ_VALUE_DURATION              = 2,
    PIZ_VALUE_CHANNEL               = 3
    };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZNote {
    long tag;
    long position;
    long values[4];
    } PIZNote;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
typedef struct _PIZSequence {
    long                tempIndex;
    PIZError            tempError;
    long                *tempValues;
    PIZHashTable        *tempHash;
    PIZNote             **tempNotes1;
    PIZNote             **tempNotes2;
    PIZArray            *map;
    PIZLinklist         **timeline;
    PIZHashTable        *lookup;
    PIZArray            *scale;
    PIZArray            *pattern;
    PIZItemset          usedNotes;
    PIZItemset          addedNotes;
    PIZItemset          changedNotes;
    PIZItemset          removedNotes;
    ulong               flags;
    long                start;
    long                end;
    long                down;
    long                up;
    long                count;
    long                index;
    long                chance;
    long                velocity;
    long                channel;
    long                chord;
    long                key;
    long                type;
    long                cell;
    long                noteValue;
    uint                seed;
    struct _PIZAgent    *agent;
    } PIZSequence;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZSequence   *pizSequenceNew (struct _PIZAgent *agent);
PIZ_LOCAL void          pizSequenceFree (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_H
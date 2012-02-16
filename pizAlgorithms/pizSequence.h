/**
 * \file    pizSequence.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup foundation
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

/** 
 * \def     PIZ_SEQUENCE_VERSION_MAJOR 
 * \brief   Current version.
 * \ingroup sequenceTypes
 */
 
/** 
 * \def     PIZ_SEQUENCE_VERSION_MINOR
 * \brief   Current version.
 * \ingroup sequenceTypes
 */
 
/** 
 * \def     PIZ_SEQUENCE_TIMELINE_SIZE
 * \brief   Number of steps in the sequence.
 * \ingroup sequenceTypes
 */
 
/**
 * \def     PIZ_SEQUENCE_MAXIMUM_NOTES 
 * \brief   Maximum number of notes in the sequence.
 * \ingroup sequenceTypes
 */

/**
 * \def     PIZ_SEQUENCE_MAXIMUM_DURATION
 * \brief   Maximum duration for a note.
 * \ingroup sequenceTypes
 */

#define PIZ_SEQUENCE_VERSION_MAJOR      1
#define PIZ_SEQUENCE_VERSION_MINOR      0

#define PIZ_SEQUENCE_TIMELINE_SIZE      576   
#define PIZ_SEQUENCE_MAXIMUM_NOTES      128   
#define PIZ_SEQUENCE_MAXIMUM_DURATION   96

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

// PIZ_POOL_SIZE = MAX (PIZ_SEQUENCE_MAXIMUM_NOTES, PIZ_SEQUENCE_MAXIMUM_DURATION)

#define PIZ_POOL_SIZE                   128
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZLOCK     pthread_mutex_lock (&x->lock);
#define PIZUNLOCK   pthread_mutex_unlock (&x->lock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \ingroup sequenceTypes
 */

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

/**
 * \ingroup sequenceTypes
 */
 
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

/**
 * \ingroup sequenceTypes
 */
 
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

/**
 * \ingroup sequenceTypes
 */
 
typedef struct  _PIZCoordinates {
    long    position;
    long    pitch;
    } PIZCoordinates;

/**
 * \ingroup sequenceTypes
 */
 
typedef enum _PIZSelector {
    PIZ_PITCH       = 0,
    PIZ_VELOCITY    = 1,
    PIZ_DURATION    = 2, 
    PIZ_CHANNEL     = 3
    } PIZSelector;

/**
 * \details Indexes/offsets used in array getters/setters.
 * \ingroup sequenceTypes
 */
 
typedef enum _PIZDataIndex {
    PIZ_DATA_POSITION       = 0,                    /*!< Offset of \a position. */
    PIZ_DATA_PITCH,                                 /*!< Offset of \a pitch. */
    PIZ_DATA_VELOCITY,                              /*!< Offset of \a velocity. */
    PIZ_DATA_DURATION,                              /*!< Offset of \a pitch. */
    PIZ_DATA_CHANNEL,                               /*!< Offset of \a channel. */
    PIZ_DATA_IS_SELECTED,                           /*!< Offset of boolean \a isSelected. */
    PIZ_DATA_IS_MARKED,                             /*!< Offset of boolean \a isMarked. */
    PIZ_DATA_NOTE_SIZE      = 7,                    /*!< Offset for a note. */
    PIZ_DATA_ORIGIN         = 7,                    /*!< Private. */
    PIZ_DATA_TAG,                                   /*!< Private. */
    PIZ_DATA_START          = 0,                    /*!< Offset of \a start. */
    PIZ_DATA_END,                                   /*!< Offset of \a end. */
    PIZ_DATA_DOWN,                                  /*!< Offset of \a down. */
    PIZ_DATA_UP,                                    /*!< Offset of \a up. */
    PIZ_DATA_ZONE_SIZE                              /*!< Size for the zone. */
    } PIZDataIndex;

/**
 * \ingroup sequenceTypes
 */
 
typedef enum _PIZAddFlag {
    PIZ_ADD_FLAG_NONE       = 0,                    /*!< No flag. */
    PIZ_ADD_FLAG_SNAP       = 1,                    /*!< Snap notes to grid. */
    PIZ_ADD_FLAG_PATTERN    = 2,                    /*!< Snap notes to pattern. */
    PIZ_ADD_FLAG_AMBITUS    = 4,                    /*!< Transpose notes to zone's ambitus. */
    PIZ_ADD_FLAG_CLIP       = 8,                    /*!< Restrict notes to zone. */
    PIZ_ADD_FLAG_UNSELECT   = 16,                   /*!< Force notes unselected/unmarked. */
    PIZ_ADD_FLAG_CLEAR      = 32,                   /*!< Clear sequence before. */
    PIZ_ADD_FLAG_ORIGIN     = 64,                   /*!< Private. */
    PIZ_ADD_FLAG_TAG        = 128                   /*!< Private. */
    } PIZAddFlag;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \ingroup sequenceTypes
 */
 
typedef struct _PIZNote {
    long    flags;                                  /*!< Various flags. */
    long    data[4];                                /*!< MIDI values. */
    long    isSelected;                             /*!< True if the note is selected. */
    long    position;                               /*!< Current position of the note. */
    long    origin;                                 /*!< Birth position of the note. */
    long    tag;                                    /*!< Label (unique) of the note. */
    } PIZNote;

/**
 * \ingroup sequenceTypes
 */
 
typedef struct _PIZSequence {
    PIZItemset1024      mapFlags;                   /*!< In use map as bit field. */
    pthread_mutex_t     lock;                       /*!< POSIX mutex. */
    long                *values1;                   /*!< Private for temporary storage. */
    long                *values2;                   /*!< Private for temporary storage. */
    PIZNote             **notes1;                   /*!< Private for temporary storage. */
    PIZNote             **notes2;                   /*!< Private for temporary storage. */
    PIZBoundedHashTable *hashTable;                 /*!< Private for temporary storage. */
    PIZLinklist         **timeline;                 /*!< Sequence as a timeline. */
    PIZGrowingArray     *scale;                     /*!< Mode/scale. */
    PIZGrowingArray     *pattern;                   /*!< Rythmic pattern. */
    PIZNote             *markedNote;                /*!< Pointer to one note in the sequence. */
    PIZGrowingArray     *map;                       /*!< List of timeline's busy index. */
    long                start;                      /*!< Playback head's start (included). */
    long                end;                        /*!< Playback head's end (not included). */
    long                down;                       /*!< Playback head's low ambitus (included). */
    long                up;                         /*!< Playback head's low ambitus (included). */
    long                count;                      /*!< Number of notes. */
    long                index;                      /*!< Current playback head's index. */
    long                chance;                     /*!< Chance value [0, 100]. */
    long                channel;                    /*!< Global MIDI channel. */
    long                velocity;                   /*!< Global MIDI velocity. */
    PIZNoteValue        grid;                       /*!< Grid of the sequence. */
    PIZNoteValue        noteValue;                  /*!< Default note value. */
    long                tempStart;                  /*!< Temporary zone storage. */
    long                tempEnd;                    /*!< Temporary zone storage. */
    long                tempDown;                   /*!< Temporary zone storage. */
    long                tempUp;                     /*!< Temporary zone storage. */
    long                tempOriginStart;            /*!< Temporary zone storage. */
    long                tempOriginDown;             /*!< Temporary zone storage. */
    long                tempOriginWidth;            /*!< Temporary zone storage. */
    long                tempOriginHeight;           /*!< Temporary zone storage. */
    } PIZSequence;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the sequence.
 * \return  A pointer to the new sequence.  
 * \ingroup sequenceClass
 */
PIZSequence *pizSequenceNew (void);

/**
 * \brief   Free the sequence.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 * \ingroup sequenceClass
 */
void pizSequenceFree (PIZSequence *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Get the number of notes in the sequence.
 * \param   x A valid pointer.
 * \return  The number of notes.
 * \ingroup sequenceClass
 */
long pizSequenceCount (PIZSequence *x);

/**
 * \brief   Get the global velocity of the sequence. 
 * \remark  Default is 0.
 * \param   x A valid pointer.
 * \return  The velocity.
 * \ingroup sequenceClass
 */
long pizSequenceVelocity (PIZSequence *x);

/**
 * \brief   Get the MIDI channel of the sequence.
 * \remark  Default is 1.
 * \param   x A valid pointer.
 * \return  The MIDI channel.
 * \ingroup sequenceClass
 */
long pizSequenceChannel (PIZSequence *x);

/**
 * \brief   Get the grid value of the sequence.
 * \param   x A valid pointer.
 * \return  The grid value.
 * \ingroup sequenceClass
 */
PIZNoteValue pizSequenceGrid (PIZSequence *x);

/**
 * \brief   Get the default note duration.
 * \param   x A valid pointer.
 * \return  The default duration.
 * \ingroup sequenceClass
 */
PIZNoteValue pizSequenceNoteValue (PIZSequence *x);

/**
 * \brief   Set the \a chance value of the sequence.
 * \remark  Default is 100.
 * \param   x A valid pointer.
 * \param   value The value [0, 100].
 * \ingroup sequenceClass
 */
void pizSequenceSetChance (PIZSequence *x, long value);

/**
 * \brief   Set the global velocity of the sequence.
 * \param   x A valid pointer.
 * \param   value The velocity.
 * \ingroup sequenceClass
 */
void pizSequenceSetVelocity (PIZSequence *x, long value);

/**
 * \brief   Set the MIDI channel of the sequence.
 * \param   x A valid pointer.
 * \param   value The MIDI channel.
 * \ingroup sequenceClass
 */
void pizSequenceSetChannel (PIZSequence *x, long channel);

/**
 * \brief   Set the grid of the sequence.
 * \param   x A valid pointer.
 * \param   value The grid value.
 * \ingroup sequenceClass
 */
void pizSequenceSetGrid (PIZSequence *x, PIZNoteValue snapValue);

/**
 * \brief   Set the default note duration.
 * \param   x A valid pointer.
 * \param   value The default note.
 * \ingroup sequenceClass
 */
void pizSequenceSetNoteValue (PIZSequence *x, PIZNoteValue noteValue);

/**
 * \brief   Set the scale of the sequence.
 * \param   x A valid pointer.
 * \param   key The scale key.
 * \param   type The scale type.
 * \param   a The scale if the type provided is custom, otherwise should be NULL.
 * \remark	An example :  
 * \code
 *  if (type != PIZ_SCALE_CUSTOM) {
 *      pizSequenceSetScale (sequence, PIZ_KEY_C, PIZ_DORIAN, NULL);
 *  } else {
 *      pizSequenceSetScale (sequence, PIZ_KEY_C, PIZ_SCALE_CUSTOM, growingArrayPtr);
 *  }
 * \endcode 
 * \warning Custom scales must be twelve values long.
 * \ingroup sequenceClass
 */
PIZError pizSequenceSetScale (PIZSequence *x, PIZScaleKey key, PIZScaleType type, const PIZGrowingArray *a);

/**
 * \brief   Set rythmic pattern of the sequence.
 * \param   x A valid pointer.
 * \param   a The rythmic motif.
 * \ingroup sequenceClass
 */
PIZError pizSequenceSetPattern (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Clear the sequence.
 * \param   x A valid pointer.
 * \ingroup sequenceBases
 */
void pizSequenceClear (PIZSequence *x);

/**
 * \brief   Get notes splitted in two dynamic arrays according to note's selected state.
 * \param   x A valid pointer.
 * \param   unselected A pointer to the dynamic array to fill with unselected notes.
 * \param   selected A pointer to the dynamic array to fill with selected notes
 * \return  An error code.
 * \details Notes are appended. The pointer can be NULL. 
 *          Notes are transposed according to scale.
 * \remark  Examples :
 * \code
 * err |= pizSequenceNotesToArray (sequence, ptrA, ptrB); 
 * err |= pizSequenceNotesToArray (sequence, ptrA, NULL); 
 * err |= pizSequenceNotesToArray (sequence, ptrA, ptrA);
 * \endcode 
 * \ingroup sequenceBases
 */
PIZError pizSequenceNotesToArray (PIZSequence *x, PIZGrowingArray *unselected, PIZGrowingArray *selected);

/**
 * \brief   Add notes with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A pointer to the array.
 * \param   flags The flags (as ORed PIZAddFlags).
 * \return  An error code.
 * \remark  A naive example :
 * \code
 * long noteC[PIZ_DATA_NOTE_SIZE];
 * long noteG[PIZ_DATA_NOTE_SIZE] = {24, 67, 90, 24, 1, 0, 0}; 
 *
 * noteC[PIZ_DATA_POSITION]     = 0;
 * noteC[PIZ_DATA_PITCH]        = 60;
 * noteC[PIZ_DATA_VELOCITY]     = 90;
 * noteC[PIZ_DATA_DURATION]     = 24;
 * noteC[PIZ_DATA_CHANNEL]      = 0;    // use sequence's global MIDI channel
 * noteC[PIZ_DATA_IS_SELECTED]  = 0;
 * noteC[PIZ_DATA_IS_MARKED]    = 0;
 *
 * for (i = 0; i < PIZ_DATA_NOTE_SIZE; i++) {
 *      pizGrowingArrayAppend (arrayPtr, noteC[i]);
 * }
 *
 * for (i = 0; i < PIZ_DATA_NOTE_SIZE; i++) {
 *      pizGrowingArrayAppend (arrayPtr, noteG[i]);
 * }
 *
 * pizSequenceAddNotesWithArray (sequencePtr, arrayPtr, PIZ_ADD_FLAG_SNAP | PIZ_ADD_FLAG_CLEAR);
 *
 * \endcode 
 * \ingroup sequenceBases
 */
PIZError pizSequenceAddNotesWithArray (PIZSequence *x, const PIZGrowingArray *a, long flags);

/**
 * \brief   Get the zone with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A pointer to the array.
 * \return  An error code.
 * \ingroup sequenceBases
 */
PIZError pizSequenceZoneToArray (PIZSequence *x, PIZGrowingArray *a);

/**
 * \brief   Set the zone with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A pointer to the array.
 * \return  An error code.
 * \ingroup sequenceBases
 */
PIZError pizSequenceSetZoneWithArray (PIZSequence *x, const PIZGrowingArray *a);

/**
 * \brief   Snap notes according to rythmic pattern.
 * \param   x A valid pointer.
 * \return  True if changed otherwise false.
 * \ingroup sequenceBases
 */
bool pizSequenceApplyPattern (PIZSequence *x);

/**
 * \brief   Transpose notes to ambitus.
 * \param   x A valid pointer.
 * \return  True if changed otherwise false.
 * \ingroup sequenceBases
 */
bool pizSequenceApplyAmbitus (PIZSequence *x);

/**
 * \brief   Get the current playback head's index.
 * \param   x A valid pointer.
 * \return  The index.
 * \ingroup sequenceBases
 */
long pizSequenceIndex (PIZSequence *x);

/**
 * \brief   Test the end of the sequence.
 * \param   x A valid pointer.
 * \return  True if index is equal to zone's end otherwise false.
 * \ingroup sequenceBases
 */
bool pizSequenceIsAtEnd (PIZSequence *x);

/**
 * \brief   Set the index to zone's start.
 * \param   x A valid pointer.
 * \ingroup sequenceBases
 */
void pizSequenceGoToStart (PIZSequence *x);

/**
 * \brief   Fill an array with current index's notes and increment it.
 * \details Notes are appended.  Return PIZ_ERROR if index is at end. 
 *          Notes are transposed according to scale and velocity is modified by global velocity.
 * \param   x A valid pointer.
 * \param   a A pointer to a dynamic array.
 * \return  True if index is equal to zone's end otherwise false.
 * \ingroup sequenceBases
 */
PIZError pizSequenceProceedStep (PIZSequence *x, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \warning This function is private.
 * \brief   Add a note to the sequence.
 * \remark  Array must be PIZ_DATA_NOTE_SIZE long ; 
 *          one more value must be provided with PIZ_ADD_FLAG_ORIGIN, and one more with PIZ_ADD_FLAG_TAG. 
 *          For efficiency array size is NOT checked ; so crash may occured with invalid indexing.
 * \param   x A valid pointer.
 * \param   values A pointer to note values.
 * \param   flags The flags (as ORed PIZAddFlags).
 * \return  A pointer to the new note, NULL in case of error.
 * \ingroup sequenceBases
 */
PIZ_LOCAL PIZNote *pizSequenceAddNote (PIZSequence *x, long *values, long flags);

/**
 * \warning This function is private.
 * \brief   Restore the map after a note have been created or deleted.
 * \remark  For efficiency a list of busy linklist in the sequence is store in an array ; 
 *          By calling this function you guarantee that this list is ordered
 * \param   x A valid pointer.
 * \param   values A pointer to note values.
 * \param   flags The flags (as ORed PIZAddFlags).
 * \return  A pointer to the new note, NULL in case of error.
 * \ingroup sequenceBases
 */
PIZ_LOCAL            void pizSequenceCleanMap               (PIZSequence *x);
PIZ_LOCAL            void pizSequenceClearNotes             (PIZSequence *x);
PIZ_LOCAL            long pizSequenceMovePitchToAmbitus     (PIZSequence *x, long pitch);

PIZ_LOCAL PIZ_INLINE long pizSequenceSnapPositionToPattern  (PIZSequence *x, long toSnapped, long patternSize);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_H
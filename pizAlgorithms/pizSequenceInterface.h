/**
 * \file    pizSequenceInterface.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup sequenceInterface
 * \ingroup tralala
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

#ifndef PIZ_SEQUENCE_INTERFACE_H
#define PIZ_SEQUENCE_INTERFACE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \details Coordinates.   
 * \ingroup sequenceInterface
 */

typedef struct  _PIZCoordinates {
    long    position;
    long    pitch;
    } PIZCoordinates;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Test if a note is marked in the sequence.
 * \param   x A valid pointer.
 * \return  True if a note is marked, otherwise false.  
 * \ingroup sequenceInterface
 */
bool pizSequenceHasMarkedNote (PIZSequence *x);

/**
 * \brief   Get MIDI data of the marked note.
 * \remark  The function return -1 in case of error.
 * \param   x A valid pointer.
 * \param   selector A selector to choose the MIDI data type to query as \ref PIZSelector.
 * \return  The MIDI value.  
 * \ingroup sequenceInterface
 */
long pizSequenceMarkedNoteValue (PIZSequence *x, PIZSelector selector);

/**
 * \brief   Change MIDI data of the marked note.
 * \remark  Duration is increment (by grid's value) with positive argument (otherwise decrement).
 * \param   x A valid pointer.
 * \param   selector A selector to choose the MIDI data type to change as \ref PIZSelector.
 * \param   value The value to add. 
 * \ingroup sequenceInterface
 */
void pizSequenceChangeMarkedNoteValue (PIZSequence *x, PIZSelector selector, long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Initialize the temporary zone with the zone.
 * \param   x A valid pointer.
 * \ingroup sequenceInterface
 */
void pizSequenceInitTempZone (PIZSequence *x);

/**
 * \brief   Replace the zone with the temporary zone.
 * \param   x A valid pointer.
 * \return  An error code.   
 * \ingroup sequenceInterface
 */
PIZError pizSequencePlaceTempZone (PIZSequence *x);

/**
 * \brief   Resize the temporary zone with a \ref PIZCoordinates.
 * \details Use \ref PIZ_DATA_START, \ref PIZ_DATA_END, \ref PIZ_DATA_UP or \ref PIZ_DATA_DOWN 
 *          to indicate the side to change.
 * \param   x A valid pointer.
 * \param   c The Coordinates.
 * \param   side The side to set.
 * \return  True if something changed, otherwise false. 
 * \ingroup sequenceInterface
 */
bool pizSequenceResizeTempZone (PIZSequence *x, const PIZCoordinates *c, PIZDataIndex side);

/**
 * \brief   Move the temporary zone with delta values.
 * \details The temporary zone is snapped to the grid.
 * \param   x A valid pointer.
 * \param   pitch The pitch offset.
 * \param   position The position offset.
 * \return  True if something changed, otherwise false.   
 * \ingroup sequenceInterface
 */
bool pizSequenceMoveTempZone (PIZSequence *x, long pitch, long position);

/**
 * \brief   Get the temporary zone with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A pointer to the array.
 * \return  An error code.   
 * \ingroup sequenceInterface
 */
PIZError pizSequenceTempZoneToArray (PIZSequence *x, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceSelectAllNotes (PIZSequence *x);
void pizSequenceUnselectAllNotes (PIZSequence *x);

long pizSequenceSelectNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c);
long pizSequenceInvertNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c);

void pizSequenceInitLasso (PIZSequence *x);
long pizSequenceSelectNotesWithLasso (PIZSequence *x, const PIZCoordinates *m, const PIZCoordinates *n, bool r);

PIZError pizSequenceRemoveSelectedNotes (PIZSequence *x);
PIZError pizSequenceAddNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c,  long flags);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZ_INLINE void pizSequenceUnselectNotes (PIZSequence *x);
PIZ_LOCAL PIZ_INLINE long pizSequenceSnapRound (PIZSequence *x, long toSnapped);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_INTERFACE_H
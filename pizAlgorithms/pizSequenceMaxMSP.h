/**
 * \file    pizSequenceMaxMSP.h
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

#ifndef PIZ_SEQUENCE_MAXMSP_H
#define PIZ_SEQUENCE_MAXMSP_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \details Coordinates.   
 * \ingroup tralala
 */

typedef struct  _PIZCoordinates {
    long    position;
    long    pitch;
    } PIZCoordinates;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Initialize the temporary zone by the zone.
 * \param   x A valid pointer.
 * \ingroup tralala
 */
void pizSequenceInitTempZone (PIZSequence *x);

/**
 * \brief   Replace the zone by the temporary zone.
 * \param   x A valid pointer.
 * \return  An error code.   
 * \ingroup tralala
 */
PIZError pizSequencePutTempZone (PIZSequence *x);

/**
 * \brief   Resize the temporary zone with a \ref PIZCoordinates.
 * \details Use \ref PIZ_DATA_START, \ref PIZ_DATA_END, \ref PIZ_DATA_UP or \ref PIZ_DATA_DOWN 
 *          to indicate the side to change.
 * \param   x A valid pointer.
 * \param   c The Coordinates.
 * \param   side The side to set.
 * \return  True if something changed, otherwise false. 
 * \ingroup tralala
 */
bool pizSequenceResizeTempZone (PIZSequence *x, const PIZCoordinates *c, PIZDataIndex side);

/**
 * \brief   Move the temporary zone with delta values.
 * \details The temporary zone is snapped to the grid.
 * \param   x A valid pointer.
 * \param   pitch The pitch offset.
 * \param   position The position offset.
 * \return  True if something changed, otherwise false.   
 * \ingroup tralala
 */
bool pizSequenceMoveTempZone (PIZSequence *x, long pitch, long position);

/**
 * \brief   Get the temporary zone with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A pointer to the array.
 * \return  An error code.   
 * \ingroup tralala
 */
PIZError pizSequenceTempZoneToArray (PIZSequence *x, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Select all notes.
 * \param   x A valid pointer.  
 * \ingroup tralala
 */
void pizSequenceSelectAllNotes (PIZSequence *x);

/**
 * \brief   Unselect all notes.
 * \param   x A valid pointer.  
 * \ingroup tralala
 */
void pizSequenceUnselectAllNotes (PIZSequence *x);

/**
 * \brief   Given coordinates, select and mark the first note matched.
 * \details All notes are unselected except the note found.
 * \param   x A valid pointer.  
 * \param   c A coordinates as \ref PIZCoordinates.
 * \return  True if note matched, otherwise false.
 * \ingroup tralala
 */
long pizSequenceSelectNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c);

/**
 * \brief   Given coordinates, switch selected state of the first note matched.
 * \param   x A valid pointer.  
 * \param   c A coordinates as \ref PIZCoordinates.
 * \return  The new state if found, otherwise -1.
 * \ingroup tralala
 */
long pizSequenceInvertNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c);

/**
 * \brief   Initialize before to use pizSequenceSelectNotesWithLasso() with reverse mode.
 * \param   x A valid pointer.  
 * \ingroup tralala
 */
void pizSequenceInitLasso (PIZSequence *x);

/**
 * \brief   Select notes with lasso.
 * \param   x A valid pointer.  
 * \param   m The first \ref PIZCoordinates.
 * \param   n The second \ref PIZCoordinates.
 * \param   reverse Selection is done with reverse mode active (commonly shift key) if true.
 * \return  True if a note's state have changed.
 * \ingroup tralala
 */
long pizSequenceDragLasso (PIZSequence *x, const PIZCoordinates *m, const PIZCoordinates *n, bool reverse);

/**
 * \brief   Delete selected notes.
 * \param   x A valid pointer. 
 * \return  An error code. 
 * \ingroup tralala
 */
PIZError pizSequenceRemoveSelectedNotes (PIZSequence *x);

/**
 * \brief   Add a note with \ref PIZCoordinates.
 * \details Default values are provided to the new note ; 
 *          duration is set with \c noteValue or \c grid attribute.
 * \param   x A valid pointer.
 * \param   a A \ref PIZCoordinates.
 * \param   flags The flags (as ORed \ref PIZAddFlag).
 * \return  An error code.
 * \ingroup tralala
 */
PIZError pizSequenceAddNoteWithCoordinates (PIZSequence *x, const PIZCoordinates *c, long flags);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Encode to a dynamic array.
 * \param   x A valid pointer.
 * \param   a A dynamic array.
 * \return  An error code.
 * \ingroup tralala
 */
PIZError pizSequenceEncodeToArray (PIZSequence *x, PIZGrowingArray *a);

/**
 * \brief   Decode with a dynamic array.
 * \param   x A valid pointer.
 * \param   a A dynamic array.
 * \return  An error code.
 * \ingroup tralala
 */
PIZError pizSequenceDecodeWithArray (PIZSequence *x, const PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZ_INLINE void pizSequenceUnselectNotes (PIZSequence *x);
PIZ_LOCAL PIZ_INLINE long pizSequenceSnapRound (PIZSequence *x, long toSnapped);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_MAXMSP_H
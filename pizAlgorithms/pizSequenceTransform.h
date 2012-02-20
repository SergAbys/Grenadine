/**
 * \file    pizSequenceTransform.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup sequenceTransform
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

#ifndef PIZ_SEQUENCE_TRANSFORM_H
#define PIZ_SEQUENCE_TRANSFORM_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequence.h"
#include "pizFactorOracle.h"
#include "pizGaloisLattice.h"
#include "pizFiniteState.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \ingroup sequenceTransform
 * \ingroup sequenceTypes
 */
typedef enum _PIZAlgorithm {
    PIZ_ALGORITHM_NONE  = 0,            /*!<  */
    PIZ_FACTOR_ORACLE   = 1,            /*!<  */
    PIZ_GALOIS_LATTICE  = 2,            /*!<  */
    PIZ_FINITE_STATE    = 3             /*!<  */
    } PIZAlgorithm;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Learn from data-mining algorithms.
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \param   select The type of the algorithm as \ref PIZAlgorithm.
 * \param   algorithm A valid pointer.
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceProceedAlgorithm (PIZSequence *x, PIZAlgorithm select, void *algorithm);

/**
 * \brief   Kinda game of life.
 * \param   x A valid pointer.
 * \param   iterate The number of iterations of the process.
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceCellularAutomata (PIZSequence *x, long iterate);

/**
 * \brief   Rhythms generator.
 * \details The function searches the biggest beat divider (according to the cell) in the list 
 *          {2, 3, 4, 5, 7, 11} ; then random distribution tables are used to create/delete notes. 
 * \param   x A valid pointer.
 * \param   iterate The number of iterations of the process.
 * \param   division Force a divider (for example 2 even if 4 match later).
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceGenerator (PIZSequence *x, long iterate, long division);

/**
 * \brief   Rotate MIDI data between notes in the sequence.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \param   shift Number of steps to rotate. 
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceRotate (PIZSequence *x, PIZSelector selector, long shift);

/**
 * \brief   Scramble MIDI data between notes in the sequence.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceScramble (PIZSequence *x, PIZSelector selector);

/**
 * \brief   Sort MIDI data in the sequence.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \param   down Set True if sort down, otherwise up. 
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceSort (PIZSequence *x, PIZSelector selector, bool down);

/**
 * \brief   Change MIDI data for all notes in the sequence.
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \param   value The value to add. 
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceChange (PIZSequence *x, PIZSelector selector, long value);

/**
 * \brief   Set MIDI data for all notes in the sequence.
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \param   value The value to set. 
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceSet (PIZSequence *x, PIZSelector selector, long value);

/**
 * \brief   Change randomly MIDI data for all notes in the sequence.
 * \details The value is drawn at random between provided arguments.
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \param   selector A selector to set the MIDI data type altered as \ref PIZSelector.
 * \param   minValue The low border (included). 
 * \param   maxValue The high border (included).
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceRandom (PIZSequence *x, PIZSelector selector, long minValue, long maxValue);

/**
 * \brief   Delete notes in the sequence.
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceKillNotes (PIZSequence *x);

/**
 * \brief   Transpose according to cycle permutation.
 * \details http://en.wikipedia.org/wiki/Cyclic_permutation
 * \remark  This function is affected by \a chance attribute.
 * \param   x A valid pointer.
 * \param   key The tonic.
 * \param   a The cycle.
 * \return  True if something changed, otherwise false.  
 * \ingroup sequenceTransform
 */
bool pizSequenceCycle (PIZSequence *x, PIZScaleKey key, const PIZGrowingArray *a);

/**
 * \ingroup sequencePrivate
 */
PIZ_LOCAL long pizSequencePickUpNotes (PIZSequence *x);

/**
 * \ingroup sequencePrivate
 */
PIZ_LOCAL void pizSequenceFillValues (PIZSequence *x, PIZSelector selector, long k, bool reverse);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_TRANSFORM_H
/**
 * \file    pizSequenceTransform.h
 * \author  Jean Sapristi
 * \date    April 8, 2012.
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
#include "pizFiniteState.h"
#include "pizGaloisLattice.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError    pizSequenceAlgorithm   (PIZSequence *x, PIZAlgorithm *algorithm);
PIZError    pizSequenceNovember    (PIZSequence *x, long iterate); //
PIZError    pizSequenceJuliet      (PIZSequence *x, long iterate, long division); //

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizSequenceClear       (PIZSequence *x);
void        pizSequenceTranspose   (PIZSequence *x, long n);
void        pizSequenceClean       (PIZSequence *x, long value);
void        pizSequenceRotate      (PIZSequence *x, PIZMidi selector, long shift);
void        pizSequenceScramble    (PIZSequence *x, PIZMidi selector);
void        pizSequenceSort        (PIZSequence *x, PIZMidi selector, bool down);
void        pizSequenceChange      (PIZSequence *x, PIZMidi selector, long value);
void        pizSequenceSet         (PIZSequence *x, PIZMidi selector, long value);
void        pizSequenceRandom      (PIZSequence *x, PIZMidi selector, long minValue, long maxValue);
void        pizSequenceKill        (PIZSequence *x);
void        pizSequenceCycle       (PIZSequence *x, PIZScaleKey key, const PIZArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_TRANSFORM_H
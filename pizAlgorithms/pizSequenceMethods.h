/**
 * \file    pizSequenceMethods.h
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

#ifndef PIZ_SEQUENCE_METHODS_H
#define PIZ_SEQUENCE_METHODS_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizEvent.h"
#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_FLAG_NONE              (0UL)
#define PIZ_SEQUENCE_FLAG_ADD_SNAP          (1UL << 0)
#define PIZ_SEQUENCE_FLAG_ADD_AMBITUS       (1UL << 1)
#define PIZ_SEQUENCE_FLAG_ADD_CLIP          (1UL << 2) 
#define PIZ_SEQUENCE_FLAG_ADD_LOW           (1UL << 3)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
PIZ_START_C_LINKAGE

PIZError  pizSequenceInit           (PIZSequence *x, PIZEvent *event); //
PIZError  pizSequenceRefresh        (PIZSequence *x, PIZEvent *event); //

PIZError  pizSequenceNote           (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceClear          (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceClean          (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceTranspose      (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceRotate         (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceScramble       (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceSort           (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceChange         (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceFill           (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceKill           (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceCycle          (PIZSequence *x, PIZEvent *event);
PIZError  pizSequenceAlgorithm      (PIZSequence *x, PIZEvent *event);
PIZError  pizSequencePattern        (PIZSequence *x, PIZEvent *event); //
PIZError  pizSequenceJuliet         (PIZSequence *x, PIZEvent *event); //
PIZError  pizSequenceDump           (PIZSequence *x, PIZEvent *event); //
PIZError  pizSequenceStatistics     (PIZSequence *x, PIZEvent *event); //
PIZError  pizSequenceAttributes     (PIZSequence *x, PIZEvent *event); //

PIZ_END_C_LINKAGE
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_METHODS_H
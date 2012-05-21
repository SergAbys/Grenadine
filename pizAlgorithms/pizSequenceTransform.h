/**
 * \file    pizSequenceTransform.h
 * \author  Jean Sapristi
 * \date    May 20, 2012.
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

#include "pizEvent.h"
#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZError  pizSequenceNote            (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceClear           (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceClean           (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceTranspose       (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceRotate          (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceScramble        (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceSort            (PIZSequence *x, const PIZEvent *event);

PIZ_LOCAL PIZError  pizSequenceChange          (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceFill            (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceKill            (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceCycle           (PIZSequence *x, const PIZEvent *event);

PIZ_LOCAL PIZError  pizSequenceAlgorithm       (PIZSequence *x, const PIZEvent *event);
PIZ_LOCAL PIZError  pizSequenceNovember        (PIZSequence *x, const PIZEvent *event); //
PIZ_LOCAL PIZError  pizSequenceJuliet          (PIZSequence *x, const PIZEvent *event); //

PIZ_LOCAL PIZError  pizSequenceLearn           (PIZSequence *x, const PIZEvent *event);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_TRANSFORM_H
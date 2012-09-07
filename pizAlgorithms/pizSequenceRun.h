/**
 * \file    pizSequenceRun.h
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

#ifndef PIZ_SEQUENCE_RUN_H
#define PIZ_SEQUENCE_RUN_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_FLAG_INIT          (1023UL)
#define PIZ_SEQUENCE_FLAG_BPM           (1UL << 0)
#define PIZ_SEQUENCE_FLAG_CHANCE        (1UL << 1)
#define PIZ_SEQUENCE_FLAG_VELOCITY      (1UL << 2)
#define PIZ_SEQUENCE_FLAG_CHANNEL       (1UL << 3)
#define PIZ_SEQUENCE_FLAG_CHORD         (1UL << 4)
#define PIZ_SEQUENCE_FLAG_CELL          (1UL << 5)
#define PIZ_SEQUENCE_FLAG_VALUE         (1UL << 6)
#define PIZ_SEQUENCE_FLAG_SCALE         (1UL << 7)
#define PIZ_SEQUENCE_FLAG_MUTE          (1UL << 8)
#define PIZ_SEQUENCE_FLAG_ZONE          (1UL << 9)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
PIZ_START_C_LINKAGE

PIZ_LOCAL bool      pizSequenceIsAtEnd      (PIZSequence *x);
PIZ_LOCAL void      pizSequenceJumpToStart  (PIZSequence *x);

PIZ_LOCAL PIZError  pizSequenceRefresh      (PIZSequence *x); //
PIZ_LOCAL PIZError  pizSequenceStep         (PIZSequence *x); //
PIZ_LOCAL PIZError  pizSequenceStepBlank    (PIZSequence *x);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN bool pizSequenceIsAtEnd(PIZSequence *x)
{
    return (x->index >= x->end);
}

PIZ_EXTERN void pizSequenceJumpToStart(PIZSequence *x)
{
    x->index = x->start;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_RUN_H
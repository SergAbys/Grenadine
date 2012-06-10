/**
 * \file    pizSequencePrivate.h
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

#ifndef PIZ_SEQUENCE_PRIVATE_H
#define PIZ_SEQUENCE_PRIVATE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizEvent.h"
#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_SEQUENCE_FLAG_NONE          0UL
#define PIZ_SEQUENCE_FLAG_SNAP          1UL
#define PIZ_SEQUENCE_FLAG_AMBITUS       2UL
#define PIZ_SEQUENCE_FLAG_CLIP          4UL 

#define PIZ_SEQUENCE_FLAG_RANDOM        1UL
#define PIZ_SEQUENCE_FLAG_FILL          2UL
#define PIZ_SEQUENCE_FLAG_NEARBY        4UL

#define PIZ_SEQUENCE_FLAG_ALL           (511UL)
#define PIZ_SEQUENCE_FLAG_CHANCE        (1UL << 0)
#define PIZ_SEQUENCE_FLAG_VELOCITY      (1UL << 1)
#define PIZ_SEQUENCE_FLAG_CHANNEL       (1UL << 2)
#define PIZ_SEQUENCE_FLAG_CHORD         (1UL << 3)
#define PIZ_SEQUENCE_FLAG_CELL          (1UL << 4)
#define PIZ_SEQUENCE_FLAG_NOTE_VALUE    (1UL << 5)
#define PIZ_SEQUENCE_FLAG_SCALE         (1UL << 6)
#define PIZ_SEQUENCE_FLAG_PATTERN       (1UL << 7) 
#define PIZ_SEQUENCE_FLAG_ZONE          (1UL << 8)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZNote   *pizSequenceNewNote         (PIZSequence *x, long *argv, ulong flags);
PIZ_LOCAL PIZError  pizSequenceGetTag           (PIZSequence *x, long *ptr);
PIZ_LOCAL void      pizSequenceMakeMap          (PIZSequence *x);

PIZ_LOCAL PIZError  pizSequenceAddNotification  (PIZSequence *x, PIZEventCode n, long tag, long ac, long *av);

PIZ_LOCAL void      pizSequenceForEach          (PIZSequence *x, const PIZEvent *e, ulong f, PIZMethod method);
PIZ_LOCAL void      pizSequenceEachRemove       (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);
PIZ_LOCAL void      pizSequenceEachChange       (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);
PIZ_LOCAL void      pizSequenceEachCycle        (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);
PIZ_LOCAL void      pizSequenceEachDump         (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);
PIZ_LOCAL void      pizSequenceEachTempHash     (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);
PIZ_LOCAL void      pizSequenceEachTempNotes    (PIZSequence *x, const PIZEvent *e, ulong f, PIZNote *n);

PIZ_LOCAL long      pizSequenceFillTempNotes    (PIZSequence *x);
PIZ_LOCAL void      pizSequenceWithTempNotes    (PIZSequence *x, long selector, bool reverse);

PIZ_LOCAL long      pizSequenceToAmbitus        (PIZSequence *x, long pitch);
PIZ_LOCAL long      pizSequenceToPattern        (PIZSequence *x, long position);
PIZ_LOCAL long      pizSequenceToCell           (PIZSequence *x, long position);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_SEQUENCE_PRIVATE_H
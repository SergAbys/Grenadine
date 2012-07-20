/*
 * \file    pizSequenceUser.c
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

#include "pizSequenceUser.h"
#include "pizSequenceLibrary.h"
#include "pizSequenceMethods.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL PIZNote *pizSequenceNoteWithTag(PIZSequence *x, long tag);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceDelete(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachRemove(x, note, NULL, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceIncrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            long a[ ] = { argv[0], 1 };
            pizEventSetData(event, 2, a);
            pizSequenceEachChange(x, note, event, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceDecrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            long a[ ] = { argv[0], -1 };
            pizEventSetData(event, 2, a);
            pizSequenceEachChange(x, note, event, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZNote *pizSequenceNoteWithTag(PIZSequence *x, long tag)
{
    PIZNote *note = NULL;
    
    if ((tag >= 0) 
        && (tag < PIZ_SEQUENCE_MAXIMUM_NOTES) 
        && !(pizItemsetIsSetAtIndex(&x->removed, tag))
        && !(pizItemsetIsSetAtIndex(&x->changed, tag))) {
        note = x->lookup[tag];
    }
    
    return note;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
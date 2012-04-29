/*
 * \file	pizSequenceGraphic.c
 * \author	Jean Sapristi
 * \date	April 29, 2012.
 */
 
/*
 *  Copyright (c) 2012, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizSequenceGraphic.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_UNTAG   pizItemset128UnsetAtIndex 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceGetGraphicEvents (PIZSequence *x, PIZLinklist *queue)
{
    long     i;
    PIZError err = PIZ_GOOD;
    PIZNote  *note = NULL; 
    PIZEvent *notification = NULL;
    
    if (x->isZoneChanged) {
        
        long argv[ ] = { x->start, x->end, x->down, x->up };
        if (notification = pizEventNewWithZone (PIZ_EVENT_ZONE_CHANGED, argv)) {
            if (err |= pizLinklistAppend (queue, notification)) {       
                pizEventFree (notification);  
            }
        } else {
            err |= PIZ_MEMORY;
        }
        
        x->isZoneChanged = false;
    }
    
    if (pizItemset128Count (&x->removedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->removedNotes, i)) { 
            if (notification = pizEventNewWithNote (PIZ_EVENT_NOTE_REMOVED, NULL, i)) {
                if (err |= pizLinklistAppend (queue, notification)) {       
                    pizEventFree (notification);  
                }
            } else {
                err |= PIZ_MEMORY;
            }
        } 
    }
    //
    
    pizItemset128Clear (&x->removedNotes);
    }
    
    if (pizItemset128Count (&x->addedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
    //
    if (pizItemset128IsSetAtIndex (&x->addedNotes, i)) {
    //
    if (!(pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note))) {
    
        long argv[ ] = { note->position,
                         note->midi[PIZ_MIDI_PITCH],
                         note->midi[PIZ_MIDI_VELOCITY],
                         note->midi[PIZ_MIDI_DURATION], 
                         note->midi[PIZ_MIDI_CHANNEL] };
                         
        if (notification = pizEventNewWithNote (PIZ_EVENT_NOTE_ADDED, argv, i)) {
            if (err |= pizLinklistAppend (queue, notification)) {       
                pizEventFree (notification);  
            }
        } else {
            err |= PIZ_MEMORY;
        }
    }
    PIZ_UNTAG (&x->changedNotes, i);
    //
    } 
    //
    }
    
    pizItemset128Clear (&x->addedNotes);
    //
    }
    
    if (pizItemset128Count (&x->changedNotes)) {
    //
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
    //
    if (pizItemset128IsSetAtIndex (&x->changedNotes, i)) {
    //
    if (!(pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note))) {
    
        long argv[ ] = { note->position,
                         note->midi[PIZ_MIDI_PITCH],
                         note->midi[PIZ_MIDI_VELOCITY],
                         note->midi[PIZ_MIDI_DURATION], 
                         note->midi[PIZ_MIDI_CHANNEL] };
                         
        if (notification = pizEventNewWithNote (PIZ_EVENT_NOTE_CHANGED, argv, i)) {
            if (err |= pizLinklistAppend (queue, notification)) {       
                pizEventFree (notification);  
            }
        } else {
            err |= PIZ_MEMORY;
        }
    }
    //
    }
    //    
    }
    
    pizItemset128Clear (&x->changedNotes);
    //
    }
        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
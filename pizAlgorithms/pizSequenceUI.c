/*
 * \file	pizSequenceUI.c
 * \author	Jean Sapristi
 * \date	March 20, 2012.
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

#include "pizSequenceUI.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizSequenceAppendGraphicEvents (PIZSequence *x, PIZLinklist *queue)
{
    long i;
    
    PIZLOCK
    
    if (x->changedZone) {
        PIZEvent *event = NULL;   
                                           
        if (event = pizEventNew ( )) {
            event->type = PIZ_GRAPHIC_EVENT;
            event->name = PIZ_ZONE_CHANGED;
            
            event->data.values[PIZ_DATA_START] = x->start;
            event->data.values[PIZ_DATA_END]   = x->end;
            event->data.values[PIZ_DATA_DOWN]  = x->down;
            event->data.values[PIZ_DATA_UP]    = x->up;
                
            pizLinklistAppend (queue, event);
        }
    }
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
    //
    if (pizItemset128IsSetAtIndex (&x->removedNotes, i)) {
        PIZEvent *event = NULL;                                      
        if (event = pizEventNew ( )) {
            event->type = PIZ_GRAPHIC_EVENT;
            event->name = PIZ_NOTE_REMOVED;
            event->tag  = i;
            
            pizLinklistAppend (queue, event);
        }
    }
    //    
    }
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
    //
    if (pizItemset128IsSetAtIndex (&x->addedNotes, i)) {
        PIZNote  *note = NULL; 
        PIZEvent *event = NULL;  
        
        if (event = pizEventNew ( )) {
            event->type = PIZ_GRAPHIC_EVENT;
            event->name = PIZ_NOTE_ADDED;
            event->tag  = i;
            
            pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note);
            
            if (note) {
                event->data.values[PIZ_DATA_POSITION]    = note->position;
                event->data.values[PIZ_DATA_PITCH]       = note->data[PIZ_PITCH];
                event->data.values[PIZ_DATA_VELOCITY]    = note->data[PIZ_VELOCITY];
                event->data.values[PIZ_DATA_DURATION]    = note->data[PIZ_DURATION];
                event->data.values[PIZ_DATA_CHANNEL]     = note->data[PIZ_CHANNEL];
                event->data.values[PIZ_DATA_IS_SELECTED] = note->isSelected;
                event->data.values[PIZ_DATA_IS_MARKED]   = (note == x->markedNote);
            }
            
            pizLinklistAppend (queue, event);
            
            pizItemset128UnsetAtIndex (&x->changedNotes, i);
        }
    }
    //    
    }
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
    //
    if (pizItemset128IsSetAtIndex (&x->changedNotes, i)) {
        PIZNote  *note = NULL; 
        PIZEvent *event = NULL; 
         
        if (event = pizEventNew ( )) {
            event->type = PIZ_GRAPHIC_EVENT;
            event->name = PIZ_NOTE_CHANGED;
            event->tag  = i;
            
            pizBoundedHashTablePtrByKey (x->lookup, i, (void **)&note);
            
            if (note) {
                event->data.values[PIZ_DATA_POSITION]    = note->position;
                event->data.values[PIZ_DATA_PITCH]       = note->data[PIZ_PITCH];
                event->data.values[PIZ_DATA_VELOCITY]    = note->data[PIZ_VELOCITY];
                event->data.values[PIZ_DATA_DURATION]    = note->data[PIZ_DURATION];
                event->data.values[PIZ_DATA_CHANNEL]     = note->data[PIZ_CHANNEL];
                event->data.values[PIZ_DATA_IS_SELECTED] = note->isSelected;
                event->data.values[PIZ_DATA_IS_MARKED]   = (note == x->markedNote);
            }
            
            pizLinklistAppend (queue, event);
        }
    }
    //    
    }
    
    pizItemset128Clear (&x->removedNotes);
    pizItemset128Clear (&x->addedNotes);
    pizItemset128Clear (&x->changedNotes);
    
    x->changedZone = false;
    
    PIZUNLOCK
}
    
// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
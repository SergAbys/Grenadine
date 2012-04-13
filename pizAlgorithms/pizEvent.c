/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
 * \date	April 13, 2012.
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

#include "pizEvent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_INIT_ARRAY_SIZE 12

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const char *piz_eventNames[ ] = {    "Init",
                                            "Play",
                                            "Stop",
                                            "Loop",
                                            "Unloop",
                                            "Bpm",
                                            "Note Played",
                                    // 
                                            "Clear",
                                 /*   "Chance",
                                    "Velocity",
                                    "Channel",
                                    "Cell",
                                    "Scale",
                                    "Pattern",
                                    "Learn",
                                    "Transpose",
                                    "Clean",
                                    "Zoulou",
                                    "Romeo",
                                    "November",
                                    "Juliet",
                                    "Rotate",
                                    "Scramble",
                                    "Sort",
                                    "Change",
                                    "Set",
                                    "Random",
                                    "Kill",
                                    "Cycle",
                                    //
                                    "Grid",
                                    "Note Value",*/
                                            "Zone Changed",
                                            "Note Removed",
                                            "Note Added",
                                            "Note Changed",
                                            //
                                            "End",
                                            "Last",
                                            "Run Ready",
                                            "Graphic Ready"     };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew (PIZEventType type, PIZEventIdentifier ie);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZEvent *pizEventNewRun (PIZEventIdentifier ie)
{
    return pizEventNew (PIZ_EVENT_RUN, ie);
}

PIZEvent *pizEventNewRunWithTime (PIZEventIdentifier ie, const PIZTime *time)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_RUN, ie);
    
    if (event && time) {
        pizTimeCopy (&event->time, time);
    }
    
    return event;
}

PIZEvent *pizEventNewRunWithNote (PIZEventIdentifier ie, long *argv, long tag)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_RUN, ie); 
    
    if (event) {
        event->tag = tag;
        
        if (argv) {
            long i;
            for (i = 0; i < PIZ_SEQUENCE_NOTE_SIZE; i++) {
                pizArrayAppend (event->data, *(argv + i));
            }
        }
    }
    
    return event;
}

PIZEvent *pizEventNewRunWithValue (PIZEventIdentifier ie, long value)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_RUN, ie); 
    
    if (event) {
        pizArrayAppend (event->data, value);
    }
    
    return event;
}

PIZEvent *pizEventNewGraphicWithZone (PIZEventIdentifier ie, long *argv)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_GRAPHIC, ie);  
    
    if (event && argv) {
        long i;
        for (i = 0; i < PIZ_SEQUENCE_ZONE_SIZE; i++) {
            pizArrayAppend (event->data, *(argv + i));
        }
    }
    
    return event;
}

PIZEvent *pizEventNewGraphicWithNote (PIZEventIdentifier ie, long *argv, long tag)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_GRAPHIC, ie); 
    
    if (event) {
        event->tag = tag;
        
        if (argv) {
            long i;
            for (i = 0; i < PIZ_SEQUENCE_NOTE_SIZE; i++) {
                pizArrayAppend (event->data, *(argv + i));
            }
        }
    }
    
    return event;
}

PIZEvent *pizEventNewNotification (PIZEventIdentifier ie, const PIZTime *time)
{
    PIZEvent *event = pizEventNew (PIZ_EVENT_NOTIFICATION, ie);
    
    if (event && time) {
        pizTimeCopy (&event->time, time);
    }
    
    return event;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

const char *pizEventGetName (const PIZEvent *x)
{
    return piz_eventNames[x->identifier];
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZEvent *pizEventNew (PIZEventType type, PIZEventIdentifier ie) 
{
    PIZEvent *event = NULL;
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        if (event->data = pizArrayNew(PIZ_INIT_ARRAY_SIZE)) {
            event->type = type;
            event->identifier = ie;
        } else {
            free (event);
            event = NULL;
        }
    }
    
    return event;
}

void pizEventFree (PIZEvent *x)
{
    if (x) {
        pizArrayFree (x->data);
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
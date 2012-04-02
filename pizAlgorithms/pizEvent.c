/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
 * \date	April 2, 2012.
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

static char *piz_eventNames[ ] = {  "Init",
                                    "Play",
                                    "Stop",
                                    "Loop",
                                    "Unloop",
                                    "Bpm",
                                    "Note Played",
                                    //
                                    "Chance",
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
                                    "Uniform",
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
                                    "Note Value",
                                    "Zone Changed",
                                    "Note Removed",
                                    "Note Added",
                                    "Note Changed",
                                    //
                                    "End",
                                    "Last",
                                    "Run Ready",
                                    "GUI Ready"     };
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew (PIZEventType type, PIZEventIdentifier ie)
{
    PIZEvent *event = NULL;
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type       = type;
        event->identifier = ie;
    }
    
    return event;
}

PIZEvent *pizEventNewWithTime (PIZEventType type, PIZEventIdentifier ie, const PIZTime *time)
{
    PIZEvent *event = NULL;
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type       = type;
        event->identifier = ie;
        
        if (time) {
            pizTimeCopy (&event->data.time, time);
        }
    }
    
    return event;
}

PIZEvent *pizEventNewWithLong (PIZEventType type, PIZEventIdentifier ie, long n)
{
    PIZEvent *event = NULL;
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type       = type;
        event->identifier = ie;
        event->data.values[0] = n;
    }
    
    return event;
}

PIZEvent *pizEventNewWithArray (PIZEventType type, PIZEventIdentifier ie, long argc, long *argv, long tag) 
{
    PIZEvent *event = NULL; 
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type       = type;
        event->identifier = ie;
        event->tag        = tag;
        
        if (argv) {
            long i;
            
            for (i = 0; i < argc; i++) {
                event->data.values[i] = *(argv + i);
            }
        }
    }
    
    return event;
}

char *pizEventGetName (const PIZEvent *x)
{
    return piz_eventNames[x->identifier];
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizEventFree (PIZEvent *x)
{
    if (x) {
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
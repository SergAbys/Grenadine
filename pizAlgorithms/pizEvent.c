/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
 * \date	March 25, 2012.
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

PIZEvent *pizEventNew (void)
{
    PIZEvent *event = (PIZEvent *)calloc (1, sizeof(PIZEvent));
    return event;
}

PIZEvent *pizEventNewWithArray (PIZEventType t, PIZEventName name, long argc, long *argv, long tag) 
{
    PIZEvent *event = NULL; 
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type = t;
        event->name = name;
        event->tag  = tag;
        
        if (argv) {
            long i;
            
            for (i = 0; i < argc; i++) {
                event->data.values[i] = *(argv + i);
            }
        }
    }
    
    return event;
}

PIZEvent *pizEventNewNotification (PIZEventName name, PIZTime *time)
{
    PIZEvent *event = NULL;
    
    if (event = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        event->type = PIZ_NOTIFICATION;
        event->name = name;
        pizTimeCopy (&event->data.time, time);
    }
    
    return event;
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
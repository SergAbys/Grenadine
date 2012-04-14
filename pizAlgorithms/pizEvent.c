/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
 * \date	April 14, 2012.
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
#pragma mark -

PIZEvent *pizEventNewRun (PIZEventIdentifier ie)
{
    return pizEventNew (PIZ_EVENT_RUN, ie, -1, NULL, 0, NULL);
}

PIZEvent *pizEventNewRunWithTime (PIZEventIdentifier ie, const PIZTime *time)
{
    return pizEventNew (PIZ_EVENT_RUN, ie, -1, time, 0, NULL);
}

PIZEvent *pizEventNewRunWithNote (PIZEventIdentifier ie, const long *argv, long tag)
{
    return pizEventNew (PIZ_EVENT_RUN, ie, tag, NULL, PIZ_SEQUENCE_NOTE_SIZE, argv);
}

PIZEvent *pizEventNewRunWithValue (PIZEventIdentifier ie, long value)
{
    return pizEventNew (PIZ_EVENT_RUN, ie, -1, NULL, 1, &value);
}

PIZEvent *pizEventNewGraphicWithZone (PIZEventIdentifier ie, const long *argv)
{
    return pizEventNew (PIZ_EVENT_GRAPHIC, ie, -1, NULL, PIZ_SEQUENCE_ZONE_SIZE, argv);
}

PIZEvent *pizEventNewGraphicWithNote (PIZEventIdentifier ie, const long *argv, long tag)
{
    return pizEventNew (PIZ_EVENT_GRAPHIC, ie, tag, NULL, PIZ_SEQUENCE_NOTE_SIZE, argv);
}

PIZEvent *pizEventNewNotification (PIZEventIdentifier ie, const PIZTime *time)
{
    return pizEventNew (PIZ_EVENT_NOTIFICATION, ie, -1, time, 0, NULL);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizEventGetTime (const PIZEvent *x, PIZTime *time)
{
    PIZError err = PIZ_ERROR;
    
    if (!(pizTimeIsZero (&x->time))) {
        pizTimeCopy (time, &x->time);
    }
    
    return err;
}

PIZError pizEventGetValue (const PIZEvent *x, long *value)
{
    PIZError err = PIZ_ERROR;
    
    if (pizArrayCount (x->data)) {
        err = PIZ_GOOD;
        (*value) = pizArrayValueAtIndex (x->data, 0);
    }
    
    return err;
}

void pizEventGetName (const PIZEvent *x, const char **name)
{
    (*name) = piz_eventNames[x->identifier];
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZEvent *pizEventNew   (PIZEventType type, 
                        PIZEventIdentifier ie, 
                        long tag, 
                        const PIZTime *time, 
                        long argc, 
                        const long *argv) 
//                        
{
    PIZEvent *x = NULL;
    PIZError err = PIZ_GOOD;
    
    if (x = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
        if (x->data = pizArrayNew (PIZ_MAGIC_SCALE)) {
            x->type = type;
            x->identifier = ie;
            x->tag = tag; 
            
            if (time) {
                pizTimeCopy (&x->time, time);
            }
            
            if (argc && argv) {
                long i;
                for (i = 0; i < argc; i++) {
                    err |= pizArrayAppend (x->data, *(argv + i));
                }
            }
            
            if (err) {
                pizEventFree (x);
            }
            
        } else {
            free (x);
            x = NULL;
        }
    }
    
    return x;
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
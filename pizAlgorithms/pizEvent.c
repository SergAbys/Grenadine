/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
 * \date	April 27, 2012.
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

static const long piz_eventTypes[ ]  = {    PIZ_EVENT_RUN,                  // PIZ_EVENT_INIT
                                            PIZ_EVENT_RUN,                  // PIZ_EVENT_PLAY
                                            PIZ_EVENT_RUN,                  // PIZ_EVENT_STOP
                                            PIZ_EVENT_RUN,                  // PIZ_EVENT_LOOP
                                            PIZ_EVENT_RUN,                  // PIZ_EVENT_UNLOOP
                                            PIZ_EVENT_RUN,                  // PIZ_EVENT_BPM
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_NOTE
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_CLEAR
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_TRANSPOSE
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_LEARN
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_ZOULOU
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_ROMEO
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_NOVEMBER
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_JULIET
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_CLEAN
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_ROTATE
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_SCRAMBLE
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_SORT
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_CHANGE
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_SET
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_RANDOM
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_KILL
                                            PIZ_EVENT_TRANSFORM,            // PIZ_EVENT_CYCLE        
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_CHANCE
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_VELOCITY
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_CHANNEL
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_CELL
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_NOTE_VALUE
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_SCALE
                                            PIZ_EVENT_ATTRIBUTE,            // PIZ_EVENT_PATTERN
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_ZONE_CHANGED
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_NOTE_REMOVED
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_NOTE_ADDED
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_NOTE_CHANGED
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_NOTE_PLAYED
                                            PIZ_EVENT_NOTIFICATION,         // PIZ_EVENT_END
                                            PIZ_EVENT_NOTIFICATION      };  // PIZ_EVENT_LAST

   

static const char *piz_eventNames[ ] = {    "Init",
                                            "Play",
                                            "Stop",
                                            "Loop",
                                            "Unloop",
                                            "Bpm",
                                            // 
                                            "Note",
                                            "Clear",
                                            "Transpose",
                                            "Learn",
                                            "Zoulou",
                                            "Romeo",
                                            "November",
                                            "Juliet",
                                            "Clean",
                                            "Rotate",
                                            "Scramble",
                                            "Sort",
                                            "Change",
                                            "Set",
                                            "Random",
                                            "Kill",
                                            "Cycle",
                                            //
                                            
                                            //
                                            "Chance",
                                            "Velocity",
                                            "Channel",
                                            "Cell",
                                            "Note Value",
                                            "Scale",
                                            "Pattern", 
                                            //
                                            "Zone Changed",
                                            "Note Removed",
                                            "Note Added",
                                            "Note Changed",
                                            "Note Played",
                                            "End",
                                            "Last"      };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZEvent *pizEventNew (PIZEventIdentifier ie)
{
    return pizEventAlloc (ie, NULL, PIZ_SEQUENCE_NO_TAG, 0, NULL);
}

PIZEvent *pizEventNewWithTime (PIZEventIdentifier ie, const PIZTime *time)
{
    return pizEventAlloc (ie, time, PIZ_SEQUENCE_NO_TAG, 0, NULL);
}

PIZEvent *pizEventNewWithNote (PIZEventIdentifier ie, const long *argv, long tag)
{
    return pizEventAlloc (ie, NULL, tag, PIZ_SEQUENCE_NOTE_SIZE, argv);
}

PIZEvent *pizEventNewWithZone (PIZEventIdentifier ie, const long *argv)
{
    return pizEventAlloc (ie, NULL, PIZ_SEQUENCE_NO_TAG, PIZ_SEQUENCE_ZONE_SIZE, argv);
}

PIZEvent *pizEventNewWithArgs (PIZEventIdentifier ie, long argc, const long *argv)
{
    return pizEventAlloc (ie, NULL, PIZ_SEQUENCE_NO_TAG, argc, argv);
}

PIZEvent *pizEventNewWithValue (PIZEventIdentifier ie, long value)
{
    return pizEventAlloc (ie, NULL, PIZ_SEQUENCE_NO_TAG, 1, &value);
}

PIZEvent *pizEventNewCopy (PIZEvent *x)
{
    return pizEventAlloc (x->identifier, &x->time, x->tag, pizArrayCount (x->data), pizArrayPtr (x->data));
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizEventGetTime (const PIZEvent *x, PIZTime *time)
{
    PIZError err = PIZ_ERROR;
    
    if (!(pizTimeIsZero (&x->time))) {
        err = PIZ_GOOD;
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

PIZError pizEventGetData (const PIZEvent *x, long *argc, long **argv)
{
    PIZError err = PIZ_ERROR;
    
    if ((*argc) = pizArrayCount (x->data)) {
        err = PIZ_GOOD;
        (*argv) = pizArrayPtr (x->data);
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

PIZEvent *pizEventAlloc (PIZEventIdentifier ie, const PIZTime *time, long tag, long argc, const long *argv)
{
    PIZEvent *x = NULL;
    PIZError err = PIZ_GOOD;
    
    if (x = (PIZEvent *)calloc (1, sizeof(PIZEvent))) {
    //
    if (x->data = pizArrayNew (PIZ_MAGIC_SCALE)) {
    //
    x->type = piz_eventTypes[ie];
    x->identifier = ie;
    
    if (time) {
        pizTimeCopy (&x->time, time);
    } else {
        pizTimeSet (&x->time);
    }
    
    x->tag = tag; 
        
    if (argc && argv) {
        long i;
        for (i = 0; i < argc; i++) {
            err |= pizArrayAppend (x->data, *(argv + i));
        }
    }
    
    if (err) {
        pizEventFree (x);
        x = NULL;
    }
    //    
    } else {
        free (x);
        x = NULL;
    }
    //
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
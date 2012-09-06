/*
 * \file	pizEvent.c
 * \author	Jean Sapristi
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

static const long pizEventTypes[ ]  = { PIZ_EVENT_RUN,              // PIZ_EVENT_NONE
                                        PIZ_EVENT_RUN,              // PIZ_EVENT_INIT
                                        PIZ_EVENT_RUN,              // PIZ_EVENT_PLAY
                                        PIZ_EVENT_RUN,              // PIZ_EVENT_STOP
                                        PIZ_EVENT_RUN,              // PIZ_EVENT_LOOP
                                        PIZ_EVENT_RUN,              // PIZ_EVENT_UNLOOP
                                        //
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_LEARN
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_FORGET
                                        //
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_BPM
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CHANCE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_VELOCITY
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CHANNEL
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CHORD
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CELL
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_VALUE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_SCALE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_MUTE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_ZONE
                                        //
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_NOTE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CLEAR
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CLEAN
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_TRANSPOSE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_ROTATE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_SCRAMBLE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_SORT
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CHANGE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_FILL
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_KILL
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_CYCLE
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_PATTERN
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_ZOULOU
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_ROMEO
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_JULIET
                                        //  
                                        PIZ_EVENT_HIGH,             // PIZ_EVENT_DUMP
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_STATISTICS
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_ATTRIBUTES
                                        //  
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_NOTE_DELETE
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_NOTE_INCREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_NOTE_DECREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_NOTE_FORWARD
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_NOTE_BACKWARD
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_ZONE_INCREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_ZONE_DECREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CHANNEL_INCREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CHANNEL_DECREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CELL_INCREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CELL_DECREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CELL_UP
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_CELL_DOWN
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_VALUE_INCREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_VALUE_DECREMENT
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_VALUE_MODIFIER_UP
                                        PIZ_EVENT_LOW,              // PIZ_EVENT_VALUE_MODIFIER_DOWN
                                        //
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_BPM
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_CHANCE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_VELOCITY
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_CHANNEL
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_CHORD
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_CELL
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_VALUE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_SCALE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_MUTE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_ZONE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_INFO_COUNT
                                        //                            
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_BPM
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_CHANCE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_VELOCITY
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_CHANNEL
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_CHORD
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_CELL
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_VALUE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_SCALE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_MUTE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_CHANGED_ZONE
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_NOTE_ADDED
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_NOTE_CHANGED
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_NOTE_REMOVED
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_NOTE_PLAYED
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_NOTE_DUMPED
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_END
                                        PIZ_EVENT_NOTIFICATION,     // PIZ_EVENT_WILL_END
                                        PIZ_EVENT_NOTIFICATION };   // PIZ_EVENT_WILL_DUMP
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZEvent *pizEventNew(PIZEventCode code)
{
    PIZEvent *x = NULL;
    
    if (x = (PIZEvent *)calloc(1, sizeof(PIZEvent))) {
        x->code = code;
        x->type = pizEventTypes[code];
        
        pizTimeSet(&x->time);
    }
    
    return x;
}

PIZEvent *pizEventNewCopy(PIZEvent *event)
{
    PIZEvent *x = NULL;
    
    if (event && (x = (PIZEvent *)calloc(1, sizeof(PIZEvent)))) {
    //
    long i;
    
    x->code = event->code;
    x->type = event->type;
    pizTimeCopy(&x->time, &event->time);
    x->identifier = event->identifier;
    x->size = event->size;
    
    for (i = 0; i < PIZ_EVENT_DATA_SIZE; i++) {
        x->data[i] = event->data[i];
    }
        
    //
    }
    
    return x;
}
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizEventFree(PIZEvent *x)
{
    free(x);
}

void pizEventSetIdentifier(PIZEvent *x, long identifier)
{
    x->identifier = identifier;
}

void pizEventSetType(PIZEvent *x, PIZEventType type)
{
    x->type = type;
}

void pizEventSetTime(PIZEvent *x, const PIZTime *time)
{
    pizTimeCopy(&x->time, time);
}

void pizEventSetData(PIZEvent *x, long argc, const long *argv)
{
    if (argc && argv) {
        long i;
        x->size = MIN(argc, PIZ_EVENT_DATA_SIZE);
        for (i = 0; i < x->size; i++) {
            x->data[i] = *(argv + i);
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizEventCode(const PIZEvent *x, PIZEventCode *code)
{
    (*code) = x->code;
}

void pizEventType(const PIZEvent *x, PIZEventType *type)
{
    (*type) = x->type;
}

void pizEventIdentifier(const PIZEvent *x, long *identifier)
{
    (*identifier) = x->identifier;
}

void pizEventTime(const PIZEvent *x, PIZTime *time)
{
    pizTimeCopy(time, &x->time);
}

PIZError pizEventData(const PIZEvent *x, long *argc, long **argv)
{
    PIZError err = PIZ_ERROR;
    
    if ((*argc) = x->size) {
        (*argv) = (long *)x->data;
        err = PIZ_GOOD;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
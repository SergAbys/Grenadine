
/*
 *  Copyright (c) 2012, Nicolas Danet, 
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

static const PIZEventType pizEventTypes[ ]  = { PIZ_EVENT_RUN,              // PIZ_MSG_NONE
                                                PIZ_EVENT_RUN,              // PIZ_MSG_INIT
                                                PIZ_EVENT_RUN,              // PIZ_MSG_PLAY
                                                PIZ_EVENT_RUN,              // PIZ_MSG_STOP
                                                PIZ_EVENT_RUN,              // PIZ_MSG_LOOP
                                                PIZ_EVENT_RUN,              // PIZ_MSG_UNLOOP
                                                PIZ_EVENT_RUN,              // PIZ_MSG_REFRESH
                                                //
                                                PIZ_EVENT_LOW,              // PIZ_MSG_LEARN
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_FORGET
                                                //
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_BPM
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CHANCE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_VELOCITY
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CHANNEL
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CHORD
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CELL
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_VALUE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_SCALE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_MUTE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_ZONE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_NOTE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CLEAR
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CLEAN
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_TRANSPOSE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_ROTATE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_SCRAMBLE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_SORT
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CHANGE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_FILL
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_KILL
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_CYCLE
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_PATTERN
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_ZOULOU
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_ROMEO
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_JULIET
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_DUMP
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_STATISTICS
                                                PIZ_EVENT_HIGH,             // PIZ_MSG_ATTRIBUTES
                                                //  
                                                PIZ_EVENT_LOW,              // PIZ_GUI_NOTE_DELETE
                                                PIZ_EVENT_LOW,              // PIZ_GUI_NOTE_INCREMENT
                                                PIZ_EVENT_LOW,              // PIZ_GUI_NOTE_DECREMENT
                                                PIZ_EVENT_LOW,              // PIZ_GUI_NOTE_FORWARD
                                                PIZ_EVENT_LOW,              // PIZ_GUI_NOTE_BACKWARD
                                                PIZ_EVENT_LOW,              // PIZ_GUI_ZONE_INCREMENT
                                                PIZ_EVENT_LOW,              // PIZ_GUI_ZONE_DECREMENT
                                                //
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_BPM
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_CHANCE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_VELOCITY
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_CHANNEL
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_CHORD
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_CELL
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_VALUE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_SCALE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_MUTE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_ZONE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_INFORM_COUNT
                                                //                            
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_BPM
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_CHANCE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_VELOCITY
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_CHANNEL
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_CHORD
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_CELL
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_VALUE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_SCALE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_MUTE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_ZONE
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_ADDED
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_CHANGED
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_REMOVED
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_PLAYED
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_DUMPED
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_END
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_WILL_END
                                                PIZ_EVENT_NOTIFICATION,     // PIZ_NOTIFY_WILL_DUMP
                                                PIZ_EVENT_NOTIFICATION };   // PIZ_NOTIFY_WILL_REFRESH
    
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
    x->size = event->size;
    
    pizTimeCopy(&x->time, &event->time);
    
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

#ifndef PIZ_EXTERN_INLINE

void pizEventFree(PIZEvent *x)
{
    free(x);
}

void pizEventSetType(PIZEvent *x, PIZEventType type)
{
    x->type = type;
}

void pizEventCode(const PIZEvent *x, PIZEventCode *code)
{
    (*code) = x->code;
}

void pizEventType(const PIZEvent *x, PIZEventType *type)
{
    (*type) = x->type;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

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
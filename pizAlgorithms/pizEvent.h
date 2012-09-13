/**
 * \file	pizEvent.h
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

#ifndef PIZ_EVENT_H
#define PIZ_EVENT_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTime.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_EVENT_DATA_SIZE     14

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

enum {
    PIZ_EVENT_DATA_POSITION     = 0,
    PIZ_EVENT_DATA_PITCH        = 1,
    PIZ_EVENT_DATA_VELOCITY     = 2,
    PIZ_EVENT_DATA_DURATION     = 3,
    PIZ_EVENT_DATA_CHANNEL      = 4,
    PIZ_EVENT_DATA_TAG          = 5,
    PIZ_EVENT_DATA_BPM          = 6,
    PIZ_EVENT_DATA_LOW          = 7
    };

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_EVENT_RUN               = 1,
    PIZ_EVENT_LOW,
    PIZ_EVENT_HIGH,
    PIZ_EVENT_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventCode {
    //
    PIZ_MSG_NONE                = 0,
    PIZ_MSG_INIT                = 1,
    PIZ_MSG_PLAY                = 2,
    PIZ_MSG_STOP,
    PIZ_MSG_LOOP,
    PIZ_MSG_UNLOOP,
    PIZ_MSG_LEARN,
    PIZ_MSG_FORGET,
    PIZ_MSG_BPM,
    PIZ_MSG_CHANCE,
    PIZ_MSG_VELOCITY,
    PIZ_MSG_CHANNEL,
    PIZ_MSG_CHORD,
    PIZ_MSG_CELL,
    PIZ_MSG_VALUE,
    PIZ_MSG_SCALE,
    PIZ_MSG_MUTE,
    PIZ_MSG_ZONE,
    PIZ_MSG_NOTE,
    PIZ_MSG_CLEAR,
    PIZ_MSG_CLEAN,
    PIZ_MSG_TRANSPOSE,
    PIZ_MSG_ROTATE,
    PIZ_MSG_SCRAMBLE,
    PIZ_MSG_SORT,
    PIZ_MSG_CHANGE,
    PIZ_MSG_FILL,
    PIZ_MSG_KILL,
    PIZ_MSG_CYCLE, 
    PIZ_MSG_PATTERN,
    PIZ_MSG_ZOULOU,
    PIZ_MSG_ROMEO,
    PIZ_MSG_JULIET,
    PIZ_MSG_DUMP, 
    PIZ_MSG_STATISTICS,
    PIZ_MSG_ATTRIBUTES,
    //
    PIZ_GUI_LENGTH,
    PIZ_GUI_NOTE_DELETE,
    PIZ_GUI_NOTE_INCREMENT,
    PIZ_GUI_NOTE_DECREMENT,
    PIZ_GUI_NOTE_FORWARD,
    PIZ_GUI_NOTE_BACKWARD,
    PIZ_GUI_ZONE_INCREMENT,
    PIZ_GUI_ZONE_DECREMENT,
    //
    PIZ_INFORM_BPM,
    PIZ_INFORM_CHANCE,
    PIZ_INFORM_VELOCITY,
    PIZ_INFORM_CHANNEL,
    PIZ_INFORM_CHORD,
    PIZ_INFORM_CELL,
    PIZ_INFORM_VALUE,
    PIZ_INFORM_SCALE,
    PIZ_INFORM_MUTE,
    PIZ_INFORM_ZONE,
    PIZ_INFORM_COUNT,
    //
    PIZ_NOTIFY_BPM,
    PIZ_NOTIFY_CHANCE,
    PIZ_NOTIFY_VELOCITY,
    PIZ_NOTIFY_CHANNEL,
    PIZ_NOTIFY_CHORD,
    PIZ_NOTIFY_CELL,
    PIZ_NOTIFY_VALUE,
    PIZ_NOTIFY_SCALE,
    PIZ_NOTIFY_MUTE,
    PIZ_NOTIFY_ZONE,
    PIZ_NOTIFY_ADDED,
    PIZ_NOTIFY_CHANGED,
    PIZ_NOTIFY_REMOVED,
    PIZ_NOTIFY_PLAYED,
    PIZ_NOTIFY_DUMPED,
    PIZ_NOTIFY_END,
    PIZ_NOTIFY_WILL_END,
    PIZ_NOTIFY_WILL_DUMP,
    PIZ_NOTIFY_WILL_REFRESH 
    //
    } PIZEventCode;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZEvent {
    PIZEventCode    code;
    PIZEventType    type;
    PIZTime         time;
    long            size;
    long            data[PIZ_EVENT_DATA_SIZE];
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
PIZ_START_C_LINKAGE

PIZEvent    *pizEventNew        (PIZEventCode code);
PIZEvent    *pizEventNewCopy    (PIZEvent *event);

void        pizEventFree        (PIZEvent *x);
void        pizEventSetType     (PIZEvent *x, PIZEventType type);
void        pizEventSetTime     (PIZEvent *x, const PIZTime *time);
void        pizEventSetData     (PIZEvent *x, long argc, const long *argv);

void        pizEventCode        (const PIZEvent *x, PIZEventCode *code);
void        pizEventType        (const PIZEvent *x, PIZEventType *type);
void        pizEventTime        (const PIZEvent *x, PIZTime *time);
PIZError    pizEventData        (const PIZEvent *x, long *argc, long **argv);

PIZ_END_C_LINKAGE
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizEventFree(PIZEvent *x)
{
    free(x);
}

PIZ_EXTERN void pizEventSetType(PIZEvent *x, PIZEventType type)
{
    x->type = type;
}

PIZ_EXTERN void pizEventCode(const PIZEvent *x, PIZEventCode *code)
{
    (*code) = x->code;
}

PIZ_EXTERN void pizEventType(const PIZEvent *x, PIZEventType *type)
{
    (*type) = x->type;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_EVENT_H
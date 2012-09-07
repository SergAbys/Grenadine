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
    PIZ_MESSAGE_NONE            = 0,
    PIZ_MESSAGE_INIT            = 1,
    PIZ_MESSAGE_PLAY            = 2,
    PIZ_MESSAGE_STOP,
    PIZ_MESSAGE_LOOP,
    PIZ_MESSAGE_UNLOOP,
    PIZ_MESSAGE_LEARN,
    PIZ_MESSAGE_FORGET,
    PIZ_MESSAGE_BPM,
    PIZ_MESSAGE_CHANCE,
    PIZ_MESSAGE_VELOCITY,
    PIZ_MESSAGE_CHANNEL,
    PIZ_MESSAGE_CHORD,
    PIZ_MESSAGE_CELL,
    PIZ_MESSAGE_VALUE,
    PIZ_MESSAGE_SCALE,
    PIZ_MESSAGE_MUTE,
    PIZ_MESSAGE_ZONE,
    PIZ_MESSAGE_NOTE,
    PIZ_MESSAGE_CLEAR,
    PIZ_MESSAGE_CLEAN,
    PIZ_MESSAGE_TRANSPOSE,
    PIZ_MESSAGE_ROTATE,
    PIZ_MESSAGE_SCRAMBLE,
    PIZ_MESSAGE_SORT,
    PIZ_MESSAGE_CHANGE,
    PIZ_MESSAGE_FILL,
    PIZ_MESSAGE_KILL,
    PIZ_MESSAGE_CYCLE, 
    PIZ_MESSAGE_PATTERN,
    PIZ_MESSAGE_ZOULOU,
    PIZ_MESSAGE_ROMEO,
    PIZ_MESSAGE_JULIET,
    PIZ_MESSAGE_DUMP, 
    PIZ_MESSAGE_STATISTICS,
    PIZ_MESSAGE_ATTRIBUTES,
    //
    PIZ_GRAPHIC_CELL_UP,
    PIZ_GRAPHIC_CELL_DOWN,
    PIZ_GRAPHIC_CELL_RIGHT,
    PIZ_GRAPHIC_CELL_LEFT,
    PIZ_GRAPHIC_VALUE_UP,
    PIZ_GRAPHIC_VALUE_DOWN,
    PIZ_GRAPHIC_VALUE_RIGHT,
    PIZ_GRAPHIC_VALUE_LEFT,
    PIZ_GRAPHIC_NOTE_DELETE,
    PIZ_GRAPHIC_NOTE_INCREMENT,
    PIZ_GRAPHIC_NOTE_DECREMENT,
    PIZ_GRAPHIC_NOTE_FORWARD,
    PIZ_GRAPHIC_NOTE_BACKWARD,
    PIZ_GRAPHIC_ZONE_INCREMENT,
    PIZ_GRAPHIC_ZONE_DECREMENT,
    PIZ_GRAPHIC_CHANNEL_INCREMENT,
    PIZ_GRAPHIC_CHANNEL_DECREMENT,
    //
    PIZ_INFORMATION_BPM,
    PIZ_INFORMATION_CHANCE,
    PIZ_INFORMATION_VELOCITY,
    PIZ_INFORMATION_CHANNEL,
    PIZ_INFORMATION_CHORD,
    PIZ_INFORMATION_CELL,
    PIZ_INFORMATION_VALUE,
    PIZ_INFORMATION_SCALE,
    PIZ_INFORMATION_MUTE,
    PIZ_INFORMATION_ZONE,
    PIZ_INFORMATION_COUNT,
    //
    PIZ_NOTIFICATION_BPM,
    PIZ_NOTIFICATION_CHANCE,
    PIZ_NOTIFICATION_VELOCITY,
    PIZ_NOTIFICATION_CHANNEL,
    PIZ_NOTIFICATION_CHORD,
    PIZ_NOTIFICATION_CELL,
    PIZ_NOTIFICATION_VALUE,
    PIZ_NOTIFICATION_SCALE,
    PIZ_NOTIFICATION_MUTE,
    PIZ_NOTIFICATION_ZONE,
    PIZ_NOTIFICATION_ADDED,
    PIZ_NOTIFICATION_CHANGED,
    PIZ_NOTIFICATION_REMOVED,
    PIZ_NOTIFICATION_PLAYED,
    PIZ_NOTIFICATION_DUMPED,
    PIZ_NOTIFICATION_END,
    PIZ_NOTIFICATION_WILL_END,
    PIZ_NOTIFICATION_WILL_DUMP
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
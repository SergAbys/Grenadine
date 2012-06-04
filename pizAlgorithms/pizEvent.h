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

#define PIZ_EVENT_DATA_SIZE  12

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DEBUGEVENT      \
if (event) {            \
    PIZTime t;          \
    pizTimeSet (&t);    \
    t = t / 1000000.;   \
    post ("%llu / %ld / %s / %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld / %ld / %ld",  \
    t, event->code, __FUNCTION__,   \
    event->data[0],     \
    event->data[1],     \
    event->data[2],     \
    event->data[3],     \
    event->data[4],     \
    event->data[5],     \
    event->data[6],     \
    event->data[7],     \
    event->data[8],     \
    event->data[9],     \
    event->data[10],    \
    event->data[11],    \
    event->option,      \
    event->tag);        \
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_EVENT_RUN                   = 1,
    PIZ_EVENT_LOW,
    PIZ_EVENT_HIGH,
    PIZ_EVENT_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventCode {
    //
    PIZ_EVENT_INIT                  = 0,
    PIZ_EVENT_PLAY                  = 1,
    PIZ_EVENT_STOP                  = 2,
    PIZ_EVENT_LOOP                  = 3,
    PIZ_EVENT_UNLOOP                = 4,
    PIZ_EVENT_BPM                   = 5,
    PIZ_EVENT_LEARN                 = 6,
    PIZ_EVENT_FORGET                = 7,
    PIZ_EVENT_DUMP                  = 8,
    //
    PIZ_EVENT_CHANCE                = 9,
    PIZ_EVENT_VELOCITY              = 10,
    PIZ_EVENT_CHANNEL               = 11,
    PIZ_EVENT_CHORD                 = 12,
    PIZ_EVENT_CELL                  = 13,
    PIZ_EVENT_VALUE                 = 14,
    PIZ_EVENT_SCALE                 = 15,
    PIZ_EVENT_PATTERN               = 16,
    PIZ_EVENT_NOTE                  = 17,
    PIZ_EVENT_ZONE                  = 18,
    PIZ_EVENT_CLEAR                 = 19,
    PIZ_EVENT_CLEAN                 = 20,
    PIZ_EVENT_ROTATE                = 21,
    PIZ_EVENT_SCRAMBLE              = 22,
    PIZ_EVENT_SORT                  = 23,
    PIZ_EVENT_CHANGE                = 24,
    PIZ_EVENT_FILL                  = 25,
    PIZ_EVENT_KILL                  = 26,
    PIZ_EVENT_CYCLE                 = 27, 
    PIZ_EVENT_ZOULOU                = 28,
    PIZ_EVENT_ROMEO                 = 29,
    PIZ_EVENT_JULIET                = 30,
    //    
    PIZ_EVENT_CHANGED_BPM           = 31,
    PIZ_EVENT_CHANGED_CHANCE        = 32,
    PIZ_EVENT_CHANGED_VELOCITY      = 33,
    PIZ_EVENT_CHANGED_CHANNEL       = 34,
    PIZ_EVENT_CHANGED_CHORD         = 35,
    PIZ_EVENT_CHANGED_CELL          = 36,
    PIZ_EVENT_CHANGED_VALUE         = 37,
    PIZ_EVENT_CHANGED_SCALE         = 38,
    PIZ_EVENT_CHANGED_PATTERN       = 39,
    PIZ_EVENT_CHANGED_ZONE          = 40,
    PIZ_EVENT_NOTE_ADDED            = 41,
    PIZ_EVENT_NOTE_CHANGED          = 42,
    PIZ_EVENT_NOTE_REMOVED          = 43,
    PIZ_EVENT_NOTE_PLAYED           = 44,
    PIZ_EVENT_NOTE_DUMPED           = 45,
    PIZ_EVENT_END                   = 46,
    PIZ_EVENT_WILL_END              = 47
    //
    } PIZEventCode;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZEvent {
    PIZEventCode    code;
    PIZEventType    type;
    long            tag;
    long            option;
    long            size;
    long            data[PIZ_EVENT_DATA_SIZE];
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent    *pizEventNew        (PIZEventCode code);

void        pizEventFree        (PIZEvent *x);
void        pizEventSetTag      (PIZEvent *x, long tag);
void        pizEventSetOption   (PIZEvent *x, long option);
void        pizEventSetData     (PIZEvent *x, long argc, const long *argv);

void        pizEventCode        (const PIZEvent *x, PIZEventCode *code);
void        pizEventType        (const PIZEvent *x, PIZEventType *type);
void        pizEventTag         (const PIZEvent *x, long *tag);
void        pizEventOption      (const PIZEvent *x, long *option);
PIZError    pizEventData        (const PIZEvent *x, long *argc, long **argv);
PIZError    pizEventValue       (const PIZEvent *x, long *value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizEventFree (PIZEvent *x)
{
    free (x);
}

PIZ_EXTERN void pizEventSetTag (PIZEvent *x, long tag)
{
    x->tag = tag;
}

PIZ_EXTERN void pizEventSetOption (PIZEvent *x, long option)
{
    x->option = option;
}

PIZ_EXTERN void pizEventCode (const PIZEvent *x, PIZEventCode *code)
{
    (*code) = x->code;
}

PIZ_EXTERN void pizEventType (const PIZEvent *x, PIZEventType *type)
{
    (*type) = x->type;
}

PIZ_EXTERN void pizEventTag (const PIZEvent *x, long *tag)
{
    (*tag) = x->tag;
}

PIZ_EXTERN void pizEventOption (const PIZEvent *x, long *option)
{
    (*option) = x->option;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_EVENT_H
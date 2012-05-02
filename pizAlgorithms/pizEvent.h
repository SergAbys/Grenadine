/**
 * \file	pizEvent.h
 * \author	Jean Sapristi
 * \date	May 2, 2012.
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

#include "pizSequence.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_EVENT_DATA_SIZE 12

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_EVENT_RUN             = 1,
    PIZ_EVENT_TRANSFORM,
    PIZ_EVENT_GRAPHIC,
    PIZ_EVENT_ATTRIBUTE, 
    PIZ_EVENT_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventIdentifier {
    // RUN
    PIZ_EVENT_INIT            = 0,
    PIZ_EVENT_PLAY            = 1,
    PIZ_EVENT_STOP,
    PIZ_EVENT_LOOP,
    PIZ_EVENT_UNLOOP,
    PIZ_EVENT_BPM,
    // TRANSFORM 
    PIZ_EVENT_NOTE,
    PIZ_EVENT_CLEAR,
    PIZ_EVENT_TRANSPOSE,
    PIZ_EVENT_LEARN,
    PIZ_EVENT_ZOULOU,
    PIZ_EVENT_ROMEO,
    PIZ_EVENT_NOVEMBER,
    PIZ_EVENT_JULIET,
    PIZ_EVENT_CLEAN,
    PIZ_EVENT_ROTATE,
    PIZ_EVENT_SCRAMBLE,
    PIZ_EVENT_SORT,
    PIZ_EVENT_CHANGE,
    PIZ_EVENT_SET,
    PIZ_EVENT_RANDOM,
    PIZ_EVENT_KILL,
    PIZ_EVENT_CYCLE,        
    // GRAPHIC     
    
    // ATTRIBUTE 
    PIZ_EVENT_CHANCE,
    PIZ_EVENT_VELOCITY,
    PIZ_EVENT_CHANNEL,
    PIZ_EVENT_CELL,
    PIZ_EVENT_NOTE_VALUE,
    PIZ_EVENT_SCALE,
    PIZ_EVENT_PATTERN,
    // NOTIFICATION 
    PIZ_EVENT_BPM_CHANGED,
    PIZ_EVENT_ZONE_CHANGED,
    PIZ_EVENT_NOTE_ADDED,
    PIZ_EVENT_NOTE_CHANGED,
    PIZ_EVENT_NOTE_REMOVED,
    PIZ_EVENT_NOTE_PLAYED,
    PIZ_EVENT_END,
    PIZ_EVENT_WILL_END
     //
    } PIZEventIdentifier;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZEvent {
    PIZEventType        type;
    PIZEventIdentifier  identifier;
    long                tag;
    long                size;
    long                data[PIZ_EVENT_DATA_SIZE];
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEvent                  (PIZEventIdentifier ie);
PIZEvent *pizEventWithNote          (PIZEventIdentifier ie, const long *argv, long tag);
PIZEvent *pizEventWithZone          (PIZEventIdentifier ie, const long *argv);
PIZEvent *pizEventWithArgs          (PIZEventIdentifier ie, long argc, const long *argv);
PIZEvent *pizEventWithValue         (PIZEventIdentifier ie, long value);
PIZEvent *pizEventCopy              (PIZEvent *x);

void     pizEventFree               (PIZEvent *x);
PIZError pizEventValue              (const PIZEvent *x, long *value);
PIZError pizEventPtr                (const PIZEvent *x, long *argc, long **argv);
void     pizEventName               (const PIZEvent *x, const char **name);
void     pizEventIdentifier         (const PIZEvent *x, PIZEventIdentifier *ie);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew               (PIZEventIdentifier ie, long tag, long argc, const long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN PIZEvent *pizEvent (PIZEventIdentifier ie)
{
    return pizEventNew (ie, PIZ_SEQUENCE_NO_TAG, 0, NULL);
}

PIZ_EXTERN PIZEvent *pizEventWithNote (PIZEventIdentifier ie, const long *argv, long tag)
{
    return pizEventNew (ie, tag, PIZ_SEQUENCE_NOTE_SIZE, argv);
}

PIZ_EXTERN PIZEvent *pizEventWithZone (PIZEventIdentifier ie, const long *argv)
{
    return pizEventNew (ie, PIZ_SEQUENCE_NO_TAG, PIZ_SEQUENCE_ZONE_SIZE, argv);
}

PIZ_EXTERN PIZEvent *pizEventWithArgs (PIZEventIdentifier ie, long argc, const long *argv)
{
    return pizEventNew (ie, PIZ_SEQUENCE_NO_TAG, argc, argv);
}

PIZ_EXTERN PIZEvent *pizEventWithValue (PIZEventIdentifier ie, long value)
{
    return pizEventNew (ie, PIZ_SEQUENCE_NO_TAG, 1, &value);
}

PIZ_EXTERN PIZEvent *pizEventCopy (PIZEvent *x)
{
    return pizEventNew (x->identifier, x->tag, x->size, x->data);
}

PIZ_EXTERN void pizEventFree (PIZEvent *x)
{
    free (x);
}

PIZ_EXTERN void pizEventIdentifier (const PIZEvent *x, PIZEventIdentifier *ie)
{
    (*ie) = x->identifier;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_EVENT_H
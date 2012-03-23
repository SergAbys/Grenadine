/**
 * \file	pizEvent.h
 * \author	Jean Sapristi
 * \date	March 20, 2012.
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

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_EVENT_DATA_VALUES_SIZE  7

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_RUN = 1,
    PIZ_GRAPHIC,
    PIZ_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventName {
// Run
    PIZ_PLAY = 1,
    PIZ_LOOP,
    PIZ_UNLOOP,
    PIZ_STOP,
    PIZ_GRAPHIC_ENABLE,
    PIZ_GRAPHIC_DISABLE,
// Graphic
    PIZ_NOTE_REMOVED,
    PIZ_NOTE_ADDED,
    PIZ_NOTE_CHANGED,
    PIZ_ZONE_CHANGED,
// Notification
    PIZ_END
    } PIZEventName;
    
typedef union _PIZEventData {
    PIZTime time;
    long    values[PIZ_EVENT_DATA_VALUES_SIZE];
} PIZEventData;
    
typedef struct _PIZEvent {
    PIZEventType    type;
    PIZEventName    name;
    long            tag;
    PIZEventData    data; 
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew (void);
void     pizEventFree (PIZEvent *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN PIZEvent *pizEventNew (void)
{
    PIZEvent *event = (PIZEvent *)calloc (1, sizeof(PIZEvent));
    return event;
}

PIZ_EXTERN void pizEventFree (PIZEvent *x)
{
    if (x) {
        free (x);
    }
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_EVENT_H
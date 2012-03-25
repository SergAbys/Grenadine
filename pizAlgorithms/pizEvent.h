/**
 * \file	pizEvent.h
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

#define PIZ_VALUES_SIZE         7
//      PIZ_DATA_NOTE_SIZE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_RUN             = 1,
    PIZ_GRAPHIC,
    PIZ_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventName {
    //
    PIZ_INIT            = 0,
    PIZ_PLAY            = 1,
    PIZ_STOP,
    PIZ_LOOP,
    PIZ_UNLOOP,
    PIZ_BPM,
    PIZ_ENABLE_GUI,
    PIZ_DISABLE_GUI,
    PIZ_NOTE_PLAYED,
    //
    PIZ_NOTE_REMOVED,
    PIZ_NOTE_ADDED,
    PIZ_NOTE_CHANGED,
    PIZ_ZONE_CHANGED,
    //
    PIZ_END,
    PIZ_RUN_READY,
    PIZ_GRAPHIC_READY,
    //
    } PIZEventName;
    
typedef union _PIZEventData {
    PIZTime time;
    long    values[PIZ_VALUES_SIZE];
} PIZEventData;
    
typedef struct _PIZEvent {
    PIZEventType    type;
    PIZEventName    name;
    long            tag;
    PIZEventData    data; 
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew               (PIZEventType type, PIZEventName name);
PIZEvent *pizEventNewWithTime       (PIZEventType type, PIZEventName name, PIZTime *t);
PIZEvent *pizEventNewWithArray      (PIZEventType type, PIZEventName name, long argc, long *argv, long tag);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void     pizEventFree               (PIZEvent *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

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
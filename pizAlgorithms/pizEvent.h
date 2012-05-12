/**
 * \file	pizEvent.h
 * \author	Jean Sapristi
 * \date	May 12, 2012.
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

#define DEBUGEVENT                                                                                          \
                                                                                                            \
if (event) {                                                                                                \
    long        i, argc;                                                                                    \
    long        *argv = NULL;                                                                               \
    const char  *name = NULL;                                                                               \
    PIZTime     t;                                                                                          \
                                                                                                            \
    pizTimeSet (&t);                                                                                        \
    t = t / 1000000.;                                                                                       \
    pizEventNameAsString (event, &name);                                                                    \
                                                                                                            \
    if (!(pizEventPtr (event, &argc, &argv))) {                                                             \
                                                                                                            \
    long a[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };                                                     \
                                                                                                            \
    for (i = 0; i < argc; i++) {                                                                            \
        a[i] = argv[i];                                                                                     \
    }                                                                                                       \
                                                                                                            \
    post ("                     %llu / %s / %s / %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",          \
                                t, name, __FUNCTION__,                                                      \
                                a[0],  a[1], a[2],  a[3],                                                   \
                                a[4],  a[5], a[6],  a[7],                                                   \
                                a[8],  a[9], a[10], a[11] );                                                \
                                                                                                            \
    } else {                                                                                                \
                                                                                                            \
    post ("                     %llu / %s / %s", t, name, __FUNCTION__);                                    \
                                                                                                            \
    }                                                                                                       \
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum _PIZEventType {
    PIZ_EVENT_RUN             = 1,
    PIZ_EVENT_TRANSFORM,
    PIZ_EVENT_GRAPHIC,
    PIZ_EVENT_ATTRIBUTE, 
    PIZ_EVENT_NOTIFICATION
    } PIZEventType;

typedef enum _PIZEventName {
    // RUN
    PIZ_EVENT_INIT            = 0,
    PIZ_EVENT_PLAY            = 1,
    PIZ_EVENT_STOP,
    PIZ_EVENT_LOOP,
    PIZ_EVENT_UNLOOP,
    PIZ_EVENT_BPM,
    // ATTRIBUTE 
    PIZ_EVENT_CHANCE,
    PIZ_EVENT_VELOCITY,
    PIZ_EVENT_CHANNEL,
    PIZ_EVENT_CELL,
    PIZ_EVENT_NOTE_VALUE,
    PIZ_EVENT_SCALE,
    PIZ_EVENT_PATTERN,
    // TRANSFORM 
    PIZ_EVENT_NOTE,
    PIZ_EVENT_CLEAR,
    PIZ_EVENT_TRANSPOSE,
    PIZ_EVENT_CLEAN,
    PIZ_EVENT_LEARN,
    PIZ_EVENT_ZOULOU,
    PIZ_EVENT_ROMEO,
    PIZ_EVENT_NOVEMBER,
    PIZ_EVENT_JULIET,
    PIZ_EVENT_ROTATE,
    PIZ_EVENT_SCRAMBLE,
    PIZ_EVENT_SORT,
    PIZ_EVENT_CHANGE,
    PIZ_EVENT_FILL,
    PIZ_EVENT_RANDOM,
    PIZ_EVENT_KILL,
    PIZ_EVENT_CYCLE,        
    // GRAPHIC     
    
    // NOTIFICATION 
    PIZ_EVENT_CHANGED_BPM,
    PIZ_EVENT_CHANGED_CHANCE,
    PIZ_EVENT_CHANGED_VELOCITY,
    PIZ_EVENT_CHANGED_CHANNEL,
    PIZ_EVENT_CHANGED_CELL,
    PIZ_EVENT_CHANGED_NOTE_VALUE,
    PIZ_EVENT_CHANGED_SCALE,
    PIZ_EVENT_CHANGED_PATTERN,
    PIZ_EVENT_CHANGED_ZONE,
    //
    PIZ_EVENT_NOTE_ADDED,
    PIZ_EVENT_NOTE_CHANGED,
    PIZ_EVENT_NOTE_REMOVED,
    PIZ_EVENT_NOTE_PLAYED,
    //
    PIZ_EVENT_END,
    PIZ_EVENT_WILL_END
     //
    } PIZEventName;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZEvent {
    PIZEventName    name;
    PIZEventType    type;
    long            tag;
    long            size;
    long            data[PIZ_EVENT_DATA_SIZE];
    } PIZEvent;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZEvent *pizEventNew           (PIZEventName name, long tag, long argc, const long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void     pizEventFree           (PIZEvent *x);
void     pizEventName           (const PIZEvent *x, PIZEventName *name);
PIZError pizEventValue          (const PIZEvent *x, long *value);
PIZError pizEventPtr            (const PIZEvent *x, long *argc, long **argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void     pizEventNameAsString   (const PIZEvent *x, const char **name);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizEventFree (PIZEvent *x)
{
    free (x);
}

PIZ_EXTERN void pizEventName (const PIZEvent *x, PIZEventName *name)
{
    (*name) = x->name;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_EVENT_H
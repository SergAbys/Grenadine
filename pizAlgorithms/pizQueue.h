/**
 * \file    pizQueue.h
 * \author  Jean Sapristi
 * \date    28 February 2012
 */

/*
 *  Copyright (c) 2011, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *  This software is governed by the CeCILL-C license under French law and
 *  abiding by the rules of distribution of free software. You can use, 
 *  modify and/or redistribute the software under the terms of the CeCILL-C
 *  license as circulated by CEA, CNRS and INRIA at the following URL
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and rights to copy,
 *  modify and redistribute granted by the license, users are provided only
 *  with a limited warranty  and the software's author, the holder of the
 *  economic rights, and the successive licensors have only limited
 *  liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated
 *  with loading, using, modifying and/or developing or reproducing the
 *  software by the user in light of its specific status of free software,
 *  that may mean that it is complicated to manipulate, and that also
 *  therefore means that it is reserved for developers and experienced
 *  professionals having in-depth computer knowledge. Users are therefore
 *  encouraged to load and test the software's suitability as regards their
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and, more generally, to use and operate it in the 
 *  same conditions as regards security. 
 *  
 *  The fact that you are presently reading this means that you have had
 *  knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef PIZ_QUEUE_H
#define PIZ_QUEUE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZQueueElement {
    long                    value;
    struct _PIZQueueElement *next;
    struct _PIZQueueElement *previous;
    } PIZQueueElement;

typedef struct _PIZQueue {
    long            count;
    long            poppedValue;
    PIZQueueElement *head;
    PIZQueueElement *tail;
    } PIZQueue;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

PIZQueue    *pizQueueNew            (void); 
void        pizQueueFree            (PIZQueue *x);
void        pizQueueClear           (PIZQueue *x);
PIZError    pizQueueAppend          (PIZQueue *x, long value);
PIZError    pizQueuePop             (PIZQueue *x);
PIZError    pizQueuePopLastValue    (PIZQueue *x);
long        pizQueueCount           (const PIZQueue *x);
long        pizQueuePoppedValue     (const PIZQueue *x);

PIZ_END_C_LINKAGE
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN long pizQueueCount (const PIZQueue *x)
{
    return (x->count);
}

PIZ_EXTERN long pizQueuePoppedValue (const PIZQueue *x)
{
    return (x->poppedValue);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_QUEUE_H
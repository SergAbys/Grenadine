/**
 * \file    pizQueue.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup structures
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

/**
 * \brief Linklist element.
 */
 
typedef struct _PIZQueueElement {
    long                    value;                  /*!< Value to store. */
    struct _PIZQueueElement *next;                  /*!< Pointer to next element. */
    struct _PIZQueueElement *previous;              /*!< Pointer to previous element. */
    } PIZQueueElement;

/**
 * \brief The FIFO queue (linklist).
 */
 
typedef struct _PIZQueue {
    long            count;                          /*!< Number of value in the queue. */
    long            poppedValue;                    /*!< Last dequeued value. */
    PIZQueueElement *head;                          /*!< Pointer to first element. */
    PIZQueueElement *tail;                          /*!< Pointer to last element. */
    } PIZQueue;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the queue.
 * \details In case of failure the pointer is NULL.
 * \return  A pointer to the new queue.
 */
PIZQueue *pizQueueNew (void); 

/**
 * \brief   Free the queue.
 * \details It is safe to pass NULL pointer.
 * \param   x A Pointer.
 */
void pizQueueFree (PIZQueue *x);

/**
 * \brief   Clear the queue.
 * \param   x A valid pointer.
 */
void pizQueueClear (PIZQueue *x);

/**
 * \brief   Enqueue a value.
 * \param   x A valid pointer.
 * \param   value The value to append.
 * \return  An error code.
 */
PIZError pizQueueAppend (PIZQueue *x, long value);

/**
 * \brief   Dequeue a value.
 * \details To get the dequeued value use pizQueuePoppedValue(). 
 *          Return PIZ_ERROR if the queue is empty.
 * \param   x A valid pointer.
 * \return  An error code.
 */
PIZError pizQueuePop (PIZQueue *x);

/**
 * \brief   Dequeue last value (stack way).
 * \details To get the dequeued value use pizQueuePoppedValue(). 
 *          Return PIZ_ERROR if the queue is empty.
 * \param   x A valid pointer.
 * \return  An error code.
 */
PIZError pizQueuePopLastValue (PIZQueue *x);

/**
 * \brief   Get the number of values in the queue.
 * \param   x A valid pointer.
 * \return  The number of values.
 */
long pizQueueCount (const PIZQueue *x);

/**
 * \brief   Get the dequeued value.
 * \details This value is initialized with -1. 
 * \param   x A valid pointer.
 * \return  The value.
 */
long pizQueuePoppedValue (const PIZQueue *x);

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
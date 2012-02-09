/**
 * \file    pizBoundedStack.h
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

#ifndef PIZ_BOUNDED_STACK_H
#define PIZ_BOUNDED_STACK_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \remark  Implemented with an array.
 */
 
typedef struct _PIZBoundedStack {
    long bound;                             /*!< Maximum number of value in the queue. */
    long stack;                             /*!< Number of value in the queue. */
    long poppedValue;                       /*!< Last unstacked value. */
    long *values;                           /*!< Pointer to the array. */
    } PIZBoundedStack; 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the stack.
 * \details In case of failure the pointer is NULL.
 * \param   size The maximum number of values in the stack.
 * \return  A pointer to the new queue.
 */
PIZBoundedStack *pizBoundedStackNew (long size);

/**
 * \brief   Free the stack.
 * \details It is safe to pass NULL pointer.
 * \param   x A Pointer.
 */
void pizBoundedStackFree (PIZBoundedStack *x);

/**
 * \brief   Clear the stack.
 * \param   x A valid pointer.
 */
void pizBoundedStackClear (PIZBoundedStack *x);

/**
 * \brief   Stack a value.
 * \details Return PIZ_ERROR if the number of values is over the size of the stack.
 * \param   x A valid pointer.
 * \param   value The value to append.
 * \return  An error code.
 */
PIZError pizBoundedStackPush (PIZBoundedStack *x, long value);

/**
 * \brief   Unstack a value.
 * \details To get the unstacked value use pizBoundedStackPoppedValue().
 *          Return PIZ_ERROR if the queue is empty.
 * \param   x A valid pointer.
 * \return  An error code.
 */
PIZError pizBoundedStackPop (PIZBoundedStack *x);

/**
 * \brief   Get the number of values in the stack.
 * \param   x A valid pointer.
 * \return  The number of values.
 */
long pizBoundedStackCount (const PIZBoundedStack *x);

/**
 * \brief   Get the unstacked value.
 * \remark  This value is initialized with -1. 
 * \param   x A valid pointer.
 * \return  The value.
 */
long pizBoundedStackPoppedValue (const PIZBoundedStack *x);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizBoundedStackClear (PIZBoundedStack *x)
{
    x->stack        = 0;
    x->poppedValue  = -1;
}

PIZ_EXTERN long pizBoundedStackCount (const PIZBoundedStack *x)
{
    return (x->stack);
}

PIZ_EXTERN long pizBoundedStackPoppedValue (const PIZBoundedStack *x)
{
    return (x->poppedValue);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_BOUNDED_STACK_H
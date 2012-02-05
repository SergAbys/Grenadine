/**
 * \file    pizArrayPool.h
 * \author  Jean Sapristi
 * \date    15 janvier 2012
 * \remark  Multithreaded with single global mutex.
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

#ifndef PIZ_ARRAY_POOL_H
#define PIZ_ARRAY_POOL_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"
#include "pizLinklist.h"
#include "pizGrowingArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <pthread.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   The pool of dynamic arrays.
 * \details Implemented with a linklist.
 * \warning It is currently a very simple approach, and should be improved ;
 *          If an array is not released, the pool will grow dramatically for each query.
 */
 
typedef struct _PIZArrayPool {
    pthread_mutex_t     lock;                       /*!< POSIX mutex. */
    long                retain;                     /*!< Number of arrays in use. */
    long                initArraySize;              /*!< Memory size of new array. */
    PIZGrowingArray     *cache;                     /*!< Pointer to the last array delivered. */
    PIZLinklist         *pool;                      /*!< Pointer to the pool's linklist. */
    } PIZArrayPool;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the pool of dynamic arrays.
 * \details The function accepts two arguments : the number of pre-allocated arrays (default is 2)
 *          and the size of the memory to initialize them. 
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer to arguments.
 * \return  A pointer to the new pool of dynamic arrays.
 * \remark	The following shows how to create a pool of dynamic arrays.  
 * \code
 * long            args[2] = {4, 32};
 * PIZArrayPool    *pool = NULL;
 *
 * pool = pizArrayPoolNew (2, args);  
 * pool = pizArrayPoolNew (0, NULL);  // default values.
 *
 * \endcode
 */
PIZArrayPool *pizArrayPoolNew (long argc, long *argv);

/**
 * \brief   Free the pool of dynamic arrays.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizArrayPoolFree (PIZArrayPool *x);

/**
 * \brief   Get a dynamic array.
 * \details In case of failure the pointer is NULL.
 * \param   x A valid pointer.
 * \return  A pointer to the dynamic array.
 */
PIZGrowingArray *pizArrayPoolGetArray (PIZArrayPool *x);

/**
 * \brief   Release a dynamic array.
 * \param   x A valid pointer.
 * \param   array A pointer to the dynamic array.
 * \return  An error code.
 */
PIZError pizArrayPoolReleaseArray (PIZArrayPool *x, PIZGrowingArray *array);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_ARRAY_POOL_H
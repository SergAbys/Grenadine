/**
 * \file    pizGrowingArray.h
 * \author  Jean Sapristi
 * \date    26 janvier 2012
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

#ifndef PIZ_GROWING_ARRAY_H
#define PIZ_GROWING_ARRAY_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief The dynamic array.
 */
 
typedef struct _PIZGrowingArray {
    long size;                                  /*!< Current size of the array. */
    long index;                                 /*!< Number of values in the array. */
    long *values;                               /*!< Pointer to the array. */
    } PIZGrowingArray;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the dynamic array.
 * \param   size The size of memory allocated initially.
 * \details In case of failure the pointer is NULL.
 * \return  A pointer to the new dynamic array.
 */
PIZGrowingArray *pizGrowingArrayNew (long size);

/**
 * \brief   Free the dynamic array.
 * \details It is safe to pass NULL pointer.
 * \param   x A Pointer.
 */
void pizGrowingArrayFree (PIZGrowingArray *x);

/**
 * \brief   Clear the dynamic array.
 * \param   x A valid pointer.
 */
void pizGrowingArrayClear (PIZGrowingArray *x);

/**
 * \brief   Append a value to the array.
 * \details In case of overtaking the memory is doubled.
 *          Return PIZ_MEMORY if the memory can not be allocated.
 * \param   x A valid pointer.
 * \param   value The value to append.
 * \return  An error code.
 */
PIZError pizGrowingArrayAppend (PIZGrowingArray *x, long value);

/**
 * \brief   Set the value at a specified index.
 * \param   x A valid pointer.
 * \param   index The index.
 * \param   value The value.
 * \warning For efficiency the index is NOT checked ; so crash may occur with invalid indexing.
 */
void pizGrowingArraySetValueAtIndex (PIZGrowingArray *x, long index, long value);

/**
 * \brief   Get the number of values in the array.
 * \param   x A valid pointer.
 * \return  The number of values.
 */
long pizGrowingArrayCount (const PIZGrowingArray *x);

/**
 * \brief   Get the value at a specified index.
 * \param   x A valid pointer.
 * \param   index The index.
 * \return  The value.
 * \warning For efficiency the index is NOT checked ; so crash may occur with invalid indexing.
 */
long pizGrowingArrayValueAtIndex (const PIZGrowingArray *x, long index);

/**
 * \brief   Get the pointer to internal array.
 * \param   x A valid pointer.
 * \return  The pointer.
 * \warning This function is provided for efficiency but it should be used carefully.
 */
long *pizGrowingArrayPtr (const PIZGrowingArray *x); 

/**
 * \brief   Remove the value at a specified index.
 * \param   x A valid pointer.
 * \param   index The index.
 */
void pizGrowingArrayRemoveIndex (PIZGrowingArray *x, long index);

/**
 * \brief   Remove the last value.
 * \param   x A valid pointer.
 */
PIZError pizGrowingArrayRemoveLastValue (PIZGrowingArray *x);

/**
 * \brief   Given a value, get the index of the first equal value found.
 * \param   x A valid pointer.
 * \param   value The value to find.
 * \return  The index if found, otherwise -1.
 */
long pizGrowingArrayFirstIndexOfValue (const PIZGrowingArray *x, long value);

/**
 * \brief   To know if the array countains the provided value.
 * \param   x A valid pointer.
 * \param   value The value to find.
 * \return  True if found, otherwise false.
 */
bool pizGrowingArrayContainsValue (const PIZGrowingArray *x, long value);


PIZError            pizGrowingArrayCopy              (PIZGrowingArray *x, const PIZGrowingArray *toCopy);
PIZError            pizGrowingArrayAppendArray       (PIZGrowingArray *x, const PIZGrowingArray *toAppend);
PIZError            pizGrowingArrayAppendPtr         (PIZGrowingArray *x, long argc, long *argv);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizGrowingArrayClear (PIZGrowingArray *x)
{
    x->index = 0;
}

PIZ_EXTERN void pizGrowingArraySetValueAtIndex (PIZGrowingArray *x, long index, long value)
{
    x->values[index] = value;
}

PIZ_EXTERN long pizGrowingArrayCount (const PIZGrowingArray *x)
{
    return (x->index);
}

PIZ_EXTERN long pizGrowingArrayValueAtIndex (const PIZGrowingArray *x, long index)
{   
    return (x->values[index]);
}

PIZ_EXTERN long *pizGrowingArrayPtr (const PIZGrowingArray *x)
{
    return (x->values);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_GROWING_ARRAY_H
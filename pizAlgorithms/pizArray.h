/**
 * \file    pizArray.h
 * \author  Jean Sapristi
 * \date    April 6, 2012.
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

#ifndef PIZ_ARRAY_H
#define PIZ_ARRAY_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZArray {
    long size;
    long index;
    long *values;
    } PIZArray;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZArray    *pizArrayNew                 (long size);
void        pizArrayFree                 (PIZArray *x);
void        pizArrayClear                (PIZArray *x);
PIZError    pizArrayAppend               (PIZArray *x, long value);
void        pizArraySetValueAtIndex      (PIZArray *x, long index, long value);
long        pizArrayValueAtIndex         (const PIZArray *x, long index);
long        pizArrayCount                (const PIZArray *x);
long        *pizArrayPtr                 (const PIZArray *x); 
void        pizArrayRemoveIndex          (PIZArray *x, long index);
PIZError    pizArrayRemoveLastValue      (PIZArray *x);
long        pizArrayFirstIndexOfValue    (const PIZArray *x, long value);
bool        pizArrayContainsValue        (const PIZArray *x, long value);
PIZError    pizArrayCopy                 (PIZArray *x, const PIZArray *toCopy);
PIZError    pizArrayAppendArray          (PIZArray *x, const PIZArray *toAppend);
PIZError    pizArrayAppendPtr            (PIZArray *x, long argc, long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizArrayClear (PIZArray *x)
{
    x->index = 0;
}

PIZ_EXTERN void pizArraySetValueAtIndex (PIZArray *x, long index, long value)
{
    x->values[index] = value;
}

PIZ_EXTERN long pizArrayValueAtIndex (const PIZArray *x, long index)
{   
    return (x->values[index]);
}

PIZ_EXTERN long pizArrayCount (const PIZArray *x)
{
    return (x->index);
}

PIZ_EXTERN long *pizArrayPtr (const PIZArray *x)
{
    return (x->values);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_ARRAY_H
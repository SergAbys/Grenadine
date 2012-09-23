/*
 * \file    pizArray.c
 * \author  Jean Sapristi
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

#include "pizArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_SIZE_ARRAY    4

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZArray *pizArrayNew(long size)
{
    PIZArray *x = NULL;

    if (x = (PIZArray *)malloc(sizeof(PIZArray))) {  
    //     
    x->size = PIZ_DEFAULT_SIZE_ARRAY;
    
    if (size > 0) {
        x->size = size;
    }
    
    if (x->values = (long *)malloc(x->size * sizeof(long))) {
        x->index = 0;
    } else {
        free(x);
        x = NULL;
    }
    //
    }
    
    return x;
}

void pizArrayFree(PIZArray *x)
{
    if (x) {
        free(x->values);
        x->values = NULL;
            
        free(x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizArrayAppend(PIZArray *x, long value)
{   
    PIZError err = PIZ_GOOD;
        
    if (x->index == x->size) {   
        long *newValues = NULL;
        
        if (newValues = (long *)realloc(x->values, (x->size * 2) * sizeof(long))) {
            x->size *= 2;
            x->values = newValues;
        } else {
            err = PIZ_MEMORY;
        }
    }
    
    if (!err) {
        x->values[x->index] = value;
        x->index ++;
    }
        
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#ifndef PIZ_EXTERN_INLINE

void pizArrayClear(PIZArray *x)
{
    x->index = 0;
}

void pizArraySetAtIndex(PIZArray *x, long index, long value)
{
    x->values[index] = value;
}

long pizArrayAtIndex(const PIZArray *x, long index)
{   
    return x->values[index];
}

long pizArrayCount(const PIZArray *x)
{
    return x->index;
}

long *pizArrayPtr(const PIZArray *array)
{
    return array->values;
}

void pizArrayRemoveLast(PIZArray *x)
{
    x->index --;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizArrayRemoveAtIndex(PIZArray *array, long index)
{
    long i;
            
    for (i = index; i < (array->index - 1); i++) {
        array->values[i] = array->values[i + 1];
    }
    
    array->index --;
}

bool pizArrayContainsValue(const PIZArray *x, long value)
{
    long i;
    bool k = false;
    
    for (i = 0; i < x->index; i++) {
        if (x->values[i] == value) {
            k = true;
            break;
        }
    }
        
    return k;
}

PIZError pizArrayIndexOfValue(const PIZArray *x, long value, long *index)
{
    long i;
    PIZError err = PIZ_ERROR;
    
    for (i = 0; i < x->index; i++) {
        if (x->values[i] == value) {
            (*index) = i;
            err = PIZ_GOOD;
            break;
        }
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
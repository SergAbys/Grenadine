/*
 * \file    pizArray.c
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

#include "pizArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_SIZE 4

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZArray *pizArrayNew (long size)
{
    PIZArray *x = NULL;

    if (x = (PIZArray *)malloc (sizeof(PIZArray))) {       
        x->size = PIZ_DEFAULT_SIZE;
        
        if (size > 0) {
            x->size = size;
        }
        
        if (x->values = (long *)malloc (x->size * sizeof(long))) {
            x->index = 0;
        } else {
            free (x);
            x = NULL;
        }
    }
    
    return x;
}

void pizArrayFree (PIZArray *x)
{
    if (x) {
        free (x->values);
        x->values = NULL;
            
        free (x);
    }
}

void pizArrayClear (PIZArray *x)
{
    x->index = 0;
}

PIZError pizArrayAppend (PIZArray *x, long value)
{   
    PIZError err = PIZ_GOOD;
        
    if (x->index == x->size) {   
        long *newValues = NULL;
        
        if (newValues = (long *)realloc (x->values, (x->size * 2) * sizeof(long))) {
            x->size *= 2;
            x->values = newValues;
        } else {
            err = PIZ_MEMORY;
        }
    }
    
    if (err == PIZ_GOOD) {
        x->values[x->index] = value;
        x->index ++;
    }
        
    return err;
}

void pizArraySetValueAtIndex (PIZArray *x, long index, long value)
{
    x->values[index] = value;
}

long pizArrayValueAtIndex (const PIZArray *x, long index)
{   
    return (x->values[index]);
}

long pizArrayCount (const PIZArray *x)
{
    return (x->index);
}

long *pizArrayPtr (const PIZArray *array)
{
    return (array->values);
}

void pizArrayRemoveIndex (PIZArray *array, long index)
{
    if (index >= 0 && index < array->index) {
        long i;
        
        for (i = index; i < (array->index - 1); i++) {
            array->values[i] = array->values[i + 1];
        }
        
        array->index --;
    }
}

PIZError pizArrayRemoveLastValue (PIZArray *x)
{
    PIZError err = PIZ_ERROR;
    
    if (x->index) {
        x->index --;
        err = PIZ_GOOD;
    }
        
    return err;
}

long pizArrayFirstIndexOfValue (const PIZArray *x, long value)
{
    long i, k = -1;
    
    for (i = 0; i < x->index; i++) {
        if (x->values[i] == value) {
            k = i;
            break;
        }
    }
    
    return k;
}

bool pizArrayContainsValue (const PIZArray *x, long value)
{
    long i, k = false;
    
    for (i = 0; i < x->index; i++) {
        if (x->values[i] == value) {
            k = true;
            break;
        }
    }
        
    return k;
}

PIZError pizArrayCopy (PIZArray *x, const PIZArray *toCopy)
{
    PIZError err = PIZ_GOOD;
    
    if (toCopy->index > x->size) {
        long *newValues = NULL;
        
        if (newValues = (long *)realloc (x->values, toCopy->size * sizeof(long))) {
            x->size = toCopy->size;
            x->values = newValues;
        } else {
            err = PIZ_MEMORY;
        }
    }
    
    if (!err)  {
        long i;
        
        for (i = 0; i < toCopy->index; i++) {
            x->values[i] = toCopy->values[i];
        }
            
        x->index = toCopy->index;
    }
    
    return err;
}

PIZError pizArrayAppendArray (PIZArray *x, const PIZArray *toAppend)
{
    PIZError err = PIZ_GOOD;
    
    if ((toAppend->index + x->index) > x->size) {
        long *newValues = NULL;
        long newSize = toAppend->size + x->size;

        if (newValues = (long *)realloc (x->values, newSize * sizeof(long))) {
            x->size = newSize;
            x->values = newValues;
        } else {
            err = PIZ_MEMORY;
        }
    }
    
    if (!err)  {
        long i;
    
        for (i = 0; i < toAppend->index; i++) {
            x->values[x->index + i] = toAppend->values[i];
        }
        
        x->index += toAppend->index;
    }
    
    return err;
}

PIZError pizArrayAppendPtr (PIZArray *x, long argc, long *argv)
{
    PIZError err = PIZ_GOOD;
    
    if ((argc + x->index) > x->size) {
        long *newValues = NULL;
        long newSize = x->size * 2;
        
        while (newSize < (argc + x->index)) {
            newSize = newSize * 2;
        }

        if (newValues = (long *)realloc (x->values, newSize * sizeof(long))) {
            x->size = newSize;
            x->values = newValues;
        } else {
            err = PIZ_MEMORY;
        }
    }
    
    if (!err) {
        long i;
        
        for (i = 0; i < argc; i++) {
            x->values[x->index + i] = argv[i];
        }
            
        x->index += argc;
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
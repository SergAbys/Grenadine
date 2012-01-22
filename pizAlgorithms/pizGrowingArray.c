/*
 * \file    pizGrowingArray.c
 * \author  Jean Sapristi
 * \date    15 janvier 2012
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

#include "pizGrowingArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZGrowingArray *pizGrowingArrayNew (long size)
{
    PIZGrowingArray *x = NULL;

    if (x = (PIZGrowingArray *)malloc (sizeof(PIZGrowingArray)))
        {       
            size = MAX (1, size);
            
            if (x->growingArrayValues = (long *)malloc (size * sizeof(long)))
                {
                    x->size     = size;
                    x->index    = 0;
                }
            else
                {
                    free (x);
                    x = NULL;
                }
        }
    
    return x;
}

void pizGrowingArrayFree (PIZGrowingArray *x)
{
    if (x)
        {
            free (x->growingArrayValues);
            x->growingArrayValues = NULL;
            
            free (x);
        }
}

void pizGrowingArrayClear (PIZGrowingArray *x)
{
    x->index = 0;
}

PIZError pizGrowingArrayAppend (PIZGrowingArray *x, long value)
{   
    long err = PIZ_GOOD;
        
    if (x->index == x->size)
        {   
            long *newGrowingArrayValues = NULL;
            
            if (newGrowingArrayValues = (long *)realloc (x->growingArrayValues, (x->size * 2) * sizeof(long)))
                {
                    x->size *= 2;
                    x->growingArrayValues = newGrowingArrayValues;
                }
            else
                {
                    err = PIZ_MEMORY;
                }
        }
    
    if (err == PIZ_GOOD)
        {
            x->growingArrayValues[x->index] = value;
            x->index ++;
        }
        
    return err;
}

long pizGrowingArrayCount (PIZGrowingArray *x)
{
    return (x->index);
}

long pizGrowingArrayValueAtIndex (PIZGrowingArray *x, long index)
{   
    return (x->growingArrayValues[index]);
}

void pizGrowingArraySetValueAtIndex (PIZGrowingArray *x, long index, long value)
{
    x->growingArrayValues[index] = value;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
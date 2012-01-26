/*
 * \file    pizBoundedQueue.c
 * \author  Jean Sapristi
 * \date    23 janvier 2012
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

#include "pizBoundedQueue.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZBoundedQueue *pizBoundedQueueNew (long size)
{
    PIZBoundedQueue *x = NULL;
    
    if (size > 0 && (x = (PIZBoundedQueue *)malloc (sizeof(PIZBoundedQueue))))
        {
            if (x->values = (long *)malloc ((size + 1) * sizeof(long)))
                {
                    x->count        = 0;
                    x->bound        = size;
                    x->head         = 0;
                    x->tail         = 0;
                    x->poppedValue  = -1;
                }
            else
                {
                    free (x);
                    x = NULL;
                }
        }
    
    return x;
}

void pizBoundedQueueFree (PIZBoundedQueue *x)
{
    if (x) {
            free (x->values);
            x->values = NULL;
            
            free (x);
        }
}

void pizBoundedQueueClear (PIZBoundedQueue *x)
{
    x->count    = 0;
    x->head     = 0;
    x->tail     = 0;
}

PIZError pizBoundedQueueAppend (PIZBoundedQueue *x, long value) 
{   
    long err = PIZ_ERROR;
    
    if (((x->tail + 1) != x->head) && !((x->tail == x->bound) && (x->head == 0)))
        {
            err = PIZ_GOOD;
            
            x->count ++;
            
            x->values[x->tail] = value;
            
            if (x->tail == x->bound) {
                x->tail = 0;
            } else {
                x->tail ++;
            }
        }
    
    return err;
}

PIZError pizBoundedQueuePop (PIZBoundedQueue *x)
{
    long err = PIZ_ERROR;
    
    if (x->head != x->tail)
        {
            err = PIZ_GOOD;
            
            x->count --;
            
            x->poppedValue = x->values[x->head];
            
            if (x->head == x->bound) {
                x->head = 0;
            } else {
                x->head ++;
            }
        }
    
    return err;
}

PIZError pizBoundedQueuePopLastValue (PIZBoundedQueue *x)
{
    long err = PIZ_ERROR;
    
    if (x->tail != x->head)
        {
            err = PIZ_GOOD;
            
            if (x->tail == 0) {
                x->tail = x->bound;
            } else {
                x->tail --;
            }
            
            x->count --;
                    
            x->poppedValue = x->values[x->tail];
        }
    
    return err;
}

long pizBoundedQueueCount (const PIZBoundedQueue *x)
{
    return (x->count);
}

long pizBoundedQueuePoppedValue (const PIZBoundedQueue *x)
{
    return (x->poppedValue);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
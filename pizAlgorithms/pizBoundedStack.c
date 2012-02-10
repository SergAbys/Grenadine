/*
 * \file    pizBoundedStack.c
 * \author  Jean Sapristi
 * \date    31 janvier 2012
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

#include "pizBoundedStack.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZBoundedStack *pizBoundedStackNew (long size)
{
    PIZBoundedStack *x = NULL;
    
    if (size > 0 && (x = (PIZBoundedStack *)malloc (sizeof(PIZBoundedStack))))
        {
            if (x->values = (long *)malloc (size * sizeof(long)))
                {
                    x->bound        = size;
                    x->stack        = 0;
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

void pizBoundedStackFree (PIZBoundedStack *x)
{
    if (x) {
            free (x->values);
            x->values = NULL;
            
            free (x);
        }
}

void pizBoundedStackClear (PIZBoundedStack *x)
{
    x->stack        = 0;
    x->poppedValue  = -1;
}

PIZError pizBoundedStackPush (PIZBoundedStack *x, long value) 
{   
    long err = PIZ_ERROR;
    
    if (x->stack < x->bound) 
        {
            err = PIZ_GOOD;
            
            x->values[x->stack] = value;
            x->stack ++;
        }
    
    return err;
}

PIZError pizBoundedStackPop (PIZBoundedStack *x)
{
    long err = PIZ_ERROR;
    
    if (x->stack) 
        {
            err = PIZ_GOOD;
            
            x->poppedValue = x->values[x->stack - 1];
            
            x->stack --;
        }
    
    return err;
}

long pizBoundedStackCount (const PIZBoundedStack *x)
{
    return (x->stack);
}

long pizBoundedStackPoppedValue (const PIZBoundedStack *x)
{
    return (x->poppedValue);
}   

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
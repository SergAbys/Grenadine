/*
 * \file    pizArrayPool.c
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

#include "pizArrayPool.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DEFAULT_PREALLOCATED_ARRAYS     2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZLOCK     pthread_mutex_lock (&x->lock);
#define PIZUNLOCK   pthread_mutex_unlock (&x->lock);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZArrayPool *pizArrayPoolNew (long argc, long *argv)
{
    PIZArrayPool *x = NULL;
    
    if (x = (PIZArrayPool *)malloc (sizeof(PIZArrayPool)))
        {   
            long k;
            
            x->retain           = 0;
            x->initArraySize    = 0;
            x->cache            = NULL;
            
            if (argc) {
                    k = MAX (argv[0], DEFAULT_PREALLOCATED_ARRAYS);
                }
                
            if (argc > 1) {
                    x->initArraySize = MAX (argv[1], 0);
                }
            
            if (x->pool = pizLinklistNew ( ))
                {
                    long i;
                    
                    pizLinklistSetFlags (x->pool, PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY);
                    
                    for (i = 0; i < k; i++)
                        {
                            PIZGrowingArray *array = NULL;
                            
                            if (array = pizGrowingArrayNew (x->initArraySize)) {
                                    pizLinklistAppend (x->pool, array);
                                }
                        }
                    
                    pthread_mutex_init (&x->lock, NULL);
                }
            else
                {
                    free (x);
                    x = NULL;
                }
        }
    
    return x;
}

void pizArrayPoolFree (PIZArrayPool *x)
{
    if (x) 
        {
            PIZLOCK
            pizLinklistFree (x->pool);
            PIZUNLOCK
            
            pthread_mutex_destroy (&x->lock);
            free (x);
        }
}

PIZGrowingArray *pizArrayPoolGetArray (PIZArrayPool *x)
{
    long            err = PIZ_ERROR;
    PIZGrowingArray *array = NULL;
    
    PIZLOCK
    
    if (!x->retain) 
        {
            err = pizLinklistPtrAtIndex (x->pool, 0, (void **)&array);
        }
    else 
        {
            err = pizLinklistNextByPtr (x->pool, (void *)x->cache, (void **)&array);
        }
    
    if (err && (array = pizGrowingArrayNew (x->initArraySize))) {
            err = pizLinklistAppend (x->pool, array);
        }
    
    if (!err) {
            pizGrowingArrayClear (array);
            x->retain ++;
            x->cache = array;
        }
    
    PIZUNLOCK
        
    return array;
}

void pizArrayPoolReleaseArray (PIZArrayPool *x, PIZGrowingArray *array)
{
    PIZLOCK
    
    if (array) {
            x->retain --;
        }
    
    PIZUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file    pizBoundedHashTable.c
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

#include "pizBoundedHashTable.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_HASH_SIZE   157
#define PIZ_DEFAULT_POOL_SIZE   128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZBoundedHashTable *pizBoundedHashTableNew (long argc, long *argv)
{
    PIZBoundedHashTable *x = NULL;
        
    if (x = (PIZBoundedHashTable *)malloc (sizeof(PIZBoundedHashTable))) {
        x->flags    = PIZ_BOUNDED_HASHTABLE_FLAG_NONE;
        x->count    = 0;
        x->hashSize = PIZ_DEFAULT_HASH_SIZE;
        x->poolSize = PIZ_DEFAULT_POOL_SIZE;
        
        if (argc && argv[0] > 0) {
            x->hashSize = argv[0];
        }
        
        if ((argc > 1) && argv[1] > 0) {
            x->poolSize = argv[1];
        }
        
        x->ticketMachine = pizBoundedStackNew (x->poolSize);
        
        x->pool = (PIZBoundedHashTableElement *)malloc (x->poolSize * sizeof(PIZBoundedHashTableElement));
        x->hashTable = (PIZArray **)calloc (x->hashSize, sizeof(PIZArray *));
        
        if (x->ticketMachine && x->pool && x->hashTable) {
            long j;
                    
            for (j = (x->poolSize - 1); j >= 0; j--) {
                pizBoundedStackPush (x->ticketMachine, j);
            }
        } else {
            pizBoundedHashTableFree (x);
            x = NULL;
        }
    }
    
    return x;
}

void pizBoundedHashTableSetFlags (PIZBoundedHashTable *x, PIZFlags flags)
{
    x->flags = flags;
}

void pizBoundedHashTableFree (PIZBoundedHashTable *x)
{
    if (x) {
        pizBoundedStackFree (x->ticketMachine);
        
        if (x->pool) {
            free (x->pool);
            x->pool = NULL;
        }
            
        if (x->hashTable) {
            long i;
    
            for (i = 0; i < x->hashSize; i++) {
                pizArrayFree (x->hashTable[i]);
            }
    
            free (x->hashTable);
            x->hashTable = NULL;
        }
        
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizBoundedHashTableClear (PIZBoundedHashTable *x)
{
    long i;
    
    pizBoundedStackClear (x->ticketMachine);
    
    for (i = (x->poolSize - 1); i >= 0; i--) {
        pizBoundedStackPush (x->ticketMachine, i);
    }
    
    for (i = 0; i < x->hashSize; i++) {
        if (x->hashTable[i]) {
            pizArrayClear (x->hashTable[i]);
        }
    }
    
    x->count = 0;
}

PIZError pizBoundedHashTableAdd (PIZBoundedHashTable *x, long key, void *ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (ptr && (key >= 0)) {
        long p = key % x->hashSize;
                    
        if (!x->hashTable[p]) {
            x->hashTable[p] = pizArrayNew (0);
        }
        
        if (x->hashTable[p]) {
            if (!pizBoundedStackPop (x->ticketMachine)) {
                long index = pizBoundedStackPoppedValue (x->ticketMachine);

                err = PIZ_GOOD;
                err |= pizArrayAppend (x->hashTable[p], index);
            
                if (!err) {                           
                    x->pool[index].key = key;
                    x->pool[index].ptr = ptr;
                        
                    x->count ++;
                } else {
                    pizBoundedStackPush (x->ticketMachine, index);
                }
            }
        } else {
            err |= PIZ_MEMORY;
        }
    }
    
    return err;
}

PIZError pizBoundedHashTableRemoveByKeyAndPtr (PIZBoundedHashTable *x, long key, void *ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (ptr && (key >= 0)) {
        long p = key % x->hashSize;
        
        if (x->hashTable[p]) {   
            long count, i, index;
            
            if (count = pizArrayCount (x->hashTable[p])) {
                for (i = 0; i < count; i++) {
                    index = pizArrayValueAtIndex (x->hashTable[p], i);
                    
                    if ((x->pool[index].key == key) && (x->pool[index].ptr == ptr)) {
                        err = PIZ_GOOD;
                        
                        pizArrayRemoveIndex (x->hashTable[p], i);
                        pizBoundedStackPush (x->ticketMachine, index);
                            
                        break;
                    }
                }
            }
        }
    }
    
    return err;
}

long pizBoundedHashTableCount (const PIZBoundedHashTable *x)
{
    return (x->count);
}

PIZError pizBoundedHashTablePtrByKey (const PIZBoundedHashTable *x, long key, void **ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (*ptr) {
        (*ptr) = NULL;
    }
    
    if (key >= 0) {
        long p = key % x->hashSize;
        
        if (x->hashTable[p]) {   
            long count, i, index;
            
            if (count = pizArrayCount (x->hashTable[p])) {
                for (i = 0; i < count; i++) {
                    index = pizArrayValueAtIndex (x->hashTable[p], i);
                    
                    if (x->pool[index].key == key) {
                        (*ptr) = x->pool[index].ptr;
                        err = PIZ_GOOD;
                        break;
                    }
                }
            }
        }
    }

    return err;
}

bool pizBoundedHashTableContainsKey (const PIZBoundedHashTable *x, long key)
{
    bool k = false;
    
    if (key >= 0) {
        long p = key % x->hashSize;
        
        if (x->hashTable[p]) {
            long count, i, index;
            
            if (count = pizArrayCount (x->hashTable[p])) {
                for (i = 0; i < count; i++) {
                    index = pizArrayValueAtIndex (x->hashTable[p], i);
                    
                    if (x->pool[index].key == key) {
                        k = true;
                        break;
                    }
                }
            }
        }
    }
    
    return k;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
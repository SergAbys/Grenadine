/*
 * \file    pizHashTable.c
 * \author  Jean Sapristi
 * \date    April 5, 2012.
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

#include "pizHashTable.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_DEFAULT_SIZE 157

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZHashTable *pizHashTableNew (long size)
{
    PIZHashTable *x = NULL;
        
    if (x = (PIZHashTable *)malloc (sizeof(PIZHashTable))) {
        x->flags = PIZ_HASHTABLE_FLAG_NONE;
        x->count = 0;
        x->size  = PIZ_DEFAULT_SIZE;
        
        if (size > 0) {
            x->size = size;
        }
        
        if (!(x->hashTable = (PIZLinklist **)calloc (x->size, sizeof(PIZLinklist *)))) {
            free (x);
            x = NULL;
        }
    }
    
    return x;
}

void pizHashTableSetFlags (PIZHashTable *x, PIZFlags flags)
{
    x->flags = flags;
}

void pizHashTableFree (PIZHashTable *x)
{
    if (x) {
        long i;
        
        pizHashTableClear (x);
        
        for (i = 0; i < x->size; i++) {
            if (x->hashTable[i]) {
                pizLinklistFree (x->hashTable[i]);
            }
        }
            
        free (x->hashTable);
        x->hashTable = NULL;
        
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizHashTableClear (PIZHashTable *x)
{
    long i;
    
    for (i = 0; i < x->size; i++) {
        if (x->hashTable[i]) {
            if (x->flags & PIZ_HASHTABLE_FLAG_FREE_MEMORY) {
                PIZHashTableElement *element = NULL;
                PIZHashTableElement *nextElement = NULL;
                
                pizLinklistPtrAtIndex (x->hashTable[i], 0, (void **)&element);
                
                while (element) {
                    pizLinklistNextByPtr (x->hashTable[i], (void *)element, (void **)&nextElement);
                    free (element->ptr);
                    element->ptr = NULL;
                    element = nextElement;
                }
            }

            pizLinklistClear (x->hashTable[i]);
        }
    }
    
    x->count = 0;
}

PIZError pizHashTableAdd (PIZHashTable *x, long key, void *ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (ptr && (key >= 0)) {
        long                p = key % x->size;
        PIZHashTableElement *element = NULL;
        
        err = PIZ_GOOD;
        
        if (!x->hashTable[p]) {
            x->hashTable[p] = pizLinklistNew ( );
        }
        
        if (x->hashTable[p]) {
            if (element = (PIZHashTableElement *)malloc(sizeof(PIZHashTableElement))) {
                element->key = key;
                element->ptr = ptr;
                
                err |= pizLinklistAppend (x->hashTable[p], element);
                
                if (err) {
                    free (element);
                    element = NULL;
                } else {
                    x->count ++;
                }
            } else {
                err |= PIZ_MEMORY;
            }
        } else {
            err |= PIZ_MEMORY;
        }
    }
    
    return err;
}

PIZError pizHashTableRemoveByKeyAndPtr (PIZHashTable *x, long key, void *ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (ptr && (key >= 0)) {
        long p = key % x->size;
        
        if (x->hashTable[p]) {
            PIZHashTableElement *element = NULL;
            PIZHashTableElement *nextElement = NULL;
                            
            pizLinklistPtrAtIndex (x->hashTable[p], 0, (void **)&element);
                            
            while (element) {
                pizLinklistNextByPtr (x->hashTable[p], (void *)element, (void **)&nextElement);
                
                if ((element->key == key) && (element->ptr == ptr)) {
                    err = PIZ_GOOD;
                    
                    if (x->flags & PIZ_HASHTABLE_FLAG_FREE_MEMORY) {
                        free (element->ptr);
                        element->ptr = NULL;
                    }
                        
                    err |= pizLinklistRemoveByPtr (x->hashTable[p], element);
                    break;
                }
                    
                element = nextElement;
            }
        }
    }
    
    return err;
}

PIZError pizHashTablePtrByKey (const PIZHashTable *x, long key, void **ptr)
{
    PIZError err = PIZ_ERROR;
    
    if (*ptr) {
        (*ptr) = NULL;
    }
    
    if (key >= 0) {
        long p = key % x->size;
        
        if (x->hashTable[p]) {
            PIZHashTableElement *element = NULL;
            PIZHashTableElement *nextElement = NULL;
                            
            pizLinklistPtrAtIndex (x->hashTable[p], 0, (void **)&element);
                            
            while (element) {
                pizLinklistNextByPtr (x->hashTable[p], (void *)element, (void **)&nextElement);
                
                if (element->key == key) {
                    (*ptr) = element->ptr;
                    err = PIZ_GOOD;
                    break; 
                }
                    
                element = nextElement;
            }
        }
    }
    
    return err;
}

bool pizHashTableContainsKey (const PIZHashTable *x, long key)
{
    bool k = false;
    
    if (key >= 0) {
        long p = key % x->size;
        
        if (x->hashTable[p]) {
            PIZHashTableElement *element = NULL;
            PIZHashTableElement *nextElement = NULL;
                            
            pizLinklistPtrAtIndex (x->hashTable[p], 0, (void **)&element);
                            
            while (element) {
                pizLinklistNextByPtr (x->hashTable[p], (void *)element, (void **)&nextElement);
                
                if (element->key == key) {
                    k = true;
                    break; 
                }
                    
                element = nextElement;
            }
        }
    }
    
    return k;
}

long pizHashTableCount (const PIZHashTable *x)
{
    return (x->count);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
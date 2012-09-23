/*
 * \file    pizItemset.c
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

#include "pizItemset.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#ifndef PIZ_EXTERN_INLINE

void pizItemsetSetAtIndex(PIZItemset *itemset, long index)
{
    long i, p;
    ulong m;
    
    i = index / PIZ_MAGIC_ULONG;
    p = index % PIZ_MAGIC_ULONG;
    
    m = 1UL << p;
    
    itemset->items[i] |= m;
}

void pizItemsetUnsetAtIndex(PIZItemset *itemset, long index) 
{
    long i, p;
    ulong m;
    
    i = index / PIZ_MAGIC_ULONG;
    p = index % PIZ_MAGIC_ULONG;
    
    m = 1UL << p;
    
    itemset->items[i] &= ~m;
}

void pizItemsetClear(PIZItemset *itemset)  
{
    long i;
    
    for (i = 0; i < 4; i++) {
        itemset->items[i] = 0UL;
    }
}

long pizItemsetCount(const PIZItemset *itemset)
{
    long i, k = 0;
    
    for (i = 0; i < 4; i++) {
        ulong n = itemset->items[i];
            
        while (n != 0UL) {
            k ++;
            n &= n - 1UL;
        }
    }
    
    return k;
}

bool pizItemsetIsSetAtIndex(const PIZItemset *itemset, long index) 
{
    long i, p;
    ulong k = 0;

    i = index / PIZ_MAGIC_ULONG;
    p = index % PIZ_MAGIC_ULONG;
    
    k = itemset->items[i];

    k >>= p;
    k  &= 1UL;
    
    return (k != 0UL);
}

void pizItemsetUnion(const PIZItemset *a, const PIZItemset *b, PIZItemset *r) 
{
    long i;
    
    for (i = 0; i < 4; i++) {
        r->items[i] = a->items[i] | b->items[i];
    }
}

void pizItemsetIntersection(const PIZItemset *a, const PIZItemset *b, PIZItemset *r) 
{
    long i;
    
    for (i = 0; i < 4; i++) {
        r->items[i] = a->items[i] & b->items[i];
    }
}

bool pizItemsetIsIncluded(const PIZItemset *a, const PIZItemset *b)
{
    long i;
    bool k = true;
            
    for (i = 0; i < 4; i++) {
        if (b->items[i] != (b->items[i] | a->items[i])) {
            k = false;
            break;
        }
    }
        
    return k;
}

bool pizItemsetIsEqual(const PIZItemset *a, const PIZItemset *b)
{
    long i;
    bool k = true;
            
    for (i = 0; i < 4; i++) {
        if (a->items[i] != b->items[i]) {
            k = false;
            break;
        }
    }
        
    return k;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
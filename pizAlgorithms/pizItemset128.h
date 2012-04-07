/**
 * \file    pizItemset128.h
 * \author  Jean Sapristi
 * \date    April 8, 2012.
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

#ifndef PIZ_ITEMSET128_H
#define PIZ_ITEMSET128_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ITEMSET128_SIZE  128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZItemset128 {
    ulong items[4];
    } PIZItemset128;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizItemset128SetAtIndex    (PIZItemset128 *itemset, long index);
void pizItemset128UnsetAtIndex  (PIZItemset128 *itemset, long index);
void pizItemset128Clear         (PIZItemset128 *itemset);
long pizItemset128Count         (const PIZItemset128 *itemset);
bool pizItemset128IsSetAtIndex  (const PIZItemset128 *itemset, long index);

void pizItemset128Union         (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r);
void pizItemset128Intersection  (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r);
bool pizItemset128IsIncluded    (const PIZItemset128 *a, const PIZItemset128 *b);
bool pizItemset128IsEqual       (const PIZItemset128 *a, const PIZItemset128 *b);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizItemset128SetAtIndex (PIZItemset128 *itemset, long index)
{
    long  i, p;
    ulong m;
    
    i = index / 32;
    p = index % 32;
    
    m = 1UL << p;
    
    itemset->items[i] |= m;
}

PIZ_EXTERN void pizItemset128UnsetAtIndex (PIZItemset128 *itemset, long index) 
{
    long  i, p;
    ulong m;
    
    i = index / 32;
    p = index % 32;
    
    m = 1UL << p;
    
    itemset->items[i] &= ~m;
}

PIZ_EXTERN void pizItemset128Clear (PIZItemset128 *itemset)  
{
    long i;
    
    for (i = 0; i < 4; i++) {
        itemset->items[i] = 0UL;
    }
}

PIZ_EXTERN long pizItemset128Count (const PIZItemset128 *itemset)
{
    long i, k = 0;
    
    for (i = 0; i < 4; i++) {
        ulong n = itemset->items[i];
            
        while (n != 0UL) {
            k += (n & 1UL);
            n >>= 1;
        }
    }
    
    return k;
}

PIZ_EXTERN bool pizItemset128IsSetAtIndex (const PIZItemset128 *itemset, long index) 
{
    long  i, p;
    ulong k = 0;

    i = index / 32;
    p = index % 32;
    
    k = itemset->items[i];

    k >>= p;
    k  &= 1UL;
    
    return (k != 0UL);
}

PIZ_EXTERN void pizItemset128Union (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r) 
{
    long i;
    
    for (i = 0; i < 4; i++) {
        r->items[i] = a->items[i] | b->items[i];
    }
}

PIZ_EXTERN void pizItemset128Intersection (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r) 
{
    long i;
    
    for (i = 0; i < 4; i++) {
        r->items[i] = a->items[i] & b->items[i];
    }
}

PIZ_EXTERN bool pizItemset128IsIncluded (const PIZItemset128 *a, const PIZItemset128 *b)
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

PIZ_EXTERN bool pizItemset128IsEqual (const PIZItemset128 *a, const PIZItemset128 *b)
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
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_ITEMSET128_H
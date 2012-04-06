/**
 * \file    pizBoundedHashTable.h
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

#ifndef PIZ_BOUNDED_HASHTABLE_H
#define PIZ_BOUNDED_HASHTABLE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"
#include "pizBoundedStack.h"
#include "pizArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_BOUNDED_HASHTABLE_FLAG_NONE     0UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZBoundedHashTableElement {
    long    key;
    void    *ptr;
    } PIZBoundedHashTableElement;
    
typedef struct _PIZBoundedHashTable {
    PIZFlags                    flags;
    long                        count;
    long                        hashSize;
    long                        poolSize;
    PIZBoundedStack             *ticketMachine;
    PIZBoundedHashTableElement  *pool;
    PIZArray                    **hashTable;
    } PIZBoundedHashTable;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZBoundedHashTable *pizBoundedHashTableNew               (long argc, long *argv);
void                pizBoundedHashTableSetFlags           (PIZBoundedHashTable *x, PIZFlags flags);
void                pizBoundedHashTableFree               (PIZBoundedHashTable *x);
void                pizBoundedHashTableClear              (PIZBoundedHashTable *x);
PIZError            pizBoundedHashTableAdd                (PIZBoundedHashTable *x, long key, void *ptr);
PIZError            pizBoundedHashTableRemoveByKeyAndPtr  (PIZBoundedHashTable *x, long key, void *ptr);
PIZError            pizBoundedHashTablePtrByKey           (const PIZBoundedHashTable *x, long key, void **ptr);
bool                pizBoundedHashTableContainsKey        (const PIZBoundedHashTable *x, long key);
long                pizBoundedHashTableCount              (const PIZBoundedHashTable *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizBoundedHashTableSetFlags (PIZBoundedHashTable *x, PIZFlags flags)
{
    x->flags = flags;
}

PIZ_EXTERN long pizBoundedHashTableCount (const PIZBoundedHashTable *x)
{
    return (x->count);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_BOUNDED_HASHTABLE_H
/**
 * \file    pizBoundedHashTable.h
 * \author  Jean Sapristi
 * \date    23 janvier 2012
 * \ingroup structures
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
#include "pizGrowingArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_BOUNDED_HASHTABLE_FLAG_NONE 
 * \brief   (DEFAULT) Don't free items.
 */
 
#define PIZ_BOUNDED_HASHTABLE_FLAG_NONE (0L)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * 
 */
 
typedef struct _PIZBoundedHashTableElement {
    long key;                                           /*!< The key as \c long. */
    void *ptr;                                          /*!< Pointer to the item to store. */
    } PIZBoundedHashTableElement;
    
/**
 * \remark  Bounded hashtable with \c long keys.
 *          Implemented with an array of dynamic arrays, and a pre-allocated pool of elements.
 *          To obtain the hash value : index =  key % size.
 */
 
typedef struct _PIZBoundedHashTable {
    long                        flags;              /*!< Bit Flags. */
    long                        count;              /*!< Number of elements in the hashtable. */
    long                        hashSize;           /*!< Number of dynamic arrays in the hashtable. */
    long                        poolSize;           /*!< Maximum number of items in the hashtable. */
    PIZBoundedStack             *ticketMachine;     /*!< Ticket machine for pool management. */
    PIZBoundedHashTableElement  *pool;              /*!< Pool of elements. */
    PIZGrowingArray             **hashTable;        /*!< Pointer to the hashtable's array of dynamic arrays. */
    } PIZBoundedHashTable;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the bounded hashtable.
 * \details The function accepts two arguments : the size and the pool size. 
 *          The size should be a prime number, default is 157.
 *          Default pool size is 128.
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer to arguments.
 * \return  A pointer to the new bounded hashtable.
 * \remark	The following shows how to create a bounded hashtable.  
 * \code
 * long                    args[2] = {59, 100};
 * PIZBoundedHashTable     *myHashtab = NULL;
 *
 * myHashtab = pizBoundedHashTableNew (2, args);  
 * myHashtab = pizBoundedHashTableNew (0, NULL);  // default values.
 *
 * \endcode
 */
PIZBoundedHashTable *pizBoundedHashTableNew (long argc, long *argv);

/**
 * \brief   Set bounded hashtable's bit flags.
 * \param   x A valid pointer.
 * \param   flags Bit flags.
 */
void pizBoundedHashTableSetFlags (PIZBoundedHashTable *x, long flags);

/**
 * \brief   Free the bounded hashtable.
 * \details It is safe to pass NULL pointer. 
 *          Item's memory is released according to flags.
 * \param   x A Pointer.
 * \warning Free methods are not implemented.
 */
void pizBoundedHashTableFree (PIZBoundedHashTable *x);

/**
 * \brief   Clear the bounded hashtable.
 * \details Item's memory is released according to flags.
 * \param   x A valid pointer.
 * \warning Free methods are not implemented.
 */
void pizBoundedHashTableClear (PIZBoundedHashTable *x);

/**
 * \brief   Add an item to the bounded hashtable.
 * \details The provided pointer can not be NULL. 
 *          Key must be superior or equal to zero.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 * \remark  It is safe to add items with equal keys.
 */
PIZError pizBoundedHashTableAdd (PIZBoundedHashTable *x, long key, void *ptr);

/**
 * \brief   Remove an item with a given key and a given pointer.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 * \warning Free methods are not implemented.
 */
PIZError pizBoundedHashTableRemoveByKeyAndPtr (PIZBoundedHashTable *x, long key, void *ptr);

/**
 * \brief   Get an item with a given key.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr The adress of a pointer to set.
 * \return  An error code.
 * \remark  In case of equal keys, older item is returned.
 */
PIZError pizBoundedHashTablePtrByKey (const PIZBoundedHashTable *x, long key, void **ptr);

/**
 * \brief   Test if the bounded hashtable countains an item with a given key.
 * \param   x A valid pointer.
 * \param   key The key.
 * \return  True if found the key, otherwise false.
 */
bool pizBoundedHashTableContainsKey (const PIZBoundedHashTable *x, long key);

/**
 * \brief   Get the number of items in the bounded hashtable.
 * \param   x A valid pointer.
 * \return  The number of items in the bounded hashtable.
 */
long pizBoundedHashTableCount (const PIZBoundedHashTable *x);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizBoundedHashTableSetFlags (PIZBoundedHashTable *x, long flags)
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
/**
 * \file    pizHashTable.h
 * \author  Jean Sapristi
 * \date    23 janvier 2012
 * \ingroup hashTable
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

#ifndef PIZ_HASHTABLE_H
#define PIZ_HASHTABLE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"
#include "pizLinklist.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_HASHTABLE_FLAG_NONE 
 * \brief   (DEFAULT) Don't free items.
 * \ingroup hashTable
 */
 
/**
 * \def     PIZ_HASHTABLE_FLAG_FREE_MEMORY 
 * \brief   Free items with \c free().
 * \ingroup hashTable
 */
 
#define PIZ_HASHTABLE_FLAG_NONE         (0L)
#define PIZ_HASHTABLE_FLAG_FREE_MEMORY  (1<<0)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \ingroup hashTable 
 */
 
typedef struct _PIZHashTableElement {
    long key;                                   /*!< The key as \c long. */
    void *ptr;                                  /*!< Pointer to the item to store. */
    } PIZHashTableElement;

/**
 * \remark  Simple hashtable with \c long keys. 
 *          Implemented with an array of linklists.
 *          To obtain the hash value : index =  key % size.
 * \ingroup hashTable
 */
 
typedef struct _PIZHashTable {
    long                flags;                  /*!< Bit Flags. */
    long                count;                  /*!< Number of elements in the hashtable. */
    long                size;                   /*!< Number of linklists in the hashtable. */
    PIZLinklist         **hashTable;            /*!< Pointer to the hashtable's array of linklists. */
    } PIZHashTable;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the hashtable.
 * \details The size should be a prime number, default is 157.
 *          In case of failure the pointer is NULL.
 * \param   size The size of the hashtable.
 * \return  A pointer to the new hashtable. 
 * \remark	The following shows how to create an hashtable.  
 * \code
 * PIZHashTable *hashtab = NULL;
 *
 * hashtab = pizHashTableNew (53);  
 * hashtab = pizHashTableNew (0);   // default value.
 *
 * \endcode 
 * \ingroup hashTable
 */
PIZHashTable *pizHashTableNew (long size);

/**
 * \brief   Set hashtable's bit flags.
 * \param   x A valid pointer.
 * \param   flags Bit flags.
 * \ingroup hashTable
 */
void pizHashTableSetFlags (PIZHashTable *x, long flags);

/**
 * \brief   Free the hashtable.
 * \details It is safe to pass NULL pointer. 
 *          Item's memory is released according to flags.
 * \param   x A Pointer.
 * \ingroup hashTable
 */
void pizHashTableFree (PIZHashTable *x);

/**
 * \brief   Clear the hashtable.
 * \details Item's memory is released according to flags.
 * \param   x A valid pointer.
 * \ingroup hashTable
 */
void pizHashTableClear (PIZHashTable *x);

/**
 * \brief   Add item to the hashtable.
 * \details The provided pointer can not be NULL. 
 *          Key must be superior or equal to zero.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 * \remark  It is safe to add items with equal keys.
 * \ingroup hashTable
 */
PIZError pizHashTableAdd (PIZHashTable *x, long key, void *ptr);

/**
 * \brief   Remove an item with a given key and a given pointer.
 * \details Item's memory is released according to flags.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 * \ingroup hashTable
 */
PIZError pizHashTableRemoveByKeyAndPtr (PIZHashTable *x, long key, void *ptr);

/**
 * \brief   Get an item with a given key.
 * \param   x A valid pointer.
 * \param   key The key.
 * \param   ptr The adress of a pointer to set.
 * \return  An error code.
 * \remark  In case of equal keys, older item is returned.
 * \ingroup hashTable
 */
PIZError pizHashTablePtrByKey (const PIZHashTable *x, long key, void **ptr);

/**
 * \brief   Test if the hashtable countains an item with a given key.
 * \param   x A valid pointer.
 * \param   key The key.
 * \return  True if found the key, otherwise false.
 * \ingroup hashTable
 */
bool pizHashTableContainsKey (const PIZHashTable *x, long key);

/**
 * \brief   Get the number of items in the hashtable.
 * \param   x A valid pointer.
 * \return  The number of items in the hashtable.
 * \ingroup hashTable
 */
long pizHashTableCount (const PIZHashTable *x);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizHashTableSetFlags (PIZHashTable *x, long flags)
{
    x->flags = flags;
}

PIZ_EXTERN long pizHashTableCount (const PIZHashTable *x)
{
    return (x->count);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_HASHTABLE_H
/**
 * \file    pizLinklist.h
 * \author  Jean Sapristi
 * \date    15 janvier 2012
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

#ifndef PIZ_LINKLIST_H
#define PIZ_LINKLIST_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_LINKLIST_FLAG_NONE 
 * \brief   Don't free items.
 */
 
/**
 * \def     PIZ_LINKLIST_FLAG_FREE_MEMORY 
 * \brief   (DEFAULT) Free items with \c free().
 */
 
/**
 * \def     PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY 
 * \brief   Free items with \c pizGrowingArrayFree().
 */
 
#define PIZ_LINKLIST_FLAG_NONE                  (0L)
#define PIZ_LINKLIST_FLAG_FREE_MEMORY           (1<<0)
#define PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY    (1<<1)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief  Linklist element.
 */
 
typedef struct _PIZLinklistElement {
    void                        *ptr;               /*!< Pointer to item. */
    struct _PIZLinklistElement  *next;              /*!< Pointer to next element. */
    struct _PIZLinklistElement  *previous;          /*!< Pointer to previous element. */
    } PIZLinklistElement;

/**
 * \brief The linklist.
 */
 
typedef struct _PIZLinklist {
    long                flags;                      /*!< Bit Flags. */
    long                count;                      /*!< Number of elements in the linklist. */
    PIZLinklistElement  *head;                      /*!< Pointer to head element. */
    PIZLinklistElement  *tail;                      /*!< Pointer to tail element. */
    PIZLinklistElement  *cache;                     /*!< Pointer to cached element. */
    } PIZLinklist;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   Create the linklist.
 * \details In case of failure the pointer is NULL.
 * \return  A pointer to the new linklist.
 */
PIZLinklist *pizLinklistNew (void);

/**
 * \brief   Set linklist's bit flags.
 * \param   x A valid pointer.
 * \param   flags Bit flags.
 */
void pizLinklistSetFlags (PIZLinklist *x, long flags);

/**
 * \brief   Free the linklist.
 * \details It is safe to pass NULL pointer. 
 *          Item's memory is released according to flags.
 * \param   x A Pointer.
 */
void pizLinklistFree (PIZLinklist *x);

/**
 * \brief   Clear the linklist.
 * \details Item's memory is released according to flags.
 * \param   x A valid pointer.
 */
void pizLinklistClear (PIZLinklist *x);

/**
 * \brief   Append an item's pointer to the linklist.
 * \details The provided pointer can not be NULL.
 * \param   x A valid pointer.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 */
PIZError pizLinklistAppend (PIZLinklist *x, void *ptr);

/**
 * \brief   Insert an item's pointer in front of the linklist.
 * \details The provided pointer can not be NULL.
 * \param   x A valid pointer.
 * \param   ptr A pointer to the item.
 * \return  An error code.
 */
PIZError pizLinklistInsert (PIZLinklist *x, void *ptr);

/**
 * \brief   Get an item's pointer with specified index.
 * \details If there is no item at the index, pointer is set to NULL.
 * \param   x A valid pointer.
 * \param   index The index (zero-based).
 * \param   ptr The adress of the pointer to set.
 * \return  An error code.
 */
PIZError pizLinklistPtrAtIndex (PIZLinklist *x, long index, void **ptr);

/**
 * \brief   Get the next item's pointer of a given item's pointer.
 * \details If there is no item next, pointer is set to NULL.
 * \param   x A valid pointer.
 * \param   ptr The item's pointer provided.
 * \param   nextPtr The adress of the pointer to set.
 * \return  An error code.
 * \remark	The following shows how to traverse a linklist using cache optimization.  
 * \code
 *      PIZNote *note       = NULL;
 *      PIZNote *nextNote   = NULL;
 *           
 *      pizLinklistPtrAtIndex (linklist, 0, (void **)&note);
 *           
 *      while (note) {
 *          pizLinklistNextByPtr (linklist, (void *)note, (void **)&nextNote);
 *
 *          //pizLinklistRemoveByPtr (linklist, (void *)note)); 
 *                   
 *          note = nextNote;
 *      }
 *
 *	\endcode
 */
PIZError pizLinklistNextByPtr (PIZLinklist *x, void *ptr, void **nextPtr);

/**
 * \brief   Given an item's pointer, remove this item from the linklist.
 * \details Item's memory is released according to flags.
 * \param   x A valid pointer.
 * \param   ptr The item's pointer provided.
 * \return  An error code.
 */
PIZError pizLinklistRemoveByPtr (PIZLinklist *x, void *ptr);

/**
 * \brief   Swap item's pointers in the linklist according to specified indexes.
 * \param   x A valid pointer.
 * \param   m Item's index (zero-based).
 * \param   n Item's index (zero-based).
 * \return  An error code.
 */
PIZError pizLinklistSwapByIndexes (PIZLinklist *x, long m, long n);

/**
 * \brief   Get the number of items in the linklist.
 * \param   x A valid pointer.
 * \return  The number of items.
 */
long pizLinklistCount (const PIZLinklist *x);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizLinklistSetFlags (PIZLinklist *x, long flags)
{
    x->flags = flags;
}

PIZ_EXTERN long pizLinklistCount (const PIZLinklist *x)
{
    return (x->count);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_LINKLIST_H
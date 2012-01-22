/**
 * \file    pizLinklist.h
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

#ifndef PIZ_LINKLIST_H
#define PIZ_LINKLIST_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_LINKLIST_FLAG_NONE                  (0L)
#define PIZ_LINKLIST_FLAG_FREE_MEMORY           (1<<0)
#define PIZ_LINKLIST_FLAG_FREE_GROWING_ARRAY    (1<<1)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZLinklistElement {
    void                        *ptr;
    struct _PIZLinklistElement  *next;
    struct _PIZLinklistElement  *previous;
    } PIZLinklistElement;

typedef struct _PIZLinklist {
    long                flags;
    long                count;
    PIZLinklistElement  *head;
    PIZLinklistElement  *tail;
    PIZLinklistElement  *cache;
    } PIZLinklist;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

PIZLinklist     *pizLinklistNew             (void);
void            pizLinklistSetFlags         (PIZLinklist *linklist, long flags);
void            pizLinklistFree             (PIZLinklist *linklist);

void            pizLinklistClear            (PIZLinklist *linklist);
PIZError        pizLinklistAppend           (PIZLinklist *linklist, void *ptr);
PIZError        pizLinklistInsert           (PIZLinklist *linklist, void *ptr);
long            pizLinklistCount            (PIZLinklist *linklist);
PIZError        pizLinklistPtrAtIndex       (PIZLinklist *linklist, long index, void **ptr);
PIZError        pizLinklistNextByPtr        (PIZLinklist *linklist, void *ptr, void **nextPtr);
PIZError        pizLinklistRemoveByPtr      (PIZLinklist *linklist, void *ptr);
PIZError        pizLinklistSwapByIndexes    (PIZLinklist *linklist, long m, long n);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizLinklistSetFlags (PIZLinklist *x, long flags)
{
    x->flags = flags;
}

PIZ_EXTERN long pizLinklistCount (PIZLinklist *x)
{
    return (x->count);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_LINKLIST_H
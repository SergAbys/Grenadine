/*
 * \file    pizLinklist.c
 * \author  Jean Sapristi
 * \date    May 10, 2012.
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

#include "pizLinklist.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZLinklist *pizLinklistNew (void)
{
    PIZLinklist *x = NULL;
        
    if (x = (PIZLinklist *)malloc (sizeof(PIZLinklist))) {
        x->flags = PIZ_LINKLIST_FLAG_FREE_MEMORY;
        x->count = 0;
        x->head  = NULL;
        x->tail  = NULL;
        x->cache = NULL;
    }
    
    return x;
}

void pizLinklistFree (PIZLinklist *x)
{
    if (x) {
        pizLinklistClear (x);
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizLinklistAppend (PIZLinklist *x, void *ptr)
{
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *newElement = NULL;
    
    if (ptr) {
    //
    err = PIZ_MEMORY;
    
    if (newElement = (PIZLinklistElement *)malloc (sizeof(PIZLinklistElement))) {
    //
    err = PIZ_GOOD;
    
    if (x->count) {
        newElement->ptr      = ptr;
        newElement->previous = x->tail;
        newElement->next     = NULL;
        
        x->count ++;
        
        x->tail->next = newElement;
        x->tail       = newElement;
        
    } else {
        newElement->ptr      = ptr;
        newElement->previous = NULL;
        newElement->next     = NULL;
        
        x->count ++;
        
        x->head = newElement;
        x->tail = newElement;
    }
    //
    }
    //
    }
    
    return err;
}

/*
PIZError pizLinklistInsert (PIZLinklist *x, void *ptr)
{
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *newElement = NULL;
    
    if (ptr) {
    //
    err = PIZ_MEMORY;
    
    if (newElement = (PIZLinklistElement *)malloc (sizeof(PIZLinklistElement))) {
    //
    err = PIZ_GOOD;
    
    if (x->count) {
        newElement->ptr      = ptr;
        newElement->previous = NULL;
        newElement->next     = x->head;
        
        x->count ++;
        
        x->head->previous = newElement;
        x->head           = newElement;
        
    } else {
        newElement->ptr      = ptr;
        newElement->previous = NULL;
        newElement->next     = NULL;
        
        x->count ++;
        
        x->head = newElement;
        x->tail = newElement;
    }
    //
    }
    //
    }
    
    return err;
}
*/

void pizLinklistClear (PIZLinklist *x)
{
    PIZLinklistElement *theElement = x->head;
    
    while (theElement) {
        PIZLinklistElement *toBeRemoved = NULL;

        toBeRemoved = theElement;
        theElement  = toBeRemoved->next;

        if (x->flags & PIZ_LINKLIST_FLAG_FREE_MEMORY) {
            free (toBeRemoved->ptr);
        } 
            
        free (toBeRemoved);
        toBeRemoved = NULL;
    }
    
    x->count = 0;
    x->head  = NULL;
    x->tail  = NULL;
    x->cache = NULL;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizLinklistPtrAtIndex (PIZLinklist *x, long index, void **ptr)
{
    long               i = 0;
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *theElement = NULL;
    
    if (*ptr) {
        (*ptr) = NULL;
    }
        
    theElement = x->head;
        
    while (theElement) {
        PIZLinklistElement *tempElement = NULL;

        if (i == index) {
            (*ptr)   = theElement->ptr;
            x->cache = theElement;
            err      = PIZ_GOOD;
            break;
        }
        
        tempElement = theElement;
        theElement  = tempElement->next;
        
        i ++;
    }
    
    return err; 
}

PIZError pizLinklistNextByPtr (PIZLinklist *x, void *ptr, void **nextPtr)
{
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *matchedElement = NULL;
    
    if (*nextPtr) {
        (*nextPtr) = NULL;
    }
        
    if (x->cache && (x->cache->ptr == ptr)) {
        matchedElement = x->cache;
        
    } else {
        PIZLinklistElement *theElement = x->head;

        while (theElement) {
            PIZLinklistElement *tempElement = NULL;
            
            if (theElement->ptr == ptr) {
                matchedElement = theElement;
                break;                          
            }
            
            tempElement = theElement;
            theElement  = tempElement->next;
        }
    }
        
    if (matchedElement && matchedElement->next) {
        err        = PIZ_GOOD;
        (*nextPtr) = matchedElement->next->ptr;
        x->cache   = matchedElement->next;
    }
            
    return err;
}

PIZError pizLinklistRemoveByPtr (PIZLinklist *x, void *ptr)
{
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *toBeRemoved = NULL;
    
    if (x->cache && x->cache->previous && (x->cache->previous->ptr == ptr)) {
        toBeRemoved = x->cache->previous;
        
    } else if (x->cache && (x->cache->ptr == ptr)) {
        toBeRemoved = x->cache;
        x->cache = NULL;
        
    } else {
        PIZLinklistElement *theElement = x->head;

        while (theElement) {
            PIZLinklistElement *tempElement = NULL;
            
            if (theElement->ptr == ptr) {
                toBeRemoved = theElement;    
                break;                      
            }
            
            tempElement = theElement;
            theElement  = tempElement->next;
        }
    }
        
    if (toBeRemoved) {
    //
    if (x->count == 1) {
        x->head = NULL;
        x->tail = NULL;
        
    } else if (toBeRemoved == x->tail) {
        x->tail = toBeRemoved->previous;
        toBeRemoved->previous->next = NULL;
        
    } else if (toBeRemoved == x->head) {
        x->head = toBeRemoved->next;
        toBeRemoved->next->previous = NULL;
        
    } else {
        toBeRemoved->next->previous = toBeRemoved->previous;
        toBeRemoved->previous->next = toBeRemoved->next;
    }
    
    if (x->flags & PIZ_LINKLIST_FLAG_FREE_MEMORY) {
        free (toBeRemoved->ptr);
    }
        
    free (toBeRemoved);
    toBeRemoved = NULL;
    
    x->count --;
    
    err = PIZ_GOOD;
    //
    }
            
    return err;
}

PIZError pizLinklistChuckByPtr (PIZLinklist *x, void *ptr)
{
    PIZError           err = PIZ_ERROR;
    PIZLinklistElement *toBeChucked = NULL;
    
    if (x->cache && x->cache->previous && (x->cache->previous->ptr == ptr)) {
        toBeChucked = x->cache->previous;
        
    } else if (x->cache && (x->cache->ptr == ptr)) {
        toBeChucked = x->cache;
        x->cache = NULL;
        
    } else {
        PIZLinklistElement *theElement = x->head;

        while (theElement) {
            PIZLinklistElement *tempElement = NULL;
            
            if (theElement->ptr == ptr) {
                toBeChucked = theElement;    
                break;                      
            }
            
            tempElement = theElement;
            theElement  = tempElement->next;
        }
    }
        
    if (toBeChucked) {
    //
    if (x->count == 1) {
        x->head = NULL;
        x->tail = NULL;
        
    } else if (toBeChucked == x->tail) {
        x->tail = toBeChucked->previous;
        toBeChucked->previous->next = NULL;
        
    } else if (toBeChucked == x->head) {
        x->head = toBeChucked->next;
        toBeChucked->next->previous = NULL;
        
    } else {
        toBeChucked->next->previous = toBeChucked->previous;
        toBeChucked->previous->next = toBeChucked->next;
    }
    
    x->count --;
    
    err = PIZ_GOOD;
    //
    }
            
    return err;
}

long pizLinklistCount (const PIZLinklist *x)
{
    return x->count;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
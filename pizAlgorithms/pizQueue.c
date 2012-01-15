/*
 * \file	pizQueue.c
 * \author	Jean Sapristi
 * \date	15 janvier 2012
 */
 
/*
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizQueue.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

# include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZQueue *pizQueueNew (void)
{
	PIZQueue *x = NULL;
		
	if (x = (PIZQueue *)malloc (sizeof(PIZQueue)))
		{
				x->count		= 0;
				x->poppedValue	= 0;
				x->head			= NULL;
				x->tail			= NULL;
		}
	
	return x;
}

void pizQueueFree (PIZQueue *x)
{
	if (x)
		{
			pizQueueClear (x);
			
			free (x);
		}
}

void pizQueueClear (PIZQueue *x)
{
	PIZQueueElement *theElement = x->head;
	
	while (theElement)
		{
			PIZQueueElement *elementToBeRemoved = NULL;

			elementToBeRemoved	= theElement;
			theElement			= elementToBeRemoved->next;

			free (elementToBeRemoved);
			elementToBeRemoved = NULL;
		}
		
	x->count		= 0;
	x->poppedValue	= 0;
	x->head			= NULL;
	x->tail			= NULL;
}

PIZError pizQueueAppend (PIZQueue *x, long value)
{
	long			err = PIZ_MEMORY;
	PIZQueueElement	*newElement = NULL;
	
	if (newElement = (PIZQueueElement *)malloc (sizeof(PIZQueueElement)))
		{
			err = PIZ_GOOD;
			
			if (x->count)
				{
					newElement->value		= value;
					newElement->previous	= x->tail;
					newElement->next		= NULL;
					
					x->count ++;
					
					x->tail->next			= newElement;
					x->tail					= newElement;
				}
			else
				{
					newElement->value		= value;
					newElement->previous	= NULL;
					newElement->next		= NULL;
					
					x->count ++;
					
					x->head					= newElement;
					x->tail					= newElement;
				}
		}
	
	return err;
}

long pizQueueCount (PIZQueue *x)
{
	return (x->count);
}

PIZError pizQueuePop (PIZQueue *x)
{
	long			err = PIZ_ERROR;
	PIZQueueElement *theElement = NULL;
	
	if (x->count == 1)
		{
			theElement		=  x->head;
			x->poppedValue	= theElement->value;
			
			free (theElement);
			theElement = NULL;
			
			x->count	= 0;
			x->head		= NULL;
			x->tail		= NULL;
			
			err = PIZ_GOOD;
		}
	else if (x->count > 1)
		{
			theElement					= x->head;
			theElement->next->previous	= NULL;
			
			x->count --;
			
			x->poppedValue				= theElement->value;
			x->head						= theElement->next;
			
			free (theElement);
			theElement = NULL;
			
			err = PIZ_GOOD;
		}
	
	return err;
}

PIZError pizQueuePopLastValue (PIZQueue *x)
{
	long			err = PIZ_ERROR;
	PIZQueueElement *theElement = NULL;
	
	if (x->count == 1)
		{
			theElement		= x->tail;
			x->poppedValue	= theElement->value;
			
			free (theElement);
			theElement = NULL;
			
			x->count	= 0;
			x->head		= NULL;
			x->tail		= NULL;
			
			err = PIZ_GOOD;
		}
	else if (x->count > 1)
		{
			theElement					= x->tail;
			theElement->previous->next	= NULL;
			
			x->count --;
			
			x->poppedValue				= theElement->value;
			x->tail						= theElement->previous;
			
			free (theElement);
			theElement = NULL;
			
			err = PIZ_GOOD;
		}
	
	return err;
}

long pizQueuePoppedValue (PIZQueue *x)
{
	return (x->poppedValue);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
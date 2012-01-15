/**
 * \file	pizBoundedQueue.h
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

#ifndef	PIZ_BOUNDED_QUEUE_H
#define	PIZ_BOUNDED_QUEUE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZBoundedQueue {
	long count;
	long bound;
	long head;
	long tail;
	long poppedValue;
	long *boundedQueueValues;
	} PIZBoundedQueue; 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

PIZBoundedQueue		*pizBoundedQueueNew			(long size);
void				pizBoundedQueueFree			(PIZBoundedQueue *fifo);

void				pizBoundedQueueClear		(PIZBoundedQueue *fifo);
PIZError			pizBoundedQueueAppend		(PIZBoundedQueue *fifo, long value);
long				pizBoundedQueueCount		(PIZBoundedQueue *fifo);
PIZError			pizBoundedQueuePop			(PIZBoundedQueue *fifo);
PIZError			pizBoundedQueuePopLastValue	(PIZBoundedQueue *fifo);
long				pizBoundedQueuePoppedValue	(PIZBoundedQueue *fifo);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizBoundedQueueClear (PIZBoundedQueue *x)
{
	x->head		= 0;
	x->tail		= 0;
	x->count	= 0;
}

PIZ_EXTERN long pizBoundedQueueCount (PIZBoundedQueue *x)
{
	return (x->count);
}


PIZ_EXTERN long pizBoundedQueuePoppedValue (PIZBoundedQueue *x)
{
	return (x->poppedValue);
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_BOUNDED_QUEUE_H
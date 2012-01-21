/*
 * \file	pizInterface.c
 * \author	Jean Sapristi
 * \date	21 janvier 2012
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

#include "pizInterface.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static long interface_start			= PIZ_DEFAULT_START;
static long interface_end			= PIZ_DEFAULT_END;
static long interface_down			= PIZ_DEFAULT_DOWN;
static long interface_up			= PIZ_DEFAULT_UP;

static long interface_originStart	= PIZ_DEFAULT_START;
static long interface_originDown	= PIZ_DEFAULT_DOWN;
static long interface_originWidth	= (PIZ_DEFAULT_END - PIZ_DEFAULT_START);
static long interface_originHeight	= (PIZ_DEFAULT_UP - PIZ_DEFAULT_DOWN);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceHasMarkedNote (PIZSequence *x)
{
	bool k;
	
	PIZLOCK
	k = (x->markedNote != NULL);
	PIZUNLOCK;
	
	return k;
}

long pizSequenceMarkedNoteValue (PIZSequence *x, PIZSelector selector)
{
	long k = -1;
	
	PIZLOCK
	
	if (x->markedNote) 
		{
			if (selector == PIZ_CHANNEL)
				{
					if (x->markedNote->data[PIZ_CHANNEL] == PIZ_SEQUENCE_NOTE_CHANNEL_NONE)
						{
							k = x->channel;
						}
					else
						{
							k = x->markedNote->data[PIZ_CHANNEL];
						}
				}
			else
				{
					k = x->markedNote->data[selector];
				}
		}
		
	PIZUNLOCK
	
	return k;
}

void pizSequenceChangeMarkedNoteValue (PIZSequence *x, PIZSelector selector, long value)
{
	PIZLOCK
	
	if (x->markedNote) 
		{
			if (selector == PIZ_DURATION)
				{
					long err = PIZ_GOOD;
					long temp = x->markedNote->data[PIZ_DURATION];
					
					if (value > 0) {
						temp += x->grid;
					} else {
						temp -= x->grid;
					}
						
					temp = pizSequenceSnapRoundDuration (x, temp);
					
					err |= ((x->markedNote->position + temp) > PIZ_SEQUENCE_TIMELINE_SIZE);
					err |= (temp > PIZ_SEQUENCE_MAXIMUM_DURATION);
					err |= (temp <= 0);
					
					if (!err) {
							x->markedNote->data[PIZ_DURATION] = temp;
						}
				}
			else
				{
					x->markedNote->data[selector] += value;
					
					switch (selector) {
						case PIZ_PITCH		:	x->markedNote->data[PIZ_PITCH] = 
												CLAMP (x->markedNote->data[PIZ_PITCH], 
												0, PIZ_SEQUENCE_MIDI_NOTE);
												break;
						case PIZ_VELOCITY	:	x->markedNote->data[PIZ_VELOCITY] = 
												CLAMP (x->markedNote->data[PIZ_VELOCITY], 
												0, PIZ_SEQUENCE_MIDI_VELOCITY);
												break;
						case PIZ_CHANNEL	:	x->markedNote->data[PIZ_CHANNEL] = 
												CLAMP (x->markedNote->data[PIZ_CHANNEL], 
												0, PIZ_SEQUENCE_MIDI_CHANNEL);
												break;
						}
				}
		}
		
	PIZUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceSetTempZoneStartWithCoordinates	(PIZSequence *x, PIZCoordinates *coordinates)
{
	long tempStart;
	bool haveChanged = false;
	
	PIZLOCK
	
	tempStart = CLAMP (pizSequenceSnapRoundPosition (x, coordinates->position), 0, PIZ_SEQUENCE_TIMELINE_SIZE);
	
	if (interface_start != tempStart) {
			interface_start = tempStart;
			haveChanged = true;
		}
	
	PIZUNLOCK
	
	return haveChanged;
}

bool pizSequenceSetTempZoneEndWithCoordinates (PIZSequence *x, PIZCoordinates *coordinates)
{
	long tempEnd;
	bool haveChanged = false;
	
	PIZLOCK
	
	tempEnd	= CLAMP (pizSequenceSnapRoundPosition (x, coordinates->position), 0, PIZ_SEQUENCE_TIMELINE_SIZE);
	
	if (interface_end != tempEnd) {
			interface_end = tempEnd;
			haveChanged = true;
		}
	
	PIZUNLOCK
	
	return haveChanged;
}

bool pizSequenceSetTempZoneDownWithCoordinates (PIZSequence *x, PIZCoordinates *coordinates)
{
	bool haveChanged = false;
	
	PIZLOCK
	
	if (coordinates->pitch <= interface_up)
		{
			long tempDown = CLAMP (coordinates->pitch, 0, PIZ_SEQUENCE_MIDI_NOTE);
			
			if (interface_down != tempDown) {
					interface_down = tempDown;
					haveChanged = true;
				}
		}
		
	PIZUNLOCK
	
	return haveChanged;
}

bool pizSequenceSetTempZoneUpWithCoordinates (PIZSequence *x, PIZCoordinates *coordinates)
{
	bool haveChanged = false;
	
	PIZLOCK
	
	if (coordinates->pitch >= interface_down)
		{
			long tempUp = CLAMP (coordinates->pitch, 0, PIZ_SEQUENCE_MIDI_NOTE);
			
			if (interface_up != tempUp) {
				interface_up = tempUp;
				haveChanged = true;
			}
		}
		
	PIZUNLOCK
	
	return haveChanged;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceMoveTempZoneWithDelta (PIZSequence *x, long pitch, long position)
{
	bool haveChanged = false;
	long tempStart, tempDown;
	
	PIZLOCK
	
	tempStart	= CLAMP (pizSequenceSnapRoundPosition (x, interface_originStart + position), 
						0, (PIZ_SEQUENCE_TIMELINE_SIZE - interface_originWidth));
	tempDown	= CLAMP (interface_originDown + pitch, 
						0, (PIZ_SEQUENCE_MIDI_NOTE - interface_originHeight));
	
	if ((tempStart != interface_start) || (tempDown != interface_down))
		{
			interface_start = tempStart;
			interface_end	= tempStart + interface_originWidth;
			interface_down	= tempDown;
			interface_up	= tempDown + interface_originHeight;
			
			haveChanged = true;
		}
	
	PIZUNLOCK
	
	return haveChanged;
}

PIZError pizSequenceTempZoneToArray (PIZSequence *x, PIZGrowingArray *array)
{
	long err = PIZ_ERROR;

	PIZLOCK
	
	if (array)
		{
			err = PIZ_GOOD;
			
			err |= pizGrowingArrayAppend (array, interface_start);
			err |= pizGrowingArrayAppend (array, interface_end);
			err |= pizGrowingArrayAppend (array, interface_down);
			err |= pizGrowingArrayAppend (array, interface_up);
		}
		
	PIZUNLOCK
	
	return err;
}

void pizSequenceSetTempZoneByZone (PIZSequence *x)
{
	PIZLOCK
	
	interface_start			= x->start;
	interface_end			= x->end;
	interface_down			= x->down;
	interface_up			= x->up;
	interface_originStart	= x->start;
	interface_originDown	= x->down;
	interface_originWidth	= (x->end - x->start);
	interface_originHeight	= (x->up - x->down);
	
	PIZUNLOCK
}

PIZError pizSequenceSetZoneByTempZone (PIZSequence *x)
{
	long err = PIZ_ERROR;
	
	PIZLOCK
	
	if (!(interface_start == interface_end))
		{
			if (interface_end < interface_start) {
				long k			= interface_end;
				interface_end	= interface_start;
				interface_start	= k;
				}
			
			if (interface_up < interface_down) {
				long k			= interface_up;
				interface_up	= interface_down;
				interface_down	= k;
				}
			
			x->start = interface_start;
			x->end	 = interface_end;
			x->down	 = interface_down;
			x->up	 = interface_up;
			
			err = PIZ_GOOD;
		}
		
	PIZUNLOCK
	
	return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void pizSequenceSelectAllNotes (PIZSequence *x)
{
	long i;
	
	PIZLOCK
	
	for (i = 0; i < pizGrowingArrayCount (x->map); i++)
		{	
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note) {
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					note->isSelected = true;
					note = nextNote;
				}
		}
	
	PIZUNLOCK
}

void pizSequenceUnselectAllNotes (PIZSequence *x)
{
	PIZLOCK
	
	pizSequenceUnselectNotes (x);
	
	PIZUNLOCK
}

long pizSequenceSelectNoteWithCoordinates (PIZSequence *x, PIZCoordinates *coordinates)
{
	long i, count;
	long k = 0;

	PIZLOCK
	
	count = pizGrowingArrayCount (x->map);
		
	for (i = (count - 1); i >= 0; i--)
		{
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note && !k)
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					
					if (((coordinates->position >= p) && (coordinates->position <= 
						(p + note->data[PIZ_DURATION]))) && (coordinates->pitch == note->data[PIZ_PITCH])) 
						{
							if (!note->isSelected)
								{
									pizSequenceUnselectNotes (x);
									
									note->isSelected = true;
									x->markedNote = note;
								}
							
							k = 1;
						}
					
					note = nextNote;
				}
			
			if (k) 
				{ 
					break;
				}
		}
	
	PIZUNLOCK
	
	return k;
}

long pizSequenceInvertNoteWithCoordinates (PIZSequence *x, PIZCoordinates *coordinates)
{
	long i, count;
	long k = -1;
	
	PIZLOCK
	
	count = pizGrowingArrayCount (x->map);
		
	for (i = (count - 1); i >= 0; i--)
		{
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note)
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					
					if (((coordinates->position >= p) && (coordinates->position <= 
						(p + note->data[PIZ_DURATION]))) && (coordinates->pitch == note->data[PIZ_PITCH]))
						{
							if (note->isSelected) 
								{
									if (x->markedNote == note){
											x->markedNote = NULL;
										}
										
									note->isSelected = false;
									k = 0;
								} 
							else 
								{
									note->isSelected = true;
									k = 1;
								}
							
							break;
						}
					
					note = nextNote;
				}
			
			if (k != -1) 
				{
					break;
				}
		}
	
	PIZUNLOCK
	
	return k;
}

void pizSequenceInitLasso (PIZSequence *x)
{
	long i;
	
	PIZLOCK
	
	for (i = 0; i < pizGrowingArrayCount (x->map); i++)
		{	
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note) 
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					
					note->flags &= ~PIZ_SEQUENCE_NOTE_FLAG_LASSO;
					
					note = nextNote;
				}
		}
	
	PIZUNLOCK
}

long pizSequenceSelectNotesWithLasso (PIZSequence *x, PIZCoordinates *c1, PIZCoordinates *c2, bool invert)
{
	long i, a, b, u, v;
	long k = 0;
	
	PIZLOCK
	
	a = MIN (c1->position,	c2->position);
	b = MIN (c1->pitch,		c2->pitch);
	u = MAX (c1->position,	c2->position);
	v = MAX (c1->pitch,		c2->pitch);
	
	for (i = 0; i < pizGrowingArrayCount (x->map); i++)
		{
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note)
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					
					if ((note->data[PIZ_PITCH] >= b && note->data[PIZ_PITCH] <= v) && ((p >= a && p <= u) || 
						(((p + note->data[PIZ_DURATION]) >= a) && ((p + note->data[PIZ_DURATION]) <= u))))
						{
							if (invert) 
								{
									if (!(note->flags & PIZ_SEQUENCE_NOTE_FLAG_LASSO)) {
										note->isSelected = !note->isSelected;
										note->flags |= PIZ_SEQUENCE_NOTE_FLAG_LASSO;
										k = 1;
											
										if (note == x->markedNote) {
												x->markedNote = NULL;
											}
									}
								}
							else if (!note->isSelected)
								{
									note->isSelected = true;
									k = 1;
								}

						}
					else
						{
							if (invert)
								{
									if (note->flags & PIZ_SEQUENCE_NOTE_FLAG_LASSO) {
										note->isSelected = !note->isSelected;
										note->flags &= ~PIZ_SEQUENCE_NOTE_FLAG_LASSO;
										k = 1;
									}
								}
							else if (note->isSelected) 
								{
									note->isSelected = false;
									k = 1;
								}
						}
					
					note = nextNote;
				}
		}

	PIZUNLOCK
	
	return k;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceRemoveSelectedNotes (PIZSequence *x)
{
	long i;
	long err = PIZ_GOOD;
	
	PIZLOCK
	
	for (i = 0; i < pizGrowingArrayCount (x->map); i++)
		{	
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note)
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					
					if (note->isSelected)
						{
							if (note == x->markedNote) {
									x->markedNote = NULL;
								}
								
							if (!(err |= pizLinklistRemoveByPtr (x->timeline[p], (void *)note))) {
									x->count --;
								}
						}
						
					note = nextNote;
				}
		}
	
	PIZMAPFLAG
	pizSequenceLocalMakeMap (x);
	
	PIZUNLOCK
	
	return err; 
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError pizSequenceEncodeUndoToArray (PIZSequence *x, PIZGrowingArray *a)
{
	long err = PIZ_ERROR;
	
	PIZLOCK
	
	pizSequenceLocalMakeMap (x);
	
	if (a)
		{
			long i;
			
			err	 = PIZ_GOOD;
			err |= pizGrowingArrayAppend (a, PIZ_SEQUENCE_VERSION);
			err |= pizGrowingArrayAppend (a, x->start);
			err |= pizGrowingArrayAppend (a, x->end);
			err |= pizGrowingArrayAppend (a, x->down);
			err |= pizGrowingArrayAppend (a, x->up);
			err |= pizGrowingArrayAppend (a, x->count);
			
			for (i = 0; i < pizGrowingArrayCount (x->map); i++)
				{	
					PIZNote *note		= NULL;
					PIZNote *nextNote	= NULL;
					
					long p = pizGrowingArrayValueAtIndex (x->map, i);
					
					pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
					
					while (note)
						{
							pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
							
							err |= pizGrowingArrayAppend (a, note->originPosition);
							err |= pizGrowingArrayAppend (a, note->data[PIZ_PITCH]);
							err |= pizGrowingArrayAppend (a, note->data[PIZ_VELOCITY]);
							err |= pizGrowingArrayAppend (a, note->data[PIZ_DURATION]);
							err |= pizGrowingArrayAppend (a, note->data[PIZ_CHANNEL]);
							err |= pizGrowingArrayAppend (a, note->isSelected);
							err |= pizGrowingArrayAppend (a, (note == x->markedNote));
							
							note = nextNote;
						}
				}
		}
	
	PIZUNLOCK
	
	return err;
}

PIZError pizSequenceDecodeUndoWithArray (PIZSequence *x, PIZGrowingArray *a)
{
	long err = PIZ_ERROR;
	
	PIZLOCK
	
	if (a)
		{
			long i, t;
			
			err = PIZ_GOOD;
			
			pizSequenceLocalClear (x);

			if (t = pizGrowingArrayCount (a))
				{
					long k, version, count;
					long *ptr = pizGrowingArrayPtr (a);
					
					version		= pizGrowingArrayValueAtIndex (a, 0);
	
					x->start	= pizGrowingArrayValueAtIndex (a, 1);
					x->end		= pizGrowingArrayValueAtIndex (a, 2);
					x->down		= pizGrowingArrayValueAtIndex (a, 3);
					x->up		= pizGrowingArrayValueAtIndex (a, 4);
					count		= pizGrowingArrayValueAtIndex (a, 5);
					
					k = 6;
					
					for (i = 0; i < count; i++)
						{
							if (!(pizSequenceLocalAddNote (x, ptr + (i * PIZ_SEQUENCE_NOTE_SIZE) + k, 
								PIZ_SEQUENCE_ADD_MODE_UNSELECT))) {
									err |= PIZ_ERROR;
								}
						}
				}
		}

	PIZUNLOCK
	
	return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceUndoIsEqualToUndo (PIZGrowingArray *a, PIZGrowingArray *b)
{
	bool isEqual = false;
	long count = pizGrowingArrayCount (a);
			
	if (count == pizGrowingArrayCount (b))
		{
			long i, n, k;
			long *ptrA = pizGrowingArrayPtr (a);
			long *ptrB = pizGrowingArrayPtr (b);
			bool d = false;
		
			d |= (*(ptrA + 1) != *(ptrB + 1));
			d |= (*(ptrA + 2) != *(ptrB + 2));
			d |= (*(ptrA + 3) != *(ptrB + 3));
			d |= (*(ptrA + 4) != *(ptrB + 4));
			
			n = *(ptrA + 5);
			k = 6;
			
			for (i = 0; i < n; i++) {
				d |= (*(ptrA + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_POSITION) 
						!= *(ptrB + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_POSITION));
				d |= (*(ptrA + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_PITCH) 
						!= *(ptrB + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_PITCH));
				d |= (*(ptrA + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_VELOCITY) 
						!= *(ptrB + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_VELOCITY));
				d |= (*(ptrA + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_DURATION) 
						!= *(ptrB + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_DURATION));
				d |= (*(ptrA + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_CHANNEL) 
						!= *(ptrB + k + (i * PIZ_SEQUENCE_NOTE_SIZE) + PIZ_SEQUENCE_CHANNEL));
				
				if (d) {
						break;
					}
			}
			
			isEqual = !d;
		}
		
	return isEqual;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE void pizSequenceUnselectNotes (PIZSequence *x)
{
	long i;
	
	for (i = 0; i < pizGrowingArrayCount (x->map); i++)
		{	
			PIZNote *note		= NULL;
			PIZNote *nextNote	= NULL;
			
			long p = pizGrowingArrayValueAtIndex (x->map, i);
			
			pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
			
			while (note)
				{
					pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
					note->isSelected = false;
					note = nextNote;
				}
		}
	
	x->markedNote = NULL;
}

PIZ_INLINE long pizSequenceSnapRoundDuration (PIZSequence *x, long durationToBeSnapped)
{
	return (long)(floor ((durationToBeSnapped / (double)x->grid) + 0.5)) * x->grid;
}

PIZ_INLINE long pizSequenceSnapRoundPosition (PIZSequence *x, long positionToBeSnapped)
{
	if (positionToBeSnapped > 0) 
		{
			return (long)(floor ((positionToBeSnapped / (double)x->grid) + 0.5)) * x->grid;
		}
	else
		{
			return (long)(ceil ((positionToBeSnapped / (double)x->grid) - 0.5)) * x->grid;
		}
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
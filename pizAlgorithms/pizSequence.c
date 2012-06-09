/*
 * \file    pizSequence.c
 * \author  Jean Sapristi
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

#include "pizEvent.h"
#include "pizSequence.h"
#include "pizSequencePrivate.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_DEFAULT_START           0
#define PIZ_DEFAULT_END             96
#define PIZ_DEFAULT_DOWN            60
#define PIZ_DEFAULT_UP              71
#define PIZ_DEFAULT_CHANCE          100
#define PIZ_DEFAULT_CHANNEL         1
#define PIZ_DEFAULT_VELOCITY        90

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZSequence *pizSequenceNew (struct _PIZAgent *agent)
{
    PIZSequence *x = NULL;
    
    if (x = (PIZSequence *)malloc (sizeof(PIZSequence))) {
    //
    long argv1[ ] = { 0, PIZ_SEQUENCE_MAXIMUM_NOTES };
    long argv2[ ] = { PIZ_SEQUENCE_SIZE_LOOKUP, PIZ_SEQUENCE_MAXIMUM_NOTES };
    
    x->agent         = agent;
    x->map           = pizArrayNew (PIZ_SEQUENCE_MAXIMUM_NOTES);
    x->scale         = pizArrayNew (PIZ_MAGIC_SCALE);
    x->pattern       = pizArrayNew (PIZ_EVENT_DATA_SIZE);
    x->tempValues    = (long *)malloc (sizeof(long) * PIZ_SEQUENCE_SIZE_TEMP);
    x->tempNotes1    = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_SIZE_TEMP);
    x->tempNotes2    = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_SIZE_TEMP);
    x->tempHash      = pizHashTableNew (2, argv1);
    x->lookup        = pizHashTableNew (2, argv2);
    
    if (x->map           && 
        x->scale         &&
        x->pattern       &&
        x->tempValues    &&
        x->tempNotes1    &&
        x->tempNotes2    &&
        x->tempHash      &&
        x->lookup        &&
        x->agent         &&
        (x->timeline = (PIZLinklist **)calloc (PIZ_SEQUENCE_SIZE_TIMELINE, sizeof(PIZLinklist **)))) {

        x->flags = PIZ_SEQUENCE_FLAG_NONE;
        
        pizItemsetClear (&x->usedNotes);
        pizItemsetClear (&x->addedNotes);
        pizItemsetClear (&x->removedNotes);
        pizItemsetClear (&x->changedNotes);
                
        x->start     = PIZ_DEFAULT_START;
        x->end       = PIZ_DEFAULT_END;
        x->down      = PIZ_DEFAULT_DOWN;
        x->up        = PIZ_DEFAULT_UP;
        x->count     = 0;
        x->index     = 0;
        x->chance    = PIZ_DEFAULT_CHANCE;
        x->velocity  = 0;
        x->channel   = PIZ_DEFAULT_CHANNEL;
        x->chord     = 0;
        x->key       = PIZ_KEY_C;
        x->type      = PIZ_MODE_NONE;
        x->cell      = PIZ_EIGHTH_NOTE;
        x->value     = PIZ_EIGHTH_NOTE;
        
        x->seed = PIZ_SEED;
            
    } else {
        pizSequenceFree (x);
        x = NULL;
    }
    //
    }
    
    return x;
}

void pizSequenceFree (PIZSequence *x)
{
    if (x) {
    //
    if (x->timeline) {
    //
    long i;
    
    for (i = 0; i < PIZ_SEQUENCE_SIZE_TIMELINE; i++) {
        pizLinklistFree (x->timeline[i]);
    }
        
    free (x->timeline);
    x->timeline = NULL;
    //
    }
    
    pizArrayFree (x->map);
    pizArrayFree (x->scale);
    pizArrayFree (x->pattern);
    
    pizHashTableFree (x->tempHash);
    pizHashTableFree (x->lookup);
    
    if (x->tempValues) {
        free (x->tempValues);
    }
    if (x->tempNotes1) {
        free (x->tempNotes1);
    }
    if (x->tempNotes2) {
        free (x->tempNotes2);
    }
    
    free (x);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file    pizSequence.c
 * \author  Jean Sapristi
 * \date    May 12, 2012.
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

#include "pizSequence.h"
#include "pizEvent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

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

PIZSequence *pizSequenceNew (long size)
{
    PIZSequence *x = NULL;
    
    if (x = (PIZSequence *)malloc (sizeof(PIZSequence))) {
    //
    long argv1[ ] = { 0, PIZ_SEQUENCE_MAXIMUM_NOTES };
    long argv2[ ] = { PIZ_SEQUENCE_INIT_LOOKUP_SIZE, PIZ_SEQUENCE_MAXIMUM_NOTES };
    
    if (size > 0) {
        x->size = size;
    } else {
        x->size = PIZ_SEQUENCE_DEFAULT_SIZE;
    }
    
    x->map           = pizArrayNew (PIZ_SEQUENCE_MAXIMUM_NOTES);
    x->scale         = pizArrayNew (PIZ_MAGIC_SCALE);
    x->pattern       = pizArrayNew (PIZ_EVENT_DATA_SIZE);
    x->tempValues    = (long *)malloc (sizeof(long) * PIZ_SEQUENCE_INIT_TEMP_SIZE);
    x->tempNotes1    = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_INIT_TEMP_SIZE);
    x->tempNotes2    = (PIZNote **)malloc (sizeof(PIZNote *) * PIZ_SEQUENCE_INIT_TEMP_SIZE);
    x->tempHash      = pizHashTableNew (2, argv1);
    x->lookup        = pizHashTableNew (2, argv2);
    x->factorOracle  = pizFactorOracleNew  (0, NULL);
    x->galoisLattice = pizGaloisLatticeNew (0, NULL);
    
    if (x->map           && 
        x->scale         &&
        x->pattern       &&
        x->tempValues    &&
        x->tempNotes1    &&
        x->tempNotes2    &&
        x->tempHash      &&
        x->lookup        &&
        x->factorOracle  &&
        x->galoisLattice &&
        (x->timeline = (PIZLinklist **)calloc (x->size, sizeof(PIZLinklist **)))) {

        x->flags = PIZ_SEQUENCE_FLAG_NONE;
        
        pizItemsetClear (&x->usedNotes);
        pizItemsetClear (&x->addedNotes);
        pizItemsetClear (&x->removedNotes);
        pizItemsetClear (&x->changedNotes);
        
        x->seed = (unsigned int)time(NULL);
        
        x->start     = PIZ_DEFAULT_START;
        x->end       = PIZ_DEFAULT_END;
        x->down      = PIZ_DEFAULT_DOWN;
        x->up        = PIZ_DEFAULT_UP;
        x->count     = 0;
        x->index     = 0;
        x->chance    = PIZ_DEFAULT_CHANCE;
        x->channel   = PIZ_DEFAULT_CHANNEL;
        x->velocity  = 0;
        x->key       = PIZ_KEY_C;
        x->type      = PIZ_SCALE_NONE;
        x->cell      = PIZ_EIGHTH_NOTE;
        x->noteValue = PIZ_EIGHTH_NOTE;
            
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
        long i;
        
        for (i = 0; i < x->size; i++) {
            pizLinklistFree (x->timeline[i]);
        }
            
        free (x->timeline);
        x->timeline = NULL;
    }
    
    pizArrayFree (x->map);
    pizArrayFree (x->scale);
    pizArrayFree (x->pattern);
    
    pizHashTableFree (x->tempHash);
    pizHashTableFree (x->lookup);
    
    pizFactorOracleFree  (x->factorOracle);
    pizGaloisLatticeFree (x->galoisLattice);
    
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
/*
 * \file    pizSequenceMaxMSP.c
 * \author  Jean Sapristi
 * \date    31 janvier 2012
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

#include "pizSequenceApply.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool pizSequenceApplyAmbitus (PIZSequence *x)
{
    long i, scale;
    bool haveChanged = false;

    PIZLOCK
    
    scale = pizGrowingArrayCount (x->scale);

    for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
        PIZNote *note       = NULL;
        PIZNote *nextNote   = NULL;
        
        long p = pizGrowingArrayValueAtIndex (x->map, i);
        
        pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
            
        while (note) {
            long tempPitch;
            
            pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
            
            tempPitch = pizSequenceMovePitchToAmbitus (x, note->data[PIZ_PITCH]);
            
            if (note->data[PIZ_PITCH] != tempPitch) {
                    haveChanged = true;
                    note->data[PIZ_PITCH] = tempPitch;
                }
                
            note = nextNote;
        }
    }
    
    PIZUNLOCK
    
    return haveChanged;
}

bool pizSequenceApplyPattern (PIZSequence *x)
{
    long patternSize;
    
    bool haveChanged = false;
    
    PIZLOCK
    
    if (patternSize = pizGrowingArrayCount (x->pattern)) {
        long i;
        long index = 0;
        
        for (i = 0; i < pizGrowingArrayCount (x->map); i++) {   
            PIZNote *note       = NULL;
            PIZNote *nextNote   = NULL;
            
            long p = pizGrowingArrayValueAtIndex (x->map, i);
            
            pizLinklistPtrAtIndex (x->timeline[p], 0, (void **)&note);
            
            while (note) {
                long err = PIZ_GOOD;
                long newPosition;
                
                pizLinklistNextByPtr (x->timeline[p], (void *)note, (void **)&nextNote);
                
                newPosition = pizSequenceSnapPositionToPattern (x, note->position, patternSize);
                
                if (newPosition != note->position) {
                    err |= (newPosition < 0);
                    err |= (newPosition > (PIZ_SEQUENCE_TIMELINE_SIZE - MAX (note->data[PIZ_DURATION], 1)));
                    
                    if (err) {
                        pizSequenceRemoveNote (x, note);
                    } else {
                        x->notes1[index] = note;
                        x->values1[index] = newPosition;
                        index ++;
                        haveChanged = true;
                    }
                }
                    
                note = nextNote;
            }
        }
        
        if (haveChanged) {
            for (i = 0; i < index; i++) {
                pizSequenceMoveNote (x, x->notes1[i], x->values1[i]);
            }
        
            pizSequenceMakeMap (x);
        }
    }
    
    PIZUNLOCK
    
    return haveChanged; 
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
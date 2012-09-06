/*
 * \file    pizSequenceAttributes.c
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

#include "pizSequenceRun.h"
#include "pizSequenceEach.h"
#include "pizSequenceAttributes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static const long pizSequenceModes[ ]   = { 0,-1, 0, 1, 0, 0, 1, 0,-1, 0, 1, 0,     // Ionian
                                            0,-1, 0, 0,-1, 0, 1, 0,-1, 0, 0,-1,     // Dorian
                                            0, 0,-1, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Phrygian
                                            0,-1, 0, 1, 0, 1, 0, 0,-1, 0, 1, 0,     // Lydian
                                            0,-1, 0, 1, 0, 0, 1, 0,-1, 0, 0,-1,     // Mixolydian
                                            0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Aeolian
                                            0, 0,-1, 0,-1, 0, 0,-1, 0,-1, 0,-1,     // Locrian
                                            0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 0,-1,     // Natural
                                            0,-1, 0, 0,-1, 0, 1, 0, 0,-1, 1, 0,     // Harmonic
                                            0,-1, 0, 0,-1, 0, 1, 0,-1, 0, 1, 0,     // Melodic
                                            0,-1, 0,-1, 0,-1, 0, 1, 0,-1, 0, 1,     // Whole tone
                                            0,-1, 0,-1, 0,-1, 1, 0,-1, 0,-1, 1,     // Pentatonic major
                                            0,-1, 1, 0, 1, 0,-1, 0,-1, 1, 0,-1,     // Pentatonic minor
                                            0, 0,-1, 0, 0, 1, 0, 0, 1, 0, 0,-1,     // Octatonic half whole
                                            0,-1, 0, 0, 1, 0, 0,-1, 0, 0, 1, 0,     // Octatonic whole half
                                            0,-1,-2, 1, 0,-1, 1, 0,-1,-2, 1, 0,     // Seventh major
                                            0,-1,-2, 1, 0,-1, 1, 0,-1, 1, 0,-1,     // Seventh dominant
                                            0,-1, 1, 0,-1,-2, 1, 0,-1, 1, 0,-1,     // Seventh minor
                                            0,-1,-2, 1, 0,-1, 1, 0, 1, 0,-1,-2,     // Seventh major sixth
                                            0,-1, 1, 0,-1,-2, 1, 0, 1, 0,-1,-2,     // Seventh minor sixth
                                            0,-1, 1, 0,-1, 1, 0,-1,-2, 1, 0,-1,     // Seventh half diminished
                                            0,-1, 1, 0,-1, 1, 0,-1, 1, 0,-1,-2,     // Seventh diminished
                                            0,-1,-2, 2, 1, 0, 1, 0,-1, 1, 0,-1,     // Seventh suspended
                                            0,-1,-2, 1, 0,-1, 2, 1, 0,-1, 0,-1,     // Seventh sharp five
                                            0,-1,-2, 1, 0, 1, 0,-1,-2, 1, 0,-1  };  // Seventh flat five

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL bool pizSequenceIsValidNoteValue(long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceSetBpm(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        x->bpm = argv[0];
        x->flags |= PIZ_SEQUENCE_FLAG_BPM;
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetChance(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((argv[0] >= 0) && (argv[0] <= 100)) {
            x->chance = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_CHANCE;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetVelocity(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((argv[0] >= -PIZ_MAGIC_VELOCITY) && (argv[0] <= PIZ_MAGIC_VELOCITY)) {
            x->velocity = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_VELOCITY;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetChannel(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((argv[0] >= 0) && (argv[0] <= PIZ_MAGIC_CHANNEL)) {
            x->channel = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_CHANNEL;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetChord(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((argv[0] >= 0) && (argv[0] < PIZ_MAGIC_ULONG)) {
            x->chord = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_CHORD;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetCell(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((pizSequenceIsValidNoteValue(argv[0]))) {
            x->cell = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_CELL;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetValue(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        if ((pizSequenceIsValidNoteValue(argv[0]))) {
            x->value = argv[0];
            x->flags |= PIZ_SEQUENCE_FLAG_VALUE;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetScale(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZError err = PIZ_ERROR;
    
    if ((!(pizEventData(event, &argc, &argv))) && (argc > 1)) {
    //
    pizArrayClear(x->scale);
        
    x->key  = CLAMP(argv[0], PIZ_KEY_C, PIZ_KEY_B);
    x->type = CLAMP(argv[1], PIZ_MODE_NONE, PIZ_SEVENTH_FLAT_FIVE);
    
    if (x->type != PIZ_MODE_NONE) {
        long i;
        const long *ptr = NULL;
        
        ptr = pizSequenceModes + (x->type * PIZ_MAGIC_SCALE); 
        
        for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
            err |= pizArrayAppend(x->scale, *(ptr + ((PIZ_MAGIC_SCALE - x->key + i) % PIZ_MAGIC_SCALE)));
        }
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_SCALE;
    //
    } 
    
    return err;
}   

PIZError pizSequenceSetMute(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        x->mute = (argv[0] != 0);
        x->flags |= PIZ_SEQUENCE_FLAG_MUTE;
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetZone(PIZSequence *x, const PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
    //
    long i;
    long v[ ] = { x->start, x->end, x->down, x->up };
    
    for (i = 0; i < MIN(argc, 4); i++) {
        v[i] = argv[i];
    }
    
    v[0] = pizSequenceSnapByCell(x, v[0]);
    v[1] = pizSequenceSnapByCell(x, v[1]);
    
    v[0] = CLAMP(v[0], 0, PIZ_SEQUENCE_SIZE_TIMELINE);
    v[1] = CLAMP(v[1], 0, PIZ_SEQUENCE_SIZE_TIMELINE);
    v[2] = CLAMP(v[2], 0, PIZ_MAGIC_PITCH);
    v[3] = CLAMP(v[3], 0, PIZ_MAGIC_PITCH);
    
    if (v[0] != v[1]) {
    //
    if (v[0] < v[1]) { x->start = v[0]; x->end = v[1]; } else { x->end = v[0]; x->start = v[1]; }
    if (v[2] < v[3]) { x->down = v[2]; x->up = v[3]; } else { x->up = v[2]; x->down = v[3]; }
    //
    }
        
    x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
    //
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

bool pizSequenceIsValidNoteValue(long value) 
{
    if (value == PIZ_WHOLE_NOTE_DOTTED          ||
        value == PIZ_WHOLE_NOTE                 ||
        value == PIZ_WHOLE_NOTE_TRIPLET         ||
        value == PIZ_HALF_NOTE_DOTTED           ||
        value == PIZ_HALF_NOTE                  ||
        value == PIZ_HALF_NOTE_TRIPLET          ||
        value == PIZ_QUARTER_NOTE_DOTTED        ||
        value == PIZ_QUARTER_NOTE               ||
        value == PIZ_QUARTER_NOTE_TRIPLET       ||
        value == PIZ_EIGHTH_NOTE_DOTTED         ||
        value == PIZ_EIGHTH_NOTE                ||
        value == PIZ_EIGHTH_NOTE_TRIPLET        ||
        value == PIZ_SIXTEENTH_NOTE_DOTTED      ||
        value == PIZ_SIXTEENTH_NOTE             ||
        value == PIZ_SIXTEENTH_NOTE_TRIPLET     ||
        value == PIZ_THIRTY_SECOND_NOTE         ||
        value == PIZ_THIRTY_SECOND_NOTE_TRIPLET ||
        value == PIZ_NOTE_NONE) {
        return true;
    }
        
    return false;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
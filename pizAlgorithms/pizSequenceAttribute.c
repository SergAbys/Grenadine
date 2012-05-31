/*
 * \file    pizSequenceAttribute.c
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

#include "pizSequenceAttribute.h"

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

PIZ_LOCAL bool pizSequenceIsValidNoteValue (long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceSetChance (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if ((value >= 0) && (value <= 100)) {
            x->chance = value;
            x->flags |= PIZ_SEQUENCE_FLAG_CHANCE;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetVelocity (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if ((value >= -PIZ_MAGIC_VELOCITY) && (value <= PIZ_MAGIC_VELOCITY)) {
            x->velocity = value;
            x->flags |= PIZ_SEQUENCE_FLAG_VELOCITY;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetChannel (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if ((value >= 0) && (value <= PIZ_MAGIC_CHANNEL)) {
            x->channel = value;
            x->flags |= PIZ_SEQUENCE_FLAG_CHANNEL;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetChord (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if ((value >= 0) && (value < PIZ_MAGIC_ULONG)) {
            x->chord = value;
            x->flags |= PIZ_SEQUENCE_FLAG_CHORD;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetCell (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if (pizSequenceIsValidNoteValue (value)) {
            x->cell = value;
            x->flags |= PIZ_SEQUENCE_FLAG_CELL;
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceSetNoteValue (PIZSequence *x, const PIZEvent *event)
{
    long value;
    
    if (!(pizEventValue (event, &value))) {
        if (pizSequenceIsValidNoteValue (value)) {
            x->noteValue = value;
            x->flags |= PIZ_SEQUENCE_FLAG_NOTE_VALUE;
        }
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizSequenceSetScale (PIZSequence *x, const PIZEvent *event)
{
    long value, option;
    PIZError err = PIZ_GOOD;
    
    pizArrayClear (x->scale);
    
    if (!(pizEventValue (event, &value))) {
    //
    pizEventOption (event, &option);
    
    x->type = CLAMP (value, PIZ_NONE, PIZ_SEVENTH_FLAT_FIVE);
    x->key  = CLAMP (option, PIZ_KEY_C, PIZ_KEY_B);
    
    if (x->type != PIZ_NONE) {
        long       i;
        const long *ptr = NULL;
        
        ptr = pizSequenceModes + (x->type * PIZ_MAGIC_SCALE); 
        
        for (i = 0; i < PIZ_MAGIC_SCALE; i++) {
            err |= pizArrayAppend (x->scale, *(ptr + ((PIZ_MAGIC_SCALE - x->key + i) % PIZ_MAGIC_SCALE)));
        }
    }
    //
    } else {
        x->key  = PIZ_KEY_C;
        x->type = PIZ_NONE;
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_SCALE;
    
    return err;
}   

PIZError pizSequenceSetPattern (PIZSequence *x, const PIZEvent *event)
{
    //long     argc;
    //long     *argv = NULL;
    PIZError err = PIZ_GOOD;
    /*
    pizArrayClear (x->pattern);
    
    if (!(pizEventPtr (event, &argc, &argv))) {
        long i;
        for (i = 0; i < argc; i++) {
            err |= pizArrayAppend (x->pattern, argv[i]);
        }
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_PATTERN;*/
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

bool pizSequenceIsValidNoteValue (long value) 
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
        value == PIZ_NONE) {
        return true;
    }
        
    return false;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
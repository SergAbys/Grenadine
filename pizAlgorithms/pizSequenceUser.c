/*
 * \file    pizSequenceUser.c
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

#include "pizSequenceUser.h"
#include "pizSequenceRun.h"
#include "pizSequenceEach.h"
#include "pizSequenceUtils.h"
#include "pizSequenceMethods.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_SEQUENCE_REPEAT_PITCH       12
#define PIZ_SEQUENCE_REPEAT_VELOCITY    10

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
   
PIZ_LOCAL PIZNote *pizSequenceNoteWithTag   (PIZSequence *x, long tag);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL long pizSequenceLengthUp      (long value);
PIZ_LOCAL long pizSequenceLengthDown    (long value);
PIZ_LOCAL long pizSequenceLengthRight   (long value);
PIZ_LOCAL long pizSequenceLengthLeft    (long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceLength(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
        ;
    }
    
    return PIZ_GOOD;
}

/*
PIZError pizSequenceCellUp(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLengthUp(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellDown(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLengthDown(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellRight(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLengthRight(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellLeft(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLengthLeft(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceValueUp(PIZSequence *x, PIZEvent *event)
{
    x->value = pizSequenceLengthUp(x->value);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_VALUE;
    
    return PIZ_GOOD;
}

PIZError pizSequenceValueDown(PIZSequence *x, PIZEvent *event)
{
    x->value = pizSequenceLengthDown(x->value);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_VALUE;
    
    return PIZ_GOOD;
}

PIZError pizSequenceValueRight(PIZSequence *x, PIZEvent *event)
{
    x->value = pizSequenceLengthRight(x->value);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_VALUE;
    
    return PIZ_GOOD;
}

PIZError pizSequenceValueLeft(PIZSequence *x, PIZEvent *event)
{
    x->value = pizSequenceLengthLeft(x->value);
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_VALUE;
    
    return PIZ_GOOD;
}*/

PIZError pizSequenceNoteDelete(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachRemove(x, note, NULL, PIZ_SEQUENCE_FLAG_NONE);
        }
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceNoteIncrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
    //
    if (note = pizSequenceNoteWithTag(x, argv[1])) {
        long a[2];
        long step = 1;
        
        if ((argv[0] == PIZ_VALUE_PITCH) && (argc > 2) && argv[2]) {
            step = PIZ_SEQUENCE_REPEAT_PITCH;
            
        } else if ((argv[0] == PIZ_VALUE_VELOCITY) && (argc > 2) && argv[2]) {
            step = PIZ_SEQUENCE_REPEAT_VELOCITY;
            
        } else if (argv[0] == PIZ_VALUE_DURATION) {
            step = x->value;
        } 
        
        a[0] = argv[0];
        a[1] = step;
        
        pizEventSetData(event, 2, a);
        pizSequenceEachChange(x, note, event, PIZ_SEQUENCE_FLAG_NONE);
    }
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceNoteDecrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
    //
    if (note = pizSequenceNoteWithTag(x, argv[1])) {
        long a[2];
        long step = -1;
        
        if ((argv[0] == PIZ_VALUE_PITCH) && (argc > 2) && argv[2]) {
            step = -PIZ_SEQUENCE_REPEAT_PITCH;
            
        } else if ((argv[0] == PIZ_VALUE_VELOCITY) && (argc > 2) && argv[2]) {
            step = -PIZ_SEQUENCE_REPEAT_VELOCITY;
            
        } else if (argv[0] == PIZ_VALUE_DURATION) {
            step = -x->value;
        }
        
        a[0] = argv[0];
        a[1] = step;
        
        pizEventSetData(event, 2, a);
        pizSequenceEachChange(x, note, event, PIZ_SEQUENCE_FLAG_NONE);
    }
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceNoteForward(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    x->temp.error = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachMove(x, note, event, PIZ_SEQUENCE_FLAG_EACH_FORWARD);
        }
    }
    
    return x->temp.error;
}

PIZError pizSequenceNoteBackward(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    x->temp.error = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachMove(x, note, event, PIZ_SEQUENCE_FLAG_EACH_BACKWARD);
        }
    }
    
    return x->temp.error;
}

PIZError pizSequenceZoneIncrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
    //
    long a, b;
    long hStep = x->cell;
    long vStep = 1;
    
    if ((argc > 1) && argv[1]) {
        vStep = PIZ_SEQUENCE_REPEAT_PITCH;
    }
    
    switch (argv[0]) {
    //
    case (PIZ_ZONE_START | PIZ_ZONE_END) :
        a = pizSequenceSnapByCell(x, x->start + hStep);
        b = pizSequenceSnapByCell(x, x->end + hStep);
        if ((a != b) && (b <= PIZ_SEQUENCE_SIZE_TIMELINE)) {
            x->start = a;
            x->end = b;
        }
        break;
    
    case (PIZ_ZONE_DOWN | PIZ_ZONE_UP) :
        a = x->down + vStep;
        b = x->up + vStep;
        if (b < PIZ_MAGIC_PITCH) {
            x->down = a;
            x->up = b;
        }
        break;
        
    case PIZ_ZONE_START : 
        a = pizSequenceSnapByCell(x, x->start + hStep);
        if (a < x->end) {
            x->start = a;
        }
        break;
        
    case PIZ_ZONE_END : 
        a = pizSequenceSnapByCell(x, x->end + hStep);
        if (a <= PIZ_SEQUENCE_SIZE_TIMELINE) {
            x->end = a; 
        }
        break;
        
    case PIZ_ZONE_DOWN : 
        a = x->down + vStep;
        if (a <= x->up) {
            x->down = a;
        }
        break;
        
    case PIZ_ZONE_UP :
        a = x->up + vStep;
        if (a <= PIZ_MAGIC_PITCH) {
            x->up = a; 
        }
        break;
    //
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_ZONE;
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceZoneDecrement(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    
    if (!(pizEventData(event, &argc, &argv))) {
    //
    long a, b;
    long hStep = x->cell;
    long vStep = 1;
    
    if ((argc > 1) && argv[1]) {
        vStep = PIZ_SEQUENCE_REPEAT_PITCH;
    }
    
    switch (argv[0]) {
    //
    case (PIZ_ZONE_START | PIZ_ZONE_END) :
        a = pizSequenceSnapByCell(x, x->start - hStep);
        b = pizSequenceSnapByCell(x, x->end - hStep);
        if ((a != x->start) && (a != b) && (a >= 0)) {
            x->start = a;
            x->end = b;
        }
        break;
    
    case (PIZ_ZONE_DOWN | PIZ_ZONE_UP) :
        a = x->down - vStep;
        b = x->up - vStep;
        if (a >= 0) {
            x->down = a;
            x->up = b;
        }
        break;
        
    case PIZ_ZONE_START : 
        a = pizSequenceSnapByCell(x, x->start - hStep);
        if (a >= 0) {
            x->start = a;
        }
        break;
        
    case PIZ_ZONE_END : 
        a = pizSequenceSnapByCell(x, x->end - hStep);
        if (a > x->start) {
            x->end = a; 
        }
        break;
        
    case PIZ_ZONE_DOWN : 
        a = x->down - vStep;
        if (a >= 0) {
            x->down = a;
        }
        break;
        
    case PIZ_ZONE_UP : 
        a = x->up - vStep;
        if (a >= x->down) {
            x->up = a; 
        }
        break;
    //
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_NOTIFY_ZONE;
    //
    }
    
    return PIZ_GOOD;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZNote *pizSequenceNoteWithTag(PIZSequence *x, long tag)
{
    PIZNote *note = NULL;
    
    if ((tag >= 0) 
        && (tag < PIZ_SEQUENCE_MAXIMUM_NOTES) 
        && !(pizItemsetIsSetAtIndex(&x->removed, tag))
        && !(pizItemsetIsSetAtIndex(&x->changed, tag))) {
        note = x->lookup[tag];
    }
    
    return note;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long pizSequenceLengthUp(long value)
{
    switch (value) {
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET : return PIZ_SIXTEENTH_NOTE_TRIPLET;
        case PIZ_THIRTY_SECOND_NOTE         : return PIZ_SIXTEENTH_NOTE;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     : return PIZ_EIGHTH_NOTE_TRIPLET;
        case PIZ_SIXTEENTH_NOTE             : return PIZ_EIGHTH_NOTE;
        case PIZ_SIXTEENTH_NOTE_DOTTED      : return PIZ_EIGHTH_NOTE_DOTTED;
        case PIZ_EIGHTH_NOTE_TRIPLET        : return PIZ_QUARTER_NOTE_TRIPLET;
        case PIZ_EIGHTH_NOTE                : return PIZ_QUARTER_NOTE;
        case PIZ_EIGHTH_NOTE_DOTTED         : return PIZ_QUARTER_NOTE_DOTTED;
        case PIZ_QUARTER_NOTE_TRIPLET       : return PIZ_HALF_NOTE_TRIPLET;
        case PIZ_QUARTER_NOTE               : return PIZ_HALF_NOTE;
        case PIZ_QUARTER_NOTE_DOTTED        : return PIZ_HALF_NOTE_DOTTED;
        case PIZ_HALF_NOTE_TRIPLET          : return PIZ_WHOLE_NOTE_TRIPLET;
        case PIZ_HALF_NOTE                  : return PIZ_WHOLE_NOTE;
        case PIZ_HALF_NOTE_DOTTED           : return PIZ_WHOLE_NOTE_DOTTED;
        case PIZ_WHOLE_NOTE_TRIPLET         : return PIZ_THIRTY_SECOND_NOTE_TRIPLET;
        case PIZ_WHOLE_NOTE                 : return PIZ_THIRTY_SECOND_NOTE;
        case PIZ_WHOLE_NOTE_DOTTED          : return PIZ_SIXTEENTH_NOTE_DOTTED;
    }
    
    return value;
}

long pizSequenceLengthDown(long value)
{
    switch (value) {
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET : return PIZ_WHOLE_NOTE_TRIPLET;
        case PIZ_THIRTY_SECOND_NOTE         : return PIZ_WHOLE_NOTE;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     : return PIZ_THIRTY_SECOND_NOTE_TRIPLET;
        case PIZ_SIXTEENTH_NOTE             : return PIZ_THIRTY_SECOND_NOTE;
        case PIZ_SIXTEENTH_NOTE_DOTTED      : return PIZ_WHOLE_NOTE_DOTTED;
        case PIZ_EIGHTH_NOTE_TRIPLET        : return PIZ_SIXTEENTH_NOTE_TRIPLET;
        case PIZ_EIGHTH_NOTE                : return PIZ_SIXTEENTH_NOTE;
        case PIZ_EIGHTH_NOTE_DOTTED         : return PIZ_SIXTEENTH_NOTE_DOTTED;
        case PIZ_QUARTER_NOTE_TRIPLET       : return PIZ_EIGHTH_NOTE_TRIPLET;
        case PIZ_QUARTER_NOTE               : return PIZ_EIGHTH_NOTE;
        case PIZ_QUARTER_NOTE_DOTTED        : return PIZ_EIGHTH_NOTE_DOTTED;
        case PIZ_HALF_NOTE_TRIPLET          : return PIZ_QUARTER_NOTE_TRIPLET;
        case PIZ_HALF_NOTE                  : return PIZ_QUARTER_NOTE;
        case PIZ_HALF_NOTE_DOTTED           : return PIZ_QUARTER_NOTE_DOTTED;
        case PIZ_WHOLE_NOTE_TRIPLET         : return PIZ_HALF_NOTE_TRIPLET;
        case PIZ_WHOLE_NOTE                 : return PIZ_HALF_NOTE;
        case PIZ_WHOLE_NOTE_DOTTED          : return PIZ_HALF_NOTE_DOTTED;
    }
    
    return value;
}

long pizSequenceLengthRight(long value)
{
    switch (value) {
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET : return PIZ_THIRTY_SECOND_NOTE;
        case PIZ_THIRTY_SECOND_NOTE         : return PIZ_THIRTY_SECOND_NOTE_TRIPLET;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     : return PIZ_SIXTEENTH_NOTE;
        case PIZ_SIXTEENTH_NOTE             : return PIZ_SIXTEENTH_NOTE_DOTTED;
        case PIZ_SIXTEENTH_NOTE_DOTTED      : return PIZ_SIXTEENTH_NOTE_TRIPLET;
        case PIZ_EIGHTH_NOTE_TRIPLET        : return PIZ_EIGHTH_NOTE;
        case PIZ_EIGHTH_NOTE                : return PIZ_EIGHTH_NOTE_DOTTED;
        case PIZ_EIGHTH_NOTE_DOTTED         : return PIZ_EIGHTH_NOTE_TRIPLET;
        case PIZ_QUARTER_NOTE_TRIPLET       : return PIZ_QUARTER_NOTE;
        case PIZ_QUARTER_NOTE               : return PIZ_QUARTER_NOTE_DOTTED;
        case PIZ_QUARTER_NOTE_DOTTED        : return PIZ_QUARTER_NOTE_TRIPLET;
        case PIZ_HALF_NOTE_TRIPLET          : return PIZ_HALF_NOTE;
        case PIZ_HALF_NOTE                  : return PIZ_HALF_NOTE_DOTTED;
        case PIZ_HALF_NOTE_DOTTED           : return PIZ_HALF_NOTE_TRIPLET;
        case PIZ_WHOLE_NOTE_TRIPLET         : return PIZ_WHOLE_NOTE;
        case PIZ_WHOLE_NOTE                 : return PIZ_WHOLE_NOTE_DOTTED;
        case PIZ_WHOLE_NOTE_DOTTED          : return PIZ_WHOLE_NOTE_TRIPLET;
    }
    
    return value;
}

long pizSequenceLengthLeft(long value)
{
    switch (value) {
        case PIZ_THIRTY_SECOND_NOTE_TRIPLET : return PIZ_THIRTY_SECOND_NOTE;
        case PIZ_THIRTY_SECOND_NOTE         : return PIZ_THIRTY_SECOND_NOTE_TRIPLET;
        case PIZ_SIXTEENTH_NOTE_TRIPLET     : return PIZ_SIXTEENTH_NOTE_DOTTED;
        case PIZ_SIXTEENTH_NOTE             : return PIZ_SIXTEENTH_NOTE_TRIPLET;
        case PIZ_SIXTEENTH_NOTE_DOTTED      : return PIZ_SIXTEENTH_NOTE;
        case PIZ_EIGHTH_NOTE_TRIPLET        : return PIZ_EIGHTH_NOTE_DOTTED;
        case PIZ_EIGHTH_NOTE                : return PIZ_EIGHTH_NOTE_TRIPLET;
        case PIZ_EIGHTH_NOTE_DOTTED         : return PIZ_EIGHTH_NOTE;
        case PIZ_QUARTER_NOTE_TRIPLET       : return PIZ_QUARTER_NOTE_DOTTED;
        case PIZ_QUARTER_NOTE               : return PIZ_QUARTER_NOTE_TRIPLET;
        case PIZ_QUARTER_NOTE_DOTTED        : return PIZ_QUARTER_NOTE;
        case PIZ_HALF_NOTE_TRIPLET          : return PIZ_HALF_NOTE_DOTTED;
        case PIZ_HALF_NOTE                  : return PIZ_HALF_NOTE_TRIPLET;
        case PIZ_HALF_NOTE_DOTTED           : return PIZ_HALF_NOTE;
        case PIZ_WHOLE_NOTE_TRIPLET         : return PIZ_WHOLE_NOTE_DOTTED;
        case PIZ_WHOLE_NOTE                 : return PIZ_WHOLE_NOTE_TRIPLET;
        case PIZ_WHOLE_NOTE_DOTTED          : return PIZ_WHOLE_NOTE;
    }
    
    return value;
}
    
// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
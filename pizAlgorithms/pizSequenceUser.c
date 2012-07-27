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

#define PIZ_SEQUENCE_AUTOREPEAT_PITCH       12
#define PIZ_SEQUENCE_AUTOREPEAT_VELOCITY    10

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
   
PIZ_LOCAL PIZNote   *pizSequenceNoteWithTag         (PIZSequence *x, long tag);
PIZ_LOCAL long      pizSequenceLenghtIncrement      (long value);
PIZ_LOCAL long      pizSequenceLenghtDecrement      (long value);
PIZ_LOCAL long      pizSequenceLenghtUp             (long value);
PIZ_LOCAL long      pizSequenceLenghtDown           (long value);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZError pizSequenceDelete(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachRemove(x, note, NULL, PIZ_SEQUENCE_FLAG_NONE);
            pizSequenceMakeMap(x);
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
            step = PIZ_SEQUENCE_AUTOREPEAT_PITCH;
            
        } else if ((argv[0] == PIZ_VALUE_VELOCITY) && (argc > 2) && argv[2]) {
            step = PIZ_SEQUENCE_AUTOREPEAT_VELOCITY;
            
        } else if (argv[0] == PIZ_VALUE_DURATION) {
            step = x->cell;
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
            step = -PIZ_SEQUENCE_AUTOREPEAT_PITCH;
            
        } else if ((argv[0] == PIZ_VALUE_VELOCITY) && (argc > 2) && argv[2]) {
            step = -PIZ_SEQUENCE_AUTOREPEAT_VELOCITY;
            
        } else if (argv[0] == PIZ_VALUE_DURATION) {
            step = -x->cell;
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
    
    x->tempError = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachMove(x, note, event, PIZ_SEQUENCE_FLAG_FORWARD);
            pizSequenceMakeMap(x);
        }
    }
    
    return x->tempError;
}

PIZError pizSequenceNoteBackward(PIZSequence *x, PIZEvent *event)
{
    long argc;
    long *argv = NULL;
    PIZNote *note = NULL;
    
    x->tempError = PIZ_GOOD;
    
    if (!(pizEventData(event, &argc, &argv)) && (argc > 1)) {
        if (note = pizSequenceNoteWithTag(x, argv[1])) {
            pizSequenceEachMove(x, note, event, PIZ_SEQUENCE_FLAG_BACKWARD);
            pizSequenceMakeMap(x);
        }
    }
    
    return x->tempError;
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
        vStep = PIZ_SEQUENCE_AUTOREPEAT_PITCH;
    }
    
    switch (argv[0]) {
    //
    case PIZ_ZONE_START : 
        a = pizSequenceSnapByCell(x, x->start + hStep);
        b = pizSequenceSnapByCell(x, x->end - hStep);
        x->start = MIN(a, b);
        break;
        
    case PIZ_ZONE_END : 
        a = pizSequenceSnapByCell(x, x->end + hStep);
        x->end = MIN(a, PIZ_SEQUENCE_SIZE_TIMELINE); 
        break;
        
    case PIZ_ZONE_DOWN : 
        x->down = MIN(x->down + vStep, x->up);
        break;
        
    case PIZ_ZONE_UP : 
        x->up = MIN(x->up + vStep, PIZ_MAGIC_PITCH); 
        break;
    //
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
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
        vStep = PIZ_SEQUENCE_AUTOREPEAT_PITCH;
    }
    
    switch (argv[0]) {
    //
    case PIZ_ZONE_START : 
        a = pizSequenceSnapByCell(x, x->start - hStep);
        x->start = MAX(a, 0);
        break;
        
    case PIZ_ZONE_END : 
        a = pizSequenceSnapByCell(x, x->end - hStep);
        b = pizSequenceSnapByCell(x, x->start + hStep);
        x->end = MAX(a, b); 
        break;
        
    case PIZ_ZONE_DOWN : 
        x->down = MAX(x->down - vStep, 0);
        break;
        
    case PIZ_ZONE_UP : 
        x->up = MAX(x->up - vStep, x->down); 
        break;
    //
    }
    
    x->flags |= PIZ_SEQUENCE_FLAG_ZONE;
    //
    }
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellIncrement(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLenghtIncrement(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellDecrement(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLenghtDecrement(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellUp(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLenghtUp(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_CELL;
    
    return PIZ_GOOD;
}

PIZError pizSequenceCellDown(PIZSequence *x, PIZEvent *event)
{
    x->cell = pizSequenceLenghtDown(x->cell);
    x->flags |= PIZ_SEQUENCE_FLAG_CELL;
    
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

long pizSequenceLenghtIncrement(long value)
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

long pizSequenceLenghtDecrement(long value)
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
        case PIZ_HALF_NOTE_TRIPLET          : return PIZ_EIGHTH_NOTE_TRIPLET;
        case PIZ_HALF_NOTE                  : return PIZ_EIGHTH_NOTE;
        case PIZ_HALF_NOTE_DOTTED           : return PIZ_EIGHTH_NOTE_DOTTED;
        case PIZ_WHOLE_NOTE_TRIPLET         : return PIZ_HALF_NOTE_TRIPLET;
        case PIZ_WHOLE_NOTE                 : return PIZ_HALF_NOTE;
        case PIZ_WHOLE_NOTE_DOTTED          : return PIZ_HALF_NOTE_DOTTED;
    }
    
    return value;
}

long pizSequenceLenghtUp(long value)
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

long pizSequenceLenghtDown(long value)
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
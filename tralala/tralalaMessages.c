/*
 *  tralalaMessages.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 13/02/12.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern tralalaSymbolsTableA tll_symbolsA;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_copy (t_tralala *x)
{
    PIZError err = PIZ_GOOD;
    
    ARRAY_GET (tempArrayA);
    ARRAY_GET (tempArrayB);
    
    if (tempArrayA && tempArrayB)  {
        err |= pizSequenceNotesToArray (x->listen, tempArrayA, tempArrayA);
    } else {
        err |= PIZ_MEMORY;
    }
    
    if (!err) {
        long i;
        long chance = object_attr_getlong (x, tll_sym_chance);
        long count = pizGrowingArrayCount (tempArrayA) / PIZ_DATA_NOTE_SIZE;
        long *ptr = pizGrowingArrayPtr (tempArrayA);
        
        pizGrowingArrayClear (tempArrayB);

        for (i = (count - 1); i >= 0; i--) {
            if (100 * (rand ( ) / (RAND_MAX + 1.0)) < chance) {
            pizGrowingArrayAppendPtr (tempArrayB, PIZ_DATA_NOTE_SIZE, ptr + (PIZ_DATA_NOTE_SIZE * i));
            }
        }
        
        pizSequenceAddNotesWithArray (x->live, tempArrayB, PIZ_ADD_FLAG_PATTERN);
    }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
    
    if (LIVE) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_CHANGE);
    }
}

void tralala_enter (t_tralala *x)
{
    tralala_setLiveByUser (x);
    
    if (LIVE) {
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_CHANGE);
    }
}

void tralala_setLiveByUser (t_tralala *x)
{
    PIZError err = PIZ_GOOD;
    
    ARRAY_GET (tempArrayA);
    ARRAY_GET (tempArrayB);
    
    if (tempArrayA && tempArrayB)  {
        err |= pizSequenceNotesToArray (x->user, tempArrayA, tempArrayA);
        pizSequenceZoneToArray (x->user, tempArrayB);
    } else {
        err |= PIZ_MEMORY;
    }
    
    if (!err) {
        long addFlags = PIZ_ADD_FLAG_PATTERN | PIZ_ADD_FLAG_CLEAR;
        
        pizSequenceAddNotesWithArray (x->live, tempArrayA, addFlags);
        pizSequenceSetZoneWithArray  (x->live, tempArrayB);
        pizSequenceSetZoneWithArray  (x->listen, tempArrayB);
    }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_int (t_tralala *x, long n)
{
    if (n >= 0 && n <= PIZ_MAGIC_PITCH) {
        systhread_mutex_lock (&x->learnMutex);
        pizBoundedQueueAppend (x->learnQueue, n);
        systhread_mutex_unlock (&x->learnMutex);
    }
}

void tralala_list (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && argv && (atom_gettype (argv) == A_LONG)) {
        long n = atom_getlong (argv);
            
        if (n >= 0 && n <= PIZ_MAGIC_PITCH) {
            systhread_mutex_lock (&x->learnMutex);
            pizBoundedQueueAppend (x->learnQueue, n);
            systhread_mutex_unlock (&x->learnMutex);
        }
    }
}

void tralala_mute (t_tralala *x, long n)
{   
    systhread_mutex_lock (&x->methodsMutex);
    
    if (n) {
        x->flags |= FLAG_IS_MUTED;
    } else {
        x->flags &= ~FLAG_IS_MUTED;
    }
    
    systhread_mutex_unlock (&x->methodsMutex);
}

void tralala_forget (t_tralala *x)
{
    systhread_mutex_lock (&x->algorithmsMutex);
    
    pizFactorOracleClear    (x->factorOracle);
    pizGaloisLatticeClear   (x->galoisLattice);
    pizFiniteStateClear     (x->finiteState);
    
    systhread_mutex_unlock (&x->algorithmsMutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_learnTask (t_tralala *x)
{
    if (x->learnCycle == PIZ_ALGORITHM_NONE) {
        long i = x->learnThreshold;
        
        systhread_mutex_lock (&x->learnMutex);
        
        if (pizBoundedQueueCount (x->learnQueue) >= i) {
            long err = PIZ_GOOD;
            
            pizGrowingArrayClear (x->valuesToBeLearned);
            
            while (i && !err) {
                if (!(err |= pizBoundedQueuePop (x->learnQueue))) {
                err |= pizGrowingArrayAppend (x->valuesToBeLearned, pizBoundedQueuePoppedValue (x->learnQueue));
                }
                
                i --;
            }
            
            x->learnCycle = PIZ_FACTOR_ORACLE;
            x->learnThreshold = SIZE_LEARN_MIN + ((SIZE_LEARN_RANGE + 1) * (rand ( ) / (RAND_MAX + 1.0)));
        }
            
        systhread_mutex_unlock (&x->learnMutex);
    } else {
        long k = pizGrowingArrayCount (x->valuesToBeLearned);
        long *values = pizGrowingArrayPtr (x->valuesToBeLearned);
        
        switch (x->learnCycle) {
            
            systhread_mutex_lock (&x->algorithmsMutex);
            
            case PIZ_FACTOR_ORACLE  :   pizFactorOracleAdd (x->factorOracle, k, values);
                                        x->learnCycle = PIZ_GALOIS_LATTICE; break;
            case PIZ_GALOIS_LATTICE :   pizGaloisLatticeAdd (x->galoisLattice, k, values);
                                        x->learnCycle = PIZ_FINITE_STATE; break;
            case PIZ_FINITE_STATE   :   pizFiniteStateAdd (x->finiteState, k, values);
                                        x->learnCycle = PIZ_ALGORITHM_NONE; break;
                                        
            systhread_mutex_unlock (&x->algorithmsMutex);                            
        }
    }
    
    clock_fdelay (x->learnClock, CLOCK_LEARN_INTERVAL + CLOCK_RANDOMIZE * (rand ( ) / (RAND_MAX + 1.0)));
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_anything (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    char            alloc;
    long            size = 0;
    t_atom          *atoms = NULL;
    
    if (atom_alloc_array ((argc + 1), &size, &atoms, &alloc) == MAX_ERR_NONE) {
        long i;
        
        atom_setsym (atoms, s);
        
        for (i = 0; i < argc; i++) {
            switch (atom_gettype (argv + i)) {
                case A_SYM  : atom_setsym (atoms + 1 + i, atom_getsym (argv + i)); break;
                case A_LONG : atom_setlong (atoms + 1 + i, atom_getlong (argv + i)); break;
                default     : atom_setfloat (atoms + 1 + i, 0.); break;
            }
        }
        
        tralala_handleMessages (x, tll_sym_live, argc + 1, atoms);
        
        sysmem_freeptr (atoms);
    }
}
    
void tralala_handleMessages (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && argv && atom_gettype (argv) == A_SYM) {
        bool        draw = false;
        PIZSequence *sequence = NULL;
        
        if (s == tll_sym_live) {
                sequence = x->live; draw = LIVE;
            }
        else if (s == tll_sym_user) { 
                sequence = x->user; draw = USER;
            }
        else if (s == tll_sym_listen) {
                sequence = x->listen; draw = LISTEN;
            }
        
        if (sequence && (atom_gettype (argv) == A_SYM)) {
            if (atom_getsym (argv) == tll_sym_clear) {
                    draw &= tralala_sequenceClear (x, sequence);
                }
            else if (atom_getsym (argv) == tll_sym_clean) {
                    draw &= tralala_sequenceClean (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_note) {
                    draw &= tralala_sequenceNote (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_zone) {
                    draw &= tralala_sequenceZone (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_dump) {
                    draw &= tralala_sequenceDump (x, sequence, argc - 1, argv + 1); 
                }
            else if (atom_getsym (argv) == tll_sym_rotate) {
                    draw &= tralala_sequenceRotate (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_scramble) {
                    draw &= tralala_sequenceScramble (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_sort) {
                    draw &= tralala_sequenceSort (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_change) {
                    draw &= tralala_sequenceChange (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_set) {
                    draw &= tralala_sequenceSet (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_november) {
                    draw &= tralala_sequenceNovember (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_juliet) {
                    draw &= tralala_sequenceJuliet (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_cycle) {
                    draw &= tralala_sequenceCycle (x, sequence, argc - 1, argv + 1);
                }
            else if (atom_getsym (argv) == tll_sym_kill) {
                    draw &= tralala_sequenceKill (x, sequence);
                }
            else if (atom_getsym (argv) == tll_sym_zoulou) {
                    draw &= tralala_sequenceZoulou (x, sequence);
                }
            else if (atom_getsym (argv) == tll_sym_romeo) {
                    draw &= tralala_sequenceRomeo (x, sequence);
                }
            else if (atom_getsym (argv) == tll_sym_uniform) {
                    draw &= tralala_sequenceUniform (x, sequence);
                }
            
            if (draw) {
                tralala_willChange (x);
                    
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
                    
                if (USER) {
                    DIRTYSLOTS 
                    DIRTYPATTR
                }
            }   
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

bool tralala_sequenceClear (t_tralala *x, PIZSequence *sequence)
{
    bool draw = (pizSequenceCount (sequence) != 0);
    
    pizSequenceClear (sequence);
    
    return draw;
}


bool tralala_sequenceClean (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long value = 1;
    
    if (argc && atom_gettype (argv) == A_LONG) {
            value = atom_getlong (argv);
        }
        
    draw = pizSequenceClean (sequence, value);
    
    return draw;
}

bool tralala_sequenceNote (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc == 5) {
        long i; 
        long err = PIZ_ERROR;
        
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            err = PIZ_GOOD;
            
            for (i = 0; i < 5; i++) {
                if (!(err |= (atom_gettype (argv + i) != A_LONG))){
                    switch (i) {
                        case 0 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                    / TIME_TICKS_PER_STEP)); break;
                        case 1 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                        case 2 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                        case 3 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                    / TIME_TICKS_PER_STEP)); break;
                        case 4 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;          
                    }
                }
            }
        }
        
        if (!err) {
            long mode = PIZ_ADD_FLAG_SNAP;
            
            if (sequence == x->user) {
                pizGrowingArrayAppend (tempArray, true);
                pizGrowingArrayAppend (tempArray, false);
            } else {
                pizGrowingArrayAppend (tempArray, false);
                pizGrowingArrayAppend (tempArray, false);
            }
            
            if (sequence == x->live) {
                mode = PIZ_ADD_FLAG_PATTERN;
            } else if (sequence == x->listen) {
                mode = PIZ_ADD_FLAG_AMBITUS;
            }
            
            draw = !(pizSequenceAddNotesWithArray (sequence, tempArray, mode));
        }   
        
        ARRAY_RELEASE (tempArray);
    }
    
    return draw;
}

bool tralala_sequenceZone (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc == PIZ_DATA_ZONE_SIZE) {
        long i; 
        long err = PIZ_ERROR;
        
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            err = PIZ_GOOD;
            
            for (i = 0; i < PIZ_DATA_ZONE_SIZE; i++) {
                if (!(err |= (atom_gettype (argv + i) != A_LONG))){
                    switch (i) {
                            case 0 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                        / TIME_TICKS_PER_STEP)); break; 
                            case 1 : pizGrowingArrayAppend (tempArray, (long)(atom_getlong (argv + i) 
                                        / TIME_TICKS_PER_STEP)); break;
                            case 2 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                            case 3 : pizGrowingArrayAppend (tempArray, atom_getlong (argv + i)); break;
                        }
                    }
            }
        }
        
        if (!err) {
            if (sequence == x->user) {
                pizSequenceSetZoneWithArray (sequence, tempArray);
                draw = true;
            } else if (sequence == x->live) {
                pizSequenceSetZoneWithArray (sequence, tempArray);
                pizSequenceSetZoneWithArray (x->listen, tempArray);
                draw = true;
            }
        }
        
        ARRAY_RELEASE (tempArray);
    }
    
    return draw;
}   

bool tralala_sequenceDump (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    if ((argc == 1) && (atom_gettype (argv) == A_SYM)) {
        if (atom_getsym (argv) == tll_sym_notes) {   
            long        i, count;
            PIZError    err = PIZ_GOOD;
            
            ARRAY_GET (tempArray);

            if (tempArray) {
                err |= pizSequenceNotesToArray (sequence, tempArray, tempArray);
            } else {
                err |= PIZ_MEMORY;
            }
            
            if (!err) {
                count = pizGrowingArrayCount (tempArray) / PIZ_DATA_NOTE_SIZE;
                
                outlet_anything  (x->rightOutlet, tll_sym_start, 0, NULL);
                
                for (i = 0; i < count; i++) {
                    long position = (pizGrowingArrayValueAtIndex (tempArray, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_POSITION)) * TIME_TICKS_PER_STEP;
                    long pitch    = pizGrowingArrayValueAtIndex (tempArray, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_PITCH);
                    long velocity = pizGrowingArrayValueAtIndex (tempArray, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_VELOCITY);
                    long duration = (pizGrowingArrayValueAtIndex (tempArray, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_DURATION)) * TIME_TICKS_PER_STEP;
                    long channel  = pizGrowingArrayValueAtIndex (tempArray, (PIZ_DATA_NOTE_SIZE * i) 
                                    + PIZ_DATA_CHANNEL);
                        
                    atom_setlong (x->dumpedNote + 0, position);
                    atom_setlong (x->dumpedNote + 1, pitch);
                    atom_setlong (x->dumpedNote + 2, velocity);
                    atom_setlong (x->dumpedNote + 3, duration);
                    atom_setlong (x->dumpedNote + 4, channel);
                    
                    outlet_list  (x->middleLeftOutlet, NULL, 5, x->dumpedNote);
                }
                
                outlet_anything  (x->rightOutlet, tll_sym_end, 0, NULL);
            }
            
            ARRAY_RELEASE (tempArray);
        } else if (atom_getsym (argv) == tll_sym_zone && (sequence != x->listen)) {   
            t_atom      zone[4];
            long        start, end, down, up;
            PIZError    err = PIZ_GOOD;
            
            ARRAY_GET (tempArray);

            if (tempArray) {
                pizSequenceZoneToArray (sequence, tempArray);
            } else {
                err |= PIZ_MEMORY;
            }
            
            if (!err) {
                start   = pizGrowingArrayValueAtIndex (tempArray, PIZ_DATA_START) * TIME_TICKS_PER_STEP;
                end     = pizGrowingArrayValueAtIndex (tempArray, PIZ_DATA_END) * TIME_TICKS_PER_STEP;
                down    = pizGrowingArrayValueAtIndex (tempArray, PIZ_DATA_DOWN);
                up      = pizGrowingArrayValueAtIndex (tempArray, PIZ_DATA_UP);

                atom_setlong (zone + 0, start);
                atom_setlong (zone + 1, end);
                atom_setlong (zone + 2, down);
                atom_setlong (zone + 3, up);
                
                outlet_anything  (x->rightOutlet, tll_sym_zone, 4, zone);
            }
            
            ARRAY_RELEASE (tempArray);
        } else if ((atom_getsym (argv) == tll_sym_count)) {
            t_atom  a;
            long    count = pizSequenceCount (sequence);
            
            atom_setlong (&a, count);
            
            outlet_anything  (x->rightOutlet, tll_sym_count, 1, &a);
        }
    }

    return false;
}

bool tralala_sequenceRotate (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    long        shift = 1;
    PIZSelector selector = PIZ_PITCH;
    bool        draw = false;
    
    if (argc && argv) {
        long i;
            
        for (i = 0; i < argc; i++) {
            if (atom_gettype (argv + i) == A_LONG) {
                    shift = atom_getlong (argv + i);
            } else if (atom_gettype (argv + i) == A_SYM) {
                if (atom_getsym (argv + i) == tll_sym_velocity) {
                    selector = PIZ_VELOCITY;
                } else if (atom_getsym (argv + i) == tll_sym_duration) {
                    selector = PIZ_DURATION;
                } else if (atom_getsym (argv + i) == tll_sym_channel) {
                    selector = PIZ_CHANNEL;
                }
            }
        }
    }
    
    draw = pizSequenceRotate (sequence, selector, -(shift));
    
    return draw;
}

bool tralala_sequenceScramble (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool        draw = false;
    PIZSelector selector = PIZ_PITCH;
    
    if (argc && argv) {
        if (atom_gettype (argv) == A_SYM) {
            if (atom_getsym (argv) == tll_sym_velocity) {
                selector = PIZ_VELOCITY;
            } else if (atom_getsym (argv) == tll_sym_duration) {
                selector = PIZ_DURATION;
            } else if (atom_getsym (argv) == tll_sym_channel) {
                selector = PIZ_CHANNEL;
            }
        }
    }
    
    draw = pizSequenceScramble (sequence, selector);
    
    return draw;
}

bool tralala_sequenceSort (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    long        i;
    bool        draw = false;
    bool        mode = UP;
    PIZSelector selector = PIZ_PITCH;
    
    if (argc && argv) {
        for (i = 0; i < argc; i++) {
            if (atom_gettype (argv + i) == A_SYM) {
                if (atom_getsym (argv + i) == tll_sym_down) {
                    mode = DOWN;
                }
                if (atom_getsym (argv + i) == tll_sym_velocity) {
                    selector = PIZ_VELOCITY;
                } else if (atom_getsym (argv + i) == tll_sym_duration) {
                    selector = PIZ_DURATION;
                } else if (atom_getsym (argv + i) == tll_sym_channel) {
                    selector = PIZ_CHANNEL;
                }
            }
        }
    }
    
    draw = pizSequenceSort (sequence, selector, mode);
                    
    return draw;
}

bool tralala_sequenceChange (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if ((argc == 2) && (atom_gettype (argv) == A_SYM) && (atom_gettype (argv + 1) == A_LONG)) {
        long value = atom_getlong (argv + 1);

        if (atom_getsym (argv) == tll_sym_pitch) {
            draw = pizSequenceChange (sequence, PIZ_PITCH, value);
        } else if (atom_getsym (argv) == tll_sym_velocity) {
            draw = pizSequenceChange (sequence, PIZ_VELOCITY, value);
        } else if (atom_getsym (argv) == tll_sym_duration) {
            draw = pizSequenceChange (sequence, PIZ_DURATION, (long)(value / TIME_TICKS_PER_STEP));
        } else if (atom_getsym (argv) == tll_sym_channel) {
            draw = pizSequenceChange (sequence, PIZ_CHANNEL, value);
        }
    } else if ((argc == 3) && 
        (atom_gettype (argv) == A_SYM) && 
        (atom_gettype (argv + 1) == A_LONG) &&
        (atom_gettype (argv + 2) == A_LONG)) {
        
        long minValue = atom_getlong (argv + 1);
        long maxValue = atom_getlong (argv + 2);

        if (atom_getsym (argv) == tll_sym_pitch) {
            draw = pizSequenceRandom (sequence, PIZ_PITCH, minValue, maxValue);
        } else if (atom_getsym (argv) == tll_sym_velocity) {
            draw = pizSequenceRandom (sequence, PIZ_VELOCITY, minValue, maxValue);
        } else if (atom_getsym (argv) == tll_sym_duration) {
            draw = pizSequenceRandom (sequence, PIZ_DURATION, (long)(minValue / TIME_TICKS_PER_STEP),
                (long)(maxValue / TIME_TICKS_PER_STEP));
        } else if (atom_getsym (argv) == tll_sym_channel) {
            draw = pizSequenceRandom (sequence, PIZ_CHANNEL, minValue, maxValue);
        }
    }
    
    return draw;
}

bool tralala_sequenceSet (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if ((argc == 2) && (atom_gettype (argv) == A_SYM) && (atom_gettype (argv + 1) == A_LONG)) {   
        long value = atom_getlong (argv + 1);

        if (atom_getsym (argv) == tll_sym_pitch) {
            draw = pizSequenceSet (sequence, PIZ_PITCH, value);
        } else if (atom_getsym (argv) == tll_sym_velocity) {
            draw = pizSequenceSet (sequence, PIZ_VELOCITY, value);
        } else if (atom_getsym (argv) == tll_sym_duration) {
            draw = pizSequenceSet (sequence, PIZ_DURATION, (long)(value / TIME_TICKS_PER_STEP));
        } else if (atom_getsym (argv) == tll_sym_channel) {
            draw = pizSequenceSet (sequence, PIZ_CHANNEL, value);
        }
    }
    
    return draw;
}

bool tralala_sequenceNovember (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long iterate = 1;
    
    if (argc && (atom_gettype (argv) == A_LONG)) {
        iterate = CLAMP (atom_getlong (argv), 1, SIZE_NOVEMBER_MAX);
    }
        
    draw = pizSequenceCellularAutomata (sequence, iterate);

    return draw;
}

bool tralala_sequenceJuliet (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    long division = 0;
    long iterate = 1;
    
    if (argc) {
        long i;
        
        for (i = 0; i < argc; i++) {
            if (atom_gettype (argv + i) == A_LONG) {
                iterate = CLAMP (atom_getlong (argv + i), 1, SIZE_JULIET_MAX);
            } else if (atom_gettype (argv + i) == A_SYM) {
                if (atom_getsym (argv + i) == tll_sym_duple) {
                    division = 2;
                } else if (atom_getsym (argv + i) == tll_sym_triple) {
                    division = 3;
                }
            }
        }
    }
        
    draw = pizSequenceGenerator (sequence, iterate, division);

    return draw;
}

bool tralala_sequenceCycle (t_tralala *x, PIZSequence *sequence, long argc, t_atom *argv)
{
    bool draw = false;
    
    if (argc) {
        long i;
        
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            for (i = 0; i < argc; i++) {
                if (atom_gettype (argv + i) == A_LONG) {
                    pizGrowingArrayAppend (tempArray, atom_getlong (argv + i));
                }
            }
        
            draw = pizSequenceCycle (sequence, x->key, tempArray);
            
            ARRAY_RELEASE (tempArray);
        }
    }

    return draw;
}

bool tralala_sequenceKill (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    draw = pizSequenceKillNotes (sequence);
    
    return draw;
}

bool tralala_sequenceZoulou (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmsMutex);

    draw = pizSequenceProceedAlgorithm (sequence, PIZ_FACTOR_ORACLE, (void *)x->factorOracle);
    
    systhread_mutex_unlock (&x->algorithmsMutex);
    
    return draw; 
}

bool tralala_sequenceRomeo (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmsMutex);
    
    draw = pizSequenceProceedAlgorithm (sequence, PIZ_GALOIS_LATTICE, (void *)x->galoisLattice);
    
    systhread_mutex_unlock (&x->algorithmsMutex);
    
    return draw;
}

bool tralala_sequenceUniform (t_tralala *x, PIZSequence *sequence)
{
    bool draw = false;
    
    systhread_mutex_lock (&x->algorithmsMutex);
    
    draw = pizSequenceProceedAlgorithm (sequence, PIZ_FINITE_STATE, (void *)x->finiteState);
    
    systhread_mutex_unlock (&x->algorithmsMutex);

    return draw;
}   

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_slot (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
        if (argc && argv) {
            if (atom_gettype (argv) == A_SYM) {
                if (atom_getsym (argv) == tll_sym_store) {
                    tralala_slotStore (x);
                } else if (atom_getsym (argv) == tll_sym_new) {
                    tralala_slotNew (x);
                } else if (atom_getsym (argv) == tll_sym_newCopy) {
                    tralala_slotNewCopy (x);
                } else if (atom_getsym (argv) == tll_sym_recall) {
                    long n = x->slotIndex;
                        
                    if ((argc == 2) && atom_gettype (argv + 1) == A_LONG) { 
                        n = atom_getlong (argv + 1);
                    }

                    tralala_slotRecall (x, n);
                } else if (atom_getsym (argv) == tll_sym_remove) {
                    long n = x->slotIndex;
                        
                    if ((argc == 2) && atom_gettype (argv + 1) == A_LONG) { 
                        n = atom_getlong (argv + 1);
                    }

                    tralala_slotRemove (x, n);
                } else if (atom_getsym (argv) == tll_sym_swap) {
                    if ((argc == 3) && atom_gettype (argv + 1) == A_LONG && atom_gettype (argv + 2) == A_LONG) {
                        long m = atom_getlong (argv + 1);
                        long n = atom_getlong (argv + 2);
                        
                        tralala_slotSwap (x, m, n);
                    }   
                } else if (atom_getsym (argv) == tll_sym_copy) {
                    if ((argc == 3) && atom_gettype (argv + 1) == A_LONG && atom_gettype (argv + 2) == A_LONG) {
                        long m = atom_getlong (argv + 1);
                        long n = atom_getlong (argv + 2);
                        
                        tralala_slotCopy (x, m, n);
                    }
                } else if (atom_getsym (argv) == tll_sym_next) {
                    tralala_slotNext (x);
                } else if (atom_getsym (argv) == tll_sym_previous) {
                    tralala_slotPrevious (x);
                }
            }
        }       
    }

    ATOMIC_DECREMENT (&x->popupLock);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_slotNew (t_tralala *x)
{
    PIZGrowingArray *newSlot = NULL;
    
    if (newSlot = pizGrowingArrayNew (SIZE_GROWING_ARRAY)) {
        pizLinklistAppend (x->slots, newSlot);
        tralala_slotRecall (x, pizLinklistCount (x->slots) - 1);
            
        DIRTYSLOTS
    }
}

void tralala_slotNewCopy (t_tralala *x)
{
    PIZGrowingArray *newSlot = NULL;
    
    if (newSlot = pizGrowingArrayNew (SIZE_GROWING_ARRAY)) {
        PIZGrowingArray *slot = NULL;
        PIZError        err = PIZ_GOOD;
        
        tralala_slotStore (x);
        err |= pizLinklistPtrAtIndex (x->slots, x->slotIndex, (void **)&slot);
        
        if (!err) {
            pizGrowingArrayCopy (newSlot, slot);
        }
    
        pizLinklistAppend (x->slots, newSlot);
        tralala_slotRecall (x, pizLinklistCount (x->slots) - 1);
        
        DIRTYSLOTS
    }
}

void tralala_slotRecall (t_tralala *x, long n)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, n, (void **)&slot)) {
        tralala_willChange (x);
        
        if (x->slotIndex != n) {
            tralala_slotStore (x);
        }
        
        x->slotIndex = n;
        
        pizSequenceDecodeWithArray (x->user, slot);
        
        DIRTYPATTR
        
        DIRTYLAYER_SET (DIRTY_GRID);
    }
    
    DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_ZONE | DIRTY_CHANGE);
}

void tralala_slotStore (t_tralala *x)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, x->slotIndex, (void **)&slot)) {
        pizGrowingArrayClear (slot);
        pizSequenceEncodeToArray (x->user, slot);
    }
}

void tralala_slotRemove (t_tralala *x, long n)
{
    PIZError        err = PIZ_GOOD;
    PIZGrowingArray *slot = NULL;
    
    if (!(err |= pizLinklistPtrAtIndex (x->slots, n, (void **)&slot))) {
        err |= pizLinklistRemoveByPtr (x->slots, slot);
            
        DIRTYSLOTS
    }
    
    if (!err) {
        if (!pizLinklistCount (x->slots)) {
            tralala_slotNew (x);
        } else {
            if (x->slotIndex > n) {
                x->slotIndex --;
            } else if (x->slotIndex == n) {
                if (x->slotIndex == pizLinklistCount (x->slots)) {
                        x->slotIndex --;
                    }
                    
                tralala_slotRecall (x, x->slotIndex);
            }   
        }
    }
}

void tralala_slotSwap (t_tralala *x, long m, long n)
{
    PIZError err = PIZ_GOOD;
    
    if ((x->slotIndex == m) || (x->slotIndex == n)) {
        tralala_slotStore (x);
    }
    
    err |= pizLinklistSwapByIndexes (x->slots, m, n);
    
    if (!err)  {
        if (x->slotIndex == m) {
            tralala_slotRecall (x, x->slotIndex);
        } else if (x->slotIndex == n) {
            tralala_slotRecall (x, x->slotIndex);
        }
        
        DIRTYSLOTS
    }
}

void tralala_slotCopy (t_tralala *x, long m, long n)
{
    PIZGrowingArray *a = NULL;
    PIZGrowingArray *b = NULL;
    PIZError        err = PIZ_GOOD;
    
    err |= pizLinklistPtrAtIndex (x->slots, m, (void **)&a);
    err |= pizLinklistPtrAtIndex (x->slots, n, (void **)&b);
    
    if (!err) {
        err |= pizGrowingArrayCopy (a, b);
            
        DIRTYSLOTS
    }
    
    if (!err && (x->slotIndex == m)) {
        tralala_slotRecall (x, x->slotIndex);
    }
}

void tralala_slotNext (t_tralala *x)
{
    if (x->slotIndex < (pizLinklistCount (x->slots) - 1)) {
        tralala_slotStore (x);
        tralala_slotRecall (x, x->slotIndex + 1);
    }
}

void tralala_slotPrevious (t_tralala *x)
{
    if (x->slotIndex) {
        tralala_slotStore (x);
        tralala_slotRecall (x, x->slotIndex - 1);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
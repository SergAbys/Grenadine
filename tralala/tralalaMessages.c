/*
 *  tralalaMessages.c
 *
 *  Created : 13/02/12.
 *
 *  nicolas.danet@free.fr
 *
 */

/*
 *  Last modified : 06/04/12.
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
        LIVELOCK
        err |= pizSequenceNotesToArray (x->listen, tempArrayA, tempArrayA);
        LIVEUNLOCK
    } else {
        err |= PIZ_MEMORY;
    }
    
    if (!err) {
        long i, chance = x->chance;
        long count = pizGrowingArrayCount (tempArrayA);
        long *ptr = pizGrowingArrayPtr (tempArrayA);

        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            if (100 * RANDOM < chance) {
                pizGrowingArrayAppendPtr (tempArrayB, PIZ_DATA_NOTE_SIZE, ptr + i);
            }
        }
        
        LIVELOCK
        pizSequenceAddNotes (x->live, tempArrayB, PIZ_SEQUENCE_ADD_FLAG_PATTERN);
        LIVEUNLOCK
    }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
    
    if (LIVE) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_enter (t_tralala *x)
{
    tralala_setLiveByUser (x);
    
    if (LIVE) {
        DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_setLiveByUser (t_tralala *x)
{
    PIZError err = PIZ_GOOD;
    
    ARRAY_GET (tempArrayA);
    ARRAY_GET (tempArrayB);
    
    if (tempArrayA && tempArrayB)  {
        USERLOCK
        err |= pizSequenceNotesToArray (x->user, tempArrayA, tempArrayA);
        pizSequenceZoneToArray (x->user, tempArrayB);
        USERUNLOCK
    } else {
        err |= PIZ_MEMORY;
    }
    
    if (!err) {
        PIZFlags addFlags = PIZ_SEQUENCE_ADD_FLAG_PATTERN | PIZ_SEQUENCE_ADD_FLAG_CLEAR;
        
        LIVELOCK
        pizSequenceAddNotes (x->live, tempArrayA, addFlags);
        pizSequenceSetZone  (x->live, tempArrayB);
        pizSequenceSetZone  (x->listen, tempArrayB);
        LIVEUNLOCK
    }
    
    ARRAY_RELEASE (tempArrayA);
    ARRAY_RELEASE (tempArrayB);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_int (t_tralala *x, long n)
{
    LEARNLOCK
        
    pizBoundedQueueAppend (x->learnQueue, CLAMP (n, 0, PIZ_MAGIC_PITCH));
        
    LEARNUNLOCK
}

void tralala_list (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && argv && (atom_gettype (argv) == A_LONG)) {
        tralala_int (x, atom_getlong (argv));
    }
}

void tralala_mute (t_tralala *x, long n)
{   
    METHODSLOCK
    
    if (n) {
        x->flags |= FLAG_IS_MUTED;
    } else {
        x->flags &= ~FLAG_IS_MUTED;
    }
    
    METHODSUNLOCK
}

void tralala_forget (t_tralala *x)
{
    ALGORITHMSLOCK
    
    pizFactorOracleClear  (x->factorOracle);
    pizGaloisLatticeClear (x->galoisLattice);
    pizFiniteStateClear   (x->finiteState);
    
    ALGORITHMSUNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_learnTask (t_tralala *x)
{
    if (x->learnCycle == PIZ_ALGORITHM_FLAG_NONE) {
        long i = x->learnThreshold;
        
        LEARNLOCK
        
        if (pizBoundedQueueCount (x->learnQueue) >= i) {
            long err = PIZ_GOOD;
            
            pizGrowingArrayClear (x->valuesToBeLearned);
            
            while (i && !err) {
                if (!(err |= pizBoundedQueuePop (x->learnQueue))) {
                err |= pizGrowingArrayAppend (x->valuesToBeLearned, pizBoundedQueuePoppedValue (x->learnQueue));
                }
                i --;
            }
            
            x->learnCycle       = PIZ_ALGORITHM_FLAG_FACTOR_ORACLE;
            x->learnThreshold   = SIZE_LEARN_MIN + (SIZE_LEARN_RANGE + 1) * RANDOM;
        }
            
        LEARNUNLOCK
        
    } else {
        long k = pizGrowingArrayCount (x->valuesToBeLearned);
        long *values = pizGrowingArrayPtr (x->valuesToBeLearned);
        
        ALGORITHMSLOCK
        
        switch (x->learnCycle) {
            case PIZ_ALGORITHM_FLAG_FACTOR_ORACLE  : pizFactorOracleAdd (x->factorOracle, k, values);
                                                     x->learnCycle = PIZ_ALGORITHM_FLAG_GALOIS_LATTICE; break;
            case PIZ_ALGORITHM_FLAG_GALOIS_LATTICE : pizGaloisLatticeAdd (x->galoisLattice, k, values);
                                                     x->learnCycle = PIZ_ALGORITHM_FLAG_FINITE_STATE; break;
            case PIZ_ALGORITHM_FLAG_FINITE_STATE   : pizFiniteStateAdd (x->finiteState, k, values);
                                                     x->learnCycle = PIZ_ALGORITHM_FLAG_NONE; break;
        }
        
        ALGORITHMSUNLOCK
    }
    
    clock_fdelay (x->learnClock, CLOCK_LEARN_INTERVAL + CLOCK_RANDOMIZE * RANDOM);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_handle (t_tralala *x, t_symbol *s, long argc, t_atom *argv) 
{
    if (s && argc && argv && atom_gettype(argv) == A_SYM) {
        char     alloc = 0;
        long     i, ac = 0;
        t_atom   rv;
        t_atom   *av = NULL;

        t_symbol *t = atom_getsym (argv); 
        
        if ((atom_alloc_array (argc, &ac, &av, &alloc)) == MAX_ERR_NONE) {
        
            atom_setsym (av, s);
            
            for (i = 1; i < ac; i ++) {
                switch (atom_gettype (argv + i)) {
                    case A_SYM  : atom_setsym (av + i, atom_getsym (argv + i));     break;
                    case A_LONG : atom_setlong (av + i, atom_getlong (argv + i));   break;
                    default     : atom_setlong (av + i, 0);
                }
            }
            
            object_method_typed (x, t, ac, av, &rv);
            sysmem_freeptr (av);
        }
    } 
}

void tralala_anything (t_tralala *x, t_symbol *s, long argc, t_atom *argv) 
{
    ;
}

void tralala_slot (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (ATOMIC_INCREMENT (&x->popupLock) == 1) {
        t_tralalaData   data;
        long            m = x->slotIndex;
        long            n = x->slotIndex;
        
        tralala_parseArguments (x, &data, argc, argv);        
        
        if (data.count) {
            m = data.values[0];
        }
        if (data.count > 1) {
            n = data.values[1];
        }
        
        if (data.option) {
            if (data.option == OPTION_RECALL) {
                tralala_slotRecall (x, m);
            } else if (data.option == OPTION_STORE) {
                tralala_slotStore (x);
            } else if (data.option == OPTION_NEW) {
                tralala_slotNew (x);
            } else if (data.option == OPTION_NEWCOPY) {
                tralala_slotNewCopy (x);
            } else if (data.option == OPTION_REMOVE) {
                tralala_slotRemove (x, m);
            } else if (data.option == OPTION_SWAP) {
                tralala_slotSwap (x, m, n);
            } else if (data.option == OPTION_COPY) {
                tralala_slotCopy (x, m, n);
            } else if (data.option == OPTION_NEXT) {
                tralala_slotNext (x);
            } else if (data.option == OPTION_PREVIOUS) {
                tralala_slotPrevious (x);
            }   
        
            if (data.option != OPTION_RECALL) {
                DIRTYSLOTS
            } 
        }
    }

    ATOMIC_DECREMENT (&x->popupLock);
}

void tralala_parseArguments (t_tralala *x, t_tralalaData *d, long argc, t_atom *argv)
{
    d->draw      = LIVE;
    d->count     = 0;
    d->option    = OPTION_NONE;
    d->selector  = PIZ_NOTE_PITCH;
    d->sequence  = x->live;
    d->mutex     = &x->liveMutex;
        
    if (argc && argv && d) {
    //
    long i;
    
    for (i = 0; i < argc; i++) {
    //
    long t = atom_gettype (argv + i);
    
    if (t == A_SYM) {

        t_symbol *s = atom_getsym (argv + i);
        
        if (s == tll_sym_live)          { d->draw = LIVE; d->sequence = x->live; d->mutex = &x->liveMutex; }
        else if (s == tll_sym_user)     { d->draw = USER; d->sequence = x->user; d->mutex = &x->userMutex; } 
        else if (s == tll_sym_listen)   { d->draw = 0; d->sequence = x->listen; d->mutex = &x->liveMutex; } 
        else if (s == tll_sym_pitch)    { d->selector = PIZ_NOTE_PITCH; } 
        else if (s == tll_sym_velocity) { d->selector = PIZ_NOTE_VELOCITY; } 
        else if (s == tll_sym_duration) { d->selector = PIZ_NOTE_DURATION; } 
        else if (s == tll_sym_channel)  { d->selector = PIZ_NOTE_CHANNEL; }
        else if (s == tll_sym_notes)    { d->option = OPTION_NOTES; } 
        else if (s == tll_sym_zone)     { d->option = OPTION_ZONE; } 
        else if (s == tll_sym_count)    { d->option = OPTION_COUNT; } 
        else if (s == tll_sym_down)     { d->option = OPTION_DOWN; } 
        else if (s == tll_sym_duple)    { d->option = OPTION_DUPLE; } 
        else if (s == tll_sym_triple)   { d->option = OPTION_TRIPLE; }
        else if (s == tll_sym_recall )  { d->option = OPTION_RECALL; }
        else if (s == tll_sym_store )   { d->option = OPTION_STORE; }
        else if (s == tll_sym_new)      { d->option = OPTION_NEW; }
        else if (s == tll_sym_newCopy)  { d->option = OPTION_NEWCOPY; }
        else if (s == tll_sym_remove )  { d->option = OPTION_REMOVE; }
        else if (s == tll_sym_swap)     { d->option = OPTION_SWAP; }
        else if (s == tll_sym_copy)     { d->option = OPTION_COPY; }
        else if (s == tll_sym_next)     { d->option = OPTION_NEXT; }
        else if (s == tll_sym_previous) { d->option = OPTION_PREVIOUS; }

    } else if (t == A_LONG) {
        if (d->count < PIZ_MAGIC_SCALE) {
            d->values[d->count] = atom_getlong (argv + i);
            d->count ++;
        }
    }
    //
    }
    //
    }
    
    if ((d->sequence == x->user)) {
        if (USER) {
            tralala_willChange (x);
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
        }
        
        DIRTYPATTR
    } 
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
        
void tralala_sequenceClear (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{   
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    DATALOCK
    data.draw &= (pizSequenceCount (data.sequence) > 0);
    pizSequenceClear (data.sequence);
    DATAUNLOCK 
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceKill (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    DATALOCK
    data.draw &= pizSequenceKill (data.sequence);
    DATAUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceZoulou (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    ALGORITHMSLOCK
    DATALOCK
    data.draw &= pizSequenceAlgorithm (data.sequence, (void *)x->factorOracle);
    DATAUNLOCK
    ALGORITHMSUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceRomeo (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    ALGORITHMSLOCK
    DATALOCK
    data.draw &= pizSequenceAlgorithm (data.sequence, (void *)x->galoisLattice);
    DATAUNLOCK
    ALGORITHMSUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceUniform (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    ALGORITHMSLOCK
    DATALOCK
    data.draw &= pizSequenceAlgorithm (data.sequence, (void *)x->finiteState);
    DATAUNLOCK
    ALGORITHMSUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceClean (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long            value = 1;
    t_tralalaData   data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count) {
        value = data.values[0];
    }
    
    DATALOCK
    data.draw &= pizSequenceClean (data.sequence, value);
    DATAUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceNote (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count == 5) {
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            PIZFlags mode = PIZ_SEQUENCE_ADD_FLAG_SNAP; 
            
            pizGrowingArrayAppend (tempArray, (long)(data.values[0] / TIME_TICKS_PER_STEP)); 
            pizGrowingArrayAppend (tempArray, data.values[1]);
            pizGrowingArrayAppend (tempArray, data.values[2]);
            pizGrowingArrayAppend (tempArray, (long)(data.values[3] / TIME_TICKS_PER_STEP));
            pizGrowingArrayAppend (tempArray, data.values[4]);           
            
            if (data.sequence == x->user) {
                pizGrowingArrayAppend (tempArray, true);
            } else {
                pizGrowingArrayAppend (tempArray, false);
            }
            
            pizGrowingArrayAppend (tempArray, false);
            
            if (data.sequence == x->live) {
                mode = PIZ_SEQUENCE_ADD_FLAG_PATTERN;
            } else if (data.sequence == x->listen) {
                mode = PIZ_SEQUENCE_ADD_FLAG_AMBITUS;
            }
            
            DATALOCK
            data.draw &= !(pizSequenceAddNotes (data.sequence, tempArray, mode)); 
            DATAUNLOCK
        }
        
        ARRAY_RELEASE (tempArray);
        
        if (data.draw) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
    }
}

void tralala_sequenceZone (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if ((data.sequence != x->listen) && (data.count == 4)) {
        ARRAY_GET (tempArray);
        
        if (tempArray) {
            pizGrowingArrayAppend (tempArray, (long)(data.values[0] / TIME_TICKS_PER_STEP)); 
            pizGrowingArrayAppend (tempArray, (long)(data.values[1] / TIME_TICKS_PER_STEP));
            pizGrowingArrayAppend (tempArray, data.values[2]);
            pizGrowingArrayAppend (tempArray, data.values[3]);
            
            DATALOCK
            data.draw &= !(pizSequenceSetZone (data.sequence, tempArray));
            DATAUNLOCK
            
            if (data.sequence == x->live) {
                LIVELOCK
                pizSequenceSetZone (x->listen, tempArray);
                LIVEUNLOCK
            }
        }
        
        ARRAY_RELEASE (tempArray);
        
        if (data.draw) {
            DIRTYLAYER_SET (DIRTY_ZONE | DIRTY_SEQUENCE);
        }
    }
}   

void tralala_sequenceDump (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.option == OPTION_NOTES) { 
    //
    
    ARRAY_GET (tempArray);

    if (tempArray) {
        long i, count, position, pitch, velocity, duration, channel;
        
        DATALOCK
        pizSequenceNotesToArray (data.sequence, tempArray, tempArray);
        DATAUNLOCK
        
        count = pizGrowingArrayCount (tempArray);
        outlet_anything  (x->rightOutlet, tll_sym_start, 0, NULL);
        
        for (i = 0; i < count; i += PIZ_DATA_NOTE_SIZE) {
            position = pizGrowingArrayValueAtIndex (tempArray, i + PIZ_DATA_POSITION) * TIME_TICKS_PER_STEP;
            pitch    = pizGrowingArrayValueAtIndex (tempArray, i + PIZ_DATA_PITCH);
            velocity = pizGrowingArrayValueAtIndex (tempArray, i + PIZ_DATA_VELOCITY);
            duration = pizGrowingArrayValueAtIndex (tempArray, i + PIZ_DATA_DURATION) * TIME_TICKS_PER_STEP;
            channel  = pizGrowingArrayValueAtIndex (tempArray, i + PIZ_DATA_CHANNEL);
                
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
    //   
    } else if (data.option == OPTION_ZONE) {  
    // 
    ARRAY_GET (tempArray);

    if (tempArray) {
        t_atom  zone[4];
        long    start, end, down, up;
    
        DATALOCK
        pizSequenceZoneToArray (data.sequence, tempArray);
        DATAUNLOCK
    
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
    //    
    } else if (data.option == OPTION_COUNT) {
        t_atom  a;
        long    count;
        
        DATALOCK
        count = pizSequenceCount (data.sequence);
        DATAUNLOCK
                
        atom_setlong (&a, count);
        outlet_anything  (x->rightOutlet, tll_sym_count, 1, &a);
    }
}

void tralala_sequenceRotate (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long            value = -1;
    t_tralalaData   data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count) {
        value = data.values[0];
    }
    
    DATALOCK
    data.draw &= pizSequenceRotate (data.sequence, data.selector, value);
    DATAUNLOCK
        
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceScramble (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    DATALOCK
    data.draw &= pizSequenceScramble (data.sequence, data.selector);
    DATAUNLOCK
        
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceSort (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    DATALOCK
    data.draw &= pizSequenceSort (data.sequence, data.selector, (data.option == OPTION_DOWN));
    DATAUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceChange (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count == 1) {
        if (data.selector == PIZ_NOTE_DURATION) {
            data.values[0] = (long)(data.values[0] / TIME_TICKS_PER_STEP);
        }

        DATALOCK
        data.draw &= pizSequenceChange (data.sequence, data.selector, data.values[0]);
        DATAUNLOCK
        
        if (data.draw) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
        
    } else if (data.count == 2) {
        if (data.selector == PIZ_NOTE_DURATION) {
            data.values[0] = (long)(data.values[0] / TIME_TICKS_PER_STEP);
            data.values[1] = (long)(data.values[1] / TIME_TICKS_PER_STEP);
        }
        
        DATALOCK
        data.draw &= pizSequenceRandom (data.sequence, data.selector, data.values[0], data.values[1]);
        DATAUNLOCK
        
        if (data.draw) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
    }
}

void tralala_sequenceSet (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count == 1) {
        if (data.selector == PIZ_NOTE_DURATION) {
            data.values[0] = (long)(data.values[0] / TIME_TICKS_PER_STEP);
        }

        DATALOCK
        data.draw &= pizSequenceSet (data.sequence, data.selector, data.values[0]);
        DATAUNLOCK
        
        if (data.draw) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
        }
    } 
}

void tralala_sequenceNovember (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long            value = 1;
    t_tralalaData   data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count) {
        value = CLAMP (data.values[0], 1, SIZE_NOVEMBER_MAX);
    }
    
    DATALOCK
    data.draw &= pizSequenceNovember (data.sequence, value);
    DATAUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceJuliet (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long            value = 1;
    long            division = 0;
    t_tralalaData   data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count) {
        value = CLAMP (data.values[0], 1, SIZE_JULIET_MAX);
    }
    
    if (data.option == OPTION_DUPLE) {
        division = 2;
    } else if (data.option == OPTION_TRIPLE) {
        division = 3;
    }
    
    DATALOCK
    data.draw &= pizSequenceJuliet (data.sequence, value, division);
    DATAUNLOCK
    
    if (data.draw) {
        DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
    }
}

void tralala_sequenceCycle (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    t_tralalaData   data;
    tralala_parseArguments (x, &data, argc, argv);
    
    if (data.count) {
        ARRAY_GET (tempArray);
            
        if (tempArray) {
            pizGrowingArrayAppendPtr (tempArray, data.count, data.values);
            
            DATALOCK
            data.draw = pizSequenceCycle (data.sequence, x->key, tempArray);
            DATAUNLOCK
            
            if (data.draw) {
                DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_SEQUENCE);
            }
        }
        
        ARRAY_RELEASE (tempArray);
    }
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
    }
}

void tralala_slotRecall (t_tralala *x, long n)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, n, (void **)&slot)) {
        if (x->slotIndex != n) {
            tralala_slotStore (x);
            x->slotIndex = n;
        }
        
        USERLOCK
        pizSequenceDecodeWithArray (x->user, slot);
        x->cell = pizSequenceCell (x->user);
        x->grid = pizSequenceGrid (x->user);
        x->noteValue = pizSequenceNoteValue (x->user);
        USERUNLOCK
        
        DIRTYPATTR
        
        if (USER) {
            DIRTYLAYER_SET (DIRTY_NOTES | DIRTY_GRID | DIRTY_ZONE | DIRTY_SEQUENCE);
        }
    }
}

void tralala_slotStore (t_tralala *x)
{
    PIZGrowingArray *slot = NULL;
    
    if (!pizLinklistPtrAtIndex (x->slots, x->slotIndex, (void **)&slot)) {
        pizGrowingArrayClear (slot);
        
        USERLOCK
        pizSequenceEncodeToArray (x->user, slot);
        USERUNLOCK
    }
}

void tralala_slotRemove (t_tralala *x, long n)
{
    PIZError        err = PIZ_GOOD;
    PIZGrowingArray *slot = NULL;
    
    if (!(err |= pizLinklistPtrAtIndex (x->slots, n, (void **)&slot))) {
        err |= pizLinklistRemoveByPtr (x->slots, slot);
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
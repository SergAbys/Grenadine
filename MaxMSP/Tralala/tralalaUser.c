/*
 *  tralalaUser.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaUser.h"
#include "tralalaParse.h"
#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

long tralala_hitZone(t_tll *x, t_pt pt)
{
    long argc, k = 0;
    t_atom *argv = NULL;
    long position = X_TO_POSITION(pt.x);
    long pitch = Y_TO_PITCH(pt.y);
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
        if (position < atom_getlong(argv + 1)) { 
            k = TLL_SELECTED_START; 
        } else if (position > atom_getlong(argv + 2)) { 
            k = TLL_SELECTED_END;
        } else if (pitch < atom_getlong(argv + 3)) { 
            k = TLL_SELECTED_DOWN;
        } else if (pitch > atom_getlong(argv + 4)) { 
            k = TLL_SELECTED_UP;
        } else {
            k = TLL_SELECTED;
        }
    }
    
    return k;
}

t_symbol *tralala_hitNote(t_tll *x, t_pt pt)
{
    long i, n;
    t_symbol *note = NULL;
    t_symbol **keys = NULL;
    long position = X_TO_POSITION(pt.x);
    long pitch = Y_TO_PITCH(pt.y);
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    long argc;
    t_atom *argv = NULL;
    
    for (i = 0; i < n; i++) {
        if (!(dictionary_getatoms(x->current, (*(keys + i)), &argc, &argv))) { 
        //
        if ((atom_getsym(argv) == TLL_SYM_NOTE)) {
            long p = atom_getlong(argv + 2);
            long a = atom_getlong(argv + 1);
            long b = a + atom_getlong(argv + 4);
            if ((pitch == p) && (position >= a) && (position <= b)) {
                note = (*(keys + i));
                break;
            }
        }
        //
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    return note;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
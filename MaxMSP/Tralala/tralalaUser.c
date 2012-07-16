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
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TLL_KEY_A   97
#define TLL_KEY_X   120
#define TLL_KEY_C   99
#define TLL_KEY_V   118

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef ulong (*tllMethod)( ); 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL ulong     tralala_userKeyA                (t_tll *x, long m);
PIZ_LOCAL ulong     tralala_userKeyC                (t_tll *x, long m);
PIZ_LOCAL ulong     tralala_userKeyV                (t_tll *x, long m);
PIZ_LOCAL ulong     tralala_userKeyX                (t_tll *x, long m);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL void      tralala_userAddNote             (t_tll *x);
PIZ_LOCAL ulong     tralala_userSelectAll           (t_tll *x);
PIZ_LOCAL void      tralala_userSelectNoteByClick   (t_tll *x, t_symbol *s);
PIZ_LOCAL ulong     tralala_userSelectNoteByLasso   (t_tll *x);
PIZ_LOCAL bool      tralala_userIsNoteInsideLasso   (t_tll *x, t_symbol *s, double *coordinates);
PIZ_LOCAL void      tralala_userReleaseLasso        (t_tll *x);

PIZ_LOCAL long      tralala_userHitZone             (t_tll *x);
PIZ_LOCAL t_symbol  *tralala_userHitNote            (t_tll *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_key(t_tll *x, t_object *pv, long keycode, long m, long textcharacter)
{
    ulong dirty = TLL_DIRTY_NONE;
    tllMethod f = NULL;
        
    switch (keycode) {
        case TLL_KEY_A  : f = tralala_userKeyA; break;
    }
    
    TLL_LOCK
    
    dirty |= tralala_userAbort(x);
    dirty |= (*f)(x, m);
    
    TLL_UNLOCK
    
    if (dirty & TLL_DIRTY_NOTE) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    }
    if (dirty & TLL_DIRTY_ZONE) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
    }
    if (dirty & TLL_DIRTY_LASSO) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
    }
    
    jbox_redraw((t_jbox *)x);
}

void tralala_wheel(t_tll *x, t_object *view, t_pt pt, long m, double x_inc, double y_inc)
{
    long h = object_attr_getlong(x, TLL_SYM_XOFFSET) - (x_inc * 100);
    long v = object_attr_getlong(x, TLL_SYM_YOFFSET) - (y_inc * 100);
        
    object_attr_setlong(x, TLL_SYM_XOFFSET, h);
    object_attr_setlong(x, TLL_SYM_YOFFSET, v);
}

void tralala_down(t_tll *x, t_object *pv, t_pt pt, long m)
{	
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
    x->origin.x = pt.x - 1.;
    x->origin.y = pt.y - 1.;
    
    TLL_LOCK
    
    if (!(m & eShiftKey)) {
        dictionary_clear(x->status);
    }
    
    if (m & eCommandKey) {
        tralala_userAddNote(x);
        
    } else if (m & eControlKey) {
        long k;
        if (k = tralala_userHitZone(x)) {
            dictionary_appendlong(x->status, TLL_SYM_ZONE, k);
        }
    
    } else {
        t_symbol *s = NULL;
        if (s = tralala_userHitNote(x)) {
            tralala_userSelectNoteByClick(x, s);
        } else {
            x->flags |= TLL_FLAG_LASSO;
        }
    }
    
    TLL_UNLOCK
        
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE); 
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    jbox_redraw((t_jbox *)x);  
}

void tralala_move(t_tll *x, t_object *pv, t_pt pt, long m)
{
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
    
    jbox_redraw((t_jbox *)x);
}

void tralala_drag(t_tll *x, t_object *pv, t_pt pt, long m)
{
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
     
    if (m & eShiftKey) {
        x->flags |= TLL_FLAG_SHIFT;
    } else {
        x->flags &= ~TLL_FLAG_SHIFT;
    }
    
    if (x->flags & TLL_FLAG_LASSO) {
    //
    ulong dirty;
    
    TLL_LOCK
    dirty = tralala_userSelectNoteByLasso(x);
    TLL_UNLOCK
    
    if (dirty) {
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    }

    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
    jbox_redraw((t_jbox *)x);
    //
    }
}

void tralala_up(t_tll *x, t_object *pv, t_pt pt, long m)
{
    if (x->flags & TLL_FLAG_LASSO) {
        TLL_LOCK
        tralala_userReleaseLasso(x);
        TLL_UNLOCK
        
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
        jbox_redraw((t_jbox *)x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

ulong tralala_userAbort(t_tll *x)
{
    if (x->flags & TLL_FLAG_LASSO) {
        tralala_userReleaseLasso(x);
        return TLL_DIRTY_LASSO;
    } 
    
    return TLL_DIRTY_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

ulong tralala_userKeyA(t_tll *x, long m)
{
    ulong dirty = TLL_DIRTY_NONE;
    
    if (m & eCommandKey) {
        dirty |= tralala_userSelectAll(x);
        
        if (m & eShiftKey) {
            dictionary_appendlong(x->status, TLL_SYM_ZONE, TLL_SELECTED);
            dirty |= TLL_DIRTY_ZONE;
        }
    }
    
    return dirty;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_userAddNote(t_tll *x)
{
    t_atom a[2];
    
    atom_setlong(a, TLL_X_TO_POSITION(x->cursor.x));
    atom_setlong(a + 1, TLL_Y_TO_PITCH(x->cursor.y));
    
    tralala_parseMessage(x, TLL_SYM_NOTE, 2, a);
}

ulong tralala_userSelectAll(t_tll *x)
{
    long i, n;
    t_symbol **keys = NULL;
    ulong dirty = TLL_DIRTY_NONE;
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
        for (i = 0; i < n; i++) {
        //
        long argc;
        t_atom *argv = NULL;
        t_symbol *key = (*(keys + i));
        
        if (!(dictionary_getatoms(x->current, key, &argc, &argv)) && ((atom_getsym(argv) == TLL_SYM_NOTE))) { 
            if (!(dictionary_hasentry(x->status, key))) {
                dictionary_appendlong(x->status, key, TLL_SELECTED); 
                dirty |= TLL_DIRTY_NOTE;
            }
        }
        //
        }
        
        dictionary_freekeys(x->current, n, keys);
    }
    
    return dirty;
}

void tralala_userSelectNoteByClick(t_tll *x, t_symbol *s)
{
    if (dictionary_hasentry(x->status, s)) {
        t_symbol *last = NULL;
        dictionary_getsym(x->status, TLL_SYM_LAST, &last);
        if (s == last) { dictionary_deleteentry(x->status, TLL_SYM_LAST); }
        dictionary_deleteentry(x->status, s);
        
    } else {
        dictionary_appendsym(x->status, TLL_SYM_LAST, s);
        dictionary_appendlong(x->status, s, TLL_SELECTED);
    }
}

void tralala_userReleaseLasso(t_tll *x)
{
    long i, n;
    t_symbol **keys = NULL;
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    long argc;
    t_atom *argv = NULL;
    
    for (i = 0; i < n; i++) {
    //
    long status = 0;
    t_symbol *key = (*(keys + i));
    
    if (!(dictionary_getatoms(x->current, key, &argc, &argv))) { 
        if ((atom_getsym(argv) == TLL_SYM_NOTE) && (!dictionary_getlong(x->status, key, &status))) {
            if (status == TLL_UNSELECTED) {
                dictionary_deleteentry(x->status, key);
            } else {
                dictionary_appendlong(x->status, key, TLL_SELECTED);
            }
        }
    }
    //
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    x->flags &= ~TLL_FLAG_LASSO;
}

bool tralala_userIsNoteInsideLasso(t_tll *x, t_symbol *s, double *c)
{
    bool k = false;
    long argc;
    t_atom *argv = NULL;
        
    if (!(dictionary_getatoms(x->current, s, &argc, &argv))) {
        double a = TLL_POSITION_TO_X(atom_getlong(argv + 1));
        double b = TLL_PITCH_TO_Y_UP(atom_getlong(argv + 2));
        double u = TLL_POSITION_TO_X(atom_getlong(argv + 1) + atom_getlong(argv + 4));
        double v = TLL_PITCH_TO_Y_DOWN(atom_getlong(argv + 2));
        
        k  = ((a > c[0]) && (a < c[2])) || ((u > c[0]) && (u < c[2]));
        k &= ((b > c[1]) && (b < c[3])) || ((v > c[1]) && (v < c[3]));
    }
    
    return k;
}

ulong tralala_userSelectNoteByLasso(t_tll *x)
{
    long i, n;
    ulong dirty = TLL_DIRTY_NONE;
    t_symbol **keys = NULL;
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    long argc;
    t_atom *argv = NULL;
    
    double c[ ] = { TLL_X_OFFSET(MIN(x->origin.x, x->cursor.x)), TLL_Y_OFFSET(MIN(x->origin.y, x->cursor.y)),
                    TLL_X_OFFSET(MAX(x->origin.x, x->cursor.x)), TLL_Y_OFFSET(MAX(x->origin.y, x->cursor.y)) };
    
    for (i = 0; i < n; i++) {
    //
    t_symbol *key = (*(keys + i));
    
    if (!(dictionary_getatoms(x->current, key, &argc, &argv)) && ((atom_getsym(argv) == TLL_SYM_NOTE))) { 
    //
    long status = 0;
    
    if (tralala_userIsNoteInsideLasso(x, key, c)) {

        if (!(dictionary_hasentry(x->status, key))) {
            dictionary_appendlong(x->status, key, TLL_SELECTED_LASSO);
            dirty |= TLL_DIRTY_NOTE;
            
        } else if (x->flags & TLL_FLAG_SHIFT) {
            if (!(dictionary_getlong(x->status, key, &status)) && (status == TLL_SELECTED)) {
                dictionary_appendlong(x->status, key, TLL_UNSELECTED);
                dirty |= TLL_DIRTY_NOTE;
            }
        }
        
    } else if (!(dictionary_getlong(x->status, key, &status))) {
    
        if (status == TLL_SELECTED_LASSO) {
            dictionary_deleteentry(x->status, key);
            dirty |= TLL_DIRTY_NOTE;
            
        } else if ((x->flags & TLL_FLAG_SHIFT) && (status == TLL_UNSELECTED)) {
            dictionary_appendlong(x->status, key, TLL_SELECTED);
            dirty |= TLL_DIRTY_NOTE;
        }
    }
    //
    }
    //
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
        
    return dirty;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long tralala_userHitZone(t_tll *x)
{
    long argc, k = 0;
    t_atom *argv = NULL;
    long position = TLL_X_TO_POSITION(x->cursor.x);
    long pitch = TLL_Y_TO_PITCH(x->cursor.y);
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
    //
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
    //
    }
    
    return k;
}

t_symbol *tralala_userHitNote(t_tll *x)
{
    long i, n;
    t_symbol *note = NULL;
    t_symbol **keys = NULL;
    long position = TLL_X_TO_POSITION(x->cursor.x - 1.);
    long pitch = TLL_Y_TO_PITCH(x->cursor.y - 1.);
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    long argc;
    t_atom *argv = NULL;
    
    for (i = 0; i < n; i++) {
        t_symbol *key = (*(keys + i));
        if (!(dictionary_getatoms(x->current, key, &argc, &argv))) { 
            if ((atom_getsym(argv) == TLL_SYM_NOTE)) {
                long p = atom_getlong(argv + 2);
                long a = atom_getlong(argv + 1);
                long b = a + atom_getlong(argv + 4);
                if ((pitch == p) && (position >= a) && (position <= b)) {
                    note = key;
                    break;
                }
            }
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    return note;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
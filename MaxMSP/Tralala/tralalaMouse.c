/*
 *  tralalaMouse.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaMouse.h"
#include "tralalaParse.h"
#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_HIT_NONE    0
#define TLL_HIT_SWAP    1
#define TLL_HIT_GRAB    2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL   void  tralala_mouseUnselectZone (t_tll *x);
PIZ_LOCAL   void  tralala_mouseAddNote      (t_tll *x);
PIZ_LOCAL   void  tralala_mouseReleaseLasso (t_tll *x);
PIZ_LOCAL   void  tralala_mouseHitZone      (t_tll *x);
PIZ_LOCAL   long  tralala_mouseHitNote      (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_mouseSelectLasso  (t_tll *x);
PIZ_INLINE  bool  tralala_mouseIsInLasso    (t_tll *x, t_symbol *s, double *coordinates);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_mousedown(t_tll *x, t_object *pv, t_pt pt, long m)
{	
    long k;
    
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
    x->origin.x = pt.x - 1.;
    x->origin.y = pt.y - 1.;
    
    if (m & eCommandKey) {
        tralala_mouseAddNote(x);
        
    } else if (m & eControlKey) {
        tralala_mouseUnselectAll(x);
        tralala_mouseHitZone(x);

    } else if (!(k = tralala_mouseHitNote(x, m))) {
        if (!(m & eShiftKey)) {
            tralala_mouseUnselectAll(x);
        }
        x->flags |= TLL_FLAG_LASSO;
        
    } else {
        tralala_mouseUnselectZone(x);
        if (k == TLL_HIT_GRAB) {
            post("GRABBED");
        }
    }
            
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE); 
    jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
    jbox_redraw((t_jbox *)x);  
}

void tralala_mousemove(t_tll *x, t_object *pv, t_pt pt, long m)
{
    ;
}

void tralala_mousedrag(t_tll *x, t_object *pv, t_pt pt, long m)
{
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
    
    if (m & eShiftKey) {
        x->flags |= TLL_FLAG_SHIFT;
    }
    
    if (x->flags & TLL_FLAG_LASSO) {
        if (tralala_mouseSelectLasso(x)) {
            jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
        }

        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
        jbox_redraw((t_jbox *)x);
    }
}

void tralala_mouseup(t_tll *x, t_object *pv, t_pt pt, long m)
{
    x->flags &= ~TLL_FLAG_SHIFT;
    
    if (x->flags & TLL_FLAG_LASSO) {
        tralala_mouseReleaseLasso(x);
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
        jbox_redraw((t_jbox *)x);
    }
}

void tralala_mousewheel(t_tll *x, t_object *view, t_pt pt, long m, double x_inc, double y_inc)
{
    long h = object_attr_getlong(x, TLL_SYM_XOFFSET) - (x_inc * 100);
    long v = object_attr_getlong(x, TLL_SYM_YOFFSET) - (y_inc * 100);
        
    object_attr_setlong(x, TLL_SYM_XOFFSET, h);
    object_attr_setlong(x, TLL_SYM_YOFFSET, v);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

ulong tralala_mouseAbort(t_tll *x)
{
    if (x->flags & TLL_FLAG_LASSO) {
        tralala_mouseReleaseLasso(x);
        return TLL_DIRTY_LASSO;
    } 
    
    return TLL_DIRTY_NONE;
}

void tralala_mouseUnselectAll(t_tll *x)
{
    TLL_LOCK
    dictionary_clear(x->status);
    TLL_UNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_mouseUnselectZone(t_tll *x)
{
    TLL_LOCK
    
    if (dictionary_hasentry(x->status, TLL_SYM_ZONE)) {
        dictionary_deleteentry(x->status, TLL_SYM_ZONE);
    }
    
    TLL_UNLOCK
}

void tralala_mouseAddNote(t_tll *x)
{
    t_atom a[2];
    
    atom_setlong(a, TLL_X_TO_POSITION(x->cursor.x));
    atom_setlong(a + 1, TLL_Y_TO_PITCH(x->cursor.y));
    
    tralala_parseMessage(x, TLL_SYM_NOTE, 2, a, TLL_FLAG_LOW);
}

void tralala_mouseReleaseLasso(t_tll *x)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    
    TLL_LOCK
    
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
    
    TLL_UNLOCK
    
    x->flags &= ~TLL_FLAG_LASSO;
}

void tralala_mouseHitZone(t_tll *x)
{
    long argc, k = TLL_SELECTED;
    t_atom *argv = NULL;
    long position = TLL_X_TO_POSITION(x->cursor.x);
    long pitch = TLL_Y_TO_PITCH(x->cursor.y);
    
    TLL_LOCK
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
        if (position < atom_getlong(argv + 1)) { 
            k = TLL_SELECTED_START;
        } else if (position > atom_getlong(argv + 2)) { 
            k = TLL_SELECTED_END;
        } else if (pitch < atom_getlong(argv + 3)) { 
            k = TLL_SELECTED_DOWN;
        } else if (pitch > atom_getlong(argv + 4)) { 
            k = TLL_SELECTED_UP;
        } 
    }
    
    dictionary_appendlong(x->status, TLL_SYM_ZONE, k);
    
    TLL_UNLOCK
}

long tralala_mouseHitNote(t_tll *x, long m)
{
    long i, n = 0;
    t_symbol *s = NULL;
    t_symbol **keys = NULL;
    long position = TLL_X_TO_POSITION(x->cursor.x - 1.);
    long pitch = TLL_Y_TO_PITCH(x->cursor.y - 1.);
    long k = TLL_HIT_NONE;
    
    TLL_LOCK
    
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
                    s = key;
                    break;
                }
            }
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    if (s) {
        if (dictionary_hasentry(x->status, s) && (m & eShiftKey)) {
            t_symbol *mark = NULL;
            if(!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) && (s == mark)) {
                dictionary_deleteentry(x->status, TLL_SYM_MARK); 
            }
            
            dictionary_deleteentry(x->status, s);
            k = TLL_HIT_SWAP;
            
        } else {
            dictionary_appendsym(x->status, TLL_SYM_MARK, s);
            dictionary_appendlong(x->status, s, TLL_SELECTED);
            k = TLL_HIT_GRAB;
        }
    } 
    
    TLL_UNLOCK
    
    return k;
}

ulong tralala_mouseSelectLasso(t_tll *x)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    ulong dirty = TLL_DIRTY_NONE;
    
    TLL_LOCK
    
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
    
    if (tralala_mouseIsInLasso(x, key, c)) {

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
    
    TLL_UNLOCK
        
    return dirty;
}

PIZ_INLINE bool tralala_mouseIsInLasso(t_tll *x, t_symbol *s, double *c)
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

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
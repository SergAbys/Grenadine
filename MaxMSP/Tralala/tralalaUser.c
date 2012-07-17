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
extern t_dictionary *tll_clipboard;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TLL_KEY_A   97
#define TLL_KEY_C   99
#define TLL_KEY_V   118
#define TLL_KEY_X   120

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef ulong (*tllMethod)( ); 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL ulong tralala_userKeyAll      (t_tll *x, long m);
PIZ_LOCAL ulong tralala_userKeyCopy     (t_tll *x, long m);
PIZ_LOCAL ulong tralala_userKeyPaste    (t_tll *x, long m);
PIZ_LOCAL ulong tralala_userKeyCut      (t_tll *x, long m);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_LOCAL void  tralala_userAddNote             (t_tll *x);
PIZ_LOCAL void  tralala_userSelectZone          (t_tll *x);
PIZ_LOCAL void  tralala_userSelectAll           (t_tll *x);
PIZ_LOCAL void  tralala_userUnselectAll         (t_tll *x);
PIZ_LOCAL void  tralala_userReleaseLasso        (t_tll *x);
PIZ_LOCAL void  tralala_userHitZone             (t_tll *x);
PIZ_LOCAL bool  tralala_userHitNote             (t_tll *x);
PIZ_LOCAL ulong tralala_userSelectNoteByLasso   (t_tll *x);
PIZ_LOCAL bool  tralala_userIsNoteInsideLasso   (t_tll *x, t_symbol *s, double *coordinates);
PIZ_LOCAL void  tralala_userCopyToClipboard     (t_tll *x, t_dictionary *d);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_key(t_tll *x, t_object *pv, long keycode, long m, long textcharacter)
{
    tllMethod f = NULL;
    ulong dirty = TLL_DIRTY_NONE;
        
    switch (keycode) {
        case TLL_KEY_A : f = tralala_userKeyAll;    break;
        case TLL_KEY_C : f = tralala_userKeyCopy;   break;
        case TLL_KEY_V : f = tralala_userKeyPaste;  break;
    }
    
    if (f) {
        dirty |= tralala_userAbort(x);
        dirty |= (*f)(x, m);
    }
    
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

void tralala_mousewheel(t_tll *x, t_object *view, t_pt pt, long m, double x_inc, double y_inc)
{
    long h = object_attr_getlong(x, TLL_SYM_XOFFSET) - (x_inc * 100);
    long v = object_attr_getlong(x, TLL_SYM_YOFFSET) - (y_inc * 100);
        
    object_attr_setlong(x, TLL_SYM_XOFFSET, h);
    object_attr_setlong(x, TLL_SYM_YOFFSET, v);
}

void tralala_mousedown(t_tll *x, t_object *pv, t_pt pt, long m)
{	
    x->cursor.x = pt.x - 1.;
    x->cursor.y = pt.y - 1.;
    x->origin.x = pt.x - 1.;
    x->origin.y = pt.y - 1.;
    
    if (!(m & eShiftKey)) {
        tralala_userUnselectAll(x);
    }
    
    if (m & eCommandKey) {
        tralala_userAddNote(x);
        
    } else if (m & eControlKey) {
        tralala_userHitZone(x);

    } else if (!(tralala_userHitNote(x))) {
        x->flags |= TLL_FLAG_LASSO;
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
    } else {
        x->flags &= ~TLL_FLAG_SHIFT;
    }
    
    if (x->flags & TLL_FLAG_LASSO) {
        if (tralala_userSelectNoteByLasso(x)) {
            jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE);
        }

        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_LASSO);
        jbox_redraw((t_jbox *)x);
    }
}

void tralala_mouseup(t_tll *x, t_object *pv, t_pt pt, long m)
{
    if (x->flags & TLL_FLAG_LASSO) {
        tralala_userReleaseLasso(x);
        
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

ulong tralala_userKeyAll(t_tll *x, long m)
{
    ulong dirty = TLL_DIRTY_NONE;
    
    if (m & eCommandKey) {
        tralala_userSelectAll(x);
        dirty |= TLL_DIRTY_NOTE;
        
        if (m & eShiftKey) {
            tralala_userSelectZone(x);
            dirty |= TLL_DIRTY_ZONE;
        }
    }
    
    return dirty;
}

ulong tralala_userKeyCopy(t_tll *x, long m)
{
    if (m & eCommandKey) {
        t_dictionary *t = NULL;
        
        if (t = dictionary_new( )) {
            tralala_userCopyToClipboard(x, t);
            dictionary_appendlong(t, TLL_SYM_IDENTIFIER, x->identifier);
            dictionary_clear(tll_clipboard);
            dictionary_copyunique(tll_clipboard, t);
            object_free(t);
        }
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_userKeyPaste(t_tll *x, long m)
{
    if (m & eCommandKey) {
        long k;
        t_dictionary *t = NULL;
        
        if (t = dictionary_new( )) {
            dictionary_copyunique(t, tll_clipboard);
            if (!(dictionary_getlong(t, TLL_SYM_IDENTIFIER, &k)) && (k != x->identifier)) {
                tralala_parseDictionary(x, t, TLL_DEFER);  
            }
            object_free(t);
        }
    }
    
    return TLL_DIRTY_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_userAddNote(t_tll *x)
{
    t_atom a[2];
    
    atom_setlong(a, TLL_X_TO_POSITION(x->cursor.x));
    atom_setlong(a + 1, TLL_Y_TO_PITCH(x->cursor.y));
    
    tralala_parseMessage(x, TLL_SYM_NOTE, 2, a, TLL_DEFER);
}

void tralala_userSelectZone(t_tll *x)
{
    TLL_LOCK
    dictionary_appendlong(x->status, TLL_SYM_ZONE, TLL_SELECTED);
    TLL_UNLOCK
}

void tralala_userSelectAll(t_tll *x)
{
    long i, n;
    t_symbol **keys = NULL;
    
    TLL_LOCK
    
    if (!(dictionary_getkeys(x->current, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
        long argc;
        t_atom *argv = NULL;
        t_symbol *key = (*(keys + i));
        
        if (!(dictionary_getatoms(x->current, key, &argc, &argv)) && ((atom_getsym(argv) == TLL_SYM_NOTE))) { 
            dictionary_appendlong(x->status, key, TLL_SELECTED); 
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    TLL_UNLOCK
}

void tralala_userUnselectAll(t_tll *x)
{
    TLL_LOCK
    dictionary_clear(x->status);
    TLL_UNLOCK
}

void tralala_userReleaseLasso(t_tll *x)
{
    long i, n;
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

void tralala_userHitZone(t_tll *x)
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

bool tralala_userHitNote(t_tll *x)
{
    long i, n;
    t_symbol *s = NULL;
    t_symbol **keys = NULL;
    long position = TLL_X_TO_POSITION(x->cursor.x - 1.);
    long pitch = TLL_Y_TO_PITCH(x->cursor.y - 1.);
    bool k = true;
    
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
        if (dictionary_hasentry(x->status, s)) {
            t_symbol *mark = NULL;
            if(!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) && (s == mark)) {
                dictionary_deleteentry(x->status, TLL_SYM_MARK); 
            }
            
            dictionary_deleteentry(x->status, s);
        } else {
            dictionary_appendsym(x->status, TLL_SYM_MARK, s);
            dictionary_appendlong(x->status, s, TLL_SELECTED);
        }
        
    } else {
        k = false;
    }
    
    TLL_UNLOCK
    
    return k;
}

ulong tralala_userSelectNoteByLasso(t_tll *x)
{
    long i, n;
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
    
    TLL_UNLOCK
        
    return dirty;
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

void tralala_userCopyToClipboard(t_tll *x, t_dictionary *d)
{
    long i, n;
    t_symbol **keys = NULL;
    
    TLL_LOCK
    
    if (!(dictionary_getkeys(x->status, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long status = 0;
    t_symbol *key = (*(keys + i));
    
    if ((key != TLL_SYM_MARK) && !(dictionary_getlong(x->status, key, &status)) && (status == TLL_SELECTED)) {
        long argc;
        t_atom *argv = NULL;
        if (!(dictionary_getatoms(x->current, key, &argc, &argv))) {
            dictionary_appendatoms(d, key, argc, argv);
        }
    }
    //
    }
    
    dictionary_freekeys(x->status, n, keys);
    //
    }
    
    TLL_UNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
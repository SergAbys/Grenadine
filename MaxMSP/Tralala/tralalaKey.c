/*
 *  tralalaKey.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaKey.h"
#include "tralalaParse.h"
#include "tralalaPaint.h"
#include "tralalaMouse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;
extern t_dictionary *tll_clipboard;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TLL_NADA    0

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

PIZ_LOCAL   ulong tralala_keyAll                (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyCopy               (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyPaste              (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyCut                (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyDelete             (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyUpArrow            (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyDownArrow          (t_tll *x, long m);

PIZ_LOCAL   void  tralala_keySelectZone         (t_tll *x);
PIZ_LOCAL   void  tralala_keySelectAll          (t_tll *x);
PIZ_LOCAL   void  tralala_keyCopyToClipboard    (t_tll *x, t_dictionary *d);

PIZ_LOCAL   void  tralala_keySendToZone         (t_tll *x, long keycode);
PIZ_LOCAL   void  tralala_keySendToNotes        (t_tll *x, PIZEventCode code, long selector);
PIZ_INLINE  void  tralala_keyTagWithSymbol      (long *tag, t_symbol *s);

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
        case TLL_KEY_A      : f = tralala_keyAll;       break;
        case TLL_KEY_C      : f = tralala_keyCopy;      break;
        case TLL_KEY_V      : f = tralala_keyPaste;     break;
        case TLL_KEY_X      : f = tralala_keyCut;       break;
        case JKEY_DELETE    : f = tralala_keyDelete;    break;
        case JKEY_BACKSPACE : f = tralala_keyDelete;    break;
        case JKEY_UPARROW   : f = tralala_keyUpArrow;   break;
        case JKEY_DOWNARROW : f = tralala_keyDownArrow; break;
    }
    
	//JKEY_LEFTARROW
	//JKEY_RIGHTARROW
    
    if (f) {
        dirty |= tralala_mouseAbort(x);
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

ulong tralala_keyAll(t_tll *x, long m)
{
    ulong dirty = TLL_DIRTY_NONE;
    
    if (m & eCommandKey) {
        tralala_keySelectAll(x);
        dirty |= TLL_DIRTY_NOTE;
        
        if (m & eShiftKey) {
            tralala_keySelectZone(x);
            dirty |= TLL_DIRTY_ZONE;
        }
    }
    
    return dirty;
}

ulong tralala_keyCopy(t_tll *x, long m)
{
    if (m & eCommandKey) {
        t_dictionary *t = NULL;
        
        if (t = dictionary_new( )) {
            tralala_keyCopyToClipboard(x, t);
            dictionary_clear(tll_clipboard);
            dictionary_copyunique(tll_clipboard, t);
            object_free(t);
        }
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyPaste(t_tll *x, long m)
{
    if (m & eCommandKey) {
        t_dictionary *t = NULL;
        
        if (t = dictionary_new( )) {
            tralala_mouseUnselectAll(x);
            dictionary_copyunique(t, tll_clipboard);
            tralala_parseDictionary(x, t, TLL_FLAG_LOW);  
            object_free(t);
        }
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyCut(t_tll *x, long m)
{
    tralala_keyCopy(x, m);
    tralala_keyDelete(x, m);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyDelete(t_tll *x, long m)
{
    tralala_keySendToNotes(x, PIZ_EVENT_DELETE, TLL_NADA);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyUpArrow(t_tll *x, long m)
{
    tralala_keySendToNotes(x, PIZ_EVENT_INCREMENT, PIZ_VALUE_PITCH);
    tralala_keySendToZone(x, JKEY_UPARROW);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyDownArrow(t_tll *x, long m)
{
    tralala_keySendToNotes(x, PIZ_EVENT_DECREMENT, PIZ_VALUE_PITCH);
    tralala_keySendToZone(x, JKEY_DOWNARROW);
    
    return TLL_DIRTY_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_keySelectZone(t_tll *x)
{
    TLL_LOCK
    dictionary_appendlong(x->status, TLL_SYM_ZONE, TLL_SELECTED);
    TLL_UNLOCK
}

void tralala_keySelectAll(t_tll *x)
{
    long i, n = 0;
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

void tralala_keyCopyToClipboard(t_tll *x, t_dictionary *d)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    
    TLL_LOCK
    
    if (!(dictionary_getkeys(x->status, &n, &keys))) {
    //
    long ac, cell = 0;
    t_atom *av = NULL;
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_CELL, &ac, &av))) {
        cell = atom_getlong(av + 1);
    }
        
    for (i = 0; i < n; i++) {
    //
    long status = 0;
    t_symbol *key = (*(keys + i));
    
    if ((key != TLL_SYM_MARK) && !(dictionary_getlong(x->status, key, &status)) && (status == TLL_SELECTED)) {
        long argc;
        t_atom *argv = NULL;
        
        if (!(dictionary_copyatoms(x->current, key, &argc, &argv))) {
            if (key != TLL_SYM_ZONE) {
                atom_setlong(argv + 1, (atom_getlong(argv + 1) + cell));
                atom_setlong(argv + 2, (atom_getlong(argv + 2) - 1));
            }
            
            dictionary_appendatoms(d, key, argc, argv);
            sysmem_freeptr(argv);
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
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_keySendToZone(t_tll *x, long keycode)
{
    ;
}

void tralala_keySendToNotes(t_tll *x, PIZEventCode code, long selector)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    long *ptr = NULL;
    PIZArray *t = NULL;
    
    TLL_LOCK
    
    if (t = pizArrayNew(dictionary_getentrycount(x->status))) {
    //
    if (!(dictionary_getkeys(x->status, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
        t_symbol *key = (*(keys + i));
        
        if ((key != TLL_SYM_ZONE) && (key != TLL_SYM_MARK)) {
            long tag = -1;
            tralala_keyTagWithSymbol(&tag, key);
            pizArrayAppend(t, tag);
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    //
    }
    
    TLL_UNLOCK
    
    if (t) {
    //
    ptr = pizArrayPtr(t);
    
    for (i = 0; i < pizArrayCount(t); i++) {
        PIZEvent *event = NULL;
        if (event = pizEventNew(code)) {
            long a[ ] = { selector, *(ptr + i) };
            pizEventSetData(event, 2, a);
            pizAgentDoEvent(x->agent, event);
        }
    }
    
    pizArrayFree(t);
    //
    }
}

PIZ_INLINE void tralala_keyTagWithSymbol(long *tag, t_symbol *s)
{
    if (s) {
        (*tag) = atoi(s->s_name);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 *  tralalaUser.c
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

PIZ_LOCAL   ulong tralala_userKeyAll            (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyCopy           (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyPaste          (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyCut            (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyDelete         (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyUpArrow        (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_userKeyDownArrow      (t_tll *x, long m);

PIZ_LOCAL   void  tralala_userSelectZone        (t_tll *x);
PIZ_LOCAL   void  tralala_userSelectAll         (t_tll *x);
PIZ_LOCAL   void  tralala_userCopyToClipboard   (t_tll *x, t_dictionary *d);

PIZ_LOCAL   void  tralala_userSend              (t_tll *x, PIZEventCode code);
PIZ_INLINE  void  tralala_userTagWithSymbol     (long *tag, t_symbol *s);

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
        case TLL_KEY_A      : f = tralala_userKeyAll;       break;
        case TLL_KEY_C      : f = tralala_userKeyCopy;      break;
        case TLL_KEY_V      : f = tralala_userKeyPaste;     break;
        case TLL_KEY_X      : f = tralala_userKeyCut;       break;
        case JKEY_DELETE    : f = tralala_userKeyDelete;    break;
        case JKEY_BACKSPACE : f = tralala_userKeyDelete;    break;
        case JKEY_UPARROW   : f = tralala_userKeyUpArrow;   break;
        case JKEY_DOWNARROW : f = tralala_userKeyDownArrow; break;
    }
    
	//JKEY_LEFTARROW
	//JKEY_RIGHTARROW
    
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
                tralala_parseDictionary(x, t, TLL_LOW);  
            }
            object_free(t);
        }
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_userKeyCut(t_tll *x, long m)
{
    if (m & eCommandKey) {
        ;
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_userKeyDelete(t_tll *x, long m)
{
    tralala_userSend(x, PIZ_EVENT_DELETE);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_userKeyUpArrow(t_tll *x, long m)
{
    return TLL_DIRTY_NONE;
}

ulong tralala_userKeyDownArrow(t_tll *x, long m)
{
    return TLL_DIRTY_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_userSelectZone(t_tll *x)
{
    TLL_LOCK
    dictionary_appendlong(x->status, TLL_SYM_ZONE, TLL_SELECTED);
    TLL_UNLOCK
}

void tralala_userSelectAll(t_tll *x)
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

void tralala_userCopyToClipboard(t_tll *x, t_dictionary *d)
{
    long i, n = 0;
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
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_userSend(t_tll *x, PIZEventCode code)
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
            tralala_userTagWithSymbol(&tag, key);
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
            pizEventSetData(event, 1, ptr + i);
            pizEventSetIdentifier(event, x->identifier);
            pizAgentDoEvent(x->agent, event);
        }
    }
    
    pizArrayFree(t);
    //
    }
}

PIZ_INLINE void tralala_userTagWithSymbol(long *tag, t_symbol *s)
{
    if (s) {
        (*tag) = atoi(s->s_name);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
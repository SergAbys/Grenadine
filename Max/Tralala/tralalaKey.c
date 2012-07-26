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
PIZ_LOCAL   ulong tralala_keyUp                 (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyDown               (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyLeft               (t_tll *x, long m);
PIZ_LOCAL   ulong tralala_keyRight              (t_tll *x, long m);

PIZ_LOCAL   void  tralala_keySelectAll          (t_tll *x);
PIZ_LOCAL   void  tralala_keySelectZone         (t_tll *x);
PIZ_LOCAL   void  tralala_keyCopySelected       (t_tll *x, t_dictionary *d);

PIZ_LOCAL   void  tralala_keyDuplicate          (t_tll *x, long m);
PIZ_LOCAL   void  tralala_keyChangeZone         (t_tll *x, long m, long keycode);
PIZ_LOCAL   void  tralala_keyChangeNotes        (t_tll *x, long m, PIZEventCode code, long s);
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
        case TLL_KEY_A          : f = tralala_keyAll;    break;
        case TLL_KEY_C          : f = tralala_keyCopy;   break;
        case TLL_KEY_V          : f = tralala_keyPaste;  break;
        case TLL_KEY_X          : f = tralala_keyCut;    break;
        case JKEY_DELETE        : f = tralala_keyDelete; break;
        case JKEY_BACKSPACE     : f = tralala_keyDelete; break;
        case JKEY_UPARROW       : f = tralala_keyUp;     break;
        case JKEY_DOWNARROW     : f = tralala_keyDown;   break;
        case JKEY_LEFTARROW     : f = tralala_keyLeft;   break;
        case JKEY_RIGHTARROW    : f = tralala_keyRight;  break;
    }
    
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
    t_dictionary *t = NULL;
            
    if ((m & eCommandKey) && (t = dictionary_new( ))) {
        tralala_keyCopySelected(x, t);
        dictionary_appendlong(t, TLL_SYM_IDENTIFIER, x->identifier);
        dictionary_clear(tll_clipboard);
        dictionary_copyunique(tll_clipboard, t);
        object_free(t);
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyPaste(t_tll *x, long m)
{
    long k;
    t_dictionary *t = NULL;
    
    if ((m & eCommandKey) && (t = dictionary_new( ))) {
    //
    dictionary_copyunique(t, tll_clipboard);
    
    if (!(dictionary_getlong(t, TLL_SYM_IDENTIFIER, &k)) && (k != x->identifier)) {
        tralala_mouseUnselectAll(x);
        tralala_parseDictionary(x, t, TLL_FLAG_LOW);  
    }
    
    object_free(t);
    //
    }
    
    return (TLL_DIRTY_ZONE | TLL_DIRTY_NOTE);
}

ulong tralala_keyCut(t_tll *x, long m)
{
    tralala_keyCopy(x, m);
    tralala_keyDelete(x, m);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyDelete(t_tll *x, long m)
{
    tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_DELETE, 0);
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyUp(t_tll *x, long m)
{
    if (m & eControlKey) {
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_INCREMENT, PIZ_VALUE_VELOCITY);
        
    } else {
        tralala_keyDuplicate(x, m);
        tralala_keyChangeZone(x, m, JKEY_UPARROW);
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_INCREMENT, PIZ_VALUE_PITCH);
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyDown(t_tll *x, long m)
{
    if (m & eControlKey) {
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_DECREMENT, PIZ_VALUE_VELOCITY);
    
    } else {
        tralala_keyDuplicate(x, m);
        tralala_keyChangeZone(x, m, JKEY_DOWNARROW);
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_DECREMENT, PIZ_VALUE_PITCH);
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyLeft(t_tll *x, long m)
{
    if (m & eControlKey) {
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_DECREMENT, PIZ_VALUE_DURATION);
        
    } else {
        tralala_keyDuplicate(x, m);
        tralala_keyChangeZone(x, m, JKEY_LEFTARROW);
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_BACKWARD, 0);
    }
    
    return TLL_DIRTY_NONE;
}

ulong tralala_keyRight(t_tll *x, long m)
{
    if (m & eControlKey) {
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_INCREMENT, PIZ_VALUE_DURATION);
        
    } else {
        tralala_keyDuplicate(x, m);
        tralala_keyChangeZone(x, m, JKEY_RIGHTARROW);
        tralala_keyChangeNotes(x, m, PIZ_EVENT_NOTE_FORWARD, 0);
    }
    
    return TLL_DIRTY_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

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

void tralala_keySelectZone(t_tll *x)
{
    TLL_LOCK
    dictionary_appendlong(x->status, TLL_SYM_ZONE, TLL_SELECTED);
    TLL_UNLOCK
}

void tralala_keyCopySelected(t_tll *x, t_dictionary *d)
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

void tralala_keyDuplicate(t_tll *x, long m)
{
    t_dictionary *t = NULL;
            
    if ((m & eAltKey) && (t = dictionary_new( ))) {
        tralala_keyCopySelected(x, t);
        tralala_parseDictionary(x, t, TLL_FLAG_NONE);  
        object_free(t);
    }
}

void tralala_keyChangeZone(t_tll *x, long m, long keycode)
{
    long status = 0;
    PIZArray *t = NULL;
    PIZEventCode code = PIZ_EVENT_NONE;
    
    TLL_LOCK
    
    if ((t = pizArrayNew(0)) && !(dictionary_getlong(x->status, TLL_SYM_ZONE, &status))) {
    //
    bool k = (status == TLL_SELECTED);
    
    switch (keycode) {
    //
    case JKEY_UPARROW    :
        code = PIZ_EVENT_ZONE_INCREMENT;
        if (k || (status == TLL_SELECTED_UP))   { pizArrayAppend(t, PIZ_ZONE_UP); }
        if (k || (status == TLL_SELECTED_DOWN)) { pizArrayAppend(t, PIZ_ZONE_DOWN); }
        break;
        
    case JKEY_DOWNARROW  :
        code = PIZ_EVENT_ZONE_DECREMENT;
        if (k || (status == TLL_SELECTED_DOWN)) { pizArrayAppend(t, PIZ_ZONE_DOWN); }
        if (k || (status == TLL_SELECTED_UP))   { pizArrayAppend(t, PIZ_ZONE_UP); }
        break;
        
    case JKEY_LEFTARROW  :
        code = PIZ_EVENT_ZONE_DECREMENT;
        if (k || (status == TLL_SELECTED_START)) { pizArrayAppend(t, PIZ_ZONE_START); }
        if (k || (status == TLL_SELECTED_END))   { pizArrayAppend(t, PIZ_ZONE_END); }
        break;
        
    case JKEY_RIGHTARROW :
        code = PIZ_EVENT_ZONE_INCREMENT;
        if (k || (status == TLL_SELECTED_END))   { pizArrayAppend(t, PIZ_ZONE_END); }
        if (k || (status == TLL_SELECTED_START)) { pizArrayAppend(t, PIZ_ZONE_START); }
        break;
    //
    }
    //
    }
    
    TLL_UNLOCK
    
    if (t) {
    //
    long i;
    
    for (i = 0; i < pizArrayCount(t); i++) {
    //
    PIZEvent *event = NULL;
    if (event = pizEventNew(code)) {
    
        if (m & (eAutoRepeat | eShiftKey)) {
            long a[ ] = { (*(pizArrayPtr(t) + i)), true };
            pizEventSetData(event, 2, a);
        } else {
            pizEventSetData(event, 1, (pizArrayPtr(t) + i));
        }
        
        pizAgentDoEvent(x->agent, event);
    }
    //
    }
    
    pizArrayFree(t);
    //
    }
}

void tralala_keyChangeNotes(t_tll *x, long m, PIZEventCode code, long selector)
{
    long i, n = 0;
    t_symbol **keys = NULL;
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
    for (i = 0; i < pizArrayCount(t); i++) {
    //
    PIZEvent *event = NULL;
    if (event = pizEventNew(code)) {
    
        if (m & (eAutoRepeat | eShiftKey)) {
            long a[ ] = { selector, (*(pizArrayPtr(t) + i)), true };
            pizEventSetData(event, 3, a);
        } else {
            long a[ ] = { selector, (*(pizArrayPtr(t) + i)) };
            pizEventSetData(event, 2, a);
        }
        
        pizAgentDoEvent(x->agent, event);
    }
    //
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
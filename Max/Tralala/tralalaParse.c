/*
 *  tralalaParse.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"
#include "tralalaPaint.h"
#include "tralalaMouse.h"
#include "ext_quickmap.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_TINY 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE void tralala_parseSymbolWithTag   (t_symbol **s, long tag);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static t_quickmap   *tll_type;
static t_quickmap   *tll_length;
static t_quickmap   *tll_direction;
static t_quickmap   *tll_select;
static t_quickmap   *tll_key;
static t_quickmap   *tll_notification;

static t_dictionary *tll_code;

void tralala_parseInit(t_tllSymbols *table)
{
//
table->bpm              = gensym("bpm");
table->chance           = gensym("chance");
table->velocity         = gensym("velocity");
table->channel          = gensym("channel");
table->chord            = gensym("chord");
table->cell             = gensym("cell");
table->value            = gensym("value");
table->scale            = gensym("scale");
table->end              = gensym("end");
table->clear            = gensym("clear");
table->tralala          = gensym("tralala");
table->untitled         = gensym("untitled");
table->load             = gensym("load");
table->identifier       = gensym("identifier");
table->mark             = gensym("mark");
table->run              = gensym("run");
table->note             = gensym("note");
table->text             = gensym("text");
table->zone             = gensym("zone");
table->background       = gensym("background");
table->lasso            = gensym("lasso");
table->xoffset          = gensym("xoffset");
table->yoffset          = gensym("yoffset");
table->color            = gensym("color");
table->hcolor1          = gensym("hcolor1");
table->hcolor2          = gensym("hcolor2");
table->ucolor           = gensym("ucolor");
table->lassocolor       = gensym("lassocolor");
table->attr_modified    = gensym("attr_modified");
table->getname          = gensym("getname");
table->patching_rect    = gensym("patching_rect");

tll_code = dictionary_new( );

dictionary_appendlong(tll_code, gensym("bpm"),      (TLL_TINY + PIZ_EVENT_BPM));
dictionary_appendlong(tll_code, gensym("learn"),    (TLL_TINY + PIZ_EVENT_LEARN));
dictionary_appendlong(tll_code, gensym("list"),     (TLL_TINY + PIZ_EVENT_LEARN));
dictionary_appendlong(tll_code, gensym("forget"),   (TLL_TINY + PIZ_EVENT_FORGET));
dictionary_appendlong(tll_code, gensym("dump"),     (TLL_TINY + PIZ_EVENT_DUMP));
dictionary_appendlong(tll_code, gensym("chance"),   (TLL_TINY + PIZ_EVENT_CHANCE));
dictionary_appendlong(tll_code, gensym("velocity"), (TLL_TINY + PIZ_EVENT_VELOCITY));
dictionary_appendlong(tll_code, gensym("channel"),  (TLL_TINY + PIZ_EVENT_CHANNEL));
dictionary_appendlong(tll_code, gensym("chord"),    (TLL_TINY + PIZ_EVENT_CHORD));
dictionary_appendlong(tll_code, gensym("cell"),     (TLL_TINY + PIZ_EVENT_CELL));
dictionary_appendlong(tll_code, gensym("value"),    (TLL_TINY + PIZ_EVENT_VALUE));
dictionary_appendlong(tll_code, gensym("scale"),    (TLL_TINY + PIZ_EVENT_SCALE));
dictionary_appendlong(tll_code, gensym("note"),     (TLL_TINY + PIZ_EVENT_NOTE));
dictionary_appendlong(tll_code, gensym("zone"),     (TLL_TINY + PIZ_EVENT_ZONE));
dictionary_appendlong(tll_code, gensym("clear"),    (TLL_TINY + PIZ_EVENT_CLEAR));
dictionary_appendlong(tll_code, gensym("clean"),    (TLL_TINY + PIZ_EVENT_CLEAN));
dictionary_appendlong(tll_code, gensym("rotate"),   (TLL_TINY + PIZ_EVENT_ROTATE));
dictionary_appendlong(tll_code, gensym("scramble"), (TLL_TINY + PIZ_EVENT_SCRAMBLE));
dictionary_appendlong(tll_code, gensym("sort"),     (TLL_TINY + PIZ_EVENT_SORT));
dictionary_appendlong(tll_code, gensym("change"),   (TLL_TINY + PIZ_EVENT_CHANGE));
dictionary_appendlong(tll_code, gensym("fill"),     (TLL_TINY + PIZ_EVENT_FILL));
dictionary_appendlong(tll_code, gensym("kill"),     (TLL_TINY + PIZ_EVENT_KILL));
dictionary_appendlong(tll_code, gensym("cycle"),    (TLL_TINY + PIZ_EVENT_CYCLE));
dictionary_appendlong(tll_code, gensym("pattern"),  (TLL_TINY + PIZ_EVENT_PATTERN));
dictionary_appendlong(tll_code, gensym("zoulou"),   (TLL_TINY + PIZ_EVENT_ZOULOU));
dictionary_appendlong(tll_code, gensym("romeo"),    (TLL_TINY + PIZ_EVENT_ROMEO));
dictionary_appendlong(tll_code, gensym("juliet"),   (TLL_TINY + PIZ_EVENT_JULIET));

tll_type         = (t_quickmap *)quickmap_new( );
tll_length       = (t_quickmap *)quickmap_new( );
tll_direction    = (t_quickmap *)quickmap_new( );
tll_key          = (t_quickmap *)quickmap_new( );
tll_select       = (t_quickmap *)quickmap_new( );
tll_notification = (t_quickmap *)quickmap_new( );

quickmap_add(tll_type, gensym("none"),                    (void *)(TLL_TINY + PIZ_MODE_NONE));
quickmap_add(tll_type, gensym("ionian"),                  (void *)(TLL_TINY + PIZ_IONIAN));
quickmap_add(tll_type, gensym("dorian"),                  (void *)(TLL_TINY + PIZ_DORIAN));
quickmap_add(tll_type, gensym("phrygian"),                (void *)(TLL_TINY + PIZ_PHRYGIAN));
quickmap_add(tll_type, gensym("lydian"),                  (void *)(TLL_TINY + PIZ_LYDIAN));
quickmap_add(tll_type, gensym("mixolydian"),              (void *)(TLL_TINY + PIZ_MIXOLYDIAN));
quickmap_add(tll_type, gensym("aeolian"),                 (void *)(TLL_TINY + PIZ_AEOLIAN));
quickmap_add(tll_type, gensym("locrian"),                 (void *)(TLL_TINY + PIZ_LOCRIAN));
quickmap_add(tll_type, gensym("natural"),                 (void *)(TLL_TINY + PIZ_NATURAL));
quickmap_add(tll_type, gensym("harmonic"),                (void *)(TLL_TINY + PIZ_HARMONIC));
quickmap_add(tll_type, gensym("melodic"),                 (void *)(TLL_TINY + PIZ_MELODIC));
quickmap_add(tll_type, gensym("whole_tone"),              (void *)(TLL_TINY + PIZ_WHOLE_TONE));
quickmap_add(tll_type, gensym("pentatonic_major"),        (void *)(TLL_TINY + PIZ_PENTATONIC_MAJOR));
quickmap_add(tll_type, gensym("pentatonic_minor"),        (void *)(TLL_TINY + PIZ_PENTATONIC_MINOR));
quickmap_add(tll_type, gensym("octatonic_half_whole"),    (void *)(TLL_TINY + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add(tll_type, gensym("octatonic_whole_half"),    (void *)(TLL_TINY + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add(tll_type, gensym("7th_major"),               (void *)(TLL_TINY + PIZ_SEVENTH_MAJOR));
quickmap_add(tll_type, gensym("7th_minor"),               (void *)(TLL_TINY + PIZ_SEVENTH_MINOR));
quickmap_add(tll_type, gensym("7th_dominant"),            (void *)(TLL_TINY + PIZ_SEVENTH_DOMINANT));
quickmap_add(tll_type, gensym("7th_major_sixth"),         (void *)(TLL_TINY + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add(tll_type, gensym("7th_minor_sixth"),         (void *)(TLL_TINY + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add(tll_type, gensym("7th_half_diminished"),     (void *)(TLL_TINY + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add(tll_type, gensym("7th_diminished"),          (void *)(TLL_TINY + PIZ_SEVENTH_DIMINISHED));
quickmap_add(tll_type, gensym("7th_dominant_suspended"),  (void *)(TLL_TINY + PIZ_SEVENTH_SUSPENDED));
quickmap_add(tll_type, gensym("7th_dominant_sharp_five"), (void *)(TLL_TINY + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add(tll_type, gensym("7th_dominant_flat_five"),  (void *)(TLL_TINY + PIZ_SEVENTH_FLAT_FIVE));

quickmap_add(tll_length, gensym("none"),                  (void *)(TLL_TINY + PIZ_NOTE_NONE));
quickmap_add(tll_length, gensym("whole"),                 (void *)(TLL_TINY + PIZ_WHOLE_NOTE));
quickmap_add(tll_length, gensym("half"),                  (void *)(TLL_TINY + PIZ_HALF_NOTE));
quickmap_add(tll_length, gensym("quarter"),               (void *)(TLL_TINY + PIZ_QUARTER_NOTE));
quickmap_add(tll_length, gensym("eighth"),                (void *)(TLL_TINY + PIZ_EIGHTH_NOTE));
quickmap_add(tll_length, gensym("sixteenth"),             (void *)(TLL_TINY + PIZ_SIXTEENTH_NOTE));
quickmap_add(tll_length, gensym("thirty_second"),         (void *)(TLL_TINY + PIZ_THIRTY_SECOND_NOTE));
quickmap_add(tll_length, gensym("whole_triplet"),         (void *)(TLL_TINY + PIZ_WHOLE_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("half_triplet"),          (void *)(TLL_TINY + PIZ_HALF_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("quarter_triplet"),       (void *)(TLL_TINY + PIZ_QUARTER_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("eighth_triplet"),        (void *)(TLL_TINY + PIZ_EIGHTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("sixteenth_triplet"),     (void *)(TLL_TINY + PIZ_SIXTEENTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("thirty_second_triplet"), (void *)(TLL_TINY + PIZ_THIRTY_SECOND_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("whole_dotted"),          (void *)(TLL_TINY + PIZ_WHOLE_NOTE_DOTTED));
quickmap_add(tll_length, gensym("half_dotted"),           (void *)(TLL_TINY + PIZ_HALF_NOTE_DOTTED));
quickmap_add(tll_length, gensym("quarter_dotted"),        (void *)(TLL_TINY + PIZ_QUARTER_NOTE_DOTTED));
quickmap_add(tll_length, gensym("eighth_dotted"),         (void *)(TLL_TINY + PIZ_EIGHTH_NOTE_DOTTED));
quickmap_add(tll_length, gensym("sixteenth_dotted"),      (void *)(TLL_TINY + PIZ_SIXTEENTH_NOTE_DOTTED));

quickmap_add(tll_direction, gensym("up"),                 (void *)(TLL_TINY + PIZ_UP)); 
quickmap_add(tll_direction, gensym("down"),               (void *)(TLL_TINY + PIZ_DOWN));

quickmap_add(tll_select, gensym("pitch"),                 (void *)(TLL_TINY + PIZ_VALUE_PITCH));
quickmap_add(tll_select, gensym("velocity"),              (void *)(TLL_TINY + PIZ_VALUE_VELOCITY));
quickmap_add(tll_select, gensym("duration"),              (void *)(TLL_TINY + PIZ_VALUE_DURATION));
quickmap_add(tll_select, gensym("channel"),               (void *)(TLL_TINY + PIZ_VALUE_CHANNEL));

quickmap_add(tll_key, gensym("C"),                        (void *)(TLL_TINY + PIZ_KEY_C));
quickmap_add(tll_key, gensym("C#"),                       (void *)(TLL_TINY + PIZ_KEY_C_SHARP));
quickmap_add(tll_key, gensym("D"),                        (void *)(TLL_TINY + PIZ_KEY_D));
quickmap_add(tll_key, gensym("D#"),                       (void *)(TLL_TINY + PIZ_KEY_D_SHARP));
quickmap_add(tll_key, gensym("E"),                        (void *)(TLL_TINY + PIZ_KEY_E));
quickmap_add(tll_key, gensym("F"),                        (void *)(TLL_TINY + PIZ_KEY_F));
quickmap_add(tll_key, gensym("F#"),                       (void *)(TLL_TINY + PIZ_KEY_F_SHARP));
quickmap_add(tll_key, gensym("G"),                        (void *)(TLL_TINY + PIZ_KEY_G));
quickmap_add(tll_key, gensym("G#"),                       (void *)(TLL_TINY + PIZ_KEY_G_SHARP));
quickmap_add(tll_key, gensym("A"),                        (void *)(TLL_TINY + PIZ_KEY_A));
quickmap_add(tll_key, gensym("A#"),                       (void *)(TLL_TINY + PIZ_KEY_A_SHARP));
quickmap_add(tll_key, gensym("B"),                        (void *)(TLL_TINY + PIZ_KEY_B));

quickmap_add(tll_notification, gensym("bpm"),             (void *)(TLL_TINY + PIZ_EVENT_CHANGED_BPM));
quickmap_add(tll_notification, gensym("chance"),          (void *)(TLL_TINY + PIZ_EVENT_CHANGED_CHANCE));
quickmap_add(tll_notification, gensym("velocity"),        (void *)(TLL_TINY + PIZ_EVENT_CHANGED_VELOCITY));
quickmap_add(tll_notification, gensym("channel"),         (void *)(TLL_TINY + PIZ_EVENT_CHANGED_CHANNEL));
quickmap_add(tll_notification, gensym("chord"),           (void *)(TLL_TINY + PIZ_EVENT_CHANGED_CHORD));
quickmap_add(tll_notification, gensym("cell"),            (void *)(TLL_TINY + PIZ_EVENT_CHANGED_CELL));
quickmap_add(tll_notification, gensym("value"),           (void *)(TLL_TINY + PIZ_EVENT_CHANGED_VALUE));
quickmap_add(tll_notification, gensym("scale"),           (void *)(TLL_TINY + PIZ_EVENT_CHANGED_SCALE));
quickmap_add(tll_notification, gensym("zone"),            (void *)(TLL_TINY + PIZ_EVENT_CHANGED_ZONE));
//
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseDictionary(t_tll *x, t_dictionary *d, ulong flags)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    
    if (!(dictionary_getkeys(d, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long k;
    t_atom *data = NULL;
    
    if (!dictionary_getatoms(d, (*(keys + i)), &k, &data)) {
        if (atom_gettype(data) == A_SYM) {
            tralala_parseMessage(x, atom_getsym(data), k - 1, data + 1, flags);
        }
    }
    //
    }
    
    dictionary_freekeys(d, n, keys);
    //
    }
}

void tralala_parseMessage(t_tll *x, t_symbol *s, long argc, t_atom *argv, ulong flags)
{
    PIZEventCode code = PIZ_EVENT_NONE;
        
    if (!(dictionary_getlong(tll_code, s, (long *)&code))) {
    //
    code -= TLL_TINY;
    
    if (!(((flags & TLL_FLAG_FILTER) && (code != PIZ_EVENT_NOTE) && (code != PIZ_EVENT_ZONE)))) {
    //
    long i, k = 0, msg = 0;
    long data[PIZ_EVENT_DATA_SIZE] = { 0 };
    PIZEvent *event = NULL;
    
    if (msg = ((code == PIZ_EVENT_ROTATE)   ||
               (code == PIZ_EVENT_SCRAMBLE) ||
               (code == PIZ_EVENT_SORT)     ||
               (code == PIZ_EVENT_CHANGE)   ||
               (code == PIZ_EVENT_FILL)     ||
               (code == PIZ_EVENT_CYCLE)))  { k = 1; }
                     
    for (i = 0; i < argc; i++) {
    //
    if (atom_gettype(argv + i) == A_SYM) {
    //
    long t, j = 0, p = 0;
     
    if ((code == PIZ_EVENT_SCALE) || (code == PIZ_EVENT_CYCLE)) {
        if (!(quickmap_lookup_key1(tll_type, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TLL_TINY; p = 1; 
        } else if (!(quickmap_lookup_key1(tll_key, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TLL_TINY;
        }
        
    } else if ((code == PIZ_EVENT_CELL) || (code == PIZ_EVENT_VALUE)) {
        if (!(quickmap_lookup_key1(tll_length, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TLL_TINY; p = 0;
        } else {
            continue;
        }
        
    } else if (msg) {
        if (!(quickmap_lookup_key1(tll_direction, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TLL_TINY; p = 1;
        } else if (!(quickmap_lookup_key1(tll_select, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TLL_TINY;
        }
        
    } else {
        continue;
    }

    data[p] = j;
    k = MAX(p + 1, k);
    //
    }
    //
    }
    
    for (i = 0; i < argc; i++) {
    //
    if ((k < PIZ_EVENT_DATA_SIZE) && (atom_gettype(argv + i) == A_LONG)) {
        data[k] = atom_getlong(argv + i);
        k++;
    }
    //
    }
    
    if (event = pizEventNew(code)) {
        pizEventSetData(event, k, data);
        
        if (flags & TLL_FLAG_LOW) {
            pizEventSetType(event, PIZ_EVENT_LOW);
        } else if (flags & TLL_FLAG_RUN) {
            pizEventSetType(event, PIZ_EVENT_RUN);
        }
        
        pizEventSetIdentifier(event, x->identifier);
        pizAgentDoEvent(x->agent, event);
    }
    //
    }
    //
    }
}

void tralala_parseNotification(t_tll *x, PIZEvent *event)
{
    long i, k = 0;
    long *ptr = NULL;
    t_symbol *s = NULL;
    PIZEventCode code = PIZ_EVENT_NONE;
    ulong dirty = TLL_DIRTY_NONE;
    t_atom data[PIZ_EVENT_DATA_SIZE + 1];
    
    pizEventCode(event, &code);
    pizEventData(event, &k, &ptr);
    
    for (i = 0; i < k; i++) {
        atom_setlong(data + i + 1, *(ptr + i));
    }
    
    if (!(quickmap_lookup_key2(tll_notification, (void *)(code + TLL_TINY), (void **)&s))) {
        
        if (code == PIZ_EVENT_CHANGED_ZONE) {
            dirty |= TLL_DIRTY_ZONE;
            
        } else if (code == PIZ_EVENT_CHANGED_SCALE) {
            t_symbol *sym1 = NULL;
            t_symbol *sym2 = NULL;

            quickmap_lookup_key2(tll_key,  (void *)((*(ptr + 0)) + TLL_TINY), (void **)&sym1);
            quickmap_lookup_key2(tll_type, (void *)((*(ptr + 1)) + TLL_TINY), (void **)&sym2);

            atom_setsym(data + 1, sym1);
            atom_setsym(data + 2, sym2);
        }

        atom_setsym(data, s);
        
        TLL_LOCK
        dictionary_appendatoms(x->current, s, k + 1, data); 
        TLL_UNLOCK

    } else {
        dirty |= tralala_mouseAbort(x);
        tralala_parseSymbolWithTag(&s, ptr[PIZ_EVENT_DATA_TAG]);
          
        TLL_LOCK
        
        if (code == PIZ_EVENT_NOTE_REMOVED) {
        
            if (dictionary_hasentry(x->status, s)) {
                t_symbol *mark = NULL;
                if (!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) && (s == mark)) {
                    dictionary_deleteentry(x->status, TLL_SYM_MARK); 
                }
                
                dictionary_deleteentry(x->status, s);
            }
            
            dictionary_deleteentry(x->current, s);
        
        } else {
        
            if ((code == PIZ_EVENT_NOTE_ADDED) && (*(ptr + PIZ_EVENT_DATA_LOW))) {
                dictionary_appendsym(x->status, TLL_SYM_MARK, s);
                dictionary_appendlong(x->status, s, TLL_SELECTED);
            }
            
            atom_setsym(data, TLL_SYM_NOTE);
            dictionary_appendatoms(x->current, s, 6, data);
        }
        
        TLL_UNLOCK
        
        dirty |= TLL_DIRTY_NOTE;
    }
        
    if (dirty & TLL_DIRTY_ZONE) { 
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_ZONE);
    } else if (dirty & TLL_DIRTY_NOTE) { 
        jbox_invalidate_layer((t_object *)x, NULL, TLL_SYM_NOTE); 
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

PIZ_INLINE void tralala_parseSymbolWithTag(t_symbol **s, long tag)
{
    char string[4];
    snprintf_zero(string, 4, "%ld", tag);
    (*s) = gensym(string);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
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

#define TLL_BIAS 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;
extern t_dictionary *tll_clipboard;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_STATIC void  tralala_parseEntryNote (t_tll *x, long ac, t_atom *av, long *k, long *data);
PIZ_STATIC void  tralala_parseEntryZone (t_tll *x, long ac, t_atom *av, long *k, long *data);
PIZ_STATIC void  tralala_parseEntryCode (t_tll *x, long ac, t_atom *av, long *k, long *data, PIZEventCode code);

PIZ_STATIC ulong tralala_parseEventNote     (t_tll *x, long k, long *data, PIZEventCode code);
PIZ_STATIC ulong tralala_parseEventNotify   (t_tll *x, long k, long *data, PIZEventCode code, t_symbol *s);
PIZ_STATIC void  tralala_parseEventInform   (t_tll *x, long k, long *data, PIZEventCode code, t_symbol *s);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_STATIC PIZError tralala_pitchToLong     (t_tll *x, t_atom *a, long *t);
PIZ_STATIC void     tralala_symbolWithTag   (t_symbol **s, long tag);

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
static t_quickmap   *tll_inform;
static t_quickmap   *tll_notify;

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
table->mute             = gensym("mute");
table->end              = gensym("end");
table->clear            = gensym("clear");
table->tralala          = gensym("tralala");
table->untitled         = gensym("untitled");
table->save             = gensym("save");
table->load             = gensym("load");
table->reload           = gensym("reload");
table->resave           = gensym("resave");
table->mark             = gensym("mark");
table->identifier       = gensym("identifier");
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
table->lassocolor       = gensym("lassocolor");
table->attr_modified    = gensym("attr_modified");
table->getname          = gensym("getname");
table->patching_rect    = gensym("patching_rect");
table->list             = gensym("list");
table->dumpout          = gensym("dumpout");

tll_code = dictionary_new( );

dictionary_appendlong(tll_code, gensym("learn"),        (TLL_BIAS + PIZ_MSG_LEARN));
dictionary_appendlong(tll_code, gensym("list"),         (TLL_BIAS + PIZ_MSG_LEARN));
dictionary_appendlong(tll_code, gensym("forget"),       (TLL_BIAS + PIZ_MSG_FORGET));
dictionary_appendlong(tll_code, gensym("bpm"),          (TLL_BIAS + PIZ_MSG_BPM));
dictionary_appendlong(tll_code, gensym("chance"),       (TLL_BIAS + PIZ_MSG_CHANCE));
dictionary_appendlong(tll_code, gensym("velocity"),     (TLL_BIAS + PIZ_MSG_VELOCITY));
dictionary_appendlong(tll_code, gensym("channel"),      (TLL_BIAS + PIZ_MSG_CHANNEL));
dictionary_appendlong(tll_code, gensym("chord"),        (TLL_BIAS + PIZ_MSG_CHORD));
dictionary_appendlong(tll_code, gensym("cell"),         (TLL_BIAS + PIZ_MSG_CELL));
dictionary_appendlong(tll_code, gensym("value"),        (TLL_BIAS + PIZ_MSG_VALUE));
dictionary_appendlong(tll_code, gensym("scale"),        (TLL_BIAS + PIZ_MSG_SCALE));
dictionary_appendlong(tll_code, gensym("mute"),         (TLL_BIAS + PIZ_MSG_MUTE));
dictionary_appendlong(tll_code, gensym("zone"),         (TLL_BIAS + PIZ_MSG_ZONE));
dictionary_appendlong(tll_code, gensym("note"),         (TLL_BIAS + PIZ_MSG_NOTE));
dictionary_appendlong(tll_code, gensym("clear"),        (TLL_BIAS + PIZ_MSG_CLEAR));
dictionary_appendlong(tll_code, gensym("clean"),        (TLL_BIAS + PIZ_MSG_CLEAN));
dictionary_appendlong(tll_code, gensym("transpose"),    (TLL_BIAS + PIZ_MSG_TRANSPOSE));
dictionary_appendlong(tll_code, gensym("rotate"),       (TLL_BIAS + PIZ_MSG_ROTATE));
dictionary_appendlong(tll_code, gensym("scramble"),     (TLL_BIAS + PIZ_MSG_SCRAMBLE));
dictionary_appendlong(tll_code, gensym("sort"),         (TLL_BIAS + PIZ_MSG_SORT));
dictionary_appendlong(tll_code, gensym("change"),       (TLL_BIAS + PIZ_MSG_CHANGE));
dictionary_appendlong(tll_code, gensym("fill"),         (TLL_BIAS + PIZ_MSG_FILL));
dictionary_appendlong(tll_code, gensym("kill"),         (TLL_BIAS + PIZ_MSG_KILL));
dictionary_appendlong(tll_code, gensym("cycle"),        (TLL_BIAS + PIZ_MSG_CYCLE));
dictionary_appendlong(tll_code, gensym("pattern"),      (TLL_BIAS + PIZ_MSG_PATTERN));
dictionary_appendlong(tll_code, gensym("zoulou"),       (TLL_BIAS + PIZ_MSG_ZOULOU));
dictionary_appendlong(tll_code, gensym("romeo"),        (TLL_BIAS + PIZ_MSG_ROMEO));
dictionary_appendlong(tll_code, gensym("juliet"),       (TLL_BIAS + PIZ_MSG_JULIET));
dictionary_appendlong(tll_code, gensym("dump"),         (TLL_BIAS + PIZ_MSG_DUMP));
dictionary_appendlong(tll_code, gensym("statistics"),   (TLL_BIAS + PIZ_MSG_STATISTICS));
dictionary_appendlong(tll_code, gensym("attributes"),   (TLL_BIAS + PIZ_MSG_ATTRIBUTES));

tll_type        = (t_quickmap *)quickmap_new( );
tll_length      = (t_quickmap *)quickmap_new( );
tll_direction   = (t_quickmap *)quickmap_new( );
tll_key         = (t_quickmap *)quickmap_new( );
tll_select      = (t_quickmap *)quickmap_new( );
tll_inform      = (t_quickmap *)quickmap_new( );
tll_notify      = (t_quickmap *)quickmap_new( );

quickmap_add(tll_type, gensym("none"),                    (void *)(TLL_BIAS + PIZ_MODE_NONE));
quickmap_add(tll_type, gensym("ionian"),                  (void *)(TLL_BIAS + PIZ_IONIAN));
quickmap_add(tll_type, gensym("dorian"),                  (void *)(TLL_BIAS + PIZ_DORIAN));
quickmap_add(tll_type, gensym("phrygian"),                (void *)(TLL_BIAS + PIZ_PHRYGIAN));
quickmap_add(tll_type, gensym("lydian"),                  (void *)(TLL_BIAS + PIZ_LYDIAN));
quickmap_add(tll_type, gensym("mixolydian"),              (void *)(TLL_BIAS + PIZ_MIXOLYDIAN));
quickmap_add(tll_type, gensym("aeolian"),                 (void *)(TLL_BIAS + PIZ_AEOLIAN));
quickmap_add(tll_type, gensym("locrian"),                 (void *)(TLL_BIAS + PIZ_LOCRIAN));
quickmap_add(tll_type, gensym("natural"),                 (void *)(TLL_BIAS + PIZ_NATURAL));
quickmap_add(tll_type, gensym("harmonic"),                (void *)(TLL_BIAS + PIZ_HARMONIC));
quickmap_add(tll_type, gensym("melodic"),                 (void *)(TLL_BIAS + PIZ_MELODIC));
quickmap_add(tll_type, gensym("whole_tone"),              (void *)(TLL_BIAS + PIZ_WHOLE_TONE));
quickmap_add(tll_type, gensym("pentatonic_major"),        (void *)(TLL_BIAS + PIZ_PENTATONIC_MAJOR));
quickmap_add(tll_type, gensym("pentatonic_minor"),        (void *)(TLL_BIAS + PIZ_PENTATONIC_MINOR));
quickmap_add(tll_type, gensym("octatonic_half_whole"),    (void *)(TLL_BIAS + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add(tll_type, gensym("octatonic_whole_half"),    (void *)(TLL_BIAS + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add(tll_type, gensym("7th_major"),               (void *)(TLL_BIAS + PIZ_SEVENTH_MAJOR));
quickmap_add(tll_type, gensym("7th_minor"),               (void *)(TLL_BIAS + PIZ_SEVENTH_MINOR));
quickmap_add(tll_type, gensym("7th_dominant"),            (void *)(TLL_BIAS + PIZ_SEVENTH_DOMINANT));
quickmap_add(tll_type, gensym("7th_major_sixth"),         (void *)(TLL_BIAS + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add(tll_type, gensym("7th_minor_sixth"),         (void *)(TLL_BIAS + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add(tll_type, gensym("7th_half_diminished"),     (void *)(TLL_BIAS + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add(tll_type, gensym("7th_diminished"),          (void *)(TLL_BIAS + PIZ_SEVENTH_DIMINISHED));
quickmap_add(tll_type, gensym("7th_dominant_suspended"),  (void *)(TLL_BIAS + PIZ_SEVENTH_SUSPENDED));
quickmap_add(tll_type, gensym("7th_dominant_sharp_five"), (void *)(TLL_BIAS + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add(tll_type, gensym("7th_dominant_flat_five"),  (void *)(TLL_BIAS + PIZ_SEVENTH_FLAT_FIVE));

quickmap_add(tll_length, gensym("none"),                  (void *)(TLL_BIAS + PIZ_NOTE_NONE));
quickmap_add(tll_length, gensym("whole"),                 (void *)(TLL_BIAS + PIZ_WHOLE_NOTE));
quickmap_add(tll_length, gensym("half"),                  (void *)(TLL_BIAS + PIZ_HALF_NOTE));
quickmap_add(tll_length, gensym("quarter"),               (void *)(TLL_BIAS + PIZ_QUARTER_NOTE));
quickmap_add(tll_length, gensym("eighth"),                (void *)(TLL_BIAS + PIZ_EIGHTH_NOTE));
quickmap_add(tll_length, gensym("sixteenth"),             (void *)(TLL_BIAS + PIZ_SIXTEENTH_NOTE));
quickmap_add(tll_length, gensym("thirty-second"),         (void *)(TLL_BIAS + PIZ_THIRTY_SECOND_NOTE));
quickmap_add(tll_length, gensym("whole_triplet"),         (void *)(TLL_BIAS + PIZ_WHOLE_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("half_triplet"),          (void *)(TLL_BIAS + PIZ_HALF_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("quarter_triplet"),       (void *)(TLL_BIAS + PIZ_QUARTER_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("eighth_triplet"),        (void *)(TLL_BIAS + PIZ_EIGHTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("sixteenth_triplet"),     (void *)(TLL_BIAS + PIZ_SIXTEENTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("thirty-second_triplet"), (void *)(TLL_BIAS + PIZ_THIRTY_SECOND_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("whole_dotted"),          (void *)(TLL_BIAS + PIZ_WHOLE_NOTE_DOTTED));
quickmap_add(tll_length, gensym("half_dotted"),           (void *)(TLL_BIAS + PIZ_HALF_NOTE_DOTTED));
quickmap_add(tll_length, gensym("quarter_dotted"),        (void *)(TLL_BIAS + PIZ_QUARTER_NOTE_DOTTED));
quickmap_add(tll_length, gensym("eighth_dotted"),         (void *)(TLL_BIAS + PIZ_EIGHTH_NOTE_DOTTED));
quickmap_add(tll_length, gensym("sixteenth_dotted"),      (void *)(TLL_BIAS + PIZ_SIXTEENTH_NOTE_DOTTED));

quickmap_add(tll_direction, gensym("up"),                 (void *)(TLL_BIAS + PIZ_UP)); 
quickmap_add(tll_direction, gensym("down"),               (void *)(TLL_BIAS + PIZ_DOWN));

quickmap_add(tll_select, gensym("pitch"),                 (void *)(TLL_BIAS + PIZ_VALUE_PITCH));
quickmap_add(tll_select, gensym("velocity"),              (void *)(TLL_BIAS + PIZ_VALUE_VELOCITY));
quickmap_add(tll_select, gensym("duration"),              (void *)(TLL_BIAS + PIZ_VALUE_DURATION));
quickmap_add(tll_select, gensym("channel"),               (void *)(TLL_BIAS + PIZ_VALUE_CHANNEL));

quickmap_add(tll_key, gensym("C"),                        (void *)(TLL_BIAS + PIZ_KEY_C));
quickmap_add(tll_key, gensym("C#"),                       (void *)(TLL_BIAS + PIZ_KEY_C_SHARP));
quickmap_add(tll_key, gensym("D"),                        (void *)(TLL_BIAS + PIZ_KEY_D));
quickmap_add(tll_key, gensym("D#"),                       (void *)(TLL_BIAS + PIZ_KEY_D_SHARP));
quickmap_add(tll_key, gensym("E"),                        (void *)(TLL_BIAS + PIZ_KEY_E));
quickmap_add(tll_key, gensym("F"),                        (void *)(TLL_BIAS + PIZ_KEY_F));
quickmap_add(tll_key, gensym("F#"),                       (void *)(TLL_BIAS + PIZ_KEY_F_SHARP));
quickmap_add(tll_key, gensym("G"),                        (void *)(TLL_BIAS + PIZ_KEY_G));
quickmap_add(tll_key, gensym("G#"),                       (void *)(TLL_BIAS + PIZ_KEY_G_SHARP));
quickmap_add(tll_key, gensym("A"),                        (void *)(TLL_BIAS + PIZ_KEY_A));
quickmap_add(tll_key, gensym("A#"),                       (void *)(TLL_BIAS + PIZ_KEY_A_SHARP));
quickmap_add(tll_key, gensym("B"),                        (void *)(TLL_BIAS + PIZ_KEY_B));

quickmap_add(tll_inform, gensym("bpm"),                   (void *)(TLL_BIAS + PIZ_INFORM_BPM));
quickmap_add(tll_inform, gensym("chance"),                (void *)(TLL_BIAS + PIZ_INFORM_CHANCE));
quickmap_add(tll_inform, gensym("velocity"),              (void *)(TLL_BIAS + PIZ_INFORM_VELOCITY));
quickmap_add(tll_inform, gensym("channel"),               (void *)(TLL_BIAS + PIZ_INFORM_CHANNEL));
quickmap_add(tll_inform, gensym("chord"),                 (void *)(TLL_BIAS + PIZ_INFORM_CHORD));
quickmap_add(tll_inform, gensym("cell"),                  (void *)(TLL_BIAS + PIZ_INFORM_CELL));
quickmap_add(tll_inform, gensym("value"),                 (void *)(TLL_BIAS + PIZ_INFORM_VALUE));
quickmap_add(tll_inform, gensym("scale"),                 (void *)(TLL_BIAS + PIZ_INFORM_SCALE));
quickmap_add(tll_inform, gensym("mute"),                  (void *)(TLL_BIAS + PIZ_INFORM_MUTE));
quickmap_add(tll_inform, gensym("zone"),                  (void *)(TLL_BIAS + PIZ_INFORM_ZONE));
quickmap_add(tll_inform, gensym("count"),                 (void *)(TLL_BIAS + PIZ_INFORM_COUNT));
    
quickmap_add(tll_notify, gensym("bpm"),                   (void *)(TLL_BIAS + PIZ_NOTIFY_BPM));
quickmap_add(tll_notify, gensym("chance"),                (void *)(TLL_BIAS + PIZ_NOTIFY_CHANCE));
quickmap_add(tll_notify, gensym("velocity"),              (void *)(TLL_BIAS + PIZ_NOTIFY_VELOCITY));
quickmap_add(tll_notify, gensym("channel"),               (void *)(TLL_BIAS + PIZ_NOTIFY_CHANNEL));
quickmap_add(tll_notify, gensym("chord"),                 (void *)(TLL_BIAS + PIZ_NOTIFY_CHORD));
quickmap_add(tll_notify, gensym("cell"),                  (void *)(TLL_BIAS + PIZ_NOTIFY_CELL));
quickmap_add(tll_notify, gensym("value"),                 (void *)(TLL_BIAS + PIZ_NOTIFY_VALUE));
quickmap_add(tll_notify, gensym("scale"),                 (void *)(TLL_BIAS + PIZ_NOTIFY_SCALE));
quickmap_add(tll_notify, gensym("mute"),                  (void *)(TLL_BIAS + PIZ_NOTIFY_MUTE));
quickmap_add(tll_notify, gensym("zone"),                  (void *)(TLL_BIAS + PIZ_NOTIFY_ZONE));
//
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseEntry(t_tll *x, t_symbol *s, long argc, t_atom *argv, ulong flags)
{
    long t;

    if (!(dictionary_getlong(tll_code, s, (long *)&t))) {
    //
    long k = 0;
    PIZEvent *event = NULL;
    PIZError err = PIZ_GOOD;
    long data[PIZ_EVENT_DATA_SIZE] = { 0 };
    PIZEventCode code = (PIZEventCode)(t - TLL_BIAS);
    
    if (code == PIZ_MSG_NOTE) {
        tralala_parseEntryNote(x, argc, argv, &k, data);
        
    } else if (code == PIZ_MSG_ZONE) {
        tralala_parseEntryZone(x, argc, argv, &k, data);
        
    } else if (!(flags & TLL_FLAG_FILTER)) {
        tralala_parseEntryCode(x, argc, argv, &k, data, code);
    
    } else {
        err = PIZ_ERROR;
    }
    
    if (!err && (event = pizEventNew(code))) {
        if (flags & TLL_FLAG_LOW) { pizEventSetType(event, PIZ_EVENT_LOW); } 
        else if (flags & TLL_FLAG_RUN) { pizEventSetType(event, PIZ_EVENT_RUN); }
        pizEventSetData(event, k, data);
        pizAgentDoEvent(x->agent, event);
    }
    //
    }
}

void tralala_parseEvent(t_tll *x, PIZEvent *event)
{
    long k;
    long *data = NULL;
    t_symbol *s = NULL;
    ulong dirty = TLL_FLAG_NONE;
    PIZEventCode code = PIZ_MSG_NONE;
        
    pizEventCode(event, &code);
    pizEventData(event, &k, &data);
        
    if (!(quickmap_lookup_key2(tll_notify, (void *)(code + TLL_BIAS), (void **)&s))) {
        dirty |= tralala_parseEventNotify(x, k, data, code, s);
        
    } else if (!(quickmap_lookup_key2(tll_inform, (void *)(code + TLL_BIAS), (void **)&s))) {
        tralala_parseEventInform(x, k, data, code, s);
        
    } else {
        dirty |= tralala_parseEventNote(x, k, data, code);
    }
        
    TLL_FLAG_SET(dirty);
    jbox_redraw((t_jbox *)x);
}

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
            tralala_parseEntry(x, atom_getsym(data), k - 1, data + 1, flags);
        }
    }
    //
    }
    
    dictionary_freekeys(d, n, keys);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_parseEntryNote(t_tll *x, long ac, t_atom *av, long *k, long *data)
{
    long i, temp;
    
    for (i = 0; i < ac; i++) {
        if ((*k) < PIZ_EVENT_DATA_SIZE) {
            if (atom_gettype(av + i) == A_LONG) {
                data[(*k)] = atom_getlong(av + i);
                (*k)++;
                
            } else if ((i == 1) && (atom_gettype(av + i) == A_SYM)) {
                if (!(tralala_pitchToLong(x, av + i, &temp))) {
                    data[(*k)] = temp;
                    (*k)++;
                }
            }
        }
    }
}

void tralala_parseEntryZone(t_tll *x, long ac, t_atom *av, long *k, long *data)
{
    long i, temp;
    
    for (i = 0; i < ac; i++) {
        if ((*k) < PIZ_EVENT_DATA_SIZE) {
            if (atom_gettype(av + i) == A_LONG) {
                data[(*k)] = atom_getlong(av + i);
                (*k)++;
                
            } else if (((i == 2) || (i == 3)) && (atom_gettype(av + i) == A_SYM)) {
                if (!(tralala_pitchToLong(x, av + i, &temp))) {
                    data[(*k)] = temp;
                    (*k)++;
                }
            }
        }
    }
}

void tralala_parseEntryCode(t_tll *x, long ac, t_atom *av, long *k, long *data, PIZEventCode code)
{        
    long i, msg = 0;
    
    msg |= (code == PIZ_MSG_ROTATE);
    msg |= (code == PIZ_MSG_SCRAMBLE);
    msg |= (code == PIZ_MSG_SORT);
    msg |= (code == PIZ_MSG_CHANGE);
    msg |= (code == PIZ_MSG_FILL);
    msg |= (code == PIZ_MSG_CYCLE);
    
    if (msg || (code == PIZ_MSG_TRANSPOSE)) {
        (*k) = 1;
    }
                     
    for (i = 0; i < ac; i++) {
    //
    if (atom_gettype(av + i) == A_SYM) {
    //
    long t, j = 0, p = 0;
     
    if ((code == PIZ_MSG_SCALE) || (code == PIZ_MSG_CYCLE)) {
        if (!(quickmap_lookup_key1(tll_type, (void *)atom_getsym(av + i), (void **)&t))) {
            j = t - TLL_BIAS; p = 1; 
        } else if (!(quickmap_lookup_key1(tll_key, (void *)atom_getsym(av + i), (void **)&t))) {
            j = t - TLL_BIAS;
        }
        
    } else if ((code == PIZ_MSG_CELL) || (code == PIZ_MSG_VALUE)) {
        if (!(quickmap_lookup_key1(tll_length, (void *)atom_getsym(av + i), (void **)&t))) {
            j = t - TLL_BIAS; p = 0;
        } else {
            continue;
        }
    
    } else if (msg) {
        if (!(quickmap_lookup_key1(tll_direction, (void *)atom_getsym(av + i), (void **)&t))) {
            j = t - TLL_BIAS; p = 1;
        } else if (!(quickmap_lookup_key1(tll_select, (void *)atom_getsym(av + i), (void **)&t))) {
            j = t - TLL_BIAS;
        }
    
    } else {
        continue;
    }

    data[p] = j;
    (*k) = MAX(p + 1, (*k));
    //
    }
    //
    }
    
    for (i = 0; i < ac; i++) {
        if (((*k) < PIZ_EVENT_DATA_SIZE) && (atom_gettype(av + i) == A_LONG)) {
            data[(*k)] = atom_getlong(av + i);
            (*k) ++;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

ulong tralala_parseEventNote(t_tll *x, long k, long *data, PIZEventCode code)
{
    t_symbol *s = NULL;
    ulong dirty = TLL_DIRTY_NOTE;
    
    dirty |= tralala_mouseAbort(x);
    tralala_symbolWithTag(&s, *(data + PIZ_EVENT_DATA_TAG));
      
    TLL_DATA_LOCK
    
    if (code == PIZ_NOTIFY_REMOVED) {
    //
    if (dictionary_hasentry(x->status, s)) {
    //
    t_symbol *mark = NULL;
    
    if (!(dictionary_getsym(x->status, TLL_SYM_MARK, &mark)) && (s == mark)) {
        dictionary_deleteentry(x->status, TLL_SYM_MARK); 
    }
    
    dictionary_deleteentry(x->status, s);
    //
    }
    
    dictionary_deleteentry(x->current, s);
    //
    } else if ((code == PIZ_NOTIFY_ADDED) || (code == PIZ_NOTIFY_CHANGED)) {
    //
    atom_setsym(x->note, TLL_SYM_NOTE);
    atom_setlong_array(5, x->note + 1, 5, data);
    dictionary_appendatoms(x->current, s, 6, x->note);
    
    if (*(data + PIZ_EVENT_DATA_LOW)) {
        dictionary_appendsym(x->status, TLL_SYM_MARK, s);
        dictionary_appendlong(x->status, s, TLL_SELECTED);
    }
    //
    }
    
    TLL_DATA_UNLOCK
    
    return dirty;
}

ulong tralala_parseEventNotify(t_tll *x, long k, long *data, PIZEventCode code, t_symbol *s)
{
    ulong dirty = TLL_FLAG_NONE;
    
    atom_setsym(x->notify, s);
    
    if (code == PIZ_NOTIFY_ZONE) { 
        dirty |= TLL_DIRTY_ZONE; 
    }
    
    if (code == PIZ_NOTIFY_SCALE) {
        t_symbol *sym1 = NULL;
        t_symbol *sym2 = NULL;

        quickmap_lookup_key2(tll_key,  (void *)((*(data + 0)) + TLL_BIAS), (void **)&sym1);
        quickmap_lookup_key2(tll_type, (void *)((*(data + 1)) + TLL_BIAS), (void **)&sym2);

        atom_setsym(x->notify + 1, sym1);
        atom_setsym(x->notify + 2, sym2);
        
    } else {
        atom_setlong_array(k, x->notify + 1, k, data);
    }
    
    TLL_DATA_LOCK
    dictionary_appendatoms(x->current, s, k + 1, x->notify); 
    TLL_DATA_UNLOCK
    
    outlet_anything(x->right, s, k, x->notify + 1);
    
    return dirty;
}

void tralala_parseEventInform(t_tll *x, long k, long *data, PIZEventCode code, t_symbol *s)
{    
    if (code == PIZ_INFORM_SCALE) {
        t_symbol *sym1 = NULL;
        t_symbol *sym2 = NULL;

        quickmap_lookup_key2(tll_key,  (void *)((*(data + 0)) + TLL_BIAS), (void **)&sym1);
        quickmap_lookup_key2(tll_type, (void *)((*(data + 1)) + TLL_BIAS), (void **)&sym2);

        atom_setsym(x->inform + 0, sym1);
        atom_setsym(x->inform + 1, sym2);
        
    } else {
        atom_setlong_array(k, x->inform, k, data);
    }
    
    outlet_anything(x->right, s, k, x->inform);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZError tralala_pitchToLong(t_tll *x, t_atom *a, long *t)
{
    long k;
    PIZError err = PIZ_ERROR;
    t_symbol *s = atom_getsym(a);
    
    if ((k = strlen(s->s_name)) <= 4) {
    //
    long i, p, n = 1, v = 4;
    char string[5];
    strncpy_zero(string, s->s_name, 5);
    
    for (i = 0; i < k; i++) {
        if (string[i] == '-') { 
            n = -1; string[i] = 0; 
            
        } else if ((string[i] >= '0') && (string[i] <= '8')) {
            v = string[i] - '0'; string[i] = 0; break;
        }
    }
    
    if (!(quickmap_lookup_key1(tll_key, (void *)gensym(string), (void **)&p))) {
        p -= TLL_BIAS;
        p += ((n * v) + 2) * 12;
        
        if ((p >= 0) && (p <= PIZ_MAGIC_PITCH)) {
            (*t) = p;
            err = PIZ_GOOD;
        }
    }
    //
    }
    
    return err;
}

void tralala_symbolWithTag(t_symbol **s, long tag)
{
    char string[4];
    snprintf_zero(string, 4, "%ld", tag);
    (*s) = gensym(string);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
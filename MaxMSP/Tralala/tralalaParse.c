/*
 *  tralalaParse.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"
#include "ext_quickmap.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_quickmap *tll_code;
static t_quickmap *tll_key;
static t_quickmap *tll_type;
static t_quickmap *tll_length;
static t_quickmap *tll_value;
static t_quickmap *tll_select;
static t_quickmap *tll_notification;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TINY 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark-

PIZ_INLINE t_symbol *tagToKey (long tag);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_parseInit ( )
{
//
tll_code         = quickmap_new ( );
tll_key          = quickmap_new ( );
tll_type         = quickmap_new ( );
tll_length       = quickmap_new ( );
tll_value        = quickmap_new ( );
tll_select       = quickmap_new ( );
tll_notification = quickmap_new ( );

quickmap_add (tll_code, gensym ("bpm"),                     (void *)(TINY + PIZ_EVENT_BPM));
quickmap_add (tll_code, gensym ("learn"),                   (void *)(TINY + PIZ_EVENT_LEARN));
quickmap_add (tll_code, gensym ("list"),                    (void *)(TINY + PIZ_EVENT_LEARN));
quickmap_add (tll_code, gensym ("forget"),                  (void *)(TINY + PIZ_EVENT_FORGET));
quickmap_add (tll_code, gensym ("dump"),                    (void *)(TINY + PIZ_EVENT_DUMP));
quickmap_add (tll_code, gensym ("chance"),                  (void *)(TINY + PIZ_EVENT_CHANCE));
quickmap_add (tll_code, gensym ("velocity"),                (void *)(TINY + PIZ_EVENT_VELOCITY));
quickmap_add (tll_code, gensym ("channel"),                 (void *)(TINY + PIZ_EVENT_CHANNEL));
quickmap_add (tll_code, gensym ("chord"),                   (void *)(TINY + PIZ_EVENT_CHORD));
quickmap_add (tll_code, gensym ("cell"),                    (void *)(TINY + PIZ_EVENT_CELL));
quickmap_add (tll_code, gensym ("value"),                   (void *)(TINY + PIZ_EVENT_VALUE));
quickmap_add (tll_code, gensym ("scale"),                   (void *)(TINY + PIZ_EVENT_SCALE));
quickmap_add (tll_code, gensym ("pattern"),                 (void *)(TINY + PIZ_EVENT_PATTERN));
quickmap_add (tll_code, gensym ("note"),                    (void *)(TINY + PIZ_EVENT_NOTE));
quickmap_add (tll_code, gensym ("zone"),                    (void *)(TINY + PIZ_EVENT_ZONE));
quickmap_add (tll_code, gensym ("clear"),                   (void *)(TINY + PIZ_EVENT_CLEAR));
quickmap_add (tll_code, gensym ("clean"),                   (void *)(TINY + PIZ_EVENT_CLEAN));
quickmap_add (tll_code, gensym ("rotate"),                  (void *)(TINY + PIZ_EVENT_ROTATE));
quickmap_add (tll_code, gensym ("scramble"),                (void *)(TINY + PIZ_EVENT_SCRAMBLE));
quickmap_add (tll_code, gensym ("sort"),                    (void *)(TINY + PIZ_EVENT_SORT));
quickmap_add (tll_code, gensym ("change"),                  (void *)(TINY + PIZ_EVENT_CHANGE));
quickmap_add (tll_code, gensym ("fill"),                    (void *)(TINY + PIZ_EVENT_FILL));
quickmap_add (tll_code, gensym ("kill"),                    (void *)(TINY + PIZ_EVENT_KILL));
quickmap_add (tll_code, gensym ("cycle"),                   (void *)(TINY + PIZ_EVENT_CYCLE));
quickmap_add (tll_code, gensym ("zoulou"),                  (void *)(TINY + PIZ_EVENT_ZOULOU));
quickmap_add (tll_code, gensym ("romeo"),                   (void *)(TINY + PIZ_EVENT_ROMEO));
quickmap_add (tll_code, gensym ("juliet"),                  (void *)(TINY + PIZ_EVENT_JULIET));

quickmap_add (tll_value,  gensym ("up"),                    (void *)(TINY + 0)); 
quickmap_add (tll_value,  gensym ("down"),                  (void *)(TINY + 1));
quickmap_add (tll_select, gensym ("pitch"),                 (void *)(TINY + PIZ_VALUE_PITCH));
quickmap_add (tll_select, gensym ("velocity"),              (void *)(TINY + PIZ_VALUE_VELOCITY));
quickmap_add (tll_select, gensym ("duration"),              (void *)(TINY + PIZ_VALUE_DURATION));
quickmap_add (tll_select, gensym ("channel"),               (void *)(TINY + PIZ_VALUE_CHANNEL));

quickmap_add (tll_key, gensym ("C"),                        (void *)(TINY + PIZ_KEY_C));
quickmap_add (tll_key, gensym ("C#"),                       (void *)(TINY + PIZ_KEY_C_SHARP));
quickmap_add (tll_key, gensym ("D"),                        (void *)(TINY + PIZ_KEY_D));
quickmap_add (tll_key, gensym ("D#"),                       (void *)(TINY + PIZ_KEY_D_SHARP));
quickmap_add (tll_key, gensym ("E"),                        (void *)(TINY + PIZ_KEY_E));
quickmap_add (tll_key, gensym ("F"),                        (void *)(TINY + PIZ_KEY_F));
quickmap_add (tll_key, gensym ("F#"),                       (void *)(TINY + PIZ_KEY_F_SHARP));
quickmap_add (tll_key, gensym ("G"),                        (void *)(TINY + PIZ_KEY_G));
quickmap_add (tll_key, gensym ("G#"),                       (void *)(TINY + PIZ_KEY_G_SHARP));
quickmap_add (tll_key, gensym ("A"),                        (void *)(TINY + PIZ_KEY_A));
quickmap_add (tll_key, gensym ("A#"),                       (void *)(TINY + PIZ_KEY_A_SHARP));
quickmap_add (tll_key, gensym ("B"),                        (void *)(TINY + PIZ_KEY_B));

quickmap_add (tll_type, gensym ("none"),                    (void *)(TINY + PIZ_NADA));
quickmap_add (tll_type, gensym ("ionian"),                  (void *)(TINY + PIZ_IONIAN));
quickmap_add (tll_type, gensym ("dorian"),                  (void *)(TINY + PIZ_DORIAN));
quickmap_add (tll_type, gensym ("phrygian"),                (void *)(TINY + PIZ_PHRYGIAN));
quickmap_add (tll_type, gensym ("lydian"),                  (void *)(TINY + PIZ_LYDIAN));
quickmap_add (tll_type, gensym ("mixolydian"),              (void *)(TINY + PIZ_MIXOLYDIAN));
quickmap_add (tll_type, gensym ("aeolian"),                 (void *)(TINY + PIZ_AEOLIAN));
quickmap_add (tll_type, gensym ("locrian"),                 (void *)(TINY + PIZ_LOCRIAN));
quickmap_add (tll_type, gensym ("natural"),                 (void *)(TINY + PIZ_NATURAL));
quickmap_add (tll_type, gensym ("harmonic"),                (void *)(TINY + PIZ_HARMONIC));
quickmap_add (tll_type, gensym ("melodic"),                 (void *)(TINY + PIZ_MELODIC));
quickmap_add (tll_type, gensym ("whole_tone"),              (void *)(TINY + PIZ_WHOLE_TONE));
quickmap_add (tll_type, gensym ("pentatonic_major"),        (void *)(TINY + PIZ_PENTATONIC_MAJOR));
quickmap_add (tll_type, gensym ("pentatonic_minor"),        (void *)(TINY + PIZ_PENTATONIC_MINOR));
quickmap_add (tll_type, gensym ("octatonic_half_whole"),    (void *)(TINY + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add (tll_type, gensym ("octatonic_whole_half"),    (void *)(TINY + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add (tll_type, gensym ("7th_major"),               (void *)(TINY + PIZ_SEVENTH_MAJOR));
quickmap_add (tll_type, gensym ("7th_minor"),               (void *)(TINY + PIZ_SEVENTH_MINOR));
quickmap_add (tll_type, gensym ("7th_dominant"),            (void *)(TINY + PIZ_SEVENTH_DOMINANT));
quickmap_add (tll_type, gensym ("7th_major_sixth"),         (void *)(TINY + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add (tll_type, gensym ("7th_minor_sixth"),         (void *)(TINY + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add (tll_type, gensym ("7th_half_diminished"),     (void *)(TINY + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add (tll_type, gensym ("7th_diminished"),          (void *)(TINY + PIZ_SEVENTH_DIMINISHED));
quickmap_add (tll_type, gensym ("7th_dominant_suspended"),  (void *)(TINY + PIZ_SEVENTH_SUSPENDED));
quickmap_add (tll_type, gensym ("7th_dominant_sharp_five"), (void *)(TINY + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add (tll_type, gensym ("7th_dominant_flat_five"),  (void *)(TINY + PIZ_SEVENTH_FLAT_FIVE));

quickmap_add (tll_length, gensym ("none"),                  (void *)(TINY + PIZ_NADA));
quickmap_add (tll_length, gensym ("whole"),                 (void *)(TINY + PIZ_WHOLE_NOTE));
quickmap_add (tll_length, gensym ("half"),                  (void *)(TINY + PIZ_HALF_NOTE));
quickmap_add (tll_length, gensym ("quarter"),               (void *)(TINY + PIZ_QUARTER_NOTE));
quickmap_add (tll_length, gensym ("eighth"),                (void *)(TINY + PIZ_EIGHTH_NOTE));
quickmap_add (tll_length, gensym ("sixteenth"),             (void *)(TINY + PIZ_SIXTEENTH_NOTE));
quickmap_add (tll_length, gensym ("thirty_second"),         (void *)(TINY + PIZ_THIRTY_SECOND_NOTE));
quickmap_add (tll_length, gensym ("whole_triplet"),         (void *)(TINY + PIZ_WHOLE_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("half_triplet"),          (void *)(TINY + PIZ_HALF_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("quarter_triplet"),       (void *)(TINY + PIZ_QUARTER_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("eighth_triplet"),        (void *)(TINY + PIZ_EIGHTH_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("sixteenth_triplet"),     (void *)(TINY + PIZ_SIXTEENTH_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("thirty_second_triplet"), (void *)(TINY + PIZ_THIRTY_SECOND_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("whole_dotted"),          (void *)(TINY + PIZ_WHOLE_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("half_dotted"),           (void *)(TINY + PIZ_HALF_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("quarter_dotted"),        (void *)(TINY + PIZ_QUARTER_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("eighth_dotted"),         (void *)(TINY + PIZ_EIGHTH_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("sixteenth_dotted"),      (void *)(TINY + PIZ_SIXTEENTH_NOTE_DOTTED));

quickmap_add (tll_notification, gensym ("bpm"),             (void *)(TINY + PIZ_EVENT_CHANGED_BPM));
quickmap_add (tll_notification, gensym ("chance"),          (void *)(TINY + PIZ_EVENT_CHANGED_CHANCE));
quickmap_add (tll_notification, gensym ("velocity"),        (void *)(TINY + PIZ_EVENT_CHANGED_VELOCITY));
quickmap_add (tll_notification, gensym ("channel"),         (void *)(TINY + PIZ_EVENT_CHANGED_CHANNEL));
quickmap_add (tll_notification, gensym ("chord"),           (void *)(TINY + PIZ_EVENT_CHANGED_CHORD));
quickmap_add (tll_notification, gensym ("cell"),            (void *)(TINY + PIZ_EVENT_CHANGED_CELL));
quickmap_add (tll_notification, gensym ("value"),           (void *)(TINY + PIZ_EVENT_CHANGED_VALUE));
quickmap_add (tll_notification, gensym ("scale"),           (void *)(TINY + PIZ_EVENT_CHANGED_SCALE));
quickmap_add (tll_notification, gensym ("pattern"),         (void *)(TINY + PIZ_EVENT_CHANGED_PATTERN)); 
//
}

void tralala_parseEventToDictionary (t_dictionary *d, PIZEvent *event)
{
    long         i, k = 0;
    long         *ptr;
    PIZEventCode code;
    
    pizEventCode (event, &code);
    pizEventData (event, &k, &ptr);
    
    if (ptr && k) {
    //
    t_atom   data[PIZ_EVENT_DATA_SIZE + 1];
    t_symbol *s = NULL;
    t_symbol *sym1 = NULL;
    t_symbol *sym2 = NULL;
        
    if (!(quickmap_lookup_key2 (tll_notification, (void *)(code + TINY), (void **)&s))) {
    //
    if (code == PIZ_EVENT_CHANGED_SCALE) {
        quickmap_lookup_key2 (tll_type, (void *)((*(ptr + 0)) + TINY), (void **)&sym1);
        quickmap_lookup_key2 (tll_key,  (void *)((*(ptr + 1)) + TINY), (void **)&sym2);
        
        atom_setsym (data + 1, sym1);
        atom_setsym (data + 2, sym2);
    
    } else {
        for (i = 0; i < k; i++) {
            atom_setlong (data + i + 1, *(ptr + i));
        }
    }
    //
    } else {
    //
    sym1 = tagToKey (event->tag);
    
    switch (code) {
        case PIZ_EVENT_NOTE_ADDED :
            break;
            
        case PIZ_EVENT_NOTE_CHANGED : 
            break;
            
        case PIZ_EVENT_NOTE_REMOVED : 
            break;
            
        default : 
            break;
    }
    //
    }
    
    if (s) {
        atom_setsym (data, s);
        dictionary_appendatoms (d, s, k + 1, data);
    }
    //
    }
}

void tralala_parseMessageToEvent (PIZEvent **event, t_symbol *s, long argc, t_atom *argv)
{
    long code = 0;
    
    if (!(quickmap_lookup_key1 (tll_code, (void *)s, (void **)&code))) {
    //
    long i, k = 0;
    long option = TINY;
    long data[PIZ_EVENT_DATA_SIZE];
     
    code -= TINY;
    
    for (i = 0; i < argc; i++) {
    //
    if ((atom_gettype (argv + i) == A_LONG) && (k < PIZ_EVENT_DATA_SIZE)) {
        data[k] = atom_getlong (argv + i);
        k ++;
        
    } else if (atom_gettype (argv + i) == A_SYM) {
    //
    long       temp;
    t_quickmap *d = NULL;
        
    if (!(quickmap_lookup_key1 (tll_select, (void *)atom_getsym (argv + i), (void **)&temp))) {
        option = temp;
    }
    
    if (!(quickmap_lookup_key1 (tll_key, (void *)atom_getsym (argv + i), (void **)&temp))) {
        option = temp;
    }
    
    switch (code) {
        case PIZ_EVENT_SORT  : d = tll_value;   break;
        case PIZ_EVENT_SCALE : d = tll_type;    break;
        case PIZ_EVENT_CELL  : d = tll_length;  break;
        case PIZ_EVENT_VALUE : d = tll_length;  break;
    }
    
    if (d && !(quickmap_lookup_key1 (d, (void *)atom_getsym (argv + i), (void **)data))) {
        data[0] -= TINY;
        k = MAX (1, k);
    } 
    //
    }
    //
    }
    
    option -= TINY;
    
    if ((*event) = pizEventNew (code)) {
        pizEventSetOption (*event, option);
        pizEventSetData   (*event, k, data);
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZ_INLINE t_symbol *tagToKey (long tag)
{
    char string[10];
    snprintf (string, 10, "note_%ld", tag);
    return (gensym (string));
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
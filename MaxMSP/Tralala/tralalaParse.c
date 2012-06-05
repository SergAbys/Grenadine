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

static t_quickmap   *tll_key;
static t_quickmap   *tll_mode;
static t_quickmap   *tll_length;
static t_quickmap   *tll_value;
static t_quickmap   *tll_select;

static t_dictionary *tll_code;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TINY 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_parseInit ( )
{
//
tll_key     = quickmap_new ( );
tll_mode    = quickmap_new ( );
tll_length  = quickmap_new ( );
tll_value   = quickmap_new ( );
tll_select  = quickmap_new ( );

tll_code    = dictionary_new ( );

dictionary_appendlong (tll_code, gensym ("bpm"),            PIZ_EVENT_BPM);
dictionary_appendlong (tll_code, gensym ("learn"),          PIZ_EVENT_LEARN);
dictionary_appendlong (tll_code, gensym ("list"),           PIZ_EVENT_LEARN);
dictionary_appendlong (tll_code, gensym ("forget"),         PIZ_EVENT_FORGET);
dictionary_appendlong (tll_code, gensym ("dump"),           PIZ_EVENT_DUMP);
dictionary_appendlong (tll_code, gensym ("chance"),         PIZ_EVENT_CHANCE);
dictionary_appendlong (tll_code, gensym ("velocity"),       PIZ_EVENT_VELOCITY);
dictionary_appendlong (tll_code, gensym ("channel"),        PIZ_EVENT_CHANNEL);
dictionary_appendlong (tll_code, gensym ("chord"),          PIZ_EVENT_CHORD);
dictionary_appendlong (tll_code, gensym ("cell"),           PIZ_EVENT_CELL);
dictionary_appendlong (tll_code, gensym ("value"),          PIZ_EVENT_VALUE);
dictionary_appendlong (tll_code, gensym ("scale"),          PIZ_EVENT_SCALE);
dictionary_appendlong (tll_code, gensym ("pattern"),        PIZ_EVENT_PATTERN);
dictionary_appendlong (tll_code, gensym ("note"),           PIZ_EVENT_NOTE);
dictionary_appendlong (tll_code, gensym ("zone"),           PIZ_EVENT_ZONE);
dictionary_appendlong (tll_code, gensym ("clear"),          PIZ_EVENT_CLEAR);
dictionary_appendlong (tll_code, gensym ("clean"),          PIZ_EVENT_CLEAN);
dictionary_appendlong (tll_code, gensym ("rotate"),         PIZ_EVENT_ROTATE);
dictionary_appendlong (tll_code, gensym ("scramble"),       PIZ_EVENT_SCRAMBLE);
dictionary_appendlong (tll_code, gensym ("sort"),           PIZ_EVENT_SORT);
dictionary_appendlong (tll_code, gensym ("change"),         PIZ_EVENT_CHANGE);
dictionary_appendlong (tll_code, gensym ("fill"),           PIZ_EVENT_FILL);
dictionary_appendlong (tll_code, gensym ("kill"),           PIZ_EVENT_KILL);
dictionary_appendlong (tll_code, gensym ("cycle"),          PIZ_EVENT_CYCLE);
dictionary_appendlong (tll_code, gensym ("zoulou"),         PIZ_EVENT_ZOULOU);
dictionary_appendlong (tll_code, gensym ("romeo"),          PIZ_EVENT_ROMEO);
dictionary_appendlong (tll_code, gensym ("juliet"),         PIZ_EVENT_JULIET);

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

quickmap_add (tll_mode, gensym ("none"),                    (void *)(TINY + PIZ_NADA));
quickmap_add (tll_mode, gensym ("ionian"),                  (void *)(TINY + PIZ_IONIAN));
quickmap_add (tll_mode, gensym ("dorian"),                  (void *)(TINY + PIZ_DORIAN));
quickmap_add (tll_mode, gensym ("phrygian"),                (void *)(TINY + PIZ_PHRYGIAN));
quickmap_add (tll_mode, gensym ("lydian"),                  (void *)(TINY + PIZ_LYDIAN));
quickmap_add (tll_mode, gensym ("mixolydian"),              (void *)(TINY + PIZ_MIXOLYDIAN));
quickmap_add (tll_mode, gensym ("aeolian"),                 (void *)(TINY + PIZ_AEOLIAN));
quickmap_add (tll_mode, gensym ("locrian"),                 (void *)(TINY + PIZ_LOCRIAN));
quickmap_add (tll_mode, gensym ("natural"),                 (void *)(TINY + PIZ_NATURAL));
quickmap_add (tll_mode, gensym ("harmonic"),                (void *)(TINY + PIZ_HARMONIC));
quickmap_add (tll_mode, gensym ("melodic"),                 (void *)(TINY + PIZ_MELODIC));
quickmap_add (tll_mode, gensym ("whole_tone"),              (void *)(TINY + PIZ_WHOLE_TONE));
quickmap_add (tll_mode, gensym ("pentatonic_major"),        (void *)(TINY + PIZ_PENTATONIC_MAJOR));
quickmap_add (tll_mode, gensym ("pentatonic_minor"),        (void *)(TINY + PIZ_PENTATONIC_MINOR));
quickmap_add (tll_mode, gensym ("octatonic_half_whole"),    (void *)(TINY + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add (tll_mode, gensym ("octatonic_whole_half"),    (void *)(TINY + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add (tll_mode, gensym ("7th_major"),               (void *)(TINY + PIZ_SEVENTH_MAJOR));
quickmap_add (tll_mode, gensym ("7th_minor"),               (void *)(TINY + PIZ_SEVENTH_MINOR));
quickmap_add (tll_mode, gensym ("7th_dominant"),            (void *)(TINY + PIZ_SEVENTH_DOMINANT));
quickmap_add (tll_mode, gensym ("7th_major_sixth"),         (void *)(TINY + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add (tll_mode, gensym ("7th_minor_sixth"),         (void *)(TINY + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add (tll_mode, gensym ("7th_half_diminished"),     (void *)(TINY + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add (tll_mode, gensym ("7th_diminished"),          (void *)(TINY + PIZ_SEVENTH_DIMINISHED));
quickmap_add (tll_mode, gensym ("7th_dominant_suspended"),  (void *)(TINY + PIZ_SEVENTH_SUSPENDED));
quickmap_add (tll_mode, gensym ("7th_dominant_sharp_five"), (void *)(TINY + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add (tll_mode, gensym ("7th_dominant_flat_five"),  (void *)(TINY + PIZ_SEVENTH_FLAT_FIVE));

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
//
}

PIZEvent *tralala_parseToEvent (t_symbol *s, long argc, t_atom *argv)
{
    long     code = 0;
    PIZEvent *event = NULL;
    
    if (!(dictionary_getlong (tll_code, s, &code))) {
    //
    long i, k = 0;
    long option = TINY;
    long data[PIZ_EVENT_DATA_SIZE];
     
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
        case PIZ_EVENT_SCALE : d = tll_mode;    break;
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
    
    if (event = pizEventNew (code)) {
        pizEventSetOption (event, option);
        pizEventSetData   (event, k, data);
    }
    //
    }
    
    return event;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
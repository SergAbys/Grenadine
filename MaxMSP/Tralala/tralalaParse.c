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
static t_quickmap *tll_value;
static t_quickmap *tll_select;
static t_quickmap *tll_key;
static t_quickmap *tll_mode;
static t_quickmap *tll_length;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define OFFSET 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_parseInit ( )
{
//
tll_code   = quickmap_new ( );
tll_value  = quickmap_new ( );
tll_select = quickmap_new ( );
tll_key    = quickmap_new ( );
tll_mode   = quickmap_new ( );
tll_length = quickmap_new ( );


quickmap_add (tll_code, gensym ("bpm"),                     (void *)(OFFSET + PIZ_EVENT_BPM));
quickmap_add (tll_code, gensym ("learn"),                   (void *)(OFFSET + PIZ_EVENT_LEARN));
quickmap_add (tll_code, gensym ("list"),                    (void *)(OFFSET + PIZ_EVENT_LEARN));
quickmap_add (tll_code, gensym ("forget"),                  (void *)(OFFSET + PIZ_EVENT_FORGET));
quickmap_add (tll_code, gensym ("dump"),                    (void *)(OFFSET + PIZ_EVENT_DUMP));
quickmap_add (tll_code, gensym ("chance"),                  (void *)(OFFSET + PIZ_EVENT_CHANCE));
quickmap_add (tll_code, gensym ("velocity"),                (void *)(OFFSET + PIZ_EVENT_VELOCITY));
quickmap_add (tll_code, gensym ("channel"),                 (void *)(OFFSET + PIZ_EVENT_CHANNEL));
quickmap_add (tll_code, gensym ("chord"),                   (void *)(OFFSET + PIZ_EVENT_CHORD));
quickmap_add (tll_code, gensym ("cell"),                    (void *)(OFFSET + PIZ_EVENT_CELL));
quickmap_add (tll_code, gensym ("value"),                   (void *)(OFFSET + PIZ_EVENT_VALUE));
quickmap_add (tll_code, gensym ("scale"),                   (void *)(OFFSET + PIZ_EVENT_SCALE));
quickmap_add (tll_code, gensym ("pattern"),                 (void *)(OFFSET + PIZ_EVENT_PATTERN));
quickmap_add (tll_code, gensym ("note"),                    (void *)(OFFSET + PIZ_EVENT_NOTE));
quickmap_add (tll_code, gensym ("zone"),                    (void *)(OFFSET + PIZ_EVENT_ZONE));
quickmap_add (tll_code, gensym ("clear"),                   (void *)(OFFSET + PIZ_EVENT_CLEAR));
quickmap_add (tll_code, gensym ("clean"),                   (void *)(OFFSET + PIZ_EVENT_CLEAN));
quickmap_add (tll_code, gensym ("rotate"),                  (void *)(OFFSET + PIZ_EVENT_ROTATE));
quickmap_add (tll_code, gensym ("scramble"),                (void *)(OFFSET + PIZ_EVENT_SCRAMBLE));
quickmap_add (tll_code, gensym ("sort"),                    (void *)(OFFSET + PIZ_EVENT_SORT));
quickmap_add (tll_code, gensym ("change"),                  (void *)(OFFSET + PIZ_EVENT_CHANGE));
quickmap_add (tll_code, gensym ("fill"),                    (void *)(OFFSET + PIZ_EVENT_FILL));
quickmap_add (tll_code, gensym ("kill"),                    (void *)(OFFSET + PIZ_EVENT_KILL));
quickmap_add (tll_code, gensym ("cycle"),                   (void *)(OFFSET + PIZ_EVENT_CYCLE));
quickmap_add (tll_code, gensym ("zoulou"),                  (void *)(OFFSET + PIZ_EVENT_ZOULOU));
quickmap_add (tll_code, gensym ("romeo"),                   (void *)(OFFSET + PIZ_EVENT_ROMEO));
quickmap_add (tll_code, gensym ("juliet"),                  (void *)(OFFSET + PIZ_EVENT_JULIET));

quickmap_add (tll_value,  gensym ("up"),                    (void *)(OFFSET + 0)); 
quickmap_add (tll_value,  gensym ("down"),                  (void *)(OFFSET + 1));
quickmap_add (tll_select, gensym ("pitch"),                 (void *)(OFFSET + PIZ_VALUE_PITCH));
quickmap_add (tll_select, gensym ("velocity"),              (void *)(OFFSET + PIZ_VALUE_VELOCITY));
quickmap_add (tll_select, gensym ("duration"),              (void *)(OFFSET + PIZ_VALUE_DURATION));
quickmap_add (tll_select, gensym ("channel"),               (void *)(OFFSET + PIZ_VALUE_CHANNEL));

quickmap_add (tll_key, gensym ("C"),                        (void *)(OFFSET + PIZ_KEY_C));
quickmap_add (tll_key, gensym ("C#"),                       (void *)(OFFSET + PIZ_KEY_C_SHARP));
quickmap_add (tll_key, gensym ("D"),                        (void *)(OFFSET + PIZ_KEY_D));
quickmap_add (tll_key, gensym ("D#"),                       (void *)(OFFSET + PIZ_KEY_D_SHARP));
quickmap_add (tll_key, gensym ("E"),                        (void *)(OFFSET + PIZ_KEY_E));
quickmap_add (tll_key, gensym ("F"),                        (void *)(OFFSET + PIZ_KEY_F));
quickmap_add (tll_key, gensym ("F#"),                       (void *)(OFFSET + PIZ_KEY_F_SHARP));
quickmap_add (tll_key, gensym ("G"),                        (void *)(OFFSET + PIZ_KEY_G));
quickmap_add (tll_key, gensym ("G#"),                       (void *)(OFFSET + PIZ_KEY_G_SHARP));
quickmap_add (tll_key, gensym ("A"),                        (void *)(OFFSET + PIZ_KEY_A));
quickmap_add (tll_key, gensym ("A#"),                       (void *)(OFFSET + PIZ_KEY_A_SHARP));
quickmap_add (tll_key, gensym ("B"),                        (void *)(OFFSET + PIZ_KEY_B));

quickmap_add (tll_mode, gensym ("none"),                    (void *)(OFFSET + PIZ_NADA));
quickmap_add (tll_mode, gensym ("ionian"),                  (void *)(OFFSET + PIZ_IONIAN));
quickmap_add (tll_mode, gensym ("dorian"),                  (void *)(OFFSET + PIZ_DORIAN));
quickmap_add (tll_mode, gensym ("phrygian"),                (void *)(OFFSET + PIZ_PHRYGIAN));
quickmap_add (tll_mode, gensym ("lydian"),                  (void *)(OFFSET + PIZ_LYDIAN));
quickmap_add (tll_mode, gensym ("mixolydian"),              (void *)(OFFSET + PIZ_MIXOLYDIAN));
quickmap_add (tll_mode, gensym ("aeolian"),                 (void *)(OFFSET + PIZ_AEOLIAN));
quickmap_add (tll_mode, gensym ("locrian"),                 (void *)(OFFSET + PIZ_LOCRIAN));
quickmap_add (tll_mode, gensym ("natural"),                 (void *)(OFFSET + PIZ_NATURAL));
quickmap_add (tll_mode, gensym ("harmonic"),                (void *)(OFFSET + PIZ_HARMONIC));
quickmap_add (tll_mode, gensym ("melodic"),                 (void *)(OFFSET + PIZ_MELODIC));
quickmap_add (tll_mode, gensym ("whole tone"),              (void *)(OFFSET + PIZ_WHOLE_TONE));
quickmap_add (tll_mode, gensym ("pentatonic major"),        (void *)(OFFSET + PIZ_PENTATONIC_MAJOR));
quickmap_add (tll_mode, gensym ("pentatonic minor"),        (void *)(OFFSET + PIZ_PENTATONIC_MINOR));
quickmap_add (tll_mode, gensym ("octatonic half/whole"),    (void *)(OFFSET + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add (tll_mode, gensym ("octatonic whole/half"),    (void *)(OFFSET + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add (tll_mode, gensym ("7th major"),               (void *)(OFFSET + PIZ_SEVENTH_MAJOR));
quickmap_add (tll_mode, gensym ("7th minor"),               (void *)(OFFSET + PIZ_SEVENTH_MINOR));
quickmap_add (tll_mode, gensym ("7th dominant"),            (void *)(OFFSET + PIZ_SEVENTH_DOMINANT));
quickmap_add (tll_mode, gensym ("7th major sixth"),         (void *)(OFFSET + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add (tll_mode, gensym ("7th minor sixth"),         (void *)(OFFSET + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add (tll_mode, gensym ("7th half-diminished"),     (void *)(OFFSET + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add (tll_mode, gensym ("7th diminished"),          (void *)(OFFSET + PIZ_SEVENTH_DIMINISHED));
quickmap_add (tll_mode, gensym ("7th dominant suspended"),  (void *)(OFFSET + PIZ_SEVENTH_SUSPENDED));
quickmap_add (tll_mode, gensym ("7th dominant sharp five"), (void *)(OFFSET + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add (tll_mode, gensym ("7th dominant flat five"),  (void *)(OFFSET + PIZ_SEVENTH_FLAT_FIVE));

quickmap_add (tll_length, gensym ("none"),                  (void *)(OFFSET + PIZ_NADA));
quickmap_add (tll_length, gensym ("whole"),                 (void *)(OFFSET + PIZ_WHOLE_NOTE));
quickmap_add (tll_length, gensym ("half"),                  (void *)(OFFSET + PIZ_HALF_NOTE));
quickmap_add (tll_length, gensym ("quarter"),               (void *)(OFFSET + PIZ_QUARTER_NOTE));
quickmap_add (tll_length, gensym ("eighth"),                (void *)(OFFSET + PIZ_EIGHTH_NOTE));
quickmap_add (tll_length, gensym ("sixteenth"),             (void *)(OFFSET + PIZ_SIXTEENTH_NOTE));
quickmap_add (tll_length, gensym ("thirty-second"),         (void *)(OFFSET + PIZ_THIRTY_SECOND_NOTE));
quickmap_add (tll_length, gensym ("whole triplet"),         (void *)(OFFSET + PIZ_WHOLE_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("half triplet"),          (void *)(OFFSET + PIZ_HALF_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("quarter triplet"),       (void *)(OFFSET + PIZ_QUARTER_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("eighth triplet"),        (void *)(OFFSET + PIZ_EIGHTH_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("sixteenth triplet"),     (void *)(OFFSET + PIZ_SIXTEENTH_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("thirty-second triplet"), (void *)(OFFSET + PIZ_THIRTY_SECOND_NOTE_TRIPLET));
quickmap_add (tll_length, gensym ("whole dotted"),          (void *)(OFFSET + PIZ_WHOLE_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("half dotted"),           (void *)(OFFSET + PIZ_HALF_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("quarter dotted"),        (void *)(OFFSET + PIZ_QUARTER_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("eighth dotted"),         (void *)(OFFSET + PIZ_EIGHTH_NOTE_DOTTED));
quickmap_add (tll_length, gensym ("sixteenth dotted"),      (void *)(OFFSET + PIZ_SIXTEENTH_NOTE_DOTTED));
//
}

PIZEvent *tralala_parseToEvent (t_symbol *s, long argc, t_atom *argv)
{
    long     code = 0;
    PIZEvent *event = NULL;
    
    if (!(quickmap_lookup_key1 (tll_code, (void *)s, (void **)&code))) {
    //
    long i, k = 0;
    long option = OFFSET;
    long data[PIZ_EVENT_DATA_SIZE];
    
    code -= OFFSET;
     
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
        data[0] -= OFFSET;
        k = MAX (1, k);
    } 
    //
    }
    //
    }
    
    option -= OFFSET;
    
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
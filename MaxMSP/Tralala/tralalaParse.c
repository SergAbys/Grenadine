/*
 *  tralalaParse.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_dictionary *tll_code;
static t_dictionary *tll_option;
static t_dictionary *tll_value;
static t_dictionary *tll_mode;
static t_dictionary *tll_length;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_parseInit ( )
{
    tll_code   = dictionary_new ( );
    tll_option = dictionary_new ( );
    tll_value  = dictionary_new ( );
    tll_mode   = dictionary_new ( );
    tll_length = dictionary_new ( );
    
    dictionary_appendlong (tll_code,    gensym ("bpm"),                     PIZ_EVENT_BPM); 
    dictionary_appendlong (tll_code,    gensym ("learn"),                   PIZ_EVENT_LEARN); 
    dictionary_appendlong (tll_code,    gensym ("list"),                    PIZ_EVENT_LEARN);
    dictionary_appendlong (tll_code,    gensym ("forget"),                  PIZ_EVENT_FORGET);
    dictionary_appendlong (tll_code,    gensym ("dump"),                    PIZ_EVENT_DUMP); 
    dictionary_appendlong (tll_code,    gensym ("chance"),                  PIZ_EVENT_CHANCE);
    dictionary_appendlong (tll_code,    gensym ("velocity"),                PIZ_EVENT_VELOCITY);
    dictionary_appendlong (tll_code,    gensym ("channel"),                 PIZ_EVENT_CHANNEL); 
    dictionary_appendlong (tll_code,    gensym ("chord"),                   PIZ_EVENT_CHORD); 
    dictionary_appendlong (tll_code,    gensym ("cell"),                    PIZ_EVENT_CELL);
    dictionary_appendlong (tll_code,    gensym ("value"),                   PIZ_EVENT_VALUE);
    dictionary_appendlong (tll_code,    gensym ("scale"),                   PIZ_EVENT_SCALE);
    dictionary_appendlong (tll_code,    gensym ("pattern"),                 PIZ_EVENT_PATTERN); 
    dictionary_appendlong (tll_code,    gensym ("note"),                    PIZ_EVENT_NOTE);
    dictionary_appendlong (tll_code,    gensym ("zone"),                    PIZ_EVENT_ZONE); 
    dictionary_appendlong (tll_code,    gensym ("clear"),                   PIZ_EVENT_CLEAR);
    dictionary_appendlong (tll_code,    gensym ("clean"),                   PIZ_EVENT_CLEAN);
    dictionary_appendlong (tll_code,    gensym ("rotate"),                  PIZ_EVENT_ROTATE); 
    dictionary_appendlong (tll_code,    gensym ("scramble"),                PIZ_EVENT_SCRAMBLE);
    dictionary_appendlong (tll_code,    gensym ("sort"),                    PIZ_EVENT_SORT);
    dictionary_appendlong (tll_code,    gensym ("change"),                  PIZ_EVENT_CHANGE); 
    dictionary_appendlong (tll_code,    gensym ("fill"),                    PIZ_EVENT_FILL); 
    dictionary_appendlong (tll_code,    gensym ("kill"),                    PIZ_EVENT_KILL);
    dictionary_appendlong (tll_code,    gensym ("cycle"),                   PIZ_EVENT_CYCLE);
    dictionary_appendlong (tll_code,    gensym ("zoulou"),                  PIZ_EVENT_ZOULOU); 
    dictionary_appendlong (tll_code,    gensym ("romeo"),                   PIZ_EVENT_ROMEO);
    dictionary_appendlong (tll_code,    gensym ("juliet"),                  PIZ_EVENT_JULIET); 
    
    dictionary_appendlong (tll_option,  gensym ("C"),                       PIZ_KEY_C); 
    dictionary_appendlong (tll_option,  gensym ("C#"),                      PIZ_KEY_C_SHARP); 
    dictionary_appendlong (tll_option,  gensym ("D"),                       PIZ_KEY_D); 
    dictionary_appendlong (tll_option,  gensym ("D#"),                      PIZ_KEY_D_SHARP); 
    dictionary_appendlong (tll_option,  gensym ("E"),                       PIZ_KEY_E);
    dictionary_appendlong (tll_option,  gensym ("F"),                       PIZ_KEY_F);
    dictionary_appendlong (tll_option,  gensym ("F#"),                      PIZ_KEY_F_SHARP);
    dictionary_appendlong (tll_option,  gensym ("G"),                       PIZ_KEY_G);
    dictionary_appendlong (tll_option,  gensym ("G#"),                      PIZ_KEY_G_SHARP);
    dictionary_appendlong (tll_option,  gensym ("A"),                       PIZ_KEY_A);
    dictionary_appendlong (tll_option,  gensym ("A#"),                      PIZ_KEY_A_SHARP);
    dictionary_appendlong (tll_option,  gensym ("B"),                       PIZ_KEY_B);
    dictionary_appendlong (tll_option,  gensym ("pitch"),                   PIZ_VALUE_PITCH); 
    dictionary_appendlong (tll_option,  gensym ("velocity"),                PIZ_VALUE_VELOCITY); 
    dictionary_appendlong (tll_option,  gensym ("duration"),                PIZ_VALUE_DURATION); 
    dictionary_appendlong (tll_option,  gensym ("channel"),                 PIZ_VALUE_CHANNEL); 

    dictionary_appendlong (tll_value,   gensym ("up"),                      0); 
    dictionary_appendlong (tll_value,   gensym ("down"),                    1);
    
    dictionary_appendlong (tll_mode,    gensym ("none"),                    PIZ_NADA);
    dictionary_appendlong (tll_mode,    gensym ("ionian"),                  PIZ_IONIAN);
    dictionary_appendlong (tll_mode,    gensym ("dorian"),                  PIZ_DORIAN);
    dictionary_appendlong (tll_mode,    gensym ("phrygian"),                PIZ_PHRYGIAN);
    dictionary_appendlong (tll_mode,    gensym ("lydian"),                  PIZ_LYDIAN);
    dictionary_appendlong (tll_mode,    gensym ("mixolydian"),              PIZ_MIXOLYDIAN);
    dictionary_appendlong (tll_mode,    gensym ("aeolian"),                 PIZ_AEOLIAN);
    dictionary_appendlong (tll_mode,    gensym ("locrian"),                 PIZ_LOCRIAN);
    dictionary_appendlong (tll_mode,    gensym ("natural"),                 PIZ_NATURAL);
    dictionary_appendlong (tll_mode,    gensym ("harmonic"),                PIZ_HARMONIC);
    dictionary_appendlong (tll_mode,    gensym ("melodic"),                 PIZ_MELODIC);
    dictionary_appendlong (tll_mode,    gensym ("whole tone"),              PIZ_WHOLE_TONE);
    dictionary_appendlong (tll_mode,    gensym ("pentatonic major"),        PIZ_PENTATONIC_MAJOR);
    dictionary_appendlong (tll_mode,    gensym ("pentatonic minor"),        PIZ_PENTATONIC_MINOR);
    dictionary_appendlong (tll_mode,    gensym ("octatonic half/whole"),    PIZ_OCTATONIC_HALF_WHOLE);
    dictionary_appendlong (tll_mode,    gensym ("octatonic whole/half"),    PIZ_OCTATONIC_WHOLE_HALF);
    dictionary_appendlong (tll_mode,    gensym ("7th major"),               PIZ_SEVENTH_MAJOR);
    dictionary_appendlong (tll_mode,    gensym ("7th minor"),               PIZ_SEVENTH_MINOR);
    dictionary_appendlong (tll_mode,    gensym ("7th dominant"),            PIZ_SEVENTH_DOMINANT);
    dictionary_appendlong (tll_mode,    gensym ("7th major sixth"),         PIZ_SEVENTH_MAJOR_SIXTH);
    dictionary_appendlong (tll_mode,    gensym ("7th minor sixth"),         PIZ_SEVENTH_MINOR_SIXTH);
    dictionary_appendlong (tll_mode,    gensym ("7th half-diminished"),     PIZ_SEVENTH_HALF_DIMINISHED);
    dictionary_appendlong (tll_mode,    gensym ("7th diminished"),          PIZ_SEVENTH_DIMINISHED);
    dictionary_appendlong (tll_mode,    gensym ("7th dominant suspended"),  PIZ_SEVENTH_SUSPENDED);
    dictionary_appendlong (tll_mode,    gensym ("7th dominant sharp five"), PIZ_SEVENTH_SHARP_FIVE);
    dictionary_appendlong (tll_mode,    gensym ("7th dominant flat five"),  PIZ_SEVENTH_FLAT_FIVE);
    
    dictionary_appendlong (tll_length,  gensym ("none"),                    PIZ_NADA);
    dictionary_appendlong (tll_length,  gensym ("whole"),                   PIZ_WHOLE_NOTE);
    dictionary_appendlong (tll_length,  gensym ("half"),                    PIZ_HALF_NOTE);
    dictionary_appendlong (tll_length,  gensym ("quarter"),                 PIZ_QUARTER_NOTE);
    dictionary_appendlong (tll_length,  gensym ("eighth"),                  PIZ_EIGHTH_NOTE);
    dictionary_appendlong (tll_length,  gensym ("sixteenth"),               PIZ_SIXTEENTH_NOTE);
    dictionary_appendlong (tll_length,  gensym ("thirty-second"),           PIZ_THIRTY_SECOND_NOTE);
    dictionary_appendlong (tll_length,  gensym ("whole triplet"),           PIZ_WHOLE_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("half triplet"),            PIZ_HALF_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("quarter triplet"),         PIZ_QUARTER_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("eighth triplet"),          PIZ_EIGHTH_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("sixteenth triplet"),       PIZ_SIXTEENTH_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("thirty-second triplet"),   PIZ_THIRTY_SECOND_NOTE_TRIPLET);
    dictionary_appendlong (tll_length,  gensym ("whole dotted"),            PIZ_WHOLE_NOTE_DOTTED);
    dictionary_appendlong (tll_length,  gensym ("half dotted"),             PIZ_HALF_NOTE_DOTTED);
    dictionary_appendlong (tll_length,  gensym ("quarter dotted"),          PIZ_QUARTER_NOTE_DOTTED);
    dictionary_appendlong (tll_length,  gensym ("eighth dotted"),           PIZ_EIGHTH_NOTE_DOTTED);
    dictionary_appendlong (tll_length,  gensym ("sixteenth dotted"),        PIZ_SIXTEENTH_NOTE_DOTTED);
}

PIZEvent *tralala_parseToEvent (t_symbol *s, long argc, t_atom *argv)
{
    long     code = 0;
    PIZEvent *event = NULL;
    
    if (!(dictionary_getlong (tll_code, s, &code))) {
    //
    long i, k = 0;
    long option = 0;
    long data[PIZ_EVENT_DATA_SIZE];
    
    for (i = 0; i < argc; i++) {
    //
    if ((atom_gettype (argv + i) == A_LONG) && (k < PIZ_EVENT_DATA_SIZE)) {
        data[k] = atom_getlong (argv + i);
        k ++;
        
    } else if (atom_gettype (argv + i) == A_SYM) {
    //
    long p = 0;
    t_dictionary *d = NULL;
        
    dictionary_getlong (tll_option, atom_getsym (argv + i), &option);
    
    switch (code) {
        case PIZ_EVENT_SORT  : d = tll_value;           break;
        case PIZ_EVENT_SCALE : d = tll_mode;            break;
        case PIZ_EVENT_CELL  : d = tll_length;          break;
        case PIZ_EVENT_VALUE : d = tll_length;          break;
        case PIZ_EVENT_NOTE  : d = tll_length;  p = 3;  break;
    }
    
    if (d) {
    //
    if ((!p) && !(dictionary_getlong (d, atom_getsym (argv + i), data))) {
        k = MAX (1, k);
        
    } else if ((k >= p) && !(dictionary_getlong (d, atom_getsym (argv + i), data + p))) {
        k = MAX (p + 1, k);
    }
    //
    }
    //
    }
    //
    }
    
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
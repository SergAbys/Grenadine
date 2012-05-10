/*
 *  tralalaSymbol.c
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  May 9, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaSymbol.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_symbol *tll_c                     = NULL;
static t_symbol *tll_cSharp                = NULL;
static t_symbol *tll_d                     = NULL;
static t_symbol *tll_dSharp                = NULL;
static t_symbol *tll_e                     = NULL;
static t_symbol *tll_f                     = NULL;
static t_symbol *tll_fSharp                = NULL;
static t_symbol *tll_g                     = NULL;
static t_symbol *tll_gSharp                = NULL;
static t_symbol *tll_a                     = NULL;
static t_symbol *tll_aSharp                = NULL;
static t_symbol *tll_b                     = NULL;
static t_symbol *tll_ionian                = NULL;
static t_symbol *tll_dorian                = NULL;
static t_symbol *tll_phrygian              = NULL;
static t_symbol *tll_lydian                = NULL;
static t_symbol *tll_mixolydian            = NULL;
static t_symbol *tll_aeolian               = NULL;
static t_symbol *tll_locrian               = NULL;
static t_symbol *tll_natural               = NULL;
static t_symbol *tll_harmonic              = NULL;
static t_symbol *tll_melodic               = NULL;
static t_symbol *tll_wholeTone             = NULL;
static t_symbol *tll_pentatonicMajor       = NULL;
static t_symbol *tll_pentatonicMinor       = NULL;
static t_symbol *tll_octatonicHalfWhole    = NULL;
static t_symbol *tll_octatonicWholeHalf    = NULL;
static t_symbol *tll_seventhMajor          = NULL;
static t_symbol *tll_seventhMinor          = NULL;
static t_symbol *tll_seventhDominant       = NULL;
static t_symbol *tll_seventhMajorSixth     = NULL;
static t_symbol *tll_seventhMinorSixth     = NULL;
static t_symbol *tll_seventhHalfDiminished = NULL;
static t_symbol *tll_seventhDiminished     = NULL;
static t_symbol *tll_seventhSuspended      = NULL;
static t_symbol *tll_seventhSharpFive      = NULL;
static t_symbol *tll_seventhFlatFive       = NULL;
static t_symbol *tll_whole                 = NULL;
static t_symbol *tll_half                  = NULL;
static t_symbol *tll_quarter               = NULL;
static t_symbol *tll_eighth                = NULL;
static t_symbol *tll_sixteenth             = NULL;
static t_symbol *tll_thirtySecond          = NULL;
static t_symbol *tll_wholeTriplet          = NULL;
static t_symbol *tll_halfTriplet           = NULL;
static t_symbol *tll_quarterTriplet        = NULL;
static t_symbol *tll_eighthTriplet         = NULL;
static t_symbol *tll_sixteenthTriplet      = NULL;
static t_symbol *tll_thirtySecondTriplet   = NULL;
static t_symbol *tll_wholeDotted           = NULL;
static t_symbol *tll_halfDotted            = NULL;
static t_symbol *tll_quarterDotted         = NULL;
static t_symbol *tll_eighthDotted          = NULL;
static t_symbol *tll_sixteenthDotted       = NULL;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralala_symbolsInit ( )
{
    tll_c                     = gensym ("C");
    tll_cSharp                = gensym ("C#");
    tll_d                     = gensym ("D");
    tll_dSharp                = gensym ("D#");
    tll_e                     = gensym ("E");
    tll_f                     = gensym ("F");
    tll_fSharp                = gensym ("F#");
    tll_g                     = gensym ("G");
    tll_gSharp                = gensym ("G#");
    tll_a                     = gensym ("A");
    tll_aSharp                = gensym ("A#");
    tll_b                     = gensym ("B");
    tll_ionian                = gensym ("ionian");
    tll_dorian                = gensym ("dorian");
    tll_phrygian              = gensym ("phrygian");
    tll_lydian                = gensym ("lydian");
    tll_mixolydian            = gensym ("mixolydian");
    tll_aeolian               = gensym ("aeolian");
    tll_locrian               = gensym ("locrian");
    tll_natural               = gensym ("natural");
    tll_harmonic              = gensym ("harmonic");
    tll_melodic               = gensym ("melodic");
    tll_wholeTone             = gensym ("whole tone");
    tll_pentatonicMajor       = gensym ("pentatonic major");
    tll_pentatonicMinor       = gensym ("pentatonic minor");
    tll_octatonicHalfWhole    = gensym ("octatonic half/whole");
    tll_octatonicWholeHalf    = gensym ("octatonic whole/half");
    tll_seventhMajor          = gensym ("7th major");
    tll_seventhMinor          = gensym ("7th minor");
    tll_seventhDominant       = gensym ("7th dominant");
    tll_seventhMajorSixth     = gensym ("7th major sixth");
    tll_seventhMinorSixth     = gensym ("7th minor sixth");
    tll_seventhHalfDiminished = gensym ("7th half-diminished");
    tll_seventhDiminished     = gensym ("7th diminished");
    tll_seventhSuspended      = gensym ("7th dominant suspended");
    tll_seventhSharpFive      = gensym ("7th dominant sharp five");
    tll_seventhFlatFive       = gensym ("7th dominant flat five");
    tll_whole                 = gensym ("whole");
    tll_half                  = gensym ("half");
    tll_quarter               = gensym ("quarter");
    tll_eighth                = gensym ("eighth");
    tll_sixteenth             = gensym ("sixteenth");
    tll_thirtySecond          = gensym ("thirty-second");
    tll_wholeTriplet          = gensym ("whole triplet");
    tll_halfTriplet           = gensym ("half triplet");
    tll_quarterTriplet        = gensym ("quarter triplet");
    tll_eighthTriplet         = gensym ("eighth triplet");
    tll_sixteenthTriplet      = gensym ("sixteenth triplet");
    tll_thirtySecondTriplet   = gensym ("thirty-second triplet");
    tll_wholeDotted           = gensym ("whole dotted");
    tll_halfDotted            = gensym ("half dotted");
    tll_quarterDotted         = gensym ("quarter dotted");
    tll_eighthDotted          = gensym ("eighth dotted");
    tll_sixteenthDotted       = gensym ("sixteenth dotted");
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError tralala_keyWithSymbol (const t_symbol *s, long *value)
{
    PIZError err = PIZ_ERROR;
    
    if (s && value) {
    //
    err = PIZ_GOOD;
    
    if      (s == tll_c)        { *value = PIZ_KEY_C;        } 
    else if (s == tll_cSharp)   { *value = PIZ_KEY_C_SHARP;  } 
    else if (s == tll_d)        { *value = PIZ_KEY_D;        } 
    else if (s == tll_dSharp)   { *value = PIZ_KEY_D_SHARP;  } 
    else if (s == tll_e)        { *value = PIZ_KEY_E;        } 
    else if (s == tll_f)        { *value = PIZ_KEY_F;        } 
    else if (s == tll_fSharp)   { *value = PIZ_KEY_F_SHARP;  }
    else if (s == tll_g)        { *value = PIZ_KEY_G;        } 
    else if (s == tll_gSharp)   { *value = PIZ_KEY_G_SHARP;  } 
    else if (s == tll_a)        { *value = PIZ_KEY_A;        } 
    else if (s == tll_aSharp)   { *value = PIZ_KEY_A_SHARP;  } 
    else if (s == tll_b)        { *value = PIZ_KEY_B;        } 
    
    else { err = PIZ_ERROR; }
    //
    }
            
    return err;
}

PIZError tralala_scaleWithSymbol (const t_symbol *s, long *value)
{
    PIZError err = PIZ_ERROR;
    
    if (s && value) {
    //
    
    err = PIZ_GOOD;
    
    if      (s == tll_ionian)                   { *value = PIZ_IONIAN;                   } 
    else if (s == tll_dorian)                   { *value = PIZ_DORIAN;                   } 
    else if (s == tll_phrygian)                 { *value = PIZ_PHRYGIAN;                 } 
    else if (s == tll_lydian)                   { *value = PIZ_LYDIAN;                   } 
    else if (s == tll_mixolydian)               { *value = PIZ_MIXOLYDIAN;               } 
    else if (s == tll_aeolian)                  { *value = PIZ_AEOLIAN;                  } 
    else if (s == tll_locrian)                  { *value = PIZ_LOCRIAN;                  }
    else if (s == tll_natural)                  { *value = PIZ_NATURAL;                  } 
    else if (s == tll_harmonic)                 { *value = PIZ_HARMONIC;                 } 
    else if (s == tll_melodic)                  { *value = PIZ_MELODIC;                  } 
    else if (s == tll_wholeTone)                { *value = PIZ_WHOLE_TONE;               } 
    else if (s == tll_pentatonicMajor)          { *value = PIZ_PENTATONIC_MAJOR;         } 
    else if (s == tll_pentatonicMinor)          { *value = PIZ_PENTATONIC_MINOR;         } 
    else if (s == tll_octatonicHalfWhole)       { *value = PIZ_OCTATONIC_HALF_WHOLE;     } 
    else if (s == tll_octatonicWholeHalf)       { *value = PIZ_OCTATONIC_WHOLE_HALF;     } 
    else if (s == tll_seventhMajor)             { *value = PIZ_SEVENTH_MAJOR;            } 
    else if (s == tll_seventhDominant)          { *value = PIZ_SEVENTH_DOMINANT;         } 
    else if (s == tll_seventhMinor)             { *value = PIZ_SEVENTH_MINOR;            } 
    else if (s == tll_seventhMajorSixth)        { *value = PIZ_SEVENTH_MAJOR_SIXTH;      } 
    else if (s == tll_seventhMinorSixth)        { *value = PIZ_SEVENTH_MINOR_SIXTH;      }
    else if (s == tll_seventhHalfDiminished)    { *value = PIZ_SEVENTH_HALF_DIMINISHED;  } 
    else if (s == tll_seventhDiminished)        { *value = PIZ_SEVENTH_DIMINISHED;       } 
    else if (s == tll_seventhSuspended)         { *value = PIZ_SEVENTH_SUSPENDED;        } 
    else if (s == tll_seventhSharpFive)         { *value = PIZ_SEVENTH_SHARP_FIVE;       } 
    else if (s == tll_seventhFlatFive)          { *value = PIZ_SEVENTH_FLAT_FIVE;        } 
    
    else { err = PIZ_ERROR; }
    //
    }
    
    return err;
}

PIZError tralala_noteValueWithSymbol (const t_symbol *s, long *value)
{
    PIZError err = PIZ_ERROR;
    
    if (s && value) {
    //
    
    err = PIZ_GOOD;
    
    if      (s == tll_whole)                { *value = PIZ_WHOLE_NOTE;                  } 
    else if (s == tll_half)                 { *value = PIZ_HALF_NOTE;                   } 
    else if (s == tll_quarter)              { *value = PIZ_QUARTER_NOTE;                } 
    else if (s == tll_eighth)               { *value = PIZ_EIGHTH_NOTE;                 } 
    else if (s == tll_sixteenth)            { *value = PIZ_SIXTEENTH_NOTE;              } 
    else if (s == tll_thirtySecond)         { *value = PIZ_THIRTY_SECOND_NOTE;          } 
    else if (s == tll_wholeTriplet)         { *value = PIZ_WHOLE_NOTE_TRIPLET;          } 
    else if (s == tll_halfTriplet)          { *value = PIZ_HALF_NOTE_TRIPLET;           } 
    else if (s == tll_quarterTriplet)       { *value = PIZ_QUARTER_NOTE_TRIPLET;        }
    else if (s == tll_eighthTriplet)        { *value = PIZ_EIGHTH_NOTE_TRIPLET;         }
    else if (s == tll_sixteenthTriplet)     { *value = PIZ_SIXTEENTH_NOTE_TRIPLET;      }
    else if (s == tll_thirtySecondTriplet)  { *value = PIZ_THIRTY_SECOND_NOTE_TRIPLET;  } 
    else if (s == tll_wholeDotted)          { *value = PIZ_WHOLE_NOTE_DOTTED;           } 
    else if (s == tll_halfDotted)           { *value = PIZ_HALF_NOTE_DOTTED;            } 
    else if (s == tll_quarterDotted)        { *value = PIZ_QUARTER_NOTE_DOTTED;         } 
    else if (s == tll_eighthDotted)         { *value = PIZ_EIGHTH_NOTE_DOTTED;          } 
    else if (s == tll_sixteenthDotted)      { *value = PIZ_SIXTEENTH_NOTE_DOTTED;       } 
    
    else { err = PIZ_ERROR; }
    //
    }
            
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
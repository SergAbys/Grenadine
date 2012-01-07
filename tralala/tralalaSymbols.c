/*
 *  tralalaSymbols.c
 *
 *  Created : 11/08/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *	Last modified : 07/01/12.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaSymbols.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralalaSymbolsTableAInit (tralalaSymbolsTableA *tll_symbolsA) 
{
	tll_symbolsA->sym_dump					= gensym ("dump");
	tll_symbolsA->sym_notes					= gensym ("notes");
	tll_symbolsA->sym_zone					= gensym ("zone");
	tll_symbolsA->sym_count					= gensym ("count");
	tll_symbolsA->sym_start					= gensym ("start");
	tll_symbolsA->sym_note					= gensym ("note");
	tll_symbolsA->sym_end					= gensym ("end");
	tll_symbolsA->sym_clear					= gensym ("clear");
	tll_symbolsA->sym_rotate				= gensym ("rotate");
	tll_symbolsA->sym_sort					= gensym ("sort");
	tll_symbolsA->sym_set					= gensym ("set");
	tll_symbolsA->sym_scramble				= gensym ("scramble");
	tll_symbolsA->sym_change				= gensym ("change");
	tll_symbolsA->sym_kill					= gensym ("kill");
	tll_symbolsA->sym_zoulou				= gensym ("zoulou");
	tll_symbolsA->sym_romeo					= gensym ("romeo");
	tll_symbolsA->sym_uniform				= gensym ("uniform");
	tll_symbolsA->sym_november				= gensym ("november");
	tll_symbolsA->sym_chance				= gensym ("chance");
	tll_symbolsA->sym_clean					= gensym ("clean");
	tll_symbolsA->sym_cycle					= gensym ("cycle");
	tll_symbolsA->sym_juliet				= gensym ("juliet");
	tll_symbolsA->sym_down					= gensym ("down");
	tll_symbolsA->sym_pitch					= gensym ("pitch");
	tll_symbolsA->sym_velocity				= gensym ("velocity");
	tll_symbolsA->sym_duration				= gensym ("duration");
	tll_symbolsA->sym_channel				= gensym ("channel");
	tll_symbolsA->sym_duple					= gensym ("duple");
	tll_symbolsA->sym_triple				= gensym ("triple");
	tll_symbolsA->sym_tempo					= gensym ("tempo");
	tll_symbolsA->sym_scaleKey				= gensym ("scalekey");
	tll_symbolsA->sym_scaleType				= gensym ("scaletype");
	tll_symbolsA->sym_scaleCustom			= gensym ("scalecustom");
	tll_symbolsA->sym_patternCell			= gensym ("patterncell");
	tll_symbolsA->sym_patternCustom			= gensym ("patterncustom");
	tll_symbolsA->sym_slots					= gensym ("slots");
	tll_symbolsA->sym_new					= gensym ("new");
	tll_symbolsA->sym_newCopy				= gensym ("newcopy");
	tll_symbolsA->sym_recall				= gensym ("recall");
	tll_symbolsA->sym_store					= gensym ("store");
	tll_symbolsA->sym_next					= gensym ("next");
	tll_symbolsA->sym_previous				= gensym ("previous");
	tll_symbolsA->sym_remove				= gensym ("remove");
	tll_symbolsA->sym_swap					= gensym ("swap");
	tll_symbolsA->sym_copy					= gensym ("copy");
	tll_symbolsA->sym_tralala				= gensym ("tralala");
	tll_symbolsA->sym_version				= gensym ("version");
	tll_symbolsA->sym_gridLayer				= gensym ("gridLayer");
	tll_symbolsA->sym_zoneLayer				= gensym ("zoneLayer");
	tll_symbolsA->sym_notesLayer			= gensym ("notesLayer");
	tll_symbolsA->sym_playedNotesLayer		= gensym ("playedNotesLayer");
	tll_symbolsA->sym_user					= gensym ("user");
	tll_symbolsA->sym_live					= gensym ("live");
	tll_symbolsA->sym_listen				= gensym ("listen");
	tll_symbolsA->sym_sequenceMode			= gensym ("sequencemode");
	tll_symbolsA->sym_zoomMode				= gensym ("zoommode");
	tll_symbolsA->sym_windowOffsetX			= gensym ("windowoffsetx");
	tll_symbolsA->sym_windowOffsetY			= gensym ("windowoffsety");
	tll_symbolsA->sym_dumpout				= gensym ("dumpout");
	tll_symbolsA->sym_attr_modified			= gensym ("attr_modified");
	tll_symbolsA->sym_getname				= gensym ("getname");
	tll_symbolsA->sym_patching_rect			= gensym ("patching_rect");
	tll_symbolsA->sym_modified				= gensym ("modified");
}

void tralalaSymbolsTableBInit (tralalaSymbolsTableB *tll_symbolsB) 
{
	tll_symbolsB->sym_none					= gensym ("none");
	tll_symbolsB->sym_ionian				= gensym ("ionian");
	tll_symbolsB->sym_dorian				= gensym ("dorian");
	tll_symbolsB->sym_phrygian				= gensym ("phrygian");
	tll_symbolsB->sym_lydian				= gensym ("lydian");
	tll_symbolsB->sym_mixolydian			= gensym ("mixolydian");
	tll_symbolsB->sym_aeolian				= gensym ("aeolian");
	tll_symbolsB->sym_locrian				= gensym ("locrian");
	tll_symbolsB->sym_natural				= gensym ("natural");
	tll_symbolsB->sym_harmonic				= gensym ("harmonic");
	tll_symbolsB->sym_melodic				= gensym ("melodic");
	tll_symbolsB->sym_wholeTone				= gensym ("whole tone");
	tll_symbolsB->sym_pentatonicMajor		= gensym ("pentatonic major");
	tll_symbolsB->sym_pentatonicMinor		= gensym ("pentatonic minor");
	tll_symbolsB->sym_octatonicHalfWhole	= gensym ("octatonic half/whole");
	tll_symbolsB->sym_octatonicWholeHalf	= gensym ("octatonic whole/half");
	tll_symbolsB->sym_seventhMajor			= gensym ("7th major");
	tll_symbolsB->sym_seventhMinor			= gensym ("7th minor");
	tll_symbolsB->sym_seventhDominant		= gensym ("7th dominant");
	tll_symbolsB->sym_seventhMajorSixth		= gensym ("7th major sixth");
	tll_symbolsB->sym_seventhMinorSixth		= gensym ("7th minor sixth");
	tll_symbolsB->sym_seventhHalfDiminished	= gensym ("7th half-diminished");
	tll_symbolsB->sym_seventhDiminished		= gensym ("7th diminished");
	tll_symbolsB->sym_seventhSuspended		= gensym ("7th dominant suspended");
	tll_symbolsB->sym_seventhSharpFive		= gensym ("7th dominant sharp five");
	tll_symbolsB->sym_seventhFlatFive		= gensym ("7th dominant flat five");
	tll_symbolsB->sym_custom				= gensym ("custom");
	tll_symbolsB->sym_c						= gensym ("C");
	tll_symbolsB->sym_cSharp				= gensym ("C#");
	tll_symbolsB->sym_d						= gensym ("D");
	tll_symbolsB->sym_dSharp				= gensym ("D#");
	tll_symbolsB->sym_e						= gensym ("E");
	tll_symbolsB->sym_f						= gensym ("F");
	tll_symbolsB->sym_fSharp				= gensym ("F#");
	tll_symbolsB->sym_g						= gensym ("G");
	tll_symbolsB->sym_gSharp				= gensym ("G#");
	tll_symbolsB->sym_a						= gensym ("A");
	tll_symbolsB->sym_aSharp				= gensym ("A#");
	tll_symbolsB->sym_b						= gensym ("B");
	tll_symbolsB->sym_automatic				= gensym ("automatic");
	tll_symbolsB->sym_whole					= gensym ("whole");
	tll_symbolsB->sym_half					= gensym ("half");
	tll_symbolsB->sym_quarter				= gensym ("quarter");
	tll_symbolsB->sym_eight					= gensym ("eight");
	tll_symbolsB->sym_sixteenth				= gensym ("sixteenth");
	tll_symbolsB->sym_thirtySecond			= gensym ("thirty-second");
	tll_symbolsB->sym_wholeTriplet			= gensym ("triplet whole");
	tll_symbolsB->sym_halfTriplet			= gensym ("triplet half");
	tll_symbolsB->sym_quarterTriplet		= gensym ("triplet quarter");
	tll_symbolsB->sym_eightTriplet			= gensym ("triplet eight");
	tll_symbolsB->sym_sixteenthTriplet		= gensym ("triplet sixteenth");
	tll_symbolsB->sym_thirtySecondTriplet	= gensym ("triplet thirty-second");
	tll_symbolsB->sym_dottedWhole			= gensym ("dotted whole");
	tll_symbolsB->sym_dottedHalf			= gensym ("dotted half");
	tll_symbolsB->sym_dottedQuarter			= gensym ("dotted quarter");
	tll_symbolsB->sym_dottedEight			= gensym ("dotted eight");
	tll_symbolsB->sym_dottedSixteenth		= gensym ("dotted sixteenth");
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
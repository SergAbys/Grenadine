/*
 *  tralalaSymbols.h
 *
 *  Created : 11/08/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *	Last modified : 24/12/11.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef TRALALA_SYMBOLS_H
#define TRALALA_SYMBOLS_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _tralalaSymbolsTableA {
	t_symbol *sym_dump;
	t_symbol *sym_notes;
	t_symbol *sym_zone;
	t_symbol *sym_count;
	t_symbol *sym_start;
	t_symbol *sym_note;
	t_symbol *sym_end;
	t_symbol *sym_clear;
	t_symbol *sym_rotate;
	t_symbol *sym_sort;
	t_symbol *sym_set;
	t_symbol *sym_scramble;
	t_symbol *sym_change;
	t_symbol *sym_kill;
	t_symbol *sym_zoulou;
	t_symbol *sym_romeo;
	t_symbol *sym_uniform;
	t_symbol *sym_november;
	t_symbol *sym_chance;	
	t_symbol *sym_clean;
	t_symbol *sym_cycle;
	t_symbol *sym_juliet;
	t_symbol *sym_down;
	t_symbol *sym_pitch;
	t_symbol *sym_velocity;
	t_symbol *sym_duration;
	t_symbol *sym_channel;
	t_symbol *sym_duple;
	t_symbol *sym_triple;
	t_symbol *sym_tempo;
	t_symbol *sym_scaleKey;
	t_symbol *sym_scaleType;
	t_symbol *sym_scaleCustom;
	t_symbol *sym_patternCell;
	t_symbol *sym_patternCustom;
	t_symbol *sym_slots;
	t_symbol *sym_new;
	t_symbol *sym_newCopy;
	t_symbol *sym_recall;
	t_symbol *sym_store;
	t_symbol *sym_next;
	t_symbol *sym_previous;
	t_symbol *sym_remove;
	t_symbol *sym_swap;
	t_symbol *sym_copy;
	t_symbol *sym_tralala;
	t_symbol *sym_version;
	t_symbol *sym_grid;
	t_symbol *sym_gridLayer;
	t_symbol *sym_zoneLayer;
	t_symbol *sym_notesLayer;
	t_symbol *sym_playedNotesLayer;
	t_symbol *sym_user;
	t_symbol *sym_live;
	t_symbol *sym_listen;
	t_symbol *sym_sequenceMode;
	t_symbol *sym_zoomMode;
	t_symbol *sym_windowOffsetX;
	t_symbol *sym_windowOffsetY;
	t_symbol *sym_dumpout;
	t_symbol *sym_attr_modified;
	t_symbol *sym_getname;
	t_symbol *sym_patching_rect;
	t_symbol *sym_modified;
	} tralalaSymbolsTableA;

typedef struct _tralalaSymbolsTableB {
	t_symbol *sym_none;
	t_symbol *sym_ionian;
	t_symbol *sym_dorian;
	t_symbol *sym_phrygian;
	t_symbol *sym_lydian;
	t_symbol *sym_mixolydian;
	t_symbol *sym_aeolian;
	t_symbol *sym_locrian;
	t_symbol *sym_natural;
	t_symbol *sym_harmonic;
	t_symbol *sym_melodic;
	t_symbol *sym_wholeTone;
	t_symbol *sym_pentatonicMajor;
	t_symbol *sym_pentatonicMinor;
	t_symbol *sym_octatonicHalfWhole;
	t_symbol *sym_octatonicWholeHalf;
	t_symbol *sym_seventhMajor;
	t_symbol *sym_seventhMinor;
	t_symbol *sym_seventhDominant;
	t_symbol *sym_seventhMajorSixth;
	t_symbol *sym_seventhMinorSixth;
	t_symbol *sym_seventhHalfDiminished;
	t_symbol *sym_seventhDiminished;
	t_symbol *sym_seventhSuspended;
	t_symbol *sym_seventhSharpFive;
	t_symbol *sym_seventhFlatFive;
	t_symbol *sym_custom;
	t_symbol *sym_c;
	t_symbol *sym_cSharp;
	t_symbol *sym_d;
	t_symbol *sym_dSharp;
	t_symbol *sym_e;
	t_symbol *sym_f;
	t_symbol *sym_fSharp;
	t_symbol *sym_g;
	t_symbol *sym_gSharp;
	t_symbol *sym_a;
	t_symbol *sym_aSharp;
	t_symbol *sym_b;
	t_symbol *sym_automatic;
	t_symbol *sym_whole;
	t_symbol *sym_half;
	t_symbol *sym_quarter;
	t_symbol *sym_eight;
	t_symbol *sym_sixteenth;
	t_symbol *sym_thirtySecond;
	t_symbol *sym_wholeTriplet;
	t_symbol *sym_halfTriplet;
	t_symbol *sym_quarterTriplet;
	t_symbol *sym_eightTriplet;
	t_symbol *sym_sixteenthTriplet;
	t_symbol *sym_thirtySecondTriplet;
	t_symbol *sym_dottedWhole;
	t_symbol *sym_dottedHalf;
	t_symbol *sym_dottedQuarter;
	t_symbol *sym_dottedEight;
	t_symbol *sym_dottedSixteenth;
	} tralalaSymbolsTableB;
	
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define tll_sym_dump					(tll_symbolsA.sym_dump)
#define tll_sym_notes					(tll_symbolsA.sym_notes)
#define tll_sym_zone					(tll_symbolsA.sym_zone)
#define tll_sym_count					(tll_symbolsA.sym_count)
#define tll_sym_start					(tll_symbolsA.sym_start)
#define tll_sym_note					(tll_symbolsA.sym_note)
#define tll_sym_end						(tll_symbolsA.sym_end)
#define tll_sym_clear					(tll_symbolsA.sym_clear)
#define tll_sym_rotate					(tll_symbolsA.sym_rotate)
#define tll_sym_sort					(tll_symbolsA.sym_sort)
#define tll_sym_set						(tll_symbolsA.sym_set)
#define tll_sym_scramble				(tll_symbolsA.sym_scramble)
#define tll_sym_change					(tll_symbolsA.sym_change)
#define tll_sym_kill					(tll_symbolsA.sym_kill)
#define tll_sym_zoulou					(tll_symbolsA.sym_zoulou)
#define tll_sym_romeo					(tll_symbolsA.sym_romeo)
#define tll_sym_uniform					(tll_symbolsA.sym_uniform)
#define tll_sym_november				(tll_symbolsA.sym_november)
#define tll_sym_chance					(tll_symbolsA.sym_chance)
#define tll_sym_clean					(tll_symbolsA.sym_clean)
#define tll_sym_cycle					(tll_symbolsA.sym_cycle)
#define tll_sym_juliet					(tll_symbolsA.sym_juliet)
#define tll_sym_down					(tll_symbolsA.sym_down)
#define tll_sym_pitch					(tll_symbolsA.sym_pitch)
#define tll_sym_velocity				(tll_symbolsA.sym_velocity)
#define tll_sym_duration				(tll_symbolsA.sym_duration)
#define tll_sym_channel					(tll_symbolsA.sym_channel)
#define tll_sym_duple					(tll_symbolsA.sym_duple)
#define tll_sym_triple					(tll_symbolsA.sym_triple)
#define tll_sym_tempo					(tll_symbolsA.sym_tempo)
#define tll_sym_scaleKey				(tll_symbolsA.sym_scaleKey)
#define tll_sym_scaleType				(tll_symbolsA.sym_scaleType)
#define tll_sym_scaleCustom				(tll_symbolsA.sym_scaleCustom)
#define tll_sym_patternCell				(tll_symbolsA.sym_patternCell)
#define tll_sym_patternCustom			(tll_symbolsA.sym_patternCustom)
#define tll_sym_slots					(tll_symbolsA.sym_slots)
#define tll_sym_new						(tll_symbolsA.sym_new)
#define tll_sym_newCopy					(tll_symbolsA.sym_newCopy)
#define tll_sym_recall					(tll_symbolsA.sym_recall)
#define tll_sym_store					(tll_symbolsA.sym_store)
#define tll_sym_next					(tll_symbolsA.sym_next)
#define tll_sym_previous				(tll_symbolsA.sym_previous)
#define tll_sym_remove					(tll_symbolsA.sym_remove)
#define tll_sym_swap					(tll_symbolsA.sym_swap)
#define tll_sym_copy					(tll_symbolsA.sym_copy)
#define tll_sym_tralala					(tll_symbolsA.sym_tralala)
#define tll_sym_version					(tll_symbolsA.sym_version)
#define tll_sym_grid					(tll_symbolsA.sym_grid)
#define tll_sym_gridLayer				(tll_symbolsA.sym_gridLayer)
#define tll_sym_zoneLayer				(tll_symbolsA.sym_zoneLayer)
#define tll_sym_notesLayer				(tll_symbolsA.sym_notesLayer)
#define tll_sym_playedNotesLayer		(tll_symbolsA.sym_playedNotesLayer)
#define tll_sym_user					(tll_symbolsA.sym_user)
#define tll_sym_live					(tll_symbolsA.sym_live)
#define tll_sym_listen					(tll_symbolsA.sym_listen)
#define tll_sym_sequenceMode			(tll_symbolsA.sym_sequenceMode)
#define tll_sym_zoomMode				(tll_symbolsA.sym_zoomMode)
#define tll_sym_windowOffsetX			(tll_symbolsA.sym_windowOffsetX)
#define tll_sym_windowOffsetY			(tll_symbolsA.sym_windowOffsetY)
#define tll_sym_dumpout					(tll_symbolsA.sym_dumpout)
#define tll_sym_attr_modified			(tll_symbolsA.sym_attr_modified)
#define tll_sym_getname					(tll_symbolsA.sym_getname)
#define tll_sym_patching_rect			(tll_symbolsA.sym_patching_rect)
#define tll_sym_modified				(tll_symbolsA.sym_modified)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define tll_sym_none					(tll_symbolsB.sym_none)
#define tll_sym_ionian					(tll_symbolsB.sym_ionian)
#define tll_sym_dorian					(tll_symbolsB.sym_dorian)
#define tll_sym_phrygian				(tll_symbolsB.sym_phrygian)
#define tll_sym_lydian					(tll_symbolsB.sym_lydian)
#define tll_sym_mixolydian				(tll_symbolsB.sym_mixolydian)
#define tll_sym_aeolian					(tll_symbolsB.sym_aeolian)
#define tll_sym_locrian					(tll_symbolsB.sym_locrian)
#define tll_sym_natural					(tll_symbolsB.sym_natural)
#define tll_sym_harmonic				(tll_symbolsB.sym_harmonic)
#define tll_sym_melodic					(tll_symbolsB.sym_melodic)
#define tll_sym_wholeTone				(tll_symbolsB.sym_wholeTone)
#define tll_sym_pentatonicMajor			(tll_symbolsB.sym_pentatonicMajor)
#define tll_sym_pentatonicMinor			(tll_symbolsB.sym_pentatonicMinor)
#define tll_sym_octatonicHalfWhole		(tll_symbolsB.sym_octatonicHalfWhole)
#define tll_sym_octatonicWholeHalf		(tll_symbolsB.sym_octatonicWholeHalf)
#define tll_sym_seventhMajor			(tll_symbolsB.sym_seventhMajor)
#define tll_sym_seventhMinor			(tll_symbolsB.sym_seventhMinor)
#define tll_sym_seventhDominant			(tll_symbolsB.sym_seventhDominant)
#define tll_sym_seventhMajorSixth		(tll_symbolsB.sym_seventhMajorSixth)
#define tll_sym_seventhMinorSixth		(tll_symbolsB.sym_seventhMinorSixth)
#define tll_sym_seventhHalfDiminished	(tll_symbolsB.sym_seventhHalfDiminished)
#define tll_sym_seventhDiminished		(tll_symbolsB.sym_seventhDiminished)
#define tll_sym_seventhSuspended		(tll_symbolsB.sym_seventhSuspended)
#define tll_sym_seventhSharpFive		(tll_symbolsB.sym_seventhSharpFive)
#define tll_sym_seventhFlatFive			(tll_symbolsB.sym_seventhFlatFive)
#define tll_sym_custom					(tll_symbolsB.sym_custom)
#define tll_sym_c						(tll_symbolsB.sym_c)
#define tll_sym_cSharp					(tll_symbolsB.sym_cSharp)
#define tll_sym_d						(tll_symbolsB.sym_d)
#define tll_sym_dSharp					(tll_symbolsB.sym_dSharp)
#define tll_sym_e						(tll_symbolsB.sym_e)
#define tll_sym_f						(tll_symbolsB.sym_f)
#define tll_sym_fSharp					(tll_symbolsB.sym_fSharp)
#define tll_sym_g						(tll_symbolsB.sym_g)
#define tll_sym_gSharp					(tll_symbolsB.sym_gSharp)
#define tll_sym_a						(tll_symbolsB.sym_a)
#define tll_sym_aSharp					(tll_symbolsB.sym_aSharp)
#define tll_sym_b						(tll_symbolsB.sym_b)
#define tll_sym_automatic				(tll_symbolsB.sym_automatic)
#define tll_sym_whole					(tll_symbolsB.sym_whole)
#define tll_sym_half					(tll_symbolsB.sym_half)
#define tll_sym_quarter					(tll_symbolsB.sym_quarter)
#define tll_sym_eight					(tll_symbolsB.sym_eight)
#define tll_sym_sixteenth				(tll_symbolsB.sym_sixteenth)
#define tll_sym_thirtySecond			(tll_symbolsB.sym_thirtySecond)
#define tll_sym_wholeTriplet			(tll_symbolsB.sym_wholeTriplet)
#define tll_sym_halfTriplet				(tll_symbolsB.sym_halfTriplet)
#define tll_sym_quarterTriplet			(tll_symbolsB.sym_quarterTriplet)
#define tll_sym_eightTriplet			(tll_symbolsB.sym_eightTriplet)
#define tll_sym_sixteenthTriplet		(tll_symbolsB.sym_sixteenthTriplet)
#define tll_sym_thirtySecondTriplet		(tll_symbolsB.sym_thirtySecondTriplet)
#define tll_sym_dottedWhole				(tll_symbolsB.sym_dottedWhole)
#define tll_sym_dottedHalf				(tll_symbolsB.sym_dottedHalf)
#define tll_sym_dottedQuarter			(tll_symbolsB.sym_dottedQuarter)
#define tll_sym_dottedEight				(tll_symbolsB.sym_dottedEight)
#define tll_sym_dottedSixteenth			(tll_symbolsB.sym_dottedSixteenth)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void tralalaSymbolsTableAInit (tralalaSymbolsTableA *tll_symbolsA);
void tralalaSymbolsTableBInit (tralalaSymbolsTableB *tll_symbolsB);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
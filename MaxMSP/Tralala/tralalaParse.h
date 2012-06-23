/*
 *  tralalaParse.h
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef TLL_TRALALA_PARSE_H
#define TLL_TRALALA_PARSE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tralalaSymbols {
    t_symbol *sym_note;
    t_symbol *sym_clear;
    t_symbol *sym_tralala;
    t_symbol *sym_current;
    t_symbol *sym_restore;
    } t_tralalaSymbols;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseInit (t_tralalaSymbols *table);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_NOTE    (tll_table.sym_note)
#define TLL_CLEAR   (tll_table.sym_clear)
#define TLL_TRALALA (tll_table.sym_tralala)
#define TLL_CURRENT (tll_table.sym_current)
#define TLL_RESTORE (tll_table.sym_restore)

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseDictionary    (t_tralala *x, t_dictionary *d);
void tralala_parseMessage       (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_parseNotification  (t_tralala *x, PIZEvent *event);
      
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_TRALALA_PARSE_H
/*
 *  tralalaParse.h
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef TLL_PARSE_H
#define TLL_PARSE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"
#include "ext_quickmap.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TINY 2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_SYM_END             (tll_table.end)
#define TLL_SYM_CLEAR           (tll_table.clear)
#define TLL_SYM_TRALALA         (tll_table.tralala)
#define TLL_SYM_UNTITLED        (tll_table.untitled)
#define TLL_SYM_LAST            (tll_table.last)
#define TLL_SYM_RUN             (tll_table.run)
#define TLL_SYM_NOTE            (tll_table.note)
#define TLL_SYM_TEXT            (tll_table.text)
#define TLL_SYM_ZONE            (tll_table.zone)
#define TLL_SYM_BACKGROUND      (tll_table.background)
#define TLL_SYM_XOFFSET         (tll_table.xoffset)
#define TLL_SYM_YOFFSET         (tll_table.yoffset)
#define TLL_SYM_COLOR           (tll_table.color)
#define TLL_SYM_HCOLOR1         (tll_table.hcolor1)
#define TLL_SYM_HCOLOR2         (tll_table.hcolor2)
#define TLL_SYM_ATTR_MODIFIED   (tll_table.attr_modified)
#define TLL_SYM_GETNAME         (tll_table.getname)
#define TLL_SYM_PATCHING_RECT   (tll_table.patching_rect)
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
typedef struct _tllSymbols {
    t_symbol *end;
    t_symbol *clear;
    t_symbol *tralala;
    t_symbol *untitled;
    t_symbol *last;
    t_symbol *run;
    t_symbol *note;
    t_symbol *text;
    t_symbol *zone;
    t_symbol *background;
    t_symbol *xoffset;
    t_symbol *yoffset;
    t_symbol *color;
    t_symbol *hcolor1;
    t_symbol *hcolor2;
    t_symbol *attr_modified;
    t_symbol *getname;
    t_symbol *patching_rect;
    } t_tllSymbols;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseInit          (t_tllSymbols *table);
void tralala_parseDictionary    (t_tll *x, t_dictionary *d);
void tralala_parseMessage       (t_tll *x, t_symbol *s, long argc, t_atom *argv);
void tralala_parseNotification  (t_tll *x, PIZEvent *event);
          
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_PARSE_H
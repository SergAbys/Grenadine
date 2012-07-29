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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_SYM_BPM             (tll_table.bpm)
#define TLL_SYM_CHANCE          (tll_table.chance)
#define TLL_SYM_VELOCITY        (tll_table.velocity)
#define TLL_SYM_CHANNEL         (tll_table.channel)
#define TLL_SYM_CHORD           (tll_table.chord)
#define TLL_SYM_CELL            (tll_table.cell)
#define TLL_SYM_VALUE           (tll_table.value)
#define TLL_SYM_SCALE           (tll_table.scale)
#define TLL_SYM_END             (tll_table.end)
#define TLL_SYM_CLEAR           (tll_table.clear)
#define TLL_SYM_TRALALA         (tll_table.tralala)
#define TLL_SYM_UNTITLED        (tll_table.untitled)
#define TLL_SYM_LOAD            (tll_table.load)
#define TLL_SYM_IDENTIFIER      (tll_table.identifier)
#define TLL_SYM_MARK            (tll_table.mark)
#define TLL_SYM_RUN             (tll_table.run)
#define TLL_SYM_NOTE            (tll_table.note)
#define TLL_SYM_TEXT            (tll_table.text)
#define TLL_SYM_ZONE            (tll_table.zone)
#define TLL_SYM_LASSO           (tll_table.lasso)
#define TLL_SYM_BACKGROUND      (tll_table.background)
#define TLL_SYM_XOFFSET         (tll_table.xoffset)
#define TLL_SYM_YOFFSET         (tll_table.yoffset)
#define TLL_SYM_COLOR           (tll_table.color)
#define TLL_SYM_HCOLOR1         (tll_table.hcolor1)
#define TLL_SYM_HCOLOR2         (tll_table.hcolor2)
#define TLL_SYM_UCOLOR          (tll_table.ucolor)
#define TLL_SYM_LASSOCOLOR      (tll_table.lassocolor)
#define TLL_SYM_ATTR_MODIFIED   (tll_table.attr_modified)
#define TLL_SYM_GETNAME         (tll_table.getname)
#define TLL_SYM_PATCHING_RECT   (tll_table.patching_rect)
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
typedef struct _tllSymbols {
    t_symbol *bpm;
    t_symbol *chance;
    t_symbol *velocity;
    t_symbol *channel;
    t_symbol *chord;
    t_symbol *cell;
    t_symbol *value;
    t_symbol *scale;
    t_symbol *end;
    t_symbol *clear;
    t_symbol *tralala;
    t_symbol *untitled;
    t_symbol *load;
    t_symbol *identifier;
    t_symbol *mark;
    t_symbol *run;
    t_symbol *note;
    t_symbol *text;
    t_symbol *zone;
    t_symbol *background;
    t_symbol *lasso;
    t_symbol *xoffset;
    t_symbol *yoffset;
    t_symbol *color;
    t_symbol *hcolor1;
    t_symbol *hcolor2;
    t_symbol *ucolor;
    t_symbol *lassocolor;
    t_symbol *attr_modified;
    t_symbol *getname;
    t_symbol *patching_rect;
    } t_tllSymbols;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TLL_FLAG_RUN        1UL
#define TLL_FLAG_LOW        2UL
#define TLL_FLAG_FILTER     4UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseInit          (t_tllSymbols *table);
void tralala_parseDictionary    (t_tll *x, t_dictionary *d, ulong flags);
void tralala_parseMessage       (t_tll *x, t_symbol *s, long argc, t_atom *argv, ulong flags);
void tralala_parseNotification  (t_tll *x, PIZEvent *event);
          
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // TLL_PARSE_H
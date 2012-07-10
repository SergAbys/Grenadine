/*
 *  tralalaLibrary.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaLibrary.h"
#include "ext_strings.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

extern t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_paintText            (t_tll *x, t_object *pv, char *string);
PIZ_LOCAL void tralala_paintZone            (t_tll *x, t_object *pv, long argc, t_atom *argv);
PIZ_LOCAL void tralala_paintNotes           (t_tll *x, t_object *pv, t_atomarray **notes);

PIZ_LOCAL void tralala_symbolWithTag        (t_symbol **s, long tag);
PIZ_LOCAL void tralala_tagWithSymbol        (long *tag, t_symbol *s);

PIZ_LOCAL void tralala_strncatZone          (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_strncatNote          (char *dst, long argc, t_atom *argv);
PIZ_LOCAL void tralala_strncatAttribute     (char *dst, long argc, t_atom *argv);

PIZ_LOCAL void tralala_pitchAsString        (char *s, long k, long size);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define POSITION_TO_X(a)    ((a) * TLL_PIXELS_PER_STEP)
#define PITCH_TO_Y_UP(a)    ((PIZ_MAGIC_PITCH - (a)) * TLL_PIXELS_PER_SEMITONE)
#define PITCH_TO_Y_DOWN(a)  ((PIZ_MAGIC_PITCH - (a) + 1) * TLL_PIXELS_PER_SEMITONE)
#define Y_TO_PITCH(a)       (PIZ_MAGIC_PITCH - ((long)((x->offsetY + (a)) / TLL_PIXELS_PER_SEMITONE)))
#define X_TO_POSITION(a)    ((long)((x->offsetX + (a)) / TLL_PIXELS_PER_STEP))

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TINY 2

#define TLL_MAXIMUM_STRING_SIZE     128
#define TLL_PIXELS_PER_STEP         1.
#define TLL_PIXELS_PER_SEMITONE     12.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static t_quickmap *tll_code;
static t_quickmap *tll_type;
static t_quickmap *tll_length;
static t_quickmap *tll_direction;
static t_quickmap *tll_select;
static t_quickmap *tll_key;
static t_quickmap *tll_notification;

void tralala_parseInit(t_tllSymbols *table)
{
//
tll_code         = (t_quickmap *)quickmap_new( );
tll_type         = (t_quickmap *)quickmap_new( );
tll_length       = (t_quickmap *)quickmap_new( );
tll_direction    = (t_quickmap *)quickmap_new( );
tll_key          = (t_quickmap *)quickmap_new( );
tll_select       = (t_quickmap *)quickmap_new( );
tll_notification = (t_quickmap *)quickmap_new( );

table->end           = gensym("end");
table->clear         = gensym("clear");
table->tralala       = gensym("tralala");
table->untitled      = gensym("untitled");
table->run           = gensym("run");
table->note          = gensym("note");
table->text          = gensym("text");
table->zone          = gensym("zone");
table->background    = gensym("background");
table->xoffset       = gensym("xoffset");
table->yoffset       = gensym("yoffset");
table->color         = gensym("color");
table->hcolor        = gensym("hcolor");
table->attr_modified = gensym("attr_modified");
table->getname       = gensym("getname");
table->patching_rect = gensym("patching_rect");

quickmap_add(tll_code, gensym("bpm"),                     (void *)(TINY + PIZ_EVENT_BPM));
quickmap_add(tll_code, gensym("learn"),                   (void *)(TINY + PIZ_EVENT_LEARN));
quickmap_add(tll_code, gensym("list"),                    (void *)(TINY + PIZ_EVENT_LEARN));
quickmap_add(tll_code, gensym("forget"),                  (void *)(TINY + PIZ_EVENT_FORGET));
quickmap_add(tll_code, gensym("dump"),                    (void *)(TINY + PIZ_EVENT_DUMP));
quickmap_add(tll_code, gensym("chance"),                  (void *)(TINY + PIZ_EVENT_CHANCE));
quickmap_add(tll_code, gensym("velocity"),                (void *)(TINY + PIZ_EVENT_VELOCITY));
quickmap_add(tll_code, gensym("channel"),                 (void *)(TINY + PIZ_EVENT_CHANNEL));
quickmap_add(tll_code, gensym("chord"),                   (void *)(TINY + PIZ_EVENT_CHORD));
quickmap_add(tll_code, gensym("cell"),                    (void *)(TINY + PIZ_EVENT_CELL));
quickmap_add(tll_code, gensym("value"),                   (void *)(TINY + PIZ_EVENT_VALUE));
quickmap_add(tll_code, gensym("scale"),                   (void *)(TINY + PIZ_EVENT_SCALE));
quickmap_add(tll_code, gensym("pattern"),                 (void *)(TINY + PIZ_EVENT_PATTERN));
quickmap_add(tll_code, gensym("note"),                    (void *)(TINY + PIZ_EVENT_NOTE));
quickmap_add(tll_code, gensym("zone"),                    (void *)(TINY + PIZ_EVENT_ZONE));
quickmap_add(tll_code, gensym("clear"),                   (void *)(TINY + PIZ_EVENT_CLEAR));
quickmap_add(tll_code, gensym("clean"),                   (void *)(TINY + PIZ_EVENT_CLEAN));
quickmap_add(tll_code, gensym("rotate"),                  (void *)(TINY + PIZ_EVENT_ROTATE));
quickmap_add(tll_code, gensym("scramble"),                (void *)(TINY + PIZ_EVENT_SCRAMBLE));
quickmap_add(tll_code, gensym("sort"),                    (void *)(TINY + PIZ_EVENT_SORT));
quickmap_add(tll_code, gensym("change"),                  (void *)(TINY + PIZ_EVENT_CHANGE));
quickmap_add(tll_code, gensym("fill"),                    (void *)(TINY + PIZ_EVENT_FILL));
quickmap_add(tll_code, gensym("kill"),                    (void *)(TINY + PIZ_EVENT_KILL));
quickmap_add(tll_code, gensym("cycle"),                   (void *)(TINY + PIZ_EVENT_CYCLE));
quickmap_add(tll_code, gensym("zoulou"),                  (void *)(TINY + PIZ_EVENT_ZOULOU));
quickmap_add(tll_code, gensym("romeo"),                   (void *)(TINY + PIZ_EVENT_ROMEO));
quickmap_add(tll_code, gensym("juliet"),                  (void *)(TINY + PIZ_EVENT_JULIET));

quickmap_add(tll_type, gensym("none"),                    (void *)(TINY + PIZ_MODE_NONE));
quickmap_add(tll_type, gensym("ionian"),                  (void *)(TINY + PIZ_IONIAN));
quickmap_add(tll_type, gensym("dorian"),                  (void *)(TINY + PIZ_DORIAN));
quickmap_add(tll_type, gensym("phrygian"),                (void *)(TINY + PIZ_PHRYGIAN));
quickmap_add(tll_type, gensym("lydian"),                  (void *)(TINY + PIZ_LYDIAN));
quickmap_add(tll_type, gensym("mixolydian"),              (void *)(TINY + PIZ_MIXOLYDIAN));
quickmap_add(tll_type, gensym("aeolian"),                 (void *)(TINY + PIZ_AEOLIAN));
quickmap_add(tll_type, gensym("locrian"),                 (void *)(TINY + PIZ_LOCRIAN));
quickmap_add(tll_type, gensym("natural"),                 (void *)(TINY + PIZ_NATURAL));
quickmap_add(tll_type, gensym("harmonic"),                (void *)(TINY + PIZ_HARMONIC));
quickmap_add(tll_type, gensym("melodic"),                 (void *)(TINY + PIZ_MELODIC));
quickmap_add(tll_type, gensym("whole_tone"),              (void *)(TINY + PIZ_WHOLE_TONE));
quickmap_add(tll_type, gensym("pentatonic_major"),        (void *)(TINY + PIZ_PENTATONIC_MAJOR));
quickmap_add(tll_type, gensym("pentatonic_minor"),        (void *)(TINY + PIZ_PENTATONIC_MINOR));
quickmap_add(tll_type, gensym("octatonic_half_whole"),    (void *)(TINY + PIZ_OCTATONIC_HALF_WHOLE));
quickmap_add(tll_type, gensym("octatonic_whole_half"),    (void *)(TINY + PIZ_OCTATONIC_WHOLE_HALF));
quickmap_add(tll_type, gensym("7th_major"),               (void *)(TINY + PIZ_SEVENTH_MAJOR));
quickmap_add(tll_type, gensym("7th_minor"),               (void *)(TINY + PIZ_SEVENTH_MINOR));
quickmap_add(tll_type, gensym("7th_dominant"),            (void *)(TINY + PIZ_SEVENTH_DOMINANT));
quickmap_add(tll_type, gensym("7th_major_sixth"),         (void *)(TINY + PIZ_SEVENTH_MAJOR_SIXTH));
quickmap_add(tll_type, gensym("7th_minor_sixth"),         (void *)(TINY + PIZ_SEVENTH_MINOR_SIXTH));
quickmap_add(tll_type, gensym("7th_half_diminished"),     (void *)(TINY + PIZ_SEVENTH_HALF_DIMINISHED));
quickmap_add(tll_type, gensym("7th_diminished"),          (void *)(TINY + PIZ_SEVENTH_DIMINISHED));
quickmap_add(tll_type, gensym("7th_dominant_suspended"),  (void *)(TINY + PIZ_SEVENTH_SUSPENDED));
quickmap_add(tll_type, gensym("7th_dominant_sharp_five"), (void *)(TINY + PIZ_SEVENTH_SHARP_FIVE));
quickmap_add(tll_type, gensym("7th_dominant_flat_five"),  (void *)(TINY + PIZ_SEVENTH_FLAT_FIVE));

quickmap_add(tll_length, gensym("none"),                  (void *)(TINY + PIZ_NOTE_NONE));
quickmap_add(tll_length, gensym("whole"),                 (void *)(TINY + PIZ_WHOLE_NOTE));
quickmap_add(tll_length, gensym("half"),                  (void *)(TINY + PIZ_HALF_NOTE));
quickmap_add(tll_length, gensym("quarter"),               (void *)(TINY + PIZ_QUARTER_NOTE));
quickmap_add(tll_length, gensym("eighth"),                (void *)(TINY + PIZ_EIGHTH_NOTE));
quickmap_add(tll_length, gensym("sixteenth"),             (void *)(TINY + PIZ_SIXTEENTH_NOTE));
quickmap_add(tll_length, gensym("thirty_second"),         (void *)(TINY + PIZ_THIRTY_SECOND_NOTE));
quickmap_add(tll_length, gensym("whole_triplet"),         (void *)(TINY + PIZ_WHOLE_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("half_triplet"),          (void *)(TINY + PIZ_HALF_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("quarter_triplet"),       (void *)(TINY + PIZ_QUARTER_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("eighth_triplet"),        (void *)(TINY + PIZ_EIGHTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("sixteenth_triplet"),     (void *)(TINY + PIZ_SIXTEENTH_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("thirty_second_triplet"), (void *)(TINY + PIZ_THIRTY_SECOND_NOTE_TRIPLET));
quickmap_add(tll_length, gensym("whole_dotted"),          (void *)(TINY + PIZ_WHOLE_NOTE_DOTTED));
quickmap_add(tll_length, gensym("half_dotted"),           (void *)(TINY + PIZ_HALF_NOTE_DOTTED));
quickmap_add(tll_length, gensym("quarter_dotted"),        (void *)(TINY + PIZ_QUARTER_NOTE_DOTTED));
quickmap_add(tll_length, gensym("eighth_dotted"),         (void *)(TINY + PIZ_EIGHTH_NOTE_DOTTED));
quickmap_add(tll_length, gensym("sixteenth_dotted"),      (void *)(TINY + PIZ_SIXTEENTH_NOTE_DOTTED));

quickmap_add(tll_direction, gensym("up"),                 (void *)(TINY + PIZ_UP)); 
quickmap_add(tll_direction, gensym("down"),               (void *)(TINY + PIZ_DOWN));

quickmap_add(tll_select, gensym("pitch"),                 (void *)(TINY + PIZ_VALUE_PITCH));
quickmap_add(tll_select, gensym("velocity"),              (void *)(TINY + PIZ_VALUE_VELOCITY));
quickmap_add(tll_select, gensym("duration"),              (void *)(TINY + PIZ_VALUE_DURATION));
quickmap_add(tll_select, gensym("channel"),               (void *)(TINY + PIZ_VALUE_CHANNEL));

quickmap_add(tll_key, gensym("C"),                        (void *)(TINY + PIZ_KEY_C));
quickmap_add(tll_key, gensym("C#"),                       (void *)(TINY + PIZ_KEY_C_SHARP));
quickmap_add(tll_key, gensym("D"),                        (void *)(TINY + PIZ_KEY_D));
quickmap_add(tll_key, gensym("D#"),                       (void *)(TINY + PIZ_KEY_D_SHARP));
quickmap_add(tll_key, gensym("E"),                        (void *)(TINY + PIZ_KEY_E));
quickmap_add(tll_key, gensym("F"),                        (void *)(TINY + PIZ_KEY_F));
quickmap_add(tll_key, gensym("F#"),                       (void *)(TINY + PIZ_KEY_F_SHARP));
quickmap_add(tll_key, gensym("G"),                        (void *)(TINY + PIZ_KEY_G));
quickmap_add(tll_key, gensym("G#"),                       (void *)(TINY + PIZ_KEY_G_SHARP));
quickmap_add(tll_key, gensym("A"),                        (void *)(TINY + PIZ_KEY_A));
quickmap_add(tll_key, gensym("A#"),                       (void *)(TINY + PIZ_KEY_A_SHARP));
quickmap_add(tll_key, gensym("B"),                        (void *)(TINY + PIZ_KEY_B));

quickmap_add(tll_notification, gensym("bpm"),             (void *)(TINY + PIZ_EVENT_CHANGED_BPM));
quickmap_add(tll_notification, gensym("chance"),          (void *)(TINY + PIZ_EVENT_CHANGED_CHANCE));
quickmap_add(tll_notification, gensym("velocity"),        (void *)(TINY + PIZ_EVENT_CHANGED_VELOCITY));
quickmap_add(tll_notification, gensym("channel"),         (void *)(TINY + PIZ_EVENT_CHANGED_CHANNEL));
quickmap_add(tll_notification, gensym("chord"),           (void *)(TINY + PIZ_EVENT_CHANGED_CHORD));
quickmap_add(tll_notification, gensym("cell"),            (void *)(TINY + PIZ_EVENT_CHANGED_CELL));
quickmap_add(tll_notification, gensym("value"),           (void *)(TINY + PIZ_EVENT_CHANGED_VALUE));
quickmap_add(tll_notification, gensym("scale"),           (void *)(TINY + PIZ_EVENT_CHANGED_SCALE));
quickmap_add(tll_notification, gensym("pattern"),         (void *)(TINY + PIZ_EVENT_CHANGED_PATTERN)); 
quickmap_add(tll_notification, gensym("zone"),            (void *)(TINY + PIZ_EVENT_CHANGED_ZONE));
//
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_parseDictionary(t_tll *x, t_dictionary *d)
{
    long i, n = 0;
    t_symbol **keys = NULL;
    
    if (!(dictionary_getkeys(d, &n, &keys))) {
    //
    for (i = 0; i < n; i++) {
    //
    long k;
    t_atom *data = NULL;
    
    if (!dictionary_getatoms(d, (*(keys + i)), &k, &data)) {
        if (atom_gettype(data) == A_SYM) {
            tralala_parseMessage(x, atom_getsym(data), k - 1, data + 1);
        }
    }
    //
    }
    
    dictionary_freekeys(d, n, keys);
    //
    }
}

void tralala_parseMessage(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    PIZTime time;
    PIZEventCode code = 0;
        
    if (!(quickmap_lookup_key1(tll_code, (void *)s, (void **)&code))) {
    //
    long i, k = 0, msg = 0;
    long data[PIZ_EVENT_DATA_SIZE] = { 0 };
    PIZEvent *event = NULL;
     
    code -= TINY;
    
    if (msg = ((code == PIZ_EVENT_ROTATE)   ||
               (code == PIZ_EVENT_SCRAMBLE) ||
               (code == PIZ_EVENT_SORT)     ||
               (code == PIZ_EVENT_CHANGE)   ||
               (code == PIZ_EVENT_FILL)     ||
               (code == PIZ_EVENT_CYCLE)))  { k = 1; }
                     
    for (i = 0; i < argc; i++) {
    //
    if (atom_gettype(argv + i) == A_SYM) {
    //
    long t, j = 0, p = 0;
     
    if ((code == PIZ_EVENT_SCALE) || (code == PIZ_EVENT_CYCLE)) {
        if (!(quickmap_lookup_key1(tll_type, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TINY; p = 1; 
        } else if (!(quickmap_lookup_key1(tll_key, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TINY;
        }
        
    } else if ((code == PIZ_EVENT_CELL) || (code == PIZ_EVENT_VALUE)) {
        if (!(quickmap_lookup_key1(tll_length, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TINY; p = 0;
        } else {
            continue;
        }
        
    } else if (msg) {
        if (!(quickmap_lookup_key1(tll_direction, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TINY; p = 1;
        } else if (!(quickmap_lookup_key1(tll_select, (void *)atom_getsym(argv + i), (void **)&t))) {
            j = t - TINY;
        }
        
    } else {
        continue;
    }

    data[p] = j;
    k = MAX(p + 1, k);
    //
    }
    //
    }
    
    for (i = 0; i < argc; i++) {
    //
    if ((k < PIZ_EVENT_DATA_SIZE) && (atom_gettype(argv + i) == A_LONG)) {
        data[k] = atom_getlong(argv + i);
        k++;
    }
    //
    }
    
    if (event = pizEventNew(code)) {
        pizTimeSet(&time);
        pizEventSetTime(event, &time);
        pizEventSetData(event, k, data);
        pizEventSetIdentifier(event, x->identifier);
        pizAgentDoEvent(x->agent, event);
    }
    //
    }
}

void tralala_parseNotification(t_tll *x, PIZEvent *event)
{
    long i, k = 0;
    long *ptr;
    t_atom data[PIZ_EVENT_DATA_SIZE + 1];
    t_symbol *s = NULL;
    PIZEventCode code;
    
    pizEventCode(event, &code);
    pizEventData(event, &k, &ptr);
    
    for (i = 0; i < k; i++) {
        atom_setlong(data + i + 1, *(ptr + i));
    }
    
    if (!(quickmap_lookup_key2(tll_notification, (void *)(code + TINY), (void **)&s))) {
    //
    if (code == PIZ_EVENT_CHANGED_SCALE) {
        t_symbol *sym1 = NULL;
        t_symbol *sym2 = NULL;

        quickmap_lookup_key2(tll_key,  (void *)((*(ptr + 0)) + TINY), (void **)&sym1);
        quickmap_lookup_key2(tll_type, (void *)((*(ptr + 1)) + TINY), (void **)&sym2);

        atom_setsym(data + 1, sym1);
        atom_setsym(data + 2, sym2);
    }

    atom_setsym(data, s);
    dictionary_appendatoms(x->current, s, k + 1, data);

    if (code == PIZ_EVENT_CHANGED_ZONE) {
        TLL_DIRTY_ZONE
    } 
    
    //
    } else {
    //
    tralala_symbolWithTag(&s, ptr[PIZ_EVENT_DATA_TAG]);
        
    if (code == PIZ_EVENT_NOTE_REMOVED) {
        if (dictionary_hasentry(x->status, s)) {
            dictionary_deleteentry(x->status, s);
        }
        
        dictionary_deleteentry(x->current, s);
        
    } else {
        atom_setsym(data, TLL_SYM_NOTE);
        dictionary_appendatoms(x->current, s, k - 1, data);
    }
    
    TLL_DIRTY_NOTE
    //
    }
    
    jbox_redraw((t_jbox *)x);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paintBackground(t_tll *x, t_object *pv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;

    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, w, h)) {
        jgraphics_set_source_jrgba(g, &x->color);
        jgraphics_rectangle_draw_fast(g, 0., 0., w, h, 1.);
        jbox_end_layer((t_object*)x, pv, TLL_SYM_BACKGROUND);
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_BACKGROUND, -x->offsetX, -x->offsetY);
}

void tralala_paintDictionary(t_tll *x, t_object *pv)
{
    long i, argc, n = 0;
    t_atom *argv = NULL;
    t_symbol *s = NULL;
    t_symbol **keys = NULL;
    t_atomarray *notes[2];
    
    char string[TLL_MAXIMUM_STRING_SIZE] = "";
    
    notes[0] = atomarray_new(0, NULL);
    notes[1] = atomarray_new(0, NULL);
    
    for (i = PIZ_EVENT_CHANGED_BPM; i < PIZ_EVENT_CHANGED_ZONE; i++) {
        if (!(quickmap_lookup_key2(tll_notification, (void *)(i + TINY), (void **)&s))) {
            if (!(dictionary_getatoms(x->current, s, &argc, &argv))) {
                tralala_strncatAttribute(string, argc - 1, argv + 1);
            }
        }
    }
    
    
    if (dictionary_hasentry(x->status, TLL_SYM_ZONE)) {
        if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
            tralala_strncatZone(string, argc - 1, argv + 1);
        } 
    }
    
    if (notes[0] && notes[1] && !(dictionary_getkeys(x->current, &n, &keys))) {
    //
    long max = 4;
    
    for (i = 0; i < n; i++) {
        if (!(dictionary_getatoms(x->current, (*(keys + i)), &argc, &argv))) { 
        //
        if ((atom_getsym(argv) == TLL_SYM_NOTE)) {
        
            if (dictionary_hasentry(x->status, (*(keys + i)))) {
                if (max) {
                    tralala_strncatNote(string, argc - 1, argv + 1);
                    max --;
                }
                atomarray_appendatoms(notes[0], argc - 1, argv + 1);
            } else {
                atomarray_appendatoms(notes[1], argc - 1, argv + 1);
            }
        }
        //
        }
    }
    
    dictionary_freekeys(x->current, n, keys);
    //
    }
    
    tralala_paintNotes(x, pv, notes);
    
    if (!(dictionary_getatoms(x->current, TLL_SYM_ZONE, &argc, &argv))) {
        tralala_paintZone(x, pv, argc - 1, argv + 1);
    }
    
    if (x->viewText) {
        tralala_paintText(x, pv, string);
    }
    
    object_free(notes[0]);
    object_free(notes[1]);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_paintText(t_tll *x, t_object *pv, char *string)
{
    t_rect rect;
    t_jgraphics *g = NULL; 
    t_jfont *font = NULL;
    t_jgraphics_text_justification justification = JGRAPHICS_TEXT_JUSTIFICATION_RIGHT;
    
    jbox_get_rect_for_view((t_object *)x, pv, &rect);
    
    g = (t_jgraphics *)patcherview_get_jgraphics(pv);
    font = jfont_create((jbox_get_fontname(((t_object *)x)))->s_name, 
                (jbox_get_font_slant((t_object *)x)), 
                (jbox_get_font_weight((t_object *)x)), 
                (jbox_get_fontsize((t_object *)x))); 
    
    jtextlayout_set(x->layer, string, font, 5., 5., rect.width - 10., rect.height - 5., justification, 0L);
    jtextlayout_settextcolor(x->layer, &x->text);    
    jtextlayout_draw(x->layer, g);
    
    jfont_destroy(font);
}

void tralala_paintZone(t_tll *x, t_object *pv, long argc, t_atom *argv)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_ZONE, w, h)) {
    //
    t_rect rect; 
    long zone[4];
    long status = 0;
    
    atom_getlong_array(argc, argv, 4, zone);
    
    if (!(dictionary_getlong(x->status, TLL_SYM_ZONE, &status)) && (status == TLL_SELECTED)) {
        jgraphics_set_source_jrgba(g, &x->hcolor);
    } else {
        jgraphics_set_source_jrgba(g, &x->color);
    }
    
    rect.x = POSITION_TO_X(zone[0]);
    rect.y = PITCH_TO_Y_UP(zone[3]); 
    rect.width  = POSITION_TO_X(zone[1]) - rect.x; 
    rect.height = PITCH_TO_Y_DOWN(zone[2]) - rect.y;
        
    jgraphics_rectangle_draw_fast(g, rect.x, rect.y, rect.width, rect.height, 1.);
    
    if (status > TLL_SELECTED) {
        switch (status) {
            case TLL_SELECTED_START : rect.x = POSITION_TO_X(zone[0]); rect.width = 3.; break ;
            case TLL_SELECTED_END   : rect.x = POSITION_TO_X(zone[1]) - 3.; rect.width = 3.; break ;
            case TLL_SELECTED_DOWN  : rect.y = PITCH_TO_Y_DOWN(zone[2]) - 3.; rect.height = 3.; break;
            case TLL_SELECTED_UP    : rect.height = 3.; break;
        }
        
        jgraphics_set_source_jrgba(g, &x->hcolor);
        jgraphics_rectangle_draw_fast(g, rect.x, rect.y, rect.width, rect.height, 1.);
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_ZONE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_ZONE, -x->offsetX, -x->offsetY);
}

void tralala_paintNotes(t_tll *x, t_object *pv, t_atomarray **notes)
{
    double w = (PIZ_SEQUENCE_SIZE_TIMELINE * TLL_PIXELS_PER_STEP);
    double h = ((PIZ_MAGIC_PITCH + 1) * TLL_PIXELS_PER_SEMITONE);
    t_jgraphics *g = NULL;
    
    if (g = jbox_start_layer((t_object *)x, pv, TLL_SYM_NOTE, w, h)) {
    //
    long i, argc;
    t_atom *argv = NULL;
    
    for (i = 0; i < 2; i++) {
    //
    if (!atomarray_getatoms(notes[i], &argc, &argv)) {
    //
    long j;
    for (j = 0; j < atomarray_getsize(notes[i]); j += 5) {
        t_rect rect;
        long note[5];
        
        atom_getlong_array(5, argv + j, 5, note);
        
        rect.x = POSITION_TO_X(note[0]);
        rect.y = PITCH_TO_Y_UP(note[1]); 
        rect.width  = POSITION_TO_X(note[0] + note[3]) - rect.x; 
        rect.height = PITCH_TO_Y_DOWN(note[1]) - rect.y;
    
        if (i) {
            jgraphics_set_source_jrgba(g, &x->color);
        } else {
            jgraphics_set_source_jrgba(g, &x->hcolor);
        }
        
        jgraphics_rectangle_fill_fast(g, rect.x, rect.y, rect.width, rect.height);
    }
    //
    }
    //
    }
    
    jbox_end_layer((t_object*)x, pv, TLL_SYM_NOTE);
    //
    }
        
    jbox_paint_layer((t_object *)x, pv, TLL_SYM_NOTE, -x->offsetX, -x->offsetY);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_symbolWithTag(t_symbol **s, long tag)
{
    char string[4];
    snprintf_zero(string, 4, "%ld", tag);
    (*s) = gensym(string);
}

void tralala_tagWithSymbol(long *tag, t_symbol *s)
{
    (*tag) = atoi(s->s_name);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -


void tralala_strncatZone(char *dst, long argc, t_atom *argv)
{
    char a[4];
    char b[4];
    char temp[16];
    
    tralala_pitchAsString(a, atom_getlong(argv + 2), 4);
    tralala_pitchAsString(b, atom_getlong(argv + 3), 4);
    snprintf_zero(temp, 16, "%ld %ld %s %s \n", atom_getlong(argv), atom_getlong(argv + 1), a, b);
    strncat_zero(dst, temp, TLL_MAXIMUM_STRING_SIZE);
}

void tralala_strncatNote(char *dst, long argc, t_atom *argv)
{
    char a[4];
    char temp[16];
    long p = atom_getlong(argv);
    long v = atom_getlong(argv + 2);
    long d = atom_getlong(argv + 3);
    long c = atom_getlong(argv + 4);
    
    tralala_pitchAsString(a, atom_getlong(argv + 1), 4);
    snprintf_zero(temp, 16, "%ld %s %ld %ld %ld \n", p, a, v, d, c);
    strncat_zero(dst, temp, TLL_MAXIMUM_STRING_SIZE);
}

void tralala_strncatAttribute(char *dst, long argc, t_atom *argv)
{
    long k = 0;
    char *p = NULL;
    
    if (!(atom_gettext(argc, argv, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE))) {
        strncat_zero(dst, p, TLL_MAXIMUM_STRING_SIZE);
        strncat_zero(dst, "\n", TLL_MAXIMUM_STRING_SIZE);
        sysmem_freeptr(p);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_pitchAsString(char *s, long k, long size)
{
    long m = k % 12;
    long n = ((long)(k / 12.)) - 2;
    
    switch (m) {
        case 0  : snprintf_zero(s, size, "%s%ld", "C",  n); break;
        case 1  : snprintf_zero(s, size, "%s%ld", "C#", n); break;
        case 2  : snprintf_zero(s, size, "%s%ld", "D",  n); break;
        case 3  : snprintf_zero(s, size, "%s%ld", "D#", n); break;
        case 4  : snprintf_zero(s, size, "%s%ld", "E",  n); break;
        case 5  : snprintf_zero(s, size, "%s%ld", "F",  n); break;
        case 6  : snprintf_zero(s, size, "%s%ld", "F#", n); break;
        case 7  : snprintf_zero(s, size, "%s%ld", "G",  n); break;
        case 8  : snprintf_zero(s, size, "%s%ld", "G#", n); break;
        case 9  : snprintf_zero(s, size, "%s%ld", "A",  n); break;
        case 10 : snprintf_zero(s, size, "%s%ld", "A#", n); break;
        case 11 : snprintf_zero(s, size, "%s%ld", "B",  n); break;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
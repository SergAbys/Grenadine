/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"  
#include "tralalaSymbols.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define TICKS_PER_STEP              20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define SEND(code)                  PIZEvent *event = NULL;                                 \
                                    if (event = pizEventNew ((code), -1, 0, NULL)) {        \
                                        pizAgentAddEvent (x->agent, event);                 \
                                    }
#define SEND_VALUE(code, n)         PIZEvent *event = NULL;                                 \
                                    if (event = pizEventNew ((code), -1, 1, &(n))) {        \
                                        pizAgentAddEvent (x->agent, event);                 \
                                    }
#define SEND_ARGS(code, ac, av)     PIZEvent *event = NULL;                                 \
                                    if (event = pizEventNew ((code), -1, (ac), (av))) {     \
                                        pizAgentAddEvent (x->agent, event);                 \
                                    }
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

long tralala_parse (long argc, t_atom *argv, long *values);
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_symbol *tll_end = NULL; 
static t_symbol *tll_willEnd = NULL; 
static t_class  *tralala_class = NULL;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

int main (void)
{	
    t_class	*c = NULL;
    
    c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

    class_addmethod (c, (method)tralala_assist,     "assist",       A_CANT, 0);
    class_addmethod (c, (method)tralala_bang,       "bang",         0);
    class_addmethod (c, (method)tralala_play,       "play",         0);
    class_addmethod (c, (method)tralala_stop,       "stop",         0);
    class_addmethod (c, (method)tralala_loop,       "loop",         0);
    class_addmethod (c, (method)tralala_unloop,     "unloop",       0);
    class_addmethod (c, (method)tralala_clear,      "clear",        0);
    class_addmethod (c, (method)tralala_kill,       "kill",         0);
    class_addmethod (c, (method)tralala_zoulou,     "zoulou",       0);
    class_addmethod (c, (method)tralala_romeo,      "romeo",        0);
    class_addmethod (c, (method)tralala_bpm,        "bpm",          A_LONG, 0);
    class_addmethod (c, (method)tralala_chance,     "chance",       A_LONG, 0);
    class_addmethod (c, (method)tralala_velocity,   "velocity",     A_LONG, 0);
    class_addmethod (c, (method)tralala_channel,    "channel",      A_LONG, 0);
    class_addmethod (c, (method)tralala_chord,      "chord",        A_LONG, 0);
    class_addmethod (c, (method)tralala_transpose,  "transpose",    A_LONG, 0);
    class_addmethod (c, (method)tralala_clean,      "clean",        A_LONG, 0);
    class_addmethod (c, (method)tralala_int,        "int",          A_LONG, 0);
    class_addmethod (c, (method)tralala_list,       "list",         A_GIMME, 0);
    class_addmethod (c, (method)tralala_cell,       "cell",         A_GIMME, 0);
    class_addmethod (c, (method)tralala_scale,      "scale",        A_GIMME, 0);
    class_addmethod (c, (method)tralala_pattern,    "pattern",      A_GIMME, 0);
    class_addmethod (c, (method)tralala_note,       "note",         A_GIMME, 0);
    class_addmethod (c, (method)tralala_rotate,     "rotate",       A_GIMME, 0);
    class_addmethod (c, (method)tralala_scramble,   "scramble",     A_GIMME, 0);
    class_addmethod (c, (method)tralala_sort,       "sort",         A_GIMME, 0);
    class_addmethod (c, (method)tralala_change,     "change",       A_GIMME, 0);
    class_addmethod (c, (method)tralala_fill,       "fill",         A_GIMME, 0);
    class_addmethod (c, (method)tralala_cycle,      "cycle",        A_GIMME, 0);
    class_addmethod (c, (method)tralala_november,   "november",     A_GIMME, 0);

    class_register (CLASS_BOX, c);

    tralala_class = c;
    
    tralala_symbolsInit ( );
    
    tll_end     = gensym ("end");
    tll_willEnd = gensym ("will");
        
    return 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new (t_symbol *s, long argc, t_atom *argv)
{
    t_tralala *x = NULL;

    if (x = (t_tralala *)object_alloc (tralala_class)) {
        if (x->agent = pizAgentNew ( )) {
        
            x->rightOutlet = outlet_new ((t_object *)x, NULL);
            x->leftOutlet  = listout ((t_object *)x);
            pizAgentAttach (x->agent, (void *)x, tralala_notify);
            
        } else {
            object_free (x);
            x = NULL;
        }
    }	
	
	return x;
}

void tralala_free (t_tralala *x)
{ 
    pizAgentDetach (x->agent, (void *)x);
    pizAgentFree   (x->agent);
}

void tralala_assist (t_tralala *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf (s, "Messages");
        
    } else {	
        switch (a) {
            case 0 : sprintf (s, "(List) Notes Played"); break;
            case 1 : sprintf (s, "(Anything) Messages"); break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_notify (void *ptr, PIZEvent *event)
{
    long         argc = 0;
    long         *argv = NULL;
    t_tralala    *x = NULL;
    PIZEventCode code;
    
    x = (t_tralala *)ptr;
    pizEventCode (event, &code);
    
    if (code == PIZ_EVENT_NOTE_PLAYED) {
        pizEventPtr (event, &argc, &argv);
        atom_setlong_array (4, x->a, argc - 1, argv + 1);
        outlet_list (x->leftOutlet, NULL, 4, x->a); 
    
    } else if (code == PIZ_EVENT_END) {
        outlet_anything (x->rightOutlet, tll_end, 0, NULL); 

    } else if (code == PIZ_EVENT_WILL_END) {
        outlet_anything (x->rightOutlet, tll_willEnd, 0, NULL);
    }
    
    DEBUGEVENT
    
    pizEventFree (event);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_bang (t_tralala *x) 
{   
    SEND (PIZ_EVENT_PLAY)
}

void tralala_play (t_tralala *x) 
{   
    SEND (PIZ_EVENT_PLAY)
}

void tralala_stop (t_tralala *x) 
{   
    SEND (PIZ_EVENT_STOP)
}

void tralala_loop (t_tralala *x) 
{   
    SEND (PIZ_EVENT_LOOP)
}

void tralala_unloop (t_tralala *x) 
{   
    SEND (PIZ_EVENT_UNLOOP)
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_bpm (t_tralala *x, long n) 
{   
    SEND_VALUE (PIZ_EVENT_BPM, n)
}

void tralala_chance (t_tralala *x, long n) 
{   
    SEND_VALUE (PIZ_EVENT_CHANCE, n)
}

void tralala_velocity (t_tralala *x, long n) 
{   
    SEND_VALUE (PIZ_EVENT_VELOCITY, n)
}

void tralala_channel (t_tralala *x, long n) 
{   
    SEND_VALUE (PIZ_EVENT_CHANNEL, n)
}

void tralala_chord (t_tralala *x, long n) 
{   
    SEND_VALUE (PIZ_EVENT_CHORD, n)
}

void tralala_cell (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc) {
    //
    long     n, size = 0;
    char     *string = NULL;
    PIZError err = PIZ_GOOD;

    atom_gettext (argc, argv, &size, &string, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
    
    if (string) {
        err = tralala_noteValueWithSymbol ((gensym (string)), &n);
        
        if (!err) {
            SEND_VALUE (PIZ_EVENT_CELL, n)
        }
        
        sysmem_freeptr (string);
    }
    //
    }
}

void tralala_scale (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc > 1) {
    //
    if (atom_gettype (argv) == A_SYM) {
        long     key, type, size = 0;
        char     *string = NULL;
        PIZError err = PIZ_GOOD;

        err |= tralala_keyWithSymbol (atom_getsym (argv), &key);
        
        atom_gettext (argc - 1, argv + 1, &size, &string, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (string) {
            err |= tralala_scaleWithSymbol ((gensym (string)), &type);
            sysmem_freeptr (string);
        }

        if (!err) {
            long values[ ] = { key, type };
            SEND_ARGS (PIZ_EVENT_SCALE, 2, values)
        }
    }
    //
    } else {
        SEND (PIZ_EVENT_SCALE)
    }
}

void tralala_pattern (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc) {
    //
    long values[argc];
    
    if (!(atom_getlong_array (argc, argv, argc, values))) {
        SEND_ARGS (PIZ_EVENT_PATTERN, argc, values)
    }
    //
    } else {
        SEND (PIZ_EVENT_PATTERN)
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_note (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc > 1) {
    //
    long values[ ] = { 0, 0, 0, 0, 0 };
    
    if (!(atom_getlong_array (argc, argv, 5, values))) {
        values[0] = (long)(values[0] / TICKS_PER_STEP);
        values[3] = (long)(values[3] / TICKS_PER_STEP);
        SEND_ARGS (PIZ_EVENT_NOTE, argc, values)
    }
    //
    }
}

void tralala_clear (t_tralala *x) 
{   
    SEND (PIZ_EVENT_CLEAR)
}

void tralala_clean (t_tralala *x, long n)
{
    SEND_VALUE (PIZ_EVENT_CLEAN, n)
}

void tralala_transpose (t_tralala *x, long n)
{
    SEND_VALUE (PIZ_EVENT_TRANSPOSE, n)
}

void tralala_rotate (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long values[ ] = { 1, PIZ_VALUE_PITCH };
    
    if (argc && argv) {
        tralala_parse (argc, argv, values);
    }
    
    SEND_ARGS (PIZ_EVENT_ROTATE, 2, values)
}

void tralala_scramble (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long values[ ] = { 0, PIZ_VALUE_PITCH };
    
    if (argc && argv) {
        tralala_parse (argc, argv, values);
    }
    
    SEND_ARGS (PIZ_EVENT_SCRAMBLE, 2, values)
}

void tralala_sort (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long values[ ] = { 0, PIZ_VALUE_PITCH };
    
    if (argc && argv) {
        tralala_parse (argc, argv, values);
    }
    
    SEND_ARGS (PIZ_EVENT_SORT, 2, values)
}

void tralala_change (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long values[ ] = { 0, PIZ_VALUE_PITCH };
    
    if (argc && argv && ((tralala_parse (argc, argv, values)) == 2)) {
        SEND_ARGS (PIZ_EVENT_CHANGE, 2, values)
    }
}

void tralala_fill (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long values[ ] = { 0, PIZ_VALUE_VELOCITY };
    
    if (argc && argv && ((tralala_parse (argc, argv, values)) == 2)) {
        SEND_ARGS (PIZ_EVENT_FILL, 2, values)
    }
}

void tralala_kill (t_tralala *x)
{
    SEND (PIZ_EVENT_KILL)
}

void tralala_cycle (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc > 1) {
    //
    if (atom_gettype (argv) == A_SYM) {
    //
    long     values[argc];
    PIZError err = PIZ_GOOD;

    err |= tralala_keyWithSymbol (atom_getsym (argv), values);

    if (!err) {
        if (!(atom_getlong_array (argc - 1, argv + 1, argc - 1, values + 1))) {
            SEND_ARGS (PIZ_EVENT_CYCLE, argc, values)
        }
    }
    //
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_int (t_tralala *x, long n)
{
    SEND_VALUE (PIZ_EVENT_LEARN, n)
}

void tralala_zoulou (t_tralala *x)
{
    SEND (PIZ_EVENT_ZOULOU)
}

void tralala_romeo (t_tralala *x)
{
    SEND (PIZ_EVENT_ROMEO)
}

void tralala_november (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    long n = 1;
    
    if (argc && (atom_gettype (argv) == A_LONG)) {
        n = atom_getlong (argv);
    } 
    
    SEND_VALUE (PIZ_EVENT_NOVEMBER, n)
}

void tralala_list (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc && (atom_gettype (argv) == A_LONG)) { 
        tralala_int (x, atom_getlong (argv));
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

long tralala_parse (long argc, t_atom *argv, long *values)
{
    long i;
    bool v = false;
    bool o = false;
    
    for (i = 0; i < argc; i++) {
    //
    if ((atom_gettype (argv + i)) == A_LONG) {
        (*values) = atom_getlong (argv + i); 
        v = true;
        
    } else if (atom_gettype (argv + i) == A_SYM) {
        if (!(tralala_valueWithSymbol (atom_getsym (argv + i), values))) { 
            v = true; 
        }
        if (!(tralala_optionWithSymbol (atom_getsym (argv + i), values + 1))) { 
            o = true; 
        }
    }
    //
    }
    
    return (v + o);
} 

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
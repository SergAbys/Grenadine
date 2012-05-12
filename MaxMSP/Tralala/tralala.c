/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  May 12, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"  
#include "tralalaSymbol.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TICKS_PER_STEP          20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TRALALA(a)              PIZEvent *event = NULL;                                 \
                                if (event = pizEventNew ((a), -1, 0, NULL)) {           \
                                    pizAgentAddEvent (x->agent, event);                 \
                                }
#define TRALALA_ARGS(a,b,c)     PIZEvent *event = NULL;                                 \
                                if (event = pizEventNew ((a), -1, (b), (c))) {          \
                                    pizAgentAddEvent (x->agent, event);                 \
                                } 
                                                                                 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class *tralala_class;

int main (void)
{	
    t_class	*c = NULL;

    c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

    class_addmethod (c, (method)tralala_assist,    "assist",    A_CANT, 0);
    class_addmethod (c, (method)tralala_bang,      "bang",      0);
    class_addmethod (c, (method)tralala_play,      "play",      0);
    class_addmethod (c, (method)tralala_stop,      "stop",      0);
    class_addmethod (c, (method)tralala_loop,      "loop",      0);
    class_addmethod (c, (method)tralala_unloop,    "unloop",    0);
    class_addmethod (c, (method)tralala_clear,     "clear",     0);
    class_addmethod (c, (method)tralala_bpm,       "bpm",       A_LONG, 0);
    class_addmethod (c, (method)tralala_chance,    "chance",    A_LONG, 0);
    class_addmethod (c, (method)tralala_velocity,  "velocity",  A_LONG, 0);
    class_addmethod (c, (method)tralala_channel,   "channel",   A_LONG, 0);
    class_addmethod (c, (method)tralala_transpose, "transpose", A_LONG, 0);
    class_addmethod (c, (method)tralala_cell,      "cell",      A_GIMME, 0);
    class_addmethod (c, (method)tralala_scale,     "scale",     A_GIMME, 0);
    class_addmethod (c, (method)tralala_pattern,   "pattern",   A_GIMME, 0);
    class_addmethod (c, (method)tralala_note,      "note",      A_GIMME, 0);

    class_register (CLASS_BOX, c);

    tralala_class = c;
        
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
        
            x->rightOutlet          = bangout ((t_object *)x);
            x->middleRightOutlet    = bangout ((t_object *)x);
            x->middleLeftOutlet     = listout ((t_object *)x); 
            x->leftOutlet           = listout ((t_object *)x);
            
            pizAgentAttach (x->agent, (void *)x, tralala_notify);
            tralala_symbolsInit ( );

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
            case 1 : sprintf (s, "(List) Notes Dumped"); break;
            case 2 : sprintf (s, "(Bang) End");          break;
            case 3 : sprintf (s, "(Bang) Will End");     break;
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
    PIZEventName name;
    
    x = (t_tralala *)ptr;
    pizEventName (event, &name);
    
    if (name == PIZ_EVENT_NOTE_PLAYED) {
        pizEventPtr (event, &argc, &argv);
        
        atom_setlong_array (4, x->notePlayed, argc - 1, argv + 1);
        outlet_list (x->leftOutlet, NULL, 4, x->notePlayed); 
    
    } else if (name == PIZ_EVENT_END) {
        outlet_bang (x->middleRightOutlet);

    } else if (name == PIZ_EVENT_WILL_END) {
        outlet_bang (x->rightOutlet);
    }
    
    DEBUGEVENT
    
    pizEventFree (event);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_bang (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_PLAY)
}

void tralala_play (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_PLAY)
}

void tralala_stop (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_STOP)
}

void tralala_loop (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_LOOP)
}

void tralala_unloop (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_UNLOOP)
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_bpm (t_tralala *x, long n) 
{   
    TRALALA_ARGS (PIZ_EVENT_BPM, 1, &n)
}

void tralala_chance (t_tralala *x, long n) 
{   
    TRALALA_ARGS (PIZ_EVENT_CHANCE, 1, &n)
}

void tralala_velocity (t_tralala *x, long n) 
{   
    TRALALA_ARGS (PIZ_EVENT_VELOCITY, 1, &n)
}

void tralala_channel (t_tralala *x, long n) 
{   
    TRALALA_ARGS (PIZ_EVENT_CHANNEL, 1, &n)
}

void tralala_cell (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    PIZError err = PIZ_GOOD;
    long     value = PIZ_NOTE_VALUE_NONE;
    
    if (argc) {
    //
    long size = 0;
    char *string = NULL;

    atom_gettext (argc, argv, &size, &string, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
    
    if (string) {
        err |= tralala_noteValueWithSymbol ((gensym (string)), &value);
        sysmem_freeptr (string);
    }
    //
    }
    
    if (!err) {
        TRALALA_ARGS (PIZ_EVENT_CELL, 1, &value);
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
            TRALALA_ARGS (PIZ_EVENT_SCALE, 2, values);
        }
    }
    //
    } else {
        TRALALA (PIZ_EVENT_SCALE)
    }
}

void tralala_pattern (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc) {
    //
    long values[argc];
    
    if (!(atom_getlong_array (argc, argv, argc, values))) {
        TRALALA_ARGS (PIZ_EVENT_PATTERN, argc, values)
    }
    //
    } else {
        TRALALA (PIZ_EVENT_PATTERN)
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_clear (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_CLEAR)
}

void tralala_note (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc > 1) {
    //
    long values[ ] = { 0, 0, 0, 0, 0 };
    
    if (!(atom_getlong_array (argc, argv, 5, values))) {
        values[0] = (long)(values[0] / TICKS_PER_STEP);
        values[3] = (long)(values[3] / TICKS_PER_STEP);
        TRALALA_ARGS (PIZ_EVENT_NOTE, argc, values)
    }
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_transpose (t_tralala *x, long n)
{
    TRALALA_ARGS (PIZ_EVENT_TRANSPOSE, 1, &n)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
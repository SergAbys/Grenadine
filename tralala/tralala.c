/*
 *  tralala.c
 *
 *  Created : 17/05/12.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  April 22, 2012.
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TICKS_PER_STEP      20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define EVENT(a)            PIZEvent *event = NULL;                                             \
                            if (event = pizEventNew ((a))) {                                    \
                                pizAgentAddEvent (x->agent, event);                             \
                            }
#define EVENT_VALUE(a,b)    PIZEvent *event = NULL;                                             \
                            if (event = pizEventNewWithValue ((a), (b))) {                      \
                                pizAgentAddEvent (x->agent, event);                             \
                            }
#define EVENT_ARGS(a,b,c)   PIZEvent *event = NULL;                                             \
                            if (event = pizEventNewWithArgs ((a), (b), (c))) {                  \
                                pizAgentAddEvent (x->agent, event);                             \
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
        
            EVENT (PIZ_EVENT_INIT)
            x->outlet = outlet_new ((t_object *)x, NULL);
            
        } else {
            object_free (x);
            x = NULL;
        }
    }	
	
	return x;
}

void tralala_free (t_tralala *x)
{ 
    pizAgentFree (x->agent);
}

void tralala_assist (t_tralala *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf (s, "Messages");
    } else {	
        sprintf (s, "Out");
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_notify (t_tralala *x, PIZEvent *event)
{
    ;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_bang (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_PLAY)
}

void tralala_play (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_PLAY)
}

void tralala_stop (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_STOP)
}

void tralala_loop (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_LOOP)
}

void tralala_unloop (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_UNLOOP)
}

void tralala_bpm (t_tralala *x, long n) 
{   
    EVENT_VALUE (PIZ_EVENT_BPM, n)
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_note (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if ((argc > 1) && (argc <= PIZ_SEQUENCE_NOTE_SIZE)) {
    //
    long values[PIZ_SEQUENCE_NOTE_SIZE];
    
    atom_getlong_array (argc, argv, PIZ_SEQUENCE_NOTE_SIZE, values);
    
    values[PIZ_DATA_POSITION] = (long)(values[PIZ_DATA_POSITION] / TICKS_PER_STEP);
    values[PIZ_DATA_DURATION] = (long)(values[PIZ_DATA_DURATION] / TICKS_PER_STEP);
    
    EVENT_ARGS (PIZ_EVENT_NOTE, argc, values)
    //
    }
}

void tralala_clear (t_tralala *x) 
{   
    EVENT (PIZ_EVENT_CLEAR)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
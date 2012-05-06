/*
 *  tralala.c
 *
 *  Created : 17/05/12.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  May 5, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "pizAgent.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _tralala {
	t_object    ob;
    PIZAgent    *agent;
	void        *leftOutlet;
    void        *middleLeftOutlet;
    void        *middleRightOutlet;
    void        *rightOutlet;
	} t_tralala;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *tralala_new               (t_symbol *s, long argc, t_atom *argv);
void tralala_free               (t_tralala *x);
void tralala_assist             (t_tralala *x, void *b, long m, long a, char *s);

void tralala_notify             (void *ptr, PIZEvent *event);

void tralala_bang               (t_tralala *x);
void tralala_play               (t_tralala *x);
void tralala_stop               (t_tralala *x);
void tralala_loop               (t_tralala *x);
void tralala_unloop             (t_tralala *x);

void tralala_bpm                (t_tralala *x, long n);
void tralala_chance             (t_tralala *x, long n);
void tralala_velocity           (t_tralala *x, long n);
void tralala_channel            (t_tralala *x, long n);

void tralala_note               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_clear              (t_tralala *x);

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
            case 0 : sprintf (s, "(List) Notes Played");   break;
            case 1 : sprintf (s, "(List) Notes Dumped");   break;
            case 2 : sprintf (s, "(Bang) End");      break;
            case 3 : sprintf (s, "(Bang) Will End"); break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_notify (void *ptr, PIZEvent *event)
{
    PIZEventName name;
    t_tralala *x = (t_tralala *)ptr;
    
    pizEventName (event, &name);
    
    if (name == PIZ_EVENT_NOTE_PLAYED) {
        long    argc;
        long    *argv = NULL;
        t_atom  notePlayed[4];
        
        if (!(pizEventPtr (event, &argc, &argv))) {
            atom_setlong_array (4, notePlayed, 4, argv + 1);
            outlet_list (x->leftOutlet, NULL, 4, notePlayed); 
        }
    
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_note (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    if ((argc > 1) && (argc <= 5)) {
        long values[5];
        
        atom_getlong_array (argc, argv, 5, values);
        
        values[0] = (long)(values[0] / TICKS_PER_STEP);
        values[3] = (long)(values[3] / TICKS_PER_STEP);
        
        TRALALA_ARGS (PIZ_EVENT_NOTE, argc, values)
    }
}

void tralala_clear (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_CLEAR)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
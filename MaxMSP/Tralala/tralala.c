/*
 *  tralala.c
 *
 *  Created : 17/05/12.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  April 29, 2012.
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

void tralala_note               (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_clear              (t_tralala *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TICKS_PER_STEP          20

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define TRALALA(a)              PIZEvent *event = NULL;                                             \
                                if (event = pizEventNew ((a))) {                                    \
                                    pizAgentAddEvent (x->agent, event);                             \
                                }
#define TRALALA_VALUE(a,b)      PIZEvent *event = NULL;                                             \
                                if (event = pizEventNewWithValue ((a), (b))) {                      \
                                    pizAgentAddEvent (x->agent, event);                             \
                                }
#define TRALALA_ARGS(a,b,c)     PIZEvent *event = NULL;                                             \
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
            
            x->rightOutlet          = bangout ((t_object *)x);
            x->middleRightOutlet    = bangout ((t_object *)x);
            x->middleLeftOutlet     = listout ((t_object *)x); 
            x->leftOutlet           = listout ((t_object *)x);
            
            pizAgentAttach (x->agent, (void *)x, tralala_notify);
            TRALALA (PIZ_EVENT_INIT)

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
            case 0 : sprintf (s, "(List) Played");  break;
            case 1 : sprintf (s, "(List) Dumped");  break;
            case 2 : sprintf (s, "(Bang) End");     break;
            case 3 : sprintf (s, "(Bang) Will End");    break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_notify (void *ptr, PIZEvent *event)
{
    PIZEventIdentifier ie;
    t_tralala *x = (t_tralala *)ptr;
    
    pizEventGetIdentifier (event, &ie);
    
    if (ie == PIZ_EVENT_NOTE_PLAYED) {
    //
    long    argc;
    long    *argv = NULL;
    t_atom  notePlayed[4];
    
    if ((!(pizEventGetData (event, &argc, &argv)) && (argc == PIZ_SEQUENCE_NOTE_SIZE))) {
        atom_setlong_array (4, notePlayed, 4, argv + PIZ_DATA_PITCH);
        outlet_list (x->leftOutlet, NULL, 4, notePlayed); 
    }
    //
    
    } else if (ie == PIZ_EVENT_END) {
        outlet_bang (x->middleRightOutlet);

    } else if (ie == PIZ_EVENT_WILL_END) {
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
    TRALALA_VALUE (PIZ_EVENT_BPM, n)
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
    
    TRALALA_ARGS (PIZ_EVENT_NOTE, argc, values)
    //
    }
}

void tralala_clear (t_tralala *x) 
{   
    TRALALA (PIZ_EVENT_CLEAR)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
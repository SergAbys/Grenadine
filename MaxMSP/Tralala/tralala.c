/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

typedef struct _tralala {
    t_object    ob;
    t_atom      temp[4];
    PIZAgent    *agent;
    void        *leftOutlet;
    void        *rightOutlet;
    } t_tralala;
                
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define SEND(code)          PIZEvent *event = NULL;                                     \
                            if (event = pizEventNew (code)) {                           \
                                pizAgentAddEvent (x->agent, event);                     \
                            }
#define PARSE(s, ac, av)    PIZEvent *event = NULL;                                     \
                            if (event = tralala_parseToEvent ((s), (ac), (av))) {       \
                                pizAgentAddEvent (x->agent, event);                     \
                            }
                      
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new       (t_symbol *s, long argc, t_atom *argv);

void tralala_free       (t_tralala *x);
void tralala_assist     (t_tralala *x, void *b, long m, long a, char *s);
void tralala_notify     (void *ptr, PIZEvent *event);

void tralala_bang       (t_tralala *x);
void tralala_play       (t_tralala *x);
void tralala_stop       (t_tralala *x);
void tralala_loop       (t_tralala *x);
void tralala_unloop     (t_tralala *x);

void tralala_list       (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
void tralala_anything   (t_tralala *x, t_symbol *s, long argc, t_atom *argv);
                    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static t_class  *tralala_class;

static t_symbol *tll_end; 
static t_symbol *tll_willEnd; 

int main (void)
{	
    t_class	*c = NULL;
    
    c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

    class_addmethod (c, (method)tralala_bang,       "bang",     0);
    class_addmethod (c, (method)tralala_play,       "play",     0);
    class_addmethod (c, (method)tralala_stop,       "stop",     0);
    class_addmethod (c, (method)tralala_loop,       "loop",     0);
    class_addmethod (c, (method)tralala_unloop,     "unloop",   0);
    class_addmethod (c, (method)tralala_list,       "list",     A_GIMME, 0);
    class_addmethod (c, (method)tralala_anything,   "anything", A_GIMME, 0);

    class_register (CLASS_BOX, c);

    tralala_class = c;
    
    tralala_parseInit ( );
    
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
    //
    if (x->agent = pizAgentNew ( )) {
    
        x->rightOutlet = outlet_new ((t_object *)x, NULL);
        x->leftOutlet  = listout ((t_object *)x);
        
        pizAgentAttach (x->agent, (void *)x, tralala_notify);
        
    } else {
        object_free (x);
        x = NULL;
    }
    //
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
            case 1 : sprintf (s, "Messages"); break;
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
    //
    pizEventData (event, &argc, &argv);
    atom_setlong_array (4, x->temp, argc - 1, argv + 1);
    outlet_list (x->leftOutlet, NULL, 4, x->temp); 
    //
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

void tralala_list (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    PARSE (s, argc, argv)
}

void tralala_anything (t_tralala *x, t_symbol *s, long argc, t_atom *argv)
{
    PARSE (s, argc, argv)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
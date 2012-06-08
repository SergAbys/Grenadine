/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralala.h"
#include "tralalaParse.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

t_tralalaSymbols tll_symbolsTable;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static long    identifier = 1;
static t_class *tralala_class;

int main (void)
{	
    t_class	*c = NULL;
    
    c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

    class_addmethod (c, (method)tralala_bang,                   "bang",                 0);
    class_addmethod (c, (method)tralala_play,                   "play",                 0);
    class_addmethod (c, (method)tralala_stop,                   "stop",                 0);
    class_addmethod (c, (method)tralala_loop,                   "loop",                 0);
    class_addmethod (c, (method)tralala_unloop,                 "unloop",               0);
    class_addmethod (c, (method)tralala_assist,                 "assist",               A_CANT, 0);
    class_addmethod	(c, (method)tralala_appendtodictionary,     "appendtodictionary",   A_CANT, 0);
    class_addmethod (c, (method)tralala_list,                   "list",                 A_GIMME, 0);
    class_addmethod (c, (method)tralala_anything,               "anything",             A_GIMME, 0);

    class_register (CLASS_BOX, c);

    tralala_class = c;
    
    tralala_parseInit (&tll_symbolsTable);

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
    t_dictionary *d = NULL;
    t_dictionary *sd = NULL; 
        
    if (d = (t_dictionary *)gensym ("#D")->s_thing) {
    //
    if (dictionary_entryisdictionary (d, tll_tralala)) {
        dictionary_getdictionary (d, tll_tralala, (t_object **)&x->data);
        dictionary_chuckentry (d, tll_tralala);
        
    } else {
        sd      = dictionary_new ( );
        x->data = dictionary_new ( );
        dictionary_appenddictionary (x->data, tll_current, (t_object *)sd);
    }
    //
    }
    
    if (x->data && (x->agent = pizAgentNew (identifier++))) {
    
        x->right       = bangout ((t_object *)x);
        x->middleRight = bangout ((t_object *)x);
        x->middleLeft  = outlet_new ((t_object *)x, NULL);
        x->left        = listout ((t_object *)x);
        
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
    if (x->agent) {
        pizAgentDetach (x->agent, (void *)x);
        pizAgentFree   (x->agent);
    }
}

void tralala_assist (t_tralala *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf (s, "Messages");
        
    } else {	
        switch (a) {
            case 0 : sprintf (s, "(List) Played");     break;
            case 1 : sprintf (s, "(Anything) Dumped"); break;
            case 2 : sprintf (s, "(Bang) End");        break;
            case 3 : sprintf (s, "(Bang) Will End");   break;
        }
    }
}

void tralala_appendtodictionary (t_tralala *x, t_dictionary *d)
{
    if (d) {
        //dictionary_appenddictionary (d, tll_tralala, (t_object *)x->data);
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
    t_dictionary *d = NULL;
    PIZEventCode code;
    
    x = (t_tralala *)ptr;
    pizEventCode (event, &code);
    
    DEBUGEVENT
    
    switch (code) {
    //
    case PIZ_EVENT_NOTE_PLAYED :
        pizEventData (event, &argc, &argv);
        atom_setlong_array (4, x->played, argc - 1, argv + 1);
        outlet_list (x->left, NULL, 4, x->played); 
        break;
        
    case PIZ_EVENT_NOTE_DUMPED :
        pizEventData (event, &argc, &argv);
        atom_setlong_array (5, x->dumped, argc, argv);
        outlet_anything (x->middleLeft, tll_note, 5, x->dumped); 
        break;

    case PIZ_EVENT_WILL_DUMP :
        outlet_anything (x->middleLeft, tll_clear, 0, NULL);
        break;

    case PIZ_EVENT_WILL_END :
        outlet_bang (x->right);
        break;
    
    case PIZ_EVENT_END :
        outlet_bang (x->middleRight); 
        break;
    
    default :
        if (!(dictionary_getdictionary (x->data, tll_current, (t_object **)&d))) {
            tralala_parseEventToDictionary (d, event);
        } break;
    //
    }
    
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
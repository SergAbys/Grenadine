/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"
#include "jpatcher_api.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

t_tralalaTable tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static t_class *tralala_class;

int main (void)
{	
    t_class	*c = NULL;
    
    c = class_new ("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tralala), 0L, A_GIMME, 0);

    class_addmethod (c, (method)tralala_bang,           "bang",                 0);
    class_addmethod (c, (method)tralala_play,           "play",                 0);
    class_addmethod (c, (method)tralala_stop,           "stop",                 0);
    class_addmethod (c, (method)tralala_loop,           "loop",                 0);
    class_addmethod (c, (method)tralala_unloop,         "unloop",               0);
    class_addmethod (c, (method)tralala_dblclick,       "dblclick",             A_CANT, 0);
    class_addmethod (c, (method)tralala_dictionary,     "appendtodictionary",   A_CANT, 0);
    class_addmethod (c, (method)tralala_assist,         "assist",               A_CANT, 0);
    class_addmethod (c, (method)tralala_list,           "list",                 A_GIMME, 0);
    class_addmethod (c, (method)tralala_anything,       "anything",             A_GIMME, 0);

    class_register (CLASS_BOX, c);

    tralala_class = c;
    
    tralala_parseInit (&tll_table);

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
        if (dictionary_entryisdictionary (d, TLL_TRALALA)) {
            dictionary_getdictionary (d, TLL_TRALALA, (t_object **)&x->data);
            dictionary_chuckentry (d, TLL_TRALALA);
            
        } else {
            sd = dictionary_new ( );
            x->data = dictionary_new ( );
            dictionary_appenddictionary (x->data, TLL_CURRENT, (t_object *)sd);
        }
    }
    
    if (x->data && (x->agent = pizAgentNew ( ))) {
        x->patcher      = NULL;
        x->right        = bangout ((t_object *)x);
        x->middleRight  = bangout ((t_object *)x);
        x->middleLeft   = outlet_new ((t_object *)x, NULL);
        x->left         = listout ((t_object *)x);
        
        x->flags        = TLL_FLAG_NONE;
        
        pizAgentAttach (x->agent, (void *)x, (PIZMethod)tralala_callback);
        defer_low (x, (method)tralala_init, NULL, 0, NULL);

    } else {
        object_free (x);
        x = NULL;
    }
    //
    }	
	
	return x;
}

void tralala_init (t_tralala *x, t_symbol *s, short argc, t_atom *argv)
{
    t_dictionary *sd = NULL;
    
    object_obex_lookup (x, gensym ("#P"), &x->patcher);
    
    if (dictionary_entryisdictionary (x->data, TLL_RESTORE)) {
        dictionary_getdictionary (x->data, TLL_RESTORE, (t_object **)&sd);
        tralala_parseDictionary (x, sd);
        
    } else {
        sd = dictionary_new ( );
        dictionary_appenddictionary (x->data, TLL_RESTORE, (t_object *)sd);
        SEND (PIZ_EVENT_INIT)
    }
}

void tralala_free (t_tralala *x)
{ 
    if (x->agent) {
        pizAgentDetach (x->agent, (void *)x);
        pizAgentFree   (x->agent);
    }
    
    object_free (x->data);
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

void tralala_dictionary (t_tralala *x, t_dictionary *d)
{
    if (d) {
        t_dictionary *temp = NULL;
        t_dictionary *current = NULL;
        t_dictionary *storage = NULL;
        
        dictionary_getdictionary (x->data, TLL_RESTORE, (t_object **)&storage);
        dictionary_getdictionary (x->data, TLL_CURRENT, (t_object **)&current);
        
        dictionary_clear (storage);
        dictionary_copyunique (storage, current);
        dictionary_clear (current);
        
        temp = dictionary_new ( );
        dictionary_copyunique (temp, x->data);
        dictionary_appenddictionary (d, TLL_TRALALA, (t_object *)temp);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_dblclick (t_tralala *x)
{
    post ("DBLCLICK");
    
    dictionary_dump (x->data, 1, 0);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_callback (void *ptr, PIZEvent *event)
{
    long         k, argc = 0;
    long         *argv = NULL;
    t_tralala    *x = NULL;
    PIZEventCode code;
    
    x = (t_tralala *)ptr;
    pizEventCode (event, &code);
    
    DEBUGEVENT
    
    switch (code) {
    //
    case PIZ_EVENT_NOTE_PLAYED :
        pizEventData (event, &argc, &argv);
        k = argv[PIZ_EVENT_NOTE_DURATION]; 
        argv[PIZ_EVENT_NOTE_DURATION] = (long)(k * (PIZ_AGENT_CONSTANT_BPM_MS / argv[PIZ_EVENT_NOTE_BPM]));
        atom_setlong_array (4, x->played, 4, argv + 1);
        outlet_list (x->left, NULL, 4, x->played); 
        break;
        
    case PIZ_EVENT_NOTE_DUMPED :
        pizEventData (event, &argc, &argv);
        atom_setlong_array (5, x->dumped, 5, argv);
        outlet_anything (x->middleLeft, TLL_NOTE, 5, x->dumped); 
        break;

    case PIZ_EVENT_WILL_DUMP :
        outlet_anything (x->middleLeft, TLL_CLEAR, 0, NULL);
        break;

    case PIZ_EVENT_WILL_END :
        outlet_bang (x->right);
        break;
    
    case PIZ_EVENT_END :
        outlet_bang (x->middleRight); 
        break;
    
    default :
        tralala_parseNotification (x, event);
        if (x->flags & TLL_FLAG_SAVE) { jpatcher_set_dirty (x->patcher, 1); }
        break;
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
    x->flags |= TLL_FLAG_SAVE; PARSE (s, argc, argv)
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
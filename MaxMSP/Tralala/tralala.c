/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaParse.h"
#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_tllSymbols tll_table;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_send (t_tll *x, PIZEventCode code, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

static t_class *tll_class;
static long identifier;

int main(void)
{	
    t_class	*c = NULL;
    
    c = class_new("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tll), 0L, A_GIMME, 0);

    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR);
    
    class_addmethod(c, (method)tralala_jsave,       "jsave",     A_CANT,  0);
    class_addmethod(c, (method)tralala_assist,      "assist",    A_CANT,  0);
    class_addmethod(c, (method)tralala_paint,       "paint",     A_CANT,  0);
    class_addmethod(c, (method)tralala_key,         "key",       A_CANT,  0);
    class_addmethod(c, (method)tralala_store,       "store",     A_GIMME, 0);
    class_addmethod(c, (method)tralala_restore,     "restore",   A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "bang",      A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "play",      A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "end",       A_GIMME, 0);
    class_addmethod(c, (method)tralala_loop,        "loop",      A_GIMME, 0);
    class_addmethod(c, (method)tralala_stop,        "stop",      A_GIMME, 0);
    class_addmethod(c, (method)tralala_unloop,      "unloop",    A_GIMME, 0);
    class_addmethod(c, (method)tralala_list,        "list",      A_GIMME, 0);
    class_addmethod(c, (method)tralala_anything,    "anything",  A_GIMME, 0);

    CLASS_ATTR_RGBA                 (c, "color", 0, t_tll, background); 
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "color", 0, "0. 0. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL          (c, "color", 0, "rgba", "Background Color");
    CLASS_ATTR_CATEGORY             (c, "color", 0, "Color");
    CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0. 0. 50. 50.");
    
    class_register(CLASS_BOX, c);

    tll_class = c;
    
    tralala_parseInit(&tll_table);

    return 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new(t_symbol *s, long argc, t_atom *argv)
{
    t_tll *x = NULL;
    t_dictionary *d = NULL;
    t_dictionary *t = NULL;
    
    if (d = object_dictionaryarg(argc, argv)) {
    //
    if (x = (t_tll *)object_alloc(tll_class)) {
    //
    long boxflags = 0L  | JBOX_DRAWFIRSTIN 
                        | JBOX_NODRAWBOX 
                        | JBOX_DRAWINLAST
                        | JBOX_GROWBOTH
                        | JBOX_DRAWBACKGROUND
                        | JBOX_HILITE;
    
    jbox_new((t_jbox *)x, boxflags, argc, argv);
    x->box.b_firstin = (void *)x;
            
    x->right        = bangout((t_object *)x);
    x->middleRight  = outlet_new((t_object *)x, NULL);
    x->middleLeft   = outlet_new((t_object *)x, NULL);
    x->left         = listout((t_object *)x);
    
    jbox_ready((t_jbox *)x);
    attr_dictionary_process(x, d);
    
    if ((x->data = dictionary_new( )) && (x->current = dictionary_new( ))) {
    //
    if (dictionary_entryisdictionary(d, TLL_SYM_TRALALA)) {
        dictionary_getdictionary(d, TLL_SYM_TRALALA, (t_object **)&t);
        dictionary_copyunique(x->data, t);
    } 
    
    x->identifier = ++identifier;
    x->flags = TLL_FLAG_NONE;
        
    if (x->agent = pizAgentNew(x->identifier)) {
        pizAgentAttach(x->agent, (void *)x, (PIZMethod)tralala_callback);
        defer_low(x, (method)tralala_init, NULL, 0, NULL);

    } else {
        object_free(x);
        x = NULL;
    }
    //
    }
    //
    }	
    //
    }
	
	return x;
}

void tralala_init(t_tll *x, t_symbol *s, short argc, t_atom *argv)
{
    t_symbol *link = NULL;
    
    link = symbol_unique( );
    link->s_thing = (t_object *)x;
    atom_setsym(&x->link, link);
    
    tralala_send(x, PIZ_EVENT_INIT, 0, NULL);
}

void tralala_free(t_tll *x)
{ 
    if (x->agent) {
        pizAgentDetach(x->agent, (void *)x);
        pizAgentFree(x->agent);
    }
    
    object_free(x->data);
    object_free(x->current);
    
    jbox_free((t_jbox *)x);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_assist(t_tll *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf(s, "Messages");
        
    } else {	
        switch (a) {
            case 0 : sprintf(s, "(List) Played");     break;
            case 1 : sprintf(s, "(Anything) Dumped"); break;
            case 2 : sprintf(s, "(Anything) End");    break;
            case 3 : sprintf(s, "(Bang) Will End");   break;
        }
    }
}

void tralala_jsave(t_tll *x, t_dictionary *d)
{
    if (d) {
        t_dictionary *t = NULL;
        
        if (t = dictionary_new( )) {
            dictionary_copyunique(t, x->data);
            dictionary_appenddictionary(d, TLL_SYM_TRALALA, (t_object *)t);
        }
    }
}

void tralala_store(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    t_dictionary *t = NULL;
    t_symbol *name = TLL_SYM_UNTITLED;

    if (argc && argv) {
        long k = 0;
        char *p = NULL;
        
        atom_gettext(argc, argv, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (p) {
            name = gensym(p);            
        }
    }
    
    if (t = dictionary_new( )) {
        dictionary_copyunique(t, x->current);
        dictionary_appenddictionary(x->data, name, (t_object *)t);
        jpatcher_set_dirty(jbox_get_patcher((t_object *)x), 1);
    }
}
           
void tralala_restore(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    t_dictionary *t = NULL;
    t_symbol *name = TLL_SYM_UNTITLED;
    
    if (argc && argv) {
        long k = 0;
        char *p = NULL;
        
        atom_gettext(argc, argv, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (p) {
            name = gensym(p);            
        }
    }
    
    if ((dictionary_entryisdictionary(x->data, name)) 
        && !(dictionary_getdictionary(x->data, name, (t_object **)&t))) {
        tralala_parseDictionary(x, t);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_callback(void *ptr, PIZEvent *event)
{
    long k, argc = 0;
    long *argv = NULL;
    t_tll *x = NULL;
    PIZEventCode code;
    
    x = (t_tll *)ptr;
    pizEventCode(event, &code);
    
    switch (code) {
    //
    case PIZ_EVENT_NOTE_PLAYED :
        pizEventData(event, &argc, &argv);
        k = argv[PIZ_EVENT_DATA_DURATION]; 
        argv[PIZ_EVENT_DATA_DURATION] = (long)(k * (PIZ_AGENT_CONSTANT_BPM_MS / argv[PIZ_EVENT_DATA_BPM]));
        atom_setlong_array(4, x->played, 4, argv + 1);
        outlet_list(x->left, NULL, 4, x->played); 
        break;
        
    case PIZ_EVENT_NOTE_DUMPED :
        pizEventData(event, &argc, &argv);
        atom_setlong_array(5, x->dumped, 5, argv);
        outlet_anything(x->middleLeft, TLL_SYM_NOTE, 5, x->dumped); 
        break;

    case PIZ_EVENT_WILL_DUMP :
        outlet_anything(x->middleLeft, TLL_SYM_CLEAR, 0, NULL);
        break;

    case PIZ_EVENT_WILL_END :
        outlet_bang(x->right);
        break;
    
    case PIZ_EVENT_END :
        pizEventTime(event, &x->time);
        outlet_anything(x->middleRight, TLL_SYM_END, 1, &x->link);
        break;
    
    default :
        tralala_parseNotification(x, event);
        break;
    //
    }
    
    DEBUGEVENT
        
    pizEventFree(event);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_play(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_EVENT_PLAY, argc, argv);
}

void tralala_loop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_EVENT_LOOP, argc, argv);
}

void tralala_stop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_EVENT_STOP, 0, NULL);
}

void tralala_unloop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_EVENT_UNLOOP, 0, NULL);
}

void tralala_list(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    tralala_parseMessage(x, s, argc, argv);
}

void tralala_anything(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    tralala_parseMessage(x, s, argc, argv);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_paint(t_tll *x, t_object *patcherview)
{
	t_rect rect;
	t_jgraphics *g = NULL;
	
	g = (t_jgraphics*)patcherview_get_jgraphics(patcherview);		
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);		
	
	jgraphics_set_source_jrgba(g, &x->background);
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_fill(g);
}

void tralala_key(t_tll *x, t_object *patcherview, long keycode, long m, long textcharacter)
{
	post("Key : %ld %ld %ld", keycode, m, textcharacter);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -
        
void tralala_send(t_tll *x, PIZEventCode code, long argc, t_atom *argv)
{
    PIZEvent *event = NULL;
    
    if (event = pizEventNew(code)) {
    //
    if ((argc && argv) && (atom_gettype(argv) == A_SYM)) {
    //
    t_symbol *s = atom_getsym(argv);
    t_object *o = s->s_thing;
    
    if (o && !NOGOOD(o) && object_classname_compare(o, TLL_SYM_TRALALA)) {
        PIZTime time;
        t_tll *ptr = (t_tll *)o;
        
        pizTimeCopy(&time, &ptr->time);
        pizEventSetTime(event, &time);
    }
    //
    }
                        
    pizEventSetIdentifier(event, x->identifier);
    pizAgentAddEvent(x->agent, event);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
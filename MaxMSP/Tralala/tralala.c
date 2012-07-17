/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaUser.h"
#include "tralalaParse.h"
#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_tllSymbols tll_table;
t_dictionary *tll_clipboard;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_send (t_tll *x, PIZEventCode code, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class *tll_class;
static t_int32_atomic tll_identifier;

static void destructor(void) __attribute__ ((destructor));

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

int main(void)
{	
    t_class	*c = NULL;
    
    c = class_new("tralala", (method)tralala_new, (method)tralala_free, sizeof(t_tll), 0L, A_GIMME, 0);

    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_FONTATTR);
    
    class_addmethod(c, (method)tralala_assist,      "assist",        A_CANT,  0);
    class_addmethod(c, (method)tralala_jsave,       "jsave",         A_CANT,  0);
    class_addmethod(c, (method)tralala_paint,       "paint",         A_CANT,  0);
    class_addmethod(c, (method)tralala_params,      "getdrawparams", A_CANT,  0);
    class_addmethod(c, (method)tralala_key,         "key",           A_CANT,  0);
    class_addmethod(c, (method)tralala_wheel,       "mousewheel",    A_CANT,  0);
    class_addmethod(c, (method)tralala_down,        "mousedown",     A_CANT,  0);
    class_addmethod(c, (method)tralala_move,        "mousemove",     A_CANT,  0);
    class_addmethod(c, (method)tralala_drag,        "mousedrag",     A_CANT,  0);
	class_addmethod(c, (method)tralala_up,          "mouseup",       A_CANT,  0);
    class_addmethod(c, (method)tralala_notify,      "notify",        A_CANT,  0);
    class_addmethod(c, (method)tralala_store,       "store",         A_GIMME, 0);
    class_addmethod(c, (method)tralala_recall,      "recall",        A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "bang",          A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "play",          A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "end",           A_GIMME, 0);
    class_addmethod(c, (method)tralala_loop,        "loop",          A_GIMME, 0);
    class_addmethod(c, (method)tralala_stop,        "stop",          A_GIMME, 0);
    class_addmethod(c, (method)tralala_unloop,      "unloop",        A_GIMME, 0);
    class_addmethod(c, (method)tralala_list,        "list",          A_GIMME, 0);
    class_addmethod(c, (method)tralala_anything,    "anything",      A_GIMME, 0);

    CLASS_ATTR_RGBA         (c, "bgcolor", 0, t_tll, backgroundColor); 
    CLASS_ATTR_DEFAULT_SAVE (c, "bgcolor", 0, "0. 0. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "bgcolor", 0, "rgba", "Background Color");
    CLASS_ATTR_CATEGORY     (c, "bgcolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "bordercolor", 0, t_tll, borderColor); 
    CLASS_ATTR_DEFAULT_SAVE (c, "bordercolor", 0, "0. 0. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "bordercolor", 0, "rgba", "Border Color");
    CLASS_ATTR_CATEGORY     (c, "bordercolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "color", 0, t_tll, color); 
    CLASS_ATTR_DEFAULT_SAVE (c, "color", 0, "0.30 0.24 0.28 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "color", 0, "rgba", "Color");
    CLASS_ATTR_CATEGORY     (c, "color", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor1", 0, t_tll, highlightedColor1); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor1", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor1", 0, "rgba", "Highlighted Color");
    CLASS_ATTR_CATEGORY     (c, "hcolor1", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor2", 0, t_tll, highlightedColor2); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor2", 0, "1. 0.75 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor2", 0, "rgba", "Highlighted Color");
    CLASS_ATTR_CATEGORY     (c, "hcolor2", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "textcolor", 0, t_tll, textColor); 
    CLASS_ATTR_DEFAULT_SAVE (c, "textcolor", 0, "0.51 0.44 0.49 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "textcolor", 0, "rgba", "Text Color");
    CLASS_ATTR_CATEGORY     (c, "textcolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "lassocolor", 0, t_tll, lassoColor); 
    CLASS_ATTR_DEFAULT_SAVE (c, "lassocolor", 0, "0.51 0.44 0.49 0.40"); 
    CLASS_ATTR_STYLE_LABEL  (c, "lassocolor", 0, "rgba", "Lasso Color");
    CLASS_ATTR_CATEGORY     (c, "lassocolor", 0, "Color");
  
    CLASS_ATTR_LONG         (c, "xoffset", 0, t_tll, offsetX);
    CLASS_ATTR_DEFAULT_SAVE (c, "xoffset", 0, "-37");
    CLASS_ATTR_LABEL        (c, "xoffset", 0, "Offset X");
    CLASS_ATTR_CATEGORY     (c, "xoffset", 0, "Value");

    CLASS_ATTR_LONG         (c, "yoffset", 0, t_tll, offsetY);
    CLASS_ATTR_DEFAULT_SAVE (c, "yoffset", 0, "658");
    CLASS_ATTR_LABEL        (c, "yoffset", 0, "Offset Y");
    CLASS_ATTR_CATEGORY     (c, "yoffset", 0, "Value");

    CLASS_ATTR_LONG         (c, "viewtext", 0, t_tll, viewText);
    CLASS_ATTR_DEFAULT_SAVE (c, "viewtext", 0, "1");
    CLASS_ATTR_LABEL        (c, "viewtext", 0, "View Text");
    CLASS_ATTR_STYLE        (c, "viewtext", 0, "onoff");
    CLASS_ATTR_FILTER_CLIP  (c, "viewtext", 0, 1);
    CLASS_ATTR_CATEGORY     (c, "viewtext", 0, "Appearance");
 
    CLASS_ATTR_ORDER        (c, "color",       0, "1");
    CLASS_ATTR_ORDER        (c, "hcolor1",     0, "2");
    CLASS_ATTR_ORDER        (c, "hcolor2",     0, "3");
    CLASS_ATTR_ORDER        (c, "textcolor",   0, "4");
    CLASS_ATTR_ORDER        (c, "lassocolor",  0, "5");
    CLASS_ATTR_ORDER        (c, "bordercolor", 0, "6");
    CLASS_ATTR_ORDER        (c, "bgcolor",     0, "7");
    
    CLASS_ATTR_DEFAULT      (c, "fontname", 0, "Arial");
    CLASS_ATTR_DEFAULT      (c, "fontsize", 0, "14.");
    CLASS_ATTR_DEFAULT      (c, "fontface", 0, "1");

    CLASS_ATTR_DEFAULT      (c, "patching_rect", 0, "0. 0. 200. 200.");
    
    class_register(CLASS_BOX, c);

    tll_class = c;
    
    tralala_parseInit(&tll_table);
    tll_clipboard = dictionary_new( );

    return 0;
}

void destructor(void) 
{
    cpost("Max: Et voil�, tralala, Zut � celui qui le lira !\n");
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *tralala_new(t_symbol *s, long argc, t_atom *argv)
{
    t_tll *x = NULL;
    t_dictionary *d = NULL;
    t_dictionary *t = NULL;
    PIZError err = PIZ_GOOD;
    
    if (tll_clipboard && (d = object_dictionaryarg(argc, argv))) {
    //
    if (x = (t_tll *)object_alloc(tll_class)) {
    //
    long boxflags = 0L  | JBOX_DRAWFIRSTIN 
                        | JBOX_DRAWINLAST
                        | JBOX_GROWBOTH
                        | JBOX_DRAWBACKGROUND
                        | JBOX_HILITE
                        | JBOX_TRANSPARENT;
    
    jbox_new((t_jbox *)x, boxflags, argc, argv);
    x->box.b_firstin = (void *)x;
                        
    x->right = bangout((t_object *)x);
    x->middleRight = outlet_new((t_object *)x, NULL);
    x->middleLeft = outlet_new((t_object *)x, NULL);
    x->left = listout((t_object *)x);
    
    jbox_ready((t_jbox *)x);
    attr_dictionary_process(x, d);
    
    err |= !(x->data = dictionary_new( ));
    err |= !(x->current = dictionary_new( ));
    err |= !(x->status = dictionary_new( ));
    err |= !(x->layer = jtextlayout_create( ));
    
    err |= (systhread_mutex_new(&x->mutex, SYSTHREAD_MUTEX_NORMAL) != MAX_ERR_NONE);
    
    if (!err) {
        if (dictionary_entryisdictionary(d, TLL_SYM_TRALALA)) {
            dictionary_getdictionary(d, TLL_SYM_TRALALA, (t_object **)&t);
            dictionary_copyunique(x->data, t);
        } 
        
        x->identifier = ATOMIC_INCREMENT(&tll_identifier);
            
        if (!(err |= !(x->agent = pizAgentNew(x->identifier)))) {
            pizAgentAttach(x->agent, (void *)x, (PIZMethod)tralala_callback);
            defer_low(x, (method)tralala_init, NULL, 0, NULL);
        } 
    } 
    
    if (err) {
        object_free(x);
        x = NULL;
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
    
    if (x->mutex) {
        systhread_mutex_free(x->mutex);
    }
    
    if (x->layer) {
        jtextlayout_destroy(x->layer);
    }
    
    object_free(x->status);
    object_free(x->current);
    object_free(x->data);
    
    jbox_free((t_jbox *)x);
}

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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

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
            sysmem_freeptr(p);         
        }
    }
    
    if (t = dictionary_new( )) {
        TLL_LOCK
        dictionary_copyunique(t, x->current);
        TLL_UNLOCK
        dictionary_appenddictionary(x->data, name, (t_object *)t);
        jpatcher_set_dirty(jbox_get_patcher((t_object *)x), 1);
    }
}
           
void tralala_recall(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    t_dictionary *d = NULL;
    t_dictionary *t = NULL;
    t_symbol *name = TLL_SYM_UNTITLED;
    
    if (argc && argv) {
        long k = 0;
        char *p = NULL;
        
        atom_gettext(argc, argv, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (p) {
            name = gensym(p); 
            sysmem_freeptr(p);           
        }
    }
    
    if (!(dictionary_getdictionary(x->data, name, (t_object **)&d))) {
        if (t = dictionary_new ( )) {
            dictionary_copyunique(t, d);
            tralala_send(x, PIZ_EVENT_CLEAR, 0, NULL);
            tralala_parseDictionary(x, t);
            object_free(t);
        }
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
    
    PIZ_DEBUGEVENT
    
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
    tralala_parseMessage(x, s, argc, argv, TLL_NONE);
}

void tralala_anything(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    tralala_parseMessage(x, s, argc, argv, TLL_NONE);
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
        t_symbol *s = atom_getsym(argv);
        t_object *o = s->s_thing;
        
        if (o && !NOGOOD(o) && object_classname_compare(o, TLL_SYM_TRALALA)) {
            PIZTime time;
            t_tll *ptr = (t_tll *)o;
            
            pizTimeCopy(&time, &ptr->time);
            pizEventSetTime(event, &time);
        }
    }
                        
    pizEventSetIdentifier(event, x->identifier);
    pizAgentDoEvent(x->agent, event);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
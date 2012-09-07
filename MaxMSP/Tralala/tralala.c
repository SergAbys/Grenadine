/*
 *  tralala.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "tralalaKey.h"
#include "tralalaMouse.h"
#include "tralalaParse.h"
#include "tralalaPaint.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_tllSymbols tll_table;
t_dictionary *tll_clipboard;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class *tll_class;
static t_int32_atomic tll_identifier;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void tralala_send             (t_tll *x, PIZEventCode code, long argc, t_atom *argv, ulong flags);
PIZ_LOCAL void tralala_switchDaemon     (t_tll *x, PIZEventCode code);
PIZ_LOCAL void tralala_keepAttributes   (t_tll *x, t_symbol *name, t_dictionary *t);

PIZ_LOCAL t_symbol *tralala_unique      (t_tll *x);
PIZ_LOCAL t_symbol *tralala_slotName    (long argc, t_atom *argv);

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
    
    class_addmethod(c, (method)tralala_assist,      "assist",               A_CANT,  0);
    class_addmethod(c, (method)tralala_jsave,       "jsave",                A_CANT,  0);
    class_addmethod(c, (method)tralala_paint,       "paint",                A_CANT,  0);
    class_addmethod(c, (method)tralala_params,      "getdrawparams",        A_CANT,  0);
    class_addmethod(c, (method)tralala_focusGained, "focusgained",          A_CANT,  0);
    class_addmethod(c, (method)tralala_focusLost,   "focuslost",            A_CANT,  0);
    class_addmethod(c, (method)tralala_key,         "key",                  A_CANT,  0);
    class_addmethod(c, (method)tralala_mouseWheel,  "mousewheel",           A_CANT,  0);
    class_addmethod(c, (method)tralala_mouseDown,   "mousedown",            A_CANT,  0);
    class_addmethod(c, (method)tralala_mouseDrag,   "mousedrag",            A_CANT,  0);
    class_addmethod(c, (method)tralala_mouseUp,     "mouseup",              A_CANT,  0);
    class_addmethod(c, (method)tralala_notify,      "notify",               A_CANT,  0);
    class_addmethod(c, (method)object_obex_dumpout, "dumpout",              A_CANT,  0);
    class_addmethod(c, (method)tralala_store,       "store",                A_GIMME, 0);
    class_addmethod(c, (method)tralala_store,       "save",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_store,       "resave",               A_GIMME, 0);
    class_addmethod(c, (method)tralala_recall,      "recall",               A_GIMME, 0);
    class_addmethod(c, (method)tralala_recall,      "load",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_recall,      "reload",               A_GIMME, 0);
    class_addmethod(c, (method)tralala_remove,      "remove",               A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "bang",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "play",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_play,        "end",                  A_GIMME, 0);
    class_addmethod(c, (method)tralala_loop,        "loop",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_stop,        "stop",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_unloop,      "unloop",               A_GIMME, 0);
    class_addmethod(c, (method)tralala_list,        "list",                 A_GIMME, 0);
    class_addmethod(c, (method)tralala_anything,    "anything",             A_GIMME, 0);
        
    CLASS_ATTR_RGBA         (c, "lassocolor", 0, t_tll, lasso); 
    CLASS_ATTR_DEFAULT_SAVE (c, "lassocolor", 0, "0.51 0.44 0.49 0.40"); 
    CLASS_ATTR_STYLE_LABEL  (c, "lassocolor", 0, "rgba", "Lasso Color");
    CLASS_ATTR_CATEGORY     (c, "lassocolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "color", 0, t_tll, color); 
    CLASS_ATTR_DEFAULT_SAVE (c, "color", 0, "0.30 0.24 0.28 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "color", 0, "rgba", "Color");
    CLASS_ATTR_CATEGORY     (c, "color", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "ucolor", 0, t_tll, uColor); 
    CLASS_ATTR_DEFAULT_SAVE (c, "ucolor", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "ucolor", 0, "rgba", "Color Unfocused");
    CLASS_ATTR_CATEGORY     (c, "ucolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor1", 0, t_tll, hColor1); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor1", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor1", 0, "rgba", "Color Highlighted");
    CLASS_ATTR_CATEGORY     (c, "hcolor1", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor2", 0, t_tll, hColor2); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor2", 0, "1. 1. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor2", 0, "rgba", "Color Highlighted");
    CLASS_ATTR_CATEGORY     (c, "hcolor2", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor3", 0, t_tll, hColor3); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor3", 0, "0.80 1. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor3", 0, "rgba", "Color Highlighted");
    CLASS_ATTR_CATEGORY     (c, "hcolor3", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor4", 0, t_tll, hColor4); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor4", 0, "0.51 0.44 0.49 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor4", 0, "rgba", "Color Highlighted");
    CLASS_ATTR_CATEGORY     (c, "hcolor4", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "hcolor5", 0, t_tll, hColor5); 
    CLASS_ATTR_DEFAULT_SAVE (c, "hcolor5", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "hcolor5", 0, "rgba", "Color Highlighted");
    CLASS_ATTR_CATEGORY     (c, "hcolor5", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "textcolor", 0, t_tll, text); 
    CLASS_ATTR_DEFAULT_SAVE (c, "textcolor", 0, "0.51 0.44 0.49 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "textcolor", 0, "rgba", "Text Color");
    CLASS_ATTR_CATEGORY     (c, "textcolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "utextcolor", 0, t_tll, uText); 
    CLASS_ATTR_DEFAULT_SAVE (c, "utextcolor", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "utextcolor", 0, "rgba", "Text Unfocused");
    CLASS_ATTR_CATEGORY     (c, "utextcolor", 0, "Color");
  
    CLASS_ATTR_RGBA         (c, "bordercolor", 0, t_tll, border); 
    CLASS_ATTR_DEFAULT_SAVE (c, "bordercolor", 0, "0. 0. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "bordercolor", 0, "rgba", "Border Color");
    CLASS_ATTR_CATEGORY     (c, "bordercolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "ubordercolor", 0, t_tll, uBorder); 
    CLASS_ATTR_DEFAULT_SAVE (c, "ubordercolor", 0, "0.83 0.74 0.84 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "ubordercolor", 0, "rgba", "Border Unfocused");
    CLASS_ATTR_CATEGORY     (c, "ubordercolor", 0, "Color");
    
    CLASS_ATTR_RGBA         (c, "bgcolor", 0, t_tll, background); 
    CLASS_ATTR_DEFAULT_SAVE (c, "bgcolor", 0, "0. 0. 0. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "bgcolor", 0, "rgba", "Background Color");
    CLASS_ATTR_CATEGORY     (c, "bgcolor", 0, "Color");

    CLASS_ATTR_RGBA         (c, "ubgcolor", 0, t_tll, uBackground); 
    CLASS_ATTR_DEFAULT_SAVE (c, "ubgcolor", 0, "1. 1. 1. 1."); 
    CLASS_ATTR_STYLE_LABEL  (c, "ubgcolor", 0, "rgba", "Background Unfocused");
    CLASS_ATTR_CATEGORY     (c, "ubgcolor", 0, "Color");
    
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
  
    CLASS_ATTR_ORDER        (c, "color",            0, "1");
    CLASS_ATTR_ORDER        (c, "ucolor",           0, "2");
    CLASS_ATTR_ORDER        (c, "hcolor1",          0, "3");
    CLASS_ATTR_ORDER        (c, "hcolor2",          0, "4");
    CLASS_ATTR_ORDER        (c, "hcolor3",          0, "5");
    CLASS_ATTR_ORDER        (c, "hcolor4",          0, "6");
    CLASS_ATTR_ORDER        (c, "hcolor5",          0, "7");
    CLASS_ATTR_ORDER        (c, "textcolor",        0, "8");
    CLASS_ATTR_ORDER        (c, "utextcolor",       0, "9");
    CLASS_ATTR_ORDER        (c, "bordercolor",      0, "10");
    CLASS_ATTR_ORDER        (c, "ubordercolor",     0, "11");
    CLASS_ATTR_ORDER        (c, "bgcolor",          0, "12");
    CLASS_ATTR_ORDER        (c, "ubgcolor",         0, "13");
    CLASS_ATTR_ORDER        (c, "lassocolor",       0, "14");
    
    CLASS_ATTR_DEFAULT      (c, "fontname", 0, "Arial");
    CLASS_ATTR_DEFAULT      (c, "fontsize", 0, "14.");
    CLASS_ATTR_DEFAULT      (c, "fontface", 0, "1");

    CLASS_ATTR_DEFAULT      (c, "patching_rect", 0, "0. 0. 250. 200.");
    
    class_register(CLASS_BOX, c);

    tll_class = c;
    
    tralala_parseInit(&tll_table);
    tll_clipboard = dictionary_new( );

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

    x->right = outlet_new((t_object *)x, NULL);
    x->middleRight = bangout((t_object *)x);
    x->middle = outlet_new((t_object *)x, NULL);
    x->middleLeft = outlet_new((t_object *)x, NULL);
    x->left = listout((t_object *)x);
    
    object_obex_store((void *)x, TLL_SYM_DUMPOUT, (t_object *)x->right);

    jbox_ready((t_jbox *)x);
    attr_dictionary_process(x, d);
    
    err |= !(x->temp[0] = pizArrayNew(0));
    err |= !(x->temp[1] = pizArrayNew(0));
    err |= !(x->temp[2] = pizArrayNew(0));
    err |= !(x->runCopy = pizLinklistNew( ));
    err |= !(x->run = pizLinklistNew( ));
    err |= !(x->daemon = pizLinklistNew( ));
    err |= !(x->data = dictionary_new( ));
    err |= !(x->current = dictionary_new( ));
    err |= !(x->status = dictionary_new( ));
    err |= !(x->layer = jtextlayout_create( ));
    err |= !(x->runClock = clock_new(x, (method)tralala_runTask));
    err |= !(x->daemonClock = clock_new(x, (method)tralala_daemonTask));
    err |= !(x->gainedClock = clock_new(x, (method)tralala_gainedTask));
    err |= !(x->lostClock = clock_new(x, (method)tralala_lostTask));
    
    err |= (systhread_mutex_new(&x->dataMutex, SYSTHREAD_MUTEX_NORMAL) != MAX_ERR_NONE);
    err |= (systhread_mutex_new(&x->runMutex, SYSTHREAD_MUTEX_NORMAL) != MAX_ERR_NONE);
    err |= (systhread_mutex_new(&x->daemonMutex, SYSTHREAD_MUTEX_NORMAL) != MAX_ERR_NONE);
    
    x->identifier = ATOMIC_INCREMENT(&tll_identifier);
    
    if (!err) {
        if (dictionary_entryisdictionary(d, TLL_SYM_TRALALA)) {
            dictionary_getdictionary(d, TLL_SYM_TRALALA, (t_object **)&t);
            dictionary_copyunique(x->data, t);
        } 
        
        if (!(err |= !(x->agent = pizAgentNew( )))) {
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
    
    link = tralala_unique(x);
    link->s_thing = (t_object *)x;
    atom_setsym(&x->link, link);
    
    tralala_send(x, PIZ_MSG_INIT, 0, NULL, TLL_FLAG_NONE);
}

void tralala_free(t_tll *x)
{ 
    if (x->agent) {
        pizAgentDetach(x->agent, (void *)x);
        pizAgentFree(x->agent);
    }
    
    if (x->dataMutex) {
        systhread_mutex_free(x->dataMutex);
    }
    
    if (x->runMutex) {
        systhread_mutex_free(x->runMutex);
    }
    
    if (x->daemonMutex) {
        systhread_mutex_free(x->daemonMutex);
    }
    
    if (x->layer) {
        jtextlayout_destroy(x->layer);
    }
    
    if (x->runClock) {
        clock_unset(x->runClock);
        object_free(x->runClock);
    }
    
    if (x->daemonClock) {
        clock_unset(x->daemonClock);
        object_free(x->daemonClock);
    }
    
    if (x->gainedClock) {
        clock_unset(x->gainedClock);
        object_free(x->gainedClock);
    }
    
    if (x->lostClock) {
        clock_unset(x->lostClock);
        object_free(x->lostClock);
    }
    
    pizArrayFree(x->temp[0]);
    pizArrayFree(x->temp[1]);
    pizArrayFree(x->temp[2]);
    
    pizLinklistFree(x->runCopy);
    pizLinklistFree(x->run);
    pizLinklistFree(x->daemon);

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
            case 0 : sprintf(s, "(List) Played");       break;
            case 1 : sprintf(s, "(Anything) Dumped");   break;
            case 2 : sprintf(s, "(Anything) End");      break;
            case 3 : sprintf(s, "(Bang) Will End");     break;
            case 4 : sprintf(s, "(Anything) Dumpout");  break;
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
    t_symbol *name = NULL;

    if (t = dictionary_new( )) {
        TLL_DATA_LOCK
        dictionary_copyunique(t, x->current);
        TLL_DATA_UNLOCK
    
        if (s == TLL_SYM_RESAVE) {
            name = x->name;
        } else {
            name = tralala_slotName(argc, argv);
            x->name = name;
        }  
    
        if ((s == TLL_SYM_SAVE) ||(s == TLL_SYM_RESAVE)) { 
            tralala_keepAttributes(x, name, t); 
        }
    
        if (name) {
            dictionary_appenddictionary(x->data, name, (t_object *)t);
            jpatcher_set_dirty(jbox_get_patcher((t_object *)x), 1);
        }
    }
}

void tralala_recall(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    t_dictionary *d = NULL;
    t_dictionary *t = NULL;
    t_symbol *name = NULL;
    ulong flags = TLL_FLAG_RUN;
    
    if (s == TLL_SYM_RELOAD) {
        name = x->name;
    } else {
        name = tralala_slotName(argc, argv);
        x->name = name;
    }        
    
    if ((s == TLL_SYM_LOAD) || (s == TLL_SYM_RELOAD)) {
        flags |= TLL_FLAG_FILTER;
    } 
    
    if (name && !(dictionary_getdictionary(x->data, name, (t_object **)&d))) {
        if (t = dictionary_new ( )) {
            dictionary_copyunique(t, d);
            tralala_send(x, PIZ_MSG_CLEAR, 0, NULL, TLL_FLAG_RUN);
            tralala_parseDictionary(x, t, flags);
            object_free(t);
        }
    }
}

void tralala_remove(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    if (!(dictionary_deleteentry(x->data, tralala_slotName(argc, argv)))) {
        jpatcher_set_dirty(jbox_get_patcher((t_object *)x), 1);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_callback(void *ptr, PIZEvent *event)
{
    PIZEvent *copy = NULL;
    t_tll *x = (t_tll *)ptr;
    PIZEventCode code = PIZ_MSG_NONE;

    TLL_RUN_LOCK
    pizLinklistAppend(x->run, (void *)event);
    TLL_RUN_UNLOCK
    
    clock_fdelay(x->runClock, 0.);
    
    pizEventCode(event, &code);
    tralala_switchDaemon(x, code);
        
    if ((code == PIZ_NOTIFY_PLAYED) && (copy = pizEventNewCopy(event))) {
    //
    TLL_DAEMON_LOCK
    pizLinklistAppend(x->daemon, (void *)copy);
    TLL_DAEMON_UNLOCK
        
    TLL_FLAG_SET(TLL_DIRTY_RUN)
    jbox_redraw((t_jbox *)x);
    //
    }
}

void tralala_runTask (t_tll *x)
{
    PIZEvent *event = NULL;
    PIZEvent *nextEvent = NULL;
    
    TLL_RUN_LOCK
    
    pizLinklistPtrAtIndex(x->run, 0, (void **)&event);
    
    while (event) {
        pizLinklistNextWithPtr(x->run, (void *)event, (void **)&nextEvent);
        pizLinklistAppend(x->runCopy, (void *)event);
        pizLinklistChuckWithPtr(x->run, (void *)event);
        event = nextEvent;
    }
    
    TLL_RUN_UNLOCK
    
    pizLinklistPtrAtIndex(x->runCopy, 0, (void **)&event);
          
    while (event) {
    //
    long argc;
    long *argv = NULL;
    PIZEventCode code = PIZ_MSG_NONE;
    long a[ ] = { 0, 0, 0, 0 };
    
    pizLinklistNextWithPtr(x->runCopy, (void *)event, (void **)&nextEvent);
    
    pizEventCode(event, &code);
        
    switch (code) {
    //
    case PIZ_NOTIFY_PLAYED :
        pizEventData(event, &argc, &argv);
        a[0] = argv[PIZ_EVENT_DATA_PITCH];
        a[1] = argv[PIZ_EVENT_DATA_VELOCITY];
        a[2] = argv[PIZ_EVENT_DATA_DURATION];
        a[2] = (long)(a[2] * (PIZ_AGENT_CONSTANT_BPM_MS / argv[PIZ_EVENT_DATA_BPM]));
        a[3] = argv[PIZ_EVENT_DATA_CHANNEL];
        atom_setlong_array(4, x->played, 4, a);
        outlet_list(x->left, TLL_SYM_LIST, 4, x->played); 
        break;
        
    case PIZ_NOTIFY_DUMPED :
        pizEventData(event, &argc, &argv);
        atom_setlong_array(5, x->dumped, 5, argv);
        outlet_anything(x->middleLeft, TLL_SYM_NOTE, 5, x->dumped); 
        break;

    case PIZ_NOTIFY_WILL_DUMP :
        outlet_anything(x->middleLeft, TLL_SYM_CLEAR, 0, NULL);
        break;

    case PIZ_NOTIFY_WILL_END :
        outlet_bang(x->middleRight);
        break;
    
    case PIZ_NOTIFY_END :
        pizEventTime(event, &x->time);
        outlet_anything(x->middle, TLL_SYM_END, 1, &x->link);
        break;
    
    default :
        tralala_parseEvent(x, event);
        break;
    //
    }
    
    pizLinklistRemoveWithPtr(x->runCopy, (void *)event);
    event = nextEvent;
    //
    }
}

void tralala_daemonTask (t_tll *x)
{
    PIZNano m, n;
    PIZTime now, t;
    PIZEvent *event = NULL;
    PIZEvent *nextEvent = NULL;
    bool dirty = false;
    
    pizTimeSet(&now);
        
    TLL_DAEMON_LOCK
    
    pizLinklistPtrAtIndex(x->daemon, 0, (void **)&event);
    
    while (event) {
    //
    long argc;
    long *argv = NULL;

    pizLinklistNextWithPtr(x->daemon, (void *)event, (void **)&nextEvent);
    
    pizEventTime(event, &t);
    pizEventData(event, &argc, &argv);
    
    pizNanoSet(&n, argv[PIZ_EVENT_DATA_DURATION] * (PIZ_AGENT_CONSTANT_BPM_NS / argv[PIZ_EVENT_DATA_BPM]));
    pizTimeAddNano(&t, &n);

    if (pizTimeElapsedNano(&now, &t, &m)) {
       pizLinklistRemoveWithPtr(x->daemon, (void *)event);
       dirty = true;
    }
        
    event = nextEvent;
    //
    }
       
    TLL_DAEMON_UNLOCK
    
    if (dirty) {
        TLL_FLAG_SET(TLL_DIRTY_RUN)
        jbox_redraw((t_jbox *)x);
    }
            
    if (TLL_FLAG_TRUE(TLL_FLAG_DAEMON)) {
        clock_fdelay(x->daemonClock, TLL_CLOCK_DAEMON_WORK);
    } else {
        clock_fdelay(x->daemonClock, TLL_CLOCK_DAEMON_IDLE);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void tralala_play(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_MSG_PLAY, argc, argv, TLL_FLAG_NONE);
}

void tralala_loop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_MSG_LOOP, argc, argv, TLL_FLAG_NONE);
}

void tralala_stop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_MSG_STOP, 0, NULL, TLL_FLAG_NONE);
}

void tralala_unloop(t_tll *x, t_symbol *s, long argc, t_atom *argv) 
{   
    tralala_send(x, PIZ_MSG_UNLOOP, 0, NULL, TLL_FLAG_NONE);
}

void tralala_list(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    tralala_parseEntry(x, s, argc, argv, TLL_FLAG_NONE);
}

void tralala_anything(t_tll *x, t_symbol *s, long argc, t_atom *argv)
{
    tralala_parseEntry(x, s, argc, argv, TLL_FLAG_NONE);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

void tralala_send(t_tll *x, PIZEventCode code, long argc, t_atom *argv, ulong flags)
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
    
    if (flags & TLL_FLAG_RUN) {
        pizEventSetType(event, PIZ_EVENT_RUN);
    }
    
    pizAgentDoEvent(x->agent, event);
    
    tralala_switchDaemon(x, code);
    //
    }
}

void tralala_switchDaemon(t_tll *x, PIZEventCode code)
{
    if ((code == PIZ_NOTIFY_END) || (code == PIZ_MSG_STOP)) {
        TLL_FLAG_UNSET(TLL_FLAG_DAEMON)

    } else if ((code == PIZ_NOTIFY_PLAYED) && (TLL_FLAG_FALSE(TLL_FLAG_DAEMON))) {
        TLL_FLAG_SET(TLL_FLAG_DAEMON)
        clock_fdelay(x->daemonClock, TLL_CLOCK_DAEMON_WORK); 
    }
}

void tralala_keepAttributes(t_tll *x, t_symbol *name, t_dictionary *t)
{
    t_dictionary *d = NULL;
    t_symbol *s[ ] = { 
        TLL_SYM_BPM, 
        TLL_SYM_CHANCE, 
        TLL_SYM_VELOCITY, 
        TLL_SYM_CHANNEL, 
        TLL_SYM_CHORD,
        TLL_SYM_CELL, 
        TLL_SYM_VALUE, 
        TLL_SYM_SCALE, 
        TLL_SYM_MUTE
    };
    
    if (!(dictionary_getdictionary(x->data, name, (t_object **)&d))) {
        dictionary_copyentries(d, t, s);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_symbol *tralala_unique(t_tll *x)
{
    char string[16];    
    snprintf_zero(string, 16, "%s%ld", "_tll_", x->identifier);
    return gensym(string);
}

t_symbol *tralala_slotName(long argc, t_atom *argv) 
{
    t_symbol *s = TLL_SYM_UNTITLED;
    
    if (argc && argv) {
        long k = 0;
        char *p = NULL;
        
        atom_gettext(argc, argv, &k, &p, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
        
        if (p) {
            s = gensym(p); 
            sysmem_freeptr(p);           
        }
    }
    
    return s;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
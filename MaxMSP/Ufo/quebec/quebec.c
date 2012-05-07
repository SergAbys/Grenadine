/*
 *  quebec.c
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  May 7, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "jpatcher_api.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _quebec {
    t_object        ob;
    t_object        *patcher;
    t_object        *firstview;
    t_symbol        *title;
    void            *leftOutlet;
    void            *rightOutlet;
    } t_quebec;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *quebec_new        (t_symbol *s, long argc, t_atom *argv);
void quebec_free        (t_quebec *x);
void quebec_assist      (t_quebec *x, void *b, long m, long a, char *s);
void quebec_notify      (t_quebec *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void quebec_bang        (t_quebec *x);
void quebec_attachView  (t_quebec *x);

void quebec_dumpTitle   (t_quebec *x);
void quebec_dumpLocked  (t_quebec *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class *quebec_class;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new ("quebec", (method)quebec_new, (method)quebec_free, (long)sizeof(t_quebec), 0L, A_GIMME, 0);
    
    class_addmethod (c, (method)quebec_assist,  "assist", A_CANT, 0);
    class_addmethod (c, (method)quebec_notify,  "notify", A_CANT, 0); 
    class_addmethod (c, (method)quebec_bang,    "bang", 0); 

    class_register (CLASS_BOX, c); 
    
    quebec_class = c;
    
    return 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *quebec_new (t_symbol *s, long argc, t_atom *argv)
{
    t_quebec *x = NULL;
    
    if (x = (t_quebec *)object_alloc (quebec_class)) {       
        x->patcher      = NULL;
        x->firstview    = NULL;
        x->title        = NULL;
        
        x->rightOutlet  = outlet_new ((t_object *)x, NULL);
        x->leftOutlet   = bangout ((t_object *)x);
        
        object_obex_lookup (x, gensym ("#P"), &x->patcher); 
        
        if (x->patcher) {
            object_attach_byptr_register (x, x->patcher, CLASS_NOBOX);
        }
    }
            
    return x;
}

void quebec_free (t_quebec *x)
{ 
    if (x->patcher) {
        object_detach_byptr (x, x->patcher);
    }
        
    if (x->firstview) {
        object_detach_byptr (x, x->firstview);
    }
}

void quebec_assist (t_quebec *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf (s, "(bang)");
    } else {   
        switch (a) {
            case 0 : sprintf (s, "(bang) Resized"); break;
            case 1 : sprintf (s, "(anything) Notifications"); break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void quebec_notify (t_quebec *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (msg == gensym ("attr_modified")) { 
        t_symbol *attr_name = NULL;
                
        attr_name = (t_symbol *)object_method (data, gensym ("getname"));
                
        if (sender == x->patcher) {
            if (attr_name == gensym ("name")) {
                quebec_dumpTitle (x);
            }
        } else if (sender == x->firstview) {
            if (attr_name == gensym ("locked")) {
                quebec_dumpLocked (x);
            }
            if (attr_name == gensym ("rect")) {
                outlet_bang (x->leftOutlet);
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void quebec_bang (t_quebec *x)
{
    if (!x->firstview) {    
        defer_low (x, (method)quebec_attachView, NULL, 0, NULL);
    }
}

void quebec_attachView (t_quebec *x)
{
    if (x->patcher && (x->firstview = object_attr_getobj (x->patcher, gensym ("firstview")))) {
        object_attach_byptr_register (x, x->firstview, CLASS_NOBOX);
        quebec_dumpTitle (x);
        quebec_dumpLocked (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void quebec_dumpTitle (t_quebec *x)
{
    t_object *box = NULL;
                    
    if ((box = jpatcher_get_box (x->patcher)) && (object_classname (box) == gensym ("newobj"))) {
        t_object *textfield = NULL;
        
        if (textfield = object_attr_getobj (box, gensym ("textfield"))) {
            long argc = 0;
            long textSize = 0;
            t_atom *argv = NULL;
            char *text = NULL;
            
            object_method (textfield, gensym ("gettextptr"), &text, &textSize);
            atom_setparse (&argc, &argv, text);
            
            if (argc && argv) {
                if ((atom_gettype (argv + 1) == A_SYM) && (x->title != atom_getsym (argv + 1))) {
                    x->title = atom_getsym (argv + 1);
                    outlet_anything  (x->rightOutlet, gensym ("title"), 1, argv + 1);
                }
            
                sysmem_freeptr (argv);
            }
        }
    }
}

void quebec_dumpLocked (t_quebec *x)
{
    t_atom a;
    
    atom_setlong (&a, ((patcherview_get_locked (x->firstview)) != 0));
    
    outlet_anything  (x->rightOutlet, gensym ("locked"), 1, &a);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
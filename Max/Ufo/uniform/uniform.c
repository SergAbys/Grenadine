/*
 *  uniform.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "ext_systhread.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizFiniteState.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define MAXIMUM_SIZE_LIST   256

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define LOCK    systhread_mutex_lock(x->algorithmMutex);
#define UNLOCK  systhread_mutex_unlock(x->algorithmMutex);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _uniform {
    t_object            ob;
    long                *values;
    PIZFiniteState      *finiteState;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_uniform;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *uniform_new       (t_symbol *s, long argc, t_atom *argv);
void uniform_free       (t_uniform *x);
void uniform_assist     (t_uniform *x, void *b, long m, long a, char *s);

void uniform_learn      (t_uniform *x, t_symbol *s, long argc, t_atom *argv);
void uniform_int        (t_uniform *x, long n);
void uniform_clear      (t_uniform *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class *uniform_class;

int main(void)
{   
//
t_class *c = NULL;

c = class_new("uniform", (method)uniform_new, (method)uniform_free, (long)sizeof(t_uniform), 0L, A_GIMME, 0);

class_addmethod(c, (method)uniform_assist,      "assist",   A_CANT, 0); 
class_addmethod(c, (method)uniform_learn,       "learn",    A_GIMME, 0);
class_addmethod(c, (method)uniform_int,         "int",      A_LONG, 0);
class_addmethod(c, (method)uniform_clear,       "clear",    0);
class_addmethod(c, (method)object_obex_dumpout, "dumpout",  A_CANT, 0); 

class_register(CLASS_BOX, c); 

uniform_class = c;

return MAX_ERR_NONE;
//    
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *uniform_new(t_symbol *s, long argc, t_atom *argv)
{
    t_uniform *x = NULL;
    
    if (x = (t_uniform *)object_alloc(uniform_class)) {
    //
    long k = 0;
    
    if (argc && atom_gettype(argv) == A_LONG) {
        k = atom_getlong(argv);
    }
    
    x->values = (long *)sysmem_newptr(sizeof(long) * MAXIMUM_SIZE_LIST);
    x->finiteState = pizFiniteStateNew(1, &k);
                            
    if (x->values && x->finiteState) {
    
        x->rightOutlet  = outlet_new(x, NULL);
        object_obex_store((void *)x, gensym("dumpout"), (t_object *)x->rightOutlet);
        x->leftOutlet = listout((t_object *)x);
                
        systhread_mutex_new(&x->algorithmMutex, SYSTHREAD_MUTEX_NORMAL);
        
    } else {
        object_free(x);
        x = NULL;
    }
    //
    }
            
    return x;
}

void uniform_free(t_uniform *x)
{ 
    pizFiniteStateFree(x->finiteState);
        
    if (x->values) {
        sysmem_freeptr(x->values);
    }
        
    if (x->algorithmMutex) {
        systhread_mutex_free(x->algorithmMutex);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void uniform_assist(t_uniform *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf(s, "(int) learn clear");
        
    } else {   
        switch (a) {
            case 0 : sprintf(s, "(list) Navigate"); break;
            case 1 : sprintf(s, "Dumpout"); break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void uniform_learn(t_uniform *x, t_symbol *s, long argc, t_atom *argv)
{   
    LOCK
    
    atom_getlong_array(argc, argv, MIN(MAXIMUM_SIZE_LIST, argc), x->values);
    pizFiniteStateAdd(x->finiteState, MIN(MAXIMUM_SIZE_LIST, argc), x->values);
    
    UNLOCK
}

void uniform_int(t_uniform *x, long n)
{   
    char alloc;
    t_atom *argv = NULL;
    long argc = 0;

    if ((n > 0) && (atom_alloc_array(MIN(n, MAXIMUM_SIZE_LIST), &argc, &argv, &alloc) == MAX_ERR_NONE)) {
    //
    PIZError err = PIZ_ERROR;
        
    LOCK

    if (pizFiniteStateCount(x->finiteState)) {
        if (!(err = pizFiniteStateProceed(x->finiteState, argc, x->values))) {
            atom_setlong_array(argc, argv, argc, x->values);
        }
    }
    
    UNLOCK

    if (!err) {
        outlet_list(x->leftOutlet, NULL, argc, argv);
    }
        
    sysmem_freeptr(argv);
    //
    }
}
    
void uniform_clear(t_uniform *x)
{
    LOCK
    
    pizFiniteStateClear(x->finiteState);
    
    UNLOCK
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
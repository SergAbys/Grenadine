/*
 *  foxtrot.c
 *
 *  Created : 15/03/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  Last modified : 31/01/12.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "ext_systhread.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizMaxMSP.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define MAXIMUM_LIST_SIZE       256

#define DEFAULT_PERSISTENCE     0.5

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _foxtrot {
    t_object            ob;
    double              persistence;
    long                *values;
    PIZMarkovModel      *markovModel;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_foxtrot;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        *foxtrot_new            (t_symbol *s, long argc, t_atom *argv);
void        foxtrot_free            (t_foxtrot *x);
void        foxtrot_assist          (t_foxtrot *x, void *b, long m, long a, char *s);

t_max_err   foxtrot_setPersistence  (t_foxtrot *x, t_object *attr, long argc, t_atom *argv);

void        foxtrot_learn           (t_foxtrot *x, t_symbol *s, long argc, t_atom *argv);
void        foxtrot_int             (t_foxtrot *x, long n);
void        foxtrot_dump            (t_foxtrot *x, long n);
void        foxtrot_clear           (t_foxtrot *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class  *foxtrot_class;

static t_symbol *foxtrot_sym_dumpout    = NULL;
static t_symbol *foxtrot_sym_start      = NULL;
static t_symbol *foxtrot_sym_transitions = NULL;
static t_symbol *foxtrot_sym_emissions  = NULL;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new ("foxtrot", (method)foxtrot_new, (method)foxtrot_free,
        (long)sizeof(t_foxtrot), 0L, A_GIMME, 0);

    class_addmethod (c, (method)foxtrot_assist,         "assist",   A_CANT, 0);
    class_addmethod (c, (method)foxtrot_learn,          "learn",    A_GIMME, 0);
    class_addmethod (c, (method)foxtrot_int,            "int",      A_LONG, 0);
    class_addmethod (c, (method)foxtrot_dump,           "dump",     A_DEFLONG, 0);
    class_addmethod (c, (method)foxtrot_clear,          "clear",    0);
    class_addmethod (c, (method)object_obex_dumpout,    "dumpout",  A_CANT, 0);
    
    CLASS_ATTR_DOUBLE       (c, "persistence",      0, t_foxtrot, persistence);
    CLASS_ATTR_LABEL        (c, "persistence",      0, "Learning Persistence");
    CLASS_ATTR_ACCESSORS    (c, "persistence",      NULL, foxtrot_setPersistence);
    CLASS_ATTR_FILTER_MIN   (c, "persistence",      0.);
    
    foxtrot_sym_dumpout     = gensym ("dumpout");
    foxtrot_sym_start       = gensym ("start");
    foxtrot_sym_transitions = gensym ("transitions");
    foxtrot_sym_emissions   = gensym ("emissions");

    class_register  (CLASS_BOX, c); 
    
    foxtrot_class = c;
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *foxtrot_new (t_symbol *s, long argc, t_atom *argv)
{
    t_foxtrot *x = NULL;
    
    if (x = (t_foxtrot *)object_alloc (foxtrot_class)) 
        {
            long k[2] = {0, 0};
            
            if (argc && atom_gettype (argv) == A_LONG)
                {
                    k[0] = atom_getlong (argv);
                    
                    if (argc == 2 && atom_gettype (argv + 1) == A_LONG)
                        {
                            k[1] = atom_getlong (argv + 1);
                        }
                }
            
            x->values = (long *)sysmem_newptr (sizeof(long) * MAXIMUM_LIST_SIZE);
            x->markovModel = pizMarkovModelNew (2, k);
                                    
            if (x->values && x->markovModel)
                {
                    x->persistence = DEFAULT_PERSISTENCE;
                    
                    x->rightOutlet  = outlet_new (x, NULL);
                    object_obex_store ((void *)x, foxtrot_sym_dumpout, (t_object *)x->rightOutlet);
                    
                    x->leftOutlet = listout ((t_object *)x);
                            
                    systhread_mutex_new (&x->algorithmMutex, SYSTHREAD_MUTEX_NORMAL);
                    
                    attr_args_process (x, argc, argv);
                }
            else
                {
                    object_free (x);
                    x = NULL;
                }
        }
            
    return x;
}

void foxtrot_free (t_foxtrot *x)
{ 
    pizMarkovModelFree (x->markovModel);
        
    if (x->values)
        {
            sysmem_freeptr (x->values);
        }
        
    if (x->algorithmMutex)
        {
            systhread_mutex_free (x->algorithmMutex);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void foxtrot_assist (t_foxtrot *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) 
        { 
            sprintf (s, "(int) learn clear dump");
        } 
    else 
        {   
            switch (a) {
                case 0 :
                    sprintf (s, "(list) Navigate");
                    break;
                case 1 :
                    sprintf (s, "Dumpout");
                    break;
            }
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

t_max_err foxtrot_setPersistence (t_foxtrot *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->persistence = atom_getfloat (argv);
            pizMarkovModelSetPersistence (x->markovModel, x->persistence);
        }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void foxtrot_learn (t_foxtrot *x, t_symbol *s, long argc, t_atom *argv)           
{   
    systhread_mutex_lock (&x->algorithmMutex);
    
    atom_getlong_array (argc, argv, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    pizMarkovModelAdd (x->markovModel, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

void foxtrot_int (t_foxtrot *x, long n)
{
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_LIST_SIZE), &argc, &argv, &alloc) == MAX_ERR_NONE)) 
        {
            long err = PIZ_ERROR;
                
            systhread_mutex_lock (&x->algorithmMutex);
    
            if (pizMarkovModelCount (x->markovModel) &&
                (!(err = pizMarkovModelProceed (x->markovModel, argc, x->values))))
                {
                    atom_setlong_array (argc, argv, argc, x->values);
                }
            
            systhread_mutex_unlock (&x->algorithmMutex);
    
            if (!err)
                {
                    outlet_list (x->leftOutlet, NULL, argc, argv);
                }
                
            sysmem_freeptr (argv);
        }
}
    
void foxtrot_clear (t_foxtrot *x)
{
    systhread_mutex_lock (&x->algorithmMutex);
    
    pizMarkovModelClear (x->markovModel);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

void foxtrot_dump (t_foxtrot *x, long n)
{
    char            alloc;
    long            size;
    long            argc = 0;
    long            err = PIZ_GOOD;
    t_atom          *argv = NULL;
    PIZGrowingArray *values = pizGrowingArrayNew (256);
    
    systhread_mutex_lock (&x->algorithmMutex);
    err = pizMarkovModelEncodeNodeToArray (x->markovModel, n, values);
    systhread_mutex_unlock (&x->algorithmMutex);
    
    if (!err)
        {
            long t = pizGrowingArrayValueAtIndex (values, PIZ_MARKOV_MODEL_TRANSITIONS);
            long e = pizGrowingArrayValueAtIndex (values, PIZ_MARKOV_MODEL_EMISSIONS);
    
            size = MAX (t, e);
            
            if (atom_alloc_array (size, &argc, &argv, &alloc) == MAX_ERR_NONE)
                {
                    long *ptr = pizGrowingArrayPtr (values);

                    atom_setlong (argv, pizGrowingArrayValueAtIndex (values, PIZ_MARKOV_MODEL_START));
                    outlet_anything (x->rightOutlet, foxtrot_sym_start, 1, argv);
                    
                    atom_setlong_array (t, argv, t, ptr + PIZ_MARKOV_MODEL_DATA);
                    outlet_anything (x->rightOutlet, foxtrot_sym_transitions, t, argv);
                    
                    atom_setlong_array (e, argv, e, ptr + PIZ_MARKOV_MODEL_DATA + t);
                    outlet_anything (x->rightOutlet, foxtrot_sym_emissions, e, argv);

                    sysmem_freeptr (argv);
                }
        }
    
    pizGrowingArrayFree (values);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
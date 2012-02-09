/*
 *  yankee.c
 *
 *  Created : 10/05/11.
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
#define DEFAULT_LAMBDA          2
#define DEFAULT_ALPHA           0.5
#define DEFAULT_BETA            0.1
#define DEFAULT_EPSILON1        0.5
#define DEFAULT_EPSILON2        0.25
#define DEFAULT_KAPPA           10

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _yankee {
    t_object            ob;
    long                lambda;
    long                kappa;
    double              epsilon1;
    double              epsilon2;
    double              alpha;
    double              beta;
    long                *values; 
    PIZNeuralGas        *neuralGas;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_yankee;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        *yankee_new         (t_symbol *s, long argc, t_atom *argv);
void        yankee_free         (t_yankee *x);
void        yankee_assist       (t_yankee *x, void *b, long m, long a, char *s);

t_max_err   yankee_setLambda    (t_yankee *x, t_object *attr, long argc, t_atom *argv);
t_max_err   yankee_setEpsilon1  (t_yankee *x, t_object *attr, long argc, t_atom *argv);
t_max_err   yankee_setEpsilon2  (t_yankee *x, t_object *attr, long argc, t_atom *argv);
t_max_err   yankee_setAlpha     (t_yankee *x, t_object *attr, long argc, t_atom *argv);
t_max_err   yankee_setBeta      (t_yankee *x, t_object *attr, long argc, t_atom *argv);
t_max_err   yankee_setKappa     (t_yankee *x, t_object *attr, long argc, t_atom *argv);

void        yankee_learn        (t_yankee *x, t_symbol *s, long argc, t_atom *argv);
void        yankee_int          (t_yankee *x, long n);
void        yankee_dump         (t_yankee *x, long n);
void        yankee_clear        (t_yankee *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class  *yankee_class;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new   ("yankee", (method)yankee_new, (method)yankee_free, (long)sizeof(t_yankee), 0L, A_GIMME, 0);

    class_addmethod (c, (method)yankee_assist,          "assist",   A_CANT, 0); 
    class_addmethod (c, (method)yankee_learn,           "learn",    A_GIMME, 0);
    class_addmethod (c, (method)yankee_int,             "int",      A_LONG, 0);
    class_addmethod (c, (method)yankee_dump,            "dump",     A_DEFLONG, 0);
    class_addmethod (c, (method)yankee_clear,           "clear",    0);
    class_addmethod (c, (method)object_obex_dumpout,    "dumpout",  A_CANT, 0);
    
    CLASS_ATTR_LONG         (c, "kappa",        0, t_yankee, kappa);
    CLASS_ATTR_LABEL        (c, "kappa",        0, "Utility Threshold");
    CLASS_ATTR_ACCESSORS    (c, "kappa",        NULL, yankee_setKappa);
    CLASS_ATTR_FILTER_MIN   (c, "kappa",        1);
    
    CLASS_ATTR_LONG         (c, "lambda",       0, t_yankee, lambda);
    CLASS_ATTR_LABEL        (c, "lambda",       0, "New Node Rate");
    CLASS_ATTR_ACCESSORS    (c, "lambda",       NULL, yankee_setLambda);
    CLASS_ATTR_FILTER_MIN   (c, "lambda",       1);
    
    CLASS_ATTR_DOUBLE       (c, "epsilon1",     0, t_yankee, epsilon1);
    CLASS_ATTR_LABEL        (c, "epsilon1",     0, "Move Winner Factor");
    CLASS_ATTR_ACCESSORS    (c, "epsilon1",     NULL, yankee_setEpsilon1);
    CLASS_ATTR_FILTER_CLIP  (c, "epsilon1",     0., 1.);
    
    CLASS_ATTR_DOUBLE       (c, "epsilon2",     0, t_yankee, epsilon2);
    CLASS_ATTR_LABEL        (c, "epsilon2",     0, "Move Neighbours Factor");
    CLASS_ATTR_ACCESSORS    (c, "epsilon2",     NULL, yankee_setEpsilon2);
    CLASS_ATTR_FILTER_CLIP  (c, "epsilon2",     0., 1.);
    
    CLASS_ATTR_DOUBLE       (c, "alpha",        0, t_yankee, alpha);
    CLASS_ATTR_LABEL        (c, "alpha",        0, "Decrease Generators Error Factor");
    CLASS_ATTR_ACCESSORS    (c, "alpha",        NULL, yankee_setAlpha);
    CLASS_ATTR_FILTER_CLIP  (c, "alpha",        0., 1.);
    
    CLASS_ATTR_DOUBLE       (c, "beta",         0, t_yankee, beta);
    CLASS_ATTR_LABEL        (c, "beta",         0, "Decrease Global Error Factor");
    CLASS_ATTR_ACCESSORS    (c, "beta",         NULL, yankee_setBeta);
    CLASS_ATTR_FILTER_CLIP  (c, "beta",         0., 1.);
    
    CLASS_ATTR_ORDER        (c, "lambda",       0, "1");
    CLASS_ATTR_ORDER        (c, "epsilon1",     0, "2");
    CLASS_ATTR_ORDER        (c, "epsilon2",     0, "3");
    CLASS_ATTR_ORDER        (c, "alpha",        0, "4");
    CLASS_ATTR_ORDER        (c, "beta",         0, "5");
    CLASS_ATTR_ORDER        (c, "kappa",        0, "6");
    
    class_register  (CLASS_BOX, c); 
    
    yankee_class = c;
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *yankee_new (t_symbol *s, long argc, t_atom *argv)
{
    t_yankee *x = NULL;
    
    if (x = (t_yankee *)object_alloc (yankee_class)) 
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
            x->neuralGas = pizNeuralGasNew (2, k);
                                    
            if (x->values && x->neuralGas)
                {
                    x->lambda       = DEFAULT_LAMBDA;
                    x->epsilon1     = DEFAULT_EPSILON1;
                    x->epsilon2     = DEFAULT_EPSILON2;
                    x->alpha        = DEFAULT_ALPHA;
                    x->beta         = DEFAULT_BETA;
                    x->kappa        = DEFAULT_KAPPA;
                    
                    x->rightOutlet  = outlet_new (x, NULL);
                    object_obex_store ((void *)x, gensym ("dumpout"), (t_object *)x->rightOutlet);
                    
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

void yankee_free (t_yankee *x)
{ 
    pizNeuralGasFree (x->neuralGas);
        
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
    
void yankee_assist (t_yankee *x, void *b, long m, long a, char *s)
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

t_max_err yankee_setLambda (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->lambda = atom_getlong (argv);
            pizNeuralGasSetLambda (x->neuralGas, x->lambda);
        }

    return MAX_ERR_NONE;
}

t_max_err yankee_setEpsilon1 (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->epsilon1 = atom_getfloat (argv);
            pizNeuralGasSetEpsilon1 (x->neuralGas, x->epsilon1);
        }

    return MAX_ERR_NONE;
}

t_max_err yankee_setEpsilon2 (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->epsilon2 = atom_getfloat (argv);
            pizNeuralGasSetEpsilon2 (x->neuralGas, x->epsilon2);
        }

    return MAX_ERR_NONE;
}

t_max_err yankee_setAlpha (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->alpha = atom_getfloat (argv);
            pizNeuralGasSetAlpha (x->neuralGas, x->alpha);
        }

    return MAX_ERR_NONE;
}

t_max_err yankee_setBeta (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->beta = atom_getfloat (argv);
            pizNeuralGasSetBeta (x->neuralGas, x->beta);
        }

    return MAX_ERR_NONE;
}

t_max_err yankee_setKappa (t_yankee *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->kappa = atom_getlong (argv);
            pizNeuralGasSetKappa (x->neuralGas, (double)x->kappa);
        }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void yankee_learn (t_yankee *x, t_symbol *s, long argc, t_atom *argv)           
{   
    systhread_mutex_lock (&x->algorithmMutex);
    
    atom_getlong_array (argc, argv, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    pizNeuralGasAdd (x->neuralGas, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

void yankee_int (t_yankee *x, long n)
{
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_LIST_SIZE), &argc, &argv, &alloc) == MAX_ERR_NONE)) 
        {
            long err = PIZ_ERROR;
                
            systhread_mutex_lock (&x->algorithmMutex);
    
            if (pizNeuralGasCount (x->neuralGas) &&
                (!(err = pizNeuralGasProceed (x->neuralGas, argc, x->values))))
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
    
void yankee_clear (t_yankee *x)
{
    systhread_mutex_lock (&x->algorithmMutex);
    
    pizNeuralGasClear (x->neuralGas);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

void yankee_dump (t_yankee *x, long n)
{
    char            alloc;
    long            size;
    long            argc = 0;
    long            err = PIZ_GOOD;
    t_atom          *argv = NULL;
    PIZGrowingArray *values = pizGrowingArrayNew (4);
    
    systhread_mutex_lock (&x->algorithmMutex);
    err = pizNeuralGasEncodeVectorToArray (x->neuralGas, n, values);
    systhread_mutex_unlock (&x->algorithmMutex);
    
    if (!err)
        {
            size = pizGrowingArrayCount (values);
    
            if (atom_alloc_array (size, &argc, &argv, &alloc) == MAX_ERR_NONE)
                {
                    long *ptr = pizGrowingArrayPtr (values);

                    atom_setlong_array (argc, argv, argc, ptr);
                                    
                    outlet_list (x->rightOutlet, NULL, argc, argv);
                        
                    sysmem_freeptr (argv);
                }
        }
    
    pizGrowingArrayFree (values);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
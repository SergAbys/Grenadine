/*
 *  zoulou.c
 *
 *  Created : 10/05/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  Last modified : 02/09/11.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include    "ext.h"
#include    "ext_obex.h"
#include    "ext_systhread.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include    "pizFactorOracle.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define     MAXIMUM_LIST_SIZE           256

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _zoulou {
    t_object            ob;
    long                backwardThreshold;
    double              straightRatio;
    long                *values;
    PIZFactorOracle     *factorOracle;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_zoulou;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        *zoulou_new                     (t_symbol *s, long argc, t_atom *argv);
void        zoulou_free                     (t_zoulou *x);
void        zoulou_assist                   (t_zoulou *x, void *b, long m, long a, char *s);

t_max_err   zoulou_setStraightRatio         (t_zoulou *x, t_object *attr, long argc, t_atom *argv);
t_max_err   zoulou_setBackwardThreshold     (t_zoulou *x, t_object *attr, long argc, t_atom *argv);

void        zoulou_learn                    (t_zoulou *x, t_symbol *s, long argc, t_atom *argv);
void        zoulou_int                      (t_zoulou *x, long n);
void        zoulou_dump                     (t_zoulou *x, long n);
void        zoulou_clear                    (t_zoulou *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class  *zoulou_class;

static t_symbol *zoulou_sym_dumpout = NULL;
static t_symbol *zoulou_sym_node = NULL;
static t_symbol *zoulou_sym_ref = NULL;
static t_symbol *zoulou_sym_lrs = NULL;
static t_symbol *zoulou_sym_arc = NULL;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new   ("zoulou", (method)zoulou_new, (method)zoulou_free, (long)sizeof(t_zoulou), 0L, A_GIMME, 0);

    class_addmethod (c, (method)zoulou_assist,              "assist",   A_CANT, 0); 
    class_addmethod (c, (method)zoulou_learn,               "learn",    A_GIMME);
    class_addmethod (c, (method)zoulou_int,                 "int",      A_LONG, 0);
    class_addmethod (c, (method)zoulou_dump,                "dump",     A_DEFLONG, 0);
    class_addmethod (c, (method)zoulou_clear,               "clear",    0);
    class_addmethod (c, (method)object_obex_dumpout,        "dumpout",  A_CANT, 0); 

    CLASS_ATTR_DOUBLE       (c, "straightratio",     0, t_zoulou, straightRatio);
    CLASS_ATTR_LABEL        (c, "straightratio",     0, "Straight Ratio");
    CLASS_ATTR_ACCESSORS    (c, "straightratio",     NULL, zoulou_setStraightRatio);
    CLASS_ATTR_FILTER_CLIP  (c, "straightratio",     0., 1.);
    
    CLASS_ATTR_LONG         (c, "backwardthreshold", 0, t_zoulou, backwardThreshold);
    CLASS_ATTR_LABEL        (c, "backwardthreshold", 0, "Backward Threshold");
    CLASS_ATTR_ACCESSORS    (c, "backwardthreshold", NULL, zoulou_setBackwardThreshold);
    CLASS_ATTR_FILTER_MIN   (c, "backwardthreshold", 0);
    
    CLASS_ATTR_ORDER        (c, "straightratio",     0, "1");
    CLASS_ATTR_ORDER        (c, "backwardthreshold", 0, "2");

    zoulou_sym_dumpout      = gensym ("dumpout");
    zoulou_sym_node         = gensym ("node");
    zoulou_sym_ref          = gensym ("ref");
    zoulou_sym_lrs          = gensym ("lrs");
    zoulou_sym_arc          = gensym ("arc");
    
    class_register  (CLASS_BOX, c); 
    
    zoulou_class = c;
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *zoulou_new (t_symbol *s, long argc, t_atom *argv)
{
    t_zoulou *x = NULL;

    if (x = (t_zoulou *)object_alloc (zoulou_class)) 
        {
            x->values = (long *)sysmem_newptr (sizeof(long) * MAXIMUM_LIST_SIZE);
            x->factorOracle = pizFactorOracleNew (0, NULL);
            
            if (x->values && x->factorOracle)
                {
                    x->straightRatio        = PIZ_FACTOR_ORACLE_DEFAULT_STRAIGHT_RATIO;
                    x->backwardThreshold    = PIZ_FACTOR_ORACLE_DEFAULT_BACKWARD_THRESHOLD;
                    
                    x->rightOutlet  = outlet_new (x, NULL);
                    object_obex_store ((void *)x, zoulou_sym_dumpout, (t_object *)x->rightOutlet);
                    
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

void zoulou_free (t_zoulou *x)
{       
    if (x->values)
        {
            sysmem_freeptr (x->values);
        }
    
    pizFactorOracleFree (x->factorOracle);
        
    if (x->algorithmMutex)
        {
            systhread_mutex_free (x->algorithmMutex);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void zoulou_assist (t_zoulou *x, void *b, long m, long a, char *s)
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

t_max_err zoulou_setStraightRatio (t_zoulou *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->straightRatio = atom_getfloat (argv);
            pizFactorOracleSetStraightRatio (x->factorOracle, x->straightRatio);
        }

    return MAX_ERR_NONE;
}

t_max_err zoulou_setBackwardThreshold (t_zoulou *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv)
        {
            x->backwardThreshold = atom_getlong (argv);
            pizFactorOracleSetBackwardThreshold (x->factorOracle, x->backwardThreshold);
        }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void zoulou_learn (t_zoulou *x, t_symbol *s, long argc, t_atom *argv)          
{   
    systhread_mutex_lock (&x->algorithmMutex);
    
    atom_getlong_array (argc, argv, MIN (argc, MAXIMUM_LIST_SIZE), x->values);
    pizFactorOracleAdd (x->factorOracle, MIN (argc, MAXIMUM_LIST_SIZE), x->values);
    
    systhread_mutex_unlock (&x->algorithmMutex);
}

void zoulou_int (t_zoulou *x, long n)
{   
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_LIST_SIZE), &argc, &argv, &alloc) == MAX_ERR_NONE))
        {
            long err = PIZ_ERROR;
                
            systhread_mutex_lock (&x->algorithmMutex);
    
            if (pizFactorOracleCount (x->factorOracle) &&
                (!(err = pizFactorOracleProceed (x->factorOracle, argc, x->values))))
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
    
void zoulou_clear (t_zoulou *x)
{
    systhread_mutex_lock (&x->algorithmMutex);
    
    pizFactorOracleClear (x->factorOracle);

    systhread_mutex_unlock (&x->algorithmMutex);
}

void zoulou_dump (t_zoulou *x, long n)
{
    long err = PIZ_GOOD;
    PIZGrowingArray *values = pizGrowingArrayNew (4);

    systhread_mutex_lock (&x->algorithmMutex);
    err = pizFactorOracleEncodeNodeToArray (x->factorOracle, n, values);
    systhread_mutex_unlock (&x->algorithmMutex);
    
    if (!err)
        {
            long    i, k;
            long    ref;
            long    lrs;
            t_atom  result[4];
            
            ref = pizGrowingArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_ENCODE_REFER);
            lrs = pizGrowingArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_ENCODE_LRS);
            k   = pizGrowingArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_ENCODE_ARCS);
            
            atom_setlong        (result, n);
            atom_setsym         (result + 1, zoulou_sym_ref);
            atom_setlong        (result + 2, ref);
            outlet_anything     (x->rightOutlet, zoulou_sym_node, 3, result);
            
            atom_setsym         (result + 1, zoulou_sym_lrs);
            atom_setlong        (result + 2, lrs);
            outlet_anything     (x->rightOutlet, zoulou_sym_node, 3, result);
            
            atom_setsym         (result + 1, zoulou_sym_arc);
            
            for (i = 0; i < k; i++)
                {
                    atom_setlong (result + 2, pizGrowingArrayValueAtIndex 
                            (values, PIZ_FACTOR_ORACLE_ENCODE_ARCS + 1 + i));
                    atom_setlong (result + 3, pizGrowingArrayValueAtIndex 
                            (values, PIZ_FACTOR_ORACLE_ENCODE_ARCS + 1 + i + k));
        
                    outlet_anything (x->rightOutlet, zoulou_sym_node, 4, result);
                }
        }
    
    pizGrowingArrayFree (values);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
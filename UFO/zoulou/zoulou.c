/*
 *  zoulou.c
 *
 *  Created : 10/05/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 * April 11, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "ext_systhread.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizFactorOracle.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define MAXIMUM_LIST_SIZE               256
#define DEFAULT_BACKWARD_THRESHOLD      2
#define DEFAULT_STRAIGHT_RATIO          0.25

#define PIZ_FACTOR_ORACLE_REFER         0
#define PIZ_FACTOR_ORACLE_LRS           1
#define PIZ_FACTOR_ORACLE_ARCS          2
#define PIZ_FACTOR_ORACLE_DATA          3

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define LOCK    systhread_mutex_lock (&x->algorithmMutex);
#define UNLOCK  systhread_mutex_unlock (&x->algorithmMutex);

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

PIZ_INLINE void     pizFactorOracleSetBackwardThreshold     (PIZFactorOracle *x, long n);
PIZ_INLINE void     pizFactorOracleSetStraightRatio         (PIZFactorOracle *x, double f);
PIZ_INLINE PIZError pizFactorOracleEncodeToArray            (const PIZFactorOracle *x, long node, PIZArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class  *zoulou_class;

static t_symbol *zoulou_sym_dumpout = NULL;
static t_symbol *zoulou_sym_node = NULL;
static t_symbol *zoulou_sym_ref = NULL;
static t_symbol *zoulou_sym_lrs = NULL;
static t_symbol *zoulou_sym_arc = NULL;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new ("zoulou", (method)zoulou_new, (method)zoulou_free, (long)sizeof(t_zoulou), 0L, A_GIMME, 0);

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
#pragma mark -

void *zoulou_new (t_symbol *s, long argc, t_atom *argv)
{
    t_zoulou *x = NULL;

    if (x = (t_zoulou *)object_alloc (zoulou_class)) {
        x->values = (long *)sysmem_newptr (sizeof(long) * MAXIMUM_LIST_SIZE);
        x->factorOracle = pizFactorOracleNew (0, NULL);
        
        if (x->values && x->factorOracle) {
            x->straightRatio        = DEFAULT_STRAIGHT_RATIO;
            x->backwardThreshold    = DEFAULT_BACKWARD_THRESHOLD;
            
            x->rightOutlet  = outlet_new (x, NULL);
            object_obex_store ((void *)x, zoulou_sym_dumpout, (t_object *)x->rightOutlet);
            x->leftOutlet = listout ((t_object *)x);
                    
            systhread_mutex_new (&x->algorithmMutex, SYSTHREAD_MUTEX_NORMAL);
            
            attr_args_process (x, argc, argv);
                
        } else {
            object_free (x);
            x = NULL;
        }
    }
            
    return x;
}

void zoulou_free (t_zoulou *x)
{       
    if (x->values) {
        sysmem_freeptr (x->values);
    }
    
    pizFactorOracleFree (x->factorOracle);
        
    if (x->algorithmMutex) {
        systhread_mutex_free (x->algorithmMutex);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void zoulou_assist (t_zoulou *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { 
        sprintf (s, "(int) learn clear dump");
    } else {   
        switch (a) {
            case 0 : sprintf (s, "(list) Navigate"); break;
            case 1 : sprintf (s, "Dumpout"); break;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

t_max_err zoulou_setStraightRatio (t_zoulou *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->straightRatio = atom_getfloat (argv);
        pizFactorOracleSetStraightRatio (x->factorOracle, x->straightRatio);
    }

    return MAX_ERR_NONE;
}

t_max_err zoulou_setBackwardThreshold (t_zoulou *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->backwardThreshold = atom_getlong (argv);
        pizFactorOracleSetBackwardThreshold (x->factorOracle, x->backwardThreshold);
    }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void zoulou_learn (t_zoulou *x, t_symbol *s, long argc, t_atom *argv)          
{   
    LOCK
    
    atom_getlong_array (argc, argv, MIN (argc, MAXIMUM_LIST_SIZE), x->values);
    pizFactorOracleAdd (x->factorOracle, MIN (argc, MAXIMUM_LIST_SIZE), x->values);
    
    UNLOCK
}

void zoulou_int (t_zoulou *x, long n)
{   
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_LIST_SIZE), &argc, &argv, &alloc) == MAX_ERR_NONE)) {
        PIZError err = PIZ_ERROR;
            
        LOCK

        if (pizFactorOracleCount (x->factorOracle)) {
            if (!(err = pizFactorOracleProceed (x->factorOracle, argc, x->values))) {
                atom_setlong_array (argc, argv, argc, x->values);
            }
        }
        
        UNLOCK

        if (!err) {
            outlet_list (x->leftOutlet, NULL, argc, argv);
        }
            
        sysmem_freeptr (argv);
    }
}
    
void zoulou_clear (t_zoulou *x)
{
    LOCK
    
    pizFactorOracleClear (x->factorOracle);

    UNLOCK
}

void zoulou_dump (t_zoulou *x, long n)
{
    PIZError err = PIZ_GOOD;
    PIZArray *values = pizArrayNew (4);

    LOCK
    
    err = pizFactorOracleEncodeToArray (x->factorOracle, n, values);
    
    UNLOCK
    
    if (!err) {
        long    i, k, ref, lrs;
        t_atom  result[4];
        
        ref = pizArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_REFER);
        lrs = pizArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_LRS);
        k   = pizArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_ARCS);
        
        atom_setlong        (result, n);
        atom_setsym         (result + 1, zoulou_sym_ref);
        atom_setlong        (result + 2, ref);
        outlet_anything     (x->rightOutlet, zoulou_sym_node, 3, result);
        
        atom_setsym         (result + 1, zoulou_sym_lrs);
        atom_setlong        (result + 2, lrs);
        outlet_anything     (x->rightOutlet, zoulou_sym_node, 3, result);
        
        atom_setsym         (result + 1, zoulou_sym_arc);
        
        for (i = 0; i < k; i++) {
            atom_setlong (result + 2, pizArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_DATA + i));
            atom_setlong (result + 3, pizArrayValueAtIndex (values, PIZ_FACTOR_ORACLE_DATA + i + k));
    
            outlet_anything (x->rightOutlet, zoulou_sym_node, 4, result);
        }
    }
    
    pizArrayFree (values);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE void pizFactorOracleSetBackwardThreshold (PIZFactorOracle *x, long n)
{
    if (n >= 0) {
        x->backwardThreshold = n;
    }
}

PIZ_INLINE void pizFactorOracleSetStraightRatio (PIZFactorOracle *x, double f)
{
    if ((f >= 0.) && (f <= 1.)) {
        x->straightRatio = f;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE PIZError pizFactorOracleEncodeToArray (const PIZFactorOracle *x, long node, PIZArray *a)
{
    long     i, count;
    PIZError err = PIZ_ERROR;
    
    if ((node < x->index) && a) {
            err = PIZ_GOOD;
            
            err |= pizArrayAppend (a, x->nodes[node].referTo);
            err |= pizArrayAppend (a, x->nodes[node].lengthRepeatedSuffix);
            err |= pizArrayAppend (a, (count = pizArrayCount (x->nodes[node].arcDestinations)));
            
            for (i = 0; i < count; i++) {
                err |= pizArrayAppend (a, pizArrayValueAtIndex (x->nodes[node].arcDestinations, i));
            }
            
            for (i = 0; i < count; i++) {
                err |= pizArrayAppend (a, pizArrayValueAtIndex (x->nodes[node].arcValues, i));
            }
        }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
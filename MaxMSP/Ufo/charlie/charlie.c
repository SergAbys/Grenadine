/*
 *  charlie.c
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  May 12, 2012.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "ext_systhread.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizKohonenMap.h"
#include "pizArray.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define MAXIMUM_LIST_SIZE       256
#define DEFAULT_RANGE           10
#define DEFAULT_TRAINING        60
#define DEFAULT_STEP            1.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define LOCK    systhread_mutex_lock (&x->algorithmMutex);
#define UNLOCK  systhread_mutex_unlock (&x->algorithmMutex);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _charlie {
    t_object            ob;
    long                range;
    long                training;
    double              step;
    long                *values; 
    PIZKohonenMap       *kohonenMap;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_charlie;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        *charlie_new            (t_symbol *s, long argc, t_atom *argv);
void        charlie_free            (t_charlie *x);
void        charlie_assist          (t_charlie *x, void *b, long m, long a, char *s);

t_max_err   charlie_setRange        (t_charlie *x, t_object *attr, long argc, t_atom *argv);
t_max_err   charlie_setTraining     (t_charlie *x, t_object *attr, long argc, t_atom *argv);
t_max_err   charlie_setStep         (t_charlie *x, t_object *attr, long argc, t_atom *argv);

void        charlie_learn           (t_charlie *x, t_symbol *s, long argc, t_atom *argv);
void        charlie_int             (t_charlie *x, long n);
void        charlie_dump            (t_charlie *x, long n);
void        charlie_clear           (t_charlie *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE void     pizKohonenMapSetRange           (PIZKohonenMap *x, long n);
PIZ_INLINE void     pizKohonenMapSetTraining        (PIZKohonenMap *x, long n);
PIZ_INLINE void     pizKohonenMapSetStep            (PIZKohonenMap *x, double f);
PIZ_INLINE PIZError pizKohonenMapEncodeToArray      (const PIZKohonenMap *x, long n, PIZArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class  *charlie_class;

int main (void)
{   
//
t_class *c = NULL;

c = class_new ("charlie", (method)charlie_new, (method)charlie_free, (long)sizeof(t_charlie), 0L, A_GIMME, 0);

class_addmethod (c, (method)charlie_assist,         "assist",   A_CANT, 0);
class_addmethod (c, (method)charlie_learn,          "learn",    A_GIMME, 0);
class_addmethod (c, (method)charlie_int,            "int",      A_LONG, 0);
class_addmethod (c, (method)charlie_dump,           "dump",     A_DEFLONG, 0);
class_addmethod (c, (method)charlie_clear,          "clear",    0);
class_addmethod (c, (method)object_obex_dumpout,    "dumpout",  A_CANT, 0);

CLASS_ATTR_LONG         (c, "range",        0, t_charlie, range);
CLASS_ATTR_LABEL        (c, "range",        0, "Range of Neighborhood Influence");
CLASS_ATTR_ACCESSORS    (c, "range",        NULL, charlie_setRange);
CLASS_ATTR_FILTER_MIN   (c, "range",        1);

CLASS_ATTR_LONG         (c, "training",     0, t_charlie, training);
CLASS_ATTR_LABEL        (c, "training",     0, "Training Iterations");
CLASS_ATTR_ACCESSORS    (c, "training",     NULL, charlie_setTraining);
CLASS_ATTR_FILTER_MIN   (c, "training",     1);

CLASS_ATTR_DOUBLE       (c, "step",         0, t_charlie, step);
CLASS_ATTR_LABEL        (c, "step",         0, "Learning Step");
CLASS_ATTR_ACCESSORS    (c, "step",         NULL, charlie_setStep);
CLASS_ATTR_FILTER_MIN   (c, "step",         0.);

CLASS_ATTR_ORDER        (c, "range",        0, "1");
CLASS_ATTR_ORDER        (c, "training",     0, "2");
CLASS_ATTR_ORDER        (c, "step",         0, "3");

class_register  (CLASS_BOX, c); 

charlie_class = c;

return MAX_ERR_NONE;
//
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *charlie_new (t_symbol *s, long argc, t_atom *argv)
{
    t_charlie *x = NULL;
    
    if (x = (t_charlie *)object_alloc (charlie_class)) {
        long k[2] = {0, 0};
        
        if (argc && atom_gettype (argv) == A_LONG) {
            k[0] = atom_getlong (argv);
            
            if (argc == 2 && atom_gettype (argv + 1) == A_LONG) {
                k[1] = atom_getlong (argv + 1);
            }
        }
        
        x->values = (long *)sysmem_newptr (sizeof(long) * MAXIMUM_LIST_SIZE);
        x->kohonenMap = pizKohonenMapNew (2, k);
                                
        if (x->values && x->kohonenMap) {
            x->range    = DEFAULT_RANGE;
            x->training = DEFAULT_TRAINING;
            x->step     = DEFAULT_STEP;
            
            x->rightOutlet  = outlet_new (x, NULL);
            object_obex_store ((void *)x, gensym ("dumpout"), (t_object *)x->rightOutlet);
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

void charlie_free (t_charlie *x)
{ 
    pizKohonenMapFree (x->kohonenMap);
        
    if (x->values) {
        sysmem_freeptr (x->values);
    }
        
    if (x->algorithmMutex) {
        systhread_mutex_free (x->algorithmMutex);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void charlie_assist (t_charlie *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET)  { 
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

t_max_err charlie_setRange (t_charlie *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->range = atom_getlong (argv);
        pizKohonenMapSetRange (x->kohonenMap, x->range);
    }

    return MAX_ERR_NONE;
}

t_max_err charlie_setTraining (t_charlie *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->training = atom_getlong (argv);
        pizKohonenMapSetTraining (x->kohonenMap, x->training);
    }

    return MAX_ERR_NONE;
}

t_max_err charlie_setStep (t_charlie *x, t_object *attr, long argc, t_atom *argv)
{
    if (argc && argv) {
        x->step = atom_getfloat (argv);
        pizKohonenMapSetStep (x->kohonenMap, x->step);
    }

    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void charlie_learn (t_charlie *x, t_symbol *s, long argc, t_atom *argv)
{   
    LOCK
    
    atom_getlong_array (argc, argv, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    pizKohonenMapAdd (x->kohonenMap, MIN (MAXIMUM_LIST_SIZE, argc), x->values);
    
    UNLOCK
}

void charlie_int (t_charlie *x, long n)
{
    char     alloc;
    t_atom   *argv = NULL;
    long     argc = 0;
    PIZError err = PIZ_ERROR;
    
    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_LIST_SIZE), &argc, &argv, &alloc) == MAX_ERR_NONE)) {

        LOCK

        if (pizKohonenMapCount (x->kohonenMap)) {
            if (!(err = pizKohonenMapProceed (x->kohonenMap, argc, x->values))) {
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
    
void charlie_clear (t_charlie *x)
{
    LOCK
    
    pizKohonenMapClear (x->kohonenMap);
    
    UNLOCK
}

void charlie_dump (t_charlie *x, long n)
{
    char     alloc;
    long     size, argc = 0;
    PIZError err = PIZ_GOOD;
    t_atom   *argv = NULL;
    PIZArray *values = pizArrayNew (4);
    
    LOCK
    
    err = pizKohonenMapEncodeToArray (x->kohonenMap, n, values);
    
    UNLOCK
    
    if (!err) {
        size = pizArrayCount (values);

        if (atom_alloc_array (size, &argc, &argv, &alloc) == MAX_ERR_NONE) {
            long *ptr = pizArrayPtr (values);

            atom_setlong_array (argc, argv, argc, ptr);
            outlet_list (x->rightOutlet, NULL, argc, argv);
                    
            sysmem_freeptr (argv);
        }
    }
    
    pizArrayFree (values);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE void pizKohonenMapSetRange (PIZKohonenMap *x, long n)
{
    x->range = MAX (n, 1);
}

PIZ_INLINE void pizKohonenMapSetTraining (PIZKohonenMap *x, long n)
{
    x->training = MAX (n, 1);
}

PIZ_INLINE void pizKohonenMapSetStep (PIZKohonenMap *x, double f)
{
    if (f > 0.) {
        x->step = f;
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE PIZError pizKohonenMapEncodeToArray (const PIZKohonenMap *x, long n, PIZArray *a)
{
    PIZError err = PIZ_ERROR;
    
    if ((n >= 0) && (n < x->mapSize) && a) {
        long i;
        
        err = PIZ_GOOD;
        
        for (i = 0; i < x->vectorSize; i++) {
            err |= pizArrayAppend (a, (long)(((*(x->map + (n * x->vectorSize) + i)) + 0.5)));
        }
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
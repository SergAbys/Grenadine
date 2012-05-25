/*
 *  romeo.c
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

#include "pizGaloisLattice.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define MAXIMUM_SIZE_LIST               256

#define PIZ_GALOIS_LATTICE_CONCEPTS     0
#define PIZ_GALOIS_LATTICE_DATA         1

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define LOCK    systhread_mutex_lock (&x->algorithmMutex);
#define UNLOCK  systhread_mutex_unlock (&x->algorithmMutex);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _romeo {
    t_object            ob;
    long                *values;
    PIZGaloisLattice    *galoisLattice;
    t_systhread_mutex   algorithmMutex;
    void                *leftOutlet;
    void                *rightOutlet;
    } t_romeo;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *romeo_new     (t_symbol *s, long argc, t_atom *argv);
void romeo_free     (t_romeo *x);
void romeo_assist   (t_romeo *x, void *b, long m, long a, char *s);

void romeo_learn    (t_romeo *x, t_symbol *s, long argc, t_atom *argv);
void romeo_int      (t_romeo *x, long n);
void romeo_dump     (t_romeo *x, long n);
void romeo_clear    (t_romeo *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE PIZError pizGaloisLatticeEncodeToArray   (const PIZGaloisLattice *x, long n, PIZArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

static t_class  *romeo_class;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new   ("romeo", (method)romeo_new, (method)romeo_free, (long)sizeof(t_romeo), 0L, A_GIMME, 0);

    class_addmethod (c, (method)romeo_assist,           "assist",   A_CANT, 0); 
    class_addmethod (c, (method)romeo_learn,            "learn",    A_GIMME, 0);
    class_addmethod (c, (method)romeo_int,              "int",      A_LONG, 0);
    class_addmethod (c, (method)romeo_dump,             "dump",     A_LONG, 0);
    class_addmethod (c, (method)romeo_clear,            "clear",    0);
    class_addmethod (c, (method)object_obex_dumpout,    "dumpout",  A_CANT, 0); 

    class_register  (CLASS_BOX, c); 
    
    romeo_class = c;
    
    return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

void *romeo_new (t_symbol *s, long argc, t_atom *argv)
{
    t_romeo *x = NULL;
    
    if (x = (t_romeo *)object_alloc (romeo_class)) {
        long k = 0;
        
        if (argc && atom_gettype (argv) == A_LONG) {
            k = atom_getlong (argv);
        }
        
        x->values = (long *)sysmem_newptr (sizeof(long) * MAXIMUM_SIZE_LIST);
        x->galoisLattice = pizGaloisLatticeNew (1, &k);
                                
        if (x->values && x->galoisLattice) {
            x->rightOutlet  = outlet_new (x, NULL);
            object_obex_store ((void *)x, gensym ("dumpout"), (t_object *)x->rightOutlet);
            x->leftOutlet = listout ((t_object *)x);
                    
            systhread_mutex_new (&x->algorithmMutex, SYSTHREAD_MUTEX_NORMAL);
            
        } else {
            object_free (x);
            x = NULL;
        }
    }
            
    return x;
}

void romeo_free (t_romeo *x)
{ 
    pizGaloisLatticeFree (x->galoisLattice);
        
    if (x->values) {
        sysmem_freeptr (x->values);
    }
        
    if (x->algorithmMutex) {
        systhread_mutex_free (x->algorithmMutex);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
    
void romeo_assist (t_romeo *x, void *b, long m, long a, char *s)
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

void romeo_learn (t_romeo *x, t_symbol *s, long argc, t_atom *argv)
{   
    LOCK
    
    atom_getlong_array (argc, argv, MIN (MAXIMUM_SIZE_LIST, argc), x->values);
    pizGaloisLatticeAdd (x->galoisLattice, MIN (MAXIMUM_SIZE_LIST, argc), x->values);
    
    UNLOCK
}

void romeo_int (t_romeo *x, long n)
{   
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if ((n > 0) && (atom_alloc_array (MIN (n, MAXIMUM_SIZE_LIST), &argc, &argv, &alloc) == MAX_ERR_NONE)) {
        PIZError err = PIZ_ERROR;
            
        LOCK

        if (pizGaloisLatticeCount (x->galoisLattice)) {
            if (!(err = pizGaloisLatticeProceed (x->galoisLattice, argc, x->values))) {
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
    
void romeo_clear (t_romeo *x)
{
    LOCK
    
    pizGaloisLatticeClear (x->galoisLattice);
    
    UNLOCK
}

void romeo_dump (t_romeo *x, long n)
{
    char    alloc;
    t_atom  *argv = NULL;
    long    argc = 0;

    if (atom_alloc_array (n, &argc, &argv, &alloc) == MAX_ERR_NONE) {
        PIZError err = PIZ_GOOD;
        PIZArray *values = pizArrayNew (4);

        LOCK
        
        err = pizGaloisLatticeEncodeToArray (x->galoisLattice, n, values);
        
        UNLOCK

        if (!err) {
            long i, count = pizArrayAtIndex (values, PIZ_GALOIS_LATTICE_CONCEPTS);
            long *ptr  = pizArrayPtr (values);
                                
            for (i = 0; i < count; i++) {
                atom_setlong_array (argc, argv, argc, ptr + PIZ_GALOIS_LATTICE_DATA + (n * i));
                outlet_list (x->rightOutlet, NULL, argc, argv);
            }
        }
            
        sysmem_freeptr (argv);
        pizArrayFree (values);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE PIZError pizGaloisLatticeEncodeToArray (const PIZGaloisLattice *x, long n, PIZArray *a)
{
    PIZError err = PIZ_ERROR;
    
    if ((n > 0) && (n < PIZ_ITEMSET_SIZE) && a) {
        long i, count = pizArrayCount (x->map[n]);
        
        err = PIZ_GOOD;
        
        err |= pizArrayAppend (a, count);
        
        for (i = 0; i < count; i++) {
            long j, p = pizArrayAtIndex (x->map[n], i);
            
            for (j = 0; j < PIZ_ITEMSET_SIZE; j++) {
                if (pizItemsetIsSetAtIndex (&(x->stock[p].itemset), j)) {
                    err |= pizArrayAppend (a, j);
                }
            }
        }
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
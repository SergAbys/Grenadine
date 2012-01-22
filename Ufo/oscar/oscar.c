/*
 *  oscar.c
 *
 *  Created : 12/03/11.
 *
 *  nicolas.danet@free.fr
 *
 */
 
/*
 *  Last modified : 09/09/11.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define DEFAULT_INPUT_LAYER_SIZE 4
#define MAXIMUM_INPUT_LAYER_SIZE 128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _oscar {
    t_object    ob;
    t_object    *papa;
    long        layerSize;
    void        *outlet[MAXIMUM_INPUT_LAYER_SIZE];
    } t_oscar;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------


void *oscar_new             (t_symbol *s, long argc, t_atom *argv);
void oscar_free             (t_oscar *x);
void oscar_assist           (t_oscar *x, void *b, long m, long a, char *s);
void oscar_setPapa          (t_oscar *x, t_object *ptr);

void oscar_list             (t_oscar *x, t_symbol *s, long argc, t_atom *argv);
void oscar_learn            (t_oscar *x, t_symbol *s, long argc, t_atom *argv);
void oscar_target           (t_oscar *x, t_symbol *s, long argc, t_atom *argv);
void oscar_int              (t_oscar *x, long n);
void oscar_float            (t_oscar *x, double f);
void oscar_dump             (t_oscar *x);
void oscar_clear            (t_oscar *x);
void oscar_bias             (t_oscar *x, double f);
void oscar_threshold        (t_oscar *x, double f);
void oscar_learning         (t_oscar *x, double f);
void oscar_papaTransfer     (t_oscar *x, long n);
void oscar_oscarTransfer    (t_oscar *x, long n);
void oscar_embed            (t_oscar *x, long n);
void oscar_error            (t_oscar *x, long n);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class *oscar_class;

int main (void)
{   
    t_class *c = NULL;
    
    c = class_new ("oscar", (method)oscar_new, (method)oscar_free, (long)sizeof(t_oscar), 0L, A_GIMME, 0);
    
    class_addmethod (c, (method)oscar_assist,           "assist", A_CANT, 0);
    class_addmethod (c, (method)oscar_int,              "int", A_LONG, 0);
    class_addmethod (c, (method)oscar_float,            "float", A_FLOAT, 0);
    class_addmethod (c, (method)oscar_list,             "list", A_GIMME, 0);
    class_addmethod (c, (method)oscar_learn,            "learn", A_GIMME, 0);
    class_addmethod (c, (method)oscar_target,           "target", A_GIMME, 0);
    class_addmethod (c, (method)oscar_dump,             "dump", 0);
    class_addmethod (c, (method)oscar_clear,            "clear", 0);
    class_addmethod (c, (method)oscar_setPapa,          "setpapa", A_CANT, 0);
    class_addmethod (c, (method)oscar_bias,             "bias", A_FLOAT, 0);
    class_addmethod (c, (method)oscar_threshold,        "threshold", A_FLOAT, 0);
    class_addmethod (c, (method)oscar_learning,         "learning", A_FLOAT, 0);
    class_addmethod (c, (method)oscar_papaTransfer,     "papafunction", A_LONG, 0);
    class_addmethod (c, (method)oscar_oscarTransfer,    "oscarfunction", A_LONG, 0);
    class_addmethod (c, (method)oscar_embed,            "embed", A_LONG, 0);
    class_addmethod (c, (method)oscar_error,            "error", A_LONG, 0);
    
    class_register (CLASS_BOX, c); 
    
    oscar_class = c;
    
    return 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *oscar_new (t_symbol *s, long argc, t_atom *argv)
{
    t_oscar *x = NULL;
    
    if (x = (t_oscar *)object_alloc (oscar_class)) 
        {   
            long i;
             
            x->papa         = NULL;
            x->layerSize    = DEFAULT_INPUT_LAYER_SIZE;
            
            if (argc)
                {
                    if (atom_gettype (argv) == A_LONG)
                        {
                            long k = atom_getlong (argv);
                            
                            if (k > 0 && k <= MAXIMUM_INPUT_LAYER_SIZE)
                                {
                                    x->layerSize = k;
                                }
                        }
                }
                                            
            attr_args_process (x, argc, argv);
    
            for (i = 0; i < x->layerSize; i++)
                {
                    x->outlet[i] = outlet_new ((t_object *)x, NULL);
                }
            
            for (i = 0; i < x->layerSize; i++)
                {
                    inlet_new (x, NULL); 
                }
        }
            
    return x;
}

void oscar_free (t_oscar *x)
{ 
    ;
}

void oscar_assist (t_oscar *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) 
        { 
            if (!a)
                {
                    sprintf (s, "(list) clear dump learn target");
                }
            else
                {
                    sprintf (s, "(nothing)");
                }
        } 
    else
        { 
            sprintf (s, "(nothing)");
        }
}

void oscar_setPapa (t_oscar *x, t_object *ptr)
{
    x->papa = ptr;
}
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void oscar_list (t_oscar *x, t_symbol *s, long argc, t_atom *argv)
{   
    if (x->papa) {
            object_method_typed (x->papa, gensym ("list"), argc, argv, NULL);
        }
}

void oscar_learn (t_oscar *x, t_symbol *s, long argc, t_atom *argv)
{   
    if (x->papa) {
            object_method_typed (x->papa, gensym ("learn"), argc, argv, NULL);
        }
}

void oscar_target (t_oscar *x, t_symbol *s, long argc, t_atom *argv)
{   
    if (x->papa) {
            object_method_typed (x->papa, gensym ("target"), argc, argv, NULL);
        }
}

void oscar_int (t_oscar *x, long n)
{   
    if (x->papa) {
            object_method_long (x->papa, gensym ("int"), n, NULL);
        }
}

void oscar_float (t_oscar *x, double f)
{   
    if (x->papa) {
            object_method_float (x->papa, gensym ("float"), f, NULL);
        }
}
    
void oscar_dump (t_oscar *x)
{   
    if (x->papa) {
            object_method (x->papa, gensym ("dump"));
        }
}

void oscar_clear (t_oscar *x)
{   
    if (x->papa) {
            object_method (x->papa, gensym ("clear"));
        }
}

void oscar_bias (t_oscar *x, double f)
{
    if (x->papa) {
            object_attr_setfloat (x->papa, gensym ("bias"), f);
        }
}

void oscar_threshold (t_oscar *x, double f)
{
    if (x->papa) {
            object_attr_setfloat (x->papa, gensym ("threshold"), f);
        }
}

void oscar_learning (t_oscar *x, double f)
{
    if (x->papa) {
            object_attr_setfloat (x->papa, gensym ("learning"), f);
        }
}

void oscar_papaTransfer (t_oscar *x, long n)
{
    if (x->papa) {
            object_attr_setlong (x->papa, gensym ("papafunction"), n);
        }
}

void oscar_oscarTransfer (t_oscar *x, long n)
{
    if (x->papa) {
            object_attr_setlong (x->papa, gensym ("oscarfunction"), n);
        }
}

void oscar_embed (t_oscar *x, long n)
{
    if (x->papa) {
            object_attr_setlong (x->papa, gensym ("embed"), n);
        }
}

void oscar_error (t_oscar *x, long n)
{
    if (x->papa) {
            object_attr_setlong (x->papa, gensym ("error"), n);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
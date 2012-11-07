/*
 *  papa.c
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "papa.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

static t_class	*papa_class;

static t_symbol	*papa_sym_error = NULL;

int main (void)
{	
	t_class *c = NULL;
	
	common_symbols_init ();
	
	c = class_new ("papa", (method)papa_new, (method)papa_free, (long)sizeof(t_papa), 0L, A_GIMME, 0);
	
	class_addmethod	(c, (method)papa_appendToDictionary,	"appendtodictionary",	A_CANT, 0);
	class_addmethod (c, (method)papa_assist,				"assist",				A_CANT, 0);
	class_addmethod (c, (method)papa_dblclick,				"dblclick",				A_CANT, 0);
	class_addmethod (c, (method)papa_clear,					"clear",				0);
	class_addmethod (c, (method)papa_anything,				"anything",				A_GIMME, 0);
	class_addmethod (c, (method)papa_dump,					"dump",					0);
	class_addmethod (c, (method)papa_int,					"int",					A_LONG, 0);
	class_addmethod (c, (method)papa_float,					"float",				A_FLOAT, 0);
	class_addmethod (c, (method)papa_list,					"list",					A_GIMME, 0);
	class_addmethod (c, (method)papa_learn,					"learn",				A_GIMME, 0);
	class_addmethod (c, (method)papa_target,				"target",				A_GIMME, 0);
	
	CLASS_ATTR_LONG                 (c, "embed",			ATTR_FLAGS_NONE, t_papa, saveDataWithPatcher);
	CLASS_ATTR_STYLE_LABEL			(c, "embed",			ATTR_FLAGS_NONE, "onoff", "Save Data With Patcher");
	CLASS_ATTR_DEFAULT_SAVE         (c, "embed",			ATTR_FLAGS_NONE, "0");
	CLASS_ATTR_FILTER_CLIP			(c, "embed",			0, 1);
	
	CLASS_ATTR_RGBA					(c, "synapse",			ATTR_FLAGS_NONE, t_papa, lineSynapse);
    CLASS_ATTR_DEFAULT_SAVE         (c, "synapse",          ATTR_FLAGS_NONE, "0.62 0.55 0.83 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "synapse",			ATTR_FLAGS_NONE, "rgba", "Line Synapse");
	CLASS_ATTR_RGBA					(c, "feedback",			ATTR_FLAGS_NONE, t_papa, lineFeedback);
    CLASS_ATTR_DEFAULT_SAVE         (c, "feedback",         ATTR_FLAGS_NONE, "1.00 0.60 0.60 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "feedback",			ATTR_FLAGS_NONE, "rgba", "Line Feedback");
	CLASS_ATTR_RGBA					(c, "wrong",			ATTR_FLAGS_NONE, t_papa, lineWrong);
    CLASS_ATTR_DEFAULT_SAVE         (c, "wrong",            ATTR_FLAGS_NONE, "0.85 0.85 0.85 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "wrong",			ATTR_FLAGS_NONE, "rgba", "Line Wrong");
	CLASS_ATTR_RGBA					(c, "linear",			ATTR_FLAGS_NONE, t_papa, boxLinear);
    CLASS_ATTR_DEFAULT_SAVE         (c, "linear",           ATTR_FLAGS_NONE, "0.92 0.94 0.67 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "linear",			ATTR_FLAGS_NONE, "rgba", "Box Linear");
	CLASS_ATTR_RGBA					(c, "sigmoid",			ATTR_FLAGS_NONE, t_papa, boxSigmoid);
    CLASS_ATTR_DEFAULT_SAVE         (c, "sigmoid",          ATTR_FLAGS_NONE, "0.68 0.82 0.82 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "sigmoid",			ATTR_FLAGS_NONE, "rgba", "Box Sigmoid");
	CLASS_ATTR_RGBA					(c, "tanh",				ATTR_FLAGS_NONE, t_papa, boxTanh);
    CLASS_ATTR_DEFAULT_SAVE         (c, "tanh",             ATTR_FLAGS_NONE, "0.83 0.74 0.84 1.0");
	CLASS_ATTR_STYLE_LABEL			(c, "tanh",				ATTR_FLAGS_NONE, "rgba", "Box Tanh");
    
	CLASS_ATTR_DOUBLE				(c, "bias",				ATTR_FLAGS_NONE, t_papa, bias);
	CLASS_ATTR_DEFAULT_SAVE         (c, "bias",				ATTR_FLAGS_NONE, "0.");
	CLASS_ATTR_LABEL				(c, "bias",				ATTR_FLAGS_NONE, "Bias");
	CLASS_ATTR_FILTER_CLIP			(c, "bias",				-5., 5.);
	CLASS_ATTR_DOUBLE				(c, "threshold",		ATTR_FLAGS_NONE, t_papa, threshold);
	CLASS_ATTR_DEFAULT_SAVE         (c, "threshold",		ATTR_FLAGS_NONE, "0.");
	CLASS_ATTR_LABEL				(c, "threshold",		ATTR_FLAGS_NONE, "Learning Threshold");
	CLASS_ATTR_FILTER_MIN			(c, "threshold",		0.);
	CLASS_ATTR_DOUBLE				(c, "learning",			ATTR_FLAGS_NONE, t_papa, learning);
	CLASS_ATTR_DEFAULT_SAVE         (c, "learning",			ATTR_FLAGS_NONE, "1.");
	CLASS_ATTR_LABEL				(c, "learning",			ATTR_FLAGS_NONE, "Learning Rate");
	CLASS_ATTR_FILTER_MIN			(c, "learning",			0.);
	
	CLASS_ATTR_LONG					(c, "papafunction",		ATTR_FLAGS_NONE, t_papa, papaTransfer);
	CLASS_ATTR_ENUMINDEX			(c, "papafunction",		ATTR_FLAGS_NONE, "Linear Sigmoid Tanh");
	CLASS_ATTR_LABEL				(c, "papafunction",		ATTR_FLAGS_NONE, "Function Papa");
	CLASS_ATTR_DEFAULT_SAVE         (c, "papafunction",		ATTR_FLAGS_NONE, "2");
	CLASS_ATTR_FILTER_CLIP			(c, "papafunction",		0, 2);
	CLASS_ATTR_LONG					(c, "oscarfunction",	ATTR_FLAGS_NONE, t_papa, oscarTransfer);
	CLASS_ATTR_ENUMINDEX			(c, "oscarfunction",	ATTR_FLAGS_NONE, "Linear Sigmoid Tanh");
	CLASS_ATTR_LABEL				(c, "oscarfunction",	ATTR_FLAGS_NONE, "Function Oscar");
	CLASS_ATTR_DEFAULT_SAVE         (c, "oscarfunction",	ATTR_FLAGS_NONE, "0");
	CLASS_ATTR_FILTER_CLIP			(c, "oscarfunction",	0, 2);
	
	CLASS_ATTR_LONG					(c, "error",			ATTR_FLAGS_NONE, t_papa, error);
	CLASS_ATTR_STYLE_LABEL			(c, "error",			ATTR_FLAGS_NONE, "onoff", "Error");
	CLASS_ATTR_DEFAULT_SAVE         (c, "error",			ATTR_FLAGS_NONE, "0");
	CLASS_ATTR_FILTER_CLIP			(c, "error",			0, 1);

    CLASS_ATTR_ORDER				(c, "bias",				ATTR_FLAGS_NONE, "1");
	CLASS_ATTR_ORDER				(c, "learning",			ATTR_FLAGS_NONE, "2");
	CLASS_ATTR_ORDER				(c, "threshold",		ATTR_FLAGS_NONE, "3");
	CLASS_ATTR_ORDER				(c, "oscarfunction",	ATTR_FLAGS_NONE, "4");
	CLASS_ATTR_ORDER				(c, "papafunction",		ATTR_FLAGS_NONE, "5");
    CLASS_ATTR_ORDER				(c, "synapse",          ATTR_FLAGS_NONE, "6");
    CLASS_ATTR_ORDER				(c, "feedback",         ATTR_FLAGS_NONE, "7");
    CLASS_ATTR_ORDER				(c, "wrong",            ATTR_FLAGS_NONE, "8");
    CLASS_ATTR_ORDER				(c, "linear",           ATTR_FLAGS_NONE, "9");
    CLASS_ATTR_ORDER				(c, "sigmoid",          ATTR_FLAGS_NONE, "10");
    CLASS_ATTR_ORDER				(c, "tanh",             ATTR_FLAGS_NONE, "11");
    CLASS_ATTR_ORDER				(c, "error",			ATTR_FLAGS_NONE, "12");
    CLASS_ATTR_ORDER				(c, "embed",            ATTR_FLAGS_NONE, "13");
    
	papa_sym_error = gensym ("error");
		
	class_register (CLASS_BOX, c); 
		
	papa_class = c;
	
	return MAX_ERR_NONE;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void *papa_new (t_symbol *s, long argc, t_atom *argv)
{
	t_papa *x = NULL;
    
	if (x = (t_papa *)object_alloc (papa_class)) 
		{
			long			i;
			t_dictionary	*d = NULL; 
			
			srand (time(NULL));
			
            x->learning					= 1.;
			x->sizeOutputLayer			= 4;
			x->papaTransfer				= TRANSFER_TANH;
			x->oscarTransfer			= TRANSFER_LINEAR;

			jrgba_set (&x->lineSynapse,     0.62, 0.55, 0.83, 1.0);
			jrgba_set (&x->lineFeedback,    1.00, 0.60, 0.60, 1.0);
			jrgba_set (&x->lineWrong,       0.85, 0.85, 0.85, 1.0);
			jrgba_set (&x->boxLinear,       0.92, 0.94, 0.67, 1.0);
			jrgba_set (&x->boxSigmoid,      0.68, 0.82, 0.82, 1.0);
			jrgba_set (&x->boxTanh,         0.83, 0.74, 0.84, 1.0);
            jrgba_set (&x->boxError,        0.95, 0.95, 0.95, 1.0);
            
			x->queue = (long *)sysmem_newptrclear (sizeof(long) * (QUEUE_SIZE + 1));
			
			if (argc)
				{
					if (atom_gettype (argv) == A_LONG)
						{
							long k = atom_getlong (argv);
							
							if (k > 0 && k <= MAXIMUM_OUTPUT_LAYER_SIZE)
								{
									x->sizeOutputLayer = k;
								}
						}
				}
											
			attr_args_process (x, argc, argv);
	
			for (i = 0; i < (x->sizeOutputLayer - 1); i++)
				{
					inlet_new (x, NULL); 
				}
			
			for (i = 0; i < x->sizeOutputLayer + 1; i++)
				{
					x->outlet[i] = outlet_new ((t_object *)x, NULL);
				}
			
			systhread_mutex_new (&x->mutex, SYSTHREAD_MUTEX_NORMAL);
			
			x->clock = clock_new ((t_object *)x, (method)papa_task);
			
			if (d = (t_dictionary *)gensym ("#D")->s_thing)
				{
					long	argc1, argc2, argc3;
					t_atom	*ptr1 = NULL;
					t_atom	*ptr2 = NULL;
					t_atom	*ptr3 = NULL;
					
					dictionary_getatoms (d, gensym ("head_papa_data"),		&argc1, &ptr1);
					dictionary_getatoms (d, gensym ("graf_papa_data"),		&argc2, &ptr2);
					dictionary_getatoms (d, gensym ("synapses_papa_data"),	&argc3, &ptr3);
					
					if (ptr1 && argc1)
						{
							if (x->headData = atomarray_new (0, NULL))
								{
									atomarray_setatoms (x->headData, argc1, ptr1);
								}
						}
					
					
					if (ptr2 && argc2)
						{
							if (x->graphData = atomarray_new (0, NULL))
								{
									atomarray_setatoms (x->graphData, argc2, ptr2);
								}
						}
					
					if (ptr3 && argc3)
						{
							if (x->synapsesData = atomarray_new (0, NULL))
								{
									atomarray_setatoms (x->synapsesData, argc3, ptr3);
								}
						}
				}
			
			defer_low (x, (method)papa_rebuildFromData, NULL, 0, NULL);			
		}
			
	return x;
}

void papa_free (t_papa *x)
{ 
	if (x->queue)
		{
			sysmem_freeptr (x->queue);
		}
	
	papa_freeGraph (x);
	
	object_free (x->headData);
	object_free (x->graphData);
	object_free (x->synapsesData);
		
	if (x->clock)
		{
			clock_unset (x->clock);
			object_free (x->clock);
		}
	
	if (x->mutex)
		{
			systhread_mutex_free (x->mutex);
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_assist (t_papa *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) 
		{ 
			sprintf (s, "(nothing)");
		} 
	else 
		{	
			if (!a)
				{
					sprintf (s, "(anything)");
				}
			else
				{
					sprintf (s, "(nothing)");
				}
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_dblclick (t_papa *x)
{
	if (!systhread_mutex_trylock (x->mutex))
		{
			papa_buildGraph (x);
			
			systhread_mutex_unlock (x->mutex);
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_clear (t_papa *x) 
{
	systhread_mutex_lock (x->mutex);
	
	if (x->graph && x->graphMap && x->stock)
		{   
			long i;
			
			for (i = 0; i < (x->maximumDistance + 2); i++)
				{
					long j;
											
					for (j = 0; j < x->graphMap[i]; j ++)
						{
							long p;
							
							p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];
							
							x->stock[p].potential		= 0.;
							x->stock[p].signal			= 0.;
							x->stock[p].delayA			= 0.;
							x->stock[p].delayB			= 0.;
							x->stock[p].tempPotential	= 0.;
							x->stock[p].tempSignal		= 0.;
							x->stock[p].error			= 0.;
							
							if (x->stock[p].index)
								{
									long t;
									
									for (t = 0; t < x->stock[p].index; t++) 
										{
											switch (x->stock[p].type[t]) {
												case SYNAPSE			:	x->stock[p].weight[t] 
																			= papa_randomGaussian ();	break;
												case SYNAPSE_FEEDBACK	:	x->stock[p].weight[t] 
																			= WEIGHT_FEEDBACK;			break;
												case SYNAPSE_WRONG		:	x->stock[p].weight[t] 
																			= WEIGHT_WRONG;				break;
												}
										}
								}
						}
				}
		}
	
	systhread_mutex_unlock (x->mutex);
}
	
void papa_anything (t_papa *x, t_symbol *s, long argc, t_atom *argv) 
{
	;
}

void papa_int (t_papa *x, long n)
{
	t_atom a;
	atom_setlong (&a, n);
	
	papa_list (x, _sym_list, 1, &a);
}

void papa_float	(t_papa *x, double f)
{
	t_atom a;
	atom_setfloat (&a, f);
	
	papa_list (x, _sym_list, 1, &a);
}

void papa_list (t_papa *x, t_symbol *s, long argc, t_atom *argv) 
{	
	systhread_mutex_lock (x->mutex);
	
	if (argc && argv && x->vectorIn && x->vectorOut && x->stock && x->graph && x->graphMap)
		{
			long	i;
			long	k = 0;
			t_atom	result[MAXIMUM_OUTPUT_LAYER_SIZE];

			for (i = 0; i < x->sizeInputLayer; i++)
				{
					if (i < argc && atom_gettype (argv + i) == A_FLOAT)
						{	
							x->vectorIn[i] = atom_getfloat (argv + i);
						}
					else if (i < argc && atom_gettype (argv + i) == A_LONG)
						{	
							if (x->oscarTransfer == TRANSFER_LINEAR || x->oscarTransfer == TRANSFER_TANH)
								{
									x->vectorIn[i] = 
										(double)(((CLIP (atom_getlong (argv + i), 0, 127) * 2.) / 127.) - 1.);
								}
							else
								{
									x->vectorIn[i] = 
										(double)(CLIP (atom_getlong (argv + i), 0, 127) / 127.);
								}
							
							k = 1;
						}
					else
						{
							x->vectorIn[i] = 0.;
						}
				}
			
			papa_propagation (x);
			papa_initialisation (x);
			
			if (k)
				{
					long j;
					
					for (j = 0; j < x->sizeOutputLayer; j++)
						{
							long n;
							
							if (x->papaTransfer == TRANSFER_LINEAR || x->papaTransfer == TRANSFER_TANH)
								{
									n = floor ((((x->vectorOut[j] + 1.) / 2.) * 127.) + 0.5);
									atom_setlong (result + j, CLIP (n, 0, 127));
								}
							else
								{
									n = floor ((x->vectorOut[j] * 127.) + 0.5);
									atom_setlong (result + j, CLIP (n, 0, 127));
								}
						}
				}
			else
				{
					long j;
					
					for (j = 0; j < x->sizeOutputLayer; j++)
						{
							atom_setfloat (result + j, x->vectorOut[j]);
						}
				}
			
			systhread_mutex_unlock (x->mutex);
			
			outlet_anything ((void *)x->outlet[x->sizeOutputLayer], _sym_list, x->sizeOutputLayer, result);
		}
	else
		{
			systhread_mutex_unlock (x->mutex);
		}
}
	
void papa_learn	(t_papa *x, t_symbol *s, long argc, t_atom *argv) 
{
	long	t = 0;
	double	k = -1.;
	
	systhread_mutex_lock (x->mutex);
	
	if (argc && argv && x->vectorIn && x->vectorOut && x->stock && x->graph && x->graphMap)
		{
			long i;
			
			for (i = 0; i < x->sizeInputLayer; i++)
				{
					if (i < argc && atom_gettype (argv + i) == A_FLOAT)
						{	
							x->vectorIn[i] = atom_getfloat (argv + i);
						}
					else if (i < argc && atom_gettype (argv + i) == A_LONG)
						{	
							if (x->oscarTransfer == TRANSFER_LINEAR || x->oscarTransfer == TRANSFER_TANH)
								{
									x->vectorIn[i] = 
										(double)(((CLIP (atom_getlong (argv + i), 0, 127) * 2.) / 127.) - 1.);
								}
							else
								{
									x->vectorIn[i] = (double)(CLIP (atom_getlong (argv + i), 0, 127) / 127.);
								}
						}
					else
						{
							x->vectorIn[i] = 0.;
						}
				}
			
			papa_propagation (x);
			
			k = papa_computeErrors (x);
			
			if (k > x->threshold) 
				{
					papa_backpropagation (x);
				}
			else
				{
					t = 1;
				}
			
			papa_initialisation (x);
		}
				
	systhread_mutex_unlock (x->mutex);	
	
	if (k > 0. && x->error && !t)
		{
			t_atom a;
			
			atom_setfloat (&a, k);
			
			outlet_anything ((void *)x->outlet[x->sizeOutputLayer], papa_sym_error, 1, &a);
		}
		
	if (x->error && t)
		{
			outlet_anything ((void *)x->outlet[x->sizeOutputLayer], _sym_bang, 0, NULL);
		}
}

void papa_target (t_papa *x, t_symbol *s, long argc, t_atom *argv) 
{
	systhread_mutex_lock (x->mutex);
				
	if (argc && argv && x->vectorTarget)
		{
			long i;
			
			for (i = 0; i < x->sizeOutputLayer; i++)
				{
					if (i < argc && atom_gettype (argv + i) == A_FLOAT)
						{	
							x->vectorTarget[i] = atom_getfloat (argv + i);
						}
					else if (i < argc && atom_gettype (argv + i) == A_LONG)
						{	
							if (x->papaTransfer == TRANSFER_LINEAR || x->papaTransfer == TRANSFER_TANH)
								{
									x->vectorTarget[i] = 
										(double)(((CLIP (atom_getlong (argv + i), 0, 127) * 2.) / 127.) - 1.);
								}
							else
								{
									x->vectorTarget[i] = 
										(double)(CLIP (atom_getlong (argv + i), 0, 127) / 127.);
								}
						}
					else
						{
							x->vectorTarget[i] = 0.;
						}
				}
		}
	
	systhread_mutex_unlock (x->mutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_dump (t_papa *x)
{  
	if (x->graph && x->graphMap && x->stock && !x->lockDump)
		{
			long i;
			
			x->lockDump = 1;
			
			for (i = 0; i < (x->maximumDistance + 2); i++)
				{
					long j;
					
					post ("// %ld", i);
											
					for (j = 0; j < x->graphMap[i]; j ++)
						{
							long p;
							
							p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];
							
							switch (x->stock[p].transfer) 
								{
									case TRANSFER_LINEAR	:	post ("Noeud %ld : Linear", p);
																break;
									case TRANSFER_SIGMOID	:	post ("Noeud %ld : Sigmoid", p);
																break;
									case TRANSFER_TANH		:	post ("Noeud %ld : Tanh", p);
																break;
								}		
							
							post ("    potential : %.2lf", x->stock[p].tempPotential);
							post ("    signal : %.2lf", x->stock[p].tempSignal);
							post ("    delay : %.2lf", x->stock[p].delayA);
							post ("    error : %f", x->stock[p].error);

							if (x->stock[p].index)
								{
									long t;
									
									for (t = 0; t < x->stock[p].index; t++)
										{
											post ("    -- destination : %ld / val : %.2lf", 
												x->stock[p].destination[t], x->stock[p].weight[t]);
										}
								}
						}
				}
			
			clock_fdelay (x->clock, 1000.);
		}
}

void papa_task (t_papa *x)
{
	  x->lockDump = 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_findOscar (t_papa *x)
{
	t_object *ptr_p = NULL;
			
	if (!(object_obex_lookup (x, gensym ("#P"), &ptr_p)))
		{
			long		result = 0;
			t_object	*mybox = NULL;
			
			if (!(object_obex_lookup (x, gensym ("#B"), &mybox))) 
				{
					x->me		= jbox_get_object (mybox);
					x->meBox	= mybox;
		
					object_method (ptr_p, gensym ("iterate"), 
						(method)papa_iterateToFindOscar, x, PI_WANTBOX, &result);
				}
		}
	
	if (x->oscar)
		{
			object_method (x->oscar, gensym ("setpapa"), x->me);
		}
}

long papa_iterateToFindOscar (t_papa *x, t_object *box)
{
	t_symbol *sym_b = jpatcher_get_maxclass (box); 
	
	if (sym_b == gensym ("oscar"))
		{
			if (!x->oscar)
				{
					x->oscar	= jbox_get_object (box);
					x->oscarBox	= box;
				}
		}
		
	return 0;
}

void papa_rebuildFromData (t_papa *x)
{	
	if (!systhread_mutex_trylock (x->mutex))
		{
			papa_freeGraph (x);
			papa_findOscar (x);
			
			if (x->headData && x->graphData)
				{
					long	argc1, argc2;
					t_atom	*hptr = NULL;
					t_atom	*gptr = NULL;
					
					if (!(atomarray_getatoms (x->headData, &argc1, &hptr)) && 
						!(atomarray_getatoms (x->graphData, &argc2, &gptr)))
						{
							x->stockSize		= atom_getlong (hptr);
							x->maximumDistance	= atom_getlong (hptr + 1) - 2;
							x->sizeInputLayer	= atom_getlong (hptr + 2);
							
							x->graphMap	= (long *)sysmem_newptrclear (sizeof(long) * (x->maximumDistance + 2));
							x->graph	= (long *)sysmem_newptrclear (sizeof(long) * 
												((x->maximumDistance + 2) * MAXIMUM_NODES_PER_LAYER));
							x->stock	= (t_node *)sysmem_newptrclear (sizeof(t_node) * x->stockSize);
							
							x->vectorIn	= (double *)sysmem_newptrclear (sizeof(double) * x->sizeInputLayer);
							x->vectorOut	= (double *)sysmem_newptrclear (sizeof(double) * x->sizeOutputLayer);
							x->vectorTarget	= (double *)sysmem_newptrclear (sizeof(double) * x->sizeOutputLayer);
							
							if (x->graph && x->graphMap && x->stock)
								{
									long i;
									long k = 0;
														
									for (i = 0; i < (x->maximumDistance + 2); i++)
										{
											long j;
											
											x->graphMap[i] = atom_getlong (hptr + 2 + i);
											
											for (j = 0; j < x->graphMap[i]; j++)
												{
													long p;
													
													p = atom_getlong (gptr + k);
													
													x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j] = p;
													
													x->stock[p].mode	 = atom_getlong (gptr + k + 1);
													x->stock[p].transfer = atom_getlong (gptr + k + 2);
													x->stock[p].index	 = 0;
													
													k += 3;
												}
										}
								}
						}	
				}
			
			if (x->synapsesData)
				{
					if (x->graph && x->graphMap && x->stock)
						{
							long	argc;
							t_atom	*ptr = NULL;
					
							if (!(atomarray_getatoms (x->synapsesData, &argc, &ptr)))
								{
									long i;
									long t = argc / 4;
									long k = 0;
									
									for (i = 0; i < t; i++)
										{	
											long p, a;
											
											p = atom_getlong (ptr + k);
											a = x->stock[p].index;
											
											x->stock[p].line[a]	= NULL;
											x->stock[p].type[a]	= atom_getlong	(ptr + k + 2);
											x->stock[p].destination[a]	= atom_getlong	(ptr + k + 1);
											
											if (x->saveDataWithPatcher)
												{
													x->stock[p].weight[a] = atom_getfloat (ptr + k + 3);
												}
											else
												{	
													switch (atom_getlong (ptr + k + 2)) {
														case SYNAPSE_WRONG	  :	x->stock[p].weight[a] 
																				= WEIGHT_WRONG; break;
														case SYNAPSE		  :	x->stock[p].weight[a] 
																				= papa_randomGaussian (); break;
														case SYNAPSE_FEEDBACK :	x->stock[p].weight[a] 
																				= WEIGHT_FEEDBACK; break;
														}
												}
											
											x->stock[p].index ++;
											
											k += 4;
										}
								}
						}
				}
				
			systhread_mutex_unlock (x->mutex);
		}
}

void papa_appendToDictionary (t_papa *x, t_dictionary *bd)
{
	systhread_mutex_lock (x->mutex);
	
	if (x->stock && x->graph && x->graphMap)
		{	
			long	m = 0;
			long	n = 0;
			t_atom	*head = NULL;
			t_atom	*graph = NULL;
			t_atom	*synapses = NULL;
			
			if (head = (t_atom *)sysmem_newptrclear (sizeof(t_atom) * (x->maximumDistance + 2 + 2)))
				{
					long i;
					
					atom_setlong (head, x->stockSize);
					atom_setlong (head + 1, x->maximumDistance + 2);
					
					for (i = 0; i < (x->maximumDistance + 2); i++)
						{
							atom_setlong ((head + i + 2), x->graphMap[i]);
							m += x->graphMap[i];
						}
						
					dictionary_appendatoms (bd, gensym ("head_papa_data"), (x->maximumDistance + 2 + 2), head);
					
					sysmem_freeptr (head);
				}	
			
			if (graph = (t_atom *)sysmem_newptrclear (sizeof(t_atom) * (m * 3)))
				{
					long i;
					long k = 0;
					
					for (i = 0; i < (x->maximumDistance + 2); i++)
						{
							long j;
													
							for (j = 0; j < x->graphMap[i]; j ++)
								{
									long p;
									
									p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];
									
									n += x->stock[p].index;
									
									atom_setlong (graph + k,		p);
									atom_setlong (graph + k + 1, x->stock[p].mode);
									atom_setlong (graph + k + 2, x->stock[p].transfer);
									
									k += 3;
								}
						}
						
					dictionary_appendatoms (bd, gensym ("graf_papa_data"), (m * 3), graph);
					
					sysmem_freeptr (graph);
				}
			
			if (synapses = (t_atom *)sysmem_newptrclear (sizeof(t_atom) * (n * 4)))
				{
					long i;
					long k = 0;
					
					for (i = 0; i < (x->maximumDistance + 2); i++)
						{
							long j;
													
							for (j = 0; j < x->graphMap[i]; j ++)
								{
									long p;
									
									p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];
									
									if (x->stock[p].index)
										{
											long t;
											
											for (t = 0; t < x->stock[p].index; t++) 
												{
													atom_setlong  (synapses + k,	 p);
													atom_setlong  (synapses + k + 1, x->stock[p].destination[t]);
													atom_setlong  (synapses + k + 2, x->stock[p].type[t]);
													atom_setfloat (synapses + k + 3, x->stock[p].weight[t]);
									
													k += 4;
												}
										}
								}
						}
						
					dictionary_appendatoms (bd, gensym ("synapses_papa_data"), (n * 4), synapses);
					
					sysmem_freeptr (synapses);
				}
		}
	
	systhread_mutex_unlock (x->mutex);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_propagation (t_papa *x) 
{  
	long i;
	
	for (i = 0; i < x->sizeInputLayer; i++)
		{
			x->stock[i].potential = x->vectorIn[i];
		}
	
	for (i = 0; i < (x->maximumDistance + 2); i++)
		{
			long j;
									
			for (j = 0; j < x->graphMap[i]; j ++)
				{
					long	p;
					double	f;
					
					p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];
					
					if (i)
						{
							f = x->stock[p].potential + x->stock[p].delayA + x->bias;
						}
					else
						{
							f = x->stock[p].potential + x->stock[p].delayA;
						}
					
					switch (x->stock[p].transfer) 
						{
							case TRANSFER_LINEAR	:	x->stock[p].signal = f; break;
							case TRANSFER_SIGMOID	:	x->stock[p].signal = 1. / (1. + exp (-f)); break;
							case TRANSFER_TANH		:	x->stock[p].signal 
														= (exp (f) - exp (-f)) / (exp (f) + exp (-f)); break;
						}
					
					if (x->stock[p].index)
						{
							long t;
															
							for (t = 0; t < x->stock[p].index; t++)
								{
									if (x->stock[p].type[t] == SYNAPSE) 
										{
											x->stock[x->stock[p].destination[t]].potential 
												+= x->stock[p].signal * x->stock[p].weight[t];
										}
									else if (x->stock[p].type[t] == SYNAPSE_FEEDBACK)
										{
											if (x->stock[x->stock[p].destination[t]].mode 
												== MODE_NORMAL)
												{
													x->stock[x->stock[p].destination[t]].delayB 
														+= x->stock[p].signal * WEIGHT_FEEDBACK;
												}
											else if (x->stock[x->stock[p].destination[t]].mode 
												== MODE_NO_BACKWARD)
												{
													x->stock[x->stock[p].destination[t]].potential 
														+= x->stock[p].signal * WEIGHT_FEEDBACK;
												}
										}	
								}
						}
				}
		}
	
	for (i = 0; i < x->sizeOutputLayer; i++)
		{
			x->vectorOut[i] = x->stock[x->sizeInputLayer + i].signal;
		}
}

double papa_computeErrors (t_papa *x)
{
	long	i;
	double	k = 0.;
	
	for (i = 0; i < x->sizeOutputLayer; i++)
		{
			double f, s;
			double g = 0.;
			
			f = x->vectorTarget[i] - x->vectorOut[i];
			s = x->stock[x->sizeInputLayer + i].potential;
			
			switch (x->stock[x->sizeInputLayer + i].transfer) 
				{
					case TRANSFER_LINEAR  :	g = 1.; break;
					case TRANSFER_SIGMOID :	g = (1. / (1. + exp (-s))) * (1. - (1. / (1. + exp (-s)))); break;
					case TRANSFER_TANH	  :	g = 4. / (pow ((exp (s) + exp (-s)), 2)); break;
				}
								
			x->stock[x->sizeInputLayer + i].error = f * g;
				
			k += pow (f, 2);
		}
	
	x->totalError = k * 0.5;
	
	return x->totalError;
}

void papa_backpropagation (t_papa *x) 
{
	long i;
	
	for (i = x->maximumDistance; i >= 0; i--)
		{
			long j;
									
			for (j = 0; j < x->graphMap[i]; j ++)
				{
					long	p;
					double	s;
					double	g = 0.;
					double	k = 0.;
					
					p = x->graph[(i * MAXIMUM_NODES_PER_LAYER) + j];

					if (x->stock[p].index)
						{
							long t;
							
							for (t = 0; t < x->stock[p].index; t++)
								{
									if (x->stock[p].type[t] == SYNAPSE)
										{
											k += (x->stock[x->stock[p].destination[t]].error 
												* x->stock[p].weight[t]);
										}
								}
						}
					
					s = x->stock[p].potential;
					
					switch (x->stock[p].transfer) 
						{
							case TRANSFER_LINEAR  :	g = 1.; break;
							case TRANSFER_SIGMOID :	g = (1. / (1. + exp (-s))) * (1. - (1. / (1. + exp (-s))));
													break;
							case TRANSFER_TANH	  :	g = 4. / (pow ((exp (s) + exp (-s)), 2)); break;
						}
						
					x->stock[p].error = g * k;
					
					if (x->stock[p].index)
						{
							long t;
							
							for (t = 0; t < x->stock[p].index; t++) 
								{
									if (x->stock[p].type[t] == SYNAPSE)
										{
											x->stock[p].weight[t] += x->learning 
												* x->stock[x->stock[p].destination[t]].error * x->stock[p].signal;
										}
								}
						}
				}
		}
}

void papa_initialisation (t_papa *x)
{
	long i;
	
	for (i = 0; i < x->stockSize; i++)
		{
			x->stock[i].delayA			= x->stock[i].delayB;
			x->stock[i].tempPotential	= x->stock[i].potential;
			x->stock[i].tempSignal		= x->stock[i].signal;
			x->stock[i].delayB			= 0.;
			x->stock[i].potential		= 0.;
			x->stock[i].signal			= 0.;
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void papa_buildGraph (t_papa *x)
{
	t_object *ptr_p = NULL;
					
	papa_freeGraph (x);
	
	if (!(object_obex_lookup (x, gensym ("#P"), &ptr_p)))
		{
			long		result	= 0;
			t_object	*mybox	= NULL;
			
			if (!(object_obex_lookup (x, gensym ("#B"), &mybox))) 
				{
					object_method (ptr_p, gensym ("iterate"), (method)papa_iterateToBuildGraph, x, PI_WANTBOX, &result);
					
					if (x->oscar)   
						{
							x->vectorIn		= (double *)sysmem_newptrclear (sizeof(double) * x->sizeInputLayer);
							x->vectorOut	= (double *)sysmem_newptrclear (sizeof(double) * x->sizeOutputLayer);
							x->vectorTarget	= (double *)sysmem_newptrclear (sizeof(double) * x->sizeOutputLayer);
					
							object_method (x->oscar, gensym ("setpapa"), x->me);
							
							if (x->sizeHiddenLayers <= MAXIMUM_NODES)
								{
									t_object	*box = NULL;
									t_object	*line = NULL;
									long		position = 0;
									long		synapses_total = 0;
									
									x->stockSize = x->sizeHiddenLayers + x->sizeInputLayer + x->sizeOutputLayer;
																	
									if (x->stock = (t_node *)sysmem_newptrclear (sizeof(t_node) * x->stockSize))
										{
											long i;
											long k = 0;
											
											for (i = 0; i < x->sizeInputLayer; i ++)
												{
													x->stock[position].object	= x->oscar;
													x->stock[position].box		= x->oscarBox;
													x->stock[position].mode		= MODE_START;
													x->stock[position].state	= STATE_GREY;
													x->stock[position].distance	= 0;
													x->stock[position].transfer	= x->oscarTransfer;
													x->stock[position].index	= 0;
													
													position ++;
												}
																								
											for (i = 0; i < x->sizeOutputLayer; i ++)
												{
													x->stock[position].object	= x->me;
													x->stock[position].box		= x->meBox;
													x->stock[position].mode		= MODE_NORMAL;
													x->stock[position].state	= STATE_BLACK;
													x->stock[position].distance	= DISTANCE_OUTPUT_LAYER;
													x->stock[position].transfer	= x->papaTransfer;
													x->stock[position].index	= 0;
													
													position ++;
												}
											
											box = jpatcher_get_firstobject (ptr_p); 

											while (box) 
												{
													t_symbol *sym_b = jpatcher_get_maxclass (box);
													
													if (sym_b != gensym ("oscar") && sym_b != gensym ("papa"))
														{
															x->stock[position].object	= jbox_get_object (box);
															x->stock[position].box		= box;
															x->stock[position].mode		= MODE_NORMAL;	
															x->stock[position].state	= STATE_WHITE;
															x->stock[position].distance	= DISTANCE_LONELY_NODE;
															x->stock[position].transfer	= TRANSFER_TANH;
															x->stock[position].index	= 0;
																															
															if (sym_b == gensym ("+"))
																{
																	x->stock[position].mode		= MODE_NO_BACKWARD;
																	x->stock[position].transfer	= TRANSFER_LINEAR;
																}
															else if (sym_b == gensym ("-"))
																{
																	x->stock[position].mode		= MODE_NORMAL;
																	x->stock[position].transfer	= TRANSFER_LINEAR;
																}
															else if (sym_b == gensym ("pack"))
																{
																	x->stock[position].transfer	= TRANSFER_SIGMOID;
																}
															
															position ++;	
														}
																
													box = jbox_get_nextobject (box); 
												}
											
											line = jpatcher_get_firstline (ptr_p);
											
											while (line) 
												{
													long		m, n;
													t_object	*ptr_b1 = jpatchline_get_box1 (line);
													t_object	*ptr_b2 = jpatchline_get_box2 (line);
													t_object	*ptr_o1 = jbox_get_object (ptr_b1);
													t_object	*ptr_o2 = jbox_get_object (ptr_b2);
													long		a = jpatchline_get_outletnum (line);
													long		b = jpatchline_get_inletnum (line);
													
													jpatchline_set_color (line, &x->lineWrong);
													
													m = papa_stockGetIndexByPtrOutlet (x, ptr_o1, a);
													n = papa_stockGetIndexByPtrInlet (x, ptr_o2, b);
													
													if (!(m == -1 || n == -1) && synapses_total < MAXIMUM_SYNAPSES)
														{
															x->stock[m].line[x->stock[m].index]			= line;
															x->stock[m].type[x->stock[m].index]			= SYNAPSE_WRONG;
															x->stock[m].destination[x->stock[m].index]	= n;
															x->stock[m].weight[x->stock[m].index]		= papa_randomGaussian ();
																														
															synapses_total ++;
															x->stock[m].index ++;
														}
																											
													line = jpatchline_get_nextline (line);
												}
											
											for (i = 0; i < x->sizeInputLayer; i++)
												{
													k += papa_putFifo (x, i);
												}
											
											papa_graphBoxColor (x, x->oscarBox, x->oscarTransfer);
											papa_graphBoxColor (x, x->meBox, x->papaTransfer);
											
											if (!k)
												{
													while (!papa_getFifo(x))
														{
															if (x->valueQueue >= (x->sizeInputLayer + x->sizeOutputLayer))
																{
																	papa_graphBoxColor (x, x->stock[x->valueQueue].box, 
																		x->stock[x->valueQueue].transfer);
																}
																
															if (x->stock[x->valueQueue].index)
																{
																	long j;
																	
																	for (j = 0; j < x->stock[x->valueQueue].index; j++)
																		{
																			x->stock[x->valueQueue].type[j] = SYNAPSE;
																			
																			jpatchline_set_color (x->stock[x->valueQueue].line[j], 
																				&x->lineSynapse);

																			if (x->stock[x->stock[x->valueQueue].destination[j]].distance 
																				!= DISTANCE_OUTPUT_LAYER && 
																				!x->stock[x->stock[x->valueQueue].destination[j]].state)
																				{
																					x->stock[x->stock[x->valueQueue].destination[j]].state 
																						= STATE_GREY;
																					x->stock[x->stock[x->valueQueue].destination[j]].distance 
																						= x->stock[x->valueQueue].distance + 1;
																					
																					if (x->stock[x->stock[x->valueQueue].destination[j]].distance > x->maximumDistance)
																						{
																							x->maximumDistance = x->stock[x->stock[x->valueQueue].destination[j]].distance;
																						}
																					
																					papa_putFifo (x, x->stock[x->valueQueue].destination[j]);
																				}
																		}
																}
															
															x->stock[x->valueQueue].state = STATE_BLACK;
														}
												}
											
											for (i = x->sizeInputLayer; i < (x->sizeInputLayer + x->sizeOutputLayer); i++)
												{
													if (x->stock[i].index)
														{
															long j;
															
															for (j = 0; j < x->stock[i].index; j++)
																{
																	if (x->stock[x->stock[i].destination[j]].mode != MODE_START)
																		{
																			x->stock[i].type[j]		= SYNAPSE_WRONG;
																			x->stock[i].weight[j]	= WEIGHT_WRONG;
																					
																			jpatchline_set_color (x->stock[i].line[j], &x->lineWrong);
																		}
																	else
																		{
																			x->stock[i].type[j]		= SYNAPSE_FEEDBACK;
																			x->stock[i].weight[j]	= WEIGHT_FEEDBACK;
																			
																			jpatchline_set_color (x->stock[i].line[j], &x->lineFeedback);
																		}
																}
														} 
												}
												
											x->graphMap	= (long *)sysmem_newptrclear (sizeof(long) * (x->maximumDistance + 2));
											x->graph	= (long *)sysmem_newptrclear (sizeof(long) * ((x->maximumDistance + 2) * MAXIMUM_NODES_PER_LAYER));
											
											if (x->graph && x->graphMap)
												{
													long j;
													
													for (j = 0; j < x->stockSize; j ++)
														{
															if (x->stock[j].distance == DISTANCE_OUTPUT_LAYER && 
																x->graphMap[x->maximumDistance + 1] < MAXIMUM_NODES_PER_LAYER)
																{
																	x->graph[((x->maximumDistance + 1) * MAXIMUM_NODES_PER_LAYER) 
																		+ x->graphMap[x->maximumDistance + 1]] = j;
																	x->graphMap[x->maximumDistance + 1] ++;
																}
															else if (x->stock[j].distance >= 0 && 
																x->graphMap[x->stock[j].distance] < MAXIMUM_NODES_PER_LAYER)
																{
																	x->graph[((x->stock[j].distance) * MAXIMUM_NODES_PER_LAYER) 
																		+ x->graphMap[x->stock[j].distance]] = j;
																	x->graphMap[x->stock[j].distance] ++;
																}	
															else if (x->stock[j].distance != DISTANCE_LONELY_NODE)
																{
																	long t;
																	
																	x->stock[j].state = STATE_NO_BACKPROPAGATION;
																	 
																	papa_graphBoxColor (x, x->stock[j].box, TRANSFER_ERROR);
																	
																	for (t = 0; t < x->stock[j].index; t++)
																		{
																			x->stock[j].type[t]		= SYNAPSE_WRONG;
																			x->stock[j].weight[t]	= WEIGHT_WRONG;
																			
																			jpatchline_set_color (x->stock[j].line[t], &x->lineWrong);
																		}
																}
														}
													
													for (j = x->maximumDistance; j >= 0; j--)
														{
															long w;
															
															for (w = 0; w < x->graphMap[j]; w++)
																{
																	long	p;
																	long	z = 0;
																	long	q = 1;
																	
																	p = x->graph[(j * MAXIMUM_NODES_PER_LAYER) + w];
																	
																	if (x->stock[p].index)
																		{
																			long t;
																			
																			for (t = 0; t < x->stock[p].index; t++)
																				{
																					if (x->stock[x->stock[p].destination[t]].mode == MODE_FINAL && 
																					x->stock[x->stock[p].destination[t]].distance 
																					!= DISTANCE_OUTPUT_LAYER)
																						{ 
																							x->stock[p].type[t]		= SYNAPSE_WRONG;
																							x->stock[p].weight[t]	= WEIGHT_WRONG;
																									
																							jpatchline_set_color (x->stock[p].line[t], &x->lineWrong);
																						}
																					else
																						{
																							if (x->stock[p].mode == MODE_NO_BACKWARD)
																								{
																									x->stock[p].type[t]		= SYNAPSE_FEEDBACK;
																									x->stock[p].weight[t]	= WEIGHT_FEEDBACK;
																											
																									jpatchline_set_color (x->stock[p].line[t], &x->lineFeedback);
																								}
																							else if (x->stock[x->stock[p].destination[t]].state == STATE_NO_BACKPROPAGATION)
																								{
																									if (x->stock[x->stock[p].destination[t]].mode == MODE_NORMAL 
																										|| (x->stock[p].distance < x->stock[x->stock[p].destination[t]].distance))
																										{
																											x->stock[p].type[t]		= SYNAPSE_FEEDBACK;
																											x->stock[p].weight[t]	= WEIGHT_FEEDBACK;
																											
																											jpatchline_set_color (x->stock[p].line[t], &x->lineFeedback);
																										}
																									else
																										{
																											x->stock[p].type[t]		= SYNAPSE_WRONG;
																											x->stock[p].weight[t]	= WEIGHT_WRONG;
																									
																											jpatchline_set_color (x->stock[p].line[t], &x->lineWrong);
																										}
																								}
																							else if ((x->stock[p].distance >= x->stock[x->stock[p].destination[t]].distance) && 
																							x->stock[x->stock[p].destination[t]].distance != DISTANCE_OUTPUT_LAYER)
																								{
																									if (x->stock[x->stock[p].destination[t]].mode == MODE_NORMAL)
																										{
																											x->stock[p].type[t]	 = SYNAPSE_FEEDBACK;
																											x->stock[p].weight[t]	 = WEIGHT_FEEDBACK;
																									
																											jpatchline_set_color (x->stock[p].line[t], &x->lineFeedback);
																										}
																									else
																										{
																											x->stock[p].type[t]	= SYNAPSE_WRONG;
																											x->stock[p].weight[t]	= WEIGHT_WRONG;
																									
																											jpatchline_set_color (x->stock[p].line[t], &x->lineWrong);
																										}
																								}
																							else
																								{
																									z = 1;
																								}
																						}
																				}
																				
																			for (t = 0; t < x->stock[p].index; t++)
																				{
																					if (x->stock[p].type[t]	!= SYNAPSE_WRONG)
																						{
																							q = 0;
																						}
																				}
																		}
																	
																	if (q)
																		{
																			x->stock[p].mode = MODE_FINAL;
																		}
																	
																	if (!z)
																		{
																			x->stock[p].state = STATE_NO_BACKPROPAGATION;
																		}
																}
														}
												}
										}
								}
						}
				}
		}
}	

long papa_iterateToBuildGraph (t_papa *x, t_object *box)
{
	t_symbol *sym_b = jpatcher_get_maxclass (box); 
	
	if (sym_b == gensym ("oscar"))
		{
			if (!x->oscar)
				{
					x->oscar			= jbox_get_object (box);
					x->oscarBox			= box;
					x->sizeInputLayer	= object_attr_getlong (box, gensym ("numoutlets"));
				}
		}
	else if (sym_b != gensym ("papa"))
		{
			x->sizeHiddenLayers ++;
		}
		
	return 0;
}

long papa_stockGetIndexByPtrOutlet (t_papa *x, t_object *object, long n) 
{
	long k = -1;
	
	if (object == x->oscar && x->stock[n].index < MAXIMUM_SYNAPSES_PER_NODE)
		{
			k = n;
		}
	else if (object == x->me && n)
		{
			if (x->stock[(n - 1) + x->sizeInputLayer].index < MAXIMUM_SYNAPSES_PER_NODE)
				{
					k = (n - 1) + x->sizeInputLayer;
				}
		}
	else 
		{
			long i;
			
			for (i = (x->sizeInputLayer + x->sizeOutputLayer); i < x->stockSize; i ++)
				{
					if (object == x->stock[i].object)
						{
							if (x->stock[i].index < MAXIMUM_SYNAPSES_PER_NODE)
								{
									k = i;
								}
								
							break;
						}
				}
		}
	
	return k;
}

long papa_stockGetIndexByPtrInlet (t_papa *x, t_object *object, long n) 
{
	long k = -1;
	
	if (object == x->me)
		{
			k = n + x->sizeInputLayer;
		}
	else if (object == x->oscar)
		{
			if (n)
				{
					k = n - 1;
				}
		}
	else
		{
			long i;
			
			for (i = (x->sizeInputLayer + x->sizeOutputLayer); i < x->stockSize; i ++)
				{
					if (object == x->stock[i].object)
						{
							k = i;
							break;
						}
				}
		}
	
	return k;
}

void papa_graphBoxColor (t_papa *x, t_object *box, long n)
{
	if (box)
		{
			switch (n) {
				case TRANSFER_LINEAR  :	jbox_set_color (box, &x->boxLinear);	break;
				case TRANSFER_SIGMOID :	jbox_set_color (box, &x->boxSigmoid);	break;
				case TRANSFER_TANH	  :	jbox_set_color (box, &x->boxTanh);		break;
				case TRANSFER_ERROR	  :	jbox_set_color (box, &x->boxError);		break;
			}
		}
}
	
void papa_freeGraph (t_papa *x)
{
	if (x->oscar)   
		{
			object_method (x->oscar, gensym ("setpapa"), NULL);
			x->oscar = NULL;
		}
		
	if (x->stock) {
			sysmem_freeptr (x->stock);
		}
	
	if (x->graph) {
			sysmem_freeptr (x->graph);
		}
		
	if (x->graphMap) {
			sysmem_freeptr (x->graphMap);
		}
		
	if (x->vectorIn) {
			sysmem_freeptr (x->vectorIn);
		}
	
	if (x->vectorOut) {
			sysmem_freeptr (x->vectorOut);
		}
	
	if (x->vectorTarget) {
			sysmem_freeptr (x->vectorTarget);
		}
	
	x->sizeInputLayer	= 0;
	x->sizeHiddenLayers	= 0;
	x->stockSize		= 0;
	x->maximumDistance	= 0;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

double papa_randomGaussian (void)
{
	double x1, x2, w, y;

	do { x1	= 2. * ((double)rand () / (double)RAND_MAX) - 1.;
		 x2	= 2. * ((double)rand () / (double)RAND_MAX) - 1.;
		 w	= x1 * x1 + x2 * x2;
		} while (w >= 1. || w == 0.); 

	 w	= sqrt ((-2. * log(w)) / w);
	 y  = x1 * w;
	 
	 return y * 0.5;
}

long papa_putFifo (t_papa *x, long n) 
{
	long k = -1;
	
	if (x->queue && ((x->tailQueue + 1) != x->headQueue) && 
		!((x->tailQueue == QUEUE_SIZE) && (x->headQueue == 0)))
		{
			k = 0;
			
			x->queue[x->tailQueue] = n;
			
			if (x->tailQueue == QUEUE_SIZE)
				{
					x->tailQueue = 0;
				}
			else
				{
					x->tailQueue ++;
				}
		}
	
	return k;
}
	
long papa_getFifo (t_papa *x)
{
	long k = -1;
	
	if (x->queue && (x->headQueue != x->tailQueue))
		{
			k = 0;
			
			x->valueQueue = x->queue[x->headQueue];
			
			if (x->headQueue == QUEUE_SIZE)
				{
					x->headQueue = 0;
				}
			else
				{
					x->headQueue ++;
				}
		}
	
	return k;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
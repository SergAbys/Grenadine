/*
 *  pizKohonenMap.c
 *
 *  Created : 27/08/11.
 *  Copyright 2011 : Tom Javel & Jean Sapristi.
 *
 *  nicolas.danet@free.fr
 *
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */
 
/*
 *	Last modified : 29/12/11.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizKohonenMap.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>	
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_MAXIMUM_MAP_SIZE			100
#define PIZ_MAXIMUM_VECTOR_SIZE			256
#define PIZ_ALPHABET_SIZE				128

#define PIZ_DEFAULT_MAP_SIZE			20
#define PIZ_DEFAULT_VECTOR_SIZE			4

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZKohonenMap *pizKohonenMapNew (long argc, long *argv)
{
	PIZKohonenMap *x = NULL;

	if (x = (PIZKohonenMap *)calloc (1, sizeof(PIZKohonenMap)))
		{
			x->mapSize		= PIZ_DEFAULT_MAP_SIZE;
			x->vectorSize	= PIZ_DEFAULT_VECTOR_SIZE;
			
			x->range		= PIZ_KOHONEN_MAP_DEFAULT_RANGE;
			x->training		= PIZ_KOHONEN_MAP_DEFAULT_TRAINING;
			x->step			= PIZ_KOHONEN_MAP_DEFAULT_STEP;
			
			if (argc && ((argv[0] > 0) && (argv[0] <= PIZ_MAXIMUM_MAP_SIZE)))
				{
					x->mapSize	= argv[0];
				}
			
			if ((argc == 2) && ((argv[1] > 0) && (argv[1] <= PIZ_MAXIMUM_VECTOR_SIZE)))
				{
					x->vectorSize = argv[1];
				}
			
			srand ((unsigned int)time(NULL));
					
			if (x->map = (double *)malloc (sizeof(double) * (x->mapSize * x->vectorSize)))
				{
					long i;
					
					x->count = 0;
					
					for (i = 0; i < (x->mapSize * x->vectorSize); i++)
						{
							x->map[i] = PIZ_ALPHABET_SIZE * (rand ( ) / (RAND_MAX + 1.0));
						}
				}
			else
				{
					pizKohonenMapFree (x);
					x = NULL;
				}
		}
	
	return x;
}

void pizKohonenMapFree (PIZKohonenMap *x)
{
	if (x)
		{
			if (x->map)
				{
					free (x->map);
					x->map = NULL;
				}
			
			free (x);
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizKohonenMapAdd (PIZKohonenMap *x, long argc, long *argv)
{
	long err = PIZ_ERROR;
	
	if (argc && argv)
		{
			long t;
			long k = (argc / x->vectorSize);
			
			err = PIZ_GOOD;
			
			for (t = 0; t < argc; t++)
				{
					argv[t] = CLAMP (argv[t], 0, PIZ_ALPHABET_SIZE - 1); 
				}
						
			for (t = 0; t < k; t++)
				{
					long	i;
					double	sigma ;
					double	eta;
					double	dist1 = 0.;
					double  dist2 = 0.;
					long	winner = 0;
					
					for (i = 0; i < x->vectorSize; i++)
						{
							dist1 += pow (x->map[i] - argv[(t * x->vectorSize) + i], 2);
						}
					
					dist1 = sqrt (dist1);
						
					for (i = 1; i < x->mapSize; i++)
						{
							long j;
											
							for (j = 0; j < x->vectorSize; j++)
								{
									dist2 += pow (x->map[(i * x->vectorSize) + j] - 
													argv[(t * x->vectorSize) + j], 2);
								}
							
							dist2 = sqrt (dist2);

							if (dist2 < dist1)
								{
									dist1 = dist2;
									winner = i;
								}
						}
					
					x->count	= MIN (x->count + 1, x->training);
					
					sigma		= x->range * exp (- x->count / (x->training / log (x->range)));
					eta			= x->step * exp (- x->count / (double)x->training);
					
					for (i = 0; i < x->mapSize; i++)
						{
							long	j;
							double	phi	= exp (- pow (i - winner, 2) / (2 * pow (sigma, 2)));
							
							for (j = 0; j < x->vectorSize; j++)
								{
									x->map[(i * x->vectorSize) + j] += eta * phi * 
											(argv[(t * x->vectorSize) + j] - x->map[(i * x->vectorSize) + j]);
								}
						}
				}
		}
		
	return err;
}

void pizKohonenMapClear (PIZKohonenMap *x)
{
	long i;
	
	x->count = 0;
	
	for (i = 0; i < (x->mapSize * x->vectorSize); i++)
		{
			x->map[i] = PIZ_ALPHABET_SIZE * (rand ( ) / (RAND_MAX + 1.0));
		}
}

long pizKohonenMapCount (PIZKohonenMap *x)
{
	return (x->count);
}

PIZError pizKohonenMapProceed (PIZKohonenMap *x, long argc, long *argv)
{
	long err = PIZ_ERROR;
	
	if (argc && argv)
		{
			long	i;
			double	*temp = NULL;
			
			for (i = 0; i < argc; i++)
				{
					if ((i % x->vectorSize) == 0) {
						temp = x->map + (((long)(x->mapSize * (rand ( ) / (RAND_MAX + 1.0)))) * x->vectorSize);
					}
					
					argv[i] = (long)floor (*(temp + (i % x->vectorSize)) + 0.5);
				}
			
			err = PIZ_GOOD;
		}
	
	return err;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long pizKohonenMapRange (PIZKohonenMap *x)
{
	return (x->range);
}

void pizKohonenMapSetRange (PIZKohonenMap *x, long n)
{
	x->range = MAX (n, 1);
}

long pizKohonenMapTraining (PIZKohonenMap *x)
{
	return (x->training);
}

void pizKohonenMapSetTraining (PIZKohonenMap *x, long n)
{
	x->training = MAX (n, 1);
}

double pizKohonenMapStep (PIZKohonenMap *x)
{
	return (x->step);
}

void pizKohonenMapSetStep (PIZKohonenMap *x, double f)
{
	if (f > 0.)
		{
			x->step = f;
		}
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizKohonenMapEncodeVectorToArray (PIZKohonenMap *x, long n, PIZGrowingArray *array)
{
	long err = PIZ_ERROR;
	
	if ((n >= 0) && (n < x->mapSize) && array)
		{
			long i;
			
			err = PIZ_GOOD;
			
			for (i = 0; i < x->vectorSize; i++) {
				err |= pizGrowingArrayAppend (array, (long)(floor ((*(x->map + (n * x->vectorSize) + i)) + 0.5)));
			}
		}
	
	return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
/*
 * \file    pizKohonenMap.c
 * \author  Jean Sapristi
 */
 
/*
 *  Copyright (c) 2011, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
 *
 *  This software is governed by the CeCILL-C license under French law and
 *  abiding by the rules of distribution of free software. You can use, 
 *  modify and/or redistribute the software under the terms of the CeCILL-C
 *  license as circulated by CEA, CNRS and INRIA at the following URL
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and rights to copy,
 *  modify and redistribute granted by the license, users are provided only
 *  with a limited warranty  and the software's author, the holder of the
 *  economic rights, and the successive licensors have only limited
 *  liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated
 *  with loading, using, modifying and/or developing or reproducing the
 *  software by the user in light of its specific status of free software,
 *  that may mean that it is complicated to manipulate, and that also
 *  therefore means that it is reserved for developers and experienced
 *  professionals having in-depth computer knowledge. Users are therefore
 *  encouraged to load and test the software's suitability as regards their
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and, more generally, to use and operate it in the 
 *  same conditions as regards security. 
 *  
 *  The fact that you are presently reading this means that you have had
 *  knowledge of the CeCILL-C license and that you accept its terms.
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizKohonenMap.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>   
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_SIZE_ALPHABET               128
#define PIZ_MAXIMUM_SIZE_MAP            100
#define PIZ_MAXIMUM_SIZE_VECTOR         256

#define PIZ_DEFAULT_SIZE_MAP            20
#define PIZ_DEFAULT_SIZE_VECTOR         4
#define PIZ_DEFAULT_RANGE               10
#define PIZ_DEFAULT_TRAINING            60
#define PIZ_DEFAULT_STEP                1.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZKohonenMap *pizKohonenMapNew (long argc, long *argv)
{
    PIZKohonenMap *x = NULL;

    if (x = (PIZKohonenMap *)calloc (1, sizeof(PIZKohonenMap))) {
        x->mapSize       = PIZ_DEFAULT_SIZE_MAP;
        x->vectorSize    = PIZ_DEFAULT_SIZE_VECTOR;
        x->range         = PIZ_DEFAULT_RANGE;
        x->training      = PIZ_DEFAULT_TRAINING;
        x->step          = PIZ_DEFAULT_STEP;
        
        if (argc && ((argv[0] > 0) && (argv[0] <= PIZ_MAXIMUM_SIZE_MAP))) {
            x->mapSize  = argv[0];
        }
        
        if ((argc == 2) && ((argv[1] > 0) && (argv[1] <= PIZ_MAXIMUM_SIZE_VECTOR))) {
            x->vectorSize = argv[1];
        }
        
        x->seed = (unsigned int)time(NULL);
                
        if (x->map = (double *)malloc (sizeof(double) * (x->mapSize * x->vectorSize))) {
            long i;
            
            x->count = 0;
            
            for (i = 0; i < (x->mapSize * x->vectorSize); i++) {
                x->map[i] = PIZ_SIZE_ALPHABET * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            }
        } else {
            pizKohonenMapFree (x);
            x = NULL;
        }
    }
    
    return x;
}

void pizKohonenMapFree (PIZKohonenMap *x)
{
    if (x) {
        if (x->map) {
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
    PIZError err = PIZ_ERROR;
    
    if (argc && argv) {
    //
    long t, k = (argc / x->vectorSize);
    
    err = PIZ_GOOD;
    
    for (t = 0; t < argc; t++) {
        argv[t] = CLAMP (argv[t], 0, PIZ_SIZE_ALPHABET - 1); 
    }
                
    for (t = 0; t < k; t++) {
    //
    long    i, j, winner = 0;
    double  sigma, eta ;
    double  dist1 = 0.;
    double  dist2 = 0.;
    
    for (i = 0; i < x->vectorSize; i++) {
        dist1 += pow (x->map[i] - argv[(t * x->vectorSize) + i], 2);
    }
    
    dist1 = sqrt (dist1);
        
    for (i = 1; i < x->mapSize; i++) {        
        for (j = 0; j < x->vectorSize; j++) {
            dist2 += pow (x->map[(i * x->vectorSize) + j] - argv[(t * x->vectorSize) + j], 2);
        }
        
        dist2 = sqrt (dist2);

        if (dist2 < dist1) {
            dist1 = dist2;
            winner = i;
        }
    }
    
    x->count = MIN (x->count + 1, x->training);
    
    sigma = x->range * exp (- x->count / (x->training / log (x->range)));
    eta = x->step * exp (- x->count / (double)x->training);
    
    for (i = 0; i < x->mapSize; i++) {
        double phi = exp (- pow (i - winner, 2) / (2 * pow (sigma, 2)));
        
        for (j = 0; j < x->vectorSize; j++) {
            double temp = (argv[(t * x->vectorSize) + j] - x->map[(i * x->vectorSize) + j]);
            x->map[(i * x->vectorSize) + j] += eta * phi * temp;
        }
    }
    //    
    }
    //
    }
        
    return err;
}

PIZError pizKohonenMapClear (PIZKohonenMap *x)
{
    long i;
    
    x->count = 0;
    
    for (i = 0; i < (x->mapSize * x->vectorSize); i++) {
        x->map[i] = PIZ_SIZE_ALPHABET * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    }
    
    return PIZ_GOOD;
}

PIZError pizKohonenMapProceed (PIZKohonenMap *x, long argc, long *argv)
{
    PIZError err = PIZ_ERROR;
    
    if (argc && argv) {
        long    i;
        double  *temp = NULL;
        
        for (i = 0; i < argc; i++) {
            if ((i % x->vectorSize) == 0) {
                temp = x->map + (((long)(x->mapSize * (rand_r (&x->seed) / (RAND_MAX + 1.0)))) * x->vectorSize);
            }
            
            argv[i] = (long)(*(temp + (i % x->vectorSize)) + 0.5);
        }
        
        err = PIZ_GOOD;
    }
    
    return err;
}

long pizKohonenMapCount (const PIZKohonenMap *x)
{
    return x->count;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
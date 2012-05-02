/*
 * \file    pizNeuralGas.c
 * \author  Jean Sapristi
 * \date    April 6, 2012.
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

#include "pizNeuralGas.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>   
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ALPHABET_SIZE               128
#define PIZ_MAXIMUM_VECTOR_SIZE         256

#define PIZ_DEFAULT_VECTOR_SIZE         4
#define PIZ_DEFAULT_MAXIMUM_SIZE        20
#define PIZ_DEFAULT_LAMBDA              2
#define PIZ_DEFAULT_ALPHA               0.5
#define PIZ_DEFAULT_BETA                0.1
#define PIZ_DEFAULT_EPSILON1            0.5
#define PIZ_DEFAULT_EPSILON2            0.25
#define PIZ_DEFAULT_KAPPA               10.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZNeuralGas *pizNeuralGasNew (long argc, long *argv)
{
    PIZNeuralGas *x = NULL;

    if (x = (PIZNeuralGas *)malloc (sizeof(PIZNeuralGas))) {
    //
    PIZError err = PIZ_GOOD;
    
    if (x->headStock = (PIZNeuralGasHead *)malloc (PIZ_ITEMSET128_SIZE * sizeof(PIZNeuralGasHead))) {
        long k, i;
        
        x->count         = 0;
        x->vectorSize    = PIZ_DEFAULT_VECTOR_SIZE;
        x->maximumSize   = PIZ_DEFAULT_MAXIMUM_SIZE;
        x->lambda        = PIZ_DEFAULT_LAMBDA;
        x->epsilon1      = PIZ_DEFAULT_EPSILON1;
        x->epsilon2      = PIZ_DEFAULT_EPSILON2;
        x->alpha         = PIZ_DEFAULT_ALPHA;
        x->beta          = PIZ_DEFAULT_BETA;
        x->kappa         = PIZ_DEFAULT_KAPPA;
        
        x->algorithm.type       = PIZ_ALGORITHM_TYPE_NEURAL_GAS;
        x->algorithm.add        = pizNeuralGasAdd;
        x->algorithm.clear      = pizNeuralGasClear;
        x->algorithm.proceed    = pizNeuralGasProceed;
        x->algorithm.count      = pizNeuralGasCount;
        
        x->seed = (unsigned int)time(NULL);
        
        if (argc && ((argv[0] > 1)  && (argv[0] <= PIZ_ITEMSET128_SIZE))) {
            x->maximumSize = argv[0];
        }
        
        if ((argc > 1) && ((argv[1] > 0)  && (argv[1] <= PIZ_MAXIMUM_VECTOR_SIZE))) {
            x->vectorSize = argv[1];
        }
        
        k = PIZ_ITEMSET128_SIZE * x->vectorSize;
        
        if (!(x->vectorStock = (double *)malloc (k * sizeof(double)))) {
            err = PIZ_MEMORY;
        }
            
        if (x->ticketMachine = pizBoundedStackNew (PIZ_ITEMSET128_SIZE)) {
            for (i = (PIZ_ITEMSET128_SIZE - 1); i > 1; i--) {
                pizBoundedStackPush (x->ticketMachine, i);
            }
        } else {
            err = PIZ_MEMORY;
        }
        
        if (!err) {
            for (i = 0; i < (x->vectorSize * 2); i++) {
                x->vectorStock[i] = PIZ_ALPHABET_SIZE * (rand_r (&x->seed) / (RAND_MAX + 1.0));
            }
            
            x->headStock[0].error   = 0.;
            x->headStock[0].utility = 1.;
            x->headStock[1].error   = 0.;
            x->headStock[1].utility = 1.;
            
            pizItemset128Clear (&x->headStock[0].arcs);
            pizItemset128Clear (&x->headStock[1].arcs);
            
            pizItemset128SetAtIndex (&x->headStock[0].arcs, 1);
            pizItemset128SetAtIndex (&x->headStock[1].arcs, 0);
            
            pizItemset128Clear (&x->map);
            
            pizItemset128SetAtIndex (&x->map, 0);
            pizItemset128SetAtIndex (&x->map, 1);
            
            x->mapSize = 2;
            
        } else {
            pizNeuralGasFree (x);
            x = NULL;
        }
    } else {
        free (x);
        x = NULL;
    }
    //    
    }
    
    return x;
}

void pizNeuralGasFree (PIZNeuralGas *x)
{
    if (x) {
        pizBoundedStackFree (x->ticketMachine);
        x->ticketMachine = NULL;
        
        free (x->headStock);
        x->headStock = NULL;
        
        if (x->vectorStock) {
            free (x->vectorStock);
            x->vectorStock = NULL;
        }
        
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizNeuralGasAdd (PIZNeuralGas *x, long argc, long *argv)
{
    PIZError err = PIZ_ERROR;
    
    if (argc && argv) {
    //
    long t;
    long k = (argc / x->vectorSize);
    
    err = PIZ_GOOD;
    
    for (t = 0; t < argc; t++) {
        argv[t] = CLAMP (argv[t], 0, PIZ_ALPHABET_SIZE - 1); 
    }
                
    for (t = 0; t < k; t++) {
    //   
    long   i, j;
    long   winner1 = -1;
    long   winner2 = -1;
    double dist1 = 0.;
    double dist2 = 0.;
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        double e = 0.;
        
        if (pizItemset128IsSetAtIndex (&x->map, i)) {
            for (j = 0; j < x->vectorSize; j++) {
                e += pow (x->vectorStock[(i * x->vectorSize) + j] - argv[(t * x->vectorSize) + j], 2);
            }
            
            if ((e < dist1) || (winner1 == -1)) {
                dist2   = dist1;
                winner2 = winner1;
                dist1   = e;
                winner1 = i;
            } else if ((e < dist2) || (winner2 == -1)) {
                dist2   = e;
                winner2 = i;
            }
        }   
    }

    x->headStock[winner1].error     += dist1;
    x->headStock[winner1].utility   += (dist2 - dist1);
    
    for (i = 0; i < x->vectorSize; i++) {
        double temp = (argv[(t * x->vectorSize) + i] - x->vectorStock[(winner1 * x->vectorSize) + i]);
        x->vectorStock[(winner1 * x->vectorSize) + i] += x->epsilon1 * temp;
    }
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->headStock[winner1].arcs, i)) {
            for (j = 0; j < x->vectorSize; j++) {
                double temp = (argv[(t * x->vectorSize) + j] - x->vectorStock[(i * x->vectorSize) + j]);
                x->vectorStock[(i * x->vectorSize) + j] += x->epsilon2 * temp;
            }
        }
    }
    
    if (!(x->count % x->lambda)) {
        long    maxError1 = -1;
        long    maxError2 = -1;
        long    minUtility = -1;
        double  maxError1Value = 0.;
        double  maxError2Value = 0.;
        double  minUtilityValue = 0.;
        
        for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
            if (pizItemset128IsSetAtIndex (&x->map, i)) {
                if ((x->headStock[i].error > maxError1Value) || (maxError1 == -1)) {
                    maxError1       = i;
                    maxError1Value  = x->headStock[i].error;
                }   
                
                if ((x->headStock[i].utility < minUtilityValue) || (minUtility == -1)) {
                    minUtility      = i;
                    minUtilityValue = x->headStock[i].utility;
                }
            }
        }
        
        for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
            if (pizItemset128IsSetAtIndex (&x->headStock[maxError1].arcs, i)) {
                if ((x->headStock[i].error > maxError2Value) || (maxError2 == -1)) {
                    maxError2       = i;
                    maxError2Value  = x->headStock[i].error;
                }
            }
        }
        
        if ((x->mapSize < x->maximumSize) && !(pizBoundedStackPop (x->ticketMachine))) {
            long p = pizBoundedStackPoppedValue (x->ticketMachine);
            
            pizItemset128Clear (&x->headStock[p].arcs);

            for (i = 0; i < x->vectorSize; i++) {
                double temp1 = x->vectorStock[(maxError1 * x->vectorSize) + i];
                double temp2 = x->vectorStock[(maxError2 * x->vectorSize) + i];
                x->vectorStock[(p * x->vectorSize) + i] = (temp1 + temp2) / 2.;
            }

            pizItemset128UnsetAtIndex (&x->headStock[maxError1].arcs, maxError2);
            pizItemset128UnsetAtIndex (&x->headStock[maxError2].arcs, maxError1);
            
            pizItemset128SetAtIndex (&x->headStock[maxError1].arcs, p);
            pizItemset128SetAtIndex (&x->headStock[maxError2].arcs, p);
            pizItemset128SetAtIndex (&x->headStock[p].arcs, maxError1);
            pizItemset128SetAtIndex (&x->headStock[p].arcs, maxError2);
            
            x->headStock[maxError1].error = x->alpha * x->headStock[maxError1].error;
            x->headStock[maxError2].error = x->alpha * x->headStock[maxError2].error;
            
            x->headStock[p].error = (x->headStock[maxError1].error + x->headStock[maxError2].error) / 2.;
            x->headStock[p].utility = (x->headStock[maxError1].utility + x->headStock[maxError2].utility) / 2.;
            
            pizItemset128SetAtIndex (&x->map, p);
            
            x->mapSize ++;
        }
        
        if ((maxError1Value / minUtilityValue) > x->kappa) {
            for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
                if (pizItemset128IsSetAtIndex (&x->headStock[minUtility].arcs, i)) {
                    pizItemset128UnsetAtIndex (&x->headStock[i].arcs, minUtility);
                }
            }
            
            pizItemset128UnsetAtIndex (&x->map, minUtility);
            x->mapSize --;
            pizBoundedStackPush (x->ticketMachine, minUtility);
        }
        
        for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
            if (pizItemset128IsSetAtIndex (&x->map, i)) {
                if (!(pizItemset128Count (&x->headStock[i].arcs))) {
                    pizItemset128UnsetAtIndex (&x->map, i);
                    x->mapSize --;
                    pizBoundedStackPush (x->ticketMachine, i);
                }
            }
        }
    }
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
        if (pizItemset128IsSetAtIndex (&x->map, i)) {
            x->headStock[i].error   -= x->beta * x->headStock[i].error;
            x->headStock[i].utility -= x->beta * x->headStock[i].utility;
        }
    }
    
    if (!x->mapSize) {
        pizNeuralGasClear (x);
    } else {
        x->count ++;
    }
    //    
    }
    //
    }
        
    return err;
}

PIZError pizNeuralGasClear (PIZNeuralGas *x)
{
    long i;
    
    x->count = 0;
    
    for (i = 0; i < (x->vectorSize * 2); i++) {
        x->vectorStock[i] = PIZ_ALPHABET_SIZE * (rand_r (&x->seed) / (RAND_MAX + 1.0));
    }
    
    x->headStock[0].error   = 0.;
    x->headStock[0].utility = 1.;
    x->headStock[1].error   = 0.;
    x->headStock[1].utility = 1.;
    
    pizItemset128Clear (&x->headStock[0].arcs);
    pizItemset128Clear (&x->headStock[1].arcs);
    
    pizItemset128SetAtIndex (&x->headStock[0].arcs, 1);
    pizItemset128SetAtIndex (&x->headStock[1].arcs, 0);
    
    pizItemset128Clear (&x->map);
    
    pizItemset128SetAtIndex (&x->map, 0);
    pizItemset128SetAtIndex (&x->map, 1);
    
    pizBoundedStackClear (x->ticketMachine);
    
    for (i = (PIZ_ITEMSET128_SIZE - 1); i > 1; i--) {
        pizBoundedStackPush (x->ticketMachine, i);
    }
    
    x->mapSize = 2;
    
    return PIZ_GOOD;
}

PIZError pizNeuralGasProceed (PIZNeuralGas *x, long argc, long *argv)
{
    PIZError err = PIZ_ERROR;
    
    if (argc && argv) {
    //
    long    i;
    double  *temp = NULL;
    
    for (i = 0; i < argc; i++) {
        if ((i % x->vectorSize) == 0) {
            long j, p = -1;
            long h = (long)(x->mapSize * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
            
            for (j = 0; j < PIZ_ITEMSET128_SIZE; j++) {
                if (pizItemset128IsSetAtIndex (&x->map, j)) {
                    if (!h) {
                        p = j;
                        break;
                    }
                    h --;
                }
            }
            
            temp = x->vectorStock + (p * x->vectorSize);
        }
        
        argv[i] = (long)(*(temp + (i % x->vectorSize)) + 0.5);
    }
    
    err = PIZ_GOOD;
    //    
    }
    
    return err;
}

long pizNeuralGasCount (const PIZNeuralGas *x)
{
    return x->count;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
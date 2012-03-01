/*
 * \file    pizNeuralGas.c
 * \author  Jean Sapristi
 * \date    February 29, 2012.
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

#include "pizAlgorithmsMaxMSP.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizNeuralGasSetLambda (PIZNeuralGas *x, long n)
{
    x->lambda = MAX (n, 1);
}

void pizNeuralGasSetEpsilon1 (PIZNeuralGas *x, double f)
{
    if (f > 0. && f < 1.) {
        x->epsilon1 = f;
    }
}

void pizNeuralGasSetEpsilon2 (PIZNeuralGas *x, double f)
{
    if (f > 0. && f < 1.) {
        x->epsilon2 = f;
    }
}

void pizNeuralGasSetAlpha (PIZNeuralGas *x, double f)
{
    if (f > 0. && f < 1.) {
        x->alpha = f;
    }
}

void pizNeuralGasSetBeta (PIZNeuralGas *x, double f)
{
    if (f > 0. && f < 1.) {
            x->beta = f;
        }
}

void pizNeuralGasSetKappa (PIZNeuralGas *x, double f)
{
    if (f > 0.) {
        x->kappa = f;
    }
}

long pizNeuralGasLambda (const PIZNeuralGas *x)
{
    return (x->lambda);
}

double pizNeuralGasEpsilon1 (const PIZNeuralGas *x)
{
    return (x->epsilon1);
}

double pizNeuralGasEpsilon2 (const PIZNeuralGas *x)
{
    return (x->epsilon2);
}

double pizNeuralGasAlpha (const PIZNeuralGas *x)
{
    return (x->alpha);
}

double pizNeuralGasBeta (const PIZNeuralGas *x)
{
    return (x->beta);
}

double pizNeuralGasKappa (const PIZNeuralGas *x)
{
    return (x->kappa);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizNeuralGasEncodeToArray (const PIZNeuralGas *x, long n, PIZGrowingArray *a)
{
    long err = PIZ_ERROR;
    
    if ((n >= 0) && (n < x->mapSize) && a) {
        long i, j, t, k = 0;
        
        err = PIZ_GOOD;
        
        for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
            if (pizItemset128IsSetAtIndex (&x->map, i)) {
                if (k == n) {
                    for (j = 0; j < x->vectorSize; j++) {
                        t = (long)(((*(x->vectorStock + (n * x->vectorSize) + j)) + 0.5));
                        err |= pizGrowingArrayAppend (a, t);
                    }
                }
                
                k ++;
            }
        }
    }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
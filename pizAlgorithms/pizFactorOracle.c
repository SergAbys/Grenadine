/*
 * \file    pizFactorOracle.c
 * \author  Jean Sapristi
 * \date    April 8, 2012.
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

#include "pizFactorOracle.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_INIT_ARRAY_SIZE                 4
#define PIZ_INIT_ORACLE_SIZE                32
#define PIZ_DEFAULT_BACKWARD_THRESHOLD      2
#define PIZ_DEFAULT_STRAIGHT_RATIO          0.25

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZFactorOracle *pizFactorOracleNew (long argc, long *argv)
{
    PIZFactorOracle *x = NULL;

    if (x = (PIZFactorOracle *)malloc (sizeof(PIZFactorOracle))) {
    //
    if (x->nodes = (PIZFactorOracleNode *)malloc (PIZ_INIT_ORACLE_SIZE * sizeof(PIZFactorOracleNode))) {
        x->nodes[0].arcValues = pizArrayNew (PIZ_INIT_ARRAY_SIZE);
        x->nodes[0].arcDestinations = pizArrayNew (PIZ_INIT_ARRAY_SIZE);
        
        if (x->nodes[0].arcValues && x->nodes[0].arcDestinations) {
            x->size                 = PIZ_INIT_ORACLE_SIZE;
            x->peak                 = 1;
            x->index                = 1;
            x->shuttle              = 0;
            x->backwardThreshold    = PIZ_DEFAULT_BACKWARD_THRESHOLD;
            x->straightRatio        = PIZ_DEFAULT_STRAIGHT_RATIO;
            x->seed                 = (unsigned int)time(NULL);
            
            x->algorithm.type       = PIZ_ALGORITHM_TYPE_FACTOR_ORACLE;
            x->algorithm.add        = pizFactorOracleAdd;
            x->algorithm.clear      = pizFactorOracleClear;
            x->algorithm.proceed    = pizFactorOracleProceed;
            x->algorithm.count      = pizFactorOracleCount;
            
            x->nodes[0].referTo = -1;
            x->nodes[0].lengthRepeatedSuffix = 0;
            
        } else {
            pizArrayFree (x->nodes[0].arcValues);
            pizArrayFree (x->nodes[0].arcDestinations);
            
            x->nodes[0].arcValues       = NULL;
            x->nodes[0].arcDestinations = NULL;
            
            free (x->nodes);
            x->nodes = NULL;

            free (x); 
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

void pizFactorOracleFree (PIZFactorOracle *x)
{
    if (x) {
    //
    long i;
    
    for (i = 0; i < x->peak; i++) {
        pizArrayFree (x->nodes[i].arcValues);
        pizArrayFree (x->nodes[i].arcDestinations);
        
        x->nodes[i].arcValues       = NULL;
        x->nodes[i].arcDestinations = NULL;
    }
    
    free (x->nodes);
    x->nodes = NULL;
    
    free (x);
    //
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizFactorOracleAdd (PIZFactorOracle *x, long argc, long *argv)
{
    long i;
    PIZError err = PIZ_ERROR;
    
    if ((argc > 0) && argv) {
    //
    err = PIZ_GOOD;
    
    for (i = 0; i < argc; i++) {
    //
    long p = x->index;
    
    if (p == x->size && !err) {
        PIZFactorOracleNode *temp = x->nodes;
    
        if (temp = (PIZFactorOracleNode *)realloc (x->nodes, (x->size * 2) * sizeof(PIZFactorOracleNode))) {
            x->size *= 2;
            x->nodes = temp;
        } else {
            err |= PIZ_MEMORY;
        }
    }
    
    if (!err) {
        x->nodes[p].referTo = -1;
        x->nodes[p].lengthRepeatedSuffix =  0;
        
        if (p == x->peak) {
            if (x->nodes[p].arcValues = pizArrayNew (PIZ_INIT_ARRAY_SIZE)) {
                if (x->nodes[p].arcDestinations = pizArrayNew (PIZ_INIT_ARRAY_SIZE)) {
                    x->peak ++;
                } else {
                    free (x->nodes[p].arcValues);
                    err = PIZ_MEMORY;
                }
            } else {
                err = PIZ_MEMORY;
            }
        } else {
            pizArrayClear (x->nodes[p].arcValues);
            pizArrayClear (x->nodes[p].arcDestinations);
        }
    }
                
    if (!err) {   
        long t, j;
        long w = 0;
        long repeatedSuffix = 0;

        pizArrayAppend (x->nodes[(p - 1)].arcValues, argv[i]);
        pizArrayAppend (x->nodes[(p - 1)].arcDestinations, p);
        
        t = p - 1;
        j = x->nodes[(p - 1)].referTo;
        
        while ((j > -1) && !(pizArrayContainsValue (x->nodes[j].arcValues, argv[i]))) {
            if (!(pizArrayAppend (x->nodes[j].arcValues, argv[i]))) {
                if (pizArrayAppend (x->nodes[j].arcDestinations, p)) {
                    pizArrayRemoveLastValue (x->nodes[j].arcValues);
                    err = PIZ_MEMORY;
                }
            } else {
                err = PIZ_MEMORY;
            }
            
            t = j;
            j = x->nodes[j].referTo;
        }
        
        if (j != -1) {   
            long destinationIndex = pizArrayFirstIndexOfValue (x->nodes[j].arcValues, argv[i]);
            w = pizArrayValueAtIndex (x->nodes[j].arcDestinations, destinationIndex);
        }
            
        x->nodes[p].referTo = w;
        
        if (w != 0) {
            PIZFactorOracleNode *ptrA = x->nodes + t;
            PIZFactorOracleNode *ptrB = x->nodes + (w - 1);
            
            if ((w - 1) == ptrA->referTo) {
                repeatedSuffix = ptrA->lengthRepeatedSuffix + 1;
            } else {
                while (ptrB->referTo != ptrA->referTo) {
                    ptrB = x->nodes + ptrB->referTo;
                }
                
                repeatedSuffix = MIN (ptrA->lengthRepeatedSuffix, ptrB->lengthRepeatedSuffix) + 1;
            }
        }
            
        x->nodes[p].lengthRepeatedSuffix = repeatedSuffix;
            
        x->index ++;
    }
    //
    }
    //
    } 
        
    return err;
}

PIZError pizFactorOracleClear (PIZFactorOracle *x)
{
    x->index    = 1;
    x->shuttle  = 0;
    
    x->nodes[0].referTo = -1;
    x->nodes[0].lengthRepeatedSuffix = 0;
    
    pizArrayClear (x->nodes[0].arcValues);
    pizArrayClear (x->nodes[0].arcDestinations);
    
    return PIZ_GOOD;
}

PIZError pizFactorOracleProceed (PIZFactorOracle *x, long argc, long *argv)
{
    PIZError err = PIZ_ERROR;
    
    if (((argc > 0) && argv) && (x->index > 1)) {
    //
    long k = 0;
    err = PIZ_GOOD;

    while (argc) {
        long                t = false;
        double              h = rand_r (&x->seed) / (RAND_MAX + 1.0);
        PIZFactorOracleNode *p = NULL;
        
        if (x->shuttle == (x->index - 1)) {
            x->shuttle = 0;
        }
    
        p = x->nodes + x->shuttle;
        
        if (h > x->straightRatio) {
            if ((h > ((x->straightRatio + 1.) / 2.)) && (p->lengthRepeatedSuffix > x->backwardThreshold)) {
                x->shuttle = p->referTo;
                p = x->nodes + x->shuttle;
            } else if (pizArrayCount (p->arcValues) > 1) {
                long i = (pizArrayCount (p->arcValues) - 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
                argv[k]     = pizArrayValueAtIndex (p->arcValues, (i + 1));
                x->shuttle  = pizArrayValueAtIndex (p->arcDestinations, (i + 1));
                t = true;
            }
        }
        
        if (!t) {
            argv[k] = pizArrayValueAtIndex (p->arcValues, 0);
            x->shuttle ++;
        }
        
        argc --;
        k ++;
    }
    //    
    }
    
    return err;
}

long pizFactorOracleCount (const PIZFactorOracle *x)
{
    return (x->index - 1);
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
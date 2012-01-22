/*
 * \file    pizFactorOracle.c
 * \author  Jean Sapristi
 * \date    22 janvier 2012
 */
 
/*
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

#include "PIZFactorOracle.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_GROWING_ARRAY_INIT_SIZE     4
#define PIZ_FACTOR_ORACLE_INIT_SIZE     32

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZFactorOracle *pizFactorOracleNew (long argc, long *argv)
{
    PIZFactorOracle *x = NULL;

    if (x = (PIZFactorOracle *)malloc (sizeof(PIZFactorOracle)))
        {
            if (x->nodes = (PIZFactorOracleNode *)malloc 
                (PIZ_FACTOR_ORACLE_INIT_SIZE * sizeof(PIZFactorOracleNode)))
                {
                    x->nodes[0].arcValues = pizGrowingArrayNew (PIZ_GROWING_ARRAY_INIT_SIZE);
                    x->nodes[0].arcDestinations = pizGrowingArrayNew (PIZ_GROWING_ARRAY_INIT_SIZE);

                    
                    if (x->nodes[0].arcValues && x->nodes[0].arcDestinations)
                        {
                            srand ((unsigned int)time(NULL));
                            
                            x->size                 = PIZ_FACTOR_ORACLE_INIT_SIZE;
                            x->peak                 = 1;
                            x->index                = 1;
                            x->shuttle              = 0;
                            x->backwardThreshold    = PIZ_FACTOR_ORACLE_DEFAULT_BACKWARD_THRESHOLD;
                            x->straightRatio        = PIZ_FACTOR_ORACLE_DEFAULT_STRAIGHT_RATIO;
                    
                            x->nodes[0].referTo                 = -1;
                            x->nodes[0].lengthRepeatedSuffix    = 0;
                        }
                    else
                        {
                            pizGrowingArrayFree (x->nodes[0].arcValues);
                            pizGrowingArrayFree (x->nodes[0].arcDestinations);
                            
                            x->nodes[0].arcValues       = NULL;
                            x->nodes[0].arcDestinations = NULL;
                            
                            free (x->nodes);
                            x->nodes = NULL;
            
                            free (x);
                            x = NULL;
                        }
                }
            else
                {
                    free (x);
                    x = NULL;
                }
        }
        
    return x;
}

void pizFactorOracleFree (PIZFactorOracle *x)
{
    if (x)
        {
            long i;
            
            for (i = 0; i < x->peak; i++) {
                    pizGrowingArrayFree (x->nodes[i].arcValues);
                    pizGrowingArrayFree (x->nodes[i].arcDestinations);
                    
                    x->nodes[i].arcValues       = NULL;
                    x->nodes[i].arcDestinations = NULL;
                }
            
            free (x->nodes);
            x->nodes = NULL;
            
            free (x);
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizFactorOracleAdd (PIZFactorOracle *x, long argc, long *argv)
{
    long i;
    long err = PIZ_GOOD;
    
    if ((argc > 0) && argv) 
        {
            for (i = 0; i < argc; i++)
                {
                    long p = x->index;
                    
                    if (p == x->size && !err)
                        {
                            PIZFactorOracleNode *newFactorOracleNodes = x->nodes;
                        
                            if (newFactorOracleNodes = (PIZFactorOracleNode *)realloc 
                                (x->nodes, (x->size * 2) * sizeof(PIZFactorOracleNode)))
                                {
                                    x->size *= 2;
                                    x->nodes = newFactorOracleNodes;
                                }
                            else
                                {
                                    err = PIZ_MEMORY;
                                }
                        }
                    
                    if (!err)
                        {
                            x->nodes[p].referTo                 = -1;
                            x->nodes[p].lengthRepeatedSuffix    =  0;
                            
                            if (p == x->peak) {
                                if (x->nodes[p].arcValues = pizGrowingArrayNew (PIZ_GROWING_ARRAY_INIT_SIZE)) {
                                    if (x->nodes[p].arcDestinations = pizGrowingArrayNew 
                                        (PIZ_GROWING_ARRAY_INIT_SIZE)) {
                                            x->peak ++;
                                    } else {
                                        free (x->nodes[p].arcValues);
                                        err = PIZ_MEMORY;
                                    }
                                } else {
                                    err = PIZ_MEMORY;
                                }
                            } else {
                                pizGrowingArrayClear (x->nodes[p].arcValues);
                                pizGrowingArrayClear (x->nodes[p].arcDestinations);
                            }
                        }
                                
                    if (!err)
                        {   
                            long t, j;
                            long w = 0;
                            long repeatedSuffix = 0;
                    
                            pizGrowingArrayAppend (x->nodes[(p - 1)].arcValues, argv[i]);
                            pizGrowingArrayAppend (x->nodes[(p - 1)].arcDestinations, p);
                            
                            t = p - 1;
                            j = x->nodes[(p - 1)].referTo;
                            
                            while ((j > -1) && !pizGrowingArrayContainsValue (x->nodes[j].arcValues, argv[i]))
                                {
                                    if (!pizGrowingArrayAppend (x->nodes[j].arcValues, argv[i]))
                                        {
                                            if (pizGrowingArrayAppend (x->nodes[j].arcDestinations, p))
                                                {
                                                    pizGrowingArrayRemoveLastValue (x->nodes[j].arcValues);
                                                    
                                                    err = PIZ_MEMORY;
                                                }
                                        }
                                    else
                                        {
                                            err = PIZ_MEMORY;
                                        }
                                    
                                    t = j;
                                    j = x->nodes[j].referTo;
                                }
                            
                            if (j != -1)
                                {   
                                    long destinationIndex = 
                                        pizGrowingArrayFirstIndexOfValue (x->nodes[j].arcValues, argv[i]);
                                        
                                    w = pizGrowingArrayValueAtIndex 
                                        (x->nodes[j].arcDestinations, destinationIndex);
                                }
                                
                            x->nodes[p].referTo = w;
                            
                            if (w != 0)
                                {
                                    PIZFactorOracleNode *ptrA = x->nodes + t;
                                    PIZFactorOracleNode *ptrB = x->nodes + (w - 1);
                                    
                                    if ((w - 1) == ptrA->referTo)
                                        {
                                            repeatedSuffix = ptrA->lengthRepeatedSuffix + 1;
                                        }
                                    else
                                        {
                                            while (ptrB->referTo != ptrA->referTo) {
                                                ptrB = x->nodes + ptrB->referTo;
                                            }
                                            
                                            repeatedSuffix = MIN (ptrA->lengthRepeatedSuffix, 
                                                ptrB->lengthRepeatedSuffix) + 1;
                                        }
                                }
                                
                            x->nodes[p].lengthRepeatedSuffix = repeatedSuffix;
                                
                            x->index ++;
                        }
                }
        }
    else
        {
            err = PIZ_ERROR;
        }
        
    return err;
}

void pizFactorOracleClear (PIZFactorOracle *x)
{
    x->index    = 1;
    x->shuttle  = 0;
    
    x->nodes[0].referTo                 = -1;
    x->nodes[0].lengthRepeatedSuffix    = 0;
    
    pizGrowingArrayClear (x->nodes[0].arcValues);
    pizGrowingArrayClear (x->nodes[0].arcDestinations);
}

long pizFactorOracleCount (PIZFactorOracle *x)
{
    return (x->index - 1);
}

PIZError pizFactorOracleProceed (PIZFactorOracle *x, long argc, long *argv)
{
    long err = PIZ_ERROR;
    
    if (((argc > 0) && argv) && (x->index > 1))
        {
            long k = 0;
            
            err = PIZ_GOOD;

            while (argc)
                {
                    long                t = false;
                    PIZFactorOracleNode *p = NULL;
                    double              h = rand ( ) / (RAND_MAX + 1.0);
                    
                    if (x->shuttle == (x->index - 1)) {
                            x->shuttle = 0;
                        }
                
                    p = x->nodes + x->shuttle;
                    
                    if (h > x->straightRatio)
                        {
                            if ((h > ((x->straightRatio + 1.) / 2.)) &&
                                (p->lengthRepeatedSuffix > x->backwardThreshold))
                                {
                                    x->shuttle = p->referTo;
                                    p = x->nodes + x->shuttle;
                                }
                            else if (pizGrowingArrayCount (p->arcValues) > 1)
                                {
                                    long i = (long)((pizGrowingArrayCount (p->arcValues) - 1) * 
                                                (rand ( ) / (RAND_MAX + 1.0)));

                                    argv[k]     = pizGrowingArrayValueAtIndex (p->arcValues, (i + 1));
                                    x->shuttle  = pizGrowingArrayValueAtIndex (p->arcDestinations, (i + 1));
                                    
                                    t = true;
                                }
                        }
                    
                    if (!t) {
                            argv[k] = pizGrowingArrayValueAtIndex (p->arcValues, 0);
                            x->shuttle ++;
                        }
                    
                    argc --;
                    k ++;
                }
        }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
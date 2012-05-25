/*
 * \file    pizFactorOracle.c
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

#include "pizFactorOracle.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_INIT_SIZE_ARRAY     4
#define PIZ_INIT_SIZE_ORACLE    32
#define PIZ_DEFAULT_BACKWARD    2
#define PIZ_DEFAULT_STRAIGHT    0.25

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZFactorOracle *pizFactorOracleNew (long argc, long *argv)
{
    PIZFactorOracle *x = NULL;

    if (x = (PIZFactorOracle *)malloc (sizeof(PIZFactorOracle))) {
    //
    if (x->nodes = (PIZFactorOracleNode *)malloc (PIZ_INIT_SIZE_ORACLE * sizeof(PIZFactorOracleNode))) {
        x->nodes[0].values = pizArrayNew (PIZ_INIT_SIZE_ARRAY);
        x->nodes[0].destinations = pizArrayNew (PIZ_INIT_SIZE_ARRAY);
        
        if (x->nodes[0].values && x->nodes[0].destinations) {
            x->size              = PIZ_INIT_SIZE_ORACLE;
            x->peak              = 1;
            x->index             = 1;
            x->shuttle           = 0;
            x->backwardThreshold = PIZ_DEFAULT_BACKWARD;
            x->straightRatio     = PIZ_DEFAULT_STRAIGHT;
            x->seed              = (unsigned int)time(NULL);
            
            x->nodes[0].refer = -1;
            x->nodes[0].lrs   = 0;
            
        } else {
            pizArrayFree (x->nodes[0].values);
            pizArrayFree (x->nodes[0].destinations);
            
            x->nodes[0].values       = NULL;
            x->nodes[0].destinations = NULL;
            
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
        pizArrayFree (x->nodes[i].values);
        pizArrayFree (x->nodes[i].destinations);
        
        x->nodes[i].values       = NULL;
        x->nodes[i].destinations = NULL;
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
        x->nodes[p].refer = -1;
        x->nodes[p].lrs   =  0;
        
        if (p == x->peak) {
            if (x->nodes[p].values = pizArrayNew (PIZ_INIT_SIZE_ARRAY)) {
                if (x->nodes[p].destinations = pizArrayNew (PIZ_INIT_SIZE_ARRAY)) {
                    x->peak ++;
                } else {
                    free (x->nodes[p].values);
                    err = PIZ_MEMORY;
                }
            } else {
                err = PIZ_MEMORY;
            }
        } else {
            pizArrayClear (x->nodes[p].values);
            pizArrayClear (x->nodes[p].destinations);
        }
    }
                
    if (!err) {   
        long t, j;
        long w = 0;
        long repeatedSuffix = 0;

        pizArrayAppend (x->nodes[(p - 1)].values, argv[i]);
        pizArrayAppend (x->nodes[(p - 1)].destinations, p);
        
        t = p - 1;
        j = x->nodes[(p - 1)].refer;
        
        while ((j > -1) && !(pizArrayContainsValue (x->nodes[j].values, argv[i]))) {
            if (!(pizArrayAppend (x->nodes[j].values, argv[i]))) {
                if (pizArrayAppend (x->nodes[j].destinations, p)) {
                    pizArrayRemoveLast (x->nodes[j].values);
                    err = PIZ_MEMORY;
                }
            } else {
                err = PIZ_MEMORY;
            }
            
            t = j;
            j = x->nodes[j].refer;
        }
        
        if (j != -1) {   
            long destinationIndex = pizArrayIndexOfValue (x->nodes[j].values, argv[i]);
            w = pizArrayAtIndex (x->nodes[j].destinations, destinationIndex);
        }
            
        x->nodes[p].refer = w;
        
        if (w != 0) {
            PIZFactorOracleNode *ptrA = x->nodes + t;
            PIZFactorOracleNode *ptrB = x->nodes + (w - 1);
            
            if ((w - 1) == ptrA->refer) {
                repeatedSuffix = ptrA->lrs + 1;
            } else {
                while (ptrB->refer != ptrA->refer) {
                    ptrB = x->nodes + ptrB->refer;
                }
                
                repeatedSuffix = MIN (ptrA->lrs, ptrB->lrs) + 1;
            }
        }
            
        x->nodes[p].lrs = repeatedSuffix;
            
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
    x->index   = 1;
    x->shuttle = 0;
    
    x->nodes[0].refer = -1;
    x->nodes[0].lrs   = 0;
    
    pizArrayClear (x->nodes[0].values);
    pizArrayClear (x->nodes[0].destinations);
    
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
            if ((h > ((x->straightRatio + 1.) / 2.)) && (p->lrs > x->backwardThreshold)) {
                x->shuttle = p->refer;
                p = x->nodes + x->shuttle;
            } else if (pizArrayCount (p->values) > 1) {
                long i = (pizArrayCount (p->values) - 1) * (rand_r (&x->seed) / (RAND_MAX + 1.0));
                argv[k]     = pizArrayAtIndex (p->values, (i + 1));
                x->shuttle  = pizArrayAtIndex (p->destinations, (i + 1));
                t = true;
            }
        }
        
        if (!t) {
            argv[k] = pizArrayAtIndex (p->values, 0);
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
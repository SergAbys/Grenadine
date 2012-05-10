/*
 * \file    pizFiniteState.c
 * \author  Jean Sapristi
 * \date    April 23, 2012.
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

#include "pizFiniteState.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ALPHABET_SIZE       128
#define PIZ_MAXIMUM_THRESHOLD   100
#define PIZ_INCREMENT_JUMP      1   
#define PIZ_INCREMENT_FINAL     5 

#define PIZ_INIT_QUEUE_SIZE     4
#define PIZ_DEFAULT_THRESHOLD   35

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE PIZError pizFiniteStateMergeNodes (PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZFiniteState *pizFiniteStateNew (long argc, long *argv)
{
    PIZFiniteState *x = NULL;

    if (x = (PIZFiniteState *)malloc (sizeof(PIZFiniteState))) {
        long     i;
        PIZError err = PIZ_GOOD;
        
        if (x->stock = (PIZFiniteStateNode *)malloc (PIZ_ITEMSET128_SIZE * sizeof(PIZFiniteStateNode))) {
            x->count        = 0;
            x->shuttle      = -1;
            x->jumpChance   = 0;
            x->threshold    = PIZ_DEFAULT_THRESHOLD;
            
            x->algorithm.add        = pizFiniteStateAdd;
            x->algorithm.clear      = pizFiniteStateClear;
            x->algorithm.proceed    = pizFiniteStateProceed;
            x->algorithm.count      = pizFiniteStateCount;
            
            x->seed = (unsigned int)time(NULL);
                
            if (argc && ((argv[0] > 0) && (argv[0] <= PIZ_MAXIMUM_THRESHOLD))) {
                x->threshold = argv[0];
            }
            
            if (x->ticketMachine = pizBoundedStackNew (PIZ_ITEMSET128_SIZE)) {
                for (i = (PIZ_ITEMSET128_SIZE - 1); i >= 0; i--) {
                     pizBoundedStackPush (x->ticketMachine, i);
                 }
            } else {
                err = PIZ_MEMORY;
            }
            
            if (x->lottery = (long *)malloc ((MAX (PIZ_ALPHABET_SIZE, PIZ_ITEMSET128_SIZE)) * sizeof(long))) {
                x->lotteryIndex = 0;
            } else {
                err = PIZ_MEMORY;
            }
                
            if (x->mapByValue = (PIZBoundedQueue **)malloc (PIZ_ALPHABET_SIZE * sizeof(PIZBoundedQueue *))) {
                for (i = 0; i < PIZ_ALPHABET_SIZE; i++) {
                    if (!(x->mapByValue[i] = pizBoundedQueueNew (PIZ_INIT_QUEUE_SIZE))) {
                        err = PIZ_MEMORY;
                    }
                }
            } else {
                err = PIZ_MEMORY;
            }
            
            if (err)  {
                pizFiniteStateFree (x);
                x = NULL;
            }
                
        } else {
            free (x);
            x = NULL;
        }
    }
    
    return x;
}

void pizFiniteStateFree (PIZFiniteState *x)
{
    if (x) {
        if (x->mapByValue) {
            long i;

            for (i = 0; i < PIZ_ALPHABET_SIZE; i++) {
                pizBoundedQueueFree (x->mapByValue[i]);
                x->mapByValue[i] = NULL;
            }
            
            free (x->mapByValue);
            x->mapByValue = NULL;
        }

        pizBoundedStackFree (x->ticketMachine);
        x->ticketMachine = NULL;
        
        free (x->lottery);
        
        free (x->stock);
        x->stock = NULL;
        
        free (x);
    }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizFiniteStateAdd (PIZFiniteState *x, long argc, long *argv)
{
    PIZError err1 = PIZ_ERROR;
    PIZError err2 = PIZ_GOOD;
    
    if (argv && argc > 0) {
    //
    err1 = PIZ_GOOD;            

    if (!(pizBoundedStackPop (x->ticketMachine))) {
        long i, nextNode, firstNode = pizBoundedStackPoppedValue (x->ticketMachine);
        long k = CLAMP (argv[0], 0, (PIZ_ALPHABET_SIZE - 1));
        
        if (!(pizBoundedQueueAppend (x->mapByValue[k], firstNode))) {
            x->count ++;
            
            x->stock[firstNode].value = k;
            x->stock[firstNode].final = true;
            pizItemset128Clear (&(x->stock[firstNode].parents));
            pizItemset128Clear (&(x->stock[firstNode].childs));
            
            for (i = 1; i < argc; i++) {
                if (!(pizBoundedStackPop (x->ticketMachine))) {
                
                    nextNode = pizBoundedStackPoppedValue (x->ticketMachine);
                    k = CLAMP (argv[i], 0, (PIZ_ALPHABET_SIZE - 1));
                            
                    if (!(pizBoundedQueueAppend (x->mapByValue[k], nextNode))) {
                        x->count ++;
                        
                        x->stock[nextNode].value = k;
                        x->stock[nextNode].final = true;
                        
                        pizItemset128Clear (&(x->stock[nextNode].parents));
                        pizItemset128Clear (&(x->stock[nextNode].childs));
                        
                        pizItemset128SetAtIndex (&(x->stock[nextNode].parents), firstNode);
                        
                        x->stock[firstNode].final = false;
                        
                        pizItemset128SetAtIndex (&(x->stock[firstNode].childs), nextNode);
                        
                        firstNode = nextNode;
                        
                    } else {
                        pizBoundedStackPush (x->ticketMachine, nextNode);
                        err1 = PIZ_MEMORY;
                    }
                }
            }
        } else {
            pizBoundedStackPush (x->ticketMachine, firstNode);
            err1 = PIZ_MEMORY;
        }
    }
    
    while ((x->count > x->threshold) && !err2) {
        err2 = pizFiniteStateMergeNodes (x);
    }
    //   
    }
    
    return err1;
}

PIZError pizFiniteStateClear (PIZFiniteState *x)
{
    long i;
    
    for (i = 0; i < PIZ_ALPHABET_SIZE; i++) {
        pizBoundedQueueClear (x->mapByValue[i]);
    }
        
    x->count        = 0;
    x->shuttle      = -1;
    x->jumpChance   = 0;
    
    pizBoundedStackClear (x->ticketMachine);
    
    for (i = (PIZ_ITEMSET128_SIZE - 1); i >= 0; i--) {
        pizBoundedStackPush (x->ticketMachine, i);
    }
    
    return PIZ_GOOD;
}

PIZError pizFiniteStateProceed (PIZFiniteState *x, long argc, long *argv)
{
    PIZError err = PIZ_ERROR;
    
    if (((argc > 0) && argv) && (x->count > 0)) {
    //
    long k = 0;
    err = PIZ_GOOD;
    
    while (argc) {
        long i;
        bool jump = false;
        
        if (x->shuttle == -1) {
            long a, j, h ;
            
            x->lotteryIndex = 0;

            for (j = 0; j < PIZ_ALPHABET_SIZE; j++) {
                if (pizBoundedQueueCount (x->mapByValue[j])) {
                    x->lottery[x->lotteryIndex] = j;
                    x->lotteryIndex ++;
                }
            }
            
            h = (long)(x->lotteryIndex * (rand_r (&x->seed) / (RAND_MAX + 1.0)));

            pizBoundedQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizBoundedQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            x->shuttle = a;
            pizBoundedQueueAppend ((x->mapByValue[x->lottery[h]]), a);
        }
        
        argv[k] = x->stock[x->shuttle].value;
        
        argc --;
        k ++;
        
        x->lotteryIndex = 0;
        
        jump = ((100 * (rand_r (&x->seed) / (RAND_MAX + 1.0))) < x->jumpChance);
        
        if (!jump) {
            for (i = 0; i < PIZ_ITEMSET128_SIZE; i++) {
                if (pizItemset128IsSetAtIndex (&(x->stock[x->shuttle].childs), i)) {
                    x->lottery[x->lotteryIndex] = i;
                    x->lotteryIndex ++;
                }
            }
        }
        
        if (x->lotteryIndex) {
            long h = (long)(x->lotteryIndex * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
            x->shuttle = x->lottery[h];
            
            if (x->stock[x->shuttle].final) {
                x->jumpChance += PIZ_INCREMENT_FINAL;
            } else {
                x->jumpChance += PIZ_INCREMENT_JUMP;
            }
        } else {
            x->shuttle      = -1;
            x->jumpChance   = 0;
        }
    }
    //    
    }
    
    return err;
}

long pizFiniteStateCount (const PIZFiniteState *x)
{
    return (x->count);
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_INLINE PIZError pizFiniteStateMergeNodes (PIZFiniteState *x)
{
    long i;
    PIZError err = PIZ_ERROR;
    
    x->lotteryIndex = 0;
    
    for (i = 0; i < PIZ_ALPHABET_SIZE; i++) {
        if (pizBoundedQueueCount (x->mapByValue[i])) {
            long j;
            for (j = 0; j < pizBoundedQueueCount (x->mapByValue[i]); j++) {
                x->lottery[x->lotteryIndex] = i;
                x->lotteryIndex ++;
            }
        }
    }
            
    if (x->lotteryIndex) {
        long h = (long)(x->lotteryIndex * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        
        if (pizBoundedQueueCount (x->mapByValue[x->lottery[h]]) == 1) {
            long j, a;
            
            x->count --;
            
            pizBoundedQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizBoundedQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            
            for (j = 0; j < PIZ_ITEMSET128_SIZE; j++) {
                if (pizItemset128IsSetAtIndex (&(x->stock[a].parents), j)) {
                    pizItemset128UnsetAtIndex (&(x->stock[j].childs), a);
                }
                
                if (pizItemset128IsSetAtIndex (&(x->stock[a].childs), j)) {
                    pizItemset128UnsetAtIndex (&(x->stock[j].parents), a);
                }
            }
            
            pizBoundedStackPush (x->ticketMachine, a);
            
            if (x->shuttle == a) {
                x->shuttle = -1;
            }
            
            err = PIZ_GOOD;
            
        } else {
            long            j, a, b;
            PIZItemset128   tempChilds;
            PIZItemset128   tempParents;
            
            x->count --;
            
            pizBoundedQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizBoundedQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            pizBoundedQueuePopLastValue (x->mapByValue[x->lottery[h]]);
            b = pizBoundedQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            
            pizItemset128Union (&(x->stock[a].childs), &(x->stock[b].childs), &tempChilds);
            pizItemset128Union (&(x->stock[a].parents), &(x->stock[b].parents), &tempParents);
            
            x->stock[a].final   = x->stock[a].final || x->stock[b].final;
            
            x->stock[a].childs  = tempChilds;
            x->stock[a].parents = tempParents;
            
            for (j = 0; j < PIZ_ITEMSET128_SIZE; j++) {
                if (pizItemset128IsSetAtIndex (&(x->stock[b].parents), j)) {
                    pizItemset128UnsetAtIndex (&(x->stock[j].childs), b);
                    pizItemset128SetAtIndex (&(x->stock[j].childs), a);
                }
                
                if (pizItemset128IsSetAtIndex (&(x->stock[b].childs), j)) {
                    pizItemset128UnsetAtIndex (&(x->stock[j].parents), b);
                    pizItemset128SetAtIndex (&(x->stock[j].parents), a);
                }
            }
            
            pizItemset128UnsetAtIndex (&(x->stock[a].parents), a);
            pizItemset128UnsetAtIndex (&(x->stock[a].childs), a);
            
            pizBoundedQueueAppend (x->mapByValue[x->lottery[h]], a);
            
            pizBoundedStackPush (x->ticketMachine, b);
            
            if (x->shuttle == b) {
                x->shuttle = a;
            }
                
            err = PIZ_GOOD;
        }
    }
    
    return err; 
}                               

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
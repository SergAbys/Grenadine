/*
 * \file    pizFiniteState.c
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

#include "pizFiniteState.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <time.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

#define PIZ_SIZE_ALPHABET       128
#define PIZ_MAXIMUM_THRESHOLD   100
#define PIZ_INCREMENT_JUMP      1   
#define PIZ_INCREMENT_FINAL     5 

#define PIZ_INIT_SIZE_QUEUE     4
#define PIZ_DEFAULT_THRESHOLD   35

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -

PIZ_INLINE PIZError pizFiniteStateMergeNodes (PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZFiniteState *pizFiniteStateNew (long argc, long *argv)
{
    PIZFiniteState *x = NULL;

    if (x = (PIZFiniteState *)malloc (sizeof(PIZFiniteState))) {
        long     i;
        PIZError err = PIZ_GOOD;
        
        if (x->stock = (PIZFiniteStateNode *)malloc (PIZ_ITEMSET_SIZE * sizeof(PIZFiniteStateNode))) {
            x->count       = 0;
            x->shuttle     = -1;
            x->jumpChance  = 0;
            x->threshold   = PIZ_DEFAULT_THRESHOLD;
            
            x->seed = (unsigned int)time(NULL);
                
            if (argc && ((argv[0] > 0) && (argv[0] <= PIZ_MAXIMUM_THRESHOLD))) {
                x->threshold = argv[0];
            }
            
            if (x->ticketMachine = pizStackNew (PIZ_ITEMSET_SIZE)) {
                for (i = (PIZ_ITEMSET_SIZE - 1); i >= 0; i--) {
                     pizStackPush (x->ticketMachine, i);
                 }
            } else {
                err = PIZ_MEMORY;
            }
            
            if (x->lottery = (long *)malloc ((MAX (PIZ_SIZE_ALPHABET, PIZ_ITEMSET_SIZE)) * sizeof(long))) {
                x->lotteryIndex = 0;
            } else {
                err = PIZ_MEMORY;
            }
                
            if (x->mapByValue = (PIZQueue **)malloc (PIZ_SIZE_ALPHABET * sizeof(PIZQueue *))) {
                for (i = 0; i < PIZ_SIZE_ALPHABET; i++) {
                    if (!(x->mapByValue[i] = pizQueueNew (PIZ_INIT_SIZE_QUEUE))) {
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

            for (i = 0; i < PIZ_SIZE_ALPHABET; i++) {
                pizQueueFree (x->mapByValue[i]);
                x->mapByValue[i] = NULL;
            }
            
            free (x->mapByValue);
            x->mapByValue = NULL;
        }

        pizStackFree (x->ticketMachine);
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

    if (!(pizStackPop (x->ticketMachine))) {
        long i, nextNode, firstNode = pizStackPoppedValue (x->ticketMachine);
        long k = CLAMP (argv[0], 0, (PIZ_SIZE_ALPHABET - 1));
        
        if (!(pizQueueAppend (x->mapByValue[k], firstNode))) {
            x->count ++;
            
            x->stock[firstNode].value = k;
            x->stock[firstNode].final = true;
            pizItemsetClear (&(x->stock[firstNode].parents));
            pizItemsetClear (&(x->stock[firstNode].childs));
            
            for (i = 1; i < argc; i++) {
                if (!(pizStackPop (x->ticketMachine))) {
                
                    nextNode = pizStackPoppedValue (x->ticketMachine);
                    k = CLAMP (argv[i], 0, (PIZ_SIZE_ALPHABET - 1));
                            
                    if (!(pizQueueAppend (x->mapByValue[k], nextNode))) {
                        x->count ++;
                        
                        x->stock[nextNode].value = k;
                        x->stock[nextNode].final = true;
                        
                        pizItemsetClear (&(x->stock[nextNode].parents));
                        pizItemsetClear (&(x->stock[nextNode].childs));
                        
                        pizItemsetSetAtIndex (&(x->stock[nextNode].parents), firstNode);
                        
                        x->stock[firstNode].final = false;
                        
                        pizItemsetSetAtIndex (&(x->stock[firstNode].childs), nextNode);
                        
                        firstNode = nextNode;
                        
                    } else {
                        pizStackPush (x->ticketMachine, nextNode);
                        err1 = PIZ_MEMORY;
                    }
                }
            }
        } else {
            pizStackPush (x->ticketMachine, firstNode);
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
    
    for (i = 0; i < PIZ_SIZE_ALPHABET; i++) {
        pizQueueClear (x->mapByValue[i]);
    }
        
    x->count      = 0;
    x->shuttle    = -1;
    x->jumpChance = 0;
    
    pizStackClear (x->ticketMachine);
    
    for (i = (PIZ_ITEMSET_SIZE - 1); i >= 0; i--) {
        pizStackPush (x->ticketMachine, i);
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

            for (j = 0; j < PIZ_SIZE_ALPHABET; j++) {
                if (pizQueueCount (x->mapByValue[j])) {
                    x->lottery[x->lotteryIndex] = j;
                    x->lotteryIndex ++;
                }
            }
            
            h = (long)(x->lotteryIndex * (rand_r (&x->seed) / (RAND_MAX + 1.0)));

            pizQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            x->shuttle = a;
            pizQueueAppend ((x->mapByValue[x->lottery[h]]), a);
        }
        
        argv[k] = x->stock[x->shuttle].value;
        
        argc --;
        k ++;
        
        x->lotteryIndex = 0;
        
        jump = ((100 * (rand_r (&x->seed) / (RAND_MAX + 1.0))) < x->jumpChance);
        
        if (!jump) {
            for (i = 0; i < PIZ_ITEMSET_SIZE; i++) {
                if (pizItemsetIsSetAtIndex (&(x->stock[x->shuttle].childs), i)) {
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
    return x->count;
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark ---
#pragma mark -

PIZ_INLINE PIZError pizFiniteStateMergeNodes (PIZFiniteState *x)
{
    long i;
    PIZError err = PIZ_ERROR;
    
    x->lotteryIndex = 0;
    
    for (i = 0; i < PIZ_SIZE_ALPHABET; i++) {
        if (pizQueueCount (x->mapByValue[i])) {
            long j;
            for (j = 0; j < pizQueueCount (x->mapByValue[i]); j++) {
                x->lottery[x->lotteryIndex] = i;
                x->lotteryIndex ++;
            }
        }
    }
            
    if (x->lotteryIndex) {
        long h = (long)(x->lotteryIndex * (rand_r (&x->seed) / (RAND_MAX + 1.0)));
        
        if (pizQueueCount (x->mapByValue[x->lottery[h]]) == 1) {
            long j, a;
            
            x->count --;
            
            pizQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            
            for (j = 0; j < PIZ_ITEMSET_SIZE; j++) {
                if (pizItemsetIsSetAtIndex (&(x->stock[a].parents), j)) {
                    pizItemsetUnsetAtIndex (&(x->stock[j].childs), a);
                }
                
                if (pizItemsetIsSetAtIndex (&(x->stock[a].childs), j)) {
                    pizItemsetUnsetAtIndex (&(x->stock[j].parents), a);
                }
            }
            
            pizStackPush (x->ticketMachine, a);
            
            if (x->shuttle == a) {
                x->shuttle = -1;
            }
            
            err = PIZ_GOOD;
            
        } else {
            long        j, a, b;
            PIZItemset  tempChilds;
            PIZItemset  tempParents;
            
            x->count --;
            
            pizQueuePop (x->mapByValue[x->lottery[h]]);
            a = pizQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            pizQueuePopLast (x->mapByValue[x->lottery[h]]);
            b = pizQueuePoppedValue (x->mapByValue[x->lottery[h]]);
            
            pizItemsetUnion (&(x->stock[a].childs), &(x->stock[b].childs), &tempChilds);
            pizItemsetUnion (&(x->stock[a].parents), &(x->stock[b].parents), &tempParents);
            
            x->stock[a].final   = x->stock[a].final || x->stock[b].final;
            
            x->stock[a].childs  = tempChilds;
            x->stock[a].parents = tempParents;
            
            for (j = 0; j < PIZ_ITEMSET_SIZE; j++) {
                if (pizItemsetIsSetAtIndex (&(x->stock[b].parents), j)) {
                    pizItemsetUnsetAtIndex (&(x->stock[j].childs), b);
                    pizItemsetSetAtIndex   (&(x->stock[j].childs), a);
                }
                
                if (pizItemsetIsSetAtIndex (&(x->stock[b].childs), j)) {
                    pizItemsetUnsetAtIndex (&(x->stock[j].parents), b);
                    pizItemsetSetAtIndex   (&(x->stock[j].parents), a);
                }
            }
            
            pizItemsetUnsetAtIndex (&(x->stock[a].parents), a);
            pizItemsetUnsetAtIndex (&(x->stock[a].childs), a);
            
            pizQueueAppend (x->mapByValue[x->lottery[h]], a);
            
            pizStackPush (x->ticketMachine, b);
            
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
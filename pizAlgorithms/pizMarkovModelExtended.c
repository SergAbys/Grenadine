/*
 * \file    pizMarkovModelExtended.c
 * \author  Jean Sapristi
 * \date    22 janvier 2012
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

#include "pizMarkovModel.h"
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ALPHABET_SIZE           128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void pizMarkovModelSetPersistence (PIZMarkovModel *x, double f)
{
    if (f >= 0.) {
            x->persistence = f;
        }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizMarkovModelEncodeNodeToArray (PIZMarkovModel *x, long n, PIZGrowingArray *array)
{
    long err = PIZ_ERROR;
    
    if ((n >= 0) && (n < x->graphSize) && array)
        {
            long i;
            
            err = PIZ_GOOD;
            
            err |= pizGrowingArrayAppend (array, (long)(x->start[n] * 100.));
            err |= pizGrowingArrayAppend (array, x->graphSize);
            err |= pizGrowingArrayAppend (array, PIZ_ALPHABET_SIZE);
            
            for (i = 0; i < x->graphSize; i++) {
                err |= pizGrowingArrayAppend (array, (long)(x->transition[(n * x->graphSize) + i] * 100.));
            }
            
            for (i = 0; i < PIZ_ALPHABET_SIZE; i++) {
                err |= pizGrowingArrayAppend (array, (long)(x->emission[(n * PIZ_ALPHABET_SIZE) + i] * 100.));
            }
        }
    
    return err;
}

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
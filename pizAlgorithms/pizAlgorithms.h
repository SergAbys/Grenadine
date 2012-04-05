/**
 * \file    pizAlgorithms.h
 * \author  Jean Sapristi
 * \date    April 5, 2012.
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

#ifndef PIZ_ALGORITHMS_H
#define PIZ_ALGORITHMS_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ALGORITHM_FLAG_NONE              0UL
#define PIZ_ALGORITHM_FLAG_FACTOR_ORACLE     1UL
#define PIZ_ALGORITHM_FLAG_GALOIS_LATTICE    2UL
#define PIZ_ALGORITHM_FLAG_FINITE_STATE      4UL
#define PIZ_ALGORITHM_FLAG_KOHONEN_MAP       8UL
#define PIZ_ALGORITHM_FLAG_NEURAL_GAS        16UL
#define PIZ_ALGORITHM_FLAG_MARKOV_MODEL      32UL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef unsigned long PIZAlgorithmType;
typedef long (*PIZAlgorithmMethod)( );

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZAlgorithm {
    PIZAlgorithmType   type;
    PIZAlgorithmMethod addMethod;
    PIZAlgorithmMethod clearMethod;
    PIZAlgorithmMethod proceedMethod;
    PIZAlgorithmMethod countMethod;
    } PIZAlgorithm;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif  // PIZ_ALGORITHMS_H
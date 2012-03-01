/**
 * \file    pizAlgorithmsMaxMSP.h
 * \author  Jean Sapristi
 * \date    February 28, 2012.
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

#ifndef PIZ_ALGORITHMS_MAXMSP_H
#define PIZ_ALGORITHMS_MAXMSP_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizFactorOracle.h"
#include "pizFiniteState.h"
#include "pizGaloisLattice.h"
#include "pizKohonenMap.h"
#include "pizNeuralGas.h"
#include "pizMarkovModel.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_FACTOR_ORACLE_REFER         0
#define PIZ_FACTOR_ORACLE_LRS           1
#define PIZ_FACTOR_ORACLE_ARCS          2
#define PIZ_FACTOR_ORACLE_DATA          3

#define PIZ_GALOIS_LATTICE_CONCEPTS     0
#define PIZ_GALOIS_LATTICE_DATA         1

#define PIZ_MARKOV_MODEL_START          0
#define PIZ_MARKOV_MODEL_TRANSITIONS    1
#define PIZ_MARKOV_MODEL_EMISSIONS      2
#define PIZ_MARKOV_MODEL_DATA           3

#define PIZ_ALPHABET_SIZE               128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizFactorOracleSetBackwardThreshold     (PIZFactorOracle *x, long n);
void        pizFactorOracleSetStraightRatio         (PIZFactorOracle *x, double f);
long        pizFactorOracleBackwardThreshold        (const PIZFactorOracle *x);
double      pizFactorOracleStraightRatio            (const PIZFactorOracle *x);
PIZError    pizFactorOracleEncodeToArray            (const PIZFactorOracle *x, long node, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError    pizGaloisLatticeEncodeToArray           (const PIZGaloisLattice *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizKohonenMapSetRange                   (PIZKohonenMap *x, long n);
void        pizKohonenMapSetTraining                (PIZKohonenMap *x, long n);
void        pizKohonenMapSetStep                    (PIZKohonenMap *x, double f);
long        pizKohonenMapRange                      (const PIZKohonenMap *x);
long        pizKohonenMapTraining                   (const PIZKohonenMap *x);
double      pizKohonenMapStep                       (const PIZKohonenMap *x);
PIZError    pizKohonenMapEncodeToArray              (const PIZKohonenMap *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizNeuralGasSetLambda                   (PIZNeuralGas *x, long n);
void        pizNeuralGasSetEpsilon1                 (PIZNeuralGas *x, double f);
void        pizNeuralGasSetEpsilon2                 (PIZNeuralGas *x, double f);
void        pizNeuralGasSetAlpha                    (PIZNeuralGas *x, double f);
void        pizNeuralGasSetBeta                     (PIZNeuralGas *x, double f);
void        pizNeuralGasSetKappa                    (PIZNeuralGas *x, double f);
long        pizNeuralGasLambda                      (const PIZNeuralGas *x);
double      pizNeuralGasEpsilon1                    (const PIZNeuralGas *x);
double      pizNeuralGasEpsilon2                    (const PIZNeuralGas *x);
double      pizNeuralGasAlpha                       (const PIZNeuralGas *x);
double      pizNeuralGasBeta                        (const PIZNeuralGas *x);
double      pizNeuralGasKappa                       (const PIZNeuralGas *x);
PIZError    pizNeuralGasEncodeToArray               (const PIZNeuralGas *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizMarkovModelSetPersistence            (PIZMarkovModel *x, double f);
PIZError    pizMarkovModelEncodeToArray             (const PIZMarkovModel *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_ALGORITHMS_MAXMSP_H 
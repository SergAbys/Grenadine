/**
 * \file    pizMaxMSP.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup maxMSP     
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

#ifndef PIZ_MAXMSP_H
#define PIZ_MAXMSP_H

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

/**
 * \def     PIZ_FACTOR_ORACLE_REFER 
 * \brief   Index of referTo in \c pizFactorOracleEncodeNodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_LRS 
 * \brief   Index of lengthRepeatedSuffix in \c pizFactorOracleEncodeNodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_ARCS
 * \brief   Index of number of arcs in \c pizFactorOracleEncodeNodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_DATA
 * \brief   Index of data in \c pizFactorOracleEncodeNodeToArray().
 */
 
#define PIZ_FACTOR_ORACLE_REFER     0
#define PIZ_FACTOR_ORACLE_LRS       1
#define PIZ_FACTOR_ORACLE_ARCS      2
#define PIZ_FACTOR_ORACLE_DATA      3

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_GALOIS_LATTICE_CONCEPTS 
 * \brief   Index of number of concepts in \c pizGaloisLatticeEncodeConceptsToArray().
 */

/**
 * \def     PIZ_GALOIS_LATTICE_DATA 
 * \brief   Index of data in \c pizGaloisLatticeEncodeConceptsToArray().
 */
 
#define PIZ_GALOIS_LATTICE_CONCEPTS     0
#define PIZ_GALOIS_LATTICE_DATA         1

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_MARKOV_MODEL_START 
 * \brief   Index of node's start probability in \c pizMarkovModelEncodeNodeToArray().
 */

/**
 * \def     PIZ_MARKOV_MODEL_TRANSITIONS 
 * \brief   Index of node's number of transitions in \c pizMarkovModelEncodeNodeToArray().
 */
 
/**
 * \def     PIZ_MARKOV_MODEL_EMISSIONS 
 * \brief   Index of node's number of emmissions in \c pizMarkovModelEncodeNodeToArray().
 */

/**
 * \def     PIZ_MARKOV_MODEL_DATA 
 * \brief   Index of data in \c pizMarkovModelEncodeNodeToArray.
 */
 
#define PIZ_MARKOV_MODEL_START          0
#define PIZ_MARKOV_MODEL_TRANSITIONS    1
#define PIZ_MARKOV_MODEL_EMISSIONS      2
#define PIZ_MARKOV_MODEL_DATA           3

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_ALPHABET_SIZE               128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the backward threshold of the factor oracle.
 * \details This is the minimum size of repeated sequence required to go backward. 
 *          Default value is 2.
 * \param   x A valid pointer.
 * \param   n The backward threshold.
 * \return  The number of nodes.
 */
void pizFactorOracleSetBackwardThreshold (PIZFactorOracle *x, long n);

/**
 * \brief   Set the straight ratio of the factor oracle.
 * \details This is the chance to the playback head to go straight for each step in the automaton.
 *          Minimum is 0., maximum is 1., default value is 0.25.
 * \param   x A valid pointer.
 * \param   n The straight ratio.
 */
void pizFactorOracleSetStraightRatio (PIZFactorOracle *x, double f);

/**
 * \brief   Get the backward threshold of the factor oracle.
 * \param   x A valid pointer.
 * \return  The backward threshold.
 */
long pizFactorOracleBackwardThreshold (const PIZFactorOracle *x);

/**
 * \brief   Get the straight ratio of the factor oracle.
 * \param   x A valid pointer.
 * \return  The straight ratio.
 */
double pizFactorOracleStraightRatio (const PIZFactorOracle *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the neighborhood influence of the Kohonen map.
 * \details Default is 10.
 * \param   x A valid pointer.
 * \param   n The neighborhood influence.
 */
void pizKohonenMapSetRange (PIZKohonenMap *x, long n);

/**
 * \brief   Set the number of learning iterations planned for the Kohonen map.
 * \details Default is 60.
 * \param   x A valid pointer.
 * \param   n The number of learning iterations.
 */
void pizKohonenMapSetTraining (PIZKohonenMap *x, long n);

/**
 * \brief   Set the learning step of the Kohonen map.
 * \details Default is 1.
 * \param   x A valid pointer.
 * \param   n The learning step.
 */
void pizKohonenMapSetStep (PIZKohonenMap *x, double f);

/**
 * \brief   Get the neighborhood influence of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The neighborhood influence.
 */
long pizKohonenMapRange (const PIZKohonenMap *x);

/**
 * \brief   Get the number of learning iterations planned for the Kohonen map.
 * \param   x A valid pointer.
 * \return  The number of learning iterations planned.
 */
long pizKohonenMapTraining (const PIZKohonenMap *x);

/**
 * \brief   Get the learning step of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The learning step.
 */
double pizKohonenMapStep (const PIZKohonenMap *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the period of node's birth and death in the neural gas.
 * \details Default is 1 time out of 2.
 * \param   x A valid pointer.
 * \param   n The new node rate.
 */
void pizNeuralGasSetLambda (PIZNeuralGas *x, long n);

/**
 * \brief   Set the move factor of the winner node in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move factor.
 */
void pizNeuralGasSetEpsilon1 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the move factor of neighboors nodes in the neural gas.
 * \details Default is 0.25 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move factor.
 */
void pizNeuralGasSetEpsilon2 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the error decrease factor of generators in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The decrease factor.
 */
void pizNeuralGasSetAlpha (PIZNeuralGas *x, double f);

/**
 * \brief   Set the global error decrease factor in the neural gas.
 * \details Default is 0.1 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The decrease factor.
 */
void pizNeuralGasSetBeta (PIZNeuralGas *x, double f);

/**
 * \brief   Set the utility threshold of the neural gas.
 * \details Default is 10.
 * \param   x A valid pointer.
 * \param   n The utility threshold.
 */
void pizNeuralGasSetKappa (PIZNeuralGas *x, double f);

/**
 * \brief   Get the new node rate of the neural gas.
 * \param   x A valid pointer.
 * \return  The new node rate.
 */
long pizNeuralGasLambda (const PIZNeuralGas *x);

/**
 * \brief   Get the winner move factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The winner move factor.
 */
double pizNeuralGasEpsilon1 (const PIZNeuralGas *x);

/**
 * \brief   Get the neighboors move factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The neighboors move factor.
 */
double pizNeuralGasEpsilon2 (const PIZNeuralGas *x);

/**
 * \brief   Get the generators error decrease factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The generators error decrease factor.
 */
double pizNeuralGasAlpha (const PIZNeuralGas *x);

/**
 * \brief   Get the global error decrease factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The global error decrease factor.
 */
double pizNeuralGasBeta (const PIZNeuralGas *x);

/**
 * \brief   Get the utility threshold of the neural gas.
 * \param   x A valid pointer.
 * \return  The utility threshold.
 */
double pizNeuralGasKappa (const PIZNeuralGas *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the persistence ratio of learning.
 * \remark  HMM = ((old * persistence) + new) / (persistence + 1.)
 * \param   x A valid pointer.
 * \param   n The new persistence.
 */
void pizMarkovModelSetPersistence (PIZMarkovModel *x, double f);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Encode a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   node The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \remark	An example :  
 * \code
 * long             err = PIZ_GOOD;
 * PIZGrowingArray  *a = pizGrowingArrayNew (16);
 *
 * err = pizFactorOracleEncodeNodeToArray (x->factorOracle, node, a);
 *
 * if (!err)
 *      {
 *          long i, k, ref, lrs;
 *           
 *          ref = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_REFER);
 *          lrs = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_LRS);
 *          k   = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ARCS);
 *
 *          for (i = 0; i < k; i++) 
 *              {
 *                  long destination, value;
 *
 *                  destination = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_DATA + i);
 *                  value       = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_DATA + i + k);
 *              }
 *      }
 *   
 * pizGrowingArrayFree (a);
 *
 * \endcode
 */
PIZError pizFactorOracleEncodeNodeToArray (const PIZFactorOracle *x, long node, PIZGrowingArray *a);


/**
 * \brief   Encode concepts with a given size to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The size of concepts.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \remark	An example :  
 * \code
 * long             err = PIZ_GOOD;
 * long             n = 4;
 * PIZGrowingArray  *a = pizGrowingArrayNew (16);
 *
 * err = pizGaloisLatticeEncodeConceptsToArray (lattice, n, a);
 *
 * if (!err)
 *      {
 *          long i;
 *          long count = pizGrowingArrayValueAtIndex (a, PIZ_GALOIS_LATTICE_CONCEPTS);
 *          long *ptr  = pizGrowingArrayPtr (a);
 *          long *values = NULL;
 *
 *          for (i = 0; i < count; i++) {
 *                  values = ptr + PIZ_GALOIS_LATTICE_DATA + (n * i);
 *              }
 *      }
 *
 *  pizGrowingArrayFree (a);
 *
 * \endcode
 */
PIZError pizGaloisLatticeEncodeConceptsToArray (const PIZGaloisLattice *x, long n, PIZGrowingArray *a);

/**
 * \brief   Encode the vector of a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 */
PIZError pizKohonenMapEncodeVectorToArray (const PIZKohonenMap *x, long n, PIZGrowingArray *a);

/**
 * \brief   Encode the vector of a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 */
PIZError pizNeuralGasEncodeVectorToArray (const PIZNeuralGas *x, long n, PIZGrowingArray *a);

/**
 * \brief   Encode node's probabilities to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \remark	The values obtained (percents) are approximations.
 *          An example :  
 * \code
 *  long            err = PIZ_GOOD;
 *  long            n = 4;
 *  PIZGrowingArray *a = pizGrowingArrayNew (256);
 *
 *  err = pizMarkovModelEncodeNodeToArray (hmm, n, a);
 *
 *  if (!err)
 *      {
 *          long i;
 *          long start_probability, transition_probability, emission_probability;
 *          long t = pizGrowingArrayValueAtIndex (a, PIZ_MARKOV_MODEL_TRANSITIONS);
 *          long e = pizGrowingArrayValueAtIndex (a, PIZ_MARKOV_MODEL_EMISSIONS);
 *          long *ptr = pizGrowingArrayPtr (a);
 *
 *          start_probability = pizGrowingArrayValueAtIndex (a, PIZ_MARKOV_MODEL_START);
 *
 *          for (i = 0; i < t; i++) {
 *                  transition_probability = ptr + PIZ_MARKOV_MODEL_DATA + i;
 *              }
 *          
 *          for (i = t; i < (t + e); i++) {
 *                  emission_probability = ptr + PIZ_MARKOV_MODEL_DATA + i;
 *              }          
 *
 *      }
 *
 *  pizGrowingArrayFree (a);
 *
 * \endcode
 */
PIZError pizMarkovModelEncodeNodeToArray (const PIZMarkovModel *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_MAXMSP_H 
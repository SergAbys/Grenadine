/**
 * \file    pizMaxMSP.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
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

/**
 * \defgroup zoulou Zoulou
 * \ref factorOracle extensions for maxMSP's external.
 * \ingroup  max
 * \defgroup uniform Uniform
 * \ref finiteState extensions for maxMSP's external.
 * \ingroup  max
 * \defgroup romeo Romeo
 * \ref galoisLattice extensions for maxMSP's external. 
 * \ingroup  max
 * \defgroup charlie Charlie
 * \ref kohonenMap extensions for maxMSP's external.
 * \ingroup  max
 * \defgroup yankee Yankee
 * \ref neuralGas extensions for maxMSP's external.
 * \ingroup  max
 * \defgroup foxtrot Foxtrot
 * \ref markovModel extensions for maxMSP's external.
 * \ingroup  max
 */
/**
 * \defgroup    factorOracle Factor Oracle
 * \details     Thanks : Matthieu Raffinot, Maxime Crochemore, 
 *              Cyril Allauzen, Arnaud Lefebvre et Thierry Lecroq. \n
 * \li          http://www-igm.univ-mlv.fr/~lecroq/articles/awoca2000.pdf \n
 * \li          http://www.lsi.upc.edu/~marias/teaching/bom.pdf \n
 * \li          http://en.wikipedia.org/wiki/Factor_oracle
 *
 * \ingroup     algorithms
 * \defgroup    finiteState Finite State
 * \details     A finite state automaton is :
 * \li          One value per node.
 * \li          Last value for each new sequence is a terminal state.
 * \li          Randomly, oldest first, nodes with same value are crossing-over, 
 *              lonely node for a given value are killed, 
 *              until the population is collapsed under the defined threshold.
 * \li          Terminal states increase the probability of playback head's jump.
 *
 * \ingroup     algorithms
 * \defgroup    galoisLattice Galois Lattice
 * \details     Thanks : Robert Godin, Rokia Missaoui, Hassan Alaoui. \n
 * \li          http://www.labunix.uqam.ca/~godin_r/compint95.pdf \n
 * \li          http://en.wikipedia.org/wiki/Formal_concept_analysis
 *
 * \ingroup     algorithms
 * \defgroup    kohonenMap Kohonen Map
 * \details     Thanks : Jean-philippe Rennard. \n 
 * \li          http://en.wikipedia.org/wiki/Self-organizing_map \n
 * 
 * \ingroup     algorithms
 * \defgroup    neuralGas Neural Gas
 * \details     Thanks : Jim Holmström. \n
 * \li          http://www.booru.net/download/MasterThesisProj.pdf \n
 *
 * \ingroup     algorithms
 * \defgroup    markovModel Markov Model
 * \details     Thanks : T.Brouard, M. Slimane, J.-P. Asselin de Beauville, G. Venturini. \n
 * \li      http://archive.numdam.org/ARCHIVE/RSA/RSA_1998__46_2/RSA_1998__46_2_83_0/RSA_1998__46_2_83_0.pdf \n
 * \li      http://en.wikipedia.org/wiki/Hidden_Markov_model 
 *
 * \ingroup     algorithms
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

#define PIZ_ALPHABET_SIZE 128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_FACTOR_ORACLE_REFER 
 * \ingroup zoulou
 * \brief   Index of referTo in \c pizFactorOracleEncodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_LRS 
 * \ingroup zoulou
 * \brief   Index of lengthRepeatedSuffix in \c pizFactorOracleEncodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_ARCS
 * \ingroup zoulou
 * \brief   Index of number of arcs in \c pizFactorOracleEncodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_DATA
 * \ingroup zoulou
 * \brief   Index of data in \c pizFactorOracleEncodeToArray().
 */
 
#define PIZ_FACTOR_ORACLE_REFER     0
#define PIZ_FACTOR_ORACLE_LRS       1
#define PIZ_FACTOR_ORACLE_ARCS      2
#define PIZ_FACTOR_ORACLE_DATA      3

/**
 * \brief   Set the backward threshold of the factor oracle.
 * \details This is the minimum size of repeated sequence required to go backward. 
 *          Default value is 2.
 * \param   x A valid pointer.
 * \param   n The backward threshold.
 * \return  The number of nodes.
 * \ingroup zoulou
 */
void pizFactorOracleSetBackwardThreshold (PIZFactorOracle *x, long n);

/**
 * \brief   Set the straight ratio of the factor oracle.
 * \details This is the chance to the playback head to go straight for each step in the automaton.
 *          Minimum is 0., maximum is 1., default value is 0.25.
 * \param   x A valid pointer.
 * \param   n The straight ratio.
 * \ingroup zoulou
 */
void pizFactorOracleSetStraightRatio (PIZFactorOracle *x, double f);

/**
 * \brief   Get the backward threshold of the factor oracle.
 * \param   x A valid pointer.
 * \return  The backward threshold.
 * \ingroup zoulou
 */
long pizFactorOracleBackwardThreshold (const PIZFactorOracle *x);

/**
 * \brief   Get the straight ratio of the factor oracle.
 * \param   x A valid pointer.
 * \return  The straight ratio.
 * \ingroup zoulou
 */
double pizFactorOracleStraightRatio (const PIZFactorOracle *x);

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
 * err = pizFactorOracleEncodeToArray (x->factorOracle, node, a);
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
 * \ingroup zoulou
 */
PIZError pizFactorOracleEncodeToArray (const PIZFactorOracle *x, long node, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_GALOIS_LATTICE_CONCEPTS 
 * \ingroup romeo
 * \brief   Index of number of concepts in \c pizGaloisLatticeEncodeToArray().
 */

/**
 * \def     PIZ_GALOIS_LATTICE_DATA 
 * \ingroup romeo
 * \brief   Index of data in \c pizGaloisLatticeEncodeToArray().
 */
 
#define PIZ_GALOIS_LATTICE_CONCEPTS     0
#define PIZ_GALOIS_LATTICE_DATA         1

/**
 * \brief   Encode all the concepts with a given size to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The size of the concepts.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \remark	An example :  
 * \code
 * long             err = PIZ_GOOD;
 * long             n = 4;
 * PIZGrowingArray  *a = pizGrowingArrayNew (16);
 *
 * err = pizGaloisLatticeEncodeToArray (lattice, n, a);
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
 * \ingroup romeo
 */
PIZError pizGaloisLatticeEncodeToArray (const PIZGaloisLattice *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the neighborhood influence of the Kohonen map.
 * \details Default is 10.
 * \param   x A valid pointer.
 * \param   n The neighborhood influence.
 * \ingroup charlie
 */
void pizKohonenMapSetRange (PIZKohonenMap *x, long n);

/**
 * \brief   Set the number of learning iterations planned for the Kohonen map.
 * \details Default is 60.
 * \param   x A valid pointer.
 * \param   n The number of learning iterations.
 * \ingroup charlie
 */
void pizKohonenMapSetTraining (PIZKohonenMap *x, long n);

/**
 * \brief   Set the learning step of the Kohonen map.
 * \details Default is 1.
 * \param   x A valid pointer.
 * \param   n The learning step.
 * \ingroup charlie
 */
void pizKohonenMapSetStep (PIZKohonenMap *x, double f);

/**
 * \brief   Get the neighborhood influence of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The neighborhood influence.
 * \ingroup charlie
 */
long pizKohonenMapRange (const PIZKohonenMap *x);

/**
 * \brief   Get the number of learning iterations planned for the Kohonen map.
 * \param   x A valid pointer.
 * \return  The number of learning iterations planned.
 * \ingroup charlie
 */
long pizKohonenMapTraining (const PIZKohonenMap *x);

/**
 * \brief   Get the learning step of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The learning step.
 * \ingroup charlie
 */
double pizKohonenMapStep (const PIZKohonenMap *x);

/**
 * \brief   Encode the vector of a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \ingroup charlie
 */
PIZError pizKohonenMapEncodeToArray (const PIZKohonenMap *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the period of node's birth and death in the neural gas.
 * \details Default is 1 time out of 2.
 * \param   x A valid pointer.
 * \param   n The new node rate.
 * \ingroup yankee
 */
void pizNeuralGasSetLambda (PIZNeuralGas *x, long n);

/**
 * \brief   Set the move factor of the winner node in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move factor.
 * \ingroup yankee
 */
void pizNeuralGasSetEpsilon1 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the move factor of neighboors nodes in the neural gas.
 * \details Default is 0.25 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move factor.
 * \ingroup yankee
 */
void pizNeuralGasSetEpsilon2 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the error decrease factor of generators in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The decrease factor.
 * \ingroup yankee
 */
void pizNeuralGasSetAlpha (PIZNeuralGas *x, double f);

/**
 * \brief   Set the global error decrease factor in the neural gas.
 * \details Default is 0.1 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The decrease factor.
 * \ingroup yankee
 */
void pizNeuralGasSetBeta (PIZNeuralGas *x, double f);

/**
 * \brief   Set the utility threshold of the neural gas.
 * \details Default is 10.
 * \param   x A valid pointer.
 * \param   n The utility threshold.
 * \ingroup yankee
 */
void pizNeuralGasSetKappa (PIZNeuralGas *x, double f);

/**
 * \brief   Get the new node rate of the neural gas.
 * \param   x A valid pointer.
 * \return  The new node rate.
 * \ingroup yankee
 */
long pizNeuralGasLambda (const PIZNeuralGas *x);

/**
 * \brief   Get the winner move factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The winner move factor.
 * \ingroup yankee
 */
double pizNeuralGasEpsilon1 (const PIZNeuralGas *x);

/**
 * \brief   Get the neighboors move factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The neighboors move factor.
 * \ingroup yankee
 */
double pizNeuralGasEpsilon2 (const PIZNeuralGas *x);

/**
 * \brief   Get the generators error decrease factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The generators error decrease factor.
 * \ingroup yankee
 */
double pizNeuralGasAlpha (const PIZNeuralGas *x);

/**
 * \brief   Get the global error decrease factor of the neural gas.
 * \param   x A valid pointer.
 * \return  The global error decrease factor.
 * \ingroup yankee
 */
double pizNeuralGasBeta (const PIZNeuralGas *x);

/**
 * \brief   Get the utility threshold of the neural gas.
 * \param   x A valid pointer.
 * \return  The utility threshold.
 * \ingroup yankee
 */
double pizNeuralGasKappa (const PIZNeuralGas *x);

/**
 * \brief   Encode the vector of a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   n The index of the node.
 * \param   a A pointer to a dynamic array.
 * \return  An error code.
 * \ingroup yankee
 */
PIZError pizNeuralGasEncodeToArray (const PIZNeuralGas *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_MARKOV_MODEL_START 
 * \ingroup foxtrot
 * \brief   Index of node's start probability in \c pizMarkovModelEncodeToArray().
 */

/**
 * \def     PIZ_MARKOV_MODEL_TRANSITIONS 
 * \ingroup foxtrot
 * \brief   Index of node's number of transitions in \c pizMarkovModelEncodeToArray().
 */
 
/**
 * \def     PIZ_MARKOV_MODEL_EMISSIONS 
 * \ingroup foxtrot
 * \brief   Index of node's number of emmissions in \c pizMarkovModelEncodeToArray().
 */

/**
 * \def     PIZ_MARKOV_MODEL_DATA 
 * \ingroup foxtrot
 * \brief   Index of data in \c pizMarkovModelEncodeToArray.
 */
 
#define PIZ_MARKOV_MODEL_START          0
#define PIZ_MARKOV_MODEL_TRANSITIONS    1
#define PIZ_MARKOV_MODEL_EMISSIONS      2
#define PIZ_MARKOV_MODEL_DATA           3

/**
 * \brief   Set the persistence ratio of learning.
 * \remark  HMM = ((old * persistence) + new) / (persistence + 1.)
 * \param   x A valid pointer.
 * \param   n The new persistence.
 * \ingroup foxtrot
 */
void pizMarkovModelSetPersistence (PIZMarkovModel *x, double f);

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
 *  err = pizMarkovModelEncodeToArray (hmm, n, a);
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
 * \ingroup foxtrot
 */
PIZError pizMarkovModelEncodeToArray (const PIZMarkovModel *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_MAXMSP_H 
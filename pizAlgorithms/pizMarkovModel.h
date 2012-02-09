/**
 * \file    pizMarkovModel.h
 * \details Thanks : T.Brouard, M. Slimane, J.-P. Asselin de Beauville, G. Venturini. \n
 * \li      http://archive.numdam.org/ARCHIVE/RSA/RSA_1998__46_2/RSA_1998__46_2_83_0/RSA_1998__46_2_83_0.pdf \n
 * \li      http://en.wikipedia.org/wiki/Hidden_Markov_model 
 *
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup algorithms
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

#ifndef PIZ_MARKOV_MODEL_H
#define PIZ_MARKOV_MODEL_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

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

#define PIZ_ALPHABET_SIZE 128

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \remark Implemented simply with arrays.
 */
 
typedef struct _PIZMarkovModel {
    long    count;                              /*!< Number of learning iterations performed. */
    long    vectorSize;                         /*!< Size of learning vectors. */
    long    graphSize;                          /*!< Number of nodes in the HMM. */
    double  persistence;                        /*!< Ratio between old and new HMM after learning. */
    double  *start;                             /*!< Start probabilities. */
    double  *emission;                          /*!< Emission probabilities. */
    double  *transition;                        /*!< Transition probabilities. */
    double  *tempStart;                         /*!< Reestimated start probabilities. */
    double  *tempEmission;                      /*!< Reestimated emission probabilities. */
    double  *tempTransition;                    /*!< Reestimated transition probabilities. */
    double  *alpha;                             /*!< Forward values. */
    double  *tempAlpha;                         /*!< Rescaled forward values. */
    double  *beta;                              /*!< Backward values. */
    double  *coefficient;                       /*!< Rescaling coefficient. */
    } PIZMarkovModel;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the neural gas.
 * \details The function accepts two arguments, the number of nodes in the HMM, 
 *          and the size of learning vectors.
 *          The maximum number of nodes is 32, default is 6.
 *          The maximum size of learning vectors is 32, default is 6.
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer to arguments.
 * \return  A pointer to the new HMM.
 * \remark	The following shows how to create a HMM.  
 * \code
 * long args[2] = {16, 8};
 *
 * PIZMarkovModel *hmm = pizMarkovModelNew (2, &args);
 * PIZMarkovModel *hmm = pizMarkovModelNew (0, NULL); // default values
 *
 * \endcode
 */
PIZMarkovModel *pizMarkovModelNew (long argc, long *argv);

/**
 * \brief   Free the HMM.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizMarkovModelFree (PIZMarkovModel *x);

/**
 * \brief   Add values to the HMM.
 * \param   x A valid pointer.
 * \param   argc The number of values.
 * \param   argv A pointer to the values.
 * \return  An error code.
 * \remark  The HMM use kind of Baum-Welch algorithm to learn data.
 *          Values provided are clipped [0, 127].
 */
PIZError pizMarkovModelAdd (PIZMarkovModel *x, long argc, long *argv);

/**
 * \brief   Clear the HMM.
 * \param   x A valid pointer.
 */
void pizMarkovModelClear (PIZMarkovModel *x);

/**
 * \brief   Fill a given array with HMM values.
 * \remark  The array is filled with a probabilistic sequence generated by the HMM.
 * \param   argc Number of values to proceed.
 * \param   argv Pointer to the array to fill.
 * \return  An error code.
 */
PIZError pizMarkovModelProceed (const PIZMarkovModel *x, long argc, long *argv);

/**
 * \brief   Get the number of learnings currently performed.
 * \param   x A valid pointer.
 * \return  The number of learnings.
 */
long pizMarkovModelCount (const PIZMarkovModel *x);

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

PIZ_LOCAL PIZ_INLINE void   pizMarkovModelBaumWelch             (PIZMarkovModel *x, long argc, long *argv);
PIZ_LOCAL void              pizMarkovModelFillStochastically    (long argc, double *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
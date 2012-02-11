/**
 * \file    pizMarkovModel.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup foxtrot
 * \ingroup markovModel
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
 * \remark Implemented simply with arrays.
 * \ingroup markovModel
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
    double  *coefficient;                       /*!< Rescaling coefficients. */
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
 * \ingroup markovModel
 */
PIZMarkovModel *pizMarkovModelNew (long argc, long *argv);

/**
 * \brief   Free the HMM.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 * \ingroup markovModel
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
 * \ingroup markovModel
 */
PIZError pizMarkovModelAdd (PIZMarkovModel *x, long argc, long *argv);

/**
 * \brief   Clear the HMM.
 * \param   x A valid pointer.
 * \ingroup markovModel
 */
void pizMarkovModelClear (PIZMarkovModel *x);

/**
 * \brief   Fill a given array with HMM values.
 * \remark  The array is filled with a probabilistic sequence generated by the HMM.
 * \param   argc Number of values to proceed.
 * \param   argv Pointer to the array to fill.
 * \return  An error code.
 * \ingroup markovModel
 */
PIZError pizMarkovModelProceed (const PIZMarkovModel *x, long argc, long *argv);

/**
 * \brief   Get the number of learnings currently performed.
 * \param   x A valid pointer.
 * \return  The number of learnings.
 * \ingroup markovModel
 */
long pizMarkovModelCount (const PIZMarkovModel *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZ_INLINE void   pizMarkovModelBaumWelch             (PIZMarkovModel *x, long argc, long *argv);
PIZ_LOCAL void              pizMarkovModelFillStochastically    (long argc, double *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif  // PIZ_MARKOV_MODEL_H
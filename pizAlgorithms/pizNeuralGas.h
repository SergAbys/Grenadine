/**
 * \file    pizNeuralGas.h
 * \details Thanks : Jim Holmström. \n
 * \li      http://www.booru.net/download/MasterThesisProj.pdf \n
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

#ifndef PIZ_NEURAL_GAS_H
#define PIZ_NEURAL_GAS_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Neural gas element.  
 */
 
typedef struct _PIZNeuralGasHead {
    double          error;                      /*!< Cumulative error factor of the node. */
    double          utility;                    /*!< Cumulative utility factor of the node. */
    PIZItemset128   arcs;                       /*!< Arcs (indexes as bit field). */
    }PIZNeuralGasHead;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   The neural gas. 
 * \remark  Implemented with two synchronized pools  
 *          (one of pre-allocated heads, one of pre-allocated vectors), 
 *          and a map to mark slots used. 
 */
 
typedef struct _PIZNeuralGas {
    PIZItemset128       map;                    /*!< Pool's in use flags (as bit field). */
    PIZBoundedStack     *ticketMachine;         /*!< Pool management. */
    PIZNeuralGasHead    *headStock;             /*!< Pool of heads. */
    double              *vectorStock;           /*!< Pool of vectors. */
    long                count;                  /*!< Number of learning iterations performed. */
    long                vectorSize;             /*!< Size of a node's vector. */
    long                mapSize;                /*!< Number of nodes in the neural gas. */
    long                maximumSize;            /*!< Maximum number of nodes in the neural gas. */
    long                lambda;                 /*!< New node rate. */
    double              epsilon1;               /*!< Move winner factor. */
    double              epsilon2;               /*!< Move neighbours factor. */
    double              alpha;                  /*!< Decrease winners error factor. */
    double              beta;                   /*!< Decrease error factor. */
    double              kappa;                  /*!< Utility threshold. */
    } PIZNeuralGas;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the neural gas.
 * \details The function accepts two arguments, the maximum size of the neural gas, and the size of the vectors.
 *          For the neural gas size, maximum is 128, default is 20.
 *          For the vectors, maximum is 256, default is 4.
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer to arguments.
 * \return  A pointer to the new neural gas.
 * \remark	The following shows how to create a neural gas.  
 * \code
 * long args[2] = {50, 8};
 *
 * PIZKohonenMap *map = pizKohonenMapNew (2, &args);
 * PIZKohonenMap *map = pizKohonenMapNew (0, NULL); // default values
 *
 * \endcode
 */
PIZNeuralGas *pizNeuralGasNew (long argc, long *argv);

/**
 * \brief   Free the neural gas.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizNeuralGasFree (PIZNeuralGas *x);

/**
 * \brief   Add values to the neural gas.
 * \param   x A valid pointer.
 * \param   argc The number of values.
 * \param   argv A pointer to the values.
 * \return  An error code.
 * \remark  The birth and death of nodes occurs according to lambda value (1 time out of lambda).
 *          Values provided are clipped [0, 127].
 */
PIZError pizNeuralGasAdd (PIZNeuralGas *x, long argc, long *argv);

/**
 * \brief   Clear the neural gas.
 * \param   x A valid pointer.
 */
void pizNeuralGasClear (PIZNeuralGas *x);

/**
 * \brief   Fill a given array with neural gas values.
 * \remark  A node is randomly drawn, and the array filled with values from the node's vector. 
 *          While the array is not full, process is iterated.
 * \param   argc Number of values to proceed.
 * \param   argv Pointer to the array to fill.
 * \return  An error code.
 */
PIZError pizNeuralGasProceed (const PIZNeuralGas *x, long argc, long *argv);

/**
 * \brief   Get the number of learnings currently performed.
 * \param   x A valid pointer.
 * \return  The number of learnings.
 */
long pizNeuralGasCount (const PIZNeuralGas *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the frequence of node's birth and death in the neural gas.
 * \details Default is 1 time out of 2.
 * \param   x A valid pointer.
 * \param   n The new node rate.
 */
void pizNeuralGasSetLambda (PIZNeuralGas *x, long n);

/**
 * \brief   Set the moving factor of the winner node in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move winner factor.
 */
void pizNeuralGasSetEpsilon1 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the moving factor of the neighboors of the winner node in the neural gas.
 * \details Default is 0.25 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The move neighboors factor.
 */
void pizNeuralGasSetEpsilon2 (PIZNeuralGas *x, double f);

/**
 * \brief   Set the error's decrease factor of winners nodes in the neural gas.
 * \details Default is 0.5 ; (must be [0., 1.]).
 * \param   x A valid pointer.
 * \param   n The decrease factor.
 */
void pizNeuralGasSetAlpha (PIZNeuralGas *x, double f);

void pizNeuralGasSetBeta (PIZNeuralGas *x, double f);

void pizNeuralGasSetKappa (PIZNeuralGas *x, double f);
    
long pizNeuralGasLambda (const PIZNeuralGas *x);
double pizNeuralGasEpsilon1 (const PIZNeuralGas *x);
double pizNeuralGasEpsilon2 (const PIZNeuralGas *x);
double pizNeuralGasAlpha (const PIZNeuralGas *x);
double pizNeuralGasBeta (const PIZNeuralGas *x);
double pizNeuralGasKappa (const PIZNeuralGas *x);
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizNeuralGasEncodeVectorToArray        (const PIZNeuralGas *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
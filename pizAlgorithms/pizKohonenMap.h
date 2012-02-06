/**
 * \file    pizKohonenMap.h
 * \details Thanks : Jean-philippe Rennard. \n 
 * \li      http://en.wikipedia.org/wiki/Self-organizing_map \n
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

#ifndef PIZ_FACTOR_ORACLE_H
#define PIZ_FACTOR_ORACLE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   The Kohonen map.
 * \remark  Implemented simply as an array.
 */
 
typedef struct _PIZKohonenMap {
    long            mapSize;                /*!< Number of nodes in the Kohonen map. */
    long            vectorSize;             /*!< Size of node's vector.*/
    long            count;                  /*!< Number of training done. */
    long            range;                  /*!< Influence range. */
    long            training;               /*!< Number of training planned. */
    double          step;                   /*!< Learning step. */
    double          *map;                   /*!< The data. */
    } PIZKohonenMap;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the Kohonen map.
 * \details The function accepts two arguments, the size of the map, and the size of the vectors.
 *          For the map, minimum is 1, maximum is 100, default is 20.
 *          For the vectors, minimum is 1, maximum is 256, default is 4.
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer yo arguments.
 * \return  A pointer to the new Kohonen map.
 * \remark	The following shows how to create a Kohonen map.  
 * \code
 * long args[2] = {50, 8};
 *
 * PIZKohonenMap *map = pizKohonenMapNew (2, &args);
 * PIZKohonenMap *map = pizKohonenMapNew (0, NULL); // default values
 *
 * \endcode
 */
PIZKohonenMap *pizKohonenMapNew (long argc, long *argv);

/**
 * \brief   Free the Kohonen map.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizKohonenMapFree (PIZKohonenMap *x);

/**
 * \brief   Add values to the Kohonen map.
 * \param   x A valid pointer.
 * \param   argc The number of values.
 * \param   argv A pointer to the values.
 * \return  An error code.
 * \remark  The Kohonen map continue to learn even when the number of training planned is reached 
 *          (with last training parameters kept).
 */
PIZError pizKohonenMapAdd (PIZKohonenMap *x, long argc, long *argv);

/**
 * \brief   Clear the Kohonen map.
 * \param   x A valid pointer.
 */
void pizKohonenMapClear (PIZKohonenMap *x);

/**
 * \brief   Fill a given array with Kohonen map values.
 * \remark  A node is randomly drawn, and the array filled with values from the vector. 
 *          If the array is longer than the vector, values are copied again.
 * \param   argc Number of values to proceed.
 * \param   argv Pointer to the array to fill.
 * \return  An error code.
 */
PIZError pizKohonenMapProceed (const PIZKohonenMap *x, long argc, long *argv);

/**
 * \brief   Get the number of training currently done.
 * \param   x A valid pointer.
 * \return  The number of training.
 */
long pizKohonenMapCount (const PIZKohonenMap *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Set the influence range of the Kohonen map.
 * \details Default is 10.
 * \param   x A valid pointer.
 * \param   n The influence range.
 */
void pizKohonenMapSetRange (PIZKohonenMap *x, long n);

/**
 * \brief   Set the number of training planned for the Kohonen map.
 * \details Default is 60.
 * \param   x A valid pointer.
 * \param   n The number of training.
 */
void pizKohonenMapSetTraining (PIZKohonenMap *x, long n);

/**
 * \brief   Set the training step of the Kohonen map.
 * \details Default is 1.
 * \param   x A valid pointer.
 * \param   n The training step.
 */
void pizKohonenMapSetStep (PIZKohonenMap *x, double f);

/**
 * \brief   Get the influence range of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The influence range.
 */
long pizKohonenMapRange (const PIZKohonenMap *x);

/**
 * \brief   Get the number of training planned for the Kohonen map.
 * \param   x A valid pointer.
 * \return  The number of training planned.
 */
long pizKohonenMapTraining (const PIZKohonenMap *x);

/**
 * \brief   Get the training step of the Kohonen map.
 * \param   x A valid pointer.
 * \return  The training step.
 */
double pizKohonenMapStep (const PIZKohonenMap *x);


PIZError pizKohonenMapEncodeVectorToArray (const PIZKohonenMap *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
/**
 * \file        pizFactorOracle.h
 * \author      Jean Sapristi
 * \date        31 janvier 2012
 * \ingroup     zoulou
 * \ingroup     factorOracle
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
 * \remark  The arc from one node to next node (straight) is the first.
 * \ingroup factorOracle
 */
 
typedef struct _PIZFactorOracleNode {
    long                referTo;                        /*!< Final index of longest repeated sequence. */
    long                lengthRepeatedSuffix;           /*!< Size of longest repeated sequence. */
    PIZGrowingArray     *arcDestinations;               /*!< Arc destinations indexes. */
    PIZGrowingArray     *arcValues;                     /*!< Arc values */
    } PIZFactorOracleNode;
    
/**
 * \remark  Implemented with dynamic array of nodes ; For each node, arcs are stored in dynamic arrays too.
            Node's dynamic arrays are created on the \a first query and keep in memory 
            until pizFactorOracleFree() is called.
 */
     
typedef struct _PIZFactorOracle {
    long                size;                           /*!< Current number of nodes allocated. */
    long                peak;                           /*!< Maximum number of nodes reached. */
    long                index;                          /*!< Current number of nodes. */
    long                shuttle;                        /*!< Index of the playback head. */
    long                backwardThreshold;              /*!< Threshold for backward playback. */
    double              straightRatio;                  /*!< Threshold for straight playback. */
    PIZFactorOracleNode *nodes;                         /*!< Array of nodes. */
    } PIZFactorOracle;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the factor oracle.
 * \details The function currently do not need arguments (provided for later improvements).
 *          In case of failure the pointer is NULL.
 * \param   argc 0.
 * \param   argv NULL.
 * \return  A pointer to the new factor oracle.
 * \remark	The following shows how to create a factor oracle.  
 * \code
 * PIZFactorOracle *oracle = pizFactorOracleNew (0, NULL);
 *
 * \endcode
 */
PIZFactorOracle *pizFactorOracleNew (long argc, long *argv);

/**
 * \brief   Free the factor oracle.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizFactorOracleFree (PIZFactorOracle *x);

/**
 * \brief   Add values to the factor oracle.
 * \param   x A valid pointer.
 * \param   argc The number of values.
 * \param   argv A pointer to the values.
 * \return  An error code.
 * \remark  The number of allocated nodes is doubled in case of exceeding.
 */
PIZError pizFactorOracleAdd (PIZFactorOracle *x, long argc, long *argv);

/**
 * \brief   Clear the factor oracle.
 * \param   x A valid pointer.
 */
void pizFactorOracleClear (PIZFactorOracle *x);

/**
 * \brief   Fill a given array with factor oracle values.
 * \remark  Each step a number between 0. and 1. is randomly drawn : 
 *          if it is inferior to the straightRatio the playback head moves straight, 
 *          otherwise, there is half chance to moves backward (if backward threshold is reached), 
 *          and half chance to moves forward (if arcs exist).
 *
 * \param   argc Number of step to proceed.
 * \param   argv Pointer to the array.
 * \return  An error code.
 */
PIZError pizFactorOracleProceed (PIZFactorOracle *x, long argc, long *argv);

/**
 * \brief   Get the number of nodes in the factor oracle.
 * \param   x A valid pointer.
 * \return  The number of nodes.
 */
long pizFactorOracleCount (const PIZFactorOracle *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_FACTOR_ORACLE_H
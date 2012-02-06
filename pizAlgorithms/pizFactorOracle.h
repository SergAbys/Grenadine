/**
 * \file        pizFactorOracle.h
 * \details     Thanks : Matthieu Raffinot, Maxime Crochemore, 
 *              Cyril Allauzen, Arnaud Lefebvre et Thierry Lecroq. \n
 * \li          http://www-igm.univ-mlv.fr/~lecroq/articles/awoca2000.pdf \n
 * \li          http://www.lsi.upc.edu/~marias/teaching/bom.pdf \n
 * \li          http://en.wikipedia.org/wiki/Factor_oracle
 *
 * \author      Jean Sapristi
 * \date        31 janvier 2012
 * \ingroup     algorithms
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
 * \def     PIZ_FACTOR_ORACLE_ENCODE_REFER 
 * \brief   Index of referTo in \c pizFactorOracleEncodeNodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_ENCODE_LRS 
 * \brief   Index of lengthRepeatedSuffix in \c pizFactorOracleEncodeNodeToArray().
 */

/**
 * \def     PIZ_FACTOR_ORACLE_ENCODE_ARCS
 * \brief   Index of number of arcs in \c pizFactorOracleEncodeNodeToArray().
 */
 
#define PIZ_FACTOR_ORACLE_ENCODE_REFER      0
#define PIZ_FACTOR_ORACLE_ENCODE_LRS        1
#define PIZ_FACTOR_ORACLE_ENCODE_ARCS       2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Factor oracle element.  
 * \remark  The arc from one node to next node (straight) is the first.
 */
 
typedef struct _PIZFactorOracleNode {
    long                referTo;                        /*!< Final index of longest repeated sequence. */
    long                lengthRepeatedSuffix;           /*!< Size of longest repeated sequence. */
    PIZGrowingArray     *arcDestinations;               /*!< Arc destinations indexes. */
    PIZGrowingArray     *arcValues;                     /*!< Arc values */
    } PIZFactorOracleNode;
    
/**
 * \brief   The factor oracle.
 * \remark  Implemented with dynamic array of nodes ; For each node, arcs are stored in dynamic arrays too.
            Node's dynamic arrays are created on \a first query and keep in memory 
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
 *          if it is inferior to the straightRatio the playback head go straight, 
 *          otherwise, there is half chance to go backward (if backward threshold is reached), 
 *          and half chance to go forward (if arcs exist).
 *
 * \param   argc Number of step to proceed.
 * \param   argv Pointer to the \c long array to fill.
 * \return  An error code.
 */
PIZError pizFactorOracleProceed (PIZFactorOracle *x, long argc, long *argv);

/**
 * \brief   Get the number of nodes in the factor oracle.
 * \param   x A valid pointer.
 * \return  The number of nodes.
 */
long pizFactorOracleCount (const PIZFactorOracle *x);

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
 * \brief   Encode a node to a dynamic array.
 * \param   x A valid pointer.
 * \param   node The index of the node to get.
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
 *          ref = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ENCODE_REFER);
 *          lrs = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ENCODE_LRS);
 *          k   = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ENCODE_ARCS);
 *
 *          for (i = 0; i < k; i++) 
 *              {
 *                  long destination, value;
 *
 *                  destination = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ENCODE_ARCS + 1 + i);
 *                  value       = pizGrowingArrayValueAtIndex (a, PIZ_FACTOR_ORACLE_ENCODE_ARCS + 1 + i + k);
 *              }
 *      }
 *   
 * pizGrowingArrayFree (a);
 *
 * \endcode
 */
PIZError pizFactorOracleEncodeNodeToArray (const PIZFactorOracle *x, long node, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
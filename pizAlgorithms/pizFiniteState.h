/**
 * \file    pizFiniteState.h
 * \details A \em kinda finite state automaton is :
 * \li      One value per node.
 * \li      Last value for each new sequence is a terminal state.
 * \li      Randomly, oldest first, nodes with same value are crossing-over, 
 *          lonely node for a given value are killed, 
 *          until the population is collapsed under the defined threshold.
 * \li      Terminal states increase the probability of playback head's jump.
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

#ifndef PIZ_FINITE_STATE_H
#define PIZ_FINITE_STATE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Finite state element.  
 */
 
typedef struct _PIZFiniteStateNode {
    long            value;                      /*!< Value of the node. */
    bool            final;                      /*!< Terminal node state. */
    PIZItemset128   parents;                    /*!< Arcs (indexes as bit field). */
    PIZItemset128   childs;                     /*!< Arcs (indexes as bit field). */
    } PIZFiniteStateNode;

/**
 * \brief   The \em kinda finite state automaton.  
 * \remark  Implemented as an array of bounded queues, one per alphabet's value (alphabet is 0-127). 
 *          Bounded queues contains indexes of pre-allocated nodes (pool size is 128).
 */
 
typedef struct _PIZFiniteState {
    long                count;                  /*!< Number of nodes in the automaton. */
    long                shuttle;                /*!< Index of the playback head. */
    long                lotteryIndex;           /*!< Number of elements in the lottery array. */
    long                jumpChance;             /*!< Cumulative chance to jump when reading. */ 
    long                thresholdToMergeNodes;  /*!< Number of nodes to start crossing-over. */ 
    long                *lottery;               /*!< Temporary array for lottery drawing. */ 
    PIZBoundedQueue     **mapByValue;           /*!< Pointer to the array of bounded queues. */ 
    PIZBoundedStack     *ticketMachine;         /*!< Pool management. */ 
    PIZFiniteStateNode  *stock;                 /*!< Pool of nodes. */ 
    } PIZFiniteState;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief   Create the automaton.
 * \details The function accept one argument : the threshold to start crossing-over. 
 *          Minimum is 1, maximum is 100, default is 35.
 *          In case of failure the pointer is NULL.
 * \param   argc The number of arguments.
 * \param   argv A pointer to arguments.
 * \return  A pointer to the new automaton.
 * \remark	The following shows how to create an automaton.  
 * \code
 * long args = 50;
 *
 * PIZFiniteState *fsa = pizFiniteStateNew (1, &args);
 * PIZFiniteState *fsa = pizFiniteStateNew (0, NULL); // default value
 *
 * \endcode
 */
PIZFiniteState *pizFiniteStateNew (long argc, long *argv);

/**
 * \brief   Free the automaton.
 * \details It is safe to pass NULL pointer. 
 * \param   x A Pointer.
 */
void pizFiniteStateFree (PIZFiniteState *x);

/**
 * \brief   Add values to the automaton.
 * \param   x A valid pointer.
 * \param   argc The number of values.
 * \param   argv A pointer to the values.
 * \return  An error code.
 * \remark  The number of nodes can not be more than 128 at a given time, exceedings ones will be ignored.
 */
PIZError pizFiniteStateAdd (PIZFiniteState *x, long argc, long *argv);

/**
 * \brief   Clear the automaton.
 * \param   x A valid pointer.
 */
void pizFiniteStateClear (PIZFiniteState *x);

/**
 * \brief   Fill a given array with automaton values.
 * \remark  Each step the playback head randomly moves in the automaton. 
 *          It does not reverse. At the end of a branch, it jumps. 
 *          Each step, chances to jump increase (terminal states raise it more).
 * \param   argc Number of step to proceed.
 * \param   argv Pointer to the array.
 * \return  An error code.
 */
PIZError pizFiniteStateProceed (PIZFiniteState *x, long argc, long *argv);

/**
 * \brief   Get the number of nodes in the automaton.
 * \param   x A valid pointer.
 * \return  The number of nodes.
 */
long pizFiniteStateCount (const PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZ_INLINE PIZError pizFiniteStateMergeNodes (PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
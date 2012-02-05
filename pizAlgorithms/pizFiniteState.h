/**
 * \file    pizFiniteState.h
 * \details A \em kinda finite state automaton is :
 * \li      One value per node (not in arcs).
 * \li      Last value for each new sequence is a terminal state.
 * \li      Nodes are crossing-over by equal values, 
 *          until the population is collapsed under a given threshold.
 * \li      Terminal states increase the probability of playback head's jump.
 * \author  Jean Sapristi
 * \date    23 janvier 2012
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
 */
 
typedef struct _PIZFiniteState {
    long                count;
    long                shuttle;
    long                lotteryIndex;
    long                jumpChance;
    long                thresholdToMergeNodes;
    long                *lottery;
    PIZBoundedQueue     **mapByValue;
    PIZBoundedStack     *ticketMachine;
    PIZFiniteStateNode  *stock;
    } PIZFiniteState;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZFiniteState  *pizFiniteStateNew          (long argc, long *argv);
void            pizFiniteStateFree          (PIZFiniteState *x);

PIZError        pizFiniteStateAdd           (PIZFiniteState *x, long argc, long *argv);
void            pizFiniteStateClear         (PIZFiniteState *x);
PIZError        pizFiniteStateProceed       (PIZFiniteState *x, long argc, long *argv);
long            pizFiniteStateCount         (const PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL PIZError pizFiniteStateMergeNodes (PIZFiniteState *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
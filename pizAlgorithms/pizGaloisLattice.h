/**
 * \file    pizGaloisLattice.h
 * \details Thanks : Robert Godin, Rokia Missaoui, Hassan Alaoui. \n
 * \li      http://www.labunix.uqam.ca/~godin_r/compint95.pdf \n
 * \li      http://en.wikipedia.org/wiki/Formal_concept_analysis
 *
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

#ifndef PIZ_GALOIS_LATTICE_H
#define PIZ_GALOIS_LATTICE_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def     PIZ_FACTOR_ORACLE_ENCODE_REFER 
 * \brief   Index of number of concepts in \c pizGaloisLatticeEncodeConceptsByCardinalToArray().
 */
 
#define PIZ_GALOIS_LATTICE_ENCODE_CONCEPTS  0

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Galois lattice element.  
 */
 
typedef struct _PIZGaloisLatticeConcept {
    long            cardinal;                           /*!< Number of values in the concept. */
    PIZItemset128   itemset;                            /*!< Values in the concept (as bit field). */
    PIZItemset128   parents;                            /*!< Arcs (indexes as bit field). */
    PIZItemset128   childs;                             /*!< Arcs (indexes as bit field). */
    } PIZGaloisLatticeConcept;

typedef struct _PIZGaloisLattice {
    PIZItemset128           itemsetToBeAdded;
    PIZItemset128           itemsetIntersection;
    long                    count;
    long                    thresholdToKillConcepts;
    long                    targetedConcept;
    long                    shuttle;
    long                    previousShuttle;
    long                    itemsetIntersectionCardinal;
    long                    mapByCardinalPeak;
    long                    tempMapByCardinalPeak;              //
    bool                    needToMakeMap;
    PIZGrowingArray         **mapByCardinal;
    PIZGrowingArray         **tempMapByCardinal;
    PIZBoundedStack         *ticketMachine;
    PIZGaloisLatticeConcept *stock;
    } PIZGaloisLattice;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZGaloisLattice    *pizGaloisLatticeNew        (long argc, long *argv);
void                pizGaloisLatticeFree        (PIZGaloisLattice *x);

PIZError            pizGaloisLatticeAdd         (PIZGaloisLattice *x, long argc, long *argv);
void                pizGaloisLatticeClear       (PIZGaloisLattice *x);
PIZError            pizGaloisLatticeProceed     (PIZGaloisLattice *x, long argc, long *argv);
long                pizGaloisLatticeCount       (const PIZGaloisLattice *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void      pizGaloisLatticeReconnect   (PIZGaloisLattice *x, long g, long n);
PIZ_LOCAL PIZError  pizGaloisLatticeMakeMap     (PIZGaloisLattice *x);
PIZ_LOCAL void      pizGaloisLatticeKillConcept (PIZGaloisLattice *x, long n);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizGaloisLatticeEncodeConceptsToArray (const PIZGaloisLattice *x, long n, PIZGrowingArray *a);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
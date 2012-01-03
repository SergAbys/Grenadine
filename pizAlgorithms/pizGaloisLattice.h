/*
 *  pizGaloisLattice.h
 *
 *	(Treillis de Galois / Diagramme de Hasse).
 *
 *	Variation autour de : "Incremental concept formation algorithms based 
 *	on Galois (concept) lattices", par Robert Godin, Rokia Missaoui, Hassan Alaoui.
 *
 *  Created : 02/06/11.
 *  Copyright 2011 : Tom Javel & Jean Sapristi.
 *
 *  nicolas.danet@free.fr
 *
 *	This software is governed by the CeCILL-C license under French law and
 *	abiding by the rules of distribution of free software. You can use, 
 *	modify and/or redistribute the software under the terms of the CeCILL-C
 *	license as circulated by CEA, CNRS and INRIA at the following URL
 *	"http://www.cecill.info". 
 *
 *	As a counterpart to the access to the source code and rights to copy,
 *	modify and redistribute granted by the license, users are provided only
 *	with a limited warranty  and the software's author, the holder of the
 *	economic rights, and the successive licensors have only limited
 *	liability. 
 *
 *	In this respect, the user's attention is drawn to the risks associated
 *	with loading, using, modifying and/or developing or reproducing the
 *	software by the user in light of its specific status of free software,
 *	that may mean that it is complicated to manipulate, and that also
 *	therefore means that it is reserved for developers and experienced
 *	professionals having in-depth computer knowledge. Users are therefore
 *	encouraged to load and test the software's suitability as regards their
 *	requirements in conditions enabling the security of their systems and/or 
 *	data to be ensured and, more generally, to use and operate it in the 
 *	same conditions as regards security. 
 *	
 *	The fact that you are presently reading this means that you have had
 *	knowledge of the CeCILL-C license and that you accept its terms.
 */
 
/*
 *	Last modified : 17/12/11.
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

#define PIZ_GALOIS_LATTICE_ENCODE_CONCEPTS	0

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
	
typedef struct _PIZGaloisLatticeConcept {
	long			cardinal;
	PIZItemset128	itemset;
	PIZItemset128	parents;
	PIZItemset128	childs;
	} PIZGaloisLatticeConcept;
	
typedef struct _PIZGaloisLattice {
	PIZItemset128			itemsetToBeAdded;
	PIZItemset128			itemsetIntersection;
	long					count;
	long					thresholdToKillConcepts;
	long					targetedConcept;
	long					shuttle;
	long					previousShuttle;
	long					itemsetIntersectionCardinal;
	long					mapByCardinalPeak;
	long					tempMapByCardinalPeak;				//
	bool					needToMakeMap;
	PIZGrowingArray			**mapByCardinal;
	PIZGrowingArray			**tempMapByCardinal;
	PIZBoundedStack			*ticketMachine;
	PIZGaloisLatticeConcept *stock;
	} PIZGaloisLattice;
	
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZGaloisLattice	*pizGaloisLatticeNew		(long argc, long *argv);
void				pizGaloisLatticeFree		(PIZGaloisLattice *galoisLattice);

PIZError			pizGaloisLatticeAdd			(PIZGaloisLattice *galoisLattice, long argc, long *argv);
void				pizGaloisLatticeClear		(PIZGaloisLattice *galoisLattice);
long				pizGaloisLatticeCount		(PIZGaloisLattice *galoisLattice);
PIZError			pizGaloisLatticeProceed		(PIZGaloisLattice *galoisLattice, long argc, long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
											
PIZ_LOCAL void		pizGaloisLatticeReconnect	(PIZGaloisLattice *galoisLattice, long g, long n);
PIZ_LOCAL PIZError	pizGaloisLatticeMakeMap		(PIZGaloisLattice *galoisLattice);
PIZ_LOCAL void		pizGaloisLatticeKillConcept	(PIZGaloisLattice *galoisLattice, long n);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizGaloisLatticeEncodeConceptsByCardinalToArray (PIZGaloisLattice *x, long n, PIZGrowingArray *array);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
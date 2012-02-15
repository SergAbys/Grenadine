/**
 * \file    pizDoxygen.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 */

/*
 *  Copyright 2012 : Tom Javel & Jean Sapristi.
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

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \defgroup    max Max/MSP
 * \defgroup    algorithms Algorithms
 * \defgroup    structures Custom Structures
 * \defgroup    sequence Sequence
 */
 
/**
 * \defgroup    types Types
 * \ingroup     structures
 * \defgroup    queue Queue
 * \ingroup     structures
 * \defgroup    linklist Linklist
 * \ingroup     structures
 * \defgroup    hashtable Hash Table
 * \ingroup     structures
 * \defgroup    dictionary Dictionary
 * \ingroup     structures
 * \defgroup    itemset128 Itemset 128
 * \ingroup     structures
 * \defgroup    itemset1024 Itemset 1024
 * \ingroup     structures
 * \defgroup    growingArray Growing Array
 * \ingroup     structures
 * \defgroup    arrayPool Growing Array Pool
 * \ingroup     structures
 * \defgroup    boundedQueue Bounded Queue
 * \ingroup     structures
 * \defgroup    boundedStack Bounded Stack
 * \ingroup     structures
 * \defgroup    boundedHashTable Bounded Hash Table
 * \ingroup     structures
 */
 
 /**
 * \defgroup    zoulou Zoulou
 * \ref         factorOracle extension for maxMSP's external.
 * \ingroup     max
 * \defgroup    uniform Uniform
 * \ref         finiteState extension for maxMSP's external.
 * \ingroup     max
 * \defgroup    romeo Romeo
 * \ref         galoisLattice extension for maxMSP's external. 
 * \ingroup     max
 * \defgroup    charlie Charlie
 * \ref         kohonenMap extension for maxMSP's external.
 * \ingroup     max
 * \defgroup    yankee Yankee
 * \ref         neuralGas extension for maxMSP's external.
 * \ingroup     max
 * \defgroup    foxtrot Foxtrot
 * \ref         markovModel extension for maxMSP's external.
 * \ingroup     max
 * \defgroup    tralala Tralala
 * \ref         sequence extension for maxMSP's external.
 * \ingroup     max
 */

/**
 * \defgroup    factorOracle Factor Oracle
 * \details     Thanks : Matthieu Raffinot, Maxime Crochemore, 
 *              Cyril Allauzen, Arnaud Lefebvre et Thierry Lecroq. \n
 * \li          http://www-igm.univ-mlv.fr/~lecroq/articles/awoca2000.pdf \n
 * \li          http://www.lsi.upc.edu/~marias/teaching/bom.pdf \n
 * \li          http://en.wikipedia.org/wiki/Factor_oracle
 *
 * \ingroup     algorithms
 * \defgroup    finiteState Finite State
 * \details     A finite state automaton is :
 * \li          One value per node.
 * \li          Last value for each new sequence is a terminal state.
 * \li          Randomly, oldest first, nodes with same value are crossing-over, 
 *              lonely node for a given value are killed, 
 *              until the population is collapsed under the defined threshold.
 * \li          Terminal states increase the probability of playback head's jump.
 *
 * \ingroup     algorithms
 * \defgroup    galoisLattice Galois Lattice
 * \details     Thanks : Robert Godin, Rokia Missaoui, Hassan Alaoui. \n
 * \li          http://www.labunix.uqam.ca/~godin_r/compint95.pdf \n
 * \li          http://en.wikipedia.org/wiki/Formal_concept_analysis
 *
 * \ingroup     algorithms
 * \defgroup    kohonenMap Kohonen Map
 * \details     Thanks : Jean-philippe Rennard. \n 
 * \li          http://en.wikipedia.org/wiki/Self-organizing_map \n
 * 
 * \ingroup     algorithms
 * \defgroup    neuralGas Neural Gas
 * \details     Thanks : Jim Holmström. \n
 * \li          http://www.booru.net/download/MasterThesisProj.pdf \n
 *
 * \ingroup     algorithms
 * \defgroup    markovModel Markov Model
 * \details     Thanks : T.Brouard, M. Slimane, J.-P. Asselin de Beauville, G. Venturini. \n
 * \li      http://archive.numdam.org/ARCHIVE/RSA/RSA_1998__46_2/RSA_1998__46_2_83_0/RSA_1998__46_2_83_0.pdf \n
 * \li      http://en.wikipedia.org/wiki/Hidden_Markov_model 
 *
 * \ingroup     algorithms
 */
 
/**
 * \defgroup    sequenceTypes Types
 * \ingroup     sequence
 * \defgroup    sequenceClass Class Life
 * \ingroup     sequence
 * \defgroup    sequenceBases Bases
 * \ingroup     sequence
 */
 
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
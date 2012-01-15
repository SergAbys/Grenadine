/**
 * \file	pizMarkovModel.h
 * \details	Thanks : T.Brouard, M. Slimane, J.-P. Asselin de Beauville, G. Venturini.
 * \author	Jean Sapristi
 * \date	15 janvier 2012
 */

/*
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

#ifndef PIZ_MARKOV_MODEL_H
#define PIZ_MARKOV_MODEL_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_MARKOV_MODEL_ENCODE_START			0
#define PIZ_MARKOV_MODEL_ENCODE_TRANSITIONS		1
#define PIZ_MARKOV_MODEL_ENCODE_EMISSIONS		2
#define PIZ_MARKOV_MODEL_ENCODE_VALUES			3

#define PIZ_MARKOV_MODEL_DEFAULT_PERSISTENCE	0.5

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZMarkovModel {
	long	count;
	long	vectorSize;
	long	graphSize;
	double	persistence;
	double	*start;
	double	*emission;
	double	*transition;
	double	*tempStart;
	double	*tempEmission;
	double	*tempTransition;
	double	*alpha;
	double	*tempAlpha;
	double	*beta;
	double  *coefficient;
	} PIZMarkovModel;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZMarkovModel	*pizMarkovModelNew					(long argc, long *argv);
void			pizMarkovModelFree					(PIZMarkovModel *markovModel);

PIZError		pizMarkovModelAdd					(PIZMarkovModel *markovModel, long argc, long *argv);
void			pizMarkovModelClear					(PIZMarkovModel *markovModel);
long			pizMarkovModelCount					(PIZMarkovModel *markovModel);
PIZError		pizMarkovModelProceed				(PIZMarkovModel *markovModel, long argc, long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void			pizMarkovModelSetPersistence		(PIZMarkovModel *markovModel, double f);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError		pizMarkovModelEncodeNodeToArray		(PIZMarkovModel *x, long n, PIZGrowingArray *array);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_LOCAL void	pizMarkovModelBaumWelch				(PIZMarkovModel *markovModel, long argc, long *argv);
PIZ_LOCAL void	pizMarkovModelFillStochastically	(long argc, double *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
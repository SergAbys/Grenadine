/*
 *  pizNeuralGas.h
 *
 *	Thanks to Jim Holmström. 
 *
 *  Created : 31/08/11.
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

#ifndef PIZ_NEURAL_GAS_H
#define PIZ_NEURAL_GAS_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_NEURAL_GAS_DEFAULT_LAMBDA			2
#define PIZ_NEURAL_GAS_DEFAULT_ALPHA			0.5
#define PIZ_NEURAL_GAS_DEFAULT_BETA				0.1
#define PIZ_NEURAL_GAS_DEFAULT_EPSILON1			0.5
#define PIZ_NEURAL_GAS_DEFAULT_EPSILON2			0.25
#define PIZ_NEURAL_GAS_DEFAULT_KAPPA			10.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZNeuralGasHead {
	double			error;
	double			utility;
	PIZItemset128	arcs;
	}PIZNeuralGasHead;
	
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZNeuralGas {
	PIZItemset128		map;	
	PIZNeuralGasHead	*headStock;
	PIZBoundedStack		*ticketMachine;
	double				*vectorStock;
	long				count;
	long				vectorSize;
	long				mapSize;
	long				maximumSize;
	long				lambda;
	double				epsilon1;
	double				epsilon2;					//
	double				alpha;						
	double				beta;						
	double				kappa;
	} PIZNeuralGas;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZNeuralGas	*pizNeuralGasNew				(long argc, long *argv);
void			pizNeuralGasFree				(PIZNeuralGas *neuralGas);

PIZError		pizNeuralGasAdd					(PIZNeuralGas *neuralGas, long argc, long *argv);
void			pizNeuralGasClear				(PIZNeuralGas *neuralGas);
long			pizNeuralGasCount				(PIZNeuralGas *neuralGas);
PIZError		pizNeuralGasProceed				(PIZNeuralGas *neuralGas, long argc, long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long			pizNeuralGasLambda				(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetLambda			(PIZNeuralGas *neuralGas, long n);
double			pizNeuralGasEpsilon1			(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetEpsilon1			(PIZNeuralGas *neuralGas, double f);
double			pizNeuralGasEpsilon2			(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetEpsilon2			(PIZNeuralGas *neuralGas, double f);
double			pizNeuralGasAlpha				(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetAlpha			(PIZNeuralGas *neuralGas, double f);
double			pizNeuralGasBeta				(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetBeta				(PIZNeuralGas *neuralGas, double f);
double			pizNeuralGasKappa				(PIZNeuralGas *neuralGas);
void			pizNeuralGasSetKappa			(PIZNeuralGas *neuralGas, double f);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizNeuralGasEncodeVectorToArray		(PIZNeuralGas *x, long n, PIZGrowingArray *array);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
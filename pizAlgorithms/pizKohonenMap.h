/**
 * \file    pizKohonenMap.h
 * \details Thanks : Jean-philippe Rennard.
 * \author  Jean Sapristi
 * \date    15 janvier 2012
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

#define PIZ_KOHONEN_MAP_DEFAULT_RANGE               10
#define PIZ_KOHONEN_MAP_DEFAULT_TRAINING            60
#define PIZ_KOHONEN_MAP_DEFAULT_STEP                1.

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _PIZKohonenMap {
    long            mapSize;
    long            vectorSize;
    long            count;
    long            range;
    long            training;
    double          step;
    double          *map;
    } PIZKohonenMap;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZKohonenMap   *pizKohonenMapNew               (long argc, long *argv);
void            pizKohonenMapFree               (PIZKohonenMap *kohonenMap);

PIZError        pizKohonenMapAdd                (PIZKohonenMap *kohonenMap, long argc, long *argv);
void            pizKohonenMapClear              (PIZKohonenMap *kohonenMap);
long            pizKohonenMapCount              (PIZKohonenMap *kohonenMap);
PIZError        pizKohonenMapProceed            (PIZKohonenMap *kohonenMap, long argc, long *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

long            pizKohonenMapRange              (PIZKohonenMap *kohonenMap);
void            pizKohonenMapSetRange           (PIZKohonenMap *kohonenMap, long n);
long            pizKohonenMapTraining           (PIZKohonenMap *kohonenMap);
void            pizKohonenMapSetTraining        (PIZKohonenMap *kohonenMap, long n);
double          pizKohonenMapStep               (PIZKohonenMap *kohonenMap);
void            pizKohonenMapSetStep            (PIZKohonenMap *kohonenMap, double f);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZError pizKohonenMapEncodeVectorToArray       (PIZKohonenMap *x, long n, PIZGrowingArray *array);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif
/**
 * \file	pizTime.h
 * \author	Jean Sapristi
 * \date	March 25, 2012.
 */

/*
 *  Copyright (c) 2012, Jean Sapristi & Tom Javel, 
 *  "nicolas.danet@free.fr".
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

#ifndef PIZ_TIME_H
#define PIZ_TIME_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizDataStructures.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef __MACH__

#include <mach/mach_time.h>

typedef uint64_t PIZTime;
typedef uint64_t PIZNano;

#endif // __MACH__

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_TIME_ZERO                0ULL
#define PIZ_TIME_NANO_PER_SECOND     1000000000ULL

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void        pizTimeSet          (PIZTime *t);
void        pizTimeCopy         (PIZTime *t, PIZTime *toCopy);
void        pizTimeAddNano      (PIZTime *t, PIZNano *ns); 
PIZError    pizTimeElapsedNano  (PIZTime *t0, PIZTime *t1, PIZNano *result);
void        pizTimespecWithNano (struct timespec *t, PIZNano *ns);
bool        pizTimeIsZero       (PIZTime *t);
void        pizTimeSetNano      (PIZNano *ns, double f);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

#ifdef __MACH__

PIZ_EXTERN void pizTimeSet (PIZTime *t) 
{
    (*t) = mach_absolute_time ( );
}

PIZ_EXTERN void pizTimeCopy (PIZTime *t, PIZTime *toCopy)
{
    (*t) = (*toCopy);
}

PIZ_EXTERN void pizTimespecWithNano (struct timespec *t, PIZNano *ns)
{
    t->tv_sec  = (time_t)(*ns / PIZ_TIME_NANO_PER_SECOND);
    t->tv_nsec = (long)(*ns % PIZ_TIME_NANO_PER_SECOND);
}

PIZ_EXTERN bool pizTimeIsZero (PIZTime *t)
{
    return ((*t) == PIZ_TIME_ZERO);
}

PIZ_EXTERN void pizTimeSetNano (PIZNano *ns, double f)
{
    (*ns) = (PIZNano)f;
}

#endif // __MACH__

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_TIME_H
/*
 * \file	pizTime.c
 * \author	Jean Sapristi
 * \date	March 19, 2012.
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

#include "pizTime.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef __MACH__

PIZError pizTimeSet (PIZTime *t) 
{
    *t = mach_absolute_time ( );
    
    return PIZ_GOOD;
}

void pizTimeCopy (PIZTime *t, PIZTime *toCopy)
{
    *t = *toCopy;
}

void pizTimeIncrement (PIZTime *t, PIZNano *ns)
{
    *t += *ns;
}

PIZError pizTimeElapsed (PIZTime *t0, PIZTime *t1, PIZNano *result)
{
    long                                err = PIZ_ERROR;
    uint64_t                            elapsed;
    static mach_timebase_info_data_t    piz_timebaseInfo;

    if (*t1 > *t0) {
        err = PIZ_GOOD;
        
        elapsed = *t1 - *t0;

        if (piz_timebaseInfo.denom == 0) {
            mach_timebase_info (&piz_timebaseInfo);
        }

        *result = elapsed * piz_timebaseInfo.numer / piz_timebaseInfo.denom;
    }

    return err;
}

#endif // __MACH__

// -------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------:x
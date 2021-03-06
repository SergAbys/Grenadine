
/*
 *  Copyright (c) 2011, Nicolas Danet, 
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

#ifndef PIZ_TYPES_H
#define PIZ_TYPES_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus 
    #define PIZ_START_C_LINKAGE extern "C" { 
    #define PIZ_END_C_LINKAGE   }
#else
    #define PIZ_START_C_LINKAGE      
    #define PIZ_END_C_LINKAGE
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_GOOD    0UL
#define PIZ_ERROR   1UL
#define PIZ_MEMORY  2UL

typedef unsigned long PIZError;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef __OBEX_H__
    typedef unsigned long ulong;
    typedef unsigned int  uint;
#endif // __OBEX_H__

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef __MACH__

#include <mach/mach_time.h>

#define PIZ_SEED (uint)mach_absolute_time( )

#endif // __MACH__

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_MAGIC 3402300126L

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef enum {
    PIZ_MAGIC_SCALE      = 12,
    PIZ_MAGIC_CHANNEL    = 16,
    PIZ_MAGIC_VELOCITY   = 127,
    PIZ_MAGIC_PITCH      = 127,
    PIZ_MAGIC_ULONG      = 32
    }PIZMagic;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef ABS
#define ABS(a) ((a)<0?-(a):(a))
#endif

#ifndef CLAMP
#define CLAMP(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifndef __bool_true_false_are_defined
    #define true                            1
    #define false                           0
    #define bool                            long
    #define __bool_true_false_are_defined   1
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus 
    #define PIZ_EXTERN_INLINE
    #define PIZ_EXTERN inline
#else
    #define PIZ_EXTERN extern inline
#endif 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define PIZ_STATIC static
#define PIZ_INLINE static inline
#define PIZ_LOCAL  __attribute__((visibility("hidden")))
#define PIZ_PUBLIC __attribute__((visibility("default")))

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_TYPES_H
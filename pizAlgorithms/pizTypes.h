/**
 * \file    pizTypes.h
 * \author  Jean Sapristi
 * \date    31 janvier 2012
 * \ingroup structures
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

#ifndef PIZ_TYPES_H
#define PIZ_TYPES_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Error codes.
 */
 
typedef enum {
    PIZ_GOOD    =  0,                       /*!< No error. */
    PIZ_ERROR   =  1,                       /*!< Generic error. */
    PIZ_MEMORY  =  2,                       /*!< Memory allocation error. */
    }PIZError;

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Compute the maximum of \a x and \a y.
 */
 
#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

/**
 * \brief Compute the minimum of \a x and \a y.
 */
 
#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

/**
 * \brief Compute absolute value of \a x.
 */
 
#ifndef ABS
#define ABS(x) ((x)<0?-(x):(x))
#endif

/**
 * \brief Keep \a x between \a a and \a b.
 */
 
#ifndef CLAMP
#define CLAMP(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def   true 
 * \brief Boolean value.
 */
 
/**
 * \def   false 
 * \brief Boolean value.
 */
 
/**
 * \def   bool 
 * \brief Boolean type.
 */
 
#ifndef __cplusplus
    #ifndef __bool_true_false_are_defined
        #define true                            1
        #define false                           0
        #define bool                            long
        #define __bool_true_false_are_defined   1
    #endif
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Hide the function in the exported symbol table.
 */
 
#ifndef WIN_VERSION
    #define PIZ_LOCAL  __attribute__ ((visibility("hidden")))
#else
    #define PIZ_LOCAL  
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \def   PIZ_INLINE
 * \brief Inline the function.
 */
 
#ifndef WIN_VERSION
    #define PIZ_EXTERN_INLINE
    #define PIZ_INLINE inline
    #define PIZ_EXTERN extern inline
#else
    #define PIZ_INLINE 
#endif

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief For C++ linker.
 */
 
#ifdef __cplusplus
    #define PIZ_START_C_LINKAGE \
        extern "C" {
#else
    #define PIZ_START_C_LINKAGE //
#endif 

/**
 * \brief For C++ linker.
 */
 
#ifdef __cplusplus
    #define PIZ_END_C_LINKAGE \
        }
#else
    #define PIZ_END_C_LINKAGE //
#endif 

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_TYPES_H
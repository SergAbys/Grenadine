/**
 * \file    pizItemset128.h
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

#ifndef PIZ_ITEMSET128_H
#define PIZ_ITEMSET128_H

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "pizTypes.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <string.h>

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

/**
 * \brief Bit field dimensions.
 */
enum {
    PIZ_ITEMSET128_SIZE_IN_WORDS    = 4,                /*!< Number of \c long in the bit field. */
    PIZ_ITEMSET128_SIZE_OF_ULONG    = 32,               /*!< Size of an \c unsigned \c long. */
    PIZ_ITEMSET128_SIZE_IN_BITS     = 128               /*!< Number of bits in the bit field. */
    };

/**
 * \brief The 128 bits field.
 */
typedef struct _PIZItemset128 {
    unsigned long items[4];                             /*!< 128 Bits field as 4 \c long */
    } PIZItemset128;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

PIZ_START_C_LINKAGE

/**
 * \brief   At specified index, set/let the bit to 1.
 * \param   itemset A valid pointer.
 * \param   index The index (zero-based).
 * \warning For efficiency the index is NOT checked ; so crash may occur with invalid indexing.
 */
void pizItemset128SetAtIndex (PIZItemset128 *itemset, long index);

/**
 * \brief   At specified index, set/let the bit to 0.
 * \param   itemset A valid pointer.
 * \param   index The index (zero-based).
 * \warning For efficiency the index is NOT checked ; so crash may occur with invalid indexing.
 */
void pizItemset128UnsetAtIndex (PIZItemset128 *itemset, long index);

/**
 * \brief   Set/let all bits to 0.
 * \param   itemset A valid pointer.
 */
void pizItemset128Clear (PIZItemset128 *itemset);

/**
 * \brief   Get the number of set bits.
 * \param   itemset A valid pointer.
 * \return  The number of set bits.
 */
long pizItemset128Count (const PIZItemset128 *itemset);

/**
 * \brief   At specified index, get the value of a bit.
 * \param   itemset A valid pointer.
 * \param   index The index (zero-based).
 * \return  True if set, otherwise false.
 * \warning For efficiency the index is NOT checked ; so crash may occur with invalid indexing.
 */
bool pizItemset128IsSetAtIndex  (const PIZItemset128 *itemset, long index);

/**
 * \brief   Compute union between two bit fields.
 * \details r = a | b.
 * \param   a A valid pointer.
 * \param   b A valid pointer.
 * \param   r A valid pointer.
 */
void pizItemset128Union (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r);

/**
 * \brief   Compute intersection between two bit fields.
 * \details r = a & b.
 * \param   a A valid pointer.
 * \param   b A valid pointer.
 * \param   r A valid pointer.
 */
void pizItemset128Intersection (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r);

/**
 * \brief   Test if the bit field \a a is included in the bit field \a b.
 * \param   a A valid pointer.
 * \param   b A valid pointer.
 * \return  True if included, otherwise false.
 */
bool pizItemset128IsIncluded (const PIZItemset128 *a, const PIZItemset128 *b);

/**
 * \brief   Test if the bit field \a a is equal to the bit field \a b.
 * \param   a A valid pointer.
 * \param   b A valid pointer.
 * \return  True if equal, otherwise false.
 */
bool pizItemset128IsEqual (const PIZItemset128 *a, const PIZItemset128 *b);

PIZ_END_C_LINKAGE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#ifdef PIZ_EXTERN_INLINE

PIZ_EXTERN void pizItemset128SetAtIndex (PIZItemset128 *itemset, long index)
{
    long i, p, m;
    
    i = index / PIZ_ITEMSET128_SIZE_OF_ULONG;
    p = index % PIZ_ITEMSET128_SIZE_OF_ULONG;
    
    m = 1 << p;
    
    itemset->items[i] |= m;
}

PIZ_EXTERN void pizItemset128UnsetAtIndex (PIZItemset128 *itemset, long index) 
{
    long i, p, m;
    
    i = index / PIZ_ITEMSET128_SIZE_OF_ULONG;
    p = index % PIZ_ITEMSET128_SIZE_OF_ULONG;
    
    m = 1 << p;
    
    itemset->items[i] &= ~m;
}

PIZ_EXTERN void pizItemset128Clear (PIZItemset128 *itemset)  
{
    long i;
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++) {
            itemset->items[i] = 0;
        }
}

PIZ_EXTERN long pizItemset128Count (const PIZItemset128 *itemset)
{
    long i;
    long k = 0;
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++)
        {
            unsigned long n = itemset->items[i];
            
            while (n != 0) {
                    k  += (n & 1);
                    n >>= 1;
                }
        }
    
    return k;
}

PIZ_EXTERN bool pizItemset128IsSetAtIndex (const PIZItemset128 *itemset, long index) 
{
    unsigned long k = 0;

    long i, p;

    i = index / PIZ_ITEMSET128_SIZE_OF_ULONG;
    p = index % PIZ_ITEMSET128_SIZE_OF_ULONG;
    
    k = itemset->items[i];

    k >>= p;
    k  &= 1;
    
    return (k != 0);
}

PIZ_EXTERN void pizItemset128Union (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r) 
{
    long i;
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++) {
            r->items[i] = a->items[i] | b->items[i];
        }
}

PIZ_EXTERN void pizItemset128Intersection (const PIZItemset128 *a, const PIZItemset128 *b, PIZItemset128 *r) 
{
    long i;
    
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++) {
            r->items[i] = a->items[i] & b->items[i];
        }
}

PIZ_EXTERN bool pizItemset128IsIncluded (const PIZItemset128 *a, const PIZItemset128 *b)
{
    long i;
    bool k = true;
            
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++)
        {
            if (b->items[i] != (b->items[i] | a->items[i])) {
                    k = false;
                    break;
                }
        }
        
    return k;
}

PIZ_EXTERN bool pizItemset128IsEqual (const PIZItemset128 *a, const PIZItemset128 *b)
{
    long i;
    long k = true;
            
    for (i = 0; i < PIZ_ITEMSET128_SIZE_IN_WORDS; i++)
        {
            if (a->items[i] != b->items[i]) {
                    k = false;
                    break;
                }
        }
        
    return k;
}

#endif // PIZ_EXTERN_INLINE

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
#endif // PIZ_ITEMSET128_H
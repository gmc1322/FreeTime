#pragma once

// This only works for STATIC arrays!
#define SizeOfArray( Array ) ( sizeof( Array ) / sizeof( Array[ 0 ] ) )

/**
 * \brief Sorts an array in ascending order, using stl swap and putting the result in-place.
 *
 * \tparam ArrayType - The type of the array.
 * \param Array      - The Array to sort in-place.
 * \param ArraySize  - The size of the Array.
 *
 * \return void
 * 
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 * 
 * DO NOT USE ON DESCENDING SORTED ARRAY!
 */
template< typename ArrayType >
void Sort( ArrayType *Array, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in ascending order, using stl swap and putting the result in a new array.
 *
 * \tparam ArrayType      - The type of the array.
 * \param[ in ] InArray   - The Array to sort.
 * \param[ out ] OutArray - The resulting sorted Array.
 * \param ArraySize       - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 *
 * DO NOT USE ON DESCENDING SORTED ARRAY!
 */
template< typename ArrayType >
void Sort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept;


/**
 * \brief Sorts an array in descending order, using stl swap and putting the result in-place.
 *
 * \tparam ArrayType  - The type of the array.
 * \param[ in ] Array - The Array to reverse sort.
 * \param ArraySize   - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 */
template< typename ArrayType >
void RSort( ArrayType *Array, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in descending order, using stl swap and putting the result in a new array.
 *
 * \tparam ArrayType      - The type of the array.
 * \param[ in ] InArray   - The Array to reverse sort.
 * \param[ out ] OutArray - The resulting sorted Array.
 * \param ArraySize       - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 */
template< typename ArrayType >
void RSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in ascending order, using ArrayType's swap and putting the result in-place.
 *
 * \tparam ArrayType  - The type of the array.
 * \param[ in ] Array - The Array to sort. 
 * \param ArraySize   - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 * 
 * DO NOT USE ON DESCENDING SORTED ARRAY!
 * 
 * CANNOT BE USED ON BUILT-IN TYPES!
 * 
 * SWAP FUNCTION SYNTAX: static void swap( CLASS &, CLASS & )
 */
template< typename ArrayType >
void SwapSort( ArrayType *Array, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in ascending order, using ArrayType's swap and putting the result in a new array.
 *
 * \tparam ArrayType      - The type of the array.
 * \param[ in ] InArray   - The Array to sort.
 * \param[ out ] OutArray - The resulting sorted Array.
 * \param ArraySize       - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 *
 * DO NOT USE ON DESCENDING SORTED ARRAY!
 *
 * CANNOT BE USED ON BUILT-IN TYPES!
 *
 * SWAP FUNCTION SYNTAX: static void swap( CLASS &, CLASS & )
 */
template< typename ArrayType >
void SwapSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in descending order, using ArrayType's swap and putting the result in-place.
 *
 * \tparam ArrayType  - The type of the array.
 * \param[ in ] Array - The Array to sort.
 * \param ArraySize   - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 *
 * CANNOT BE USED ON BUILT-IN TYPES!
 *
 * SWAP FUNCTION SYNTAX: static void swap( CLASS &, CLASS & )
 */
template< typename ArrayType >
void RSwapSort( ArrayType *Array, size_t ArraySize ) noexcept;

/**
 * \brief Sorts an array in descending order, using ArrayType's swap and putting the result in a new array.
 *
 * \tparam ArrayType      - The type of the array.
 * \param[ in ] InArray   - The Array to sort. 
 * \param[ out ] OutArray - The resulting sorted Array.
 * \param ArraySize       - The size of the Array.
 *
 * \return void
 *
 * DO NOT USE ON ALREADY SORTED ARRAYS OR ARRAYS WITH LOTS OF DUPLICATES!
 *
 * CANNOT BE USED ON BUILT-IN TYPES!
 *
 * SWAP FUNCTION SYNTAX: static void swap( CLASS &, CLASS & )
 */
template< typename ArrayType >
void RSwapSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept;


/**
 * \brief Moves every element to its opposite location respectivly. (IE. First elem swaps with the Last elem)
 *
 * \tparam ArrayType - The type of the array. 
 * \param Array      - The Array to reverse.
 * \param ArraySize  - The size of the Array
 *
 * \return void
 */
template< typename ArrayType >
void ReverseArray( ArrayType *Array, size_t ArraySize ) noexcept;

/**
* \brief Copys an array into another array.
*
* \tparam ArrayType      - The type of the array.
* \param[ in ] InArray   - The Array to copy.
* \param[ out ] OutArray - The resulting copied Array.
* \param ArraySize       - The size of the Array.
*
* \return void
*/
template< typename ArrayType >
void CopyArray( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept;

#include "SortSuite.cpp"

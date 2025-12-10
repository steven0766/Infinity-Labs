/* BitArray - modify and use an array of 64-bits */
#ifndef __BIT_ARR__
#define __BIT_ARR__

#include <stddef.h>   /* size_t */

typedef size_t bit_array_ty;

/***************************************************************************************
* "bit_idx" is [0,63]
*  0 is right most
****************************************************************************************/

/***************************************************************************************  
*   Receives a bit_array and returns a bit array with all bits set on.
****************************************************************************************/
bit_array_ty BitArraySetAll(bit_array_ty bit_array);

/** Sets off all bits of "bit_array" to 0 ("off" bits) */
bit_array_ty BitArrayResetAll(bit_array_ty bit_array);

/* Sets a specific bit in "bit_array" to 1 ("on" bit) */
bit_array_ty BitArraySetOn(bit_array_ty bit_array, unsigned int index);

/* Sets a specific bit in "bit_array" to 0 ("off" bit) */
bit_array_ty BitArraySetOff(bit_array_ty bit_array, unsigned int index);

/* Sets a index bit in "bit_array" to a specified value */
/* "value" accepts two values: 0 or 1 */
bit_array_ty BitArraySetBit(bit_array_ty bit_array, unsigned int index, unsigned int value);

/* Get a specific bit's value in "bit_array" */
/* Function returns two possible values: 0 for "off" bit or 1 for "on" bit */
unsigned int BitArrayGetBitVal(bit_array_ty bit_array, unsigned int index);

/* Flips index bit in bit_array  */
bit_array_ty BitArrayFlipBit(bit_array_ty bit_array, unsigned int index);

/* Generates a bit array that's a mirror of "bit_array" */
bit_array_ty BitArrayMirror(bit_array_ty bit_array);

/* Rotates "bit_array" a given number of times to the right */
bit_array_ty BitArrayRotateRight(bit_array_ty bit_array, size_t num_of_rotations);

/* Rotates "bit_array" a given number of times to the left */
bit_array_ty BitArrayRotateLeft(bit_array_ty bit_array, size_t num_of_rotations);

/* Count number of "on" bits (bits with a value of 1) in bit_array */
bit_array_ty BitArrayCountOn(bit_array_ty bit_array);

/* Count number of "off" bits (bits with a value of 0) in bit_array */
bit_array_ty BitArrayCountOff(bit_array_ty bit_array);

/* Convert "bit_array" to string, assigning it to "dest"  */
/* "dest" must be large enough to include all "bit_array" bits */
char* BitArrayToString(bit_array_ty bit_array, char* dest);

#endif /* __BIT_ARR__ */

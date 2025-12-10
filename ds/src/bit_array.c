/***********************************************************************
*File_name: bit_array.c
*Reviewer: Rozalin
*Version:  2.0
*Date:	   10.12.25
************************************************************************/

#include "../include/bit_array.h"
#include <assert.h>   /* assert     */
#include <limits.h>   /* CHAAR_BITS */


#define MASK(idx)                 ((bit_array_ty)1 << (idx))   
#define BIT_ARR_SIZE                  (sizeof(bit_array_ty)*CHAR_BIT)


bit_array_ty BitArraySetAll(bit_array_ty bit_array)
{
    bit_array = 0;
    return ~bit_array;
}

/** Sets off all bits of "bit_array" to 0 ("off" bits) */
bit_array_ty BitArrayResetAll(bit_array_ty bit_array)
{
    bit_array = 0;
    return bit_array;
}

/* Sets a specific bit in "bit_array" to 1 ("on" bit) */
bit_array_ty BitArraySetOn(bit_array_ty bit_array, unsigned int index)
{
    assert(index < BIT_ARR_SIZE);

    return (MASK(index) | bit_array);
    /* declare var one equal 1 */
    
    /* shift left var one "index" times */
    
    /* return bit_array | one */
    
}

/* Sets a specific bit in "bit_array" to 0 ("off" bit) */
bit_array_ty BitArraySetOff(bit_array_ty bit_array, unsigned int index)
{
    assert(index < BIT_ARR_SIZE);

    return (~MASK(index) & bit_array);
    /* var one equal 1 */
    
    /* shift vard left "index" times */
    
    /* bit_array & (one) */
}

/* Sets a index bit in "bit_array" to a specified value */
/* "value" accepts two values: 0 or 1 */
bit_array_ty BitArraySetBit(bit_array_ty bit_array,
                            unsigned int index, unsigned int value)
{
    assert(index < BIT_ARR_SIZE);
    
    return (0 != value) ? BitArraySetOn(bit_array, index) :
                          BitArraySetOff(bit_array, index);
    /* if value equal 1 -> use BitArraySetOn function */
    
    /* else use BitArraySetOff function */
    
}

/* Get a specific bit's value in "bit_array" */
/* Function returns two possible values: 0 for "off" bit or 1 for "on" bit */
unsigned int BitArrayGetBitVal(bit_array_ty bit_array, unsigned int index)
{
    assert(index < BIT_ARR_SIZE);
    
    return ((bit_array >> index) & 1);
    /* return shift bit_array "index" times right & mask 1 */
}

/* Flips index bit in bit_array  */
bit_array_ty BitArrayFlipBit(bit_array_ty bit_array, unsigned int index)
{
    /* if shifting bit array "index" time right and mask 1 equal 1,
       use BitArraySetOff function*/
       
    /* else use BitArraySetOn function */
    assert(index < BIT_ARR_SIZE);
    
    return (MASK(index) ^ bit_array);

}

/* Generates a bit array that's a mirror of "bit_array" */
bit_array_ty BitArrayMirror(bit_array_ty bit_array)
{
    /* swap bits int group of 32 */
    bit_array = (bit_array << 32) | (bit_array >> 32);
    /* swap bits int group of 16 */
    bit_array = ((bit_array & 0x0000FFFF0000FFFF) << 16) |
                ((bit_array >> 16) & 0x0000FFFF0000FFFF);
    /* swap bits int group of 8 */
    bit_array = ((bit_array & 0x00FF00FF00FF00FF) << 8) |
                ((bit_array >> 8) & 0x00FF00FF00FF00FF);
    /* swap bits int group of 4 */
    bit_array = ((bit_array & 0x0F0F0F0F0F0F0F0F) << 4) |
                ((bit_array >> 4) & 0x0F0F0F0F0F0F0F0F);
    /* swap bits int group of 2 */
    bit_array = ((bit_array & 0x3333333333333333) << 2) |
                ((bit_array >> 2) & 0x3333333333333333);
    /* swap bits int group of 1 */
    bit_array = ((bit_array & 0x5555555555555555) << 1) |
                ((bit_array >> 1) & 0x5555555555555555);
                
    return bit_array;
}

/* Rotates "bit_array" a given number of times to the right */
bit_array_ty BitArrayRotateRight(bit_array_ty bit_array, size_t num_of_rotations)
{
    num_of_rotations %= BIT_ARR_SIZE;
    return (bit_array << (BIT_ARR_SIZE - num_of_rotations)) |
            (bit_array >> num_of_rotations);
    /* return (bit_array << (num_of_rotation % sizeof(bit_array))) | 
              (bit_array >> num_of_rotation) */
}

/* Rotates "bit_array" a given number of times to the left */
bit_array_ty BitArrayRotateLeft(bit_array_ty bit_array, size_t num_of_rotations)
{
    num_of_rotations %= BIT_ARR_SIZE;
    return (bit_array >> (BIT_ARR_SIZE - num_of_rotations)) |
            (bit_array << num_of_rotations);
    /* return (bit_array >> (num_of_rotation % sizeof(bit_array))) | 
              (bit_array << num_of_rotation) */
}

/* Count number of "on" bits (bits with a value of 1) in bit_array */
bit_array_ty BitArrayCountOn(bit_array_ty bit_array)
{
    /* substract bit_array with mask of 0101...
       on bit_array and shift 1 right */
    /* add bit_array masked with 0011... shift 2 with
       bit_array shift 2 and masked with 0011.. */
    /* add bit_array masked with 00001111... shift 4 with
       bit_array shift 4 and masked with 00001111... */
    /* add bit_array masked with 0xff00ff00 shift 8 with
       bit_array shift 8 and masked with 0xff00ff00 */
    /* add bit_array masked with 0xffff0000... shift 16 with
       bit_array shift 16 and masked with 0xffff0000... */
    /* add bit_array masked with ~0x0 shift 32 with
       bit_array shift 32 and masked with ~0x0 */
       
    bit_array = bit_array - ((bit_array >> 1) & 0x5555555555555555);

    bit_array = (bit_array & 0x3333333333333333) + 
                ((bit_array >> 2) & 0x3333333333333333);

    bit_array = (bit_array & 0x0f0f0f0f0f0f0f0f) + 
                ((bit_array >> 4) & 0x0f0f0f0f0f0f0f0f);

    bit_array = (bit_array & 0x00ff00ff00ff00ff) + 
                ((bit_array >> 8) & 0x00ff00ff00ff00ff);

    bit_array = (bit_array & 0x0000ffff0000ffff) + 
                ((bit_array >> 16) & 0x0000ffff0000ffff);

    bit_array = (bit_array & 0x00000000ffffffff) + 
                ((bit_array >> 32) & 0x00000000ffffffff);
                
    return bit_array;
}

/* Count number of "off" bits (bits with a value of 0) in bit_array */
bit_array_ty BitArrayCountOff(bit_array_ty bit_array)
{
    return BitArrayCountOn(~bit_array);
}

/* Convert "bit_array" to string, assigning it to "dest"  */
/* "dest" must be large enough to include all "bit_array" bits */
char* BitArrayToString(bit_array_ty bit_array, char* dest)
{
    int i = 0;

    for (i = BIT_ARR_SIZE - 1; i >= 0; --i)
    {
        dest[(BIT_ARR_SIZE - 1) - i] = ((bit_array >> i) & 1) ? '1' : '0';
    }

    dest[BIT_ARR_SIZE] = '\0';
    return dest;
}


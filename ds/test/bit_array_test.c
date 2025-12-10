/***********************************************************************
*File_name: bit_array_test.c
*Reviewer: Rozalin
*Version:  2.0
*Date:	   10.12.25
************************************************************************/

#include <stdio.h>    /* printf, bit_array_ty */
#include <math.h>     /* pow                  */
#include <stdint.h>   /* SIZE_MAX             */
#include <string.h>   /* strcmp, memset       */

#include "../include/bit_array.h"


static void TestBitArraySetAll(void);
static void TestBitArrayResetAll(void);
static void TestBitArraySetOn(void);
static void TestBitArraySetOff(void);
static void TestBitArraySetBit(void);
static void TestBitArrayGetBitVal(void);
static void TestBitArrayFlipBit(void);
static void TestBitArrayMirror(void);
static void TestBitArrayRotateRight(void);
static void TestBitArrayRotateLeft(void);
static void TestBitArrayCountOn(void);
static void TestBitArrayCountOff(void);
static void TestBitArrayToString(void);
static void Print(bit_array_ty expected, bit_array_ty actual, char* func_name);


#define LAST_BIT_ON                 ((bit_array_ty)1 << 63)
#define SIZE_OF_ARR(arr)              (sizeof(arr) / sizeof(arr[0]))


int main()
{
    TestBitArraySetAll();
    TestBitArrayResetAll();
    TestBitArraySetOn();
    TestBitArraySetOff();
    TestBitArraySetBit();
    TestBitArrayGetBitVal();
    TestBitArrayFlipBit();
    TestBitArrayMirror();
    TestBitArrayRotateRight();
    TestBitArrayRotateLeft();
    TestBitArrayCountOn();
    TestBitArrayCountOff();
    TestBitArrayToString();
    
    return 0;
}


static void TestBitArraySetAll(void)
{
    bit_array_ty actual = 596;
    bit_array_ty expected = SIZE_MAX;
    actual = BitArraySetAll(actual);
    
    if (expected != actual)
    {
        Print(expected, actual, "BitArraySetAll");
    }
}

static void TestBitArrayResetAll(void)
{
    bit_array_ty actual = 39485;
    bit_array_ty expected = 0;
    actual = BitArrayResetAll(actual);
    
    if (expected != actual)
    {
        Print(expected, actual, "BitArrayResetAll");
    }
}

static void TestBitArraySetOn(void)
{
    bit_array_ty actual = 5;
    bit_array_ty index = 1;
    bit_array_ty expected = 7;
    
    actual = BitArraySetOn(actual, index);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetOn");
    }
    
    actual = 0;
    index = 0;
    expected = 1;
    
    actual = BitArraySetOn(actual, index);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetOn");
    }
}

static void TestBitArraySetOff(void)
{
    bit_array_ty actual = 5;
    bit_array_ty index = 0;
    bit_array_ty expected = 4;
    
    actual = BitArraySetOff(actual, index);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetOff");
    }
    
    actual = 2;
    index = 3;
    expected = 2;
    
    actual = BitArraySetOff(actual, index);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetOff");
    }
}

static void TestBitArraySetBit(void)
{
    bit_array_ty actual = 5;
    bit_array_ty index = 0;
    bit_array_ty expected = 4;
    
    actual = BitArraySetBit(actual, index, 0);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetBit");
    }
    
    actual = 0;
    index = 0;
    expected = 1;
    
    actual = BitArraySetBit(actual, index, 1);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArraySetBit");
    }
}

static void TestBitArrayGetBitVal(void)
{
     bit_array_ty actual = 5;
     bit_array_ty idx = 2;
     bit_array_ty expected = 1;
     
     actual = BitArrayGetBitVal(actual, idx);
     
     if (actual != expected)
     {
         Print(expected, actual, "ArrayGetBitVal");
     }
     
     actual = 2;
     idx = 3;
     expected = 0;
     
     actual = BitArrayGetBitVal(actual, idx);
     
     if (actual != expected)
     {
         Print(expected, actual, "ArrayGetBitVal");
     }
}

static void TestBitArrayFlipBit(void)
{
    bit_array_ty actual = 5;
    bit_array_ty idx = 1;
    bit_array_ty expected = 7;
    
    actual = BitArrayFlipBit(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayFlipBit");
    }
    
    actual = 0;
    idx = 63;
    expected = LAST_BIT_ON;
    
    actual = BitArrayFlipBit(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayFlipBit");
    }
}
/* --------------------------------------------------------- */
static void TestBitArrayMirror(void)
{
    bit_array_ty num = 0, expected = 0, actual = 0;

    num = 1;
    expected = LAST_BIT_ON;
    actual = BitArrayMirror(num);
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayMirror LSB→MSB");
    }

    num = LAST_BIT_ON;
    expected = 1;
    actual = BitArrayMirror(num);
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayMirror MSB→LSB");
    }

    num = 0xF0F0F0F0F0F0F0F0;
    expected = 0x0F0F0F0F0F0F0F0F;
    actual = BitArrayMirror(num);
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayMirror pattern");
    }
}

static void TestBitArrayRotateRight(void)
{
    bit_array_ty actual = 2;
    bit_array_ty idx = 1;
    bit_array_ty expected = 1;
    
    actual = BitArrayRotateRight(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayRotateRight");
    }
    
    actual = 1;
    idx = 1;
    expected = LAST_BIT_ON;
    
    actual = BitArrayRotateRight(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayRotateRight");
    }
}

static void TestBitArrayRotateLeft(void)
{
    bit_array_ty actual = 1;
    bit_array_ty idx = 3;
    bit_array_ty expected = (1 << 3);
    
    actual = BitArrayRotateLeft(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayRotateLeft");
    }
    
    actual = LAST_BIT_ON;
    idx = 1;
    expected = 1;
    
    actual = BitArrayRotateLeft(actual, idx);
    
    if (actual != expected)
    {
        Print(expected, actual, "BitArrayRotateLeft");
    }
}

static void TestBitArrayCountOn(void)
{
    /* 0xf0f0f0f0f0f0f0ff = 
       1111000011110000111100001111000011110000111100001111000011111111 */
    bit_array_ty inputs[3] = {0xf0f0f0f0f0f0f0ff, 0, ~0};
    bit_array_ty expected[3] = {36, 0, 64};
    bit_array_ty actual = 0;
    bit_array_ty i = 0;
    
    for (i = 0; i < SIZE_OF_ARR(inputs); ++i)
    {
        actual = BitArrayCountOn(inputs[i]);
        
        if (actual != expected[i])
        {
            Print(expected[i], actual, "BitArrayCountOn");
        }
    }
}

static void TestBitArrayCountOff(void)
{
    /* 0xf0f0f0f0f0f0f0ff = 
       1111000011110000111100001111000011110000111100001111000011111111 */
    bit_array_ty inputs[3] = {0xf0f0f0f0f0f0f0ff, 0, ~0};
    bit_array_ty expected[3] = {28, 64, 0};
    bit_array_ty actual = 0;
    bit_array_ty i = 0;
    
    for (i = 0; i < SIZE_OF_ARR(inputs); ++i)
    {
        actual = BitArrayCountOff(inputs[i]);
        
        if (actual != expected[i])
        {
            Print(expected[i], actual, "BitArrayCountOff");
        }
    }
}

static void TestBitArrayToString(void)
{
    bit_array_ty num = 5; /* ...00000101 */
    char actual[65] = {0};
    char expected[65] = {0};
    
    memset(expected, '0', 64);
    expected[61] = '1';
    expected[63] = '1';
    expected[64] = '\0';

    BitArrayToString(num, actual);

    if (strcmp(actual, expected) != 0)
    {
        printf("BitArrayToString failed:\nexpected: %s\nactual:   %s\n",
               expected, actual);
    }
}

static void Print(bit_array_ty expected, bit_array_ty actual, char* func_name)
{
    printf("%s failed:\n"
                "expected: %ld, actual: %ld\n", func_name, expected, actual);
}

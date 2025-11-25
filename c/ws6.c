#include <stdio.h>  /* printf, size_t */
#include <string.h> /* memcpy */

static long PowTwo(unsigned int x, unsigned int y);
static int IsPowTwo(unsigned int n);
static int IsPowTwoNoLoop(unsigned int n);
static int AddOne(unsigned int num);
static void CheckThreeOnes(unsigned int arr[], size_t size);
static unsigned char ByteMirrorLoop(unsigned char byte);
static unsigned char ByteMirrorNoLoop(unsigned char b);
static int SwapThirdAndFifth(unsigned char b);
static int ClosestLowerDivisibleBySixteen(unsigned int num);
static void SwapBit(int* num1, int* num2);
static unsigned int CountSetBitsLoop(unsigned int num);
static unsigned int CountSetBitsNoLoop(unsigned int n);

static int CheckSecAndSixBit(unsigned char b);
static int CheckSecOrSixBit(unsigned char b);
static void PrintFloatBits(float x);

int main()
{
  unsigned int arr[] = {7, 12, 13};
  int num1 = 5, num2 = 7;
  printf("powTwo: %ld\n", PowTwo(3, 4));
  printf("IsPowTwo: %d\n", IsPowTwo(8));
  printf("IsPowTwoNoLoop: %d\n", IsPowTwoNoLoop(8));
  printf("AddOne: %d\n", AddOne(7));
  printf("CheckThreeOnes: ");
  CheckThreeOnes(arr, 3);
  printf("\nByteMirrorLoop: %d\n", ByteMirrorLoop(85));
  printf("ByteMirrorNoLoop: %d\n", ByteMirrorNoLoop(85));
  printf("CheckSecAndSixBit: %d\n", CheckSecAndSixBit(0x22));
  printf("CheckSecOrSixBit: %d\n", CheckSecOrSixBit(0x22));
  printf("SwapThirdAndFifth: %d\n", SwapThirdAndFifth(90));
  printf("ClosestLowerDivisibleBySixteen: %d\n", 
  ClosestLowerDivisibleBySixteen(90));
  printf("SwapBit-> num1: %d, num2: %d\n", num1, num2);
  SwapBit(&num1, &num2);
  printf("SwapBit-> num1: %d, num2: %d\n", num1, num2);
  printf("CountSetBitsLoop: %d\n", CountSetBitsLoop(27));
  printf("CountSetBitsNoLoop: %d\n", CountSetBitsNoLoop(27));
  printf("PrintFloatBits: ");
  PrintFloatBits(27.43f);
  
  return 0;
}
/* exe 1 
   return the result of x*(2^y) */
static long PowTwo(unsigned int x, unsigned int y)
{
  return (long)x<<y;
}

/* exe 2 
   check if n is power of 2
   return 1 if its power of 2
   retorn 0 if its not power of 2*/
static int IsPowTwo(unsigned int n)
{
  unsigned int check = 2;
  
  while (check <= n)
  {
    if (check == n)
    {
      return 1;
    }
    
    check <<= 1;
  }
  
  return 0;
}
/* exe 2 without loop */
static int IsPowTwoNoLoop(unsigned int n)
{
  return (n > 0 && (n & (n-1)) == 0);
}

/* exe3 
   add 1 without algebric expressions
   return num+1 */
static int AddOne(unsigned int num)
{
  int one = 1;
  
  while (one & num)
  {
    num ^= one;
    one <<= 1;
  }
  
  num ^= one;
  return num;
}

/* exe4 
   check if every variable in arr (in binary) has 3 times 1.
   print the number if it has. */
static void CheckThreeOnes(unsigned int arr[], size_t size)
{
  size_t cnt = 0;
  size_t i = 0;
  unsigned int tmp = 0;
  
  for (i = 0; i < size; ++i)
  {
    cnt = 0;
    tmp = arr[i];
    while (tmp > 0)
    {
      if ((tmp & 1) == 1)
      {
        cnt ++;
      }
      
      tmp >>= 1;
    }
    
    if (3 == cnt)
    {
      printf("%d, ", arr[i]);
    }
  }
}

/* exe5
   mirroring the byte in binary
   return the number after mirroring */
static unsigned char ByteMirrorLoop(unsigned char byte)
{
  unsigned char mirror_result = 0;
  size_t i = 0;
  
  for (i = 0; i < 8; ++i)
  {
    mirror_result <<= 1;
    mirror_result |= (byte & 1);
    byte >>= 1;
  }
  
  return mirror_result;
}
/* exe 5 without loop */
static unsigned char ByteMirrorNoLoop(unsigned char byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

/* exe6
   check if the second bit is 1, check if the sixth bit is 1.
   return if both of them are 1 */
static int CheckSecAndSixBit(unsigned char b)
{
  return ((b >> 1) & 1) & (( b >> 5) & 1);
}

/* exe6
   check if the second bit is 1, check if the sixth bit is 1.
   return if atleast one of them is 1 */
static int CheckSecOrSixBit(unsigned char b)
{
  return (((b >> 1) & 1) | (( b >> 5) & 1));
}

/* exe 6
   swaps between third and fifth bit in b
   return the number */
static int SwapThirdAndFifth(unsigned char b)
{
  unsigned char bit3 = (b >> 2) & 1;
  unsigned char bit5 = (b >> 4) & 1;
  
  unsigned char bit_diff = bit3 ^ bit5;
  
  b ^= (bit_diff << 2);
  b ^= (bit_diff << 4);
  
  return b;
}

/* exe 7 
   first 4 lsb must be zero to be  devided by 16 */
static int ClosestLowerDivisibleBySixteen(unsigned int num)
{
  return num & ~0xF;
}

/* exe 8 */
static void SwapBit(int* num1, int* num2)
{
  *num1 ^= *num2;
  *num2 ^= *num1;
  *num1 ^= *num2;
}
/* won't work always, if they point to same variable,
   we will get 0 */

/* exe 9 
   summing how much bits '1' the number has */
static unsigned int CountSetBitsLoop(unsigned int num)
{
  unsigned int cnt = 0;
  
  while (0 != num)
  {
    cnt+= (num & 1);
     num >>= 1;
  }
  
  return cnt;
}
/* exe 9 without loop
   counting bits by summing groups by 2, then 4 then 16 */
static unsigned int CountSetBitsNoLoop(unsigned int n)
{
    n = n - ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n + (n >> 4)) & 0x0F0F0F0F;
    n = n + (n >> 8);
    n = n + (n >> 16);
    return n & 0x3F;
}

/* exe 10 
   float can't be converted normally
   convefr the float to unsigned int to be able to
   convert to binary*/
static void PrintFloatBits(float x)
{
  unsigned int num = 0;
  int i = 31;
  
  memcpy(&num, &x, sizeof(float));
  
  for (i = 31; i >= 0; --i)
  {
    printf("%d", (num >> i) & 1);
    
    if (31 == i || 23 ==i)
    {
      printf(" ");
    }
  }
  
  printf("\n");
}

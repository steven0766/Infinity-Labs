#include <stdio.h>  /* printf, size_t */
#include <string.h> /* memcpy */

static void TestPowTwo();
static void TestIsPowTwo();
static void TestIsPowTwoNoLoop();
static void TestAddBit();
static void TestCheckThreeBitsOn();
static void TestByteMirrorLoop();
static void TestByteMirrorNoLoop();
static void TestIsCheckSecAndSixBit();
static void TestIsCheckSecOrSixBit();
static void TestSwapThirdAndFifth();
static void TestClosestLowerDivisibleBySixteen();
static void TestSwapVarBit();
static void TestCountSetBitsLoop();
static void TestCountSetBitsNoLoop();
static void TestPrintFloatBits();

int main()
{
    TestPowTwo();
    TestIsPowTwo();
    TestIsPowTwoNoLoop();
    TestAddBit();
    TestCheckThreeBitsOn();
    TestByteMirrorLoop();
    TestByteMirrorNoLoop();
    TestIsCheckSecAndSixBit();
    TestIsCheckSecOrSixBit();
    TestSwapThirdAndFifth();
    TestClosestLowerDivisibleBySixteen();
    TestSwapVarBit();
    TestCountSetBitsLoop();
    TestCountSetBitsNoLoop();
    TestPrintFloatBits();
  
  return 0;
}
/* exe 1 
   return the result of x*(2^y) */
long PowTwo(unsigned int x, unsigned int y)
{
  return (long)(x << y);
}

static void TestPowTwo()
{
    unsigned int x_tests[4] = {3, 2, 5, 1};
    unsigned int y_tests[4] = {4, 3, 1, 7};
    long expected[4] = {48, 16, 10, 128};
    long result = 0;
    size_t i = 0;

    printf("Testing PowTwo:\n");
    for (i = 0; i < 4; ++i)
    {
        result = PowTwo(x_tests[i], y_tests[i]);
        printf("PowTwo(%u, %u) = %ld | Expected = %ld\n",
                x_tests[i], y_tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 2 
   check if n is power of 2
   return 1 if its power of 2
   retorn 0 if its not power of 2*/
int IsPowTwo(unsigned int n)
{
  unsigned int check = 2;
  
  if (n == 1)
  {
    return 1;
  }
  
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

static void TestIsPowTwo()
{
    unsigned int tests[4] = {1, 2, 8, 7};
    int expected[4] = {1, 1, 1, 0};
    int result = 0;
    size_t i = 0;
    
    
    printf("Testing IsPowTwo:\n");
    for (i = 0; i < 4; ++i)
    {
        result = IsPowTwo(tests[i]);
        printf("IsPowTwo(%u) = %d | Expected = %d\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 2 without loop */
int IsPowTwoNoLoop(unsigned int n)
{
  return (n > 0 && (n & (n-1)) == 0);
}

static void TestIsPowTwoNoLoop()
{
    unsigned int tests[4] = {1, 16, 64, 70};
    int expected[4] = {1, 1, 1, 0};
    int result = 0;
    size_t i = 0;

    printf("Testing IsPowTwoNoLoop:\n");
    for (i = 0; i < 4; ++i)
    {
        result = IsPowTwoNoLoop(tests[i]);
        printf("IsPowTwoNoLoop(%u) = %d | Expected = %d\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe3 
   add 1 without algebric expressions
   return num+1 */
int AddBit(unsigned int num)
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
 static void TestAddBit()
{
    unsigned int tests[4] = {0, 7, 15, 100};
    int expected[4] = {1, 8, 16, 101};
    int result = 0;
    size_t i = 0;

    printf("Testing AddBit:\n");
    for (i = 0; i < 4; ++i)
    {
        result = AddBit(tests[i]);
        printf("AddBit(%u) = %d | Expected = %d\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe4 
   check if every variable in arr (in binary) has 3 times 1.
   print the number if it has. */
void CheckThreeBitsOn(unsigned int arr[], size_t size)
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
      printf("%d ", arr[i]);
    }
  }
}

static void TestCheckThreeBitsOn()
{
    unsigned int tests[4] = {7, 12, 13, 8};
    unsigned int expected[4] = {7, 13, 0, 0};
    size_t i = 0;

    printf("Testing CheckThreeBitsOn:\n");

    printf("Input:    { %u, %u, %u, %u }\n",
            tests[0], tests[1], tests[2], tests[3]);

    printf("Expected: ");
    for (i = 0; i < 4; ++i)
    {
        if (expected[i] != 0)
        {
            printf("%u ", expected[i]);
        }
    }

    printf("\nActual:   ");
    CheckThreeBitsOn(tests, 4);

    printf("\n\n");
}

/* exe5
   mirroring the byte in binary
   return the number after mirroring */
unsigned char ByteMirrorLoop(unsigned char byte)
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

static void TestByteMirrorLoop()
{
    unsigned char tests[4] = {0x01, 0x80, 0xAA, 0x0F};
    unsigned char expected[4] = {0x80, 0x01, 0x55, 0xF0};
    unsigned char result = 'a';
    size_t i = 0;
    
    printf("Testing ByteMirrorLoop:\n");
    for (i = 0; i < 4; ++i)
    {
        result = ByteMirrorLoop(tests[i]);
        printf("ByteMirrorLoop(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 5 without loop */
unsigned char ByteMirrorNoLoop(unsigned char byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

static void TestByteMirrorNoLoop()
{
    unsigned char tests[4] = {0x01, 0x80, 0xAA, 0x0F};
    unsigned char expected[4] = {0x80, 0x01, 0x55, 0xF0};
    unsigned char result = 'a';
    size_t i = 0;

    printf("Testing ByteMirrorNoLoop:\n");
    for (i = 0; i < 4; ++i)
    {
        result = ByteMirrorNoLoop(tests[i]);
        printf("ByteMirrorNoLoop(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe6
   check if the second bit is 1, check if the sixth bit is 1.
   return if both of them are 1 */
int IsCheckSecAndSixBit(unsigned char byte)
{
  return ((byte >> 1) & 1) & (( byte >> 5) & 1);
}

static void TestIsCheckSecAndSixBit()
{
    unsigned char tests[4] = {0x22, 0x02, 0x20, 0xFF};
    int expected[4] = {1, 0, 0, 1};
    int result = 0;
    size_t i = 0;

    printf("Testing IsCheckSecAndSixBit:\n");
    for (i = 0; i < 4; ++i)
    {
        result = IsCheckSecAndSixBit(tests[i]);
        printf("IsCheckSecAndSixBit(%u) = %d | Expected = %d\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe6
   check if the second bit is 1, check if the sixth bit is 1.
   return if atleast one of them is 1 */
int IsCheckSecOrSixBit(unsigned char byte)
{
  return (((byte >> 1) & 1) | ((byte >> 5) & 1));
}

static void TestIsCheckSecOrSixBit()
{
    unsigned char tests[4] = {0x22, 0x02, 0x20, 0x00};
    int expected[4] = {1, 1, 1, 0};
    int result = 0;
    size_t i = 0;

    printf("Testing IsCheckSecOrSixBit:\n");
    for (i = 0; i < 4; ++i)
    {
        result = IsCheckSecOrSixBit(tests[i]);
        printf("IsCheckSecOrSixBit(%u) = %d | Expected = %d\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 6
   swaps between third and fifth bit in b
   return the number */
int SwapThirdAndFifth(unsigned char byte)
{
  unsigned char bit3 = (byte >> 2) & 1;
  unsigned char bit5 = (byte >> 4) & 1;
  
  unsigned char bit_diff = bit3 ^ bit5;
  
  byte ^= (bit_diff << 2);
  byte ^= (bit_diff << 4);
  
  return byte;
}

static void TestSwapThirdAndFifth()
{
    unsigned char tests[4] = {0x14, 0x0C, 0x1F, 0x00};
    unsigned char expected[4];
    unsigned char result = 0;
    size_t i = 0;

    for (i = 0; i < 4; ++i)
    {
        expected[i] = SwapThirdAndFifth(tests[i]);
    }

    printf("Testing SwapThirdAndFifth:\n");
    for (i = 0; i < 4; ++i)
    {
        result = SwapThirdAndFifth(tests[i]);
        printf("SwapThirdAndFifth(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}


/* exe 7 
   first 4 lsb must be zero to be  devided by 16 */
int ClosestLowerDivisibleBySixteen(unsigned int num)
{
  return num & ~0xF;
}

static void TestClosestLowerDivisibleBySixteen()
{
    unsigned int tests[4] = {90, 16, 31, 255};
    unsigned int expected[4] = {80, 16, 16, 240};
    unsigned int result = 0;
    size_t i = 0;

    printf("Testing ClosestLowerDivisibleBySixteen:\n");
    for (i = 0; i < 4; ++i)
    {
        result = ClosestLowerDivisibleBySixteen(tests[i]);
        printf("ClosestLowerDivisibleBySixteen(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 8 
   won't work always, if they point to same variable,
   we will get 0 */
void SwapVarBit(int* num1, int* num2)
{
  *num1 ^= *num2;
  *num2 ^= *num1;
  *num1 ^= *num2;
}

static void TestSwapVarBit()
{
    int a_tests[4] = {5, 10, 100, -3};
    int b_tests[4] = {7, 20, 200, 4};
    int a = 0;
    int b = 0;
    size_t i = 0;

    printf("Testing SwapVarBit:\n");
    for (i = 0; i < 4; ++i)
    {
        a = a_tests[i];
        b = b_tests[i];
        SwapVarBit(&a, &b);
        printf("SwapVarBit(%d, %d) -> a=%d, b=%d | Expected a=%d, b=%d\n",
                a_tests[i], b_tests[i], a, b,
                b_tests[i], a_tests[i]);
    }
    printf("\n");
}

/* exe 9 
   summing how much bits '1' the number has */
unsigned int CountSetBitsLoop(unsigned int num)
{
  unsigned int cnt = 0;
  
  while (0 != num)
  {
    cnt += (num & 1);
     num >>= 1;
  }
  
  return cnt;
}

static void TestCountSetBitsLoop()
{
    unsigned int tests[4] = {27, 0, 255, 1023};
    unsigned int expected[4] = {4, 0, 8, 10};
    unsigned int result = 0;
    size_t i = 0;

    printf("Testing CountSetBitsLoop:\n");
    for (i = 0; i < 4; ++i)
    {
        result = CountSetBitsLoop(tests[i]);
        printf("CountSetBitsLoop(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 9 without loop
   counting bits by summing groups by 2, then 4 then 16 */
unsigned int CountSetBitsNoLoop(unsigned int n)
{
    n = n - ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n + (n >> 4)) & 0x0F0F0F0F;
    n = n + (n >> 8);
    n = n + (n >> 16);
    return n & 0x3F;
}

static void TestCountSetBitsNoLoop()
{
    unsigned int tests[4] = {27, 0, 255, 1023};
    unsigned int expected[4] = {4, 0, 8, 10};
    unsigned int result = 0;
    size_t i = 0;

    printf("Testing CountSetBitsNoLoop:\n");
    for (i = 0; i < 4; ++i)
    {
        result = CountSetBitsNoLoop(tests[i]);
        printf("CountSetBitsNoLoop(%u) = %u | Expected = %u\n",
                tests[i], result, expected[i]);
    }
    printf("\n");
}

/* exe 10 
   float can't be converted normally
   convefr the float to unsigned int to be able to
   convert to binary*/
void PrintFloatBits(float x)
{
  unsigned int num = 0;
  int i = 31;
  
  memcpy(&num, &x, sizeof(float));
  
  for (i = 31; i >= 0; --i)
  {
    printf("%d", (num >> i) & 1);
    
    if (31 == i || 23 == i)
    {
      printf(" ");
    }
  }
  
  printf("\n");
}

static void TestPrintFloatBits()
{
    float tests[4] = {27.43f, -5.5f, 0.75f, 100.0f};
    size_t i = 0;

    printf("Testing PrintFloatBits:\n");
    for (i = 0; i < 4; ++i)
    {
        printf("PrintFloatBits(%f): ", tests[i]);
        PrintFloatBits(tests[i]);
    }
    printf("\n");
}


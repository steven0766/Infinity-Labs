/****************************************
 *file name: ws1                        *
 *purpose: implementation of functions: *
 *1. PrintHelloWorld                    *
 *2. PowerOfTen                         *
 *3. FlipNumber                         *
 *4. Swap                               *
 *5. test functions                     *
 *   Date: 9/11/2025                    *
 ****************************************/
#include <stdio.h>    /* printf function */
#include <math.h>     /* pow function */ 

/* print "Hello World!" */
static void PrintHelloWorld()
{
    const char hexa_hello_world[] = 
    "\x22\x48\x65\x6c\x6c\x6f"
     "\x20"
     "\x57\x6f\x72\x6c\x64\x21\x22";

    printf("%s",hexa_hello_world);
}

/* calculate 10^n */
static double PowerOfTen(const int n)
{
    int i, sign, abs;
    double result = 1;

    sign = (n < 0) ? -1: 1;
    abs = n * sign;

    for (i = 0; i < abs; i++)
    {
        result *= 10;
    }

    return (sign == -1) ? 1 / result : result;
}

/* flip number (120 will be 21) */
static int Flip(int n)
{
    int fliped = 0, digit, sign;

    fliped = 0;
    sign = (n < 0) ? -1 : 1;
    n *= sign;

    while (n > 0)
    {
        digit = n % 10;
        fliped *=10;
	fliped += digit;
	n/=10;
    }

    return fliped * sign;
}

/* swap between 2 numbers */
static void Swap(int *num1_ptr, int *num2_ptr)
{
    if (num1_ptr == NULL || num2_ptr == NULL)
	return;

    *num1_ptr += *num2_ptr;
    *num2_ptr = *num1_ptr - *num2_ptr;
    *num1_ptr -= *num2_ptr;
}

/***************************************
 test functions */

static void TestPrintHelloWorld()
{
    PrintHelloWorld();
    printf("\n");
}

static void TestPowerOfTen()
{
    int num = 20, i;

    for (i = -num; i < num; i++)
    {
	if (pow(10,i) != PowerOfTen(i))
	{
	    printf("test failed for %d\n"
	    "expected %f, recieved %f\n", i, pow(10, i), PowerOfTen(i));
	    break;
	}
    }
    printf("Test success!\n");
}

static void TestFlip()
{
    int num = 120, expected = 21;
    int num2 = -340, expected2 = -43;
    
    if (expected != Flip(num))
    {
	printf("Test failed on positive number:\n"
	"expected %d, recieved %d\n", expected, Flip(num));
	return;
    }
    if (expected2 != Flip(num2))
    {
	printf("Test failed on negative number:\n"
	"expected %d, recieved %d\n", expected2, Flip(num2));
	return;
    }
    printf("Test success!\n");
}

static void TestSwap()
{
    int num1 = 12, num2 = 8;
    int num3 = -20, num4 = -55;

    Swap(&num1, &num2);
    Swap(&num3, &num4);

    if (num1 != 8 && num2 != 12)
    {
	printf("Test failed on positive numbers:\n"
        "num1 = %d, num2 = %d\n", num1, num2);
	return;
    }
    if (num3 != -55 && num4 != -20)
    {
	printf("Test failed on negative numbers:\n"
        "num1 = %d, num2 = %d\n", num1, num2);
	return;
    }
    printf("Test seccess!\n");
}

int main()
{
    TestPrintHelloWorld();
    TestPowerOfTen();
    TestFlip();
    TestSwap();
    return 0;
}

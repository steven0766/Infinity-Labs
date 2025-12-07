#include <stdio.h>    /* printf, size_t */
#include <stdlib.h>   /* atoi   */
#include <string.h>   /* strcmp */
#include <ctype.h>
int Atoi(const char* str, int base);
static void TestAtoi();
char* Itoa(int num, char* buffer, int base);
static void ReverseString(char* left, char* right);
static void TestItoa();
static int GetCharVal(char c);
void PrintCommon(const char* a1, const char* a2, const char* a3);
static void TestCommon();
int IsLittleEndian(void);
void TestEndianMacro(void);


#define IS_LITTLE_ENDIAN()   ((*(unsigned short*)"\1\2") == 0x0201)


int main()
{
    TestAtoi();
    TestItoa();
    TestCommon();
    printf("\nFunction endian:\n");
    printf("Is my system using little indian? %s\n"
           , ((IsLittleEndian()) ? "true" : "false"));
    printf("\nTest Macro:\n");
    TestEndianMacro();
    return 0;
}

int Atoi(const char* str , int base)
{
    int result = 0;
    int sign = 1;
    int digit = 0;
    
    if (base < 2 || base > 36)
    {
        return 0;
    }
    
    while ('\0' != *str && ' ' == *str)
    {
        ++str;
    }
    
    if ('-' == *str)
    {

        sign = -1;
        ++str;
    }
    else if ('+' == *str)
    {
        ++str;
    }
    
    while ('\0' != *str)
    {
        
        digit = GetCharVal(tolower(*str));
        
        if (digit >= base || digit < 0)
        {
          break;
        }
        
        result =( result * base) +  digit;
        ++str;
    }

    
    return sign * result;
}

static int GetCharVal(char c)
{
  if ( c >= '0' && c <= '9')
  {
    return c - '0';
  }
  else if (c >= 'a' && c <= 'z') 
  {
    return (c - 'a' + 10);
  }
  else
  {
    return -1;  
  }
}

static void TestAtoi(void)
{
    const char* tests[] =
    {
        "0", "   42", "-17", "+19", "    -12   15", 
        "999", "1a", "1011", "z"
    };
    size_t count = sizeof(tests) / sizeof(tests[0]);
    size_t i = 0;
    int expected = 0;
    int result = 0;

    for (i = 0; i < count; ++i)
    {
        result = atoi(tests[i]);
        expected = Atoi(tests[i], 10);

        if (result != expected)
        {
            printf("FAIL: \"%s\" system=%d, my=%d\n",
                   tests[i], result, expected);
        }
    }

    printf("Base tests:\n");
    printf("Atoi(\"1a\", 16) = %d (expected 26)\n", Atoi("1a", 16));
    printf("Atoi(\"1011\", 2) = %d (expected 11)\n", Atoi("1011", 2));
    printf("Atoi(\"z\", 36) = %d (expected 35)\n", Atoi("z", 36));
}

char* Itoa(int num, char* buffer, int base)
{
    char* buffer_start = buffer;
    int num_sign = 1;
    int digit = 0;
    
    if (base < 2 || base > 36)
    {
        *buffer = '\0';
        return buffer;
    }
    
    if (0 == num)
    {
        *buffer = '0';
        *(buffer + 1) = '\0';
        return buffer;
    }
    
    if (num < 0)
    {
        num_sign = -1;
        num *= -1;
    }

    while (num)
    {
        digit = num % base;
        
        if (10 > digit)
        {
            *buffer = '0' + digit;
        }
        else
        {
            *buffer = 'a' + (digit - 10);
        }
        num /= base;
        ++buffer;
    }
    
    if (-1 == num_sign)
    {
        *buffer = '-';
        ++buffer;
    }
    
    *buffer = '\0';
    
    ReverseString(buffer_start, buffer - 1);
    
    return buffer_start;
}

static void ReverseString(char* left, char* right)
{
    char tmp = 0;
    
    while (left < right)
    {
        tmp = *left;
        *left = *right;
        *right = tmp;
        --right;
        ++left;
    }
}

static void TestItoa(void)
{
    const char* tests[] =
    {
        "0", "5", "-5", "333", "-333", "   42",
        "   -17   99", "+19", "999", "1a"
    };

    size_t count = sizeof(tests) / sizeof(tests[0]);
    char buffer[64];
    const char* input_str = NULL;
    int my_atoi_value = 0;
    int expected_value = 0;
    int result_value = 0;
    size_t i = 0;

    for (i = 0; i < count; ++i)
    {
        input_str = tests[i];
        my_atoi_value = Atoi(input_str, 10);
        Itoa(my_atoi_value, buffer, 10);
        expected_value = atoi(input_str);
        result_value = atoi(buffer);
        
        if (expected_value != result_value)
        {
            printf("FAIL for \"%s\":\n", input_str);
            printf("expected: \"%d\", result: \"%d\"\n",
                   expected_value, result_value);
        }
    }
}




#include <stdio.h>

void PrintCommon(const char* a1, const char* a2, const char* a3)
{
    int map[256] = {0};
    int i = 0;
    
    while (*a1 != '\0')
    {
        if (map[(unsigned char)*a1] == 0)
        {
            map[(unsigned char)*a1] = 1;
        }
        ++a1;
    }

    while (*a2 != '\0')
    {
        if (map[(unsigned char)*a2] == 1)
        {
            map[(unsigned char)*a2] = 2;
        }
        ++a2;
    }

    while (*a3 != '\0')
    {
        map[(unsigned char)*a3] = -1;
        ++a3;
    }

    for (i = 0; i < 256; ++i)
    {
        if (map[i] == 2)
        {
            printf("%c ", i);
        }
    }

    printf("\n");
}

static void TestCommon()
{
    const char* arr1 = "abcdefg";
    const char* arr2 = "bdfxyz";
    const char* arr3 = "dxk";
    int i = 0;
    char result[] = "bf";

    printf("\nPrintCommon output:\n");
    PrintCommon(arr1, arr2, arr3);

    printf("expected:\n");

    for (i = 0; result[i] != '\0'; ++i)
    {
        printf("%c ", result[i]);
    }

    printf("\n");
}

int IsLittleEndian(void)
{
    int x = 1;
    return *(char*)&x;
}

void TestEndianMacro(void)
{
    if (IS_LITTLE_ENDIAN())
    {
        printf("System is little endian\n");
    }
    else
    {
        printf("System is big endian\n");
    }
}

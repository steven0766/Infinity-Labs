/************************************
 * Reviewer: Daria                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ************************************/

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

/* decleration functions */
static void TestSwap();
static void TestCopyArr();
static void exe4();
static void TestSwapTwoSizeT();
static void TestSwapTwoPointers();
static void TestIsPalindrome();
static void TestSevenBoom();
static void TestRemoveSpace();

int main()
{
  TestSwap();
  TestCopyArr();
  exe4();
  TestSwapTwoSizeT();
  TestSwapTwoPointers();
  TestIsPalindrome();
  TestSevenBoom();
  TestRemoveSpace();
  
  return 0;
}

/* swap between 2 numbers */
void Swap(int* ptr_num1, int* ptr_num2)
{
  int temp = *ptr_num1;
  
  assert(NULL != ptr_num1);
  assert(NULL != ptr_num2);
  
  *ptr_num1 = *ptr_num2;
  *ptr_num2 = temp;
}

static void TestSwap()
{
  int test = 0;
  int num1 = 32, num2 = 11;
  int num3 = -45, num4 = -8;
  
  Swap(&num1, &num2);
  Swap(&num3, &num4);
  
  if (11 != num1 && 32 != num2)
  {
    printf("Test failed: "
    "num1 is %d, num2 is %d\n", num1, num2);
    test = 1;
  }
  if (-8 != num3 && -45 != num4)
  {
    printf("Test failed: "
    "num3 is %d, num4 is %d\n", num3, num4);
    test = 1;
  }
  if (0 == test)
    printf("Test success: TestSwap\n");
}

/* copy variables from one array to another one */
void CopyArr(int* from_arr, int* to_arr, size_t size)
{
  size_t i = 0;
  for (i = 0; i < size; i++)
    to_arr[i] = from_arr[i];
}

static void TestCopyArr()
{
  int copy_from[] = {1, 2, 3, 4, 5}, copy_to[5];
  size_t size = 5, i = 0;
  
  CopyArr(copy_from, copy_to, size);
  
  for (i = 0; i < sizeof(copy_from) / sizeof(int); i++)
  {
    if (copy_from[i] != copy_to[i])
    {
      printf("not equal in index %d\n", i);
      return;
    }
  }
  printf("Test success: TestCopyArr\n");
}

static void exe4()
{
  static int s_i = 7;
  int* ptr = &s_i;
  int i = 7;
  int* ptr2 = &i;
  int** ptr3 = &ptr2;
  int* ptr4 = (int *)malloc(sizeof(int));

  printf("ptr3: %p\n", ptr3);
  printf("ptr2: %p\n", ptr2);
  printf("ptr: %p\n", ptr);
  printf("s_i: %p\n", &i);
  printf("ptr4: %p\n", ptr4);
  printf("i: %p\n", &s_i);
  free(ptr4); 
}

void SwapTwoSizeT(size_t* size_t1, size_t* size_t2)
{
  size_t temp = *size_t1;
  *size_t1 = *size_t2;
  *size_t2 = temp;
}

static void TestSwapTwoSizeT()
{
  size_t size1 = 500, size2 = 800;
  
  SwapTwoSizeT(&size1, &size2);
  
  if (800 != size1 || 500 != size2)
  {
    printf("Error:\n"
    "size1 is %lu, size2 is %lu\n", size1, size2);
    return;
  }
  printf("Test success: TestSwapTwoSizeT\n");
}

void SwapTwoPointers(size_t** size_t1, size_t** size_t2)
{
  SwapTwoSizeT((size_t*)size_t1, (size_t*)size_t2);
}

static void TestSwapTwoPointers()
{
  size_t size3 = 120, size4 = 210;
  size_t* size31 = &size3, *size41 = &size4;
  
  SwapTwoPointers(&size31, &size41);
  if (*size31 != 210 && *size41 != 120)
  {
    printf("Error:\n"
    "size31 is %lu, size41 is %lu\n", *size31, *size41);
    return;
  }
  printf("Test success: TestSwapTwoPointers\n");
}

int Palindrome(const char* str)
{
  const char* left = str;
  const char* right = str;
  
  assert(NULL != str);
  
  while ('\0' != *right)
  {
    ++right;
  }
  
  --right;
  
  while (right > left)
  {
    if(*right != *left)
    {
      return 0;
    }
    
    --right;
    ++left;
  }
  
  return 1;
}

static int ContainSeven(int num)
{
  if (num < 0)
  {
    num = -num;
  }
  
  while (num > 0)
  {
    if (7 == num % 10)
    {
      return 1;
    }
    
    num/=10;
  }
  
  return 0;
}

void SevenBoom(int start, int end)
{
  int i = 0;
  
  for (i = start; i < end; ++i)
  {
    if (0 == i % 7 || ContainSeven(i))
    {
      printf("BOOM\n");
    }
    else
    {
      printf("%d", i);
    }
  }
}

static int IsSpaceOrTab(char c)
{
  return (' ' == c || '\t' == c);
}

void RemoveSpace(char* str)
{
  char* read = str;
  char* corrected_str = str;
  
  int space_seq = 0;
  
  while  ('\0' != *read && IsSpaceOrTab(*read))
  {
    ++read;
  }
  
  while ('\0' != *read)
  {
    if (IsSpaceOrTab(*read))
    {
      if (0 == space_seq)
      {
        space_seq = 1;
        *corrected_str = *read;
        ++corrected_str;
      }
    }
    else
    {
      *corrected_str = *read;
      space_seq = 0;
      ++corrected_str;
    }
    
    ++read;
  }
  
  while (corrected_str > str && IsSpaceOrTab(*(corrected_str - 1)))
  {
    --corrected_str;
  }
  
  *corrected_str = '\0';
}

static void TestIsPalindrome()
{
    const char* inputs[] = {"", "aa", "ab", "abcba", "12321", "holla", "   ", "\t\t"};
    int expected[] = {1, 1, 0, 1, 1, 0, 1, 1};

    size_t i = 0;

    for (i = 0; i < (sizeof(inputs) / sizeof(inputs[0])); ++i)
    {
        int result = Palindrome(inputs[i]);

        if (result != expected[i])
        {
            printf("Error: got %d, expected %d\n", inputs[i], expected[i]);
            return;
        }
    }

    printf("Test Success: Palindrome\n");
}

static void TestSevenBoom()
{
  SevenBoom(5,100);
}

static void TestRemoveSpace()
{
  char* inputs[] = {"    aaa  bbb  ", "asdf", "   dsdd", "sadd   ", "\t  adsw"};
  char* expected[] = {"aaa bbb", "asdf", "dsdd", "sadd", "adsw"};
  
  size_t total_inputs = sizeof(inputs) / sizeof(inputs[0]), i = 0;
  
  char buffer[100];
  
  for (i = 0; i < total_inputs; ++i)
  {
    strcpy(buffer, inputs[i]);
    
    RemoveSpace(buffer);
    
    if (0 != strcmp(buffer, expected[i]))
    {
      printf("Error:\n"
      "expected: %s, got: %s\n", expected[i], buffer);
      return;
    }
  }
  printf("Test success: RemoveSpace\n");
}

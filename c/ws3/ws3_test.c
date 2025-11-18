#include <stdio.h>
#include "ws3.h"

static void TestSumOfArrayRows();
static void TestJosephusProblem();
static void TestDataTypes();
static void TestPrintEvnp(char** envp);

int main(int argc, char *argv[], char * envp[])
{
  TestSumOfArrayRows();
  TestJosephusProblem();
  TestDataTypes();
  TestPrintEvnp(envp);
  
  return 0;
}

static void TestSumOfArrayRows()
{
  int inputs[3][4] = {{1, 2, 3, 4}, {3, 4, 5, 6}, {5, 6, 7, 8}};
  int expected[3] = {10, 18, 26};
  int result[3] = {0};
  size_t i = 0;
  
  SumOfArrayRows(inputs, 3, result);
  
  for (i = 0; i < 3; ++i)
  {
    if (expected[i] != result[i])
    {
      printf("result: %d, expected: %d\n", result[i], expected[i]);
    }
  }
}

static void TestJosephusProblem()
{
  int test_result[5] = {2, 4, 15, 25, 30};
  int expected[5] = {0, 0, 14, 18, 28};
  int i = 0;
  
  for (i = 0; i < 5; ++i)
  {
    if (JosephusProblem(test_result[i]) != expected[i])
      {
        printf("Error:\n"
        "got: %d, expected: %d\n",
        JosephusProblem(test_result[i]), expected[i]);
      }
  }
}

static void TestDataTypes()
{
  char** buffer;
  DataTypes();
}

static void TestPrintEvnp(char** envp)
{
  PrintEvnp(envp);
}

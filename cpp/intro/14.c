#include <stdio.h>

const int g_const = 10;
int g_global = 14;

int main()
{
  const int i = 3;
  int j = 2;
  int* ip = (int*)&i;
  *ip = 5;
  int arr[i];

  printf("%d %d\n", i, *ip);
  return 0;
}
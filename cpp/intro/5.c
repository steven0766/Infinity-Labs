#include <stdio.h>
#include <string.h>

int main()
{
  int res = add(3, 5);
  char buffer[10] = {0};
  printf("gravity: %f\n", get_g());
  fill_buffer(buffer, 10);
  printf("buffer: %s\n", buffer);

  return 0;
}

int add(char a, int b)
{
  return a + b;
}

double get_g()
{
  return 10;
}

void fill_buffer(char* buffer, int size)
{
  memset(buffer, 'S', size - 1);
  buffer[size - 1] = '\0';
}
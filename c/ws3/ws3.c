#include <stddef.h> /* NULL, size_t */
#include <string.h> /* strlen */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, calloc, free */
#include <ctype.h> /* tolower */

void SumOfArrayRows(int (*matrix)[4], size_t rows, int* dest)
{
  size_t i = 0;
  size_t j = 0;
  
  for (i = 0; i < rows; ++i)
  {
    dest[i] = 0;
    
    for(j = 0; j < 4; j++)
    {
      dest[i] += matrix[i][j];
    }
  }
}

int JosephusProblem(size_t soldiers_size)
{
  size_t count = soldiers_size;
  size_t sword_idx = 0;
  /* 0 find who to kill, 1 find who to give sword to */
  size_t kill_or_sword = 0; 
  size_t* soldiers = (size_t*)calloc(soldiers_size, sizeof(size_t));
  size_t idx = 0;
  
  if (NULL == soldiers)
  {
    return 0;
  }
  
  while (count > 1)
  {
      ++idx;
      if ((0 == soldiers[idx % soldiers_size]))
      {
        if ((0 == kill_or_sword))
        {
          soldiers[idx % soldiers_size] = 1;
          --count;
          kill_or_sword = 1;
        }
        else
        {
          sword_idx = idx % soldiers_size;
          kill_or_sword = 0;
        }
      }
  }
  
  free(soldiers);
  soldiers = NULL;
  return (0 == soldiers_size) ? 0 : (sword_idx + 1);
}

void DataTypes()
{
  printf("int: %zu\n", sizeof(int));
  printf("signed: %zu\n",sizeof(signed));
  printf("signed int: %zu\n",sizeof(signed int));
  printf("unsigned: %zu\n",sizeof(unsigned));
  printf("unsigned int: %zu\n",sizeof(unsigned int));
  printf("float: %zu\n", sizeof(float));
  printf("double: %zu\n", sizeof(double));
  printf("long double: %zu\n",sizeof(long double));
  printf("char: %zu\n", sizeof(char));
  printf("signed char: %zu\n", sizeof(signed char));
  printf("unsigned char: %zu\n", sizeof(unsigned char));
  printf("short: %zu\n",sizeof(short));
  printf("short int: %zu\n", sizeof(short int));
  printf("signed short: %zu\n", sizeof(signed short));
  printf("signed short int: %zu\n", sizeof(signed short int));
  printf("unsigned short: %zu\n", sizeof(unsigned short));
  printf("unsigned short int: %zu\n", sizeof(unsigned short int));
  printf("long: %zu\n", sizeof(long));
  printf("long int: %zu\n", sizeof(long int));
}

size_t CountEnvp(char** envp)
{
  size_t cnt = 0;
  while (NULL != envp[cnt])
  {
    ++cnt;
  }
  
  return cnt;
}
void ToLower(char* envp)
{
  while ('\0' != *envp)
  {
    *envp = tolower(*envp);
    ++envp;
  }
}

char* CopyEnvToStr(char* envp)
{
  size_t cnt = strlen(envp);
  char* envp_lower = (char*)malloc((cnt + 1) * sizeof(char));
  
  if (NULL == envp_lower)
  {
    return NULL;
  }
  
  strcpy(envp_lower, envp);
  ToLower(envp_lower);
  return envp_lower;
}

void PrintEnvp(char** envp)
{
  size_t cnt_envp = CountEnvp(envp);
  size_t cur_value = 0;
  char** lower_case_envp = (char**)malloc(cnt_envp * sizeof(char*));
  size_t i = 0;
  
  if (NULL == lower_case_envp)
  {
    return;
  }
  
  while (*(envp + cur_value) != NULL)
  {
    *(lower_case_envp + cur_value) = CopyEnvToStr(*(envp + cur_value));
    
    if (NULL == *(lower_case_envp + cur_value))
    {
      cnt_envp = cur_value + 1;
      break;
    }
    
    printf("%s\n", lower_case_envp[cur_value]);
    ++cur_value;
  }
  
  for (i = 0; i < cnt_envp; ++i)
  {
    free(lower_case_envp[i]);
    lower_case_envp[i] = NULL;
  }
  
  free(lower_case_envp);
  lower_case_envp = NULL;
} 

/*************************************
* Reviewer: nimrod                   *
* Date: 22.11.25                     *
* Version: 1                         *
**************************************/

#include <stdio.h>    /* printf */
#include <string.h>   /*strcmp  */

typedef void (*PtrFunction)(int);
void Print(int print);
void StructPractice();
void Logger(char* file_name);

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    StructPractice();
  }
  else
  {
    Logger(argv[1]);
  }
  
  return 0;
}

  /* exercise 1 */
void StructPractice()
{
  typedef struct print_me
  {
    int a;
    PtrFunction ptr;
  }print_me;

  print_me arr[10];
  size_t j = 0;
  
  for(j = 0; j < 10; ++j)
  {
    arr[j].a = j;
    arr[j].ptr = Print;
    arr[j].ptr(arr[j].a);
  }
}

void Print(int print)
{
  printf("%d\n", print);
}

  /* exercise 2 */
typedef enum {OK, ERROR, EXIT} op_handler;
  
typedef struct
{
  char* cmd_name;
  int (*compare)(const char* input, const char* command);
  op_handler (*operation)(const char* input, const char* filename);
}handler;

op_handler Remove(const char* input, const char* filename)
{
  if (0 == remove(filename))
  {
    return EXIT;
  }
  else
  {
    return ERROR;
  }
}

op_handler Count(const char* input, const char* filename)
{
  int cnt = 0;
  char line[100];
  FILE* fptr = fopen(filename, "r");
  
  if (!fptr)
  {
    return ERROR;
  }
  
  while (NULL != fgets(line, sizeof(line), fptr))
  {
    ++cnt;
  }
  
  fclose(fptr);
  printf("%d\n", cnt);
  
  return OK;
}

op_handler Exit(const char* input, const char* filename)
{
  return EXIT;
}

int CompareCommand(const char* input, const char* command)
{
  return 0 == strcmp(input, command);
}

op_handler Prepend(const char* input, const char* filename)
{
  FILE *file_old;
  FILE *file_new;
  char line[1024];
  const char *content;
  
  content = input + 1;
  file_old = fopen(filename, "r");
  
  if (NULL == file_old)
  {
    file_new = fopen(filename, "w");
    if (NULL == file_new)
    {    
      return ERROR;
    }
    fputs(content, file_new);
    fputc('\n', file_new);
    fclose(file_new);
    return OK;
  }

  file_new = fopen("tmp_file.txt", "w");
  
  if (NULL == file_new)
  {
    fclose(file_old);
    return ERROR;
  }

  fputs(content, file_new);
  fputc('\n', file_new);

  while (NULL != fgets(line, sizeof(line), file_old))
  {
    fputs(line, file_new);
  }

  fclose(file_old);
  fclose(file_new);
  remove(filename);
  rename("tmp_file.txt", filename);

  return OK;
}

int ComparePrepend(const char* input, const char* command)
{
  return input[0] == command[0];
}

void Logger(char* file_name)
{
  FILE* file_ptr;
  char input[100];
  op_handler state = OK;
  size_t append_flag = 1;
  size_t i = 0;
  
  handler command_lut[] = 
  {
    {"-remove", CompareCommand, Remove},
    {"-count", CompareCommand, Count},
    {"-exit", CompareCommand, Exit},
    {"<", ComparePrepend, Prepend}
  };
  
  while (EXIT != state)
  {
    fgets(input, sizeof(input), stdin);
    append_flag = 1;
    input[strcspn(input, "\n")] = '\0';
    
    for (i = 0; i < sizeof(command_lut) / sizeof(handler); ++i)
    {
      if (command_lut[i].compare(input, command_lut[i].cmd_name))
      {
        state = command_lut[i].operation(input, file_name);
        append_flag = 0;
        break;
      }
    }
    
    if (1 == append_flag)
    {
      file_ptr = fopen(file_name, "a");
      if(file_ptr)
      {
        fputs(input, file_ptr);
        fputs("\n", file_ptr);
        fclose(file_ptr);
      }
    }
  }
}

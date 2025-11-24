/*************************************
* Reviewer: nimrod                   *
* Date: 22.11.25                     *
* Version: 1                         *
**************************************/

#include <stdio.h>    /* printf */
#include <string.h>   /*strcmp, strcspn */

typedef void (*PtrFunction)(int);
#define UNUSED(x) (void)(x);
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
  }print_me_ty;

  print_me_ty arr[10] = {0};
  size_t j = 0;
  
  for(j = 0; j < 10; ++j)
  {
    arr[j].a = j;
    arr[j].ptr = Print;
    arr[j].ptr(arr[j].a);
  }
}

/* print number */
void Print(int print)
{
  printf("%d\n", print);
}

  /* exercise 2 */
typedef enum {OK, ERROR, EXIT} op_handler_ty;

typedef struct
{
  char* cmd_name;
  int (*compare_func_ty)(const char* input, const char* command);
  op_handler_ty (*operation_func_ty)(const char* input, const char* filename);
}handler_ty;

/* removing file */
op_handler_ty Remove(const char* input, const char* filename)
{
  UNUSED(input);
  if (0 == remove(filename))
  {
    return EXIT;
  }
  else
  {
    printf("Exit Error: didn't exit\n");
    return ERROR;
  }
}

/* count how much lines on file */
op_handler_ty Count(const char* input, const char* filename)
{
  int cnt = 0;
  char line[100];
  FILE* fptr = NULL;
  
  UNUSED(input);
  
  fptr = fopen(filename, "r");

  if (!fptr)
  {
    printf("Count Error: can't open the file\n");
    return ERROR;
  }
  
  while (NULL != fgets(line, sizeof(line), fptr))
  {
    ++cnt;
  }
  
  if (!(0 == fclose(fptr)))
  {
    printf("Count Error: can't close the file\n");
    return ERROR;
  }
  printf("%d\n", cnt);
  
  return OK;
}

/* exit the program */
op_handler_ty Exit(const char* input, const char* filename)
{
  UNUSED(input);
  UNUSED(filename);
  return EXIT;
}

/* check if input is command (count, exit or remove) */
int CompareCommand(const char* input, const char* command)
{
  return 0 == strcmp(input, command);
}

/* append the input to the first life of the file */
op_handler_ty Prepend(const char* input, const char* filename)
{
  FILE *file_old;
  FILE *file_new;
  char line[100];
  const char *content;
  
  content = input + 1;
  file_old = fopen(filename, "r");
  
  if (NULL == file_old)
  {
    file_new = fopen(filename, "w");
    if (NULL == file_new)
    {    
      printf("Prepend Error: cant open new or old file\n");
      return ERROR;
    }
    fputs(content, file_new);
    fputc('\n', file_new);
    if (!(0 == fclose(file_new)))
    {
      printf("Prepend Error: can't close old file with 1 sentence\n");
      return ERROR;
    }
    return OK;
  }

  file_new = fopen("tmp_file.txt", "w");
  
  if (NULL == file_new)
  {
    if (!(0 == fclose(file_old)))
    {
      printf("Prepend Error: can't close old file\n");
      return ERROR;
    }
    printf("Prepend Error: can't open new empty file\n");
    return ERROR;
  }

  fputs(content, file_new);
  fputc('\n', file_new);

  while (NULL != fgets(line, sizeof(line), file_old))
  {
    fputs(line, file_new);
  }

  if (!(0 == fclose(file_old)))
  {
    printf("Prepend Error: can't close old empty file\n");
    return ERROR;
  }
  if (!(0 == fclose(file_new)))
  {
    printf("Prepend Error: can't close updated new file\n");
    return ERROR;
  }
  if (!(0 == remove(filename)))
  {
    printf("Prepend Error: can't delete old file to switch to new file");
    return ERROR;
  }
  if (!(0 == rename("tmp_file.txt", filename)))
  {
    printf("Prepend Error: can't change file name to the old one");
    return ERROR;
  }

  return OK;
}

/* check if the input start with "<" */
int ComparePrepend(const char* input, const char* command)
{
  return input[0] == command[0];
}

/* append the input to the first line of the line */
op_handler_ty Append(const char* input, const char* filename)
{
  FILE* file_ptr;
  file_ptr = fopen(filename, "a");
  if(!file_ptr)
  {
    printf("Append Error: can't open file\n");
    return ERROR;
  }
  fputs(input, file_ptr);
  fputs("\n", file_ptr);
  if (!(0 == fclose(file_ptr)))
  {
    printf("Append Error: can't close file\n");
    return ERROR;
  }
  return OK;
}
/* the last command will always return true to active the function */
int CompareAppend(const char* input, const char* command)
{
  UNUSED(input);
  UNUSED(command);
  return 1;
}

/* main logger function.
   for every input the logger will search through commands
   to find the right operation. 
   will exit when the input is "-exit". */ 
void Logger(char* file_name)
{
  char input[100];
  op_handler_ty state = OK;
  size_t i = 0;
  
  handler_ty command_lut[] = 
  {
    {"-remove", CompareCommand, Remove},
    {"-count", CompareCommand, Count},
    {"-exit", CompareCommand, Exit},
    {"<", ComparePrepend, Prepend},
    {" ", CompareAppend, Append}
  };
  
  while (EXIT != state)
  {
    if (NULL == fgets(input, sizeof(input), stdin))
    {
      continue;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    for (i = 0; i < sizeof(command_lut) / sizeof(handler_ty); ++i)
    {
      if (command_lut[i].compare_func_ty(input, command_lut[i].cmd_name))
      {
        state = command_lut[i].operation_func_ty(input, file_name);
        break;
      }
    }
  }
}

/*************************************
* Reviewer: Rozaline                 *
* Date: 19.11.25                     *
* Version: 1                         *
**************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* system */

typedef void (*PtrFunction)(void);

/* printing A-pressed or T-pressed without type print
   using "if else" statement*/
void IfKeyboardInput();

/* printing A-pressed or T-pressed without type print
   using "switch" statement*/
void SwitchKeyboardInput();

/* printing A-pressed or T-pressed without type print
   using look-up-table */
void LutKeyboardInput();

int main()
{
  printf("IfKeyboardInput:\n"); 
  IfKeyboardInput();
  
  printf("SwitchKeyboardInput:\n"); 
  SwitchKeyboardInput();
  
  printf("LutKeyboardInput:\n"); 
  LutKeyboardInput();
  
  return 0;
}

void IfKeyboardInput()
{
  char letter = '\0';
  
  system("stty -icanon -echo");
  
  while (27 != letter)
  {
    letter = getchar();
    
    if ('a' == letter)
    {
      printf("A pressed\n");
    }
    else if('t' == letter)
    {
      printf("T pressed\n");
    }
    else
    {
      continue;
    }
  }
  
  system("stty icanon echo");
}

void SwitchKeyboardInput()
{
  char letter = '\0';
  
  system("stty -icanon -echo");
  
  while (27 != letter)
  {
    letter = getchar();
    
    switch(letter)
    {
      case 'a':
        printf("A pressed\n");
        break;
      case 't':
        printf("T pressed\n");
        break;
      default:
        break;
    }
  }
  system("stty icanon echo");
}

void PrintA(void)
{
  printf("A-Pressed\n");
}

void PrintT(void)
{
  printf("T-Pressed\n");
}

void Empty(void)
{
  return;
}

void LutKeyboardInput()
{
  PtrFunction lut[256];
  char letter = '\0';
  size_t i = 0;
  
  for (i = 0; i < 256; ++i)
  {
    lut[i] = Empty;
  }
  
  lut['A'] = PrintA;
  lut['a'] = PrintA;
  lut['T'] = PrintT;
  lut['t'] = PrintT;
  
  system("stty -icanon -echo");
  while (27 != letter)
  {
    letter = getchar();
    lut[letter]();
  }
  system("stty icanon echo");
}



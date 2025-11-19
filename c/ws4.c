/*************************************
* Reviewer: shir                     *
* Date: 19.11.25                     *
* Version: 1                         *
**************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* system */

typedef void (*PtrFunction)(void);

/* printing A-pressed or T-pressed without type print
   using "if else" statement*/
void IfKeyboardInputPrint();

/* printing A-pressed or T-pressed without type print
   using "switch" statement*/
void SwitchKeyboardInputPrint();

/* printing A-pressed or T-pressed without type print
   using look-up-table */
void LutKeyboardInputPrint();

/* help functions for LutKeyboardInput */
void PrintA(void);
void PrintT(void);
void Empty(void);

int main()
{
  printf("IfKeyboardInputPrint:\n"); 
  IfKeyboardInputPrint();
  
  printf("SwitchKeyboardInputPrint:\n"); 
  SwitchKeyboardInputPrint();
  
  printf("LutKeyboardInputPrint:\n"); 
  LutKeyboardInputPrint();
  
  return 0;
}

void IfKeyboardInputPrint(void)
{
  char letter = '\0';
  
  system("stty -icanon -echo");
  
  while (27 != letter)
  {
    letter = getchar();
    
    if ('A' == letter)
    {
      printf("A pressed\n");
    }
    else if('T' == letter)
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

void SwitchKeyboardInputPrint(void)
{
  char letter = '\0';
  
  system("stty -icanon -echo");
  
  while (27 != letter)
  {
    letter = getchar();
    
    switch(letter)
    {
      case 'A':
        printf("A pressed\n");
        break;
      case 'T':
        printf("T pressed\n");
        break;
      default:
        break;
    }
  }
  system("stty icanon echo");
}

void LutKeyboardInputPrint(void)
{
  PtrFunction lut[256];
  char letter = '\0';
  size_t i = 0;
  
  for (i = 0; i < 256; ++i)
  {
    lut[i] = Empty;
  }
  
  lut['A'] = PrintA;
  lut['T'] = PrintT;
  
  system("stty -icanon -echo");
  while (27 != letter)
  {
    letter = getchar();
    lut[letter]();
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

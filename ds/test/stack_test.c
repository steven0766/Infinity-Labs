/***********************************************************************
*File_name: stack_test.c
*Reviewer: Nimrod
*Version:  1.0
*Date:	   10.12.25
************************************************************************/


#include <stdio.h>    /* printf */

#include "../include/stack.h"


static void TestStackCreate(void);
static void TestStackPop(void);
static void TestStackPushAndPeek(void);
static void TestStackSize(void);
static void TestStackCapacity(void);

static void PrintError(char* msg);


int main()
{
    TestStackCreate();
    TestStackPushAndPeek();
    TestStackSize();
    TestStackPop();
    TestStackCapacity();
    
    return 0;
}


static void TestStackCreate(void)
{
    size_t stack_capacity = 3; 
    stack_ty* stk = StackCreate(stack_capacity, sizeof(size_t));
    
    if (NULL == stk)
    {
        PrintError("StackCreate");
    }

    if (!StackIsEmpty(stk))
    {
        PrintError("StackCreate, StackIsEmpty");
    }
    
    StackDestroy(stk);
}

static void TestStackPop(void)
{
    size_t stack_capacity = 3;
    size_t x = 2, y = 4;
    stack_ty* stk = StackCreate(stack_capacity, sizeof(size_t));
    
    if (NULL == stk)
    {
        PrintError("StackCreate");
    }
    
    StackPush(stk, &x);
    StackPush(stk, &y);
    StackPop(stk);
    
    if (*(size_t*)(StackPeek(stk)) != x)
    {
        PrintError("TestStackPop");
    }
    
    StackDestroy(stk);
}

static void TestStackPushAndPeek(void)
{
    size_t stack_capacity = 3;
    size_t x = 2, y = 4;
    stack_ty* stk = StackCreate(stack_capacity, sizeof(size_t));
    
    if (NULL == stk)
    {
        PrintError("Push&peek, StackCreate");
    }
    
    StackPush(stk, &x);
    StackPush(stk, &y);
    
    if (StackIsEmpty(stk))
    {
        PrintError("Push&peek, StackIsEmpty");
    }

    if (*(size_t*)(StackPeek(stk)) != y)
    {
        PrintError("StackPeek");
    } 
    
    StackDestroy(stk);
}

static void TestStackSize()
{
    size_t stack_capacity = 3;
    size_t x = 2;
    stack_ty* stk = StackCreate(stack_capacity, sizeof(size_t));
    
    if (NULL == stk)
    {
        PrintError("TestStackSize, StackCreate");
    }
    
    StackPush(stk, &x);
    
    if (1 != StackSize(stk))
    {
        PrintError("StackSize");
    }
    
    StackDestroy(stk);
}

static void TestStackCapacity(void)
{
    size_t stack_capacity = 3;
    stack_ty* stk = StackCreate(stack_capacity, sizeof(size_t));
    
    if (NULL == stk)
    {
        PrintError("TestStackCapacity, StackCreate");
    }
    
    if (stack_capacity != StackCapacity(stk))
    {
        PrintError("StackCapacity");
    }
    
    StackDestroy(stk);
}

static void PrintError(char* msg)
{
    printf("%s failed, try again!", msg);
}

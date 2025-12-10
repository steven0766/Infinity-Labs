/***********************************************************************
*File_name: stack.c
*Reviewer: Nimrod
*Version:  1.0
*Date:	   10.12.25
************************************************************************/


#include <stdio.h>

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
    stack_ty* stk = StackCreate(stack_capacity, ele_size);
    
    if (NULL == stk)
    {
        PrintError("StackCreate");
    }

    if (!StackIsEmpty(stk))
    {
        PrintError("StackCreate, StackIsEmpty");
    }
    
    DestroyStack(stk);
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
    
    DestroyStack(stk);
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
    
    DestroyStack(stk);
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
    
    DestroyStack(stk);
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
    
    DestroyStack(stk);
}

static void PrintError(char* msg)
{
    printf("%s failed, try again!", msg);
}

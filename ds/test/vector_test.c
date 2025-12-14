/***********************************************************************
*File_name: vector_test.c
*Reviewer: shir
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stdio.h> /* printf, size_t */

#include "../include/vector.h"


static void TestVectorCreate();
static void TestVectorPushBackAndGetAccess();
static void TestVectorPopBack();
static void TestVectorSize();
static void TestVectorCapacity();
static void TestVectorShrinkToFit();
static void TestVectorReserve();

static void Print(char* msg);


int main()
{
    TestVectorCreate();
    TestVectorPushBackAndGetAccess();
    TestVectorPopBack();
    TestVectorSize();
    TestVectorCapacity();
    TestVectorShrinkToFit();
    TestVectorReserve();
    
    return 0;
}


static void TestVectorCreate()
{
    size_t capacity = 3;
    size_t ele_size = 4;
    vector_ty* vector = VectorCreate(capacity, ele_size);
    
    if (NULL == vector)
    {
        Print("VectorCreate");
    }
    
    VectorDestroy(vector);
}

static void TestVectorPushBackAndGetAccess()
{
    int num1 = 1, num2 = 2, num3 = 3, num4 = 4;
    size_t i = 0;
    vector_ty* vector = VectorCreate(3, sizeof(int));
    
    if (NULL == vector)
    {
        Print("VectorCreate");
    }
    
    VectorPushBack(vector, &num1);
    VectorPushBack(vector, &num2);
    VectorPushBack(vector, &num3);
    VectorPushBack(vector, &num4);
    
    printf("print every value in vector:\n");
    
    for (i = 0; i < VectorSize(vector); ++i)
    {
        printf("%d\n", *(int*)VectorGetAccessToElement(vector, i));
    }
    printf("\n");
}

static void TestVectorPopBack()
{
    int num1 = 1, num2 = 2, num3 = 3;
    vector_ty* vector = VectorCreate(3, sizeof(int));
    
    if (NULL == vector)
    {
        Print("VectorCreate");
    }
    
    VectorPushBack(vector, &num1);
    VectorPushBack(vector, &num2);
    VectorPushBack(vector, &num3);
    
    printf("TestVectorPopBack before pop function:\n");
    printf("%d\n", *(int*)VectorGetAccessToElement(vector, VectorSize(vector)) - 1);
    
    VectorPopBack(vector);
    
    printf("TestVectorPopBack after pop function:\n");
    printf("%d\n", *(int*)VectorGetAccessToElement(vector, VectorSize(vector)) - 1);
    printf("\n");
}

static void TestVectorSize()
{
    size_t capacity = 3;
    int num1 = 1;
    
    vector_ty* vector = VectorCreate(capacity, sizeof(int));
    
    if (NULL == vector)
    {
        Print("TestVectorSize -> VectorCreate");
    }
    
    if (VectorSize(vector) != 0)
    {
        Print("TestVectorSize -> VectorSize");
    }
    
    VectorPushBack(vector, &num1);
    
    if (VectorSize(vector) != 1)
    {
        Print("TestVectorSize -> VectorSize");
    }
    
    VectorDestroy(vector);
}

static void TestVectorCapacity()
{
    size_t capacity = 3;
    int num1 = 1, num2 = 2, num3 = 3, num4 = 4;
    vector_ty* vector = VectorCreate(capacity, sizeof(int));
    
    if (NULL == vector)
    {
        Print("TestVectorCapacity -> VectorCreate");
    }
    
    if (3 != VectorCapacity(vector))
    {
        Print("TestVectorCapacity -> VectorCapacity");
    }
    
    VectorPushBack(vector, &num1);
    VectorPushBack(vector, &num2);
    VectorPushBack(vector, &num3);
    VectorPushBack(vector, &num4);
    
    /* capacity now is 6(growth factor is 2 so capacity * 2 = 6) */
    if (6 != VectorCapacity(vector))
    {
        Print("TestVectorCapacity -> VectorCapacity");
    }
}

static void TestVectorShrinkToFit()
{
    size_t capacity = 3;
    int num1 = 1, num2 = 2, num3 = 3, num4 = 4;
    vector_ty* vector = VectorCreate(capacity, sizeof(int));
    
    if (NULL == vector)
    {
        Print("TestVectorCapacity -> VectorCreate");
    }
    
    VectorPushBack(vector, &num1);
    VectorPushBack(vector, &num2);
    VectorPushBack(vector, &num3);
    VectorPushBack(vector, &num4);
    
    printf("vector capacity before shrink is: %ld\n", VectorCapacity(vector));
    
    VectorShrinkToFit(vector);
    
    printf("vector capacity after shrink is: %ld\n\n", VectorCapacity(vector));
}

static void TestVectorReserve()
{
    size_t capacity = 3;
    size_t new_capacity = 10;
    vector_ty* vector = VectorCreate(capacity, sizeof(int));
    
    if (NULL == vector)
    {
        Print("TestVectorCapacity -> VectorCreate");
    }
    
    printf("capacity before VectorReserve: %ld\n", VectorCapacity(vector));
    
    VectorReserve(vector, new_capacity);
    
    printf("capacity after VectorReserve: %ld\n\n", VectorCapacity(vector));
}

static void Print(char* msg)
{
    printf("houston we have a problem in \"%s\" function!\n", msg);
}

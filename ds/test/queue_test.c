/***********************************************************************
*File_name: queue_test.c
*Reviewer: itai
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stdio.h> /*  prinf */

#include "queue.h"


static void TestQueueCreate(void);
static void TestQueueEnqueueAndPeek(void);
static void TestQueueDequeue(void);
static void TestQueueSize(void);

int main()
{
    TestQueueCreate();
    TestQueueEnqueueAndPeek(); 
    TestQueueDequeue();
    TestQueueSize();
    
    return 0;
}

static void TestQueueCreate(void)
{
    queue_ty* queue = QueueCreate();
    
    if (NULL == queue)
    {
        printf("problem in QueueCreate\n");
    }

    if(1 != QueueIsEmpty(queue))
    {
        printf("queue isn't empty!\n");
    }
    
    if (0 != QueueSize(queue))
    {
        printf("queue size isn't 0\n");
    }
    
    QueueDestroy(queue);
}

static void TestQueueEnqueueAndPeek(void)
{
    queue_ty* queue = QueueCreate();
    int expected = 1, b = 2, c = 3, actual = 0;
    
    QueueEnqueue(queue, &expected);
    QueueEnqueue(queue, &b);
    QueueEnqueue(queue, &c);
    
    actual = *(int*)QueuePeek(queue);

    if (expected != actual)
    {
        printf("1. wrong number has been found!\n");
    }  
    
    if (3 != QueueSize(queue))
    {
        printf("queue size isn't match actual siz\n");
    }
    
    QueueDestroy(queue);
}

static void TestQueueDequeue(void)
{
    queue_ty* queue = QueueCreate();
    int a = 1, expected = 2, c = 3, actual = 0;
    
    QueueEnqueue(queue, &a);
    QueueEnqueue(queue, &expected);
    QueueEnqueue(queue, &c);
    QueueDequeue(queue);
    
    actual = *(int*)QueuePeek(queue);
    
    if (expected != actual)
    {
        printf("2. wrong number has been found!\n");
    }
    
    QueueDestroy(queue);
}

static void TestQueueSize(void)
{
    queue_ty* queue = QueueCreate();
    int a = 1, b = 2;
    size_t actual = 2;
    
    QueueEnqueue(queue, &a);
    QueueEnqueue(queue, &b);
    
    if (QueueSize(queue) != actual)
    {
        printf("wrong size ya habibi");
    }
}

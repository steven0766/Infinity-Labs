/***********************************************************************
*File_name: queue_test.c
*Reviewer: itai
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stdio.h> /*  printf */

#include "queue.h"
#include "slist.h"


static void TestQueueCreate(void);
static void TestQueueEnqueueAndPeek(void);
static void TestQueueDequeue(void);
static void TestQueueSize(void);
static void TestQueueAppend(void);

int main()
{
    TestQueueCreate();
    TestQueueEnqueueAndPeek(); 
    TestQueueDequeue();
    TestQueueSize();
    TestQueueAppend();
    
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
    
    QueueDestroy(queue);
}

static void TestQueueAppend(void)
{
    queue_ty* q1 = QueueCreate();
    queue_ty* q2 = QueueCreate();
    int a = 1, b = 2, c = 3, d = 4, e = 5, size = 0;

    QueueEnqueue(q1, &a);
    QueueEnqueue(q1, &b);
    QueueEnqueue(q1, &c);

    QueueEnqueue(q2, &d);
    QueueEnqueue(q2, &e);

    QueueAppend(q1, q2);
    
    size = QueueSize(q1);

    if (5 != size)
    {
        printf("didn't append right, got %d\n", size);
    }
    
    size = QueueSize(q2);

    if (!QueueIsEmpty(q2))
    {
        printf("q2 isn't empty, got %d\n", size);
    }

    QueueDestroy(q1);
    QueueDestroy(q2);

}

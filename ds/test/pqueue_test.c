/***********************************************************************
*File_name: uid.c
*Reviewer: itay
*Version:  1.0
*Date:	   6.1.26
************************************************************************/

#include <stdio.h> /* printf */

#include "pqueue.h"

static void PQCreateTest(void);
static void PQEnqueueAndDequeue(void);
static void PQRemoveTest(void);

static int Cmp(const void* data1, const void* data2, void* param);
static int IsMatch(const void* data, void* param);
static void Print(char* func, char* problem);

int main()
{
    PQCreateTest();
    PQEnqueueAndDequeue();
    PQRemoveTest();
    
    return 0;
}

static void PQCreateTest(void)
{
    int param = 1;
    pq_ty* p_q = PQCreate(Cmp, &param);
    
    if (NULL == p_q)
    {
        Print("PQCreateTest", "PQcreate");
    }
    
    if (!PQIsEmpty(p_q))
    {
        Print("PQCreateTest", "PQIsEmpty");
    }
    
    PQDestroy(p_q);
}

static void PQEnqueueAndDequeue(void)
{
    int param = 1;
    pq_ty* p_q = PQCreate(Cmp, &param);
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    
    if (NULL == p_q)
    {
        Print("PQEnqueueAndDequeue", "PQcreate");
    }
    
    PQEnqueue(p_q, &c);
    PQEnqueue(p_q, &e);
    PQEnqueue(p_q, &a);
    PQEnqueue(p_q, &d);
    PQEnqueue(p_q, &b);
    
    if (1 != *(int*)PQDequeue(p_q))
    {
        Print("PQEnqueueAndDequeue", "PQDequeue");
    }
    
    if (2 != *(int*)PQPeek(p_q))
    {
        Print("PQEnqueueAndDequeue", "PQPeek");
    }

    if (4 !=PQCount(p_q))
    {
        Print("PQEnqueueAndDequeue", "PQCount");
    }
    
    PQDestroy(p_q);
}

static void PQRemoveTest(void)
{
    int param = 1;
    pq_ty* p_q = PQCreate(Cmp, &param);
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int is_match_param = 3;
    
    if (NULL == p_q)
    {
        Print("PQRemove", "PQcreate");
    }
    
    PQEnqueue(p_q, &c);
    PQEnqueue(p_q, &e);
    PQEnqueue(p_q, &a);
    PQEnqueue(p_q, &d);
    PQEnqueue(p_q, &b);
    
    if (3 != *(int*)PQRemove(p_q, IsMatch, &is_match_param))
    {
        Print("PQCreateTest", "PQRemove");
    }
    
    PQDestroy(p_q);
}

static int Cmp(const void* data1, const void* data2, void* param)
{
    int result = 0;
    
    switch (*(int*)param)
    {
        case 1:
            result = *(size_t*)data1 - *(size_t*)data2;
            break;
        
        default:
            result = *(size_t*)data2 - *(size_t*)data1;
            break;
    }
    
    return result;
}

static int IsMatch(const void* data, void* param)
{
    return (0 == *(int*)data % *(int*)param);
}

static void Print(char* func, char* problem)
{
    printf("houston we have a problem in \"%s\" function!\n"
           "problem in:  %s\n", func, problem);
}

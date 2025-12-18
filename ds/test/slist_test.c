/***********************************************************************
*File_name: vector_test.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   12.12.25
************************************************************************/


#include <stdio.h> /* printf */

#include "slist.h"


static void TestSListEndIter(void);
static void TestSListBeginIter(void);
static void TestIterNext(void);
static void TestIterIsEqual(void);
static void TestIterSetData(void);
static void TestSListCreate(void);
static void TestSListCount(void);
static void TestSListRemove(void);
static void TestSlistInsertBefore(void);
static void TestSListFind(void);
static void TestSListForEach(void);
static void TestSListAppend(void);

static int MatchInt(void* data, void* param);
static int AddToInt(void* data, void* param);
static void Print(char* msg);


int main()
{
    TestSListCreate();
    TestSListBeginIter();
    TestSListEndIter();
    TestIterNext();
    TestIterIsEqual();
    TestIterSetData();
    TestSListCount();
    TestSlistInsertBefore();
    TestSListRemove();
    TestSListFind();
    TestSListForEach();
    TestSListAppend();
    
    return 0;
}

static void TestSListCreate(void)
{
    slist_ty* list = SListCreate();
    
    if(NULL == list)
    {
        Print("TestSListCreate");
    }
    
    SListDestroy(list);
}

static void TestSListEndIter(void)
{
    slist_ty* list = SListCreate();

    if (!IterIsEqual(SListBeginIter(list), SListEndIter(list)))
    {
        Print("TestSListEndIter");
    }
  
    SListDestroy(list);
}

static void TestSListBeginIter(void)
{
    slist_ty* list = SListCreate();
    int x = 10;

    slist_iter_ty iter = SlistInsertBefore(list, SListBeginIter(list), &x);

    if (SListBeginIter(list) != iter)
    {
        Print("TestSListBeginIter");
    }

    SListDestroy(list);
}

static void TestIterNext(void)
{
    slist_ty* list = SListCreate();
    int a = 1, b = 2;

    slist_iter_ty it1 = SlistInsertBefore(list, SListBeginIter(list), &a);
    slist_iter_ty it2 = SlistInsertBefore(list, it1, &b);

    if (IterIsEqual(IterNext(it2), it2))
    {
        Print("TestIterNext");
    }

    SListDestroy(list);
}

static void TestIterIsEqual(void)
{
    slist_ty* list = SListCreate();

    if (!IterIsEqual(SListBeginIter(list), SListBeginIter(list)))
    {
        Print("TestIterIsEqual");
    }

    SListDestroy(list);
}

static void TestIterSetData(void)
{
    slist_ty* list = SListCreate();
    int x = 1, y = 2;

    slist_iter_ty it = SlistInsertBefore(list, SListBeginIter(list), &x);
    IterSetData(it, &y);

    if (*(int*)IterGetData(it) != 2)
    {
        Print("TestIterSetData");
    }

    SListDestroy(list);
}

static void TestSListCount(void)
{
    slist_ty* list = SListCreate();
    int a = 1, b = 2, c = 3;

    if (SListCount(list) != 0)
    {
        Print("TestSListCount");
    }

    SlistInsertBefore(list, SListBeginIter(list), &a);
    SlistInsertBefore(list, SListBeginIter(list), &b);
    SlistInsertBefore(list, SListBeginIter(list), &c);

    if (SListCount(list) != 3)
    {
        Print("TestSListCount");
    }

    SListDestroy(list);
}

static void TestSlistInsertBefore(void)
{
    slist_ty* list = SListCreate();
    int x = 5;

    slist_iter_ty it = SlistInsertBefore(list, SListBeginIter(list), &x);

    if (*(int*)IterGetData(it) != 5)
    {
        Print("TestSlistInsertBefore");
    }

    SListDestroy(list);
}

static void TestSListFind(void)
{
    slist_ty* list = SListCreate();
    int a = 1, b = 2, key = 2, bad = 9;

    SlistInsertBefore(list, SListBeginIter(list), &a);
    SlistInsertBefore(list, SListBeginIter(list), &b);

    if (SListFind(SListBeginIter(list),
                  SListEndIter(list),
                  MatchInt,
                  &key) == SListEndIter(list))
    {
        Print("TestSListFind");
    }

    if (SListFind(SListBeginIter(list),
                  SListEndIter(list),
                  MatchInt,
                  &bad) != SListEndIter(list))
    {
        Print("TestSListFind");
    }

    SListDestroy(list);
}

static void TestSListRemove(void)
{
    slist_ty* list = SListCreate();
    int a = 1, b = 2, c = 3;

    slist_iter_ty it1 = SlistInsertBefore(list, SListBeginIter(list), &a);
    slist_iter_ty it2 = SlistInsertBefore(list, it1, &b);
    SlistInsertBefore(list, it2, &c);

    SListRemove(it2);

    if (SListCount(list) != 2)
    {
        Print("TestSListRemove");
    }

    SListDestroy(list);
}

static void TestSListForEach(void)
{
    slist_ty* list = SListCreate();
    int a = 1, b = 2, add = 10;

    SlistInsertBefore(list, SListBeginIter(list), &a);
    SlistInsertBefore(list, SListBeginIter(list), &b);

    SListForEach(SListBeginIter(list),
                 SListEndIter(list),
                 AddToInt,
                 &add);

    if (*(int*)IterGetData(SListBeginIter(list)) != 12)
    {
        Print("TestSListForEach");
    }

    SListDestroy(list);
}

static void TestSListAppend(void)
{
    slist_ty* list1 = SListCreate();
    slist_ty* list2 = SListCreate();
    int a = 1, b = 2, c = 3, d = 4, cnt = 0;
    
    SlistInsertBefore(list1, SListBeginIter(list1), &a);
    SlistInsertBefore(list1, SListBeginIter(list1), &b);
    
    SlistInsertBefore(list2, SListBeginIter(list2), &c);
    SlistInsertBefore(list2, SListBeginIter(list2), &d);
    
    SListAppend(list1, list2);
    
    cnt = SListCount(list1);
    
    if (4 != cnt)
    {
        printf("append count isn't right, %d", cnt);
    }
    
    cnt = SListCount(list2);
    
    if (0 != cnt)
    {
        printf("second list isnt empty, %d", cnt);
    }
}

static int MatchInt(void* data, void* param)
{
    return (*(int*)data == *(int*)param);
}

static int AddToInt(void* data, void* param)
{
    *(int*)data += *(int*)param;
    return 0;
}

static void Print(char* msg)
{
    printf("houston we have a problem in \"%s\" function!\n", msg);
}



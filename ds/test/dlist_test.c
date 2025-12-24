/***********************************************************************
*File_name: dlist_test.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   23.12.25
************************************************************************/


#include <stdio.h> /* printf */

#include "dlist.h"


static void TestDListCreateAndDestroy(void);
static void TestDListPushAndPop(void);
static void TestFindAndSlice(void);

static void Print(char* func, char* problem);
static void PrintList(dlist_ty* list);
static int Match(const void* data, void* param);


int main()
{
    TestDListCreateAndDestroy();
    TestDListPushAndPop();
    TestFindAndSlice();
    
    return 0;
}


static void TestDListCreateAndDestroy(void)
{
    dlist_ty* dlist = DListCreate();
    
    if (NULL == dlist)
    {
        Print("TestDListCreate", "create dlist");
    }
    
    if (0 != DListIsEmpty(dlist))
    {
        Print("TestDListCreate", "not empty");
    }
    
    if (0 != DListCount(dlist))
    {
        Print("TestDListCreate", "count dummy");
    }
    
    DListDestroy(dlist);
}

static void TestDListPushAndPop(void)
{
    int a = 1, b = 2, c = 3, d = 4;
    dlist_iter_ty end_iter = NULL;
    dlist_iter_ty begin_iter = NULL;
    dlist_ty* dlist = DListCreate();
    
    if (NULL == dlist)
    {
        Print("TestDListPushAndPop", "create dlist");
    }
    
    DListPushFront(dlist, &b);
    DListPushFront(dlist, &a);
    DListPushBack(dlist, &c);
    DListPushBack(dlist, &d);
    
    if (4 != DListCount(dlist))
    {
        Print("TestDListPushAndPop", "wrong count");
    }
    
    if (0 == DListIsEmpty(dlist))
    {
        Print("TestDListPushAndPop", "why empty?");
    }
    
    end_iter = DListEndIter(dlist);
    begin_iter = DListBeginIter(dlist);
    
    if (NULL == end_iter)
    {
        Print("TestDListPushAndPop", "no nodes in dlist");
    }
    
    end_iter = DListIterPrev(end_iter);
    
    if (4 != *(int*)DListIterGetData(end_iter))
    {
        Print("TestDListPushAndPop", "1.wrong end number");
    }
    
    if (1 != *(int*)DListIterGetData(begin_iter))
    {
        Print("TestDListPushAndPop", "1.wrong begin number");
    }
    
    end_iter = DListIterPrev(end_iter);
    begin_iter = DListIterNext(begin_iter);
    
    DListPopFront(dlist);
    DListPopBack(dlist);

    if (3 != *(int*)DListIterGetData(end_iter))
    {
        Print("TestDListPushAndPop", "2.wrong end number");
    }
    
    if (2 != *(int*)DListIterGetData(begin_iter))
    {
        Print("TestDListPushAndPop", "2.wrong begin number");
        printf("%d", *(int*)DListIterGetData(begin_iter));
    }
    
    DListDestroy(dlist);
}

static void TestFindAndSlice(void)
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    dlist_iter_ty where = NULL;
    dlist_iter_ty from = NULL;
    dlist_iter_ty to = NULL;
    dlist_iter_ty result = NULL;
    dlist_ty* dlist1 = DListCreate();
    dlist_ty* dlist2 = DListCreate();
    dlist_ty* dlist3 = DListCreate();
    
    if (NULL == dlist1 || NULL == dlist2 || NULL == dlist3)
    {
        Print("TestDListPushAndPop", "create dlist");
    }
    /* list1 -> H-1-2-3-4-5-T */
    DListPushFront(dlist1, &b);
    DListPushFront(dlist1, &a);
    DListPushBack(dlist1, &c);
    where = DListEndIter(dlist1);
    where = DListIterPrev(where);
    DListPushBack(dlist1, &d);
    DListPushBack(dlist1, &e);
    
    /* list2 -> H-1-2-3-T */
    DListPushFront(dlist2, &b);
    DListPushFront(dlist2, &a);
    DListPushBack(dlist2, &c);
    
    from = DListBeginIter(dlist2);
    to = DListEndIter(dlist2);
    to = DListIterPrev(to);
    
    /* list1 -> H-1-2-1-2-3-4-5-T */
    DListSplice(from, to, where);
    
    if (7 != DListCount(dlist1))
    {
        Print("TestDListPushAndPop", "wrong count");
        PrintList(dlist1);
        printf("\n");
    }
    
    from = DListBeginIter(dlist1);
    to = DListEndIter(dlist1);
    to = DListIterPrev(to);
    
    if(0 != DListMultiFind(from, to, dlist3, Match, &c))
    {
        Print("TestDListPushAndPop", "DListMultiFind");
        PrintList(dlist3);
        printf("\n");
    }
    
    result = DListFind(from, to, Match, &e);
    
    if (e != *(int*)DListIterGetData(result))
    {
        Print("TestDListPushAndPop", "didn't find it");
    }
    
    DListDestroy(dlist1);
    DListDestroy(dlist2);
    DListDestroy(dlist3);
}


static void Print(char* func, char* problem)
{
    printf("houston we have a problem in \"%s\" function!\n"
           "problem in:  %s\n", func, problem);
}

static void PrintList(dlist_ty* list)
{
    dlist_iter_ty from = DListBeginIter(list);
    dlist_iter_ty to = DListEndIter(list);
    
    while (from != to)
    {
        printf("%d, ", *(int*)DListIterGetData(from));
        from = DListIterNext(from);
    }
}

static int Match(const void* data, void* param)
{
    return (data == param) ? 0 : 1;
}

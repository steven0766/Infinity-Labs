/***********************************************************************
*File_name: test_sorted_list.c
*Reviewer: Rozalin
*Version:  1.0
*Date:	   27.12.25
************************************************************************/

#include <stdio.h> /* printf */

#include "sorted_list.h"

static void TestCreateAndDestroy(void);
static void TestInsertAndRemove(void);
static void TestFind(void);
static void TestFindIf(void);
static void TestIterSetData(void);

static void Print(char* func, char* problem);
static int Cmp(const void* data1, const void* data2, void* param);
static void PrintListOfInts(sorted_list_ty* list);
static int MatchEven(const void* data, void* param);
static int CheckSortedInts(sorted_list_ty* list, int* expected, size_t size);

int main()
{
    TestCreateAndDestroy();
    TestInsertAndRemove();
    TestFind();
    TestFindIf();
    TestIterSetData();
    
    return 0;
}

static void TestCreateAndDestroy(void)
{
    int para = 1;
    sorted_list_ty* sorted_list = SortedListCreate(Cmp, &para);
    
    if (NULL == sorted_list)
    {
        Print("TestCreateAndDestroy", "didn't create list");
    }
    
    if (1 != SortedListIsEmpty(sorted_list))
    {
        Print("TestCreateAndDestroy", "list isnt empty");
    }
    
    if (0 != SortedListCount(sorted_list))
    {
        Print("TestCreateAndDestroy", "list have nodes");
    }
    
    SortedListDestroy(sorted_list);
}

static void TestInsertAndRemove(void)
{
    int para = 1;
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int expected1[] = {1, 2, 3, 4, 5};
    int expected2[] = {1, 2, 4, 5};
    int expected3[] = {2, 4};

    sorted_list_ty* list = SortedListCreate(Cmp, &para);
    sorted_list_iter_ty iter;

    if (NULL == list)
    {
        Print("TestInsertAndRemove", "create failed");
        return;
    }

    SortedListInsert(list, &a);
    SortedListInsert(list, &c);
    SortedListInsert(list, &e);
    SortedListInsert(list, &d);
    iter = SortedListInsert(list, &b);

    if (!CheckSortedInts(list, expected1, 5))
    {
        Print("Insert", "order after insert is wrong");
    }

    iter = SortedListIterNext(iter);
    SortedListRemove(iter);

    if (!CheckSortedInts(list, expected2, 4))
    {
        Print("Remove", "order after removing middle is wrong");
    }

    SortedListRemove(SortedListBeginIter(list));

    SortedListRemove(SortedListIterPrev(SortedListEndIter(list)));

    if (!CheckSortedInts(list, expected3, 2))
    {
        Print("Remove", "order after removing edges is wrong");
    }

    SortedListDestroy(list);
}


static void TestFind(void)
{
    int para = 1;
    int a = 1, b = 2, c = 3;
    int not_exist = 10;
    sorted_list_ty* sorted_list = NULL;
    sorted_list_iter_ty it;

    sorted_list = SortedListCreate(Cmp, &para);
    if (NULL == sorted_list)
    {
        Print("TestFind", "create failed");
        return;
    }

    SortedListInsert(sorted_list, &a);
    SortedListInsert(sorted_list, &b);
    SortedListInsert(sorted_list, &c);

    it = SortedListFind(sorted_list, &b);
    if (2 != *(int*)SortedListIterGetData(it))
    {
        Print("Find", "didn't find existing value");
    }

    it = SortedListFind(sorted_list, &not_exist);
    if (!SortedListIterIsEqual(it, SortedListEndIter(sorted_list)))
    {
        Print("Find", "should return EndIter");
    }

    SortedListDestroy(sorted_list);
}

static void TestFindIf(void)
{
    int para = 1;
    int a = 1, b = 2, c = 3, d = 4;
    sorted_list_ty* sorted_list = NULL;
    sorted_list_iter_ty it;

    sorted_list = SortedListCreate(Cmp, &para);
    if (NULL == sorted_list)
    {
        Print("TestFindIf", "create failed");
        return;
    }

    SortedListInsert(sorted_list, &a);
    SortedListInsert(sorted_list, &b);
    SortedListInsert(sorted_list, &c);
    SortedListInsert(sorted_list, &d);

    it = SortedListFindIf(SortedListBeginIter(sorted_list),
                          SortedListEndIter(sorted_list),
                          MatchEven,
                          NULL);

    if (2 != *(int*)SortedListIterGetData(it))
    {
        Print("FindIf", "did not find first even number");
    }

    SortedListDestroy(sorted_list);
}

static void TestIterSetData(void)
{
    int para = 1;
    int a = 1, b = 3, c = 5;
    int x = 4;
    int y = 0;
    int z = 2;

    int expected1[] = {1, 4, 5};
    int expected2[] = {0, 1, 4};
    int expected3[] = {0, 2, 4};

    sorted_list_ty* list = NULL;
    sorted_list_iter_ty it;

    list = SortedListCreate(Cmp, &para);
    if (NULL == list)
    {
        Print("TestIterSetData", "create failed");
        return;
    }

    SortedListInsert(list, &a);
    it = SortedListInsert(list, &b);
    SortedListInsert(list, &c);

    SortedListIterSetData(list, it, &x);
    if (!CheckSortedInts(list, expected1, 3))
    {
        Print("SetData", "middle to big broke order");
        PrintListOfInts(list);
    }

    it = SortedListIterPrev(SortedListEndIter(list));
    SortedListIterSetData(list, it, &y);
    if (!CheckSortedInts(list, expected2, 3))
    {
        Print("SetData", "end to small broke order");
        PrintListOfInts(list);
    }

    it = SortedListIterNext(SortedListBeginIter(list));
    SortedListIterSetData(list, it, &z);
    if (!CheckSortedInts(list, expected3, 3))
    {
        Print("SetData", "first to middle broke order");
        PrintListOfInts(list);
    }

    SortedListDestroy(list);
}

static void Print(char* func, char* problem)
{
    printf("houston we have a problem in \"%s\" function!\n"
           "problem in:  %s\n", func, problem);
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

static void PrintListOfInts(sorted_list_ty* list)
{
	sorted_list_iter_ty cur_iter = SortedListBeginIter(list);

	while(!SortedListIterIsEqual(cur_iter, SortedListEndIter(list)))
	{
		printf("%d --> ", *(int*)SortedListIterGetData(cur_iter));
		cur_iter = SortedListIterNext(cur_iter);
	}
	printf("END\n\n");
}

static int MatchEven(const void* data, void* param)
{
    (void)param;
    return (*(int*)data % 2 == 0);
}

static int CheckSortedInts(sorted_list_ty* list, int* expected, size_t size)
{
    size_t i = 0;
    sorted_list_iter_ty iter = SortedListBeginIter(list);

    /* compare expected values */
    for (i = 0; i < size; ++i)
    {
        if (SortedListIterIsEqual(iter, SortedListEndIter(list)))
        {
            printf("list ended early at index %lu\n", i);

            printf("expected: ");
            for (i = 0; i < size; ++i)
            {
                printf("%d ", expected[i]);
            }
            printf("\nactual:\n");
            PrintListOfInts(list);

            return 0;
        }

        if (*(int*)SortedListIterGetData(iter) != expected[i])
        {
            printf("mismatch at index %lu\n", i);

            printf("expected:\n");
            for (i = 0; i < size; ++i)
            {
                printf("%d ", expected[i]);
            }

            printf("\nactual:\n");
            PrintListOfInts(list);

            return 0;
        }

        iter = SortedListIterNext(iter);
    }

    if (!SortedListIterIsEqual(iter, SortedListEndIter(list)))
    {
        printf("list has extra elements\n");

        printf("expected:\n");
        for (i = 0; i < size; ++i)
        {
            printf("%d ", expected[i]);
        }

        printf("\nactual:\n");
        PrintListOfInts(list);

        return 0;
    }

    return 1;
}



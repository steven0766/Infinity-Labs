/***********************************************************************
*File_name: linkedlist_exe_test.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   16.12.25
************************************************************************/


#include <stdio.h>        /* printf */
#include <stdlib.h>       /*  */
#include <assert.h>       /*  */

#include "slist_exe.h"

static void TestFlip(void);
static void TestHasLoop(void);
static void TestFindIntersection(void);
static node_ty* CreateNode(void* data);
static void FreeList(node_ty* head);


int main(void)
{
    TestFlip();
    TestHasLoop();
    TestFindIntersection();

    return 0;
}


static void TestFlip(void)
{
    int a = 1, b = 2, c = 3;

    node_ty* n1 = CreateNode(&a);
    node_ty* n2 = CreateNode(&b);
    node_ty* n3 = CreateNode(&c);
    node_ty* new_head = NULL;

    n1->next = n2;
    n2->next = n3;

    new_head = Flip(n1);

    if (new_head != n3)
    {
        printf("problem with Flip: head is not n3\n");
    }

    FreeList(new_head);
}

static void TestHasLoop(void)
{
    int a = 1, b = 2, c = 3;

    node_ty* n1 = CreateNode(&a);
    node_ty* n2 = CreateNode(&b);
    node_ty* n3 = CreateNode(&c);

    n1->next = n2;
    n2->next = n3;

    if (HasLoop(n1) != 0)
    {
        printf("problem with HasLoop\n");
    }

    n3->next = n2; /* create loop */

    if (HasLoop(n1) != 1)
    {
        printf("problem with HasLoop\n");
    }

    n3->next = NULL;
    FreeList(n1);
}

static void TestFindIntersection(void)
{
    int a = 1, b = 2, c = 3, x = 4;

    node_ty* n1 = CreateNode(&a);
    node_ty* n2 = CreateNode(&b);
    node_ty* n3 = CreateNode(&c);
    node_ty* x1 = CreateNode(&x);
    
    n1->next = n2;
    n2->next = n3;
    x1->next = n3;

    if (FindIntersection(n1, x1) != NULL)
    {
        printf("problem with FindIntersection: wrong node found\n");
    }

    free(x1);
    FreeList(n1);
}


static node_ty* CreateNode(void* data)
{
    node_ty* node = (node_ty*)malloc(sizeof(node_ty));
    assert(node);

    node->data = data;
    node->next = NULL;

    return node;
}

static void FreeList(node_ty* head)
{
    node_ty* tmp = NULL;

    while (head)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}


#include <stdio.h>

#include "../ds/src/bst.c"

static void MirrorTree(bst_ty* bst);
static void MirrorNode(bst_node_ty* node);
static void SwapChildren(bst_node_ty* node);
static int PrintInt(void* data, void* param);
static int IntCmp(const void* data1, const void* data2, void* param);

int main()
{
	int values[] = {4, 2, 6, 1, 3, 5, 7};
    size_t i = 0;

    bst_ty* bst = BstCreate(IntCmp, NULL);

    for (i = 0; i < 7; ++i)
    {
        BstInsert(bst, &values[i]);
    }

    printf("In-order BEFORE mirror:\n");
    BstForEach(BstItBegin(bst), BstItEnd(bst), PrintInt, NULL);
    printf("\n");

    MirrorTree(bst);

    printf("In-order AFTER mirror:\n");
    BstForEach(BstItBegin(bst), BstItEnd(bst), PrintInt, NULL);
    printf("\n");

    BstDestroy(bst);
	
	return 0;
}

static void MirrorTree(bst_ty* bst)
{
	if (!bst)
	{
		return;
	}
	
	MirrorNode(bst->dummy.left);
}

static void MirrorNode(bst_node_ty* node)
{
	if (!node)
	{
		return;
	}
	
	SwapChildren(node);
	MirrorNode(node->left);
	MirrorNode(node->right);
}

static void SwapChildren(bst_node_ty* node)
{
	bst_node_ty* tmp_node = node->left;
	node->left = node->right;
	node->right = tmp_node;
}

static int PrintInt(void* data, void* param)
{
    (void)param;
    printf("%d ", *(int*)data);
    return 0;
}

static int IntCmp(const void* data1, const void* data2, void* param)
{
    (void)param;
    return *(int*)data1 - *(int*)data2;
}

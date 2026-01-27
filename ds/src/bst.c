#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <stdio.h> /* size_t */

#include "bst.h"

struct bst_node
{
	bst_node_ty* parent;
	bst_node_ty* left;
	bst_node_ty* right;
	void* data;
};

struct bst
{
	bst_node_ty dummy;
	bst_cmp_ty cmp_func;
	void* cmp_param;
};

static bst_iter_ty GetIter(const bst_node_ty* node);
static bst_node_ty* GetNode(bst_iter_ty iter);
static int IsLeaf(bst_node_ty* node);
static int IsSingleChild(bst_node_ty* node);
static bst_node_ty* GetLeftMost(bst_node_ty* node);
static bst_node_ty* GetRightMost(bst_node_ty* node);
static int IsFromSmallerThenTo(bst_iter_ty from, bst_iter_ty to);

bst_ty* BstCreate(bst_cmp_ty cmp_func, void* cmp_param)
{
	bst_ty* bst = NULL;
	bst_node_ty dummy = {0};
	
	assert(NULL != cmp_func);
	
	bst = (bst_ty*)malloc(sizeof(*bst) * sizeof(char));
	if (NULL == bst)
	{
		return NULL;
	}
	bst->dummy = dummy;
	bst->cmp_func = cmp_func;
	bst->cmp_param = cmp_param;

	return bst;
}

void BstDestroy(bst_ty* bst)
{
	bst_node_ty* parent = NULL, *curr = NULL;
	if (NULL == bst)
	{
		return;
	}
	
	curr = bst->dummy.left;
	if (NULL != curr)
	{
		while (curr != BstItEnd(bst))
		{
			parent = curr;
			
			if (curr->left)
			{
				curr = curr->left;
				parent->left = NULL;
			}
			else if(curr->right)
			{
				curr = curr->right;
				parent->right = NULL;
			}
			else
			{
				parent = parent->parent;
				free(curr);
				curr = parent;
			}
		}
	}
	free(bst);
	bst = NULL;
}

bst_iter_ty BstInsert(bst_ty* bst, void* data)
{
	bst_node_ty* curr = NULL, *prev = NULL, *new_node = NULL;
	
	assert(NULL != bst);
	
	new_node = (bst_node_ty*)malloc(sizeof(bst_node_ty) * sizeof(char));
	if (!new_node)
	{
		return BstItEnd(bst);
	}
	
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	
	prev = &bst->dummy;
	curr = bst->dummy.left;
	
	while (curr)
	{
		prev = curr;
		if (bst->cmp_func(curr->data, data, bst->cmp_param) > 0)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	
	if (prev == &bst->dummy)
	{
		bst->dummy.left = new_node;
	}
	else if (bst->cmp_func(prev->data, data, bst->cmp_param) > 0)
	{
		prev->left = new_node;
	}
	else
	{
		prev->right = new_node;
	}
	new_node->parent = prev;
	
	return GetIter(new_node);
}

bst_iter_ty BstFind(const bst_ty* bst, void* data)
{
	bst_iter_ty curr = NULL;
	
	assert(NULL != bst);
	
	curr = GetIter(bst->dummy.left);
	while (curr)
	{
		if (0 == bst->cmp_func(curr->data, data, bst->cmp_param))
		{
			return curr;
		}
		else if (0 < bst->cmp_func(curr->data, data, bst->cmp_param))
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	
	return BstItEnd(bst);
	
}

bst_iter_ty BstRemove(bst_iter_ty iter)
{
	bst_node_ty* to_remove = NULL, *parent = NULL, *tmp = NULL;
	void* tmp_data = NULL;
	bst_iter_ty next = {0};
	
	assert(NULL != GetNode(iter)->parent);

	to_remove = GetNode(iter);
	next = BstItNext(iter);
	parent = to_remove->parent;

	/* case 1: remove leaf */
	if (IsLeaf(to_remove))
	{
		/* set parent left or right to null */
		if (parent->left == to_remove)
		{
			parent->left = NULL;
		}
		else
		{
			parent->right = NULL;
		}

		free(to_remove);
		to_remove = NULL;
	}
	else if(IsSingleChild(to_remove))
	{
		tmp = to_remove->left ? to_remove->left : to_remove->right;

		if (parent->left == to_remove)
		{
			parent->left = tmp;
		}
		else
		{
			parent->right = tmp;
		}
		tmp->parent = parent;

		free(to_remove);
		to_remove = NULL;
	}
	else
	{
		tmp_data = to_remove->data;
		to_remove->data = GetNode(next)->data;
		GetNode(next)->data = tmp_data;

		BstRemove(next);
		
		return GetIter(to_remove);
	}

	return next;
}
size_t BstSize(const bst_ty* bst)
{
	bst_node_ty* node = NULL;
	size_t cnt = 0;
	
	assert(NULL != bst);
	
	node = BstItBegin(bst);
	
	while (node && !BstItEqual(node, BstItEnd(bst)))
	{
		node = BstItNext(node);
		++cnt;
	}
	
	return cnt;
}

int BstForEach(bst_iter_ty from,
               bst_iter_ty to,
               bst_action_func_ty action,
               void* param)
{
	bst_node_ty* runner = NULL, *end = NULL;
	int result = 0;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action);
	assert(1 == IsFromSmallerThenTo(from, to));
	
	runner = from;
	end = to;
	while (!BstItEqual(runner, end))
	{
		result = action(BstGetData(runner), param);
		if (result)
		{
			return result;
		}

		runner = BstItNext(runner);
	}
	
	return 0;
}

bst_iter_ty BstItBegin(const bst_ty* bst)
{
	bst_node_ty* node = NULL;
	
	assert(NULL != bst);
	
	
	node = bst->dummy.left;
	return node? GetIter(GetLeftMost(node)) : &bst->dummy;
}

bst_iter_ty BstItNext(bst_iter_ty iter)
{
	bst_node_ty* node = NULL, *parent = NULL;
	
	assert(NULL != GetNode(iter));
	assert(NULL != GetNode(iter)->parent);
	
	node = GetNode(iter);
	
	if (node->right)
	{
		return GetIter(GetLeftMost(node->right));
	}
	
	parent = node->parent;
	
	while (NULL != parent && parent->right == node)
	{
		parent = parent->parent;
		node = node->parent;
	}
	
	return GetIter(parent);
}

bst_iter_ty BstItPrev(bst_iter_ty iter)
{
	bst_node_ty* node = NULL, *parent = NULL;
	assert(NULL != iter);
	
	node = GetNode(iter);
	
	if (node->left)
	{
		node = GetRightMost(node->left);
		return GetIter(node);
	}
	
	parent = node->parent;
	while (parent->right != node)
	{
		node = parent;
		parent = parent->parent;
	}
	
	return GetIter(parent);
}

bst_iter_ty BstItEnd(const bst_ty* bst)
{
	assert(NULL != bst);
	return GetIter(&bst->dummy);
}

int BstItEqual(bst_iter_ty iter1, bst_iter_ty iter2)
{
	return (GetNode(iter1) == GetNode(iter2));
}

void* BstGetData(bst_iter_ty iter)
{
	bst_node_ty* node = NULL;
	assert(NULL != iter);
	
	node = GetNode(iter);
	return node->data;
}

int BstIsEmpty(const bst_ty* bst)
{
	return (NULL == bst->dummy.left);
}

/*=================== help functions =================== */

static bst_iter_ty GetIter(const bst_node_ty* node)
{
	return (bst_iter_ty)node;
}

static bst_node_ty* GetNode(bst_iter_ty iter)
{
	return (bst_node_ty*)iter;
}

static int IsLeaf(bst_node_ty* node)
{
	return (!node->left && !node->right);
}

static int IsSingleChild(bst_node_ty* node)
{
	return ((node->left && !node->right) ||
			(!node->left && node->right));
}

static bst_node_ty* GetLeftMost(bst_node_ty* node)
{
	if(node)
	{
		while (node->left)
		{
			node = node->left;
		}
	}
	return node;
}

static bst_node_ty* GetRightMost(bst_node_ty* node)
{
	if (node)
	{
		while (node->right)
		{
			node = node->right;
		}
	}
	
	return node;
}

static int IsFromSmallerThenTo(bst_iter_ty from, bst_iter_ty to)
{
	while (from != to)
	{
		from = BstItNext(from);
	}
	
	return 1;
}

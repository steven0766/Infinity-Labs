#include <stddef.h>         /* size_t */
#include <assert.h>			/* assert */
#include <stdlib.h>			/* malloc, free */

#include "avl.h"			/* traverse_ty */

#define SUCCESS				0
#define FAIL				1
#define MAX(a,b)			(((a) > (b)) ? (a) : (b))

typedef struct avl_node
{
	struct avl_node* children[2];
	void* data;
	int height;
}avl_node_ty;

struct avl
{
	avl_node_ty* root;
	avl_cmp_ty cmp_func;
	void* cmp_param;
};

typedef enum {
    LEFT = 0,
    RIGHT = 1
} direction_ty; 

typedef enum {
    ORD_ROOT = 0,
    ORD_LEFT = 1,
    ORD_RIGHT = 2
} order_ty; 

static avl_node_ty* CreateNode(void* data);
static void DestroyAllNodes(avl_node_ty* node);
static avl_node_ty* RemoveNode(avl_ty* avl, avl_node_ty* node, void* data, int* status);
static int IsLeaf(avl_node_ty* node);
static int IsSingleChild(avl_node_ty* node);
static avl_node_ty* GetLeftMost(avl_node_ty* node);
static int GetNodeHeight(avl_node_ty* node);
static void UpdateNodeHeight(avl_node_ty* node);
static int GetBalanceFactor(avl_node_ty* node);
static avl_node_ty* ReBalance(avl_node_ty* node);
static avl_node_ty* RotateRight(avl_node_ty* node);
static avl_node_ty* RotateLeft(avl_node_ty* node);
static avl_node_ty* InsertNode(avl_ty* avl, avl_node_ty* node, avl_node_ty* new_node);
static int Travel(avl_node_ty* node, order_ty* order, avl_action_func_ty action, void* param);
static size_t CountNodes(avl_node_ty* node);
static void* FindNode(avl_node_ty* node, void* elem_to_find, avl_cmp_ty cmp_func, void* param);

avl_ty* AvlCreate(avl_cmp_ty cmp_func, void* cmp_param)
{
	avl_ty* avl = NULL;
	
	assert(cmp_func);
	
	avl = (avl_ty*)malloc(sizeof(*avl) * sizeof(char));
	if (NULL == avl)
	{
		return NULL;
	}
	
	avl->root = NULL;
	avl->cmp_func = cmp_func;
	avl->cmp_param = cmp_param;

	return avl;
}

void AvlDestroy(avl_ty* avl)
{
	if (!avl)
	{
		return;
	}
	
	DestroyAllNodes(avl->root);
	free(avl);
	avl = NULL;
}

int AvlRemove(avl_ty* avl, void* data)
{
	int status = 0;
	
	assert(avl);
	
	avl->root = RemoveNode(avl, avl->root, data, &status);
	
	return status;
}

int AvlInsert(avl_ty* avl, void* data)
{
	avl_node_ty* new_node = NULL;
	
	assert(avl);
	assert(!AvlFind(avl, data));
	
	new_node = CreateNode(data);
	if (!new_node)
	{
		return FAIL;
	}
	
	avl->root =  InsertNode(avl, avl->root, new_node);
	
	return SUCCESS;
}

int AvlForEach(avl_ty* avl, traverse_ty traverse, avl_action_func_ty action, void* param)
{
	order_ty order[3][3] = {{ORD_LEFT, ORD_ROOT, ORD_RIGHT},
						 	{ORD_ROOT, ORD_LEFT, ORD_RIGHT},
							{ORD_LEFT, ORD_RIGHT, ORD_ROOT}};
						  
	assert(avl);
	assert(action);
	
	return Travel(avl->root, order[traverse], action, param);
}

size_t AvlSize(const avl_ty* avl)
{
	assert(avl);
	
	return CountNodes(avl->root);
}

int AvlIsEmpty(const avl_ty* avl)
{
	assert(avl);
	
	
	return (NULL == avl->root);
}

void* AvlFind(avl_ty* avl, void* elem_to_find)
{
	assert(avl);
	
	return FindNode(avl->root, elem_to_find, avl->cmp_func, avl->cmp_param);
}

size_t AvlHeight(const avl_ty* avl)
{
	assert(avl);
	
	return MAX(0, GetNodeHeight(avl->root));
}

static avl_node_ty* CreateNode(void* data)
{
	avl_node_ty* node = (avl_node_ty*)malloc(sizeof(avl_node_ty) * sizeof(char));
	if (!node)
	{
		return NULL;
	}
	
	node->data = data;
	node->height = 0;
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	
	return node;
}

static void DestroyAllNodes(avl_node_ty* node)
{
	if (!node)
	{
		return;
	}
	
	DestroyAllNodes(node->children[LEFT]);
	DestroyAllNodes(node->children[RIGHT]);
	
	free(node);
	node = NULL;
}

static avl_node_ty* RemoveNode(avl_ty* avl, avl_node_ty* node, void* data, int* status)
{
	int cmp_result = 0;
	avl_node_ty* child = NULL;
	avl_node_ty* successor = NULL;
	direction_ty dir = LEFT;
	
	if (!node)
	{
		*status = FAIL;
		return NULL;
	}
	
	cmp_result = avl->cmp_func(data, node->data, avl->cmp_param);
	
	if (0 == cmp_result)
	{
		if (IsLeaf(node))
		{
			free(node);
			node = NULL;
			
			return NULL;
		}
		else if (IsSingleChild(node))
		{
			child = node->children[LEFT] ? node->children[LEFT] : node->children[RIGHT];
			
			free(node);
			node = NULL;
			
			return child;
		}
		else
		{
			successor = GetLeftMost(node->children[RIGHT]);
			node->data = successor->data;
			
			node->children[RIGHT] = RemoveNode(avl, node->children[RIGHT], successor->data, status);
			
			UpdateNodeHeight(node);
			node = ReBalance(node);
		}
	}
	else
	{
		dir = 0 > cmp_result ? LEFT : RIGHT;
		
		node->children[dir] = RemoveNode(avl, node->children[dir], data, status);
		
		UpdateNodeHeight(node);
		
		node = ReBalance(node);
	}
	
	return node;
}

static int IsLeaf(avl_node_ty* node)
{
	return (!node->children[LEFT] && !node->children[RIGHT]);
}

static int IsSingleChild(avl_node_ty* node)
{
	return ((node->children[LEFT] && !node->children[RIGHT]) ||
		    (!node->children[LEFT] && node->children[RIGHT]));
}

static avl_node_ty* GetLeftMost(avl_node_ty* node)
{
	if (!node->children[LEFT])
	{
		return node;
	}
	
	return GetLeftMost(node->children[LEFT]);
}

static int GetNodeHeight(avl_node_ty* node)
{
	return node ? node->height : -1;
}

static void UpdateNodeHeight(avl_node_ty* node)
{
	if (!node)
	{
		return;
	}
	node->height =  1 + MAX(GetNodeHeight(node->children[LEFT]), GetNodeHeight(node->children[RIGHT]));
}

static int GetBalanceFactor(avl_node_ty* node)
{
	return GetNodeHeight(node->children[LEFT]) - GetNodeHeight(node->children[RIGHT]);
}

static avl_node_ty* ReBalance(avl_node_ty* node)
{
	int bf = GetBalanceFactor(node);
	
	if (bf >= -1 && bf <= 1)
	{
		return node;
	}
	
	if (bf > 1)
	{
		if (GetBalanceFactor(node->children[LEFT]) < 0)
		{
			node->children[LEFT] = RotateLeft(node->children[LEFT]);
		}
		node = RotateRight(node);
	}
	else
	{
		if (GetBalanceFactor(node->children[RIGHT] ) > 0)
		{
			node->children[RIGHT] = RotateRight(node->children[RIGHT]);
		}
		node = RotateLeft(node);
	}
	
	return node;
}

static avl_node_ty* RotateRight(avl_node_ty* node)
{
	avl_node_ty* new_root = node->children[LEFT];
	
	node->children[LEFT] = new_root->children[RIGHT];
	new_root->children[RIGHT] = node;
	
	UpdateNodeHeight(node);
	UpdateNodeHeight(new_root);
	
	return new_root;
}

static avl_node_ty* RotateLeft(avl_node_ty* node)
{
	avl_node_ty* new_root = node->children[RIGHT];
	
	node->children[RIGHT] = new_root->children[LEFT];
	new_root->children[LEFT] = node;
	
	UpdateNodeHeight(node);
	UpdateNodeHeight(new_root);
	
	return new_root;
}

static avl_node_ty* InsertNode(avl_ty* avl, avl_node_ty* node, avl_node_ty* new_node)
{
	int cmp_result = 0;
	direction_ty dir = LEFT; 
	
	if (!node)
	{
		return new_node;
	}

	cmp_result = avl->cmp_func(new_node->data, node->data, avl->cmp_param);
	
	dir = cmp_result < 0 ? LEFT : RIGHT;
	
	node->children[dir] = InsertNode(avl, node->children[dir], new_node);
	
	UpdateNodeHeight(node);
	
	return ReBalance(node);
	
}

static int Travel(avl_node_ty* node, order_ty* order, avl_action_func_ty action, void* param)
{
	size_t i = 0;
	int status = 0;
	
	if (!node)
	{
		return SUCCESS;
	}
	
	for (i = 0 ; i < 3; ++i)
	{
		switch (order[i])
		{
			case ORD_ROOT:
				status = action(node->data, param);
				break;
			case ORD_LEFT:
				status = Travel(node->children[LEFT], order, action, param);
				break;
			case ORD_RIGHT:
				status = Travel(node->children[RIGHT], order, action, param);
				break;
		}
		
		if (status)
		{
			break;
		}
	}
	
	return status;
}

static size_t CountNodes(avl_node_ty* node)
{
	if (!node)
	{
		return 0;	
	}
	
	return 1 + CountNodes(node->children[LEFT]) + CountNodes(node->children[RIGHT]);
}

static void* FindNode(avl_node_ty* node, void* elem_to_find, avl_cmp_ty cmp_func, void* param)
{
	int cmp_result = 0;
	int dir = 0;
	
	if (!node)
	{
		return NULL;
	}
	
	cmp_result = cmp_func(elem_to_find, node->data, param);
	if (0 == cmp_result)
	{
		return node->data;
	}
	
	dir = cmp_result < 0 ? LEFT : RIGHT;
	
	return FindNode(node->children[dir], elem_to_find, cmp_func, param);
}

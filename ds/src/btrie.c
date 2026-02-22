#include <stdlib.h> /* malloc, free */
#include <stddef.h> /* size_t */
#include <math.h>   /* pow */
#include <assert.h> /* assert */

#include "btrie.h"  /* api */
#include "utils.h"  /* DEBUG, bad mem */

typedef struct node
{
	struct node* nodes[2];
	int is_occupied;
}node_ty;

struct btrie
{
	node_ty* root;
	unsigned int num_bits;
};

static node_ty* CreateNode(void);
static void DestroyNodes(node_ty* node);
static int ReleaseIMP(node_ty** node, size_t height_, num_ty addr_);
static int GetIMP(node_ty** node, size_t idx, num_ty* num);
static int GetDirIMP(const num_ty* num, size_t idx);
static void RoundUpAtBitIMP(num_ty* num, size_t idx);
static size_t CountOccupied(node_ty* node, int height, int total_height);

btrie_ty* BTrieCreate(size_t num_bits_)
{
	num_ty num = 0;
	/* allocate btrie (ret) */
	btrie_ty* ret = (btrie_ty*)malloc(sizeof(btrie_ty));
	/* handle fail */
	if(!ret)
	{
		return NULL;
	}
	
	/* allocate ret->root */
	ret->root = CreateNode();
	/* handle fail */
	if (!ret->root)
	{
		free(ret);
		DEBUG_ONLY(ret = BAD_MEM(btrie_ty*););
		return NULL;
	}
	/* init num_bits */
	ret->num_bits = num_bits_;
	/* allocate num 0 and handle fail */
	if (FAIL == GetIMP(&ret->root, num_bits_, &num))
	{
		BTrieDestroy(ret);
		DEBUG_ONLY(ret = BAD_MEM(btrie_ty*););
	}

	/* return ret*/
	return ret;
}

static node_ty* CreateNode(void)
{
	/* allocate ret */
	node_ty* ret = (node_ty*)calloc(1, sizeof(node_ty));
	/* handle fail */
	if (!ret)
	{
		return NULL;
	}

	/* return ret */
	return ret;
}

void BTrieDestroy(btrie_ty* trie_)
{
	/* handle trie is NULL */
	if (!trie_)
	{
		return;
	}

	/* call DestroyNodes(root) */
	DestroyNodes(trie_->root);
	/* set trie root to bad mem */
	DEBUG_ONLY(trie_->root = BAD_MEM(node_ty*););
	/* free trie */
	free(trie_);
}

static void DestroyNodes(node_ty* node)
{
	/* return if node is NULL */
	if (!node)
	{
		return;
	}
	
	/* call DestroyNodes(left node) */
	DestroyNodes(node->nodes[0]);
	/* call DestroyNodes(right node) */
	DestroyNodes(node->nodes[1]);
	/* bad mem on left and right node */
	DEBUG_ONLY(node->nodes[0] = BAD_MEM(node_ty*););
	DEBUG_ONLY(node->nodes[1] = BAD_MEM(node_ty*););
	/* free node */
	free(node);
}

void BTrieRelease(btrie_ty* trie_, num_ty num_)
{
	/* assert */
	assert(trie_);

	/* ReleaseIMP(root, num of bits, num) */
	ReleaseIMP(&trie_->root, trie_->num_bits, num_);
}

static int ReleaseIMP(node_ty** node, size_t height_, num_ty num_)
{
	int dir = 0;
	/* if node is NULL */
	if (NULL == *node)
	{
		/* return fail */
		return FAIL;
	}
	/* set is_occupied to false */
	/* if height_ is 0 */
	if (0 == height_)
	{
		/* return SUCCESS */
		(*node)->is_occupied = FALSE;
		return SUCCESS;
	}
	
	/* get direction to go */	
	dir = GetDirIMP(&num_, height_ - 1);

	/* if R is success */
	if (SUCCESS == ReleaseIMP(&((*node)->nodes[dir]), height_ - 1, num_))
	{
		/* set is_occupied to false */
		(*node)->is_occupied = FALSE;
		return SUCCESS;
	}

	/* return fail */
	return FAIL;
}

num_ty BTrieGet(btrie_ty* trie_, num_ty num_)
{
	num_ty ret = num_;
	/* assert */
	assert(trie_);
	
	/* if GetIMP(root, num of bits, &ret) is false */
	if (FAIL == GetIMP(&trie_->root, trie_->num_bits, &ret))
	{
		/* return 0 */
		return 0;
	}
	
	/* return ret */
	return ret;
}

static int GetIMP(node_ty** node, size_t height_, num_ty* num)
{
	size_t dir = 0;
	int left_occupied = 0;
	int right_occupied = 0;
	/* if *node is NULL */
	if (NULL == *node)
	{
		/* create node */
		*node = CreateNode();
		/* handle failure */
		if (NULL == *node)
		{
			return FAIL;
		}
	}
		
	/* if node is occupied */
	if (1 == (*node)->is_occupied)
	{
		/* return FAIL */
		return FAIL;
	}
		
	/* if last bit */
	if (0 == height_)
	{
		/* set occupied true */
		(*node)->is_occupied = TRUE;
		/* return success */
		return SUCCESS;
	}
	
	/* get direction */
	dir = GetDirIMP(num, height_ - 1);
	/* if GetIMP return FAIL */
	if (FAIL == GetIMP((&(*node)->nodes[dir]), height_ - 1, num))
	{
		/* if dir is 1 */
		if (dir == 1)
		{
			/* handle FAIL */
		    return FAIL;
		}
		/* set cur bit to 1 and right bits to 0 */
		RoundUpAtBitIMP(num, height_ - 1);
		
		/* if GetIMP return FAIL */
		if (FAIL == GetIMP((&(*node)->nodes[1]), height_ - 1, num))
		{
			/* return FAIL */
			return FAIL;
		}	
	}
	/* check if there is left node and he is occupied*/
	left_occupied = ((*node)->nodes[0] != NULL) &&
					 ((*node)->nodes[0]->is_occupied);
	/* check if there is right node and he is occupied*/
	right_occupied = ((*node)->nodes[1] != NULL) && 
					 ((*node)->nodes[1]->is_occupied);
	/* set node is_ocuupied to 1 if left and right node exist and occupied  */
	(*node)->is_occupied = left_occupied && right_occupied;
		
	/* return SUCCESS */
	return SUCCESS;
}

static int GetDirIMP(const num_ty* num, size_t idx)
{
	return ((*num >> idx) & 1);
}

static void RoundUpAtBitIMP(num_ty* num, size_t idx)
{
	size_t i = 0;
	
	*num |= ((size_t)1 << idx);
	
	if (1 <= idx)
	{
		for (i = 0; i < idx; ++i)
		{
			*num &= ~((size_t)1 << i);
		}
	}
}

size_t BTrieCountAvailable(const btrie_ty* trie_)
{
	size_t cnt = 0;
	/* assert */
	assert(trie_);
	/* count = CountOccupied(root, 0) */
	cnt = CountOccupied(trie_->root, 0, trie_->num_bits);
	/* return max_numbers - cnt */
	return (1 << trie_->num_bits) - cnt;
}

static size_t CountOccupied(node_ty* node, int height, int total_height)
{
	/* if node is null return 0  */
	if (!node)
	{
		return 0;
	}
	
	/* if is occupied  */
	if (1 == node->is_occupied)
	{
		/* return 2^(total_height - height) */
		return (1 << (total_height - height));
	}
	
	/* return left nodes occupied + right nodes occupied*/
	return CountOccupied(node->nodes[0], height + 1, total_height) + 
		   CountOccupied(node->nodes[1], height + 1, total_height);
}

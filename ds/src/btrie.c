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
static int ReleaseIMP(node_ty** node, size_t idx, num_ty addr_);
static int GetIMP(node_ty** node, size_t idx, num_ty* num);
static int GetDirIMP(const num_ty* num, size_t idx);
static void SwitchBitIMP(num_ty* num, size_t idx);
static void SetBitIMP(num_ty* num, size_t idx, int bit);
static size_t CountOccupied(node_ty* node, int height, int total_height);

btrie_ty* BTrieCreate(size_t num_bits_)
{
	num_ty num = 0;
	node_ty* curr = NULL;
	/* allocate btrie (ret) */
	/* handle fail */
	btrie_ty* ret = (btrie_ty*)malloc(sizeof(btrie_ty));
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
	ret->num_bits = num_bits_;

	GetIMP(&ret->root, num_bits_, &num);

	/* return ret*/
	return ret;
}

static node_ty* CreateNode(void)
{
	/* allocate ret */
	/* handle fail */
	node_ty* ret = (node_ty*)calloc(1, sizeof(node_ty));
	if (!ret)
	{
		return NULL;
	}

	/* return ret */
	return ret;
}

void BTrieDestroy(btrie_ty* trie_)
{
	node_ty* node = NULL;
	/* handle trie is NULL */
	if (!trie_)
	{
		return;
	}
	
	/* root = get root */
	node = trie_->root;
	/* call DestroyNodes(root) */
	DestroyNodes(node);
	/* free trie */
	free(trie_);
	/* set trie to bad mem */
	DEBUG_ONLY(trie_ = BAD_MEM(btrie_ty*););
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

static int ReleaseIMP(node_ty** node, size_t idx, num_ty num_)
{
	int dir = 0;
	int status = 0;
	/* if node is NULL */
		/* return fail */
	if (NULL == *node)
	{
		return 0;
	}
	/* if idx is 0 */
		/* free node */
		/* set node to bad mem */
		/* return success */
	if (0 == idx)
	{
		free(*node);
		*node = NULL;
		return 1;
	}
	
	/* dir = GetDir(addr_, idx - 1) */	
	dir = GetDirIMP(&num_, idx - 1);
	/* status = ReleaseIMP(node[dir], idx - 1, addr_) */
	status = ReleaseIMP(&((*node)->nodes[dir]), idx - 1, num_);
	/* set is_occupied to 0 */
	(*node)->is_occupied = 0;
	
	/* if status is success */
	if (1 == status)
	{
		/* if other child is NULL */
		if ((*node)->nodes[!dir] == NULL)
		{
			/* free node */
			/* set node to bad mem */
			/* return success */
			free(*node);
			*node = NULL;
			return 1;
		}
	}
	/* return fail */
	return 0;
}

num_ty BTrieGet(btrie_ty* trie_, num_ty num_)
{
	num_ty ret = 0;
	node_ty* node = NULL;
	/* assert */
	assert(trie_);
	
	/* ret = addr_ */
	ret = num_;
	
	/* if GetIMP(root, num of bits, &ret) is false */
		/* return 0 */
	if (0 == GetIMP(&trie_->root, trie_->num_bits, &ret))
	{
		return 0;
	}
	
	/* return ret */
	return ret;
}

static int GetIMP(node_ty** node, size_t idx, num_ty* num)
{
	size_t dir = 0;
	int left_occupied = 0;
	int right_occupied = 0;
	int is_created = 0;
	int ret = 0;
	/* if *node is NULL */
		/* create node */
		/* handle failure */
	if (NULL == *node)
	{
		*node = CreateNode();
		if (NULL == *node)
		{
			return 0;
		}
		is_created = 1;
	}
		
	/* if node is occupied */
		/* return fail */
	if (1 == (*node)->is_occupied)
	{
		return 0;
	}
		
	/* if last bit */
		/* set occupied true */
		/* return success */
	if (0 == idx)
	{
		(*node)->is_occupied = 1;
		return 1;
	}
	
	/* dir = GetDir(num, idx - 1) */
	dir = GetDirIMP(num, idx - 1);
	/* ret = GetIMP(node[dir], idx - 1, num) */
	ret = GetIMP((&(*node)->nodes[dir]), idx - 1, num);
	
	/* if ret is fail */
	if (0 == ret)
	{
		/* if curr bit is 0 */
			/* set cur bit to 1 and right bits to 0 */
		/* else */
			/* set cur bit to 0 and right bits to 1 */
		SwitchBitIMP(num, idx - 1);
		if (2 <= idx)
		{
			SetBitIMP(num, idx - 2, dir);
		}
		/* ret = GetIMP(node[!dir], idx - 1, num) */
		/* ret is fail */
			/* return ret */
		ret = GetIMP((&(*node)->nodes[!dir]), idx - 1, num);
		if (0 == ret)
		{
			if (1 == is_created)
			{
				free(*node);
				*node = NULL;
			}
			return ret;
		}	
	}
	/* if both bits occupied */
		/* set node is ocuupied true */
	left_occupied = ((*node)->nodes[0] != NULL) &&
					 ((*node)->nodes[0]->is_occupied);
	right_occupied = ((*node)->nodes[1] != NULL) && 
					 ((*node)->nodes[1]->is_occupied);
	(*node)->is_occupied = left_occupied && right_occupied;
		
	/* return ret */
	return ret;
}

static int GetDirIMP(const num_ty* num, size_t idx)
{
	return ((*num >> idx) & 1);
}

static void SwitchBitIMP(num_ty* num, size_t idx)
{
	*num ^= ((size_t)1 << idx);
}

static void SetBitIMP(num_ty* num, size_t idx, int bit)
{
	size_t i = 0;
	for (i = 0; i <= idx; ++i)
	{
		if (bit)
		{
			*num |= ((size_t)1 << i);
		}
		else
		{
			*num &= ~((size_t)1 << i);
		}
	}
}

size_t BTrieCountAvailable(const btrie_ty* trie_)
{
	size_t cnt = 0;
	assert(trie_);
	/* count = CountOccupied(root, 0) */
	cnt = CountOccupied(trie_->root, 0, trie_->num_bits);
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
		/* return 2^(total_height - height) */
	if (1 == node->is_occupied)
	{
		return (1 << (total_height - height));
	}
	
	/* return left nodes occupied + right nodes occupied*/
	return CountOccupied(node->nodes[0], height + 1, total_height) + 
		   CountOccupied(node->nodes[1], height + 1, total_height);
}

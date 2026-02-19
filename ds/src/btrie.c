struct trie
{
	node_ty* root;
};

struct node
{
	struct node* nodes[2];
	unsigned int num_bits;
};

static node_ty* CreateNode(void);
static void Destroynodes(void);
static int GetIMP(node_ty** node, size_t idx, num_ty* num);

btrie_ty* BTrieCreate(size_t num_bits_)
{
	/* allocate btrie (ret) */
	/* handle fail */
	
	/* allocate ret->root */
	/* handle fail */
	
	/* curr = root[0] */
	
	/* loop from 0 to idx */
		/* curr[0] = createnode */
		/* handle fail */
		/* curr = curr[0] */
		
	/* set curr's is ocuppied to true */
	
	/* return ret*/
}

static node_ty* CreateNode(void)
{
	/* allocate ret */
	/* handle fail */
	
	/* return ret */
}

void BTrieDestroy(btrie_ty* trie_)
{
	/* handle trie is NULL */
	
	/* call DestroyNodes */
	
	/* free trie */
	/* set trie to bad mem */
}

static void Destroynodes(node_ty* node)
{
	/* handle node is NULL */
	
	/* call DestroyNodes(left node) */
	/* call DestroyNodes(right node) */
	/* free node */
	/* set node to bad mem */
}

void BTrieRelease(btrie_ty* trie_, num_ty addr_)
{
	/* assert */
	
	/* ret = addr_ */
	
	/* get root */
	
	/* if ReleaseIMP(root, num of bits, ret) */
}

static int ReleaseIMP(node_ty* node, size_t idx, num_ty addr_)
{
	/* if *node is NULL */
		/* return fail */
	
	/* dir = GetDir(addr_, idx - 1) */	
	/* ret = ReleaseIMP(node[dir], idx - 1, addr_) */
	
	/* if ret is success */
		/* if other child is NULL */
			/* free node */
			/* return ret */
	
	/* return fail */
}

num_ty BTrieGet(btrie_ty* trie_, num_ty addr_)
{
	/* assert */
	
	/* ret = addr_ */
	
	/* get root */
	
	/* if GetIMP(root, num of bits, &ret) is false */
		/* return 0 */
	
	/* return ret */
}

static int GetIMP(node_ty** node, size_t idx, num_ty* num)
{
	/* if *node is NULL */
		/* create node */
		/* handle failure */
		
	/* if node is occupied */
		/* return fail */
		
	/* if last bit */
		/* set occupied true */
		/* return success */
	
	/* dir = GetDir(num, idx - 1) */
	/* ret = GetIMP(node[dir], idx - 1, num) */
	
	/* if ret is fail */
		/* if curr bit is 0 */
			/* set cur bit to 1 and right bits to 0 */
		/* else */
			/* set cur bit to 0 and right bits to 1 */
		
		/* dir = GetDir(num, idx - 1) */
		/* ret = GetIMP(node[dir], idx - 1, num) */
		/* ret is fail */
			/* return ret */
			
	/* if both bits occupied */
		/* sry is ocuupied true */
		
	/* return ret */
}

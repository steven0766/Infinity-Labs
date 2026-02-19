static int GetIMP(node_ty** node, size_t idx, num_ty* num);

num_ty BTrieGet(btrie_ty* trie_, addr_ty addr_)
{
	/* assert */
	
	/* get root */
	
	/* if GetIMP(toor, num of bits, &addr) is false */
		/* return 0 */
		
	/* return addr */
}

static int GetIMP(node_ty** node, size_t idx, num_ty* num)
{
	/* if *node is NULL */
		/* create node */
		
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
			/* set cur bit to 0 */
			
		/* set occupied true */
		/* ret = GetIMP(node[dir], idx - 1, num) */
			
		/* ret is fail */
			/* return ret */
			
	/* if both bits occupied */
		/* sry is ocuupied true */
		
	/* return ret */
}

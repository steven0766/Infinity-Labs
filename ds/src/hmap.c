#include <stdlib.h>

#include "dlist.h"

struct hmap
{
	size_t capacity;
	hash_ty hash_func;
	void* hash_param;
	is_match_ty is_match;
	void* match param;
	dlist_ty* buckets[1];
};


hmap_ty* HMapCreate(size_t capacity_, hmap_hash_ty hash_func_, 
			const void* hash_param_, hmap_is_match_ty is_match_, 
						      const void* match_param_)
{	
	/* allocate ret(hmap): */
		/* size of offsetof(hmap, buckets) + sizeof(dlist_ty*) * capacity */

	/* assert */

	/* handle ret allocation failure */

	/* for each idx in buckets */
		/* create dlist */	
		/* handle failure */

	/* init ret with capacity, functions and params */

	/* return ret */
}

void HMapDestroy(hmap_ty* hmap_)
{
	/* if hmap_ is null return */

	/* for each lst in buckets */
		/* for each pair in lst */
			/* free pair */
		
		/* set bad to entry */
		/* destroy list */
	
	/* free hmap_ */
}


void HMapRemove(hmap_ty* hmap_, const void* key_)
{
	/* init iter pair_to_remove */
	
	/* assert */
	
	/* idx = GetEntry */
	
	/* lst = hmap_ buckets[idx] */
	
	/* create hmap_ param = hmap, key */
	
	/* to_remove = DListFind(Begin iter, End iter, IsPairMatch, hmap_param) */
	
	/* pair_to_remove = get data from iter to_remove */
	
	/* DListRemove(pair_to_remove) */
	
	/* set bad mem */
	
	/* free pair_to_remove */

}

int HMapInsert(hmap_ty* hmap_, const void* key_, void* data_)
{
	/* allocate pair */
	
	/* assert */
	
	/* handle pair failure */
	/* init pair */
	
	/* idx = GetEntry */
	
	/* lst = hmap buckets[idx] */
	
	/* insert pair before end of lst */
	/* handle failure */

	/* return success */
}

size_t HMapSize(const hmap_ty* hmap_)
{
	/* ret = 0 */
	
	/* assert */
	
	/* for each idx in buckets */
		/* ret =  DListSize of buckets[idx] */
	
	/* return ret */
}

int HMapIsEmpty(const hmap_ty* hmap_)
{
	/* assert */
	
	/* return HMapSize == 0 */
}

pair_ty HMapFind(hmap_ty* hmap, const void* key)
{
	/* init empty pair ret */
	
	/* assert */
	
	/* idx = GetEntry */
	
	/* lst = hmap buckets[idx] */
	
	/* init hmap_match_param */
	
	/* found iter =  DListFind(Begin iter, End iter, IsPairMatch, hmap_match_param) */
	
	/* if found iter is EndIter */
		/* return ret */
		
	/* ret = * get data from found iter */
	
	/* return ret */
}

int HMapForEach(hmap_ty* hmap, action_ty action, void* param)
{
	/* ret = SUCCESS */
	
	/* assert */
	
	/* for each idx in buckets */
		/* lst = hmap buckets[idx] */
		/* ret =  DListForEach( lst begin, lst end, PairAction,  hmap_action_param); */
		/* if ret is nonzero */
			/* return ret */
	
	/* return ret */
}

static size_t GetEntry(hmap_ty* hamp_, const void* key)
{
	/* assert */
	
	/* call to hash func on key */
	
	/* ret =  hash value % capacity */
	
	/* return ret */
}

static int IsPairMatch(const void* pair, void* hmap_param)
{
	/* assert */
	
	/* get key1 from pair */
	
	/* get key2 from hmap_param  */
	
	/* return hmap is_match(key1, key2, match_param) */
}

static int PairAction(void* pair, void* param)
{
	/* assert */
	
	/* extract hmap action param from param */
	/* extract hmap action function from param */
	
	/* ret = action_func(key from pair, data from pair, action param); */
	
	/* return ret */
}




























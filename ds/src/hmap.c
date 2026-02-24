#include <stdlib.h>		/* malloc, free, size_t */
#include <stddef.h>		/* offsetof */
#include <assert.h>		/* assert */

#include "hmap.h"
#include "dlist.h"
#include "utils.h"

struct hmap
{
	size_t capacity;
	hash_ty hash_func;
	const void* hash_param;
	is_match_ty is_match;
	const void* match_param;
	/* TODO: dlist_ty** */
	dlist_ty* buckets[1];
};

typedef struct hmap_match_param
{
	hmap_ty hmap;
	void* key;
}match_param_ty;

typedef struct hmap_action_param
{
	action_ty action_func;
	void* action_param;
}action_param_ty;

static size_t GetEntryIMP(hmap_ty* hmap_, const void* key);
static int IsPairMatchIMP(const void* pair, void* hmap_param);
static int PairActionIMP(void* pair, void* param);
static int FreePairIMP(void* data, void* param);

hmap_ty* HMapCreate(size_t capacity_, hash_ty hash_func_, 
			const void* hash_param_, is_match_ty is_match_, 
						      const void* match_param_)
{	
	size_t i = 0;
	/* allocate ret(hmap): */
		/* size of offsetof(hmap, buckets) + sizeof(dlist_ty*) * capacity */
	hmap_ty* ret = (hmap_ty*)malloc(offsetof(hmap_ty, buckets) + sizeof(dlist_ty*) * capacity_);
	/* assert */
	assert(hash_func_);
	assert(is_match_);
	assert(0 < capacity_);

	/* handle ret allocation failure */
	if (!ret)
	{
		return NULL;
	}
	
	/* init capacity */
	ret->capacity = capacity_;

	/* for each idx in buckets */
	for (i = 0; i < capacity_; ++i)
	{
		/* create dlist */	
		ret->buckets[i] = DListCreate();
		/* handle failure */
		if (!ret->buckets[i])
		{
			HMapDestroy(ret);
			return NULL;
		}
	}

	/* init ret with functions and params */
	ret->hash_func = hash_func_;
	ret->hash_param = hash_param_;
	ret->is_match = is_match_;
	ret->match_param = match_param_;

	/* return ret */
	return ret;
}

void HMapDestroy(hmap_ty* hmap_)
{
	size_t i = 0;
	dlist_ty* lst = NULL;
	/* if hmap_ is null return */
	if (!hmap_)
	{
		return;
	}

	/* for each lst in buckets */
	for (i = 0; i < hmap_->capacity; ++i)
	{
		lst = hmap_->buckets[i];
		/* for each pair in lst */
			/* free pair */
		DListForEach(DListBeginIter(lst), DListEndIter(lst), 
												FreePairIMP, NULL);
		
		/* destroy list */
		DListDestroy(lst);
		/* set bad mem to bucket */
		DEBUG_BAD_MEM(hmap_->buckets[i], dlist_ty*);
	}
	
	
	/* set bad mem to all ptrs */
	DEBUG_BAD_MEM(hmap_->hash_func, hash_ty);
	DEBUG_BAD_MEM(hmap_->hash_param, void*);
	DEBUG_BAD_MEM(hmap_->is_match, is_match_ty);
	DEBUG_BAD_MEM(hmap_->match_param, void*);
	/* free hmap_ */
	free(hmap_);
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
	size_t idx = 0;
	dlist_ty* lst = NULL;
	/* allocate pair */
	pair_ty* pair = (pair_ty*)malloc(sizeof(pair_ty));
	
	/* assert */
	assert(key_);
	
	/* handle pair failure */
	if (!pair)
	{
		/* init pair */
		free(pair);
		return FAIL;
	}
	
	/* idx = GetEntry */
	idx = GetEntryIMP(hmap_, key_);
	
	/* lst = hmap buckets[idx] */
	lst = hmap_->buckets[idx];
	
	/* insert pair before end of lst */
	/* handle failure */
	if (DListIterIsEqual(DListPushBack(hmap_->buckets[idx], pair),
													 DListEndIter(lst)))
	{
		free(pair);
		return FAIL;
	}

	/* return success */
	return SUCCESS;
}

size_t HMapSize(const hmap_ty* hmap_)
{
	/* ret = 0 */
	
	/* assert */
	
	/* for each idx in buckets */
		/* ret =  DListSize of buckets[idx] */
	
	/* return ret */
	return 0;
}

int HMapIsEmpty(const hmap_ty* hmap_)
{
	/* assert */
	
	/* return HMapSize == 0 */
	return 0;
}

pair_ty HMapFind(hmap_ty* hmap_, const void* key_)
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
	return 0;
}

static size_t GetEntryIMP(hmap_ty* hmap_, const void* key_)
{
	size_t ret = 0;
	/* assert */
	assert(hmap_);
	
	/* call to hash func on key */
	ret = hmap_->hash_func(key_, hmap_->hash_param);
	
	/* ret =  hash value % capacity */
	ret %= hmap_->capacity;
	
	/* return ret */
	return ret;
}

static int IsMatchPairIMP(const void* pair_, void* hmap_param_)
{
	/* assert */
	
	/* get key1 from pair */
	
	/* get key2 from hmap_param  */
	
	/* return hmap is_match(key1, key2, match_param) */
	return 0;
}

static int PairActionIMP(void* pair_, void* param_)
{
	/* assert */
	
	/* extract hmap action param from param */
	/* extract hmap action function from param */
	
	/* ret = action_func(key from pair, data from pair, action param); */
	
	/* return ret */
	return 0;
}

static int FreePairIMP(void* data_, void* ignored)
{
	(void)ignored;
	
	free(data_);
	
	return SUCCESS;
}




























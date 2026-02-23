/***********************************************************************
*File_name: dhcp.c
*Reviewer: Tomer
*Developer: Steve
*Version:  1.0
*Date:	   23.2.26
************************************************************************/

#include <stdlib.h>		/* malloc, free, size_t */
#include <assert.h>		/* assert */

#include "dhcp.h"
#include "btrie.h"		/* BTrieCreate, BTrieDestroy */
#include "utils.h"		/* BAD_MEM, DEBUG_ONLY */

struct dhcp
{
	btrie_ty* trie;
	addr_ty subnet_id;
	addr_ty subnet_mask;
};

dhcp_ty* DhcpCreate(addr_ty subnet_id_, unsigned int host_numb_)
{
	/* allocate dhcp */
	dhcp_ty* ret = (dhcp_ty*)malloc(sizeof(dhcp_ty));
	/* assert host_numb_ max and above 0 */
	assert(0 < host_numb_);
	assert(64 >= host_numb_);
	
	/* handle failure */
	if (NULL == ret)
	{
		return NULL;
	}
	
	/* init subnet_id and subnet_mask */
	ret->subnet_id = subnet_id_;
	ret->subnet_mask = (~(size_t)0 << host_numb_);
	
	/* allocate trie */
	ret->trie = BTrieCreate(host_numb_);
	/* handle failue */
	if (NULL == ret->trie)
	{
		free(ret);
		return NULL;
	}
	
	/* BTrieGet max (broadcast address */
	/* BTrieGet max - 1 (server address) */
	if (0 == BTrieGet(ret->trie, (~(size_t)0)) || 
		0 == BTrieGet(ret->trie, (~(size_t)0 << 1)))
	{
		/* handle failure */
		BTrieDestroy(ret->trie);
		free(ret);
		return NULL;
	}
	
	/* return dhcp */
	return ret;
}

void DhcpDestroy(dhcp_ty* dhcp_)
{
	/* return if dhcp is NULL */
	if (NULL == dhcp_)
	{
		return;
	}
	
	/* BTrieDestroy */
	BTrieDestroy(dhcp_->trie);
	/* bad mem on btrie */
	DEBUG_ONLY(dhcp_->trie = BAD_MEM(btrie_ty*););
	
	/* free dhcp */
	free(dhcp_);
}

addr_ty DhcpAllocateIp(dhcp_ty* dhcp_, addr_ty addr_)
{
	/* init ret as addr */
	addr_ty ret = addr_;
	
	/* assert dhcp */
	assert(dhcp_);
	/* assert addr_ prefix */
	assert(dhcp_->subnet_id == (dhcp_->subnet_mask & addr_));
	
	/* add ret to trie */
	ret = BTrieGet(dhcp_->trie, ret);
	
	/* if ret is 0 */
	if (0 == ret)
	{
		/* add 0 to trie */
		ret = BTrieGet(dhcp_->trie, ret);
		/* if ret still 0 */
		if (0 == ret)
		{
			/* return 0 */
			return 0;
		}
	}
			
	/* return ret with subnet_id */
	return ret | dhcp_->subnet_id;
}

void DhcpFreeIp(dhcp_ty* dhcp_, addr_ty addr_)
{
	/* assert dhcp */
	assert(dhcp_);
	/* assert addr_ prefix */
	assert(dhcp_->subnet_id == (dhcp_->subnet_mask & addr_));
	
	/* BTrieRelease addr_ */
	BTrieRelease(dhcp_->trie, addr_);
}

size_t DhcpCountFree(const dhcp_ty* dhcp_)
{
	/* assert */
	assert(dhcp_);
	
	/* BTrieCountAvailable */
	return BTrieCountAvailable(dhcp_->trie);
}

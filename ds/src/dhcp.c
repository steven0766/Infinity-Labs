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
	/* assert host_numb_ max */
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
	if (0 == BTrieGet(ret->trie, (~(size_t)0)))
	{
		/* handle failure */
		BTrieDestroy(ret->trie);
		return NULL;
	}
	
	/* BTrieGet max - 1 (server address) */
	if (0 == BTrieGet(ret->trie, (~(size_t)0 << 1)))
	{
		/* handle failure */
		BTrieDestroy(ret->trie);
		return NULL;
	}
	
	/* return dhcp */
	return ret;
}

void DhcpDestroy(dhcp_ty* dhcp_)
{
	/* return if dhcp is NULL */
	
	/* BTrieDestroy */
	/* bad mem on btrie */
	
	/* free dhcp */
}

addr_ty DhcpAllocateIp(dhcp_ty* dhcp_, addr_ty addr_)
{
	/* assert addr_ prefix */
	/* assert dhcp */
	
	/* init ret as addr */
	
	/* add ret to trie */
	
	/* if ret is 0 */
		/* add 0 to trie */
			/* return 0 */
			
	/* return ret with subnet_id */
}

void DhcpFreeIp(dhcp_ty* dhcp_, addr_ty addr_)
{
	/* assert addr_ prefix */
	/* assert dhcp */
	
	/* BTrieRelease addr_ */
}

size_t DhcpCountFree(const dhcp_ty* dhcp_)
{
	/* assert */
	
	/* BTrieCountAvailable */
}

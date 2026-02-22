#include "dhcp.h"
#include "utils.h"

struct dhcp
{
	btrie_ty* trie;
	addr_ty subnet_id;
	addr_ty subnet_mask;
};

dhcp_ty* DhcpCreate(addr_ty subnet_id_, unsigned int host_numb_)
{
	/* assert host_numb_ max */
	
	/* allocate dhcp */
	/* handle failure */
	
	/* allocate trie */
	/* handle failue */
	
	/* init subnet_id and subnet_mask */
	
	/* BTrieGet 0 */
	/* handle failure */
	
	/* BTrieGet max (broadcast address */
	/* handle failure */
	
	/* BTrieGet max - 1 (server address) */
	/* handle failure */
	
	/* return dhcp */
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
	/* assert subnet id == addr & subnet mask */
	
	/*  */
	
	/*  */
}

void DhcpFreeIp(dhcp_ty* dhcp_, addr_ty addr_)
{

}

size_t DhcpCountFree(const dhcp_ty* dhcp_)
{
	/* assert */
	
	/* BTrieCountAvailable */
}

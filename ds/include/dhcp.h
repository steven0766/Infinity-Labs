#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h>     /* size_t */

typedef struct dhcp dhcp_ty;
typedef size_t addr_ty;

/*********************************************************************************************
TODO: 1. Do we receive subnet_id_ or bit_num_?  bit_num_ (num_of_available_bits_in_address_not_the_prefix)
      2. What's Server Address? after prefix almost all 1ns and the last bit is 0.   
      if prefix is 192.168 you should preallocate N 192.168.0.0   S 192.168.255.254   B 192.168.255.255 
      3. In AllocateIP and FreeIP do we receive full address or only host part? full address, assert for wrong prefix
*********************************************************************************************/
/*
 * Allocates 
 * 		X.000...000 (Network Address)
 * 		X.111...110 (Server Address)
 * 		X.111...111 (Broadcast Address)
 * On failure returns NULL
 * Undefined Behaviour:
 *      host_numb_ is larger than IPV4 num bits together
 * O(log(n)), n - number of available addresses
*/
dhcp_ty* DhcpCreate(addr_ty subnet_id_, unsigned int host_numb_);

/*
*   NULL is allowed
*   O(n)
*/
void DhcpDestroy(dhcp_ty* dhcp_);

/*
 * Tries to allocate the requested address,
 * 		If "addr_" is not available, tries to allocated the closest bigger available address
 * 		If "addr_" is 0, arbitrary available address will be allocated
 *      On failure, returns 0
 * Undefined behaviour:
 * 		"dhcp_" is NULL
 * O(log(n)), n - number of available addresses
*/
addr_ty DhcpAllocateIp(dhcp_ty* dhcp_, addr_ty addr_);

/*
 * Frees "addr_" from "dhcp_"
 * Undefined behaviour:
 * 		"dhcp_" is NULL
 * 		addr_ wasn't previously allocated
 * O(log(n)), n - number of available addresses
*/
void DhcpFreeIp(dhcp_ty* dhcp_, addr_ty addr_);

/*
 * Undefined behaviour:
 * 		"dhcp_" is NULL
 *  O(n)
*/
size_t DhcpCountFree(const dhcp_ty* dhcp_);

#endif /* __DHCP_H__ */


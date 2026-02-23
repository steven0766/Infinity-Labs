#include <stddef.h>		/* size_t */

#include "test_utils.h"
#include "dhcp.h"

#define SUBNET_ID		((size_t)1 << 12)	
#define NUM_BITS		8
#define ARRAY_LEN(arr)	(sizeof(arr) / sizeof(arr[0]))

static void TestDhcpCreate(void);
static void TestDhcpAllocateIp(void);

int main()
{
	TestSuiteBegin("dhcp");
	
	TestPrintSection("dhcp create");
	TestDhcpCreate();
	
	TestPrintSection("dhcp allocate");
	TestDhcpAllocateIp();
	
	return 0;
}

static void TestDhcpCreate(void)
{
	dhcp_ty* dhcp = DhcpCreate(SUBNET_ID, NUM_BITS);
	
	if (!ASSERT_NOT_NULL(dhcp))
	{
		return;
	}
	
	DhcpDestroy(dhcp);
}

static void TestDhcpAllocateIp(void)
{
	dhcp_ty* dhcp = DhcpCreate(SUBNET_ID, NUM_BITS);
	addr_ty arr[5] = {1, 2, 100, 252, 253};
	addr_ty result = 0;
	addr_ty expected[5] = {1, 2, 100, 252, 253};
	size_t i = 0;
	
	if (!ASSERT_NOT_NULL(dhcp))
	{
		return;
	}

	for (i = 0; i < ARRAY_LEN(arr); ++i)
	{
		result = DhcpAllocateIp(dhcp, SUBNET_ID | arr[i]);
		ASSERT_SIZE(SUBNET_ID | expected[i], result);
	}

	DhcpDestroy(dhcp);
}

#include <stddef.h>		/* size_t */

#include "test_utils.h"
#include "dhcp.h"

#define SUBNET_ID		((size_t)1 << 12)	
#define NUM_BITS		8

static void TestDhcpCreate(void);

int main()
{
	TestSuiteBegin("dhcp");
	
	TestPrintSection("dhcp create");
	TestDhcpCreate();
	
	return 0;
}

static void TestDhcpCreate(void)
{
	dhcp_ty* dhcp = DhcpCreate(SUBNET_ID, NUM_BITS);
	
	if (ASSERT_NOT_NULL(dhcp))
	{
		return;
	}
	
	DhcpDestroy(dhcp);
}

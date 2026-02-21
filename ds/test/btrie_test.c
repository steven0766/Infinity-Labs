#include <stdio.h>

#include "test_utils.h"
#include "btrie.h"

static void TestBTrieCreate(void);
static void TestBTrieGet(void);

int main()
{
	TestSuiteBegin("btrie");
	
	TestPrintSection("btrie create");
	TestBTrieCreate();
	
	TestPrintSection("btrie get");
	TestBTrieGet();
	
	return 0;
}

static void TestBTrieCreate(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	
	ASSERT_NOT_NULL(btrie);
	ASSERT_INT(1, BTrieCountAvailable(btrie));
	
	BTrieDestroy(btrie);
}

static void TestBTrieGet(void)
{
	size_t size = 8;
	num_ty arr[20] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 255, 254,
					  253, 252, 251, 255, 254, 253, 252, 251};
	num_ty expected[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255,
						   254, 253, 252, 251, 250, 249, 248, 247, 246};
	num_ty actual = 0;
	btrie_ty* btrie = BTrieCreate(size);
	size_t i = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i)
	{
		actual = BTrieGet(btrie, arr[i]);
		ASSERT_INT(expected[i], actual);
	}
	
	BTrieDestroy(btrie);
}

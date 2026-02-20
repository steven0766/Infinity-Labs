#include <stdio.h>

#include "test_utils.h"
#include "btrie.h"

static void TestBTrieCreate(void);

int main()
{
	TestSuiteBegin("btrie");
	
	TestPrintSection("btrie create");
	TestBTrieCreate();
	
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

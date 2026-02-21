#include <stdio.h>

#include "test_utils.h"
#include "btrie.h"

static void TestBTrieCreate(void);
static void TestBTrieGet(void);
static void TestBTrieRelease(void);

#define ARR_SIZE(arr)					sizeof((arr)) / sizeof((arr)[0])

int main()
{
	TestSuiteBegin("btrie");
	
	TestPrintSection("btrie create");
	TestBTrieCreate();
	
	TestPrintSection("btrie get");
	TestBTrieGet();
	
	TestPrintSection("btrie release");
	TestBTrieRelease();
	
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
	btrie_ty* btrie = BTrieCreate(size);
	num_ty arr[20] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 255, 254,
					  253, 252, 251, 255, 254, 253, 252, 251};
	num_ty expected[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255,
						   254, 253, 252, 251, 250, 249, 248, 247, 246};
	num_ty actual = 0;
	size_t i = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < ARR_SIZE(expected); ++i)
	{
		actual = BTrieGet(btrie, arr[i]);
		ASSERT_INT(expected[i], actual);
	}
	
	ASSERT_INT(ARR_SIZE(arr) + 1, BTrieCountAvailable(btrie));
	
	BTrieDestroy(btrie);
}

static void TestBTrieRelease(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	num_ty arr[20] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 255, 254,
					  253, 252, 251, 255, 254, 253, 252, 251};
	num_ty release[10] = {6, 7, 8, 9, 10, 255, 254, 253, 252, 251};
	size_t i = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < ARR_SIZE(arr); ++i)
	{
		BTrieGet(btrie, arr[i]);
	}
	
	for (i = 0; i < ARR_SIZE(release); ++i)
	{
		BTrieRelease(btrie, release[i]);
	}
	
	ASSERT_INT(ARR_SIZE(arr) - ARR_SIZE(release) + 1,
						    BTrieCountAvailable(btrie));
						    
	BTrieDestroy(btrie);
}

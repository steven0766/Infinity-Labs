#include <stdio.h>  /* printf */
#include <math.h>   /* pow */

#include "test_utils.h"
#include "btrie.h"

static void TestBTrieCreate(void);
static void TestBTrieGet(void);
static void TestBTrieGetFull(void);
static void TestBTrieRelease(void);

#define ARR_SIZE(arr)					sizeof((arr)) / sizeof((arr)[0])

int main()
{
	TestSuiteBegin("btrie");
	
	TestPrintSection("btrie create");
	TestBTrieCreate();
	
	TestPrintSection("btrie get");
	TestPrintSubSection("simple add address");
	TestBTrieGet();
	
	TestPrintSubSection("add address on limit");
	TestBTrieGetFull();
	
	TestPrintSection("btrie release");
	TestBTrieRelease();
	
	return 0;
}

static void TestBTrieCreate(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	
	ASSERT_NOT_NULL(btrie);
	ASSERT_INT((1 << size) - 1, BTrieCountAvailable(btrie));
	
	BTrieDestroy(btrie);
}

static void TestBTrieGet(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	num_ty arr[20] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 255, 254,
					  253, 252, 251, 255, 254, 253, 252, 251};
	num_ty expected[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255,
						   254, 253, 252, 251, 0, 0, 0, 0, 0};
	size_t cnt = 0;
	num_ty actual = 0;
	size_t i = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < ARR_SIZE(expected); ++i)
	{
		actual = BTrieGet(btrie, arr[i]);
		ASSERT_INT(expected[i], actual);
		cnt += (0 != actual);
	}
	
	ASSERT_INT((1 << size) - cnt - 1, BTrieCountAvailable(btrie));
	
	BTrieDestroy(btrie);
}

static void TestBTrieGetFull(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	size_t i = 0;
	size_t max_cap = pow(2, size);
	num_ty stress_arr[5] = {1, 57, 150, 230, 255};
	int result = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < max_cap; ++i)
	{
		BTrieGet(btrie, i);
	}
	
	for (i = 0; i < ARR_SIZE(stress_arr); ++i)
	{
		result = BTrieGet(btrie, stress_arr[i]);
		ASSERT_INT(0, result);
	}
}

static void TestBTrieRelease(void)
{
	size_t size = 8;
	btrie_ty* btrie = BTrieCreate(size);
	num_ty arr[20] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 255, 254,
					  253, 252, 251, 255, 254, 253, 252, 251};
	num_ty release[10] = {6, 7, 8, 9, 10, 255, 254, 253, 252, 251};
	size_t cnt = 0;
	size_t i = 0;
	
	ASSERT_NOT_NULL(btrie);
	
	for (i = 0; i < ARR_SIZE(arr); ++i)
	{
		if (0 != BTrieGet(btrie, arr[i]))
		{
			++cnt;
		}
	}
	
	for (i = 0; i < ARR_SIZE(release); ++i)
	{
		BTrieRelease(btrie, release[i]);
		--cnt;
	}
	
	ASSERT_INT((1 << size) - cnt - 1, BTrieCountAvailable(btrie));
						    
	BTrieDestroy(btrie);
}

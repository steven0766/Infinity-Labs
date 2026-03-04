#include <string.h>		/* strcmp */

#include "hmap.h"
#include "utils.h"
#include "test_utils.h"

#define CAP					11

static void TestHMapCreate(void);
static void TestHMapInsertAndRemove(void);
static void TestHMapFind(void);
static void TestHMapForEach(void);
static void TestHMapForEachStop(void);

static size_t Hash(const void* key, const void* param);
static int IsMatch(const void* key_a, const void* key_b, const void* param);
static int CountAction(const void* key, void* data, void* param);
static int StopAction(const void* key, void* data, void* param);

int main()
{
	TestSuiteBegin("hmap test begin");
	
	TestPrintSection("Test create");
	TestHMapCreate();
	
	TestPrintSection("Test insert and remove");
	TestHMapInsertAndRemove();
	
	TestPrintSection("Test find");
	TestHMapFind();
	
	TestPrintSection("Test foreach");
	TestHMapForEach();

	TestPrintSection("Test foreach stop");
	TestHMapForEachStop();
	
	return TestSuiteEnd();
}

static void TestHMapCreate(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);
	
	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}
	
	ASSERT_INT(TRUE, HMapIsEmpty(map));
	
	HMapDestroy(map);
}

static void TestHMapInsertAndRemove(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);
	size_t i = 0;
	char* keys[6] = {"1000", "2455", "378676", "475445", "585", "6"};
	char* val[6] = {"asd", "zxc", "qwe", "ter", "gfd", "fds"};

	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}
	
	/* add some pairs */
	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		ASSERT_INT(SUCCESS, HMapInsert(map, keys[i], val[i]));
	}
	
	ASSERT_SIZE(ARRAY_LEN(keys), HMapSize(map));
	
	/* remove something not found */
	HMapRemove(map, "0");
	ASSERT_SIZE(ARRAY_LEN(keys), HMapSize(map));
	
	/* remove all pairs */
	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		HMapRemove(map, keys[i]);
		ASSERT_SIZE(ARRAY_LEN(keys) - i - 1, HMapSize(map));
	}
	
	HMapDestroy(map);
}

static void TestHMapFind(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);
	pair_ty pair = {0};

	char* keys[3] = {"1000", "2455", "378676"};
	char* vals[3] = {"asd", "zxc", "qwe"};

	size_t i = 0;

	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}

	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		ASSERT_INT(SUCCESS, HMapInsert(map, keys[i], vals[i]));
	}

	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		pair = HMapFind(map, keys[i]);

		ASSERT_PTR(keys[i], pair.key);
		ASSERT_PTR(vals[i], pair.data);
	}

	pair = HMapFind(map, "128");

	ASSERT_NULL(pair.key);
	ASSERT_NULL(pair.data);

	HMapDestroy(map);
}

static void TestHMapForEach(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);

	char* keys[4] = {"100", "200", "300", "400"};
	char* vals[4] = {"a", "b", "c", "d"};

	size_t counter = 0;
	size_t i = 0;

	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}

	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		ASSERT_INT(SUCCESS, HMapInsert(map, keys[i], vals[i]));
	}

	ASSERT_INT(SUCCESS, HMapForEach(map, CountAction, &counter));

	ASSERT_SIZE(ARRAY_LEN(keys), counter);

	HMapDestroy(map);
}

static void TestHMapForEachStop(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);

	char* keys[4] = {"1", "2", "3", "4"};
	char* vals[4] = {"a", "b", "c", "d"};

	size_t counter = 0;
	size_t i = 0;

	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}

	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		ASSERT_INT(SUCCESS, HMapInsert(map, keys[i], vals[i]));
	}

	ASSERT_INT(FAIL, HMapForEach(map, StopAction, &counter));

	ASSERT_INT(2, counter);

	HMapDestroy(map);
}

static size_t Hash(const void* key, const void* param)
{
	size_t hash = 5381;
	char* str = (char*)key;
	char c = 0;
	
	(void)param;
	
	while ((c = *str++)) 
	{
        	hash = ((hash << 5) + hash) + c;
    }
    	
    return hash;
}

static int IsMatch(const void* key_a, const void* key_b, const void* param)
{
	(void)param;
	
	return strcmp(key_a, key_b);
}

static int CountAction(const void* key, void* data, void* param)
{
	size_t* counter = (size_t*)param;

	(void)key;
	(void)data;

	++(*counter);

	return SUCCESS;
}

static int StopAction(const void* key, void* data, void* param)
{
	size_t* counter = (size_t*)param;

	(void)key;
	(void)data;

	++(*counter);

	if (*counter == 2)
	{
		return FAIL;
	}

	return SUCCESS;
}

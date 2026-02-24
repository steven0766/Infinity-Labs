#include <string.h>		/* strcmp */

#include "hmap.h"
#include "utils.h"
#include "test_utils.h"

#define CAP					11

static void TestHMapCreate(void);
static void TestHMapInsert(void);

static size_t Hash(const void* key, const void* param);
static int IsMatch(const void* key_a, const void* key_b, const void* param);

int main()
{
	TestSuiteBegin("hmap test begin");
	
	TestPrintSection("Test create");
	TestHMapCreate();
	
	TestPrintSection("Test insert");
	TestHMapInsert();
	
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

static void TestHMapInsert(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);
	size_t i = 0;
	int keys[6] = {1000, 2455, 378676, 475445, 585, 6};
	char* val[6] = {"asd", "zxc", "qwe", "ter", "gfd", "fds"};

	if (!ASSERT_NOT_NULL(map))
	{
		return;
	}

	for (i = 0; i < ARRAY_LEN(keys); ++i)
	{
		ASSERT_INT(SUCCESS, HMapInsert(map, &keys[i], val[i]));
	}
	
	ASSERT_SIZE(ARRAY_LEN(keys), HMapSize(map));
	
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

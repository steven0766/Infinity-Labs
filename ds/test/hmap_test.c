#include <string.h>		/* strcmp */

#include "hmap.h"
#include "test_utils.h"

#define CAP					8

static void TestHMapCreate(void);
static size_t Hash(const void* key, const void* param);
static int IsMatch(const void* key_a, const void* key_b, const void* param);

int main()
{
	TestHMapCreate();
	
	return 0;
}

static void TestHMapCreate(void)
{
	hmap_ty* map = HMapCreate(CAP, Hash, NULL, IsMatch, NULL);
	
	ASSERT_NOT_NULL(map);
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

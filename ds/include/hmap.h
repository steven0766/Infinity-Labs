#ifndef __HMAP_H__
#define __HMAP_H__

typedef struct hmap hmap_ty;
typedef struct pair
{
	void* first;
	void* second;
}pair_ty;

/* hash function */
typedef size_t (*hash_ty) (const void* key, const void* param );

/* is_match func for key */
typedef int (*is_match_ty) (const void* key_a, const void* key_b, const void* param);

/*returns non_zero if wants to stop*/
typedef int (*action_ty) (const void* key, void* data, void* param);


hmap_ty* HMapCreate(size_t capacity, hash_ty hash_func, const void* hash_param, is_match_ty is_match, const void* match_param);

void HMapDestroy(hmap_ty* hmap_);

void HMapRemove(hmap_ty* hmap_, const void* key);

int HMapInsert(hmap_ty* hmap, const void* key, void* data);

size_t HMapSize(const hmap_ty* hmap_);
int HMapIsEmpty(const hmap_ty* hmap_);
pair_ty HMapFind(hmap_ty* hmap, const void* key);

/*order is not specified*/
int HMapForEach(hmap_ty* hmap, action_ty action, void* param);

/*

*/

#endif /* __HMAP_H__ */

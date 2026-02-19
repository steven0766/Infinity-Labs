#ifndef __BIT_ARR__
#define __BIT_ARR__

#include <stddef.h>

typedef struct btrie btrie_ty;
typedef size_t addr_ty;

/****************************************************************************** 
* 	Description:
* 		Creates an empty "btrie"
* 	Return value:
* 		On success, a pointer to the created trie
* 		On failure, NULL
*   Undefined behavior:
* 		num_bits == 0
* 	O(1)
******************************************************************************/
btrie_ty* BTrieCreate(size_t num_bits);

/****************************************************************************** 
* 	Description:
* 		Destroys the "trie"
* 		No operations performed if "trie" is NULL
* 	Return value:
* 		None
*   O(n)
******************************************************************************/
void BTrieDestroy(btrie_ty* trie);

/****************************************************************************** 
* 	Description:
* 		Frees "addr" from "trie"
* 	Return value:	
* 		None
* 	Undefined behaviour:
* 		None
*   O(1)
******************************************************************************/
void BTrieFree(btrie_ty* trie, addr_ty* addr);

/****************************************************************************** 
* 	Description:
* 		Allocates a new available address.
* 	Return value:	
* 		On success, returns the nearest address available to "addr_"
*       On fail, returns address with 0 value.
* 	Undefined behaviour:
        "trie_" is NULL.
*   O(1)
******************************************************************************/
addr_ty* BTrieGet(btrie_ty* trie, addr_ty* addr);

/****************************************************************************** 
* 	Description:
*       Count all available addresses in trie.
* 	Return value:
*       Number of available addresses in trie.
*   Undefined behaviour:
*       "trie_" is NULL
*   O(n)
******************************************************************************/
size_t BTrieAvailable(btrie_ty* trie);

#endif /* __BIT_ARR__ */

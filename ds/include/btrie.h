#ifndef __BTRIE_H__
#define __BTRIE_H__

#include <stddef.h>     /* size_t */

typedef struct btrie btrie_ty;
typedef size_t num_ty;
 
/*
* On failure returns NULL
* O(1)
*/
btrie_ty* BTrieCreate(size_t num_bits_);

/*
*   NULL is allowed
*   
*/
void BTrieDestroy(btrie_ty* trie_);

/*
*   Frees "addr" from "trie"
*   O(1)
*/
void BTrieRelease(btrie_ty* trie_, num_ty addr_);

/*
* 	Return value:	
* 		On success, returns the nearest address available to "addr_"
*       On fail, returns address with 0 value.
* 	Undefined behaviour:
*        "trie_" is NULL
*   O(1)
*/
num_ty BTrieGet(btrie_ty* trie_, num_ty addr_);
    
/*
* 	Undefined behaviour:
*        "trie_" is NULL
*/
size_t BTrieCountAvailable(const btrie_ty* trie_);

#endif /* __BTRIE_H__ */

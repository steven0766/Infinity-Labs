/***********************************************************************
*File_name: fsa.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   14.01.26
************************************************************************/

#include <assert.h> /* assert */

#include "fsa.h"

#define WORD_SIZE					sizeof(void*)

static size_t Align(size_t size);

struct fsa
{
	size_t next_free;
};

size_t FsaSuggestedSize(size_t n_blocks, size_t block_size)
{
	assert(block_size);
	
	return (Align(block_size) * n_blocks) +
						 Align(sizeof(fsa_ty));
}


fsa_ty* FsaInit(void* pool_p, size_t pool_size, size_t block_size)
{
	size_t blocks_count = 0;
	size_t header_size = 0;
	size_t current_offset = 0;
	char* block_runner = NULL;
	fsa_ty* fsa = NULL;

	assert(NULL != pool_p);
	assert(0 != block_size);

	block_size = Align(block_size);
	header_size = Align(sizeof(fsa_ty));

	blocks_count = (pool_size - header_size) / block_size;

	fsa = (fsa_ty*)pool_p;
	fsa->next_free = header_size;

	block_runner = (char*)pool_p + header_size;
	current_offset = header_size;

	while (1 < blocks_count)
	{
		*(size_t*)block_runner = current_offset + block_size;

		block_runner += block_size;
		current_offset += block_size;
		--blocks_count;
	}

	*(size_t*)block_runner = 0;

	return fsa;
}

void* FsaAlloc(fsa_ty* fsa)
{
    void* block = NULL;
    size_t offset = 0;

    assert(NULL != fsa);

    if (0 == fsa->next_free)
    {
        return NULL;
    }

    block = (char*)fsa + fsa->next_free;
    offset = *(size_t*)block;
    fsa->next_free = offset;

    return block;
}

void FsaFree(fsa_ty* fsa, void* p)
{
	size_t prev_free = 0;
	
	assert(NULL != fsa);
	
	if (NULL == p)
	{
		return;
	}
	
	prev_free = fsa->next_free;
	fsa->next_free = (size_t)((char*)p - (char*)fsa);
	*(size_t*)p = prev_free;
}

size_t FsaAvailableBlocks(fsa_ty* fsa)
{
    size_t count = 0;
    void* runner = NULL;
    size_t offset = 0;

    assert(NULL != fsa);

    offset = fsa->next_free;

    while (0 != offset)
    {
        runner = (char*)fsa + offset;
        offset = *(size_t*)runner;
        ++count;
    }

    return count;
}

static size_t Align(size_t size)
{
    return (size + WORD_SIZE - 1) & ~(WORD_SIZE - 1);
}


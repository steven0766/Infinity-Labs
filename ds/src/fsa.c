/***********************************************************************
*File_name: fsa.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   14.01.26
************************************************************************/

#include <assert.h> /* assert */

#include "fsa.h"

struct fsa
{
	void* free_space;
};

size_t FsaSuggestedSize(size_t n_blocks, size_t block_size)
{
    size_t eff_size;
    size_t size;
    size_t rem;

    if (block_size < sizeof(void*))
    {
        eff_size = sizeof(void*);
    }
    else
    {
        eff_size = block_size;
    }

    size = sizeof(fsa_ty) + n_blocks * eff_size;

    rem = size % sizeof(void*);
    if (0 != rem)
    {
        size += sizeof(void*) - rem;
    }

    return size;
}

fsa_ty* FsaInit(void* pool_p, size_t pool_size, size_t block_size)
{
    fsa_ty* fsa;
    size_t eff_size;
    char* runner;
    char* end;
    void* next;

    assert(NULL != pool_p);
    assert(0 != block_size);

    if (block_size < sizeof(void*))
    {
        eff_size = sizeof(void*);
    }
    else
    {
        eff_size = block_size;
    }

    fsa = (fsa_ty*)pool_p;

    runner = (char*)pool_p + sizeof(fsa_ty);
    end = (char*)pool_p + pool_size;

    fsa->free_space = runner;

    while (runner + eff_size <= end)
    {
        next = runner + eff_size;

        if ((char*)next + eff_size <= end)
        {
            *(size_t*)runner = (size_t)((char*)next - (char*)fsa);
        }
        else
        {
            *(void**)runner = NULL;
        }

        runner = (char*)next;
    }

    return fsa;
}

void* FsaAlloc(fsa_ty* fsa)
{
    void* block;
    size_t offset = 0;

    assert(NULL != fsa);

    if (NULL == fsa->free_space)
    {
        return NULL;
    }

    block = fsa->free_space;
    offset = *(size_t*)block;
	fsa->free_space = offset ? (char*)fsa + offset : NULL;


    return (char*)block;
}

void FsaFree(fsa_ty* fsa, void* p)
{
	size_t offset = 0;	
	
    assert(NULL != fsa);
    assert(NULL != p);

    offset = (size_t)((char*)fsa->free_space - (char*)fsa);
	*(size_t*)p = offset;
	fsa->free_space = p;

}

size_t FsaAvailableBlocks(fsa_ty *fsa)
{
    size_t count = 0;
    void* runner;
    size_t offset = 0;

    assert(NULL != fsa);

    runner = fsa->free_space;
    while (NULL != runner)
    {
        offset = *(size_t*)runner;
		runner = offset ? (char*)fsa + offset : NULL;

        ++count;
    }

    return count;
}

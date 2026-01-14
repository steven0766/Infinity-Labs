#include <stdio.h> /* printf */
#include <stdlib.h>/* malloc, free */

#include "fsa.h"

#define BLOCK_NUM   20

typedef struct
{
	char* name;
	size_t age;
}human_ty;

typedef struct
{
	int num;
}num_ty;

static void TestFsaInit(void);
static void TestAllocAndFree(void);
static void TestStressPool(void);

static void PrintError(char* test_name, char* problem);

int main()
{
	TestFsaInit();
	TestAllocAndFree();
	TestStressPool();
	
	return 0;
}

static void TestFsaInit(void)
{
	size_t block_size = sizeof(human_ty);
	size_t total_size = FsaSuggestedSize(BLOCK_NUM, block_size);
	fsa_ty* pool = NULL;
	char* people = (char*)malloc(total_size * sizeof(char));
	if (NULL == people)
	{
		return;
	}
	
	pool = FsaInit(people, total_size, block_size);
	if (BLOCK_NUM != FsaAvailableBlocks(pool))
	{
		PrintError("TestFsaInit", "FsaAvailableBlocks");
	}
	
	free(people);
}

static void TestAllocAndFree(void)
{
	size_t block_size = sizeof(human_ty);
	size_t total_size = FsaSuggestedSize(BLOCK_NUM, block_size);
	fsa_ty* pool = NULL;
	human_ty* human_a = NULL;
	human_ty* human_b = NULL;
	human_ty* human_c = NULL;
	human_ty* human_d = NULL;
	char* people = (char*)malloc(total_size * sizeof(char));
	if (NULL == people)
	{
		return;
	}
	
	pool = FsaInit(people, total_size, block_size);
	
	human_a = FsaAlloc(pool);
	human_b = FsaAlloc(pool);
	human_c = FsaAlloc(pool);
	human_d = FsaAlloc(pool);
	
	if (0 != (size_t)human_a % 4)
	{
		PrintError("TestAllocAndFree", "not aligned");
	}
	
	if (BLOCK_NUM - 4 != FsaAvailableBlocks(pool))
	{
		PrintError("TestAllocAndFree", "FsaAvailableBlocks");
	}
	
	FsaFree(pool, human_c);
	FsaFree(pool, human_a);
	
	if (BLOCK_NUM - 2 != FsaAvailableBlocks(pool))
	{
		PrintError("TestAllocAndFree", "FsaAvailableBlocks");
	}
	
	FsaFree(pool, human_b);
	FsaFree(pool, human_d);
	
	free(people);
}

static void TestStressPool(void)
{
    size_t block_size = sizeof(num_ty);
    size_t total_size = FsaSuggestedSize(BLOCK_NUM, block_size);
    char* arr = (char*)malloc(total_size * sizeof(char));
    fsa_ty* fsa = FsaInit(arr, total_size, block_size);
    void* blocks[BLOCK_NUM] = {0};
    size_t i = 0;

    for (i = 0; i < BLOCK_NUM; ++i)
    {
        blocks[i] = FsaAlloc(fsa);
        if (NULL == blocks[i])
        {
            PrintError("TestStressFull", "early NULL alloc");
        }
    }

    if (NULL != FsaAlloc(fsa))
    {
        PrintError("TestStressFull", "alloc past capacity");
    }

    for (i = 0; i < BLOCK_NUM; ++i)
    {
        FsaFree(fsa, blocks[i]);
    }

    if (BLOCK_NUM != FsaAvailableBlocks(fsa))
    {
        PrintError("TestStressFull", "not all freed");
    }
    
    blocks[i] = FsaAlloc(fsa);
    if (NULL == blocks[i])
    {
        PrintError("TestStressFull", "early NULL alloc");
    }
    
    if (BLOCK_NUM - 1 != FsaAvailableBlocks(fsa))
    {
        PrintError("TestStressFull", "alloc after all blocks freed");
    }

    free(arr);
}

static void PrintError(char* test_name, char* problem)
{
	printf("got problem in function %s.\n"
			"the problem is in %s.\n", test_name, problem);
}

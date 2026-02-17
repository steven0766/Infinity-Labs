#include "heap_sort.h"

static void BuildHeapIMP(int* arr_, size_t len_);
static void HeapifyUpIMP(int* arr_, size_t curr_idx_);
static void HeapifyDownIMP(int* arr_, size_t len_, size_t curr_idx_);
static size_t GetLeftChildIMP(size_t idx_);
static size_t GetRightChildIMP(size_t idx_);
static int* GetStartIMP(int* arr_);
static void SwapIMP(int* a_, int* b_);

void HeapSort(int* arr_, size_t len_)
{
	/* assert */
	
	/* build heap  */
	
	/*  get start */
	
	/* while len > 0 */
		/* swap root with last element in heap */
		/* shrink heap- decrease len by 1 */
		/* heapify down on root */
}

static void BuildHeapIMP(int* arr_, size_t len_)
{
	/* assert */
	
	/* get start */
	
	/* for each element start from second element in the heap */
		/* heapify up current element */
}

static void HeapifyUpIMP(int* arr_, size_t curr_idx_)
{
	/* assert */
	
	/* get start */
	
	/* while curr index > 1 and curr < parent */
		/* swap curr with parent */
		/* curr = parent */
}

static void HeapifyDownIMP(int* arr_, size_t len_, size_t curr_idx_)
{
	/* assert */
	
	/* get start */
	
	/* while have left child */
		/* set min child to left child */
		/* if have right child */
			/* get min child */
		/* if current > min child */
			/* swap current with min child */
			/* current = min child index */
		/* else break */
}

static size_t GetLeftChildIMP(size_t idx_)
{
	/* return idx_ * 2 */
}

static size_t GetRightChildIMP(size_t idx_)
{
	/* return idx_ * 2 + 1 */
}

static int* GetStartIMP(int* arr_)
{
	/* return (arr - 1) */
}

static void SwapIMP(int* a_, int* b_)
{
	/* save a in tmp */
	/* change a to b */
	/* change b to tmp */
}




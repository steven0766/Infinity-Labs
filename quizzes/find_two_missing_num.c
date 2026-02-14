#include <stddef.h>
#include <stdio.h>

static void FindTwoMissingNums(int* arr, size_t size);
static void FindTwoMissing_XOR(int* arr, size_t size);

int main()
{
	int arr[10] = {1, 2, 3, 12, 5, 11, 7, 8, 9, 10};
	size_t size = 10;
	
	FindTwoMissingNums(arr, size);
	FindTwoMissing_XOR(arr, size);
	
	return 0;
}

static void FindTwoMissingNums(int* arr, size_t size)
{
	int i = 0;
	size_t sum = 0;
	size_t arr_sum = 0;
	size_t diff = 0;
	int avg = 0;
	
	for (i = 0; i < size; ++i)
	{
		arr_sum += arr[i];
	}
	sum = ((size + 2) * (1 + size + 2)) / 2;
	
	diff = sum - arr_sum;
	avg = diff / 2;
	sum = 0;
	arr_sum = 0;
	
	for (i = 0; i < size; ++i)
	{		
		if(avg >= arr[i])
		{
			arr_sum += arr[i];
		}
	}
	
	sum = (avg * (1 + avg)) / 2;
	
	sum -= arr_sum;
	printf("first num: %ld\n", sum);
	printf("second num: %ld\n", diff - sum);
}

static void FindTwoMissing_XOR(int* arr, size_t size)
{
    int N = size + 2;
    int xor_all = 0;
    int i = 0;
    int rightmost_bit = 0;
    int x = 0, y = 0;

    for (i = 1; i <= N; ++i)
    {
        xor_all ^= i;
    }

    for (i = 0; i < size; ++i)
    {
        xor_all ^= arr[i];
    }

    rightmost_bit = xor_all & -xor_all;

    for (i = 1; i <= N; ++i)
    {
        if (i & rightmost_bit)
        {
            x ^= i;
            }
        else
        {
            y ^= i;
        }
    }

    for (i = 0; i < size; ++i)
    {
        if (arr[i] & rightmost_bit)
        {
            x ^= arr[i];
        }
        else
        {
            y ^= arr[i];
        }
    }

    printf("Missing numbers: %d , %d\n", x, y);
}







#include <limits.h>

int main()
{
	int arr[5] = {7, 4, 1, 9, 3}
	int size = 5;
	int k = 3
	printf("%d smallest var is: %d",k , FindKthSmallest(arr, size, k));
	
	return 0;
}

static int FindKthSmallest(int* arr, int n, int k)
{
    int current_min = INT_MIN;
    int next_min = INT_MAX;

    for (int step = 0; step < k; ++step)
    {
        next_min = INT_MAX;

        for (int i = 0; i < n; ++i)
        {
            if (arr[i] > current_min && arr[i] < next_min)
            {
                next_min = arr[i];
            }
        }

        current_min = next_min;
    }

    return current_min;
}


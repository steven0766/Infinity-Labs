#include <stdio.h>
#include <stddef.h>

int IsArrJump(size_t arr[], int arr_size, int index)
{
	size_t save_num = arr[index];
	
	if (arr_size - 1 == index)
	{
		return 1;
	}
	if (index >= arr_size || index < 0 || 0 == arr[index])
	{
		return 0;
	}
	
	arr[index] = 0;
	
	return IsArrJump(arr, arr_size, index + save_num) || IsArrJump (arr, arr_size, index - save_num);
}

int main()
{
	size_t arr[5] = {3,1,2,1,4};
	size_t i = 0;
	
	printf("is arr jump: %d\n", IsArrJump(arr, 5, 0));
	
	for (i = 0; i < 5; ++i)
	{
		printf("%ld\n", arr[i]);
	}
	
	return 0;
}

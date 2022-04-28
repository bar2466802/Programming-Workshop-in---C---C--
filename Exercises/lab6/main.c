#include <stdio.h>

int main()
{
//	// assume that the address of arr[0][0] is 500
//	int arr[3][4] = { {10, 11, 12, 13}, {20, 21, 22, 23}, {30, 31, 32, 33} };
//	int (*ptr)[4];
//	ptr = arr;
//	printf("%p %p %p ", ptr, ptr + 1, ptr + 2);
//	printf("%p %p %p ", *ptr, *(ptr + 1), *(ptr + 2));
//	printf("%d %d %d ", **ptr, *(*(ptr + 1) + 2), *(*(ptr + 2) + 3));
//	printf("%d %d %d", ptr[0][0], ptr[1][2],ptr[2][3]);

	int *p;
	int (*ptr)[5];
	int arr[5] = {1, 2, 3, 4, 5};

	p = arr;
	ptr = &arr;

	printf("%p %p ", p, ptr);
	printf("%d %p ", *p, *ptr);
	p++;
	ptr++;
	printf("%p %p ", p, ptr);
	printf("%d %p", *p, *ptr);
}

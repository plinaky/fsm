#include <stdio.h>
#include <string.h>

#define MAX 1000ul
static unsigned int arr[MAX];

void myFunction(unsigned int size)
{
	unsigned int my_arr[size];

	for (unsigned int i = 0; i < size; i++) {
		my_arr[i] = i;
	}
	memcpy(arr + size / 2,  my_arr, (size_t)size);
}

int main(void)
{
	unsigned int i;

	for (i = 0; i < 20; i++)
		myFunction(20 * i);

	for (i = 0; i < MAX; i++)
		printf("%ud ", arr[i]);

	printf("\n");

	return 0;
}

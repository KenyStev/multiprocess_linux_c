#include  <stdio.h>
#include  <string.h>

int compare_function(const void *a,const void *b) {
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

int main(int argc, char *argv[])
{
	int nums[argc];
	for (int i = 1; i < argc; ++i)
	{
		nums[i-1] = atoi(argv[i]);
	}
	qsort (nums, argc, sizeof(int), compare_function);
	//printf("Num: %d\n", nums[argc-2]);
	return nums[argc-2];
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>


#define TEST_HEAPSTACK

int set_max_stacksize(void)
{
	struct rlimit rl;
	int res;
	static unsigned int once = 0;

	if (once) {
		printf("stack size already increased\n");
		return res;
	}

	res = getrlimit(RLIMIT_STACK, &rl);

	if (0 != res) {
		printf("getrlimit returned result = %d\n", res);
		return res;
	}

	rl.rlim_cur = rl.rlim_max;
	res = setrlimit(RLIMIT_STACK, &rl);

	if (0 != res)
		printf("setrlimit returned result = %d\n", res);
	else
		once = 1;

	return res;
}


#ifdef TEST_HEAPSTACK
struct data {
	unsigned char field[0x3FFFFFFDF];
};

static void recurse();

static void recurse()
{
	struct data d;

	printf("%p\n", (void *)&d);
	recurse();
}

int main(void)
{
	set_max_stacksize();
	recurse();
	return 0;
}
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fsm.h"
#include <sys/resource.h>

struct data {
	unsigned long long int field[1024 * 1024];
};

static void recurse(unsigned int i, struct data *add1, struct data *add2);

static void recurse(unsigned int i, struct data *add1, struct data *add2)
{
	struct data d1;
	struct data *d2;

	//d2 = malloc(sizeof(struct data));

	if (0 == i) {
		add1 = &d1;
		add2 = d2;
	}
	printf("%ld : %ld %ld \n", i, &d1 - add1, d2 - add2);
	recurse(i + 1, add1, add2);
}

int main(void)
{
	struct rlimit rl;
	int res;

	res = getrlimit(RLIMIT_STACK, &rl);

	if (0 == res) {
		rl.rlim_cur = rl.rlim_max;
		res = setrlimit(RLIMIT_STACK, &rl);
		if (0 != res)
			printf("setrlimit returned result = %d\n", res);
	}

	recurse(0, NULL, NULL);
	return 0;
}


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>


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


#ifdef TEST_BIG_STACK
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

void myFunction(unsigned int size)
{
    int my_arr[size];

    for (unsigned int i = 0; i < size; i++) {
        my_arr[i] = i;
    }

    // Utilisation du tableau my_arr
}

int main(void)
{
	/* set_max_stacksize();
	recurse(); */
	myFunction(10); // Exemple d'appel de la fonction avec une taille spécifique
	return 0;
}
#endif

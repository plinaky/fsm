#include "game.h"

int main(void)
{
	struct board b;

	printf("board %lu \n", sizeof(struct board));

	srand(time(NULL));   // Initialization, should only be called once.

	for (uint32_t i = 1; i <= 100; i++) {
		init_board(&b, pref);
		printf("game %3d : ", i) ;
		play(&b);
	}

	printf("\n");

	return 0;
}

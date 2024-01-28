#include "game.h"

int main(void)
{
	struct board b;

	printf("board %lu \n", sizeof(struct board));
	//printf("node %lu \n", sizeof(struct node));

	srand(time(NULL));   // Initialization, should only be called once.

	for (uint32_t i = 0; i < 1; i++) {
		init_board(&b, pref);
		play(&b, 400);
	}

	return 0;
}

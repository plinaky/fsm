#include "standards.h"
#include "board.h"
#include "game.h"

int main(void)
{
	struct board b1, b2;

	printf("board %lu \n", sizeof(struct board));

	srand(time(NULL));   // Initialization, should only be called once.
	init_board(&b2, pref);

	for (uint32_t i = 1; i <= 1000; i++) {
		memcpy(&b1, &b2, sizeof(struct board));
		printf("\n\ngame %3d : ", i) ;
		play(&b1);
	}

	printf("\n");

	return 0;
}

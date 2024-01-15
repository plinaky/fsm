#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "rules.h"

struct piece board1[8][8] = {
	{WR_, WN_, WB_, WQ_, WK_, WB_, WN_, WR_},
	{WP_, WP_, WP_, WP_, WP_, WP_, WP_, WP_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{BP_, BP_, BP_, BP_, BP_, BP_, BP_, BP_},
	{BR_, BN_, BB_, BQ_, BK_, BB_, BN_, BR_}
};

struct piece board2[8][8] = {
	{WR_, OO_, OO_, OO_, WK_, OO_, OO_, WR_},
	{WP_, WB_, WP_, WQ_, WP_, WP_, WB_, WP_},
	{WN_, OO_, OO_, WP_, OO_, WN_, WP_, OO_},
	{OO_, BB_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, BP_, BP_, BP_, BP_, BP_, OO_, OO_},
	{BN_, OO_, OO_, OO_, BB_, BN_, OO_, OO_},
	{BP_, WP_, OO_, BQ_, OO_, BP_, BP_, BP_},
	{BR_, OO_, OO_, OO_, BK_, OO_, OO_, BR_}
};

struct piece board3[8][8] = {
	{OO_, OO_, OO_, OO_, WK_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, WN_, WB_, OO_, OO_, OO_, OO_, WR_},
	{OO_, OO_, OO_, OO_, WQ_, OO_, WB_, OO_},
	{WR_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{WN_, BK_, WN_, WN_, WN_, WN_, WN_, WN_},
	{WP_, WP_, WP_, WP_, WP_, WP_, WP_, WP_},
	{BR_, BN_, BB_, BQ_, OO_, BB_, BN_, BR_}
};

int main(void)
{
	struct position po;
	int8_t i, j;
	uint64_t k, l;

	srand(time(NULL));   // Initialization, should only be called once.

	for (k = 0; k < 0xFFFFFFFFFFFFFFFF; k++) {
		for (i = 0; i < 8; i++)
			for (j = 0; j < 8; j++)
				set_piece(&po, i, j, board1[i][j]);

		po.W_OO_1 = 1;
		po.W_OOO_1 = 1;
		po.B_OO_1 = 1;
		po.B_OOO_1 = 1;

		po.W_OO_2 = 0;
		po.W_OOO_2 = 0;
		po.B_OO_2 = 0;
		po.B_OOO_2 = 0;

		po.turn = WHITE;
		po.a_passe = 0;
		po.en_passant = 0; 

		if (play_game(&po))
			printf("Game %ld\n", k);
	}

	return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "board.h"
#include "pieces.h"

static uint8_t p1[8][8] = {
	{BR_, BN_, BB_, BQ_, BK_, BB_, BN_, BR_},
	{BP_, BP_, BP_, BP_, BP_, BP_, BP_, BP_},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{WP_, WP_, WP_, WP_, WP_, WP_, WP_, WP_},
	{WR_, WN_, WB_, WQ_, WK_, WB_, WN_, WR_}
};

static uint8_t p2[8][8] = {
	{  0,   0,   0,   0,   0, BR_, BK_,   0},
	{  0,   0,   0,   0, WP_,   0, WP_,   0},
	{  0,   0,   0,   0, WK_,   0,   0,   0},
	{  0, BP_, WP_,   0,   0,   0,   0,   0},
	{  0, WP_,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{WP_,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0}
};

int main(void)
{
	struct board b1, b2;

	printf("board %lu \n", sizeof(struct board));

	b2.turn   = 1;
	b2.wsc    = 1;
	b2.wbc    = 1;
	b2.bsc    = 1;
	b2.bbc    = 1;
	b2.hx     = 0;
	b2.hy     = 0;

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++)
			set_piece(&b2, i, j, p1[7 - i][j]);

	print_board(&b1);

	srand(time(NULL));   // Initialization, should only be called once.

	for (uint8_t i = 0; i < 1; i++) {
		memcpy(&b1, &b2, sizeof(struct board));
		play_game(&b1, 5);
	}

	return 0;
}

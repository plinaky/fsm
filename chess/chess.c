#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "board.h"
#include "pawn.h"

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
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0, BP_, WP_,   0,   0,   0,   0,   0},
	{  0, WP_,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{WP_,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0, WK_,   0,   0,   0}
};

int main(void)
{
	struct board b;
	uint16_t pm[50];
	uint8_t i, j, k, cnt;

	printf("board %lu \n", sizeof(struct board));

	b.turn   = 1;
	b.wsc    = 1;
	b.wbc    = 1;
	b.bsc    = 1;
	b.bbc    = 1;
	b.hx     = 5;
	b.hy     = 1;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			set_piece(&b, i, j, p2[7 - i][j]);

	print_board(&b);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (pawn_moves(&b, i, j, pm, &cnt))
				printf("check !!!\n");
			for (k = 0; k < cnt; k++)
				print_move(pm[k]);
			if (cnt)
				printf("\n");
			cnt = 0;
		}
	}

	return 0;
}

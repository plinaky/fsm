#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
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
/*
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
*/
int main(void)
{
	struct timeval start_time, end_time;
	struct position p0, p1;
	uint64_t k, move_total = 0;
	uint64_t elapsed_time;
	int8_t i, j;

	printf("position %lu \n", sizeof(struct position));
	printf("piece %lu \n", sizeof(struct piece));
	printf("move %lu \n", sizeof(struct move));
	
	srand(time(NULL));   // Initialization, should only be called once.

	p1.wcr = ALL_CR;
	p1.bcr = ALL_CR;
	p1.turn = WHITE;
	p1.x = 0;
	p1.y = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			p1.board[i][j] = board1[i][j];
		}
	}

	gettimeofday(&start_time, NULL);

	memcpy(&p0, &p1, sizeof(struct position));
	move_total += play_game(&p0);
/*
	for (k = 1; k < 0xFFFFFFFFFFFFFF; k++) {

		memcpy(&p0, &p1, sizeof(struct position));
		
		move_total += play_game(&p0);

		if ((move_total % 300000) > 299900) {		
			gettimeofday(&end_time, NULL);
			elapsed_time = 2 * (end_time.tv_sec - start_time.tv_sec);
			printf("mps %ld\n", move_total / elapsed_time);
		}
	}
*/
	return 0;
}

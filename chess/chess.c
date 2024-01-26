#include "board.h"
#include "tree.h"

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

static uint8_t pa[8][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

static uint8_t pb[8][8] = {
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_},
	{WK_,WK_,WK_,WK_,WK_,WK_,WK_,WK_}
};



int main(void)
{
	struct board b1, b2, b3, b4;
	uint64_t moves = 0;

	printf("board %lu \n", sizeof(struct board));
	printf("node %lu \n", sizeof(struct node));

	b2.turn   = 1;
	b2.wsc    = 1;
	b2.wbc    = 1;
	b2.bsc    = 1;
	b2.bbc    = 1;
	b2.hx     = 0;
	b2.hy     = 0;

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++) {
			set_piece(&b2, i, j, p1[7 - i][j]);
			set_piece(&b3, i, j, pa[7 - i][j]);
			set_piece(&b4, i, j, pb[7 - i][j]);
		}

	srand(time(NULL));   // Initialization, should only be called once.

	printf("compare %d\n", compare(&b3, &b4));

	for (uint32_t i = 0; i < 1; i++) {
		memcpy(&b1, &b2, sizeof(struct board));
		moves += play(&b1, 80);
	}

	return 0;
}

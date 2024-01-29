#include "board.h"

uint8_t pref[8][8] = {
	{BR_, BN_, BB_, BQ_, BK_, BB_, BN_, BR_},
	{BP_, BP_, BP_, BP_, BP_, BP_, BP_, BP_},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{  0,   0,   0,   0,   0,   0,   0,   0},
	{WP_, WP_, WP_, WP_, WP_, WP_, WP_, WP_},
	{WR_, WN_, WB_, WQ_, WK_, WB_, WN_, WR_}
};

bool in_bound(int8_t x, int8_t y)
{
	return (0 <= x) && (x < 8) && (0 <= y) && (y < 8);
}

bool on_bound(int8_t x)
{
	return (0 == x) || (x == 7);
}

void print_fig(struct board *bo, uint8_t i, uint8_t j)
{
	uint8_t pi = get_piece(bo, i, j);
	const char *fig[12] = {
		"\u2659\0",
		"\u2658\0",
		"\u2657\0",
		"\u2656\0",
		"\u2655\0",
		"\u2654\0",

		"\u265F\0",
		"\u265E\0",
		"\u265D\0",
		"\u265C\0",
		"\u265B\0",
		"\u265A\0",
	};

	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	if ((i + j) % 2)
		printf("%s", inv_start);

	if (FIG(pi)) {
		if COL(pi) {
			if ((i + j) % 2)
				printf(" %s ", fig[FIG(pi) - 1]);
			else
				printf(" %s ", fig[FIG(pi) + 5]);
		} else {
			if ((i + j) % 2)
				printf(" %s ", fig[FIG(pi) + 5]);
			else
				printf(" %s ", fig[FIG(pi) - 1]);
		}
	} else {
		printf("   ");
	}

	if ((i + j) % 2)
		printf("%s", inv_stop);

}

char to_char(uint8_t pi)
{
	char c = ' ';

	if ((pi | WHITE) == WP_) c = 'P';
	if ((pi | WHITE) == WN_) c = 'N';
	if ((pi | WHITE) == WB_) c = 'B';
	if ((pi | WHITE) == WR_) c = 'R';
	if ((pi | WHITE) == WQ_) c = 'Q';
	if ((pi | WHITE) == WK_) c = 'K';

	if (!(pi & WHITE) && pi)
		c = c - 'A' + 'a' ; 

	return c;
}

void print_square(uint8_t x, uint8_t y)
{
	printf("%c%d", (char)y + 'a', x + 1);
}

uint8_t get_piece(struct board *bo, int8_t x, int8_t y)
{
	if (in_bound(x, y)) {
		uint8_t index  =  (8 * x + y) / 2;
		uint8_t offset = ((8 * x + y) % 2) << 2;

		return (bo->pos[index] >> offset) & 0b1111;
	}

	return 0;
}

void set_piece(struct board *bo, int8_t x, int8_t y, uint8_t pi)
{
	pi &= 0b1111;

	if (in_bound(x, y)) {
		uint8_t index  =  (8 * x + y) / 2;
		uint8_t offset = ((8 * x + y) % 2) * 4; 

		bo->pos[index] &= 0b11110000 >> offset;
		bo->pos[index] |= (pi << offset) & 0b11111111;
	}
}

static inline void print_game_status(struct board *bo)
{
	printf("\n%s to play\t", (bo->turn ? "WHITE" : "BLACK"));
	printf("W/B cr: ");
	printf("%s",   (bo->wsc ? "o" : "-"));
	printf("%s/",  (bo->wbc ? "O" : "-"));
	printf("%s",   (bo->bsc ? "o" : "-"));
	printf("%s\t", (bo->bbc ? "O" : "-"));

	printf("hidden: ");
	print_square(bo->hx, bo->hy);
	printf("\n");
}

void print_pos(struct board *bo)
{
	uint8_t i, j;

	print_game_status(bo);
	printf("\t   A  B  C  D  E  F  G  H\n");
	for (i = 7; i < 8; i--) {
		printf("\t%d ", i + 1);
		for (j = 0; j < 8; j++) {
			print_fig(bo, i, j);
		}
		printf(" %d", i + 1);
		printf("\n");
	}
	printf("\t   A  B  C  D  E  F  G  H\n");
}

void print_board(struct board *bo)
{
	uint8_t i, j;

	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	print_game_status(bo);

	printf("   A  B  C  D  E  F  G  h\n");
	for (i = 7; i < 8; i--) {
		printf("%d ", i + 1);
		for (j = 0; j < 8; j++) {
			if ((i + j) % 2)
				printf("%s", inv_start);
			printf(" %c ", to_char(get_piece(bo, i, j)));
			if ((i + j) % 2)
				printf("%s", inv_stop);
		}
		printf("\n");
	}
}

void init_board(struct board *bo, uint8_t po[8][8])
{
	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++)
			set_piece(bo, i, j, po[7 - i][j]);

	bo->turn   = 1;
	bo->wsc    = 1;
	bo->wbc    = 1;
	bo->bsc    = 1;
	bo->bbc    = 1;
	bo->hx     = 0;
	bo->hy     = 0;
	bo->unused = 0;
}

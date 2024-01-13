#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"

const struct piece WN = {WHITE, KNIGHT};
const struct piece WB = {WHITE, BISHOP};
const struct piece WR = {WHITE, ROOK};
const struct piece WQ = {WHITE, QUEEN};
const struct piece WK = {WHITE, KING};

const struct piece BN = {BLACK, KNIGHT};
const struct piece BB = {BLACK, BISHOP};
const struct piece BR = {BLACK, ROOK};
const struct piece BQ = {BLACK, QUEEN};
const struct piece BK = {BLACK, KING};


/*
struct piece default_board[8][8] = {
	{WR, WN, WB, WQ, WK, WB, WN, WR},
	{WP, WP, WP, WP, WP, WP, WP, WP},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{BP, BP, BP, BP, BP, BP, BP, BP},
	{BR, BN, BB, BQ, BK, BB, BN, BR}
};
*/


struct piece default_board[8][8] = {
	{WR,  0,  0,  0, WK,  0,  0, WR},
	{WP, WB, WP, WQ, WP, WP, WB, WP},
	{WN,  0,  0, WP,  0, WN, WP,  0},
	{ 0, BB,  0,  0,  0,  0,  0,  0},
	{ 0, WP, BP, BP, BP, BP,  0,  0},
	{ 0,  0, BN,  0, BB, BN,  0,  0},
	{BP, BP, BQ,  0,  0, BP, BP, BP},
	{BR,  0,  0,  0, BK,  0,  0, BR}
};

char to_char(const struct piece pi)
{
	if (pi == WP) return 'P';
	if (pi == WN) return 'N';
	if (pi == WB) return 'B';
	if (pi == WR) return 'R';
	if (pi == WQ) return 'Q';
	if (pi == WK) return 'K';

	if (pi == BP) return 'p';
	if (pi == BN) return 'n';
	if (pi == BB) return 'b';
	if (pi == BR) return 'r';
	if (pi == BQ) return 'q';
	if (pi == BK) return 'k';

	return ' ';
}

struct piece get_piece(struct position *po, struct square sq)
{
	uint16_t i = sq.lin;
	uint16_t j = sq.col;
	uint16_t index = 8 * i + j;

	return  0xf & (po->board[index / 2] >> (4 * (j % 2)));
}

void set_piece(struct position *po, struct square sq, struct piece pi);
{
	uint16_t i = sq.lin;
	uint16_t j = sq.col;
	uint16_t index = 8 * i + j;

	po->board[index / 2] &= ~(0xf << (4 * (j % 2)));
	po->board[index / 2] |= fig << (4 * (j % 2));
}

void print_pos(struct position *po)
{
	struct square sq;
	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("\n\t    a   b   c   d   e   f   g   h\n");
	printf("\t   -------------------------------\n");

	for (int i = 7; i >= 0; i--) {

		printf("\t%d |", i + 1);

		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);
			sq = {i, j}
			printf(" %c ", to_char(get_piece(po->board, sq)));

			if ((i + j) % 2)
				printf("%s", inv_stop);

			printf("|");
		}

		printf(" %d\n", i + 1);
		printf("\t   -------------------------------\n");
	}

	printf("\t    a   b   c   d   e   f   g   h\n");
}

static void print_square(struct square sq)
{
	printf("%c%d",
	       'a' + (char)(sq.col),
	        1  + (sq.lin)
	       );
}

void print_move(struct move mo)
{
	print_square(mo.start);
	print_square(mo.stop);
	printf(" ");
}

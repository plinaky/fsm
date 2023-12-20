#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define	PAWN   1
#define KNIGHT 2
#define BISHOP 3
#define ROOK   4
#define QUEEN  5
#define KING   6

#define WHITE (0 << 3)
#define BLACK (1 << 3)

#define WR (WHITE | ROOK)
#define WN (WHITE | KNIGHT)
#define WB (WHITE | BISHOP)
#define WQ (WHITE | QUEEN)
#define WK (WHITE | KING)
#define WP (WHITE | PAWN)

#define BR (BLACK | ROOK)
#define BN (BLACK | KNIGHT)
#define BB (BLACK | BISHOP)
#define BQ (BLACK | QUEEN)
#define BK (BLACK | KING)
#define BP (BLACK | PAWN)

uint8_t default_board[8][8] = {
	{WR, WN, WB, WQ, WK, WB, WN, WR},
	{WP, WP, WP, WP, WP, WP, WP, WP},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{BP, BP, BP, BP, BP, BP, BP, BP},
	{BR, BN, BB, BQ, BK, BB, BN, BR}
};

static inline uint8_t get_piece(uint8_t board[32], uint8_t i, uint8_t j)
{
	uint16_t pos = 8 * i + j;
	return  0b1111 & (board[pos / 2] >> (4 * (j % 2)));
}

static inline void set_piece(uint8_t board[32], uint8_t i, uint8_t j, uint8_t p)
{
	uint16_t pos = 8 * i + j;

	board[pos / 2] &= ~(0b1111 << (4 * (j % 2)));
	board[pos / 2] |= p << (4 * (j % 2));
}


static char to_char(uint8_t p)
{
	if (p == WP) return 'P';
	if (p == WN) return 'N';
	if (p == WB) return 'B';
	if (p == WR) return 'R';
	if (p == WQ) return 'Q';
	if (p == WK) return 'K';

	if (p == BP) return 'p';
	if (p == BN) return 'n';
	if (p == BB) return 'b';
	if (p == BR) return 'r';
	if (p == BQ) return 'q';
	if (p == BK) return 'k';

	return ' ';
}

static void print_pos(uint8_t board[32])
{
	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("\n\t    a   b   c   d   e   f   g   h\n");
	printf("\t   -------------------------------\n");

	for (int i = 7; i >= 0; i--) {

		printf("\t%d |", i + 1);

		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);

			printf(" %c ", to_char(get_piece(board, i, j)));

			if ((i + j) % 2)
				printf("%s", inv_stop);

			printf("|");
		}

		printf(" %d\n", i + 1);
		printf("\t   -------------------------------\n");
	}

	printf("\t    a   b   c   d   e   f   g   h\n");
}

int main(void)
{
	uint8_t board[32];

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++)
			set_piece(board, i, j, default_board[i][j]);

	print_pos(board);

	return 0;
}

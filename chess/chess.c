#include <stdio.h>

#define PAWN   1
#define KNIGHT 2
#define BISHOP 3
#define ROOK   4
#define QUEEN  5
#define KING   6

#define WHITE  (1 << 4)
#define BLACK  (1 << 5)

#define WP (PAWN   | WHITE)
#define WN (KNIGHT | WHITE)
#define WB (BISHOP | WHITE)
#define WR (ROOK   | WHITE)
#define WQ (QUEEN  | WHITE)
#define WK (KING   | WHITE)
#define BP (PAWN   | BLACK)
#define BN (KNIGHT | BLACK)
#define BB (BISHOP | BLACK)
#define BR (ROOK   | BLACK)
#define BQ (QUEEN  | BLACK)
#define BK (KING   | BLACK)

typedef unsigned char u8;

static u8 the_game[8][8] = {
	{WR, WN, WB, WQ, WK, WB, WN, WR},
	{WP, WP, WP, WP, WP, WP, WP, WP},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{BP, BP, BP, BP, BP, BP, BP, BP},
	{BR, BN, BB, BQ, BK, BB, BN, BR}
};

static char to_char(u8 piece)
{
	switch (piece) {
		case WP: return 'P';
		case WN: return 'N';
		case WB: return 'B';
		case WR: return 'R';
		case WQ: return 'Q';
		case WK: return 'K';
		case BP: return 'p';
		case BN: return 'n';
		case BB: return 'b';
		case BR: return 'r';
		case BQ: return 'q';
		case BK: return 'k';
		default: return ' ';
	}
}

static void print_game(const u8 game[8][8])
{
	printf("\t    a   b   c   d   e   f   g   h\n");
	printf("\t    -----------------------------\n");
	for (int i = 7; i >= 0; i--) {
		printf("\t%d | ", i + 1);
		for (int j = 0; j < 8; j++) {
			printf("%c | ", to_char(game[i][j]));
		}
		printf("%d\n", i + 1);
		printf("\t    -----------------------------\n");
	}
	printf("\n\t    a   b   c   d   e   f   g   h\n");
}

int main(void)
{
	print_game(the_game);

	return 0;
}

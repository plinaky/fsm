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

#define MOVE_NBR 256

static inline uint8_t get_piece(uint8_t *board, uint8_t i, uint8_t j)
{
	uint16_t pos = 8 * i + j;
	return 0xf & (board[pos / 2] >> (4 * (j % 2)));
}

static inline void set_piece(uint8_t *board, uint8_t i, uint8_t j, uint8_t p)
{
	uint16_t pos = 8 * i + j;

	board[pos / 2] &= ~(0b1111 << (4 * (j % 2)));
	board[pos / 2] |= p << (4 * (j % 2));
}


static char to_char(uint8_t p)
{
	if (p == (WHITE | PAWN))   return 'P';
	if (p == (WHITE | KNIGHT)) return 'N';
	if (p == (WHITE | BISHOP)) return 'B';
	if (p == (WHITE | ROOK))   return 'R';
	if (p == (WHITE | QUEEN))  return 'Q';
	if (p == (WHITE | KING))   return 'K';

	if (p == (BLACK | PAWN))   return 'p';
	if (p == (BLACK | KNIGHT)) return 'n';
	if (p == (BLACK | BISHOP)) return 'b';
	if (p == (BLACK | ROOK))   return 'r';
	if (p == (BLACK | QUEEN))  return 'q';
	if (p == (BLACK | KING))   return 'k';

	return ' ';
}

static void print_pos(uint8_t *board)
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

	memset(board, 0 , 32);

	set_piece(board, 0, 0, WR);
	set_piece(board, 0, 1, WN);
	set_piece(board, 0, 2, WB);
	set_piece(board, 0, 3, WQ);
                            
	set_piece(board, 0, 4, WK);
	set_piece(board, 0, 5, WB);
	set_piece(board, 0, 6, WN);
	set_piece(board, 0, 7, WR);
                            
	set_piece(board, 1, 0, WP);
	set_piece(board, 1, 1, WP);
	set_piece(board, 1, 2, WP);
	set_piece(board, 1, 3, WP);
                            
	set_piece(board, 1, 4, WP);
	set_piece(board, 1, 5, WP);
	set_piece(board, 1, 6, WP);
	set_piece(board, 1, 7, WP);
                            
	set_piece(board, 7, 0, BR);
	set_piece(board, 7, 1, BN);
	set_piece(board, 7, 2, BB);
	set_piece(board, 7, 3, BQ);
                            
	set_piece(board, 7, 4, BK);
	set_piece(board, 7, 5, BB);
	set_piece(board, 7, 6, BN);
	set_piece(board, 7, 7, BR);
                            
	set_piece(board, 6, 0, BP);
	set_piece(board, 6, 1, BP);
	set_piece(board, 6, 2, BP);
	set_piece(board, 6, 3, BP);
                            
	set_piece(board, 6, 4, BP);
	set_piece(board, 6, 5, BP);
	set_piece(board, 6, 6, BP);
	set_piece(board, 6, 7, BP);

	print_pos(board);

	return 0;
}

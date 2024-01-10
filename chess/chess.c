#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main(void)
{
	uint8_t square, fig, i, j, k;
	struct game gm;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			set_piece(gm.board, SQUARE(i, j), default_board[i][j]);

	gm.castle = 0xf;

	print_pos(gm.board);

	printf("\nWhite to play\n");
	gm.turn = 0;
	gm.en_passant = SQUARE(5, 2);
	list_moves(&gm);
	for (k = 0; k < gm.move_cnt; k++) {
		print_move(get_move(gm.moves, k));
	}
	printf("\n");

	printf("\nBlack to play\n");
	gm.turn = 1;
	gm.en_passant = 0;
	list_moves(&gm);
	for (k = 0; k < gm.move_cnt; k++) {
		print_move(get_move(gm.moves, k));
	}
	printf("\n");

	return 0;
}

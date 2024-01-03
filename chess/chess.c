#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main(void)
{
	uint8_t square, fig, count, i, j, k;
	struct game gm;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			set_piece(gm.board, SQUARE(i, j), default_board[i][j]);

	gm.turn = 0;

	print_pos(gm.board);

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			square = SQUARE(i, j);
			fig = get_piece(gm.board, square);
			if (fig) {
				if (pinned(gm.board, square))
					printf("%c in %c%d : is pinned \n", to_char(fig), 'a' + SQJ(square), 1 + SQI(square));
			}
		}
	}

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			square = SQUARE(i, j);
			fig = get_piece(gm.board, square);
			if (FIG(fig) == PAWN) {
				pawn_moves(&gm, square, &count);
				for (k = 0; k < count; k++) {
					print_moves(get_move(gm.moves, k));
					printf("\n");
				}
			}
		}
	}

	return 0;
}

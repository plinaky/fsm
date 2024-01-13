#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main(void)
{
	struct position po;
	struct square sq;
	uint8_t i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			sq = {i, j};
			set_piece(&po, sq, default_board[i][j]);
		}

	print_pos(gm.board);

	return 0;
}

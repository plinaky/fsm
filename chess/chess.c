#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main(void)
{
	uint8_t board[32];
	uint8_t square, fig;

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++)
			set_piece(board, SQUARE(i, j), default_board[i][j]);

	print_pos(board);

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++) {
			square = SQUARE(i, j);
			fig = get_piece(board, square);
			if (fig) {
				if (pinned(board, square))
					printf("%c in %c%d : is pinned \n", to_char(fig), 'a' + SQJ(square), 1 + SQI(square));
			}
		}


	return 0;
}

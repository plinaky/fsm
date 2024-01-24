#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

bool compare(struct board *b1, struct board *b2)
{
	return memcmp(b1, b2, sizeof(struct board)) == 0;
}

void flip(struct board *b)
{
	uint8_t i, j, wsc, wbc, bsc, bbc;

	print_board(b);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 8; j++) {
			wsc = (get_piece(b, i, j) ? get_piece(b, i, j) ^ 0x8 : 0);
			set_piece(b, i, j, (get_piece(b, 7 - i, j) ? get_piece(b, 7 - i, j) ^ 0x8 : 0));
			set_piece(b, 7 - i, j, wsc);
		}
	}

	b->turn = (b->turn ? 0 : 1);

	wsc = b->bsc;
	wbc = b->bbc;
	bsc = b->wsc;
	bbc = b->wbc;

	b->wsc = bsc;
	b->wbc = bbc;
	b->bsc = wsc;
	b->bbc = wbc;

	if ((b->hx != 0) && (b->hy != 0))
		b->hx = 7 - b->hx;

	print_board(b);

}

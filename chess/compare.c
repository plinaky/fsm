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
	uint8_t i, j;
	struct board tb;

	print_board(b);
	memcpy(&tb, b, sizeof(struct board));

	for (uint8_t i = 0; i < 8; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			uint8_t p = get_piece(&tb, i, j);
			set_piece(b, 7 - i, j, (p ? p ^ 0x8 : 0));  
		}
	}

	b->turn = (b->turn ? 0 : 1);
	b->wsc = tb.bsc;
	b->wbc = tb.bbc;
	b->bsc = tb.wsc;
	b->bbc = tb.wbc;

	if ((b->hx != 0) && (b->hy != 0))
		b->hx = 7 - b->hx;

	print_board(b);

}

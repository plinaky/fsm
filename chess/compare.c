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
	uint8_t i, j, p1, p2;
	uint8_t wsc, wbc, bsc, bbc;

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

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 8; j++) {
			p1 = get_piece(b, i, j);
			p1 = p1 ? p1 ^ 0x8 : 0;
			p2 = get_piece(b, 7 - i, j);
			set_piece(b, i, j, p2 ? p2 ^ 0x8 : 0);
			set_piece(b, 7 - i, j, p1);
		}
	}

	printf("Flip\n");
	print_board(b);

}

bool mirror(struct board *b)
{
	uint8_t i, j, p1, p2;

	if ((b->wsc != 0) || (b->wbc != 0) || 
			(b->bsc != 0) || (b->bbc != 0))
		return false;

	if ((!on_bound(b->hx)) && (b->hy != 0))
		b->hy = 7 - b->hy;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++) {
			p1 = get_piece(b, i, j);
			p2 = get_piece(b, i, 7 - j);
			set_piece(b, i, j, p2);
			set_piece(b, 7 - i, j, p1);
		}
	}
	printf("Mirror\n");

	print_board(b);

	return true;
}

bool rotate(struct board *b, bool clock)
{
	uint8_t i, j, p1, p2, p3, p4;

	if ((b->wsc != 0) || (b->wbc != 0) || 
			(b->bsc != 0) || (b->bbc != 0))
		return false;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			if (BP_ == FIG(get_piece(b, i, j)))
				return false;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			p1 = get_piece(b, i, j);
			p2 = get_piece(b, j, 7 - i);
			p3 = get_piece(b, 7 - i, 7 - j);
			p4 = get_piece(b, j, 7 - i);

			if (clock) {
				set_piece(b, i, j, p2);
				set_piece(b, j, 7 - i, p3);
				set_piece(b, 7 - i, 7 - j, p4);
				set_piece(b, j, 7 - i, p1);
			} else {
				set_piece(b, i, j, p4);
				set_piece(b, j, 7 - i, p1);
				set_piece(b, 7 - i, 7 - j, p2);
				set_piece(b, j, 7 - i, p3);

			}
		}
	}

	printf("Rotate %c90°\n", clock ? '+' : '-');

	print_board(b);

	return true;

}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "pieces.h"

bool list_moves(struct board *bo, uint16_t *ml, uint8_t *cnt)
{
	*cnt = 0;

	for (int8_t x = 0; x < 8; x++) {
		for (int8_t y = 0; y < 8; y++) {
			if (
			    (pawn_moves(bo, x, y, ml, cnt))
			    || (king_moves(bo, x, y, ml, cnt))
			    || (knight_moves(bo, x, y, ml, cnt))
			    || (brq_moves(bo, x, y, ml, cnt))
			    ) {
				*cnt = 0;
				return true;
			}
		}
	}

	return false;
}

void print_moves(uint16_t *ml, uint8_t cnt)
{
	uint8_t x = 0;
	uint8_t y = 0;

	x = X1_OF(ml[0]);
	y = Y1_OF(ml[0]);

	for (uint8_t i = 0; i < cnt; i++) {
		if ((x != X1_OF(ml[i])) || (y != Y1_OF(ml[i]))) {
			x = X1_OF(ml[i]);
			y = Y1_OF(ml[i]);
			printf("\n");
		}
		print_move(ml[i]);
	}
	printf("\n");
}

void apply_move(struct board *bo, uint16_t mo)
{
	int8_t x1 = X1_OF(mo);
	int8_t y1 = Y1_OF(mo);
	int8_t x2 = X2_OF(mo);
	int8_t y2 = Y2_OF(mo);
	uint8_t pi1 = get_piece(bo, x1, y1);
	uint8_t fig = FIG(pi1);

	set_piece(bo, x1, y1, 0);

	if (BP_ == fig) {

		if ((x1 == x2 + 2) || ((x1 + 2) == x2)) {
			bo->hx = (x1 + x2) / 2;
			bo->hy = y1;
		} else {
			bo->hx = 0;
			bo->hy = 0;
			if ((0 == get_piece(bo, x2, y2)) && (TAKOF(mo))) {
				set_piece(bo, x1, y2, 0);
			}
		}

	} else if (BK_ == fig) {
		if (COL(fig)) {
			bo->wsc = 0;
			bo->wbc = 0;
		} else {
			bo->bsc = 0;
			bo->bbc = 0;
		}

		if (y1 == y2 + 2) {
			bo->hx = x1;
			bo->hy = (y1 + y2) / 2;
			apply_move(bo, MOVE_OF(x1, 7, x2, 5, 0, 0));
		} else if ((y1 + 2) == y2) {
			bo->hx = x1;
			bo->hy = (y1 + y2) / 2;
			apply_move(bo, MOVE_OF(x1, 0, x2, 3, 0, 0));
		} else {
			bo->hx = 0;
			bo->hy = 0;
		}

	} else {
		bo->hx = 0;
		bo->hy = 0;
	}

	set_piece(bo, x2, y2, pi1);

	bo->turn = (bo->turn ? 0 : 1);
}

bool list_legal_moves(struct board *bo, uint16_t *ml, uint8_t *cnt)
{
	struct board b;
	uint16_t ml2[256];
	uint8_t cnt2;

	if (list_moves(bo, ml, cnt))
		return true;

	for (uint8_t i = 0; i < *cnt; i++) {
		memcpy(&b, bo, sizeof(struct board));
		apply_move(&b, ml[i]);
		if (list_moves(&b, ml2, &cnt2)) {
			printf("I can eat the king\n");
			memmove(ml + i, ml + i + 1, sizeof(uint16_t) * ((*cnt) - i - 1));
			(*cnt)--;
			i--;
		}
	}

	if (*cnt)
		return false;

	memcpy(&b, bo, sizeof(struct board));
	b.turn = WHITE - b.turn;

	return list_moves(&b, ml2, &cnt2);

}


int8_t play_game(struct board *bo, uint8_t max)
{
	int r;
	uint8_t cnt, i;
	uint16_t ml[255];
	bool res;

	for (i = 0; i < max; i++) {
		print_board(bo);
		res = list_legal_moves(bo, ml, &cnt);
		print_moves(ml, cnt);
		
		if ((0 == cnt) && (res)) {
			printf("\n***** CHECKMATE at move %d! ******\n\n", i);
			return 1 - bo->turn * 2;
		}

		if ((0 == cnt) && (!res)) {
			printf("\n***** PAT at move %d!       ******\n\n", i);
			return 0;
		}

		r = rand() % cnt;
		apply_move(bo, ml[r]);

		printf("move %d: ", i);
		print_move(ml[r]);
		printf("\n");
	}

	printf("\n***** NO WIN after move %d!      ******\n\n", i);

	return 0;
}

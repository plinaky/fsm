#include "standards.h"
#include "board.h"
#include "move.h"
#include "compare.h"
#include "tree.h"

void print_move(uint16_t mo)
{
	print_square(X1_OF(mo), Y1_OF(mo));
	printf("%c", (TAKOF(mo) ? 'x' : '-'));
	print_square(X2_OF(mo), Y2_OF(mo));
	if (PROMO(mo))
		printf("%c", to_char(PROMO(mo) | WHITE));
	printf(" ");
}

void print_moves(uint16_t *ml, uint8_t cnt)
{
	for (uint8_t i = 0; i < cnt; i++) {
		if (! (i % 8))
			printf("\n%3d: ", i);
		print_move(ml[i]);
	}
	printf("\n");
}

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
			    )
				return true;
		}
	}

	return false;
}

void apply_move(struct board *bo, uint16_t mo)
{
	int8_t x1 = X1_OF(mo);
	int8_t y1 = Y1_OF(mo);
	int8_t x2 = X2_OF(mo);
	int8_t y2 = Y2_OF(mo);
	uint8_t pi1 = get_piece(bo, x1, y1);
	uint8_t fig = FIG(pi1);

	bo->hx = 0;
	bo->hy = 0;

	set_piece(bo, x1, y1, 0);

	/* rook case to clear castling rights */
	if (WR_ == pi1) {
		if ((0 == x1) && (0 == y1))
			bo->wbc = 0;
		if ((0 == x1) && (7 == y1))
			bo->wsc = 0;
	} else if (BR_ == pi1) {
		if ((7 == x1) && (0 == y1))
			bo->bbc = 0;
		if ((7 == x1) && (7 == y1))
			bo->bsc = 0;

	/* pawn case to set en passant or clear passed pawn */
	} else if (BP_ == fig) {
		if ((x1 == x2 + 2) || ((x1 + 2) == x2)) {
			bo->hx = (x1 + x2) / 2;
			bo->hy = y1;
		} else {
			if ((0 == get_piece(bo, x2, y2)) && (TAKOF(mo)))
				set_piece(bo, x1, y2, 0);
			if (PROMO(mo))
				pi1 += PROMO(mo) - BP_;
		}

	/*
	 * king case to 
	 * - clear castling rights 
	 * - set intermediate king position when castling
	 * - move took when castling
	*/
	} else if (BK_ == fig) {
		if (y1 == y2 + 2) {
			bo->hx = x1;
			bo->hy = (y1 + y2) / 2;
			apply_move(bo, MOVE_OF(x1, 0, x2, 3, 0, 0));
			bo->turn = (bo->turn ? 0 : 1);
		} else if ((y1 + 2) == y2) {
			bo->hx = x1;
			bo->hy = (y1 + y2) / 2;
			apply_move(bo, MOVE_OF(x1, 7, x2, 5, 0, 0));
			bo->turn = (bo->turn ? 0 : 1);
		}

		if (COL(pi1)) {
			bo->wsc = 0;
			bo->wbc = 0;
		} else {
			bo->bsc = 0;
			bo->bbc = 0;
		}
	}

	set_piece(bo, x2, y2, pi1);

	bo->turn = (bo->turn ? 0 : 1);
}

bool list_legal_moves(struct board *bo, uint16_t *ml, uint8_t *cnt)
{
	struct board b;
	uint16_t ml2[256];
	uint8_t cnt2;
	bool ret = false;

	if (list_moves(bo, ml, cnt))
		return true;

	for (int16_t i = 0; i < *cnt; i++) {
		memcpy(&b, bo, sizeof(struct board));
		apply_move(&b, ml[i]);
		if (list_moves(&b, ml2, &cnt2)) {
			memmove(ml + i, ml + i + 1, sizeof(uint16_t) * ((*cnt) - i - 1));
			(*cnt)--;
			i--;
		}
	}

	if (*cnt)
		return ret;

	bo->turn = (bo->turn ? 0 : 1);
	ret = list_moves(bo, ml2, &cnt2);
	bo->turn = (bo->turn ? 0 : 1);

	return ret;
}

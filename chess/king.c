#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

bool king_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt)
{
	int8_t moves[10][2] = {
		{ 1,  1}, { 1, -1}, {-1, -1}, {-1,  1},
		{ 0,  1}, { 0, -1}, { 1,  0}, {-1,  0},
		{ 0,  2}, { 0, -2},
	};
	int8_t l, c;
	uint8_t pi1 = get_piece(bo, x, y);

	if (BK_ != FIG(pi1))
		return false;

	if ((COL(pi1) >> 3) != bo->turn)
		return false;

	for (uint8_t i = 0; i < 8; i++) {
		l = x + moves[i][0];
		c = y + moves[i][1];
		if (in_bound(l, c)) {
			uint8_t pi2 = get_piece(bo, l, c);
			if (!(FIG(pi2)) && (l))
				ml[(*cnt)++] = MOVE_OF(x, y, x + 2 * dx, y, 0, 0);
				
		if ((FIG(pi2)) && (COL(pi1) != COL(pi2))) {
		}
	}

	if (!get_piece(bo, x + dx, y)) {
		ml[(*cnt)++] = MOVE_OF(x, y, x + dx, y, 0, 0);
		if (on_bound(x + dx)) {
			promote(bo, ml, cnt);
		} else if (on_bound(x - dx) && (!get_piece(bo, x + 2 * dx, y))) {
			ml[(*cnt)++] = MOVE_OF(x, y, x + 2 * dx, y, 0, 0);
		}
	}

	for (dy = -1; dy < 2; dy += 2) {
		uint8_t pi2 = get_piece(bo, x + dx, y + dy);
		if ((BK_ == FIG(pi2)) && (COL(pi1) != COL(pi2)))
			return true;
		if ((FIG(pi2)) && (COL(pi1) != COL(pi2))) {
			if (on_bound(x + dx) && (!on_bound(bo->hy)) && (x + dx == bo->hx) && (y + dy == bo->hy))
				return true;
			ml[(*cnt)++] = MOVE_OF(x, y, x + dx, y + dy, 0, 1);
			if (on_bound(x + dx))
				promote(bo, ml, cnt);
		}
		if (!(FIG(pi2)) && (x + dx == bo->hx) && (y + dy == bo->hy))
			ml[(*cnt)++] = MOVE_OF(x, y, x + dx, y + dy, 0, 1);
	}

	return false;
}

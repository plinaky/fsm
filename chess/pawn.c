#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

static void promote(struct board *bo, uint16_t *ml, uint8_t *cnt)
{
	(*cnt)--;

	uint16_t mo = ml[(*cnt)];

	ml[(*cnt)++] = (mo | (BQ_ << 12)) & 0xf;
	ml[(*cnt)++] = (mo | (BR_ << 12)) & 0xf;
	ml[(*cnt)++] = (mo | (BB_ << 12)) & 0xf;
	ml[(*cnt)++] = (mo | (BK_ << 12)) & 0xf;
}

bool pawn_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt)
{
	int8_t dx, dy;
	uint8_t pi1 = get_piece(bo, x, y);

	if (BP_ != FIG(pi1))
		return false;

	if ((COL(pi1) >> 3) != bo->turn)
		return false;

	if (COL(pi1))
		dx = 1;
	else
		dx = -1;

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
			ml[(*cnt)] = MOVE_OF(x, y, x + dx, y + dy, 0, 1);
			if (on_bound(x + dx)) 
				promote(bo, ml, cnt);
			else
				(*cnt)++;
		}
	}

	return false;
}

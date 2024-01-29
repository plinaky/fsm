#include "standards.h"
#include "board.h"

bool knight_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt)
{
	int8_t moves[8][2] = {
		{ 1,  2}, { 1, -2}, {-1, -2}, {-1,  2},
		{ 2,  1}, { 2, -1}, {-2, -1}, {-2,  1}
	};
	int8_t l, c;
	uint8_t pi1 = get_piece(bo, x, y);

	if (BN_ != FIG(pi1))
		return false;

	if ((COL(pi1) >> 3) != bo->turn)
		return false;

	for (uint8_t i = 0; i < 8; i++) {
		l = x + moves[i][0];
		c = y + moves[i][1];
		if (in_bound(l, c)) {
			uint8_t pi2 = get_piece(bo, l, c);
			if (FIG(pi2)) {
				if (COL(pi1) != COL(pi2)) {
					if (((!on_bound(bo->hy)) && (l == bo->hx) && (c == bo->hy))
					    || (BK_ == FIG(pi2)))
						return true;
					ml[(*cnt)++] = MOVE_OF(x, y, l, c, 0, 1);
				}
			} else {
				ml[(*cnt)++] = MOVE_OF(x, y, l, c, 0, 0);
			}
		}
	}

	return false;
}

#include "board.h"

bool brq_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt)
{
	int8_t offsets[8][2] = {{ 1, 0}, { 0, 1}, { 1, 1}, {-1,  1}};
	int8_t l, c;
	uint8_t pi1 = get_piece(bo, x, y);
	uint8_t start, stop;
	start = 0;
	stop  = 0;


	if ((COL(pi1) >> 3) != bo->turn)
		return false;

	if (BR_ == FIG(pi1)) {
		start = 0;
		stop  = 2;
	} else if (BB_ == FIG(pi1)) {
		start = 2;
		stop  = 4;
	} else if (BQ_ == FIG(pi1)) {
		start = 0;
		stop  = 4;
	} else {
		return false;
	}

	for (uint8_t i = start; i < stop; i++) {
		for (int8_t k = -1; k < 2; k += 2) {
			for (int8_t j = 1; j < 7; j++) {
				l = x + k * j * offsets[i][0];
				c = y + k * j * offsets[i][1];
				if (in_bound(l, c)) {
					uint8_t pi2 = get_piece(bo, l, c);
					if (FIG(pi2)) {
						if (COL(pi1) != COL(pi2)) {
							if (((l == bo->hx) && (c == bo->hy))
							    || (BK_ == FIG(pi2)))
								return true;
							ml[(*cnt)++] = MOVE_OF(x, y, l, c, 0, 1);
							break;
						} else {
							break;
						}
					} else {
						ml[(*cnt)++] = MOVE_OF(x, y, l, c, 0, 0);
					}
				} else {
					break;
				}
			}
		}
	}

	return false;
}

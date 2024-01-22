#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"

struct move move_of(uint8_t sq1, uint8_t sq2, uint8_t promo, uint8_t take)
{
	struct move mo;

	mo.sq1   = (sq1   & 0b111111) <<  0;
	mo.sq2   = (sq2   & 0b111111) <<  6;
	mo.promo = (promo & 0b111   ) << 12;
	mo.take  = (take  & 0b1     ) << 15;
}

bool promote(struct board *bo, struct move *ml, uint8_t *cnt)
{
	uint8_t sq = (ml[*cnt] >> 6) & 0b111111;

	ml[*cnt].promo = (BQ | (bo->turn << 3)) & 0b1111;
	if (0 != get_piece(bo, sq))
		ml[*cnt].take = 1;
	ml[*cnt].promo = (BR | (bo->turn << 3)) & 0b1111;
	ml[(*cnt) + 1] = ml[(*cnt)];
	ml[*cnt].promo = (BB | (bo->turn << 3)) & 0b1111;
	ml[(*cnt) + 2] = ml[(*cnt)];
	ml[*cnt].promo = (BK | (bo->turn << 3)) & 0b1111;
	ml[(*cnt) + 3] = ml[(*cnt)];
	*cnt += 4;
}

bool pawn_moves(struct board *bo, uint8_t sq, struct move *ml, uint8_t *cnt)
{
	int8_t dx;
	int8_t x = sq & 0b111;
	int8_t y = (sq >> 3) & 0b111;

	uint8_t pi = get_piece(bo, sq);

	if (BP_ != FIG(pi))
		return false;

	if ((COL(pi) >> 3) != bo->turn)
		return false;

	if (COL(pi))
		dx = 1;
	else
		dx = -1;

	if (OO_ != get_piece(bo, square_of()))
}

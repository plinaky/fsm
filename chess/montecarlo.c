#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "move.h"
static bool knight_moves(struct game *gm, uint8_t square)
{
	uint8_t figure, color, dest, take, cnt;
	int8_t x, y;
	int8_t moves[8][2] = {
		{ 1,  2},
		{ 1, -2},
		{ 2,  1},
		{ 2, -1},
		{-1,  2},
		{-1, -2},
		{-2,  1},
		{-2, -1}
	};

	figure = get_piece(gm->board, square);

	if (FIG(figure) != KNIGHT)
		return false;

	color = COL(figure);

	for (cnt = 0 ; cnt < 7 ; cnt++) {
		x = (int8_t)SQI(square) + moves[cnt][0];
		y = (int8_t)SQJ(square) + moves[cnt][1];
		if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {
			dest = SQUARE(x, y);
			take = get_piece(gm->board, dest);
			if ((0 == take) || (COL(take) != color)) {
				if (FIG(take) == KING)
					return true;
				set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
			}
		}
	}

	return false;
}

static bool brq_moves(struct game *gm, uint8_t square)
{
	uint8_t figure, color, dest, take, cnt1, cnt2, start, stop;
	int8_t x, y;
	int8_t offsets[8][2] = {
		{ 1,  1},
		{ 1, -1},
		{-1, -1},
		{-1,  1},
		{ 0,  1},
		{ 0, -1},
		{-1,  0},
		{ 1,  0}
	};

	figure = get_piece(gm->board, square);

	if (FIG(figure) == BISHOP) {
		start = 0;
		stop  = 4;
	} else if (FIG(figure) == ROOK) {
		start = 4;
		stop  = 8;
	} else if (FIG(figure) == QUEEN) {
		start = 0;
		stop  = 8;
	} else {
		return false;
	}

	color = COL(figure);

	for (cnt1 = start ; cnt1 < stop; cnt1++) {
		for (cnt2 = 1 ; cnt2 < 8 ; cnt2++) {
			x = (int8_t)SQI(square) + cnt2 * offsets[cnt1][0];
			y = (int8_t)SQJ(square) + cnt2 * offsets[cnt1][1];
			if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {
				dest = SQUARE(x, y);
				take = get_piece(gm->board, dest);
				if (0 == take) {
					set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
				} else if (COL(take) != color) {
					if (FIG(take) == KING)
						return true;
					set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
					break;
				} else {
					break;
				}
			} else {
				break;
			}
		}
	}

	return false;
}

static bool king_moves(struct game *gm, uint8_t square)
{
	uint8_t figure, color, dest, take, cnt, castle;
	int8_t x, y;
	int8_t moves[8][2] = {
		{ 1, -1},
		{ 1,  0},
		{ 1,  1},
		{ 0, -1},
		{ 0,  1},
		{-1, -1},
		{-1,  0},
		{-1,  1}
	};

	figure = get_piece(gm->board, square);

	if (FIG(figure) != KING)
		return false;

	color = COL(figure);

	for (cnt = 0 ; cnt < 7 ; cnt++) {
		x = (int8_t)SQI(square) + moves[cnt][0];
		y = (int8_t)SQJ(square) + moves[cnt][1];
		if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {
			dest = SQUARE(x, y);
			take = get_piece(gm->board, dest);
			if (0 == take) {
				set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
			} else if (COL(take) != color) { 
				if (FIG(take) == KING)
					return true;
				set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
			}
		}
	}

	return false;
}

static bool castle_moves(struct game *gm, uint8_t square)
{
	uint8_t figure, castle, x, y;

	figure = get_piece(gm->board, square);

	if (FIG(figure) != KING)
		return false;

	castle = (gm->castle >> (gm->turn * 2)) & 0x3;
	x = SQI(square);
	y = SQJ(square);

	if ((castle & 1) &&
			(GET_PIECE(gm->board, x, y + 1) == 0) &&
			(GET_PIECE(gm->board, x, y + 2) == 0))
		set_move(gm->moves, gm->move_cnt++, prepare_move_xy(x, y, x, y + 2));

	if ((castle & 2) &&
			(GET_PIECE(gm->board, x, y - 1) == 0) &&
			(GET_PIECE(gm->board, x, y - 2) == 0) &&
			(GET_PIECE(gm->board, x, y - 3) == 0)) 
		set_move(gm->moves, gm->move_cnt++, prepare_move_xy(x, y, x, y - 2));

	return false;
}

bool list_moves(struct game *gm)
{
	uint8_t i, j, square, fig;

	gm->move_cnt = 0;

	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			square = SQUARE(i, j);
			fig = get_piece(gm->board, square);
			if ((fig) && (COL(fig) == gm->turn) &&
					((pawn_moves(gm, square))  ||
					(knight_moves(gm, square)) ||
					(brq_moves(gm, square))    ||
					(king_moves(gm, square))   ||
					(castle_moves(gm, square))))
						return true;
		}
	}
}

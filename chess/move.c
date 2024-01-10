#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "move.h"

static void print_square(uint8_t square)
{
	printf("%c%d",
	       'a' + (char)((square >> 0) & 0x7),
	        1  + ((square >> 3) & 0x7)
	       );
}

void print_move(uint16_t mov)
{
	print_square((uint8_t)mov);
	print_square((uint8_t)(mov >> 6));
	printf(" ");
}


uint16_t get_move(uint32_t *moves, uint8_t pos)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t pair = *((uint32_t *)(my_moves + 3 * (pos / 2)));
	uint8_t offset = 12 * (pos % 2);

	return (uint16_t) ((pair >> offset) & 0xffffff);
}

static inline uint16_t prepare_move_xy(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	return	(((uint16_t)x2 & 0x7) << 9) |
		(((uint16_t)y2 & 0x7) << 6) |
		(((uint16_t)x1 & 0x7) << 3) |
		(((uint16_t)y1 & 0x7) << 0);
}

static inline uint16_t prepare_move_square(uint8_t start, uint8_t end)
{
	return  (((uint16_t)end   & 0x3f) << 6) |
		(((uint16_t)start & 0x3f) << 0);
}

void set_move(uint32_t *moves, uint8_t pos, uint16_t move)
{
	uint8_t  *my_moves = (uint8_t *)moves;
	uint32_t *pair = (uint32_t *)(my_moves + 3 * (pos / 2));
	uint32_t  this_move = (uint32_t)move;
	uint8_t   offset = 12 * (pos % 2);

	*pair &= ~(0xffffff << offset);
	*pair |= this_move << offset;
}

static bool pawn_moves(struct game *gm, uint8_t square)
{
	uint8_t figure, color, dest, take;
	int8_t dx;

	figure = get_piece(gm->board, square);

	if (FIG(figure) != PAWN)
		return false;

	color = COL(figure);

	if (color)
		dx = -1;
	else
		dx = 1;

	dest = SQUARE(SQI(square) + dx, SQJ(square));
	if (get_piece(gm->board, dest) == 0) {
		set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
		if ((1 == dx) && (1 == SQI(square)) ||
			((-1 == dx) && (6 == SQI(square)))) {
			dest = SQUARE(SQI(square) + 2 * dx, SQJ(square));
			if (get_piece(gm->board, dest) == 0) 
				set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
		}
	}

	if (SQJ(square) > 0) {
		dest = SQUARE(SQI(square) + dx, SQJ(square) - 1);
		take = get_piece(gm->board, dest);
		if (((0 != take) && (COL(take) != color)) || (dest == gm->en_passant)) {
			if (FIG(take) == KING)
				return true;
			set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
		}
	}

	if (SQJ(square) < 7) {
		dest = SQUARE(SQI(square) + dx, SQJ(square) + 1);
		take = get_piece(gm->board, dest);
		if (((0 != take) && (COL(take) != color)) || (dest == gm->en_passant)) {
			if (FIG(take) == KING)
				return true;
			set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
		}
	}

	return false;

}

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

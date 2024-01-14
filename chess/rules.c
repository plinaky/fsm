#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "rules.h"

/*
struct piece default_board[8][8] = {
	{WR_, WN_, WB_, WQ_, WK_, WB_, WN_, WR_},
	{WP_, WP_, WP_, WP_, WP_, WP_, WP_, WP_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, OO_, OO_, OO_, OO_, OO_, OO_, OO_},
	{BP_, BP_, BP_, BP_, BP_, BP_, BP_, BP_},
	{BR_, BN_, BB_, BQ_, BK_, BB_, BN_, BR_}
};
*/

struct piece default_board[8][8] = {
	{WR_, OO_, OO_, OO_, WK_, OO_, OO_, WR_},
	{WP_, WB_, WP_, WQ_, WP_, WP_, WB_, WP_},
	{WN_, OO_, OO_, WP_, OO_, WN_, WP_, OO_},
	{OO_, BB_, OO_, OO_, OO_, OO_, OO_, OO_},
	{OO_, BP_, BP_, BP_, BP_, BP_, OO_, OO_},
	{BN_, OO_, OO_, OO_, BB_, BN_, OO_, OO_},
	{BP_, WP_, OO_, BQ_, OO_, BP_, BP_, BP_},
	{BR_, OO_, OO_, OO_, BK_, OO_, OO_, BR_}
};

char to_char(const struct piece pi)
{
	char c = ' ';

	if (pi.fig == PAWN)   c = 'P';
	if (pi.fig == KNIGHT) c = 'N';
	if (pi.fig == BISHOP) c = 'B';
	if (pi.fig == ROOK)   c = 'R';
	if (pi.fig == QUEEN)  c = 'Q';
	if (pi.fig == KING)   c = 'K';

	if (BLACK == pi.col)
		c = c + 'a' - 'A' ; 

	return c;
}

struct piece get_piece(struct position *po, int8_t li, int8_t co)
{
	uint16_t i = li;
	uint16_t j = co;
	uint16_t index = 8 * i + j;
	uint8_t fig = 0xf & (po->board[index / 2] >> (4 * (j % 2)));
	struct piece res = *((struct piece *)(&fig));

	return res;
}

void set_piece(struct position *po, int8_t li, int8_t co, struct piece pi)
{
	uint16_t i = li;
	uint16_t j = co;
	uint16_t index = 8 * i + j;
	uint8_t fig = *((uint8_t *)(&pi));

	po->board[index / 2] &= ~(0xf << (4 * (j % 2)));
	po->board[index / 2] |= fig << (4 * (j % 2));
}

void print_pos(struct position *po)
{
	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("\n\t    a   b   c   d   e   f   g   h\n");
	printf("\t   -------------------------------\n");

	for (int i = 7; i >= 0; i--) {

		printf("\t%d |", i + 1);

		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);
			printf(" %c ", to_char(get_piece(po, i, j)));

			if ((i + j) % 2)
				printf("%s", inv_stop);

			printf("|");
		}

		printf(" %d\n", i + 1);
		printf("\t   -------------------------------\n");
	}

	printf("\t    a   b   c   d   e   f   g   h\n");
}

static void print_square(uint8_t li, uint8_t co)
{
	printf("%c%d",
	       'a' + (char)co,
	        1  + li
	       );
}

void print_move(struct move mo)
{
	print_square(mo.lin1, mo.col1);
	print_square(mo.lin2, mo.col2);
	if (EMPTY != mo.promo_fig) {
		struct piece pi;
		pi.col = mo.promo_col;
		pi.fig = mo.promo_fig;
		printf("%c", to_char(pi));
	}
	printf(" ");
}

static inline void set_move(struct move *mo, uint64_t *cnt, int8_t lin1, int8_t col1, int8_t lin2, int8_t col2)
{
	mo[*cnt].lin1 = lin1;
	mo[*cnt].col1 = col1;
	mo[*cnt].lin2 = lin2;
	mo[*cnt].col2 = col2;
	mo[*cnt].promo_col = WHITE;
	mo[*cnt].promo_fig = EMPTY;
	(*cnt)++;
}

static inline void set_promo(struct move *mo, uint64_t *cnt, int8_t lin1, int8_t col1, int8_t lin2, int8_t col2, enum color col)
{
	enum figure fig;

	for (fig = KNIGHT; fig <= QUEEN; fig++) {
		mo[*cnt].lin1 = lin1;
		mo[*cnt].col1 = col1;
		mo[*cnt].lin2 = lin2;
		mo[*cnt].col2 = col2;
		mo[*cnt].promo_col = col;
		mo[*cnt].promo_fig = fig;
		(*cnt)++;
	}
}


static bool pawn_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint64_t *cnt)
{
	struct piece pi, take;
	int8_t dx, epl;

	pi = get_piece(po, li, co);

	if (pi.fig != PAWN)
		return false;

	if (BLACK == pi.col) {
		dx = -1;
		epl = 2;
	} else {
		dx = 1;
		epl = 5;
	}

	if ((li + dx >= 0) && (li + dx <= 7)) {

		take = get_piece(po, li + dx, co);
		if (EMPTY == take.fig) {
			if ((1 == dx) && (li < 6) || ((-1 == dx) && (li > 1)))
				set_move(mo, cnt, li, co, li + dx, co);
			else
				set_promo(mo, cnt, li, co, li + dx, co, pi.col);
			if ((1 == dx) && (1 == li) || ((-1 == dx) && (6 == li))) {
				take = get_piece(po, li + 2 * dx, co);
				if (EMPTY == take.fig)
					set_move(mo, cnt, li, co, li + 2 * dx, co);
			}
		}

		if (co > 0) {
			take = get_piece(po, li + dx, co - 1);
			if ((EMPTY != take.fig) && (pi.col != take.col)) {
				if (KING == take.fig)
					return true;
				if ((1 == dx) && (li < 6) || ((-1 == dx) && (li > 1)))
					set_move(mo, cnt, li, co, li + dx, co - 1);
				else
					set_promo(mo, cnt, li, co, li + dx, co - 1, pi.col);
			}

			if ((po->a_passe) && (co - 1 == po->en_passant) && (li + dx == epl))
				set_move(mo, cnt, li, co, li + dx, co - 1);
		}

		if (co < 7) {
			take = get_piece(po, li + dx, co + 1);
			if ((EMPTY != take.fig) && (pi.col != take.col)) {
				if (KING == take.fig)
					return true;
				if ((1 == dx) && (li < 6) || ((-1 == dx) && (li > 1)))
					set_move(mo, cnt, li, co, li + dx, co + 1);
				else
					set_promo(mo, cnt, li, co, li + dx, co + 1, pi.col);
			}

			if ((po->a_passe) && (co + 1 == po->en_passant) && (li + dx == epl))
				set_move(mo, cnt, li, co, li + dx, co - 1);
		}

	}

	return false;
}

static bool kk_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint64_t *cnt)
{
	struct piece pi, take;
	int8_t index, x, y, start, stop;
	int8_t moves[16][2] = {
		{ 1,  2},
		{ 1, -2},
		{ 2,  1},
		{ 2, -1},
		{-1,  2},
		{-1, -2},
		{-2,  1},
		{-2, -1},
		{ 1, -1},
		{ 1,  0},
		{ 1,  1},
		{ 0, -1},
		{ 0,  1},
		{-1, -1},
		{-1,  0},
		{-1,  1}

	};

	pi = get_piece(po, li, co);

	if (KNIGHT == pi.fig) {
		start = 0;
		stop = 8;
	} else if (KING == pi.fig) {
		start = 8;
		stop = 16;
	} else {
		return false;
	}

	for (index = start ; index < stop ; index++) {
		x = li + moves[index][0];
		y = co + moves[index][1];
		if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {
			take = get_piece(po, x, y);
			if (EMPTY == take.fig) {
				set_move(mo, cnt, li, co, x, y);
			} else if (pi.col != take.col) {
				if (KING == take.fig)
					return true;
				set_move(mo, cnt, li, co, x, y);
			}
		}
	}

	return false;
}

static bool brq_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint64_t *cnt)
{
	struct piece pi, take;
	int8_t cnt1, cnt2, start, stop, x, y;
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

	pi = get_piece(po, li, co);

	if (BISHOP == pi.fig) {
		start = 0;
		stop  = 4;
	} else if (ROOK == pi.fig) {
		start = 4;
		stop  = 8;
	} else if (QUEEN == pi.fig) {
		start = 0;
		stop  = 8;
	} else {
		return false;
	}

	for (cnt1 = start ; cnt1 < stop; cnt1++) {
		for (cnt2 = 1 ; cnt2 < 8 ; cnt2++) {
			x = li + cnt2 * offsets[cnt1][0];
			y = co + cnt2 * offsets[cnt1][1];
			if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {
				take = get_piece(po, x, y);
				if (EMPTY == take.fig) {
					set_move(mo, cnt, li, co, x, y);
				} else if (pi.col != take.col) {
					if (KING == take.fig)
						return true;
					set_move(mo, cnt, li, co, x, y);
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

static bool castle_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint64_t *cnt)
{
	struct piece pi, t1, t2, t3;
	int8_t castle, x, y;

	pi = get_piece(po, li, co);

	if (pi.fig != KING)
		return false;

	castle = (po->castle >> ((int8_t)(po->turn) * 2)) & 0x3;

	if (castle & 1) {
		t1 = get_piece(po, li, co + 1);
		t2 = get_piece(po, li, co + 2);
		if ((EMPTY == t1.fig) && (EMPTY == t2.fig))
			set_move(mo, cnt, li, co, li, co + 2);
	}

	if (castle & 2) {
		t1 = get_piece(po, li, co - 1);
		t2 = get_piece(po, li, co - 2);
		t2 = get_piece(po, li, co - 3);
		if ((EMPTY == t1.fig) && (EMPTY == t2.fig))
			set_move(mo, cnt, li, co, li, co - 2);
	}

	return false;
}

bool list_moves(struct position *po, struct move *mo, uint64_t *cnt)
{
	struct piece pi;
	int8_t i, j;

	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			pi = get_piece(po, i, j);
			if ((EMPTY != pi.fig) && (pi.col == po->turn)) {
				if (pawn_moves(po, i, j, mo, cnt))
					return true;
				else if (kk_moves(po, i, j, mo, cnt))
					return true;
				else if (brq_moves(po, i, j, mo, cnt))
					return true;
				else if (castle_moves(po, i, j, mo, cnt))
					return true;
			}
		}
	}

	return false;
}

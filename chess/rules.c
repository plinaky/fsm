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
	int8_t dx, epl /* en passant line */;

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

			if (((1 == po->B_OO_2) && ((li + dx) == 0) && ((co - 1) == 5)) ||
			 ((1 == po->W_OO_2) && ((li + dx) == 7) && ((co - 1) == 5)) ||
			 ((1 == po->B_OOO_2) && ((li + dx) == 0) && ((co - 1) == 3)) ||
			 ((1 == po->W_OOO_2) && ((li + dx) == 7) && ((co - 1) == 3)))
				return true;

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

			if (((1 == po->B_OO_2) && ((li + dx) == 0) && ((co - 1) == 5)) ||
			 ((1 == po->W_OO_2) && ((li + dx) == 7) && ((co - 1) == 5)) ||
			 ((1 == po->B_OOO_2) && ((li + dx) == 0) && ((co - 1) == 3)) ||
			 ((1 == po->W_OOO_2) && ((li + dx) == 7) && ((co - 1) == 3)))
				return true;

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
				set_move(mo, cnt, li, co, li + dx, co + 1);
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

			if (((1 == po->B_OO_2) && (x == 0) && (y == 5)) ||
			 ((1 == po->W_OO_2) && (x == 7) && (y == 5)) ||
			 ((1 == po->B_OOO_2) && (x == 0) && (y == 3)) ||
			 ((1 == po->W_OOO_2) && (x == 7) && (y == 3)))
				return true;

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

				if (((1 == po->B_OO_2) && (x == 0) && (y == 5)) ||
				((1 == po->W_OO_2) && (x == 7) && (y == 5)) ||
				((1 == po->B_OOO_2) && (x == 0) && (y == 3)) ||
				((1 == po->W_OOO_2) && (x == 7) && (y == 3)))
					return true;

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

	if (((WHITE == po->turn) && (po->W_OO_1)) ||
			((BLACK == po->turn) && (po->B_OO_1))) {
		t1 = get_piece(po, li, co + 1);
		t2 = get_piece(po, li, co + 2);
		if ((EMPTY == t1.fig) && (EMPTY == t2.fig))
			set_move(mo, cnt, li, co, li, co + 2);
	}

	if (((WHITE == po->turn) && (po->W_OOO_1)) ||
			((BLACK == po->turn) && (po->B_OOO_1))) {
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

static void apply_move(struct position *po, struct move mo)
{
	struct piece pi1, pi2, pi3;
	int8_t dx;

	pi2.col = WHITE;
	pi2.fig = EMPTY;

	if (EMPTY != mo.promo_fig) {
		pi1.col = mo.promo_col;
		pi1.fig = mo.promo_fig;
	} else {
		pi1 = get_piece(po, mo.lin1, mo.col1);
	}
	set_piece(po, mo.lin1, mo.col1, pi2);
	if (po->a_passe) {
		if (BLACK == po->turn)
			dx = 1;
		else
			dx = -1;
		set_piece(po, mo.lin2 + dx, mo.col2, pi1);
		po->a_passe = 0;
		po->en_passant = 0;
	}
	set_piece(po, mo.lin2, mo.col2, pi1);

	if ((KING == pi1.fig) && (WHITE == pi1.col)) {
		po->W_OO_1 = 0;
		po->W_OOO_1 = 0;
		pi3.col = WHITE;
		pi3.fig = ROOK;
		if (mo.col1 + 2 == mo.col2) {
			po->W_OO_2 = 1;
			set_piece(po, mo.lin2, mo.col1 + 1, pi3);
			set_piece(po, mo.lin2, 7, pi2);
		} else if (mo.col1 + 2 == mo.col2) {
			po->W_OOO_2 = 1;
			set_piece(po, mo.lin2, mo.col2 + 1, pi3);
			set_piece(po, mo.lin2, 0, pi2);
		}
	} else {
		po->W_OO_2 = 0;
		po->W_OOO_2 = 0;
	}

	if ((ROOK == pi1.fig) && (0 == mo.lin1) && (0 == mo.col1))
		po->W_OOO_1 = 0;

	if ((ROOK == pi1.fig) && (0 == mo.lin1) && (7 == mo.col1))
		po->W_OO_1 = 0;

	if ((ROOK == pi1.fig) && (7 == mo.lin1) && (0 == mo.col1))
		po->B_OOO_1 = 0;

	if ((ROOK == pi1.fig) && (7 == mo.lin1) && (7 == mo.col1))
		po->B_OO_1 = 0;

	po->a_passe = 0;
	po->en_passant = 0;
	if ((PAWN == pi1.fig) && ((mo.lin2 == mo.lin1 + 2) || (mo.lin1 == mo.lin2 + 2))) {
		po->a_passe = 1;
		po->en_passant = mo.col1;
	}
}

bool list_legal_moves(struct position *po, struct move *mo, uint64_t *cnt)
{
	struct move mo1[200];
	struct move mo2[200];
	uint64_t cnt1 = 0, cnt2 = 0;
	bool res;

	res = list_moves(po, (struct move *)mo1, &cnt1);
	if (true == res)
		return true;

	for (uint8_t i = 0; i < cnt1; i++) {
		struct position next;
		memcpy(&next, po, sizeof(struct position));
		next.turn = BLACK - next.turn;
		apply_move(&next, mo1[i]);
		res = list_moves(po, (struct move *)(mo2), &cnt2);
		if (res) {
			memmove(mo1 + i, mo1 + i + 1, sizeof(struct move) * (cnt1 - i - 1));
			cnt1--;
		}

	}
	memcpy(mo, mo1, cnt1 * sizeof(struct move));
	*cnt += cnt1;

	return false;

}

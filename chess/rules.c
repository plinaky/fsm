#include <stdio.h>
#include <string.h>
#include "rules.h"
#include <time.h>
#include <stdlib.h>

char to_char(enum color co, enum figure fi)
{
	char c = ' ';

	if (fig == PAWN)   c = 'P';
	if (fig == KNIGHT) c = 'N';
	if (fig == BISHOP) c = 'B';
	if (fig == ROOK)   c = 'R';
	if (fig == QUEEN)  c = 'Q';
	if (fig == KING)   c = 'K';

	if (BLACK == co)
		c = c + 'a' - 'A' ; 

	return c;
}

static inline void print_square(struct square *sq)
{
	printf("%c%d",
	       'a' + (char)(sq->y),
	        1  + sq->x
	       );
}

static void print_move(struct move *mo)
{
	printf("%c", to_char(pi1));

	print_square(&mo->sq1);

	if (mo->pi1.col != mo->pi2.col)
		printf("x");
	else
		printf("-");

	print_square(&mo->sq2);

 	if (mo->pi1.fig != mo->pi2.fig)
 		printf("=%c", to_char(WHITE, mo->pi1.fig));
 	else
 		printf("  ");
}

void synthesis(struct position *po, struct move *mo, uint16_t cnt)
{
	int8_t i, j;
	uint8_t k, l;

	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("   a  b  c  d  e  f  g  h    ",);
	printf("%s to play \t WHITE CR: %s %s \t BLACK CR: %s %s\n",
	       (po->turn ? "BLACK" : "WHITE"),
	       (po->wcr & BIG_CR   ? "OOO" : "xxx"),
	       (po->wcr & SMALL_CR ? "OO " : "xx "),
	       (po->bcr & BIG_CR   ? "OOO" : "xxx"),
	       (po->bcr & SMALL_CR ? "OO " : "xx ")
	       );

	for (i = 7; i >= 0; i--) {

		printf("%d ", i + 1);

		for (j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);

			printf(" %c ",
			       to_char(po->board[i][j].col,
				       po->board[i][j].fig));

			if ((i + j) % 2)
				printf("%s", inv_stop);

		}

		printf("   ");

		for (l = 0; ((k + l) < cnt) && (l < 15); l++) 
			print_move(mo[k + l], po);

		k += l;
		printf("\n");
	}


}

static inline void set_move(struct move *mo, struct  )
{
	mo[*cnt].lin1 = lin1;
	mo[*cnt].col1 = col1;
	mo[*cnt].lin2 = lin2;
	mo[*cnt].col2 = col2;
	mo[*cnt].promo_col = WHITE;
	mo[*cnt].promo_fig = EMPTY;
	(*cnt)++;
}

static inline void set_promo(struct move *mo, uint16_t *cnt, int8_t lin1, int8_t col1, int8_t lin2, int8_t col2, enum color col)
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


static bool pawn_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint16_t *cnt)
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
			 ((1 == po->W_OOO_2) && ((li + dx) == 7) && ((co - 1) == 3))) {
				return true;
			}

			take = get_piece(po, li + dx, co - 1);
			if ((EMPTY != take.fig) && (pi.col != take.col)) {
				if ((1 == dx) && (li < 6) || ((-1 == dx) && (li > 1)))
					set_move(mo, cnt, li, co, li + dx, co - 1);
				else
					set_promo(mo, cnt, li, co, li + dx, co - 1, pi.col);
				if (KING == take.fig)
					return true;
			}

			if ((po->a_passe) && (co - 1 == po->en_passant) && (li + dx == epl))
				set_move(mo, cnt, li, co, li + dx, co - 1);
		}

		if (co < 7) {

			if (((1 == po->B_OO_2) && ((li + dx) == 0) && ((co + 1) == 5)) ||
			 ((1 == po->W_OO_2) && ((li + dx) == 7) && ((co + 1) == 5)) ||
			 ((1 == po->B_OOO_2) && ((li + dx) == 0) && ((co + 1) == 3)) ||
			 ((1 == po->W_OOO_2) && ((li + dx) == 7) && ((co + 1) == 3))) {
				return true;
			}

			take = get_piece(po, li + dx, co + 1);

			if ((EMPTY != take.fig) && (pi.col != take.col)) {
				if ((1 == dx) && (li < 6) || ((-1 == dx) && (li > 1)))
					set_move(mo, cnt, li, co, li + dx, co + 1);
				else
					set_promo(mo, cnt, li, co, li + dx, co + 1, pi.col);
				if (KING == take.fig)
					return true;
			}

			if ((po->a_passe) && (co + 1 == po->en_passant) && (li + dx == epl))
				set_move(mo, cnt, li, co, li + dx, co + 1);
		}

	}

	return false;
}

static bool kk_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint16_t *cnt)
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

	for (index = start; index < stop; index++) {
		x = li + moves[index][0];
		y = co + moves[index][1];

		if ((x >= 0) && (x <= 7) && (y >= 0) && (y <= 7)) {

			if (((1 == po->B_OO_2) && (x == 0) && (y == 5)) ||
			 ((1 == po->W_OO_2) && (x == 7) && (y == 5)) ||
			 ((1 == po->B_OOO_2) && (x == 0) && (y == 3)) ||
			 ((1 == po->W_OOO_2) && (x == 7) && (y == 3))) {
				return true;
			}
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

static bool brq_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint16_t *cnt)
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

	for (cnt1 = start; cnt1 < stop; cnt1++) {
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

static bool castle_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint16_t *cnt)
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

static bool list_moves(struct position *po, struct move *mo, uint16_t *cnt)
{
	struct piece pi;
	int8_t i, j;

	*cnt = 0;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			pi = get_piece(po, i, j);
			if ((EMPTY != pi.fig) && (pi.col == po->turn)) {
				if ((pawn_moves(po, i, j, mo, cnt)) ||
				    (kk_moves(po, i, j, mo, cnt))   ||
				    (brq_moves(po, i, j, mo, cnt))  ||
				    (castle_moves(po, i, j, mo, cnt)))
					return true;
			}
		}
	}

	return false;
}

void apply_move(struct position *po, struct move mo)
{
	struct piece pi1, pi2, pi3;
	int8_t x, dx;

	pi2.col = WHITE;
	pi2.fig = EMPTY;

	if (EMPTY != mo.promo_fig) {
		pi1.col = mo.promo_col;
		pi1.fig = mo.promo_fig;
	} else {
		pi1 = get_piece(po, mo.lin1, mo.col1);
	}

	set_piece(po, mo.lin1, mo.col1, pi2);
	set_piece(po, mo.lin2, mo.col2, pi1);

	if (po->a_passe) {
		if (BLACK == po->turn) {
			x = 2;
			dx = 1;
		} else {
			x = 5;
			dx = -1;
		}
		if ((x == mo.lin2) && (po->en_passant == mo.col2))
			set_piece(po, mo.lin2 + dx, mo.col2, pi2);
	}

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
	}

	if ((KING == pi1.fig) && (BLACK == pi1.col)) {
		po->B_OO_1 = 0;
		po->B_OOO_1 = 0;
		pi3.col = BLACK;
		pi3.fig = ROOK;
		if (mo.col1 + 2 == mo.col2) {
			po->B_OO_2 = 1;
			set_piece(po, mo.lin2, mo.col1 + 1, pi3);
			set_piece(po, mo.lin2, 7, pi2);
		} else if (mo.col1 + 2 == mo.col2) {
			po->B_OOO_2 = 1;
			set_piece(po, mo.lin2, mo.col2 + 1, pi3);
			set_piece(po, mo.lin2, 0, pi2);
		}
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

	po->turn = BLACK - po->turn;
}

bool list_legal_moves(struct position *po, struct move *mo, uint16_t *cnt)
{
	struct position next;
	struct move mo1[200];
	struct move mo2[200];
	uint16_t cnt1, cnt2;
	uint8_t i;
	bool res;

	*cnt = 0;
	res = list_moves(po, (struct move *)mo1, &cnt1);
	if (true == res) {
		synthesis((struct move *)mo1, cnt1, po);
		printf("*********************  ERROR   *********************** !\n");
		return res;
	}

	for (i = 0; i < cnt1; i++) {
		memcpy(&next, po, sizeof(struct position));
		apply_move(&next, mo1[i]);
		res = list_moves(&next, (struct move *)(mo2), &cnt2);
		if ((res) && (cnt1 > 0)) {
			memmove(&mo1[i], &mo1[i + 1], sizeof(struct move) * (cnt1 - i - 1));
			cnt1--;
			i--;
		}
	}

	if (0 == cnt1) {
		memcpy(&next, po, sizeof(struct position));
		next.turn =  BLACK - next.turn;
		res = list_moves(&next, (struct move *)(mo2), &cnt2);
	}

	memcpy(mo, mo1, cnt1 * sizeof(struct move));
	*cnt = cnt1;

	/* means checkmate */
	return res;
}

uint16_t play_game(struct position *po)
{
	int r;
	uint16_t cnt, i;
	struct move mo[200];
	bool res;

	for (i = 0; i < 100; i++) {
		res = list_legal_moves(po, mo, &cnt);
		if (0 == cnt) {
			if (res) {
				//printf("******* CHECKMATE at move %d! **************\n", i);
				//synthesis(mo, cnt, po);
			} else {
				printf("******* DRAW at move %d!      **************\n", i);
				synthesis(mo, cnt, po);
			 }
			return i;
		}
		r = rand() % cnt;
		apply_move(po, mo[r]);
	}

	//printf("******* NO WIN after move %d!      **************\n", i);
	//synthesis(mo, cnt, po);
	return i;
}

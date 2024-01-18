#include <stdio.h>
#include <string.h>
#include "rules.h"
#include <time.h>
#include <stdlib.h>

static inline bool in_bound(int8_t x, int8_t y)
{
	return (0 <= x) && (x < 8) && (0 <= y) && (y < 8);
}

static inline bool on_bound(int8_t x)
{
	return (0 == x) || (x == 8);
}

static inline struct move_of(int8_t x1, int8_t y1, int8_t x2, int8_t y2, enum figure fig)
{
	struct move mo;

	mo.sq1.x = (uint8_t)x1;
	mo.sq1.y = (uint8_t)y1;
	mo.sq2.x = (uint8_t)x2;
	mo.sq2.y = (uint8_t)y2;
	mo.promo = fig;

	return mo;
}

static inline void set_single_move(struct move *mo, int8_t x1, int8_t y1, int8_t x2, int8_t y2, enum figure fig)
{
	mo->x1    = x1;
	mo->y1    = y1;
	mo->x2    = x2;
	mo->y2    = y2;
	mo->promo = fig;
}


enum move_type {
	KING_TAKE    = 0,
	MOVE_STOP    = 1,
	MOVE_ON      = 2,
}

static enum move_type add_move(struct position *po, struct move *mo, uint8_t *cnt, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	if (!in_bound(x2, y2))
		return MOVE_STOP;

	if ((EMPTY != po->board[x2][y2].fig) && (po->board[x1][y1].col == po->board[x2][y2].col))
		return MOVE_STOP;

	if ((KING == po->board[x2][y2].fig) || ((x2 == po->board.x) && (y2 == po->board.y) && (on_bound(x2))))
		return KING_TAKE;

	if (PAWN == po->board[x1][y1].fig) {
		if (((EMPTY == po->board[x2][y2].fig) && (y1 == y2)) ||
		    ((EMPTY != po->board[x2][y2].fig) && (y1 != y2))) { 
			if (on_bound(x2)) { /* pawn promotion */
				set_single_move(&mo[*cnt++], x1, y1, x2, y2, KNIGHT);
				set_single_move(&mo[*cnt++], x1, y1, x2, y2, BISHOP);
				set_single_move(&mo[*cnt++], x1, y1, x2, y2, ROOK  );
				set_single_move(&mo[*cnt++], x1, y1, x2, y2, QUEEN );
				return MOVE_STOP;
			}
			set_single_move(&mo[*cnt++], x1, y1, x2, y2, EMPTY );
			return MOVE_ON;
		}
		return MOVE_STOP;
	}

	set_single_move(&mo[*cnt++], x1, y1, x2, y2, EMPTY);

	if (EMPTY != po->board[x2][y2].fig)
		return MOVE_STOP;
	else
		return MOVE_ON;
}

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

static inline void print_square(int8_t x, int8_t y)
{
	printf("%c%d",
	       'a' + (char)(sq.y),
	        1  + sq.x
	       );
}
static void print_move(struct position *po, struct move *mo)
{
	struct piece *p1 = &(po->board[mo->x1][mo->y1]);
	struct piece *p2 = &(po->board[mo->x2][mo->y2]);

	printf("%c", to_char(WHITE, p1->fig));

	print_square(mo->x1, mo->y1);

	if ((EMPTY != pi2->fig) || ((mo->x2 == bo->x) && (mo->y2 == bo->y)))
		printf("x");
	else
		printf("-");

	print_square(mo->x2, mo->y2);

 	if (EMPTY != mo->promo)
 		printf("=%c", to_char(WHITE, mo->promo));
 	else
 		printf("  ");
}

void synthesis(struct position *po, struct move *mo, uint16_t cnt)
{
	uint8_t i, j, k, l;

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

static bool add_pkk_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint8_t *cnt)
{
	uint8_t i, start, stop;
	int8_t moves[24][2] = {
		/* WHITE PAWN */
		{ 1,  0}, { 2,  0}, { 1, -1}, { 1, -1},

		/* BLACK PAWN */
		{-1,  0}, {-2,  0}, {-1, -1}, {-1, -1},

		/* KNIGHT */
		{ 1,  2}, { 1, -2}, { 2,  1}, { 2, -1},
		{-1,  2}, {-1, -2}, {-2,  1}, {-2, -1},

		/* KING */
		{ 1, -1}, { 1,  0}, { 1,  1}, { 0, -1},
		{ 0,  1}, {-1, -1}, {-1,  0}, {-1,  1},
	};

	if (po->turn != po->board[li][co].col)
		return false;

	if ((po->board[li][co].fig == PAWN) && (WHITE == po->board[lin][co].col)) 
			start = 0, stop  = 4;
	else if ((po->board[li][co].fig == PAWN) && (BLACK == po->board[lin][co].col)) 
			start = 4, stop  = 8;
	else if (po->board[li][co].fig == KNIGHT)
			start = 8, start = 16;
	else if (po->board[li][co].fig == KING)
			start = 16, start = 24;
	else
		return false;

	for (i = start; i < stop; i++) {
		x = li + moves[i][0], y = co + moves[i][1];
		if (add_move(mo, cnt, li, co, x, y) == KING_TAKE)
			return true;
	}

	return false;
}

static bool brq_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint16_t *cnt)
{
	int8_t cnt1, cnt2, start, stop;
	int8_t offsets[8][2] = {
		{ 1,  1}, { 1, -1}, {-1, -1}, {-1,  1},
		{ 0,  1}, { 0, -1}, {-1,  0}, { 1,  0},
	};

	if (po->turn != po->board[li][co].col)
		return false;

	if (po->board[lin][col].fig == BISHOP)
			start = 0, start = 8;
	else if (po->board[lin][col].fig == ROOK)
			start = 8, start = 16;
	else if (po->board[lin][col].fig == QUEEN)
			start = 0, start = 26;
	else
		return false;

	for (cnt1 = start; cnt1 < stop; cnt1++) {
		for (cnt2 = 1 ; cnt2 < 8 ; cnt2++) {
			x = li + cnt2 * offsets[cnt1][0];
			y = co + cnt2 * offsets[cnt1][1];
			enum move_type mt = add_move(mo, cnt, li, co, x, y);
			if (mt == KING_TAKE)
				return true;
			else if (mt == MOVE_STOP)
				break;
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

#include <stdio.h>
#include <string.h>
#include "rules.h"
#include <time.h>
#include <stdlib.h>

enum move_type {
	KING_TAKE    = 0,
	MOVE_STOP    = 1,
	MOVE_ON      = 2,
};

enum debug_level {
	DEBUG_NO     = 0,
	DEBUG_WRONG  = 1,
	DEBUG_PAT    = 2,
	DEBUG_MAT    = 3,
	DEBUG_DRAW   = 4,
	DEBUG_MOVE   = 5,
	DEBUG_NEXT   = 6
};

static enum debug_level chess_debug_level = DEBUG_MOVE;
static bool debug_offset = false;


static char to_char(enum color co, enum figure fi)
{
	char c = ' ';

	if (fi == PAWN)   c = 'P';
	if (fi == KNIGHT) c = 'N';
	if (fi == BISHOP) c = 'B';
	if (fi == ROOK)   c = 'R';
	if (fi == QUEEN)  c = 'Q';
	if (fi == KING)   c = 'K';

	if (BLACK == co)
		c = c + 'a' - 'A' ; 

	return c;
}

static inline void print_square(int8_t x, int8_t y)
{
	printf("%c%d",
	       'a' + (char)(y),
	        1  + x
	       );
}

static void print_move(struct position *po, struct move mo)
{
	struct piece *p1 = &(po->board[mo.x1][mo.y1]);
	struct piece *p2 = &(po->board[mo.x2][mo.y2]);

	printf("%c", to_char(WHITE, p1->fig));

	print_square(mo.x1, mo.y1);

	if ((EMPTY != p2->fig) || ((mo.x2 == po->x) && (mo.y2 == po->y)))
		printf("x");
	else
		printf("-");

	print_square(mo.x2, mo.y2);

 	if (EMPTY != mo.promo)
 		printf("=%c  ", to_char(WHITE, mo.promo));
	else
		printf("    ");
}

void synthesis(struct position *po, struct move *mo, uint8_t cnt)
{
	int8_t i, j, k, l;

	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	if (debug_offset)
		printf("\t\t");

	printf("   a  b  c  d  e  f  g  h    ");

	printf("%s have %2d moves",
			(po->turn == WHITE ? "WHITE" : "BLACK"),
			cnt
	      );
	printf("  ");

	printf("Wcr:%s.%s    Bcr:%s.%s    ",
			(po->wcr & SMALL_CR ? "OO"  : "__"),
			(po->wcr & BIG_CR   ? "OOO" : "___"),
			(po->bcr & SMALL_CR ? "OO"  : "__"),
			(po->bcr & BIG_CR   ? "OOO" : "___")
	      );

	if ((po->x != -1) && (po->y != -1)) {
		printf("en passant/castle:");
		print_square(po->x, po->y);
		printf("    ");
	}

	printf("\n");


	for (i = 7; i >= 0; i--) {

		if (debug_offset)
			printf("\t\t");

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
		l = 16;
		for (k = 0; k < l; k++) {
			if (((7 - i) + k * 8) < cnt)
				print_move(po, mo[7 - i + k * 8]);
		}
		printf("\n");
	}


}

static inline bool in_bound(int8_t x, int8_t y)
{
	return (0 <= x) && (x < 8) && (0 <= y) && (y < 8);
}

static inline bool on_bound(int8_t x)
{
	return (0 == x) || (x == 7);
}

static inline void set_single_move(struct move *mo, uint8_t *cnt, int8_t x1, int8_t y1, int8_t x2, int8_t y2, enum figure fig)
{
	mo[*cnt].x1    = x1;
	mo[*cnt].y1    = y1;
	mo[*cnt].x2    = x2;
	mo[*cnt].y2    = y2;
	mo[*cnt].promo = fig;
	(*cnt)++;
}

static void promote(struct position *po, struct move *mo, uint8_t *cnt, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	set_single_move(mo, cnt, x1, y1, x2, y2, KNIGHT);
	set_single_move(mo, cnt, x1, y1, x2, y2, BISHOP);
	set_single_move(mo, cnt, x1, y1, x2, y2, ROOK  );
	set_single_move(mo, cnt, x1, y1, x2, y2, QUEEN );
}

static enum move_type add_move(struct position *po, struct move *mo, uint8_t *cnt, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	enum castling_rights cr = (po->turn == WHITE ? po->wcr : po->bcr);

	/* immediately discard out of board moves */
	if (!in_bound(x2, y2)) {
		return MOVE_STOP;
	}

	/* discard moves if destination is occupied by piece of the same color */
	if ((EMPTY != po->board[x2][y2].fig) && (po->board[x1][y1].col == po->board[x2][y2].col)) {
		return MOVE_STOP;
	}

	/* check cases */
	if ((KING == po->board[x2][y2].fig) || ((x2 == po->x) && (y2 == po->y) && (on_bound(x2)))) {
		set_single_move(mo, cnt, x1, y1, x2, y2, EMPTY);
		return KING_TAKE;
	}

	/* pawn case */
	if (PAWN == po->board[x1][y1].fig) { 
		/* move forward */
		if ((y1 == y2) && (EMPTY == po->board[x2][y2].fig)) {

			/* pawn promotion */
			if (on_bound(x2)) {
				promote(po, mo, cnt, x1, y1, x2, y2);
				return MOVE_ON;
			}

			/* standard advance */
			set_single_move(mo, cnt, x1, y1, x2, y2, EMPTY);

			/* check advance by 2 at start before returning */
			if (on_bound(2 * x1 - x2) && (EMPTY == po->board[2 * x2 - x1][y2].fig))
				set_single_move(mo, cnt, x1, y1, 2 * x2 - x1, y2, EMPTY);
		}

		/* take */
		if ((y1 != y2) && ((EMPTY != po->board[x2][y2].fig) || ((x2 == po->x) && (y2 == po->y)))) {

			/* pawn promotion */
			if (on_bound(x2)) {
				promote(po, mo, cnt, x1, y1, x2, y2);
				return MOVE_ON;
			}

			/* standard take */
			set_single_move(mo, cnt, x1, y1, x2, y2, EMPTY);
		}

		/* to ensure no pawn case further */
		return MOVE_ON;
	}

	/* king castling */
	if ((KING == po->board[x1][y1].fig) && (y1 + 2 == y2) &&
			((!(cr & SMALL_CR)) ||
			(EMPTY != po->board[x2][y2].fig) ||
			(EMPTY != po->board[x2][y1 + 1].fig))) { 
		return MOVE_ON;
	}

	if ((KING == po->board[x1][y1].fig) && (y1 == y2 + 2) &&
			((!(cr & BIG_CR)) ||
			(EMPTY == po->board[x2][1].fig) ||
			(EMPTY == po->board[x2][y2 + 1].fig) ||
			(EMPTY == po->board[x2][y2].fig))) { 
		return MOVE_ON;
	}

	/* anything else */
	set_single_move(mo, cnt, x1, y1, x2, y2, EMPTY);

	if (EMPTY == po->board[x2][y2].fig)
		return MOVE_ON;
	else
		return MOVE_STOP;
}

static bool pkk_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint8_t *cnt)
{
	int8_t i, start, stop, x, y;

	int8_t moves[24][2] = {
		/* WHITE PAWN */
		{ 1, -1}, { 1, -1}, { 1,  0}, 

		/* BLACK PAWN */
		{-1, -1}, {-1, -1}, {-1,  0}, 

		/* KNIGHT */
		{ 1,  2}, { 1, -2}, { 2,  1}, { 2, -1},
		{-1,  2}, {-1, -2}, {-2,  1}, {-2, -1},

		/* KING */
		{ 1,  1}, { 1, -1}, {-1, -1}, {-1,  1},
		{ 0,  1}, { 0, -1}, { 1,  0}, {-1,  0},
		{ 0,  2}, { 0, -2},
	};

	if (po->turn != po->board[li][co].col)
		return false;

	if ((po->board[li][co].fig == PAWN) && (WHITE == po->board[li][co].col)) { 
		start = 0;
		stop  = 3;
	} else if ((po->board[li][co].fig == PAWN) && (BLACK == po->board[li][co].col)) {
		start = 3;
		stop  = 6;
	} else if (po->board[li][co].fig == KNIGHT) {
		start = 6;
		stop = 14;
	} else if (po->board[li][co].fig == KING) {
		start = 14;
		stop = 24;
	} else {
		return false;
	}

	for (i = start; i < stop; i++) {
		x = li + moves[i][0];
	       	y = co + moves[i][1];
		if (add_move(po, mo, cnt, li, co, x, y) == KING_TAKE)
			return true;
	}

	return false;
}

static bool brq_moves(struct position *po, int8_t li, int8_t co, struct move *mo, uint8_t *cnt)
{
	int8_t cnt1, cnt2, start, stop, x, y;
	int8_t offsets[8][2] = {
		{ 1,  1}, { 1, -1}, {-1, -1}, {-1,  1},
		{ 0,  1}, { 0, -1}, {-1,  0}, { 1,  0},
	};

	if (po->turn != po->board[li][co].col)
		return false;

	if (po->board[li][co].fig == BISHOP) {
		start = 0;
		stop =  4;
	} else if (po->board[li][co].fig == ROOK) {
		start = 4;
		stop =  8;
	} else if (po->board[li][co].fig == QUEEN) {
		start = 0;
	       	stop =  8;
	} else {
		return false;
	}

	for (cnt1 = start; cnt1 < stop; cnt1++) {
		for (cnt2 = 1 ; cnt2 < 8 ; cnt2++) {
			x = li + cnt2 * offsets[cnt1][0];
			y = co + cnt2 * offsets[cnt1][1];
			enum move_type mt = add_move(po, mo, cnt, li, co, x, y);
			if (KING_TAKE == mt) {
				return true;
			}
			else if (MOVE_STOP == mt)
				break;
		}
	}

	return false;
}

static bool list_moves(struct position *po, struct move *mo, uint8_t *cnt)
{
	struct piece pi;
	int8_t i, j;

	*cnt = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (pkk_moves(po, i, j, mo, cnt))
				return true;

			if (brq_moves(po, i, j, mo, cnt))
				return true;
		}
	}
	return false;
}

void apply_move(struct position *po, struct move mo)
{
	bool clear_special = true;

	/* king case */
	if (KING == po->board[mo.x1][mo.y1].fig) {

		if (mo.y1 + 2 == mo.y2) {
			po->board[mo.x2][mo.y1 + 1].fig = ROOK; 
			po->board[mo.x2][mo.y1 + 1].col = po->board[mo.x1][mo.y1].col;

			po->x = mo.x2;
			po->y = mo.y1 + 1;
			clear_special = false;
		}

		if (mo.y1 == mo.y2 + 2) {
			po->board[mo.x2][mo.y2 + 1].fig = ROOK; 
			po->board[mo.x2][mo.y2 + 1].col = po->board[mo.x1][mo.y1].col;

			po->x = mo.x2;
			po->y = mo.y2 + 1;
			clear_special = false;
		}

		if (po->turn == BLACK)
			po->bcr = NO_CR;
		else
			po->wcr = NO_CR;
	}

	/* pawn case */
	if (PAWN == po->board[mo.x1][mo.y1].fig) { 
		/* move foreward by 2 at start */
		if ((mo.x1 == mo.x2 + 2) || (mo.x1 + 2 == mo.x2)) {
			po->x = (mo.x1 + mo.x2) / 2;
			po->y = mo.y2;
			clear_special = false;
		}

		/* pawn take en passant */
		if ((mo.y1 != mo.y2) && (EMPTY == po->board[mo.x2][mo.y2].fig)) {
			int8_t dx = (WHITE == po->turn ? -1 : 1);
			po->board[po->x + dx][po->y].fig = EMPTY;
			po->board[po->x +dx ][po->y].col = WHITE;
		}
	}

	/* rook case */
	if (ROOK == po->board[mo.x1][mo.y1].fig) {
	       if ((mo.x1 == 0) && (mo.y1 == 0)) po->wcr &= ~BIG_CR;
	       if ((mo.x1 == 0) && (mo.y1 == 7)) po->wcr &= ~SMALL_CR;
	       if ((mo.x1 == 7) && (mo.y1 == 0)) po->bcr &= ~BIG_CR;
	       if ((mo.x1 == 7) && (mo.y1 == 7)) po->bcr &= ~SMALL_CR;
	}

	if (EMPTY == mo.promo)
		po->board[mo.x2][mo.y2].fig = po->board[mo.x1][mo.y1].fig;
	else 
		po->board[mo.x2][mo.y2].fig = mo.promo;

	po->board[mo.x2][mo.y2].col = po->board[mo.x1][mo.y1].col;
	po->board[mo.x1][mo.y1].fig = EMPTY;
	po->board[mo.x1][mo.y1].col = WHITE;

	if (clear_special) {
		po->x = -1; 
		po->y = -1; 
	}

	po->turn = BLACK - po->turn;
}

bool list_legal_moves(struct position *po, struct move *mo, uint8_t *cnt)
{
	struct position next;
	struct move mo2[200];
	uint8_t cnt2;
	int16_t i;
	bool res = false;

	res = list_moves(po, mo, cnt);

	if (true == res) {
		if (chess_debug_level >= DEBUG_WRONG) {
			printf("*********************  ERROR   *********************** !\n");
			synthesis(po, mo, *cnt);
		}
		return res;
	}

	for (i = 0; i < *cnt; i++) {

		memcpy(&next, po, sizeof(struct position));
		apply_move(&next, mo[i]);
		res = list_moves(&next, mo2, &cnt2);

		if (chess_debug_level >= DEBUG_NEXT) {

			debug_offset = true;
			synthesis(&next, mo2, cnt2);
			debug_offset = false;
		}

		if (*cnt > 0) {
			if (!res) {
				if (chess_debug_level >= DEBUG_MOVE) {
					printf("\tmove %3d / %3d: %s", (i + 1), *cnt, (po->turn == WHITE ? "" : "   .."));
					print_move(po, mo[i]);
					printf(" : keep");
					printf("\n");
				}
			} else {
				if (chess_debug_level >= DEBUG_MOVE) {
					printf("\tmove %3d / %3d: %s", (i + 1), *cnt, (po->turn == WHITE ? "" : "   .."));
					print_move(po, mo[i]);
					printf(" : REMOVE");
					printf("\n");
				}
				memmove(&mo[i], &mo[i + 1], sizeof(struct move) * ((*cnt) - i - 1));
				(*cnt)--;
				i--; /* in position of last operation of the loop ! */
			}
		}
	}

	if (0 == *cnt) {
		po->turn = BLACK - po->turn;
		res = list_moves(po, mo2, &cnt2);
		if (chess_debug_level >= DEBUG_MOVE)
			synthesis(po, mo, *cnt);
		po->turn = BLACK - po->turn;
	}

	return res;
}

uint8_t play_game(struct position *po)
{
	int r;
	uint8_t cnt, i;
	struct move mo[255];
	bool res;

	cnt = 0;
	res = 0;

	for (i = 0; i < 250; i++) {
		res = list_legal_moves(po, mo, &cnt);
		if (chess_debug_level >= DEBUG_MOVE)
			synthesis(po, mo, cnt);
		if (0 == cnt) {
			if (res) {
				if (chess_debug_level >= DEBUG_MAT)
					synthesis(po, mo, cnt);
					printf("\n******* CHECKMATE at move %d! **************\n\n", i);
			} else {
				if (chess_debug_level >= DEBUG_PAT)
					synthesis(po, mo, cnt);
					printf("\n******* DRAW at move %d!      **************\n\n", i);
			}
			//getchar();
			return i;
		}
		r = rand() % cnt;
		if (chess_debug_level >= DEBUG_MOVE) {
			printf("\nmove %d: %s", i, (po->turn == WHITE ? "" : "     .."));
			print_move(po, mo[r]);
			printf("\n");
		}
		apply_move(po, mo[r]);
	}

	if (chess_debug_level >= DEBUG_DRAW) {
		synthesis(po, mo, cnt);
		printf("\n******* NO WIN after move %d!      **************\n\n", i);
	}

	return i;
}

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
	{OO_, WP_, BP_, BP_, BP_, BP_, OO_, OO_},
	{OO_, OO_, BN_, OO_, BB_, BN_, OO_, OO_},
	{BP_, BP_, BQ_, OO_, OO_, BP_, BP_, BP_},
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
		c = c - 'A' + 'a'; 

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

static void print_square(int8_t li, int8_t co)
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
	printf(" ");
}
static inline struct move prepare_move(int8_t lin1, int8_t col1, int8_t lin2, int8_t col2)
{
	struct move mo;

	mo.lin1 = lin1;
	mo.col1 = col1;
	mo.lin2 = lin2;
	mo.col2 = col2;

	return mo;
}

void set_move(uint16_t *moves, uint64_t *cnt, int8_t lin1, int8_t col1, int8_t lin2, int8_t col2)
{
	struct move mo = prepare_move(lin1, col1, lin2, col2);
	uint16_t move = *((uint16_t *)(&mo));
	moves[*cnt++] = move;
}

struct move get_move(uint16_t *moves, uint64_t cnt)
{
	return *((struct move *)(moves + cnt));
}

static bool pawn_moves(struct position *po, int8_t li, int8_t co, uint16_t *mo, uint64_t *cnt)
{
	struct piece pi, take;
	int8_t dx;

	pi = get_piece(po, li, co);

	if (pi.fig != PAWN)
		return false;

	if (BLACK == pi.col)
		dx = -1;
	else
		dx = 1;

	if (get_piece(po, li + dx, co) == 0) {
		set_move(mo, cnt, li, co, li + dx, co);
		if ((1 == dx) && (li > 1) || ((-1 == dx) && (li < 6))) {
			if (get_piece(po, li + 2 * dx, co) == 0) 
				set_move(mo, cnt++, li, co, li + 2 * dx, co);
		}
	}

	if (co > 0) {
		dest = SQUARE(SQI(square) + dx, SQJ(square) - 1);
		take = get_piece(po, li + dx, co - 1);
		if (((EMPTY != take.fig) && (pi.col != take.col)) || (dest == gm->en_passant)) {
			if (FIG(take) == KING)
				return true;
			set_move(gm->moves, gm->move_cnt++, prepare_move_square(square, dest));
		}
	}

	if (co < 7) {
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



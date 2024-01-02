#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "move.h"

#define NN 1
#define NE 2
#define EE 3
#define SE 4
#define SS 5
#define SW 6
#define WW 7
#define NW 8


#define ABS(_X) ((_X) > 0 ? (_X) : -(_X))

/* special set of macros to get DX and DY from DIR */

#define PHASE(_X) (ABS(((_X) % 8) - 4) - 2)

#define DELTA(_X) (PHASE((_X)) - PHASE((_X)) / 2)

#define D_X(_DIR) (DELTA((_DIR) + 7))

#define D_Y(_DIR) (DELTA((_DIR) + 5))

#define FOR_DIR(_DIR, _DX, _DY) for (                         \
		_DIR = NN, _DX = D_X(_DIR), _DY = D_Y(_DIR) ; \
		_DIR <= NW ;                                  \
		_DIR++, _DX = D_X(_DIR), _DY = D_Y(_DIR)      \
		)

uint16_t get_move(uint32_t *moves, uint16_t pos)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t pair = *((uint32_t *)(my_moves + 3 * (pos / 2)));
	uint8_t offset = 12 * (pos % 2);

	return (uint16_t) ((pair >> offset) & 0x0fff);
}

void set_move(uint32_t *moves, uint16_t pos, uint16_t move)
{
	uint8_t  *my_moves = (uint8_t *)moves;
	uint32_t *pair = (uint32_t *)(my_moves + 3 * (pos / 2));
	uint32_t  this_move = (uint32_t)move;
	uint8_t   offset = 12 * (pos % 2);

	*pair &= ~(0x0fff << offset);
	*pair |= this_move << offset;
}

static int8_t aligned(uint8_t start, uint8_t stop)
{
	uint8_t x1 = SQI(start);
	uint8_t y1 = SQJ(start);
	uint8_t x2 = SQI(stop);
	uint8_t y2 = SQJ(stop);
	uint8_t al;

	if ((x1 <  x2) && (y1 == y2)                        ) return NN;
	if ((x1 <  x2) && (y1 <  y2) && (x2 - x1 == y2 - y1)) return NE;
	if ((x1 == x2) && (y1 <  y2)                        ) return EE;
	if ((x1 >  x2) && (y1 <  y2) && (x1 - x2 == y2 - y1)) return SE;
	if ((x1 >  x2) && (y1 == y2)                        ) return SS;
	if ((x1 >  x2) && (y1 >  y2) && (x1 - x2 == y1 - y2)) return SW;
	if ((x1 == x2) && (y1 >  y2)                        ) return WW;
	if ((x1 <  x2) && (y1 >  y2) && (x2 - x1 == y1 - y2)) return NW;

	return 0;
}

bool pinned(uint8_t board[32], uint8_t figs_square)
{
	uint8_t square, kings_square;
	uint8_t fig, figure;
	int8_t al, dx, dy;

	figure = get_piece(board, figs_square);
	
	kings_square = get_square(board, COL(figure) | KING);

	al = aligned(kings_square, figs_square);

	/*
	printf("%c %c%d alignement with its king in %c%d is %d\n",
	       to_char(figure),'a' + SQJ(figs_square), 1 + SQI(figs_square),
	       'a' + SQJ(kings_square), 1 + SQI(kings_square),
	       al);
	*/
	if (!al)
		return false; /* figure and king are not aligned */

	dx = D_X(al);
	dy = D_Y(al);

	/*
	** From now on, we know that there is an
	** alignement and we know its direction.
	** So let's heck if there is a piece between the
	** figure and the king along this direction.
	** */
	square = kings_square;
	do {
		square = SQUARE(SQI(square) + dx, SQJ(square) + dy);
		fig = get_piece(board, square);
		//printf("before king in %c%d found %c\n", 'a' + SQJ(square), 1 + SQI(square), to_char(fig));

		if ((fig != 0) && (fig != figure))
			return false;

	} while (square != figs_square);
	
	/*
	** We have reached the king without crossing any other piece.
	** So let's check if we have reached an edge
	** */
	if (((dx == -1) && (SQI(square) == 0)) ||
	    ((dx ==  1) && (SQI(square) == 7)) ||
	    ((dy == -1) && (SQJ(square) == 0)) ||
	    ((dy ==  1) && (SQJ(square) == 7)))
		return false;

	do {
		/*
		** Thanks to the previous check we know that
		** there is at least one more square to examine
		** */
		square = SQUARE(SQI(square) + dx, SQJ(square) + dy);
		fig = get_piece(board, square);
		//printf("after king in %c%d found %c\n", 'a' + SQJ(square), 1 + SQI(square), to_char(fig));
		/* There is someone here */
		if (fig != 0) {

			/* It is a friend: no pinning */
			if (COL(fig) == COL(figure))
				return false;

			/* It is a foe on diagonals: pinning */
			if (((NE == al) || (SW == al) || (NW == al) || (SE == al)) &&
			    ((FIG(fig) == BISHOP) || (FIG(fig) == QUEEN)))
				return true;

			/* It is a foe on rows or columns: pinning */
			if (((NN == al) || (SS == al) || (WW == al) || (EE == al)) &&
			    ((FIG(fig) == ROOK) || (FIG(fig) == QUEEN)))
				return true;

			/* It is a non threatening foe: no pinning */
			return false;
		}

		/*
		** We have reached an edge and we know that the square is empty
		** */
		if (((dx == -1) && (SQI(square) == 0)) ||
		    ((dx ==  1) && (SQI(square) == 7)) ||
		    ((dy == -1) && (SQJ(square) == 0)) ||
		    ((dy ==  1) && (SQJ(square) == 7)))
			return false;

	} while (1); /* It is safe to do so */
}

uint8_t *pawn_moves(struct game *gm, uint8_t square, uint8_t *count)
{
	uint32_t res[1];
	uint8_t figure, color;
	int8_t dx;

	*count = 0;

	figure = get_piece(board, square);

	if (figure & (~BLACK) != PAWN)
		return res;

	color = figure & BLACK;

	if ((color >> 3) != gm->turn)
		return res;

	if (color)
		dx = -1;
	else
		dx = 1;

	if (get_piece(gm->board, SQUARE(SQI(square + dx), SQJ(square))) == 0)
		set_move(res, 0, uint16_t move);
}

bool is_legal(struct game* fight, uint16_t move)
{
	return true;
}

#undef NN
#undef SS
#undef EE
#undef WW
#undef NE
#undef SO
#undef NO
#undef SE

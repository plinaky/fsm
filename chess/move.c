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

void print_square(uint8_t square)
{
	printf("%c%d",
	       'a' + (char)((square >> 0) & 0x7),
	        1  + ((square >> 3) & 0x7)
	       );
}

void print_move(uint16_t mov)
{
	print_square((uint8_t)mov);
	printf("-");
	print_square((uint8_t)(mov >> 6));
}


uint16_t get_move(uint32_t *moves, uint16_t pos)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t pair = *((uint32_t *)(my_moves + 3 * (pos / 2)));
	uint8_t offset = 12 * (pos % 2);

	return (uint16_t) ((pair >> offset) & 0xffffff);
}

static inline uint16_t prepare_move_xy(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	uint16_t mov = 
		(((uint16_t)x2 & 0x7) << 9) |
		(((uint16_t)y2 & 0x7) << 6) |
		(((uint16_t)x1 & 0x7) << 3) |
		(((uint16_t)y1 & 0x7) << 0);
	return mov;
}

static inline uint16_t prepare_move_square(uint8_t start, uint8_t end)
{
	uint16_t mov = 
		(((uint16_t)end   & 0x3f) << 6) |
		(((uint16_t)start & 0x3f) << 0);
	return mov;

}

void set_move(uint32_t *moves, uint16_t pos, uint16_t move)
{
	uint8_t  *my_moves = (uint8_t *)moves;
	uint32_t *pair = (uint32_t *)(my_moves + 3 * (pos / 2));
	uint32_t  this_move = (uint32_t)move;
	uint8_t   offset = 12 * (pos % 2);

	*pair &= ~(0xffffff << offset);
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

void pawn_moves(struct game *gm, uint8_t square, uint8_t *count)
{
	uint8_t figure, color, dest, take;
	int8_t dx;

	*count = 0;

	figure = get_piece(gm->board, square);

	if (FIG(figure) != PAWN)
		return;

	color = COL(figure);

	if ((color >> 3) != gm->turn)
		return;

	if (color)
		dx = -1;
	else
		dx = 1;

	dest = SQUARE(SQI(square) + dx, SQJ(square));

	if (get_piece(gm->board, dest) == 0) {
		set_move(gm->moves, (*count)++, prepare_move_square(square, dest));

		if ((1 == dx) && (1 == SQI(square)) ||
			((-1 == dx) && (6 == SQI(square)))) {
			dest = SQUARE(SQI(square) + 2 * dx, SQJ(square));
			if (get_piece(gm->board, dest) == 0) 
				set_move(gm->moves, (*count)++, prepare_move_square(square, dest));
		}
	}

	if (SQJ(square) > 0) {
		dest = SQUARE(SQI(square) + dx, SQJ(square) - 1);
		take = get_piece(gm->board, dest);
		if ((0 != take) && (COL(take) != color)) 
			set_move(gm->moves, (*count)++, prepare_move_square(square, dest));
	}

	if (SQJ(square) < 7) {
		dest = SQUARE(SQI(square) + dx, SQJ(square) + 1);
		take = get_piece(gm->board, dest);
		if ((0 != take) && (COL(take) != color)) 
			set_move(gm->moves, (*count)++, prepare_move_square(square, dest));
	}
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

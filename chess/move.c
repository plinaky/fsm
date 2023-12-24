#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "move.h"

uint16_t get_move(uint32_t *moves, uint16_t pos)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t pair = *((uint32_t *)(my_moves + 3 * (pos / 2)));
	uint8_t offset = 12 * (pos % 2);

	return (uint16_t) ((pair >> ) & 0x0fff);
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

#define NN 1
#define NE 2
#define EE 3
#define SE 4
#define SS 5
#define SW 6
#define WW 7
#define NW 8

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
	
	if (COL(figure) == BLACK)
		kings_square = get_square(board, BK);
	else
		kings_square = get_square(board, WK);

	al = aligned(kings_square, figs_square);

	/*
	printf("%c %c%d alignement with its king in %c%d is %d\n",
	       to_char(figure),'a' + SQJ(figs_square), 1 + SQI(figs_square),
	       'a' + SQJ(kings_square), 1 + SQI(kings_square),
	       al);
	*/
	switch (al) {
	case 0  : return false; /* figure and king are not aligned */
	case NN : dx =  1; dy =  0; break;
	case NE : dx =  1; dy =  1; break;
	case EE : dx =  0; dy =  1; break;
	case SE : dx = -1; dy =  1; break;
	case SS : dx = -1; dy =  0; break;
	case SW : dx = -1; dy = -1; break;
	case WW : dx =  0; dy = -1; break;
	case NW : dx =  1; dy = -1; break;
	}

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

#define (_DIR, _DX, _DY) for (_DIR = NN; _DIR <= NW; )
#define FOR_DIR(_DIR, _DX, _DY) for (_DIR = NN; _DIR <= NW; )
static bool check(struct game*)
{
	uint8_t square, kings_square;
	uint8_t fig, figure;
	int8_t al, dx, dy;

	figure = get_piece(board, figs_square);
	
	if (game->turn)
		kings_square = get_square(board, BK);
	else
		kings_square = get_square(board, WK);

	al = aligned(kings_square, figs_square);

	/*
	printf("%c %c%d alignement with its king in %c%d is %d\n",
	       to_char(figure),'a' + SQJ(figs_square), 1 + SQI(figs_square),
	       'a' + SQJ(kings_square), 1 + SQI(kings_square),
	       al);
	*/
	switch (al) {
	case 0  : return false; /* figure and king are not aligned */
	case NN : dx =  1; dy =  0; break;
	case NE : dx =  1; dy =  1; break;
	case EE : dx =  0; dy =  1; break;
	case SE : dx = -1; dy =  1; break;
	case SS : dx = -1; dy =  0; break;
	case SW : dx = -1; dy = -1; break;
	case WW : dx =  0; dy = -1; break;
	case NW : dx =  1; dy = -1; break;
	}

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

bool is_legal(struct game*, uint16_t move)
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

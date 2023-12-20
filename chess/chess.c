#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "move.h"

#define NN 1;
#define NE 2;
#define EE 3;
#define SE 4;
#define SS 5;
#define SW 6;
#define WW 7;
#define NW 8;


int8_t aligned(uint8_t start, uint8_t stop)
{
	uint8_t x1 = SQI(start);
	uint8_t y1 = SQJ(start);
	uint8_t x2 = SQI(stop);
	uint8_t y2 = SQJ(stop);
	uint8_t al;

	if ((x1 <  x2) && (y1 == y2)                        ) return NN;
	if ((x1 <  x2) && (y1 <  y2) && (x1 - x2 == y2 - y1)) return NE;
	if ((x1 == x2) && (y1 <  y2)                        ) return EE;
	if ((x1 >  x2) && (y1 <  y2) && (x1 - x2 == y2 - y1)) return SE;
	if ((x1 >  x2) && (y1 == y2)                        ) return SS;
	if ((x1 >  x2) && (y1 >  y2) && (x1 - x2 == y1 - y2)) return SW;
	if ((x1 == x2) && (y1 >  y2)                        ) return WW;
	if ((x1 <  x2) && (y1 >  y2) && (x2 - x1 == y1 - y2)) return NW;

	return 0;
}

bool pinned(uint8_t board[32], uint8_t square)
{
	uint8_t col, kings_square, king;
	int8_t al, dx, dy;

	col = COL(get_piece(board, square));
	
	if (col)
		king = BK;
	else
		king = WK;

	kings_square = get_square(board, king);

	al = aligned(square, kings_square);

	switch al {
	case 0  : return false;
	case NN : dx =  1; dy =  0; break;
	case SS : dx = -1; dy =  0; break;
	case EE : dx =  0; dy =  1; break;
	case OO : dx =  0; dy = -1; break;
	case NE : dx =  1; dy =  1; break;
	case SO : dx = -1; dy = -1; break;
	case NO : dx =  1; dy =  1; break;
	case SE : dx =  -1; dy =  1; break;
	}

	do {
		square = SQUARE(SQI(square) + dx, SQI(square) + dy); 
		fig = get_piece(board, square);
		if ((fig != 0) && (fig != king))
			return false;
	} while (square != kings_square) 


}

#undef NN
#undef SS
#undef EE
#undef WW
#undef NE
#undef SO
#undef NO
#undef SE



int main(void)
{
	uint8_t board[32];

	for (uint8_t i = 0; i < 8; i++)
		for (uint8_t j = 0; j < 8; j++)
			set_piece(board, i, j, default_board[i][j]);

	print_pos(board);

	return 0;


#ifndef RULES_H
#define RULES_H

enum figure {
	KNIGHT = 1,
	BISHOP = 2,
	ROOK   = 3,
	QUEEN  = 4,
	PAWN   = 5,
	KING   = 6,
} __attribute__((packed));

enum color {
	WHITE = 0,
	BLACK = 1
} __attribute__((packed));

struct piece {
	enum color col;
	enum figure fig;
} __attribute__((packed));

struct square {
	int8_t lin : 3;
	int8_t col : 3;
} __attribute__((packed));

struct move {
	struct square start;
	struct square stop;
	uint8_t promote : 2;
} __attribute__((packed));


enum pieces {
	WN =  1, WB =  2, WR =  3, WQ =  4, WP =  5, WK =  6,
	BN =  9, BB = 10, BR = 11, BQ = 12, BP = 13, BK = 14
} __attribute__((packed));


#define SQUARE(_i, _j) ((uint8_t)(((uint8_t)(_i) << 3  | (uint8_t)(_j)) & 0x3f))

#define FIG(_fig) ((uint8_t)((_fig) & 0x7))
#define COL(_fig) ((uint8_t)(((_fig) & 0x8) >> 3))

#define GET_PIECE(_board, _i, _j) get_piece(_board, SQUARE((_i), (_j)))
#define SET_PIECE(_board, _i, _j, _fig) set_piece(_board, SQUARE((_i), (_j)), (_fig))

extern uint8_t default_board[8][8];

char to_char(uint8_t fig);

uint8_t get_piece(uint8_t board[32], uint8_t square);
void set_piece(uint8_t board[32], uint8_t square, uint8_t fig);

void print_pos(uint8_t board[32]);

#endif

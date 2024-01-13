#ifndef RULES_H
#define RULES_H

#include <stdbool.h>

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


extern const struct piece WN;
extern const struct piece WB;
extern const struct piece WR;
extern const struct piece WQ;
extern const struct piece WK;

extern const struct piece BN;
extern const struct piece BB;
extern const struct piece BR;
extern const struct piece BQ;
extern const struct piece BK;

struct position {
    uint32_t board[8];
    uint8_t  castle     : 4;
    uint8_t  en_passant : 3;  /* column */
    uint8_t  king       : 2;  /* 1: latest is OO, 2: OOO */
    uint8_t  turn       : 1;  /* 0: white, 1 black */
} __attribute__((packed));

extern struct piece default_board[8][8];

char to_char(const struct piece pi);

struct piece get_piece(struct position *po, struct square sq);
void set_piece(struct position *po, struct square sq, struct piece pi);

void print_pos(struct position *po);

bool list_moves(struct position *po);
void print_move(struct move mo);

#endif

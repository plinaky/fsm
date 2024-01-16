#ifndef RULES_H
#define RULES_H

#include <stdbool.h>
#include <stdint.h>

#define OO_ {WHITE, EMPTY}

#define WN_ {WHITE, KNIGHT}
#define WB_ {WHITE, BISHOP}
#define WR_ {WHITE, ROOK}
#define WQ_ {WHITE, QUEEN}
#define WP_ {WHITE, PAWN}
#define WK_ {WHITE, KING}

#define BN_ {BLACK, KNIGHT}
#define BB_ {BLACK, BISHOP}
#define BR_ {BLACK, ROOK}
#define BQ_ {BLACK, QUEEN}
#define BP_ {BLACK, PAWN}
#define BK_ {BLACK, KING}


#pragma pack(push, 1)

enum figure {
	EMPTY  = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK   = 3,
	QUEEN  = 4,
	PAWN   = 5,
	KING   = 6
};

enum color {
	WHITE = 0,
	BLACK = 1
};

enum castling_rights {
	NO_CR       = 0,
	SMALL_CR    = 1,
	BIG_CR      = 2,
	ALL_CR      = 3
};

struct piece {
	enum color col  : 1;
	enum figure fig : 3;
};

struct square {
    int8_t x, y;
};

struct move {
    struct square sq[2];
    struct piece pi[2];
};

struct position {
    struct piece board[8][8];
    struct square special;
    enum color turn          : 1;
    enum castling_rights wcr : 2;
    enum castling_rights bcr : 2;
};

#pragma pack(pop)
char to_char(const struct piece pi);
void print_moves(struct move *mo, uint16_t cnt, struct position *po);
void synthesis(struct move *mo, uint16_t cnt, struct position *po);
void apply_move(struct position *po, struct move mo);
bool list_legal_moves(struct position *po, struct move *mo, uint16_t *cnt);
uint16_t play_game(struct position *po);
#endif

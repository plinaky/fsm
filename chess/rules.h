#ifndef RULES_H
#define RULES_H

#include <stdbool.h>

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

enum figure {
	EMPTY  = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK   = 3,
	QUEEN  = 4,
	PAWN   = 5,
	KING   = 6
} __attribute__((packed));

enum color {
	WHITE = 0,
	BLACK = 1
} __attribute__((packed));

struct piece {
	enum color col  : 1;
	enum figure fig : 3;
} __attribute__((packed));

struct move {
	uint8_t lin1 : 3;
	uint8_t col1 : 3;
	uint8_t lin2 : 3;
	uint8_t col2 : 3;
	enum color promo_col  : 1;
	enum figure promo_fig : 3;
} __attribute__((packed));

struct position {
    uint8_t board[32];
    uint8_t B_OO_1     : 1;
    uint8_t B_OOO_1    : 1;
    uint8_t W_OO_1     : 1;
    uint8_t W_OOO_1    : 1;
    uint8_t B_OO_2     : 1;
    uint8_t B_OOO_2    : 1;
    uint8_t W_OO_2     : 1;
    uint8_t W_OOO_2    : 1;
    uint8_t a_passe    : 1;  /* elle a passé la jeune fille */
    uint8_t en_passant : 3;  /* column */
    enum color turn    : 1;  /* 0: white, 1 black */
} __attribute__((packed));

extern struct piece default_board[8][8];

char to_char(const struct piece pi);
struct piece get_piece(struct position *po, int8_t li, int8_t co);
void set_piece(struct position *po, int8_t li, int8_t co, struct piece pi);
void print_pos(struct position *po);
bool list_moves(struct position *po, struct move *mo, uint64_t *cnt);
bool list_legal_moves(struct position *po, struct move *mo, uint64_t *cnt);

#endif

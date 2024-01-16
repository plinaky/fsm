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

#define _De_(_STR_, _SZ_) enum _STR_##_e {_STR_##_MAX_VAL = 1 << (_SZ_ - 1)} __attribute__((packed))
#define _Ue_(_STR_, _NAME_, _SZ_) enum _STR_##_e _NAME_ : _SZ_
#define _Fi_(_STR_, _NAME_, _TY_) *((struct _TY_ *)(&(_STR_._NAME_)))

struct square {
    uint8_t x : 3;
    uint8_t y : 3;
} __attribute__((packed));
_De_(square, 6);

struct piece2 {
	enum color col  : 1;
	enum figure fig : 3;
} __attribute__((packed));
_De_(piece2, 4);

struct move2 {
    _Ue_(square, start, 6);
    _Ue_(square, stop,  6);
    _Ue_(piece2, promo, 4);
} __attribute__((packed));

struct castle_right {
    uint8_t OO  : 1;
    uint8_t OOO : 1;
}__attribute__((packed));
_De_(castle_right, 2);

struct position2 {
    struct piece2 board[8][8];
    enum color turn    : 1;  /* 0: white, 1 black */
    _Ue_(castle_right, crw, 2);
    _Ue_(castle_right, crb, 2);
    _Ue_(square, special, 6);
} __attribute__((packed));

void set_piece(struct position *po, uint8_t li, uint8_t co, struct piece pi);
struct piece get_piece(struct position *po, uint8_t li, uint8_t co);
char to_char(const struct piece pi);
void print_moves(struct move *mo, uint16_t cnt, struct position *po);
void synthesis(struct move *mo, uint16_t cnt, struct position *po);
void apply_move(struct position *po, struct move mo);
bool list_legal_moves(struct position *po, struct move *mo, uint16_t *cnt);
uint16_t play_game(struct position *po);

#endif

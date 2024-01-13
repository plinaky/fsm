#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

struct position {
    uint32_t board[8];
    uint8_t  castle     : 4;
    uint8_t  en_passant : 3;  /* column */
    uint8_t  king       : 2;  /* 1: latest is OO, 2: OOO */
    uint8_t  turn       : 1;  /* 0: white, 1 black */
} __attribute__((packed));

enum piece { /* keep this order for promotion */
    QUEEN   = 1,
    ROOK    = 2,
    BISHOP  = 3,
    KNIGHT  = 4,
    PAWN    = 5,
    KING    = 6
} __attribute__((packed));

enum color {
    WHITE   = 0,
    BLACK   = 1
} __attribute__((packed));


struct move {
    uint16_t col1 : 3;
    uint16_t lin1 : 3;
    uint16_t col2 : 3;
    uint16_t lin2 : 3;
    uint16_t prom : 2;
} __attribute__((packed));

struct node {
    int64_t  score;
    int64_t  visit;
    struct node *side;
    struct node *down;
};


/*
 *  1 coordinate fits in 3 bits
 *  2 coordinates fit in 3 bits
 *  1 move fits in 12 bits
 *  2 moves fit in 24 bits
 *  2 moves fit in  3 uint8_t
 *  4 moves fit in  3 uint16_t
 *  8 moves fit in  3 uint32_t
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |               2               |               1               |               0               |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |                    MOVE 1                     |                    MOVE 0                     |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |         END 1         |       START 1         |         END 0         |       START 0         |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |    xe1    |    ye1    |    xs1    |    ys1    |    xe0    |    ye0    |    xs0    |    ys0    |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * */

uint16_t get_move(uint32_t *moves, uint8_t pos);
void set_move(uint32_t *moves, uint8_t pos, uint16_t move);
bool list_moves(struct game *gm);
void print_move(uint16_t mov);

#endif

#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

}
struct game {
    uint32_t *moves;
    uint32_t board[8];
    uint16_t move_cnt;
    uint8_t  checks[3];
    uint8_t  castle : 4;
    uint8_t  en_passant : 4;
    uint8_t  check_status : 2;
    uint8_t  turn : 1;
    uint8_t  mate : 1;
    uint8_t  null : 1;
} __attribute__((packed));


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
 * |    ye1    |    xe1    |    ys1    |    xs1    |    ye0    |    xe0    |    ys0    |    xs0    |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * */

uint16_t get_move(uint32_t *moves, uint16_t pos);
void set_move(uint32_t *moves, uint16_t pos, uint16_t move);
bool pinned(uint8_t board[32], uint8_t figs_square);
bool is_legal(struct game*, uint16_t move);

#endif

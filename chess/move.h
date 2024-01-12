#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

/* we use a fixed but small (8) number of moves
 * and store several structures of this kind
 * for actually longer games.
 *
 * The idea is to work with game chunks that combine
 * a position and the best known sequence of moves behind
 *
 * right now fits in : 12 uint32_t 
 * */
struct chunk {
    uint32_t  board[8];
    uint32_t  moves[3];
    uint32_t  chunk_cnt  : 19; /* up to 524288 chunks */
    uint32_t  move_cnt   : 3;  /* of 8 moves */
    uint32_t  castle     : 4;
    uint32_t  king       : 2;  /* 1: latest is OO, 2: OOO */
    uint32_t  en_passant : 3;  /* column */
    uint32_t  turn       : 1;
} __attribute__((packed));

struct move_list {
    uint32_t moves[96]; /* up to 256 moves */
    uint8_t  board1[32];
    uint8_t  board2[32];
    uint8_t  move_cnt;
    struct   game *gm;
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

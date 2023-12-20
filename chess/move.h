#ifndef MOVE_H
#define MOVE_H

struct game {
    uint8_t board[32];
    uint8_t *moves;
    uint8_t len;
};

/* 8 moves fit in 3 uint32_t */

uint32_t get_move(uint32_t *moves, uint64_t pos);
void set_move(uint32_t *moves, uint64_t pos, uint32_t move);

#endif

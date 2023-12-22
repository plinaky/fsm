#ifndef MOVE_H
#define MOVE_H

struct game {
    uint8_t board[32];
    uint8_t turn : 1;
    uint8_t castle : 4;
    uint8_t en_passant : 4;
    uint8_t half_moves;
    uint8_t full_moves;
    uint8_t *moves;
    uint8_t len;
};

/* 8 moves fit in 3 uint32_t */

uint32_t get_move(uint32_t *moves, uint64_t pos);
void set_move(uint32_t *moves, uint64_t pos, uint32_t move);

#endif

#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include <stdint.h>

bool pawn_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool knight_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool brq_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool king_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);

#endif

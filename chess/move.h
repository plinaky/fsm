#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include <stdint.h>

bool pawn_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool knight_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool brq_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);
bool king_moves(struct board *bo, uint8_t x, uint8_t y, uint16_t *ml, uint8_t *cnt);

void print_move(uint16_t mo);
void print_moves(uint16_t *ml, uint8_t cnt);
bool list_moves(struct board *bo, uint16_t *ml, uint8_t *cnt);
void apply_move(struct board *bo, uint16_t mo);
bool list_legal_moves(struct board *bo, uint16_t *ml, uint8_t *cnt);

#endif

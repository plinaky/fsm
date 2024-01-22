#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <stdint.h>

bool pawn_moves(struct board *bo, uint8_t x, uint8_t y, struct move *ml, uint8_t *cnt);

#endif

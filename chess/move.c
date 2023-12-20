#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "move.h"

uint32_t get_move(uint32_t *moves, uint64_t pos)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t pair = *((uint32_t *)(my_moves + 3 * (pos / 2)));

	return (pair >> 12 * (pos % 2)) & 0x0fff;
}

void set_move(uint32_t *moves, uint64_t pos, uint32_t move)
{
	uint8_t *my_moves = (uint8_t *)moves;
	uint32_t *pair = (uint32_t *)(my_moves + 3 * (pos / 2));

	*pair &= ~(0x0fff << (12 * (pos % 2)));
	*pair |= move << (12 * (pos % 2));
}

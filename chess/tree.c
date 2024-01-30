#include "standards.h"
#include "mapper.h"
#include "board.h"
#include "compare.h"

uint8_t store_pos(struct board *bmap, struct board *bo, uint16_t pos)
{
	uint16_t i;
	uint8_t res = 0;

	for (i = 0; i < pos; i++)
		if (compare(bmap + i, bo))
			res++;

	memcpy(bmap + i, bo, sizeof(struct board));

	res++;

	return res;
}

uint32_t store_board(struct board *bmap, struct board *bo, uint16_t pos)
{
	uint32_t i;

	for (i = 0; i < pos; i++)
		if (compare(bmap + i, bo))
			return i;

	memcpy(bmap + i, bo, sizeof(struct board));

	return i;
}


#include "mapper.h"
#include "compare.h"

uint8_t store_pos(void *map, struct board *b, uint16_t pos)
{
	struct board *cur = (struct board *)map;
	uint16_t i;
	uint8_t res = 0;

	for (i = 0; i < pos; i++) {
		if (compare(cur + i, b))
			res++;
	}

	res++;
	memcpy(cur + i, b, sizeof(struct board));

	return res;
}


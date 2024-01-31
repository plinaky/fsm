#include "standards.h"
#include "board.h"
#include "mapper.h"
#include "board.h"
#include "compare.h"

#define MAX_BOARD (UINT32_MAX >> 4)
#define MAX_LINK  (UINT32_MAX >> 4)
static uint32_t board_map_size;
static uint32_t  link_map_size;
static struct board *board_map;
static struct link   *link_map;

int init_maps(void)
{
	board_map = (struct board *)create_map(MAX_BOARD * sizeof(struct board));
	link_map  = ( struct link *)create_map(MAX_LINK  * sizeof(struct link ));
	board_map_size = 0;
	link_map_size  = 0;
}

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


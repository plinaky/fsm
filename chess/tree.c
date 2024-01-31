#include "standards.h"
#include "board.h"
#include "mapper.h"
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

uint8_t add_move(struct board *bo, uint16_t move)
{
	struct link lk;

	lk.start = store_board(void *map, struct board *b, uint32_t pos)
}

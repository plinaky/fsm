#include "standards.h"
#include "board.h"
#include "mapper.h"
#include "tree.h"
#include "compare.h"

#define MAX_BOARD (UINT32_MAX >> 4)
#define MAX_LINK  (UINT32_MAX >> 4)

struct board *board_map;
struct link   *link_map;

static const char board_map_file[] = "./data/boards";
static const char link_map_file[]  = "./data/links";


int open_game_maps(void)
{
	static bool init_done = false;
	off_t size;

	if (init_done)
		return EXIT_SUCCESS;

	size = MAX_BOARD * sizeof(struct board);
	board_map = (struct board *)open_map(board_map_file, size);

	if (NULL == board_map)
		board_map = (struct board *)create_map(size);

	if (NULL == board_map)
		return EXIT_FAILURE;

	size = MAX_LINK * sizeof(struct link);
	link_map = (struct link *)open_map(link_map_file, size);

	if (NULL == link_map)
		link_map  = ( struct link *)create_map(size);

	if (NULL == link_map) {
		delete_map(board_map, MAX_BOARD * sizeof(struct board));
		return EXIT_FAILURE;
	}

	struct board_head *bh = (struct board_head *)board_map;
	struct link_head  *lh = (struct link_head  *) link_map;

	bh->mapped_size  = MAX_BOARD;
	bh->current_size =         1;
	lh->mapped_size  =  MAX_LINK;
	lh->current_size =         1;

	init_done = true; 

	return EXIT_SUCCESS;

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

uint32_t store_board(struct board *bo)
{
	uint32_t i, pos;

	struct board_head *bh = (struct board_head *)board_map;

	for (i = 0; i < bh->current_size; i++)
		if (compare(board_map + i, bo))
			return i;

	if (MAX_BOARD == i)
		return 0;

	bh->current_size++;
	memcpy(board_map + i, bo, sizeof(struct board));

	return i;
}

uint32_t store_link(uint32_t start, uint32_t stop, uint16_t move)
{
	uint32_t i, pos;

	struct link_head *lh = (struct link_head *)link_map;

	for (i = 0; i < lh->current_size; i++) {
		if ((link_map[i].start == start) && (link_map[i].stop == stop))
			return i;
	}

	if (MAX_LINK == i)
		return 0;

	lh->current_size++;
	link_map[i].start = start;
	link_map[i].stop  = stop;
	link_map[i].win   = 0;
	link_map[i].loss  = 0;
	link_map[i].view  = 0;
	link_map[i].cost  = 0;
	link_map[i].move  = move;

	return i;
}

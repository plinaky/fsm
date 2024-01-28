#include "mapper.h"
#include "tree.h"

void create_root(struct node *map, uint32_t size)
{
	uint32_t *max_size = (uint32_t *)(&(map[0].u));
	uint32_t *cur_size = (uint32_t *)(&(map[0].r));

	map[0].views = 1;
	map[0].cost  = 0;
	map[0].win   = 0;

	*max_size = size;
	*cur_size = 1;
	init_board((&map[0].b), pref);
	map[0].d = NULL;
	map[0].l = NULL;
}

uint32_t store_pos(struct node *map, struct board *b, uint16_t move)
{
	uint32_t *max_size = (uint32_t *)(&(map[0].u));
	uint32_t *cur_size = (uint32_t *)(&(map[0].r));

	if ((*cur_size) < (*max_size)) {
		map[*cur_size].views = 1;
		map[*cur_size].cost  = 0;
		map[*cur_size].win   = 0;
		memcpy(&map[*cur_size].b, b, sizeof(struct board));
		map[*cur_size].move  = move;
		(*cur_size)++;
		return *cur_size;
	} else {
		return 0;
	}

}


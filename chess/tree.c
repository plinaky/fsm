#include "standards.h"
#include "board.h"
#include "mapper.h"
#include "tree.h"
#include "move.h"
#include "compare.h"

//#define MAX_LINK (UINT32_MAX / (100 * sizeof(struct link)))
#define MAX_LINK (1 << 30)

struct link *link_map = NULL;

static const char link_map_file[]  = "./data/links";

int open_game_map(void)
{
	static bool init_done = false;
	off_t size;

	if (init_done)
		return EXIT_SUCCESS;

	size = MAX_LINK * sizeof(struct link);
	link_map = (struct link *)open_map(link_map_file, size);

	if (NULL == link_map) {
		printf("failed to open game map\n");
		exit(1);
	}

	struct link_head *lh = (struct link_head *)link_map;

	lh->mapped_size  = MAX_LINK;
	lh->current_size = 1;

	init_done = true; 

	return EXIT_SUCCESS;
}

int flush_game_map(void)
{
	struct link_head *lh = (struct link_head *)link_map;

	return flush_map(link_map, lh->current_size * sizeof(struct link));
}

uint8_t store_pos(struct board *bmap, struct board *bo)
{
	uint16_t i;
	uint8_t res = 1;
	
	struct board_head *bh = (struct board_head *)bmap;

	if (0 == bh->current_size)
		bh->current_size = 2;

	for (i = 1; i < bh->current_size; i++)
		if (compare(bmap + i, bo))
			res++;

	memcpy(bmap + i, bo, sizeof(struct board));

	return res;
}

uint32_t store_link(uint32_t up, uint16_t mo)
{
	if (NULL == link_map)
		open_game_map();

	struct link_head *lh = (struct link_head *)link_map;

	uint32_t po, pv;

	if (MAX_LINK == po) {
		printf("no memory left to create move ");
		print_move(mo);
		printf("\n");
		flush_game_map();
		exit(1);
	}

	for (po = 1; po < lh->current_size; po++) {
		if ((up == link_map[po].up) && (mo == link_map[po].mo)) {
			printf("move ");
			print_move(mo);
			printf(" already exists\n");
			return po;
		}
	}

	lh->current_size++;
	memset(link_map + po, 0, sizeof(link));
	link_map[po].up = up;
	link_map[po].mo = mo;


	if (0 == link_map[up].dn) {
		link_map[up].dn = po;
	} else {
		pv = link_map[up].dn;

		while (link_map[pv].le != 0)
			pv = link_map[pv].le;

		link_map[pv].le = po;
	}

	return po;
}

void print_link(uint32_t po)
{
	if (NULL == link_map)
		return;

	struct link_head *lh = (struct link_head *)link_map;
	uint32_t p = po;

	print_move(link_map[p].mo);

	if (0 == link_map[po].dn) {
		printf(" *\n\n");
	} else {
		print_link(link_map[po].dn);
	}

	if (0 != link_map[po].le) {

		print_move(link_map[p].mo);
		print_link(link_map[po].le);
	}

}

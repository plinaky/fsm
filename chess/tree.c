#include "standards.h"
#include "board.h"
#include "mapper.h"
#include "tree.h"
#include "move.h"
#include "compare.h"

#define MAX_NODE (1 << 24)

struct node *node_map = NULL;

static const char node_map_file[]  = "./data/nodes";

int open_game_map(void)
{
	static bool init_done = false;

	if (init_done)
		return EXIT_SUCCESS;

	node_map = (struct node *)open_map(node_map_file, MAX_NODE * sizeof(struct node));

	if (NULL == node_map) {
		printf("failed to open game map\n");
		exit(1);
	}

	init_done = true; 

	return EXIT_SUCCESS;
}

int flush_game_map(void)
{
	return flush_map(node_map, MAX_NODE * sizeof(struct node));
}

uint32_t store_pos(struct board *bo)
{
	uint32_t i, j, a = 0, b = 0, c = 0;
	uint8_t *data = (uint8_t *)bo;
	struct node no;

	for (i = 0; i < 8; i++)
		a ^= data[3 * i];

	for (i = 0; i < 8; i++)
		b ^= data[3 * i + 1];

	for (i = 0; i < 8; i++)
		c ^= data[3 * i + 2];

	j = (a << 16 + b << 8 + c) % MAX_NODE;
	memset(&no, 0, sizeof(struct node));

	for (i = j; i < MAX_NODE; i++) {

		if (0 == memcmp(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (0 == memcmp(node_map + i, no)) {
			memcpy(&node_map[i].bo, bo, sizeof(struct board));
			return i;
		}
	}

	for (i = 0; i < j; i++) {

		if (0 == memcmp(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (0 == memcmp(node_map + i, no)) {
			memcpy(&node_map[i].bo, bo, sizeof(struct board));
			return i;
		}
	}

	return 0;

}

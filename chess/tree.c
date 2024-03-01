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

static uint32_t hash_pos(struct board *bo)
{
	uint8_t *data = (uint8_t *)bo;
	uint32_t res = 0;
	uint8_t i, j, hash[4];

	for (i = 0; i < 4; i++)
		hash[i] = 0;

	for (i = 0; i < sizeof(struct board); i++)
		hash[i % 4] ^= data[i];

	for (i = 0; i < 4; i++)
		res += hash[i] << (8 * i);

	return res;

}

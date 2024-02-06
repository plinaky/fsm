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
	uint32_t i, a = 0, b = 0, c = 0;
	uint8_t *data = (uint8_t *)bo;

	for (i = 0; i < 8; i++)
		a ^= data[3 * i];

	for (i = 0; i < 8; i++)
		b ^= data[3 * i + 1];

	for (i = 0; i < 8; i++)
		c ^= data[3 * i + 2];

	return ((a << 16) + (b << 8) + c) % MAX_NODE;

}

uint32_t store_pos(struct board *bo)
{
	uint32_t i, j;
	struct board nb;
	size_t bs = sizeof(struct board);
	memset(&nb, 0, sizeof(struct board));
	
	j = hash_pos(bo);

	for (i = j; i < MAX_NODE; i++) {

		if (same_pos(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (same_pos(&node_map[i].bo, &nb)) {
			memcpy(&node_map[i].bo, bo, bs);
			return i;
		}
	}

	for (i = 0; i < j; i++) {

		if (same_pos(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (same_pos(&node_map[i].bo, &nb)) {
			memcpy(&node_map[i].bo, bo, bs);
			return i;
		}
	}

	return 0;
}

void clear_pos(uint32_t po)
{
	memset(&node_map[po], 0, sizeof(struct node));
}

void moves_from_pos(uint32_t po, uint16_t *ml, uint8_t *cnt)
{
	uint32_t i, j;
	struct board nb;

	*cnt = 0;	

	if (0 == node_map[bo].vi)
		return;

	if (1 == node_map[po].bo.cm)
		return true;


	j = hash_pos(bo);

	for (i = j; i < MAX_NODE; i++) {

		if (same_pos(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (same_pos(&node_map[i].bo, &nb)) {
			memcpy(&node_map[i].bo, bo, bs);
			return i;
		}
	}

	for (i = 0; i < j; i++) {

		if (same_pos(&node_map[i].bo, bo)) {
			node_map[i].vi++;
			return i;
		}

		if (same_pos(&node_map[i].bo, &nb)) {
			memcpy(&node_map[i].bo, bo, bs);
			return i;
		}
	}

	return 0;
}

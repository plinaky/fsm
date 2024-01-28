#include "board.h"
#include "move.h"
#include "compare.h"
#include "mapper.h"
#include "tree.h"

uint16_t play(struct board *bo, uint16_t max)
{
	uint16_t i, j, ml[255];
	uint32_t r;
	uint8_t cnt = 1;
	struct node *map = create_map(max);

	for (i = 0; i < max; i++) {
		if (list_legal_moves(bo, ml, &cnt) || !cnt)
			break;
		r = rand() % cnt;
		apply_move(bo, ml[r]);
		store_pos(map, bo, ml[r]);
	}

	flush_map("game0.txt", map, max);
	map = open_map("game0.txt", max);

	for (i = 0; i < ((uint32_t)(map[0].r)); i++) {
		printf("Move %d : ", i);
		print_move(map[i].move);
		print_pos(&map[i].b);
	}
	return i;
}

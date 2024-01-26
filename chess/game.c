#include "board.h"
#include "move.h"
#include "compare.h"

uint16_t play(struct board *bo, uint16_t max)
{
	uint16_t ml[200];
	int r;
	uint16_t i, game[max];
	uint8_t cnt;
	bool res;
	struct board b;

	memcpy(&b, bo, sizeof(struct board));

	for (i = 0; i < max; i++) {
		res = list_legal_moves(bo, ml, &cnt);
		if (res || !cnt)
			break;
		r = rand() % cnt;
		apply_move(bo, ml[r]);
		game[i] = ml[r];
		printf("%d ", compare(bo, &b));
	}
	printf("\n\n");

	bo->turn = (bo->turn ? 0 : 1);
	if (!res && !cnt)
		print_pos(bo);

	return i;
}

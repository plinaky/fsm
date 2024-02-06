#include "standards.h"
#include "board.h"
#include "move.h"
#include "compare.h"
#include "mapper.h"
#include "tree.h"

void play(struct board *bo)
{
	uint16_t ml[512];
	uint32_t move, next;
	uint32_t up = 0;
	uint16_t i, j;
	uint16_t null_cnt = 0;
	uint8_t move_cnt = 1;
	bool check = false;
	uint16_t max_moves = 511;

	store_pos(bo);

	for (i = 1; i <= max_moves; i++) {

		check = list_legal_moves(bo, ml, &move_cnt);

		if (check && !move_cnt) {
			printf("%+02d - checkmate at move %d", (bo->turn ? -1 : 1), i / 2);
			goto checkmate;
		}

		if (!move_cnt) {
			printf(" 0 - pat at move %d", i / 2);
			goto draw;
		}

		move = rand() % move_cnt;

		if ((TAKOF(ml[move])) ||
				(BP_ == FIG(get_piece(bo, X1_OF(ml[move]), Y1_OF(ml[move])))))
			null_cnt = 0;
		else
			null_cnt++;

		if (100 == null_cnt) {
			printf(" 0 - 50 moves rule at move %d", i / 2);
			goto draw;
		}

		apply_move(bo, ml[move]);

		next = store_pos(bo);

		if (node_map[next].vi >= 3) {
			printf(" 0 - position repeated 3 times at move %d", i / 2);
			goto draw;
		}
	}

	printf("WARNING: no draw in %d half-moves!", max_moves);

draw :
	print_pos(bo);
	return;

checkmate :
	print_pos(bo);

	if (bo->turn)
		node_map[up].wi++;
	else
		node_map[up].lo++;

}

#include "standards.h"
#include "board.h"
#include "move.h"
#include "compare.h"
#include "mapper.h"
#include "tree.h"

int8_t play(struct board *bo)
{
	uint16_t ml[255];
	uint32_t r;
	uint16_t i, j;
	uint16_t null_cnt = 0;
	uint8_t move_cnt = 1;
	bool check = false;
	uint16_t max_moves = 1000;

	struct board *game = (struct board *)create_map(max_moves * sizeof(struct board));

	store_pos(game, bo, 0);

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

		r = rand() % move_cnt;

		if ((TAKOF(ml[r])) ||
				(BP_ == FIG(get_piece(bo, X1_OF(ml[r]), Y1_OF(ml[r])))))
			null_cnt = 0;
		else
			null_cnt++;

		if (100 == null_cnt) {
			printf(" 0 - 50 moves rule at move %d", i / 2);
			goto draw;
		}

		apply_move(bo, ml[r]);

		if (3 == store_pos(game, bo, i)) {
			printf(" 0 - position repeated 3 times at move %d", i / 2);
			goto draw;
		}
	}

	printf("WARNING: no draw in %d half-moves!", max_moves);

draw :
	print_pos(bo);
	delete_map(game, max_moves * sizeof(struct board));
	return 0;

checkmate :
	print_pos(bo);
	delete_map(game, max_moves * sizeof(struct board));
	return (bo->turn ? -1 : 1);

}


#include "standards.h"
#include "board.h"
#include "move.h"

#define MAX_MOVES 512

void play(struct board *bo)
{
	uint16_t ml[MAX_MOVES];
	struct board b[MAX_MOVES];
	uint32_t move;
	uint16_t i, j;
	uint16_t null_cnt = 0;
	uint8_t move_cnt = 1;
	uint8_t rep_cnt;
	bool check = false;

	memcpy(b, bo, sizeof(struct board));

	for (i = 1; i < MAX_MOVES; i++) {

		memcpy(b + i, b + i - 1, sizeof(struct board));

		check = list_legal_moves((b + i), ml, &move_cnt);

		if (check && !move_cnt) {
			printf("%+02d - checkmate at move %d", ((b + i)->turn ? -1 : 1), i / 2);
			goto checkmate;
		}

		if (!move_cnt) {
			printf(" 0 - pat at move %d", i / 2);
			goto draw;
		}

		move = rand() % move_cnt;

		if ((TAKOF(ml[move])) ||
				(BP_ == FIG(get_piece(b + i, X1_OF(ml[move]), Y1_OF(ml[move])))))
			null_cnt = 0;
		else
			null_cnt++;

		if (100 == null_cnt) {
			printf(" 0 - 50 moves rule at move %d", i / 2);
			goto draw;
		}

		apply_move(b + i, ml[move]);

		rep_cnt = 0;
		for (j = 0; j < i; j++) { 
			if (same_board(b + i, b +j))
				rep_cnt++;
			if (rep_cnt >= 3) {
				printf(" 0 - position repeated 3 times at move %d", i / 2);
				goto draw;
			}
		}
	}

	printf("WARNING: no draw in %d half-moves!", MAX_MOVES);
	print_pos(b + i - 1);
	return;

draw :
	print_pos(b + i);
	return;

checkmate :
	print_pos(b + i);
	i++;

}

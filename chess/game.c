#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "compare.h"

//#define DEBUG_MOVES
#define DEBUG_EOG

int8_t play(struct board *bo, uint16_t max)
{
	struct board org;
	int r;
	uint8_t cnt;
	uint16_t i;
	uint16_t ml[200];
	uint16_t game[max];
	bool res;

	memcpy(&org, bo, sizeof(struct board));

	for (i = 0; i < max; i++) {
		res = list_legal_moves(bo, ml, &cnt);
		if (!cnt || res)
			break;
		r = rand() % cnt;
		apply_move(bo, ml[r]);
		game[i] = ml[r];
#ifdef DEBUG_MOVES
		print_moves(ml, cnt);
		printf("move %d: ", i);
		print_move(ml[r]);
		printf("\n");
		print_board(bo);
#endif
	}

#ifdef DEBUG_EOG
	if (!cnt && res) {
		print_board(bo);
		print_moves(ml, cnt);
		printf("\n");
		print_moves(game, i);
		memcpy(bo, &org, sizeof(struct board));
		replay(bo, game, i);
		exit(0);
	}
#endif

	return 0;
}

void replay(struct board *bo, uint16_t *ml, uint16_t cnt)
{
	uint16_t ml2[200], ml3[200];
	uint8_t cnt2, cnt3;
	struct board b3;

	for (uint16_t i = 0; i < cnt; i++) {
		//printf("\nmove %d: ", i);
		//print_move(ml[i]);
		apply_move(bo, ml[i]);
		//print_board(bo);
		list_moves(bo, ml2, &cnt2);
		//print_moves(ml2, cnt2);
	}
	print_board(bo);
	printf("\nMoves after mat: \n");
	print_moves(ml2, cnt2);
	for (uint16_t i = 0; i < cnt2; i++) {
		memcpy(&b3, bo, sizeof(struct board));
		printf("\nmove %d: ", i);
		print_move(ml2[i]);
		apply_move(&b3, ml2[i]);
		print_board(&b3);
		list_moves(&b3, ml3, &cnt3);
		print_moves(ml3, cnt3);
	}

}

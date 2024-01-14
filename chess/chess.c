#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "rules.h"

int main(void)
{
	struct position po;
	uint64_t cnt, k;
	struct move moves[200];
	int8_t i, j, l;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			set_piece(&po, i, j, default_board[i][j]);
		}
	}

	po.W_OO_1 = 1;
	po.W_OOO_1 = 1;
	po.B_OO_1 = 1;
	po.B_OOO_1 = 1;

	po.W_OO_2 = 0;
	po.W_OOO_2 = 0;
	po.B_OO_2 = 0;
	po.B_OOO_2 = 0;
/*
	po.turn = WHITE;
	po.a_passe = 0;
	po.en_passant = 2; 
	cnt = 0;
	list_legal_moves(&po, (struct move *)moves, &cnt);
*/
	po.turn = BLACK;
	po.a_passe = 0;
	cnt = 0;
	list_legal_moves(&po, (struct move *)moves, &cnt);

	return 0;
}

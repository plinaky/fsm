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

	print_pos(&po);

	printf("figure   %ld \n", sizeof(enum figure));
	printf("color    %ld \n", sizeof(enum color));
	printf("piece    %ld \n", sizeof(struct piece));
	printf("move     %ld \n", sizeof(struct move));
	printf("position %ld \n", sizeof(struct position));

	po.castle = 15;

	po.turn = WHITE;
	po.a_passe = 0;
	po.en_passant = 2; 
	cnt = 0;
	list_moves(&po, (struct move *)moves, &cnt);
	printf("found %ld moves : ", cnt);
	for (k = 0; k < cnt; k++)
		print_move(moves[k]);

	po.turn = BLACK;
	po.a_passe = 0;
	cnt = 0;
	list_moves(&po, (struct move *)moves, &cnt);
	printf("found %ld moves : ", cnt);
	for (k = 0; k < cnt; k++)
		print_move(moves[k]);
	printf("\n");

	return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "rules.h"

int main(void)
{
	struct position po;
	uint64_t cnt = 0;
	struct move moves[200];
	int8_t i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			set_piece(&po, i, j, default_board[i][j]);
		}

	print_pos(&po);

	printf("figure   %ld \n", sizeof(enum figure));
	printf("color    %ld \n", sizeof(enum color));
	printf("piece    %ld \n", sizeof(struct piece));
	printf("move     %ld \n", sizeof(struct move));
	printf("position %ld \n", sizeof(struct position));

	po.castle = 7;
	po.a_passe = 0;
	po.turn = WHITE;

	list_moves(&po, (struct move *)moves, &cnt);

//	printf("found %ld moves\n", cnt);
 
	for (i = 0; i < cnt; i++)
		print_move(moves[i]);

	printf("\n");

	po.turn = BLACK;
	list_moves(&po, (struct move *)moves, &cnt);

	for (; i < cnt; i++)
		print_move(moves[i]);

	printf("\n");
	return 0;
}

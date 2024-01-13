#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "rules.h"

int main(void)
{
	struct position po;
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

	return 0;
}

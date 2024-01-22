#include <stdio.h>
#include <string.h>
#include "board.h"
#include <stdlib.h>

bool in_bound(int8_t x, int8_t y)
{
	return (0 <= x) && (x < 8) && (0 <= y) && (y < 8);
}

bool on_bound(int8_t x)
{
	return (0 == x) || (x == 7);
}

char to_char(uint8_t pi)
{
	char c = ' ';

	if ((pi | WHITE) == WP_) c = 'P';
	if ((pi | WHITE) == WN_) c = 'N';
	if ((pi | WHITE) == WB_) c = 'B';
	if ((pi | WHITE) == WR_) c = 'R';
	if ((pi | WHITE) == WQ_) c = 'Q';
	if ((pi | WHITE) == WK_) c = 'K';

	if (!(pi & WHITE) && pi)
		c = c - 'A' + 'a' ; 

	return c;
}

void print_square(uint8_t sq)
{
	uint8_t x = sq & 0b111;
	char y    = (sq >> 3) & 0b111;

	printf("%c%d", y + 'a', x + 1);
}

uint8_t square_of(uint8_t x, uint8_t y)
{
	return (((y & 0b111) << 3) |  (x & 0b111)) & 0xFF;
}

uint8_t get_piece(struct board *bo, uint8_t sq)
{
	uint8_t x = sq & 0b111;
	uint8_t y = (sq >> 3) & 0b111;
	uint8_t index  =  (8 * x + y) / 2;
	uint8_t offset = ((8 * x + y) % 2) << 2; 

	return (bo->pos[index] >> offset) & 0b1111;
}

void set_piece(struct board *bo, uint8_t sq, uint8_t pi)
{
	uint8_t x = sq & 0b111;
	uint8_t y = (sq >> 3) & 0b111;
	uint8_t index  =  (8 * x + y) / 2;
	uint8_t offset = ((8 * x + y) % 2) * 4; 

	bo->pos[index] &= 0b11110000 >> offset;
	bo->pos[index] |= (pi << offset) & 0b11111111;
}

void print_board(struct board *bo)
{
	uint8_t i, j;

	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("%s to play\t", (bo->turn ? "BLACK" : "WHITE"));
	printf("W/B cr: ");
	printf("%s",   (bo->wsc ? "o" : "-"));
	printf("%s/",  (bo->wbc ? "O" : "-"));
	printf("%s",   (bo->bsc ? "o" : "-"));
	printf("%s\t", (bo->bbc ? "O" : "-"));

	if (bo->hidden) {
		printf("hidden:");
		print_square(0b111111 & bo->hidden);
	}
	printf("\n");

	printf("   a  b  c  d  e  f  g  h\n");
	for (i = 7; i < 8; i--) {
		printf("%d ", i + 1);
		for (j = 0; j < 8; j++) {
			if ((i + j) % 2)
				printf("%s", inv_start);
			printf(" %c ", to_char(get_piece(bo, square_of(i, j))));
			if ((i + j) % 2)
				printf("%s", inv_stop);
		}
		printf("\n");
	}
}

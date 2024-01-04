#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"

/*
uint8_t default_board[8][8] = {
	{WR, WN, WB, WQ, WK, WB, WN, WR},
	{WP, WP, WP, WP, WP, WP, WP, WP},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{BP, BP, BP, BP, BP, BP, BP, BP},
	{BR, BN, BB, BQ, BK, BB, BN, BR}
};
*/

uint8_t default_board[8][8] = {
	{WR,  0, WB, WQ, WK, WB,  0, WR},
	{WP,  0, WP, WP, WP, WP, WP, WP},
	{WN,  0,  0,  0,  0, WN,  0,  0},
	{ 0, BB,  0,  0,  0,  0,  0,  0},
	{ 0, WP, BP, BP, BP, BP,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{BP, BP,  0,  0,  0, BP, BP, BP},
	{BR, BN, BB, BQ, BK,  0, BN, BR}
};

char to_char(uint8_t fig)
{
	if (fig == WP) return 'P';
	if (fig == WN) return 'N';
	if (fig == WB) return 'B';
	if (fig == WR) return 'R';
	if (fig == WQ) return 'Q';
	if (fig == WK) return 'K';

	if (fig == BP) return 'p';
	if (fig == BN) return 'n';
	if (fig == BB) return 'b';
	if (fig == BR) return 'r';
	if (fig == BQ) return 'q';
	if (fig == BK) return 'k';

	return ' ';
}

uint8_t get_piece(uint8_t board[32], uint8_t square)
{
	uint16_t i = SQI(square);
	uint16_t j = SQJ(square);
	uint16_t pos = 8 * i + j;
	return  0xf & (board[pos / 2] >> (4 * (j % 2)));
}

void set_piece(uint8_t board[32], uint8_t square, uint8_t fig)
{
	uint16_t i = SQI(square);
	uint16_t j = SQJ(square);
	uint16_t pos = 8 * i + j;

	board[pos / 2] &= ~(0xf << (4 * (j % 2)));
	board[pos / 2] |= fig << (4 * (j % 2));
}

uint8_t get_square(uint8_t board[32], uint8_t fig)
{
	if (COL(fig) == WHITE) {
		for (int8_t i = 0; i < 8; i++)
			for (int8_t j = 0; j < 8; j++)
				if (GET_PIECE(board, i, j) == fig)
					return SQUARE(i, j);
		printf("%c not found", to_char(fig));
	} else {
		for (int8_t i = 7; i >= 0; i--)
			for (int8_t j = 7; j >= 0; j--)
				if (GET_PIECE(board, i, j) == fig)
					return SQUARE(i, j);
		printf("%c not found", to_char(fig));
	}

	return 0;
}

void print_pos(uint8_t board[32])
{
	char inv_start[] = "\x1b[7m";
	char inv_stop[]  = "\x1b[0m";

	printf("\n\t    a   b   c   d   e   f   g   h\n");
	printf("\t   -------------------------------\n");

	for (int i = 7; i >= 0; i--) {

		printf("\t%d |", i + 1);

		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);

			printf(" %c ", to_char(GET_PIECE(board, i, j)));

			if ((i + j) % 2)
				printf("%s", inv_stop);

			printf("|");
		}

		printf(" %d\n", i + 1);
		printf("\t   -------------------------------\n");
	}

	printf("\t    a   b   c   d   e   f   g   h\n");
}

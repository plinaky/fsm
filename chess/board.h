#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

#define	WHITE  (1 << 3)

#define OO_ 0

#define BP_ 1
#define BN_ 2 
#define BB_ 3
#define BR_ 4
#define BQ_ 5
#define BK_ 6

#define WP_ (WHITE | BP_)
#define WN_ (WHITE | BN_)
#define WB_ (WHITE | BB_)
#define WR_ (WHITE | BR_)
#define WQ_ (WHITE | BQ_)
#define WK_ (WHITE | BK_)

#define FIG(_PI_) ((uint8_t)((_PI_) & 0b111))
#define COL(_PI_) ((uint8_t)((_PI_) & (WHITE)))

struct board {
	uint8_t   pos[32];
	uint16_t  turn   : 1;
	uint16_t  wsc    : 1;
	uint16_t  wbc    : 1;
	uint16_t  bsc    : 1;
	uint16_t  bbc    : 1;
	uint16_t  hidden : 6;
};

uint8_t get_piece(struct board *bo, uint8_t sq);
void set_piece(struct board *bo, uint8_t sq, uint8_t pi);
uint8_t square_of(uint8_t x, uint8_t y);
bool in_bound(int8_t x, int8_t y);
bool on_bound(int8_t x);
char to_char(uint8_t pi);
void print_square(uint8_t sq);
void print_board(struct board *bo);
#endif

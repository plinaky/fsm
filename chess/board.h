#ifndef BOARD_H
#define BOARD_H

#define	PAWN   1
#define KNIGHT 2
#define BISHOP 3
#define ROOK   4
#define QUEEN  5
#define KING   6

#define WHITE (0 << 3)
#define BLACK (1 << 3)

#define WR (WHITE | ROOK)
#define WN (WHITE | KNIGHT)
#define WB (WHITE | BISHOP)
#define WQ (WHITE | QUEEN)
#define WK (WHITE | KING)
#define WP (WHITE | PAWN)

#define BR (BLACK | ROOK)
#define BN (BLACK | KNIGHT)
#define BB (BLACK | BISHOP)
#define BQ (BLACK | QUEEN)
#define BK (BLACK | KING)
#define BP (BLACK | PAWN)

#define SQI(_square) ((uint8_t)(((_square) >> 3) & 0x7))
#define SQJ(_square) ((uint8_t)((_square) & 0x7))

#define SQUARE(_i, _j) ((uint8_t)(((uint8_t)(_i) << 3  | (uint8_t)(_j)) & 0x3f))

#define FIG(_fig) ((uint8_t)((_fig) & 0x7))
#define COL(_fig) ((uint8_t)(((_fig) & 0x8) >> 3))

#define GET_PIECE(_board, _i, _j) get_piece(_board, SQUARE((_i), (_j)))
#define SET_PIECE(_board, _i, _j, _fig) set_piece(_board, SQUARE((_i), (_j)), (_fig))

extern uint8_t default_board[8][8];

char to_char(uint8_t fig);

uint8_t get_piece(uint8_t board[32], uint8_t square);
void set_piece(uint8_t board[32], uint8_t square, uint8_t fig);

void print_pos(uint8_t board[32]);

#endif

#ifndef BOARD_H
#define BOARD_H

#define	WHITE  (1 << 3)

#define BK_ 6
#define BQ_ 5
#define BR_ 4
#define BB_ 3
#define BN_ 2
#define BP_ 1

#define WP_ (WHITE | BP_)
#define WN_ (WHITE | BN_)
#define WB_ (WHITE | BB_)
#define WR_ (WHITE | BR_)
#define WQ_ (WHITE | BQ_)
#define WK_ (WHITE | BK_)

#define FIG(_PI_) ((uint8_t)((_PI_) & 0b111))
#define COL(_PI_) ((uint8_t)((_PI_) & (WHITE)))
#define OP_COL(_PI_) ((uint8_t)(((_PI_) ? (_PI_) ^ 0x8 : 0) & 0b1111))

#define MOVE_OF(_X1_, _Y1_, _X2_, _Y2_, _PRO_, _TAK_) ((uint16_t)   \
	 ((((_X1_)   & 0b111) <<  0) | (((_Y1_)   & 0b111) <<  3) | \
	  (((_X2_)   & 0b111) <<  6) | (((_Y2_)   & 0b111) <<  9) | \
	  (((_PRO_)  & 0b111) << 12) | (((_TAK_)  & 0b1  ) << 15)))

#define X1_OF(_MO_) ((uint8_t)(((_MO_) >>  0) &  0b111))
#define Y1_OF(_MO_) ((uint8_t)(((_MO_) >>  3) &  0b111))
#define X2_OF(_MO_) ((uint8_t)(((_MO_) >>  6) &  0b111))
#define Y2_OF(_MO_) ((uint8_t)(((_MO_) >>  9) &  0b111))
#define PROMO(_MO_) ((uint8_t)(((_MO_) >> 12) &  0b111))
#define TAKOF(_MO_) ((uint8_t)(((_MO_) >> 15) &  0b1  ))

#ifdef NO_LOG
#define printf(...) ((void)0)
#endif

struct board {
	uint8_t  pos[32];
	uint16_t turn : 1;
	uint16_t wsc  : 1; /* castling rights */
	uint16_t wbc  : 1;
	uint16_t bsc  : 1;
	uint16_t bbc  : 1;
	uint16_t hx   : 3; /* en passant or castle square */
	uint16_t hy   : 3;
	uint16_t ca   : 4; /* last captured piece */
	uint16_t cm   : 1; /* checkmate */
};

extern uint8_t pref[8][8];

uint8_t get_piece(struct board *bo, int8_t x, int8_t y);
void set_piece(struct board *bo, int8_t x, int8_t y, uint8_t pi);
bool in_bound(int8_t x, int8_t y);
bool on_bound(int8_t x);
char to_char(uint8_t pi);
void print_square(uint8_t x, uint8_t y);
void print_board(struct board *bo);
void print_pos(struct board *bo);
void init_board(struct board *bo, uint8_t po[8][8]);

#endif

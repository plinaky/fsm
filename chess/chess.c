#include <stdio.h>
#include <string.h>
#include <stdint.h>

enum kind {
	PAWN   = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK   = 4,
	QUEEN  = 5,
	KING   = 6
} __attribute__((packed));

enum color {
       	WHITE = 0,
	BLACK = 1
} __attribute__((packed));

enum pinning {
       	UNPINNED  = 0,
	PINNED    = 1
} __attribute__((packed));

enum castle {
       	CANT  = 0,
	OO    = 1,
	OOO   = 2,
} __attribute__((packed));

struct piece {
    enum color   col : 1;
    enum kind    fig : 3;
} __attribute__((packed));

struct square {
    uint8_t line : 3;  /* 3 bits */
    uint8_t col  : 3;  /* 3 bits */
} __attribute__((packed));

struct position {
    uint8_t A1 : 4; uint8_t B1 : 4; uint8_t C1 : 4; uint8_t D1 : 4; uint8_t E1 : 4; uint8_t F1 : 4; uint8_t G1 : 4; uint8_t H1 : 4; 
    uint8_t A2 : 4; uint8_t B2 : 4; uint8_t C2 : 4; uint8_t D2 : 4; uint8_t E2 : 4; uint8_t F2 : 4; uint8_t G2 : 4; uint8_t H2 : 4; 
    uint8_t A3 : 4; uint8_t B3 : 4; uint8_t C3 : 4; uint8_t D3 : 4; uint8_t E3 : 4; uint8_t F3 : 4; uint8_t G3 : 4; uint8_t H3 : 4; 
    uint8_t A4 : 4; uint8_t B4 : 4; uint8_t C4 : 4; uint8_t D4 : 4; uint8_t E4 : 4; uint8_t F4 : 4; uint8_t G4 : 4; uint8_t H4 : 4; 
    uint8_t A5 : 4; uint8_t B5 : 4; uint8_t C5 : 4; uint8_t D5 : 4; uint8_t E5 : 4; uint8_t F5 : 4; uint8_t G5 : 4; uint8_t H5 : 4; 
    uint8_t A6 : 4; uint8_t B6 : 4; uint8_t C6 : 4; uint8_t D6 : 4; uint8_t E6 : 4; uint8_t F6 : 4; uint8_t G6 : 4; uint8_t H6 : 4; 
    uint8_t A7 : 4; uint8_t B7 : 4; uint8_t C7 : 4; uint8_t D7 : 4; uint8_t E7 : 4; uint8_t F7 : 4; uint8_t G7 : 4; uint8_t H7 : 4; 
    uint8_t A8 : 4; uint8_t B8 : 4; uint8_t C8 : 4; uint8_t D8 : 4; uint8_t E8 : 4; uint8_t F8 : 4; uint8_t G8 : 4; uint8_t H8 : 4; 
} __attribute__((packed));

#define MOVE_SIZE 256

struct game {
	struct position pos;
	uint64_t moves[MOVE_SIZE];
	enum color turn;
};

static void mem_set(uint8_t *buf, const uint8_t len, const uint64_t pos, const uint8_t val)
{
	uint64_t bit_offset = pos * len;
	uint64_t byte_index = bit_offset / 8;
	uint8_t  bit_mask   = (1 << len) - 1;
	uint8_t  bit_shift  = bit_offset % 8;
	int8_t   over_shoot = len + bit_shift - 8;

	buf[byte_index] &= ~(bit_mask << bit_shift);
	buf[byte_index] |= val << bit_shift;

	if (over_shoot > 0) {
		buf[byte_index + 1] &= ~(0xff >> (8 - over_shoot));
		buf[byte_index + 1] |= val >> (len - over_shoot);
	}
}

static uint8_t mem_get(uint8_t *buf, const uint8_t len, const uint64_t pos)
{
	uint64_t bit_offset = pos * len;
	uint64_t byte_index = bit_offset / 8;
	uint16_t res;
	uint8_t  bit_mask   = (1 << len) - 1;
	uint8_t  bit_shift  = bit_offset % 8;

	res = buf[byte_index];

	if (len + bit_shift > 7)
		res |= buf[byte_index + 1] << 8;

	res = (buf[byte_index] >> bit_shift) & bit_mask;

	return (uint8_t)res;
}

static inline struct piece get_piece(const struct position *pos, const struct square xy)
{
	uint8_t res = mem_get((uint8_t *)pos, 4, 8 * xy.line + xy.col);

	return *((struct piece *)&res);
}

static void inline set_piece(struct position *pos, struct square xy, struct piece fig)
{
	mem_set((uint8_t *)pos, 4, 8 * xy.line + xy.col, *((uint8_t *)&fig));
}

static inline struct square my_square(uint8_t i, uint8_t j)
{
	struct square res = {i & 0x7, i & 0x7};
	return res;
}

#define GET_PIECE(POS, I, J) get_piece(POS, my_square(I, J))

static char to_char(const struct piece p)
{
	switch (p.fig) {
		case PAWN   : return 'P';
		case KNIGHT : return 'N';
		case BISHOP : return 'B';
		case ROOK   : return 'R';
		case QUEEN  : return 'Q';
		case KING   : return 'K';
		default     : return ' ';
	}
}

static void print_pos(const struct position *pos)
{
	const char inv_start[] = "\x1b[7m";
	const char inv_stop[]  = "\x1b[0m";

	printf("\n\t    a   b   c   d   e   f   g   h\n");
	printf("\t   -------------------------------\n");

	for (int i = 7; i >= 0; i--) {

		printf("\t%d |", i + 1);

		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2)
				printf("%s", inv_start);

			printf(" %c ", to_char(GET_PIECE(pos, i, j)));

			if ((i + j) % 2)
				printf("%s", inv_stop);

			printf("|");
		}

		printf(" %d\n", i + 1);
		printf("\t   -------------------------------\n");
	}

	printf("\t    a   b   c   d   e   f   g   h\n");
}

static inline void add_pawn_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}

static inline void add_rook_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}

static inline void add_knight_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}

static inline void add_bishop_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}

static inline void add_queen_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}

static inline void add_king_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
}


static inline void add_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
	struct piece p = GET_PIECE(&gm->pos, i, j);
	switch (p.fig) {
		case PAWN   : 
			add_pawn_moves(gm, cur, i, j);
		case KNIGHT : 
			add_knight_moves(gm, cur, i, j);
		case BISHOP : 
			add_bishop_moves(gm, cur, i, j);
		case ROOK   : 
			add_rook_moves(gm, cur, i, j);
		case QUEEN  : 
			add_queen_moves(gm, cur, i, j);
		case KING   : 
			add_king_moves(gm, cur, i, j);
	}
}

static inline void add_piece(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j)
{
	gm->moves[*cur++] = 1;
	gm->moves[*cur++] = i;
	gm->moves[*cur++] = j;
}

static void fill_moves(struct game *gm)
{
	memset(gm->moves, 0, MOVE_SIZE);
	unsigned int cur = 0;

	for (uint8_t i = 0; i < 8; i++) {
		for (uint8_t j = 0; j < 8; j++ ) {
			struct piece p = GET_PIECE(&gm->pos, i, j);
			if (p.fig != 0) {
				add_piece(gm, &cur, i, j);
				printf("%c %c%d\n", to_char(p), 'a' + j, i + 1);
				if (cur >= MOVE_SIZE) {
					printf("size exceeded %d\n", cur);
					break;
				}
			}
		}
	}
	printf("used / available  %d / %d\n", cur, MOVE_SIZE);
}

int main(void)
{
	struct game gm;

	memset(&gm.pos, 0 , sizeof(struct position));

	gm.pos.A1 = WHITE << 3 | ROOK;
	gm.pos.B1 = WHITE << 3 | KNIGHT;
	gm.pos.C1 = WHITE << 3 | BISHOP;
	gm.pos.D1 = WHITE << 3 | QUEEN;
	gm.pos.E1 = WHITE << 3 | KING;
	gm.pos.F1 = WHITE << 3 | BISHOP;
	gm.pos.G1 = WHITE << 3 | KNIGHT;
	gm.pos.H1 = WHITE << 3 | ROOK;

	gm.pos.A2 = WHITE << 3 | PAWN;
	gm.pos.B2 = WHITE << 3 | PAWN;
	gm.pos.C2 = WHITE << 3 | PAWN;
	gm.pos.D2 = WHITE << 3 | PAWN;
	gm.pos.E2 = WHITE << 3 | PAWN;
	gm.pos.F2 = WHITE << 3 | PAWN;
	gm.pos.G2 = WHITE << 3 | PAWN;
	gm.pos.H2 = WHITE << 3 | PAWN;

	gm.pos.A8 = BLACK << 3 | ROOK;
	gm.pos.B8 = BLACK << 3 | KNIGHT;
	gm.pos.C8 = BLACK << 3 | BISHOP;
	gm.pos.D8 = BLACK << 3 | QUEEN;
	gm.pos.E8 = BLACK << 3 | KING;
	gm.pos.F8 = BLACK << 3 | BISHOP;
	gm.pos.G8 = BLACK << 3 | KNIGHT;
	gm.pos.H8 = BLACK << 3 | ROOK;

	gm.pos.A7 = BLACK << 3 | PAWN;
	gm.pos.B7 = BLACK << 3 | PAWN;
	gm.pos.C7 = BLACK << 3 | PAWN;
	gm.pos.D7 = BLACK << 3 | PAWN;
	gm.pos.E7 = BLACK << 3 | PAWN;
	gm.pos.F7 = BLACK << 3 | PAWN;
	gm.pos.G7 = BLACK << 3 | PAWN;
	gm.pos.H7 = BLACK << 3 | PAWN;


	print_pos(&gm.pos);
	//fill_moves(&gm);

	return 0;
}

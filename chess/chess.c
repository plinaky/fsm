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
    enum color   col : 1;  /* 1 bit  */
    enum kind    fig : 3;  /* 3 bits */
} __attribute__((packed));

struct square {
    uint8_t line : 3;  /* 3 bits */
    uint8_t col  : 3;  /* 3 bits */
} __attribute__((packed));

/* 
 * a move list is a list of squares
 * */

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

#define BITS_PER_SQUARE 6

void set_square(int index, struct square value)
{
    int bit_offset = index * BITS_PER_SQUARE;
    int byte_index = bit_offset / 8;
    int bit_shift = bit_offset % 8;

    /* Insérer les nouvelles valeurs */
    buffer[byte_index] |= (value.line << bit_shift);
    buffer[byte_index + 1] |= (value.col << (bit_shift + 3));
}

static void mem_set(uint8_t *buf, const uint8_t len, const uint64_t pos, const uint8_t val)
{
	uint64_t bit_offset = pos * len;
	uint64_t byte_index = bit_offset / 8;
	uint8_t  bit_mask   = (1 << len) - 1;
	uint8_t  bit_shift  = bit_offset % 8;
	uint8_t  over_shoot = len + bit_shift;

	/* Effacer les bits concernés */
	buf[byte_index] &= (~(0b111111 << bit_shift) & 0b11111111);

	if (over_shoot > 7)
		buf[byte_index + 1] &= ~(0b11111111 >> (8 - over_shoot));


	/* Insérer les nouvelles valeurs */
	buffer[byte_index] |= (value << bit_shift) & 0b11111111;
	buffer[byte_index + 1] |= value >> (bit_shift + 3));

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


static char to_char(const struct piece *figure)
{
	switch (figure->kind) {
		case PAWN   : return 'P';
		case KNIGHT : return 'N';
		case BISHOP : return 'B';
		case ROOK   : return 'R';
		case QUEEN  : return 'Q';
		case KING   : return 'K';
		default     : return ' ';
	}
}

static void print_pos(const struct piece pos[8][8])
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

			printf(" %c ", to_char(pos[i][j]));

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
	enum piece figure = gm->pos[i][j] & 7;
	switch (figure) {
		case PAWN   : 
			add_pawn_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
		case KNIGHT : 
			add_knight_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
		case BISHOP : 
			add_bishop_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
		case ROOK   : 
			add_rook_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
		case QUEEN  : 
			add_queen_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
		case KING   : 
			add_king_moves(struct game *gm, unsigned int *cur, const uint8_t i, const uint8_t j);
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
			if (gm->pos[i][j] != 0) {
				add_piece(gm, &cur, i, j);
				printf("%c %c%d\n", to_char(gm->pos[i][j]), 'a' + j, i + 1);
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

	memcpy(gm.pos, def_pos, 64);
	print_pos(gm.pos);
	fill_moves(&gm);

	return 0;
}

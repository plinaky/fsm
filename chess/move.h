#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include <stdint.h>

struct move {
	uint8_t x1    : 3;
	uint8_t y1    : 3;
	uint8_t x2    : 3;
	uint8_t y2    : 3;
	uint8_t promo : 3;
	uint8_t take  : 1;
};

#endif

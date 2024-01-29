#ifndef TREE_H
#define TREE_H

#include "board.h"

struct node {
	uint32_t start;
	uint32_t stop;	
};

uint32_t store_pos(struct node *map, struct board *b, uint16_t move);

#endif

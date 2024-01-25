#ifndef COMPARE_H
#define COMPARE_H

#include <stdbool.h>
#include <stdint.h>

bool compare(struct board *b1, struct board *b2);
void flip(struct board *b);
bool mirror(struct board *b);
bool rotate(struct board *b, bool clock);

#endif

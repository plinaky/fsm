#ifndef COMPARE_H
#define COMPARE_H

bool same_board(struct board *b1, struct board *b2);
void flip(struct board *b);
bool mirror(struct board *b);
bool rotate(struct board *b, bool clock);

#endif

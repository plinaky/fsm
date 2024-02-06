#ifndef TREE_H
#define TREE_H

struct node {
    uint32_t wi, lo, vi;
    struct board bo;
    uint16_t mo;       /* last move, used to revert play */
};

extern struct node *node_map;

int open_game_map(void);
int flush_game_map(void);
uint8_t store_pos(struct board *bo);

#endif




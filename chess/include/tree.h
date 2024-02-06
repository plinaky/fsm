#ifndef TREE_H
#define TREE_H

struct node {
    uint32_t wi, lo, vi;
    struct board bo;
};

extern struct node *node_map;

int open_game_map(void);
int flush_game_map(void);
uint32_t store_pos(struct board *bo);

#endif




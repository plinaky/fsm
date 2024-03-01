#ifndef TREE_H
#define TREE_H

struct node {
    uint32_t wi, lo, vi;
    uint32_t hash;
    uint16_t move;
};

extern struct node *node_map;

int open_game_map(void);
int flush_game_map(void);

#endif




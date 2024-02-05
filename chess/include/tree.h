#ifndef TREE_H
#define TREE_H

struct link {
    uint32_t up, dn, le, mo;
    uint32_t wi, lo, vi;
};

struct link_head {
    uint32_t up, dn, le, mo;
    uint32_t mapped_size;
    uint32_t current_size;
    uint32_t unused;
};

struct board_head {
    uint16_t mapped_size;
    uint16_t current_size;
    uint16_t unused[15];
};

extern struct link *link_map;

int open_game_map(void);
int flush_game_map(void);
uint8_t store_pos(struct board *bmap, struct board *bo);
uint32_t store_link(uint32_t up, uint16_t mo);
void print_link(uint32_t po);

#endif

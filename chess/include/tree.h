#ifndef TREE_H
#define TREE_H

struct link {
    uint32_t start;
    uint32_t stop;
    uint32_t win;
    uint32_t loss;
    uint32_t draw;
    uint16_t cost;
    uint16_t move;
};

struct link_head {
    uint32_t mapped_size;
    uint32_t current_size;
    uint32_t unused[3];
};

struct board_head {
    uint32_t mapped_size;
    uint32_t current_size;
    uint16_t unused[13];
};

const char link_map_file[]  = "./data/links";
const char board_map_file[] = "./data/boards";

uint8_t store_pos(struct board *bmap, struct board *bo, uint16_t pos);
uint32_t store_board(struct board *bmap, struct board *bo, uint16_t pos);

#endif


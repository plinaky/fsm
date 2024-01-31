#ifndef TREE_H
#define TREE_H

struct link {
    uint32_t start;
    uint32_t stop;
    uint32_t win;
    uint32_t loss;
    uint32_t draw;
    uint32_t cost;
    uint16_t move;
};

#endif

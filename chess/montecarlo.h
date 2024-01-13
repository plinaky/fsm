#ifndef MONTECARLO_H
#define MONTECARLO_H

struct node {
    int64_t  score;
    int64_t  visit;
    struct node *side;
    struct node *down;
};

#endif

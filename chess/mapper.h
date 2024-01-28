#ifndef MAPPER_H
#define MAPPER_H

#include "standards.h"
#include "tree.h"
#include "board.h"
#include "move.h"

struct node *create_map(uint32_t pos_count);
struct node *open_map(const char *file_path, uint32_t pos_count);
int flush_map(char *filename, struct node *map, uint32_t pos_count);

#endif

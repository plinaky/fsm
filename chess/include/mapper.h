#ifndef MAPPER_H
#define MAPPER_H

void *create_map(const off_t size);
void *open_map(const char *path, const off_t size);
int delete_map(void *map, const off_t size);
int flush_map(char *path, void *map, const off_t size);
uint8_t store_pos(void *map, struct board *b, uint16_t pos);

#endif

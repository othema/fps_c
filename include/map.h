#pragma once

#include <stdlib.h>

#define READ_BUF_SIZE 100
#define MIN_MAP_SIZE 64  // 64 blocks (aka 8x8)

typedef struct {
	char *data;

	size_t map_size;
	size_t map_row_size;
} Map;

Map *map_load_from_file(const char *filename);
void map_free(Map *map);
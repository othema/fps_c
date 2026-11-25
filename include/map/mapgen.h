#pragma once

#include "map/types.h"

#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>

Map* mapgen(unsigned int seed, size_t size, uint8_t scale);
void map_free(Map *map);
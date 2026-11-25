#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>

#include "assets.h"

typedef enum {
	TILE_EMPTY,
	TILE_ROAD
} MapTile;

typedef struct {
	Vector3 position;
	ModelID model;
	int yaw;
} MapStructure;

typedef struct {
	size_t size;
	uint8_t scale;

	MapTile *tiles;
	MapStructure *structures;
	size_t n_structures;
} Map;
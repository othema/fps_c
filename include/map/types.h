#pragma once

#include "map/assets.h"

#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
	TILE_EMPTY,
	TILE_ROAD
} MapTile;

typedef struct {
	Vector3 position;
	MapAsset asset;
	int yaw;
} MapStructure;

typedef struct {
	size_t size;
	uint8_t scale;

	MapAssets assets;

	MapTile *tiles;
	MapStructure *structures;
	size_t n_structures;
} Map;
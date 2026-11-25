#pragma once

#include "map/constants.h"
#include "map/assets.h"

#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
	TILE_EMPTY,
	TILE_ROAD
} MapTile;

typedef struct MapRoad {
	Vector3 position;

	uint8_t connections;
	int yaw;
} MapRoad;

typedef struct {
	MapTile tiles[MAP_SIZE][MAP_SIZE];

	MapRoad roads[MAX_ROADS];
	size_t roads_n;

	MapAssets assets;

	uint8_t scale;
} Map;
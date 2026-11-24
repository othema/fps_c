#include "map/mapgen.h"
#include "map/assets.h"
#include "map/types.h"

#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define ROAD_LINES (MAP_SIZE / 4)

static void generate_layout(unsigned char tiles[MAP_SIZE][MAP_SIZE]);
static size_t generate_roads(MapRoad out[MAX_ROADS], unsigned char tiles[MAP_SIZE][MAP_SIZE], uint8_t scale);

Map* mapgen(unsigned int seed, uint8_t scale) {
	Map *out = malloc(sizeof(Map));
	if (out == NULL) {
		fprintf(stderr, "Couldn't malloc map\n");
		return NULL;
	}
	memset(out, 0, sizeof(Map));

	out->scale = scale;

	// Generate layout into out->tiles
	srand(seed);
	generate_layout(out->tiles);

	out->roads_n = generate_roads(out->roads, out->tiles, scale);

	// Load assets
	map_load_assets(&out->assets);

	// Generate 
	return out;
}

// Parse the tile map and translate all roads to MapRoad structs
// Returns the amount of roads
static size_t generate_roads(MapRoad out[MAX_ROADS], unsigned char tiles[MAP_SIZE][MAP_SIZE], uint8_t scale) {
	size_t n = 0;

	for (size_t z = 0; z < MAP_SIZE; z++)
	for (size_t x = 0; x < MAP_SIZE; x++)
	{
		if (tiles[z][x] == TILE_ROAD) {
			out[n].position = (Vector3){ x * scale, 0, z * scale};

			// Count connections
			out[n].yaw = GetRandomValue(0, 360);

			n++;
		}
	}

	return n;
}

static void generate_layout(unsigned char tiles[MAP_SIZE][MAP_SIZE]) {
	// Init all tiles to TILE_EMPTY
	memset(tiles, TILE_EMPTY, sizeof(unsigned char) * MAP_SIZE * MAP_SIZE);

	// Generate rows and columns
	int step = MAP_SIZE / ROAD_LINES;
	int jitter = step / 3;  // Maximum jitter either side

	int roads_x[ROAD_LINES]; int roads_z[ROAD_LINES];

	for (size_t i = 0; i < ROAD_LINES; i++) {
		roads_x[i] = i * step + GetRandomValue(-jitter, jitter);
		roads_z[i] = i * step + GetRandomValue(-jitter, jitter);

		if (roads_x[i] < 0) roads_x[i] = 0;
		if (roads_z[i] < 0) roads_z[i] = 0;
	}

	// Paint road lines onto map
	for (size_t i = 0; i < ROAD_LINES; i++) {
		for (size_t x = 0; x < MAP_SIZE; x++) {
			tiles[roads_z[i]][x] = TILE_ROAD;
		}
		for (size_t z = 0; z < MAP_SIZE; z++) {
			tiles[z][roads_x[i]] = TILE_ROAD;
		}
	}
}

void map_free(Map *map) {
	map_free_assets(&map->assets);
	free(map);
}

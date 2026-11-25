#include "map/mapgen.h"
#include "map/assets.h"
#include "map/constants.h"
#include "map/types.h"

#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define ROAD_LINES (MAP_SIZE / 4)

static void generate_layout(MapTile tiles[MAP_SIZE][MAP_SIZE]);
static size_t generate_roads(MapRoad out[MAX_ROADS], MapTile tiles[MAP_SIZE][MAP_SIZE], uint8_t scale);
static inline MapTile get_rel_tile(MapTile tiles[MAP_SIZE][MAP_SIZE], int8_t x, int8_t z, int8_t dx, int8_t dz);

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

	// Using the layout, generate the road structs
	out->roads_n = generate_roads(out->roads, out->tiles, scale);

	// Load assets
	map_load_assets(&out->assets);

	return out;
}

// Parse the tile map and translate all roads to MapRoad structs
// Returns the amount of roads
static size_t generate_roads(MapRoad out[MAX_ROADS], MapTile tiles[MAP_SIZE][MAP_SIZE], uint8_t scale) {
	size_t n = 0;

	for (size_t z = 0; z < MAP_SIZE; z++)
	for (size_t x = 0; x < MAP_SIZE; x++)
	{
		if (tiles[z][x] == TILE_ROAD) {
			out[n].position = (Vector3){ x * scale, 0, z * scale};
			// Count connections

			// Get adjacent tiles
			MapTile adj[] = {
				get_rel_tile(tiles, x, z, 0, 1) == TILE_ROAD,   // Right
				get_rel_tile(tiles, x, z, 0, -1) == TILE_ROAD,  // Left
				get_rel_tile(tiles, x, z, 1, 0) == TILE_ROAD,   // Up
				get_rel_tile(tiles, x, z, -1, 0) == TILE_ROAD   // Down
			};

			for (size_t i = 0; i < 4; i++) {
				if (adj[i]) out[n].connections++;
			}

			switch (out[n].connections) {
			case 1:
				if (adj[0]) out[n].yaw = 270;  // Right
				if (adj[1]) out[n].yaw = 90;  // Left
				if (adj[2]) out[n].yaw = 0;  // Up
				if (adj[3]) out[n].yaw = 180;  // Down
				break;
			case 2:
				if (adj[0] && adj[1]) out[n].yaw = 90;  // Right && Left
				else if (adj[2] && adj[3]) out[n].yaw = 0;  // Up && Down
				else NULL;  // TODO: Bends
				break;
			case 3:
				if (adj[0] && adj[1]) {  // Right && Left
					if (adj[2]) out[n].yaw = 90;  // Facing up
					else out[n].yaw = 270;  // Facing down
				} else if (adj[2] && adj[3]) {  // Up && down
					if (adj[0]) out[n].yaw = 0;  // Facing right
					else out[n].yaw = 180;
				}
				break;
			default: break;
			}

			n++;
		}
	}

	return n;
}

static inline MapTile get_rel_tile(MapTile tiles[MAP_SIZE][MAP_SIZE], int8_t x, int8_t z, int8_t dx, int8_t dz) {
	x += dx;
	z += dz;

	if (x < 0 || x >= MAP_SIZE) return TILE_EMPTY;
	if (z < 0 || z >= MAP_SIZE) return TILE_EMPTY;

	return tiles[z][x];
}

static void generate_layout(MapTile tiles[MAP_SIZE][MAP_SIZE]) {
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

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

static MapTile* generate_layout(size_t size);
static void generate_roads(Map *m);
static inline MapTile get_rel_tile(size_t map_size, MapTile *tiles, int8_t x, int8_t z, int8_t dx, int8_t dz);
static inline MapTile tile_at(size_t map_size, MapTile *tiles, size_t x, size_t z);
static void generate_buildings(Map *m);

Map* mapgen(unsigned int seed, size_t size, uint8_t scale) {
	Map *out = malloc(sizeof(Map));
	if (out == NULL) {
		fprintf(stderr, "Couldn't malloc map\n");
		return NULL;
	}
	out->size = size;
	out->scale = scale;

	// Set seed
	srand(seed);

	// Generate layout into out->tiles
	out->tiles = generate_layout(size);
	if (out->tiles == NULL) {
		fprintf(stderr, "Couldn't malloc layout\n");
		free(out);
		return NULL;
	}

	out->structures = malloc(sizeof(MapStructure) * size * size);
	out->n_structures = 0;

	// Using the layout, generate the road structs
	generate_roads(out);

	// Generate buildings
	generate_buildings(out);

	// Load assets
	map_load_assets(&out->assets);

	return out;
}

static void generate_buildings(Map *m) {
	for (size_t z = 0; z < m->size; z++)
	for (size_t x = 0; x < m->size; x++)
	{
		if (tile_at(m->size, m->tiles, x, z) == TILE_EMPTY) {
			m->structures[m->n_structures].position = (Vector3){ x * m->scale, 0, z * m->scale };
			m->structures[m->n_structures].asset = ASSET_BUILDING_1;
			m->n_structures++;
		}
	}
}

// Parse the tile map and translate all roads to MapStructure structs
static void generate_roads(Map *m) {
	for (size_t z = 0; z < m->size; z++) {
		for (size_t x = 0; x < m->size; x++) {
			if (tile_at(m->size, m->tiles, x, z) == TILE_ROAD) {
				MapStructure *road = &m->structures[m->n_structures];
				road->position = (Vector3){ x * m->scale, 0, z * m->scale };

				// Get adjacent tiles
				bool adj[4] = {
					get_rel_tile(m->size, m->tiles, x, z, 0, 1) == TILE_ROAD,   // Right (+Z)
					get_rel_tile(m->size, m->tiles, x, z, 0, -1) == TILE_ROAD,  // Left (-Z)
					get_rel_tile(m->size, m->tiles, x, z, 1, 0) == TILE_ROAD,   // Up (+X)
					get_rel_tile(m->size, m->tiles, x, z, -1, 0) == TILE_ROAD   // Down (-X)
				};

				size_t connections = 0;
				for (size_t i = 0; i < 4; i++) {
					if (adj[i]) connections++;
				}

				// Rotate road model
				switch (connections) {
				case 1:  // Dead-end road
					road->asset = ASSET_ROAD_END;

					if (adj[0]) road->yaw = 270;  // Right
					if (adj[1]) road->yaw = 90;   // Left
					if (adj[2]) road->yaw = 0;    // Up
					if (adj[3]) road->yaw = 180;  // Down
					break;
				case 2:  // 2-way connected road
					if (adj[0] && adj[1]) {
						road->asset = ASSET_ROAD_STRAIGHT;
						road->yaw = 90;   // Right && Left
					} else if (adj[2] && adj[3]) {
						road->asset = ASSET_ROAD_STRAIGHT;
						road->yaw = 0;    // Up && Down
					} else {
						road->asset = ASSET_ROAD_BEND;
						if (adj[0] && adj[2]) road->yaw = 90;       // +Z and +X
						else if (adj[2] && adj[1]) road->yaw = 180; // +X and -Z
						else if (adj[1] && adj[3]) road->yaw = 270; // -Z and -X
						else if (adj[3] && adj[0]) road->yaw = 0;   // -X and +Z
					}
					break;
				case 3:  // 3-way intersection
					road->asset = ASSET_ROAD_INTERSECTION;

					if (adj[0] && adj[1]) {  // Right && Left
						if (adj[2]) road->yaw = 90;    // Facing up
						else road->yaw = 270;          // Facing down
					} else if (adj[2] && adj[3]) {  // Up && Down
						if (adj[0]) road->yaw = 0;     // Facing right
						else road->yaw = 180;          // Facing left
					}
					break;
				case 4:  // Crossroads
					road->asset = ASSET_ROAD_CROSSROADS;
					road->yaw = 0;
					break;
				default:
					road->yaw = 0;
					break;
				}

				m->n_structures++;
			}
		}
	}
}

static inline MapTile get_rel_tile(size_t map_size, MapTile *tiles, int8_t x, int8_t z, int8_t dx, int8_t dz) {
	x += dx;
	z += dz;

	return tile_at(map_size, tiles, x, z);
}

static MapTile* generate_layout(size_t size) {
	MapTile *out = malloc(sizeof(MapTile) * size * size);
	if (out == NULL) {
		return NULL;
	}

	// Init all tiles to TILE_EMPTY
	memset(out, TILE_EMPTY, sizeof(MapTile) * size * size);

	const size_t road_lines = size / 4;

	// Generate rows and columns
	uint step = size / road_lines;
	int jitter = step / 3;  // Maximum jitter either side

	size_t roads_x[road_lines]; size_t roads_z[road_lines];

	for (size_t i = 0; i < road_lines; i++) {
		roads_x[i] = i * step + GetRandomValue(-jitter, jitter);
		roads_z[i] = i * step + GetRandomValue(-jitter, jitter);

		if (roads_x[i] < 0 || roads_x[i] >= size) roads_x[i] = 0;
		if (roads_z[i] < 0 || roads_z[i] >= size) roads_z[i] = 0;
	}

	// Paint road lines onto map
	for (size_t i = 0; i < road_lines; i++) {
		for (size_t x = 0; x < size; x++) {
			// out[roads_z[i] * size + x]
			out[roads_z[i] * size + x] = TILE_ROAD;
		}
		for (size_t z = 0; z < size; z++) {
			// out[z][roads_x[i]]
			out[z * size + roads_x[i]] = TILE_ROAD;
		}
	}

	return out;
}

void map_free(Map *map) {
	free(map->tiles);
	free(map->structures);

	map_free_assets(&map->assets);
	free(map);
}

static inline MapTile tile_at(size_t map_size, MapTile *tiles, size_t x, size_t z) {
	if (x < 0 || x >= map_size) return TILE_EMPTY;
	if (z < 0 || z >= map_size) return TILE_EMPTY;

	return tiles[z * map_size + x];
}
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <raylib.h>

typedef uint16_t ModelID;
#define MODEL_NONE ((ModelID)UINT16_MAX)

typedef enum { TEXTURE_ROAD, TEXTURE_BUILDING, TEXTURE_COUNT } TextureID;

typedef enum {
	POOL_ROAD_STRAIGHT,
	POOL_ROAD_BEND,
	POOL_ROAD_INTERSECTION,
	POOL_ROAD_CROSSROADS,
	POOL_ROAD_END,
	POOL_BUILDING,

	POOL_COUNT
} AssetPoolID;

typedef struct {
	ModelID *ids;
	size_t count;
} AssetPool;

typedef struct {
	Model *models;
	size_t n_models;

	Texture2D *textures;
	size_t n_textures;

	AssetPool pools[POOL_COUNT];

	bool loaded;
} Assets;


bool assets_load(Assets *a);
void assets_free(Assets *a);

const Model *assets_get_model(const Assets *a, ModelID id);
ModelID assets_pick(const Assets *a, AssetPoolID pool, uint32_t rng);
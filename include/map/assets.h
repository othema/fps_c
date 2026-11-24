#pragma once

#include <raylib.h>

#define N_MODELS 4
#define N_TEXTURES 1

typedef enum {
	ASSET_ROAD_STRAIGHT,
	ASSET_ROAD_BEND,
	ASSET_ROAD_INTERSECTION,
	ASSET_ROAD_CROSSROADS
} MapAsset;

enum {
	MODEL_TEXTURE_ROAD
};

typedef struct {
	Model models[N_MODELS];
	Texture2D textures[N_TEXTURES];
} MapAssets;

void map_load_assets(MapAssets *assets);
void map_free_assets(MapAssets *assets);
Model* map_get_asset(MapAssets *assets, MapAsset asset);

#pragma once

#include <raylib.h>

#define N_MODELS 10
#define N_TEXTURES 2

typedef enum {
	ASSET_ROAD_STRAIGHT,
	ASSET_ROAD_BEND,
	ASSET_ROAD_INTERSECTION,
	ASSET_ROAD_CROSSROADS,
	ASSET_ROAD_END,

	ASSET_BUILDING_1,
	ASSET_BUILDING_2,
	ASSET_BUILDING_3,
	ASSET_BUILDING_4,
	ASSET_BUILDING_5
} MapAsset;

enum {
	MODEL_TEXTURE_ROAD,
	MODEL_TEXTURE_BUILDING
};

typedef struct {
	Model models[N_MODELS];
	Texture2D textures[N_TEXTURES];
} MapAssets;

void map_load_assets(MapAssets *assets);
void map_free_assets(MapAssets *assets);
Model* map_get_asset(MapAssets *assets, MapAsset asset);

#include "map/assets.h"

#include <raylib.h>
#include <string.h>

static Model load(const char *model_path, Texture2D *tex);

void map_load_assets(MapAssets *assets) {
	assets->textures[MODEL_TEXTURE_ROAD] = LoadTexture("assets/kenney_roads/Models/GLB format/Textures/colormap.png");

	assets->models[ASSET_ROAD_STRAIGHT] = load("assets/kenney_roads/Models/GLB format/road-straight.glb", &assets->textures[MODEL_TEXTURE_ROAD]);
	assets->models[ASSET_ROAD_BEND] = load("assets/kenney_roads/Models/GLB format/road-bend.glb", &assets->textures[MODEL_TEXTURE_ROAD]);
	assets->models[ASSET_ROAD_INTERSECTION] = load("assets/kenney_roads/Models/GLB format/road-intersection.glb", &assets->textures[MODEL_TEXTURE_ROAD]);
	assets->models[ASSET_ROAD_CROSSROADS] = load("assets/kenney_roads/Models/GLB format/road-crossroad.glb", &assets->textures[MODEL_TEXTURE_ROAD]);
	assets->models[ASSET_ROAD_END] = load("assets/kenney_roads/Models/GLB format/road-end.glb", &assets->textures[MODEL_TEXTURE_ROAD]);
}

void map_free_assets(MapAssets *assets) {
	for (size_t i = 0; i < N_MODELS; i++) {
		UnloadModel(assets->models[i]);
	}
	for (size_t i = 0; i < N_TEXTURES; i++) {
		UnloadTexture(assets->textures[i]);
	}
}

Model* map_get_asset(MapAssets *assets, MapAsset asset) {
	return &assets->models[asset];
}

static Model load(const char *model_path, Texture2D *tex) {
	Model m = LoadModel(model_path);
	SetMaterialTexture(&m.materials[0], MATERIAL_MAP_ALBEDO, *tex);
	return m;
}
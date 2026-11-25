#include "assets.h"

#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

typedef struct { const char *path; AssetPoolID pool; TextureID texture; } ModelDescriptor;
typedef struct { const char *path; } TextureDescriptor;

static const TextureDescriptor TEXTURE_DESCRIPTORS[] = {
	[TEXTURE_ROAD] = { "assets/kenney_roads/Models/GLB format/Textures/colormap.png" },
	[TEXTURE_BUILDING] = { "assets/kenney_city/Models/GLB format/Textures/colormap.png" }
};

static const ModelDescriptor MODEL_DESCRIPTORS[] = {
	// Roads
	{ "assets/kenney_roads/Models/GLB format/road-straight.glb", POOL_ROAD_STRAIGHT, TEXTURE_ROAD },
	{ "assets/kenney_roads/Models/GLB format/road-bend.glb", POOL_ROAD_BEND, TEXTURE_ROAD },
	{ "assets/kenney_roads/Models/GLB format/road-intersection.glb", POOL_ROAD_INTERSECTION, TEXTURE_ROAD },
	{ "assets/kenney_roads/Models/GLB format/road-crossroad.glb", POOL_ROAD_CROSSROADS, TEXTURE_ROAD },
	{ "assets/kenney_roads/Models/GLB format/road-end.glb", POOL_ROAD_END, TEXTURE_ROAD },
	
	// Buildings
	{ "assets/kenney_city/Models/GLB format/building-a.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-b.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-c.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-d.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-e.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-f.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-g.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-h.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-skyscraper-a.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-skyscraper-b.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-skyscraper-c.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-skyscraper-d.glb", POOL_BUILDING, TEXTURE_BUILDING },
	{ "assets/kenney_city/Models/GLB format/building-skyscraper-e.glb", POOL_BUILDING, TEXTURE_BUILDING }
};

bool assets_load(Assets *a) {
	memset(a, 0, sizeof(Assets));

	/* Texture & model loading */

	// Malloc textures
	a->n_textures = TEXTURE_COUNT;
	a->textures = malloc(sizeof(Texture2D) * a->n_textures);
	if (a->textures == NULL) {
		fprintf(stderr, "Couldn't malloc textures\n");
		return false;
	}

	// Load textures
	for (size_t i = 0; i < a->n_textures; i++) {
		a->textures[i] = LoadTexture(TEXTURE_DESCRIPTORS[i].path);
	}

	// Malloc models
	a->n_models = ARRAY_LEN(MODEL_DESCRIPTORS);
	a->models = malloc(sizeof(Model) * a->n_models);
	if (a->models == NULL) {
		fprintf(stderr, "Couldn't malloc models\n");
		assets_free(a);
		return false;
	}

	// Load models
	for (size_t i = 0; i < a->n_models; i++) {
		a->models[i] = LoadModel(MODEL_DESCRIPTORS[i].path);

		if (a->models[i].materialCount > 0)
			SetMaterialTexture(&a->models[i].materials[0], MATERIAL_MAP_ALBEDO, a->textures[MODEL_DESCRIPTORS[i].texture]);
	}

	/* Asset pooling */

	// Count the size of each pool
	size_t counts[POOL_COUNT] = { 0 };
	for (size_t i = 0; i < a->n_models; i++)
		counts[MODEL_DESCRIPTORS[i].pool]++;

	// Allocate pool arrays
	for (size_t i = 0; i < POOL_COUNT; i++) {
		a->pools[i].count = 0;  // This doubles as the write cursor when filling the pool array
		a->pools[i].ids = counts[i] ? malloc(sizeof(ModelID) * counts[i]) : NULL;
		if (counts[i] > 0 && a->pools[i].ids == NULL) {
			assets_free(a);
			return false;
		};
	}

	// Fill pool arrays
	for (size_t i = 0; i < a->n_models; i++) {
		AssetPool *pool = &a->pools[MODEL_DESCRIPTORS[i].pool];
		pool->ids[pool->count++] = (ModelID)i;
	}

	a->loaded = true; return true;
}

void assets_free(Assets *a) {
	if (a->models) {
		for (size_t i = 0; i < a->n_models; i++)  UnloadModel(a->models[i]);
		free(a->models);
	}

	if (a->textures) {
		for (size_t i = 0; i < a->n_textures; i++) UnloadTexture(a->textures[i]);
		free(a->textures);
	}

	// Free pool arrays
	for (size_t i = 0; i < POOL_COUNT; i++) free(a->pools[i].ids);

	memset(a, 0, sizeof(Assets));
	a->loaded = false;
}

ModelID assets_pick(const Assets *a, AssetPoolID pool, uint32_t rng) {
	if (!a || pool >= POOL_COUNT) return MODEL_NONE;
	const AssetPool *p = &a->pools[pool];
	if (p->count == 0) return MODEL_NONE;
	return p->ids[rng % p->count];
}

const Model* assets_get_model(const Assets *a, ModelID id) {
	if (!a || !a->models || id >= a->n_models) return NULL;

	return &a->models[id];
}
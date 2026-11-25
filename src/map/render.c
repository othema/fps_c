#include "map/render.h"
#include "map/types.h"

#include "assets.h"

#include <stdlib.h>
#include <raylib.h>

void map_render(Vector3 origin, Map *map, Assets *assets) {
	// Render roads
	for (size_t i = 0; i < map->n_structures; i++) {
		MapStructure *r = &map->structures[i];

		DrawModelEx(
			*assets_get_model(assets, r->model),
			map->structures[i].position,
			(Vector3){ 0, map->structures[i].yaw, 0 },
			map->structures[i].yaw,
			(Vector3){ map->scale, map->scale, map->scale },
			WHITE
		);
	}

	// // Render buildings
	// DrawModelEx(
	// 	*map_get_asset(&map->assets, ASSET_BUILDING_1),
	// 	(Vector3){ 0, 0, 0 },
	// 	(Vector3){ 0, 0, 0 },
	// 	0,
	// 	(Vector3){ map->scale, map->scale, map->scale },
	// 	WHITE
	// );
}
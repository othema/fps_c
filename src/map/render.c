#include "map/render.h"
#include "map/assets.h"
#include "map/types.h"

#include <stdlib.h>
#include <raylib.h>

void map_render(Vector3 origin, Map *map) {
	for (size_t i = 0; i < map->roads_n; i++) {
		MapRoad *r = &map->roads[i];

		MapAsset asset;

		switch (r->connections) {
		case 1: asset = ASSET_ROAD_END; break;
		case 2: asset = ASSET_ROAD_STRAIGHT; break;
		case 3: asset = ASSET_ROAD_INTERSECTION; break;
		case 4: asset = ASSET_ROAD_CROSSROADS; break;
		default: asset = ASSET_ROAD_BEND; break;
		}

		DrawModelEx(
			*map_get_asset(&map->assets, asset),
			map->roads[i].position,
			(Vector3){ 0, map->roads[i].yaw, 0 },
			map->roads[i].yaw,
			(Vector3){ map->scale, map->scale, map->scale },
			WHITE
		);
	}
}
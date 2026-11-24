#include "map.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

Map *map_load_from_file(const char *filename) {
	FILE *fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Couldn't open file: %s\n", filename);
		return NULL;
	}

	Map *out = malloc(sizeof(Map));
	if (out == NULL) {
		fprintf(stderr, "Couldn't malloc map\n");
		fclose(fptr);
		return NULL;
	}

	size_t data_size = MIN_MAP_SIZE;
	out->data = malloc(sizeof(char) * data_size);
	if (out->data == NULL) {
		fprintf(stderr, "Couldn't malloc map data\n");
		free(out);
		fclose(fptr);
		return NULL;
	}

	out->map_size = 0;
	out->map_row_size = 0;

	char c;
	while ((c = fgetc(fptr)) != EOF) {
		// Calculate map_row_size based on size of first line
		if (out->map_row_size == 0) {
			if (c == '\n') out->map_row_size = out->map_size;
		}

		if (!isdigit(c)) continue;  // Only deal with numbers

		// Realloc out ptr if about to overflow
		if (out->map_size >= data_size) {
			data_size += MIN_MAP_SIZE;
			out->data = realloc(out->data, sizeof(char) * data_size);
		}

		out->data[out->map_size++] = (int)c - 48;  // '0' is 48 in ASCII
	}

	fclose(fptr);

	return out;
}

void map_free(Map *map) {
	free(map->data);
	free(map);
}
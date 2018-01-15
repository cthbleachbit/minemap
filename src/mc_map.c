# include <stdint.h>
# include <string.h>
# include <stdint.h>

# include "mc_map.h"
# include "mc_nbt.h"
# include "utils.h"

// Init map and nbt from parameter
mc_map *init_map(char scale, char dim, int16_t width, int16_t height, int32_t xC, int32_t yC) {
	mc_map *map = protected_malloc(sizeof(mc_map));
	map -> scale = init_byte("scale");
	map -> scale -> payload = scale;
	map -> dimension = init_byte("dimension");
	map -> dimension -> payload = dim;
	map -> width = init_short("width");
	set_short(map -> width, width);
	map -> height = init_short("height");
	set_short(map -> height, height);
	map -> xCenter = init_int("xCenter");
	set_int(map -> xCenter, xC);
	map -> yCenter = init_int("yCenter");
	set_int(map -> yCenter, yC);
	map -> colors = init_byte_array("colors", 16384);
	return map;
}

void free_map(mc_map *map) {
	free(map -> scale);
	free(map -> dimension);
	free(map -> width);
	free(map -> height);
	free(map -> xCenter);
	free(map -> yCenter);
	free_byte_array(map -> colors);
}

// Convert to byte sequence
size_t size_map_raw(mc_map *map) {
	size_t base = map_header_size + map_footer_size;
	base += size_byte_nbt(map -> scale);
	base += size_byte_nbt(map -> dimension);
	base += size_short_nbt(map -> width);
	base += size_short_nbt(map -> height);
	base += size_int_nbt(map -> xCenter);
	base += size_int_nbt(map -> yCenter);
	base += size_byte_array_nbt(map -> colors);
	return base;
}

void generate_map_raw(unsigned char *output, mc_map *map) {
	size_t index = 0;
	memcpy(output, map_header, map_header_size);
	index += map_header_size;

	generate_byte_nbt(output + index, map -> scale);
	index += size_byte_nbt(map -> scale);
	generate_byte_nbt(output + index, map -> dimension);
	index += size_byte_nbt(map -> dimension);

	generate_short_nbt(output + index, map -> width);
	index += size_short_nbt(map -> width);
	generate_short_nbt(output + index, map -> height);
	index += size_short_nbt(map -> width);

	generate_int_nbt(output + index, map -> xCenter);
	index += size_int_nbt(map -> xCenter);
	generate_int_nbt(output + index, map -> yCenter);
	index += size_int_nbt(map -> yCenter);

	generate_byte_array_nbt(output + index, map -> colors);
	index += size_byte_array_nbt(map -> colors);

	memcpy(output + index, map_footer, map_footer_size);
}


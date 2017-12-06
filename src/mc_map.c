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

// Convert to byte sequence
unsigned char *output_raw(mc_map *map) {
	uint8_t *output = protected_calloc(1, sizeof(uint8_t));
	return output;
}

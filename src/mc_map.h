# include "mc_nbt.h"

# ifndef MM_MCMAP_H
# define MM_MCMAP_H

static unsigned char map_header[] = {
		0x0a, 0x00, 0x00, // root compound tag without name
		// compound tag named data
		0x0a, 0x00, 0x04, 0x64, 0x61, 0x74, 0x61};

static size_t map_header_size = 10;

static unsigned char map_footer[] = {
		0x00, 0x00};

static size_t mao_footer_size = 2;

typedef struct {
	nbt_byte *scale;
	nbt_byte *dimension;
	nbt_short *width;
	nbt_short *height;
	nbt_int *xCenter;
	nbt_int *yCenter;
	nbt_byte_array *colors;
} mc_map;

// Init map and nbt from parameter
mc_map *init_map(char scale, char dim, int16_t width, int16_t height, int32_t xC, int32_t yC);

// Convert to byte sequence
unsigned char *output_raw(mc_map *);

# endif

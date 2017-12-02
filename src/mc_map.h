# include "mc_nbt.h"

# ifndef MM_MCMAP_H
# define MM_MCMAP_H

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
mc_map *init_map(char scale, char dim, short width, short height, int xC, int yC);

// Convert to byte sequence
char *output_raw(mc_map *);

# endif

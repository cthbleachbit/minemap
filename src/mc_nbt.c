# include <stdlib.h>
# include "mc_nbt.h"
# include "endian.h"
# include "utils.h"

nbt_byte *init_byte(char *name) {
	nbt_byte *nbt = protected_malloc(sizeof(nbt_byte));
	nbt -> name = name;
	return nbt;
}

void free_byte(nbt_byte *ptr) {
	free(ptr);
}

nbt_short *init_short(char *name) {
	nbt_short *nbt = protected_malloc(sizeof(nbt_short));
	nbt -> name = name;
	return nbt;
};

nbt_int *init_int(char *name) {
	nbt_int *nbt = protected_malloc(sizeof(nbt_int));
	nbt -> name = name;
	return nbt;
};
short get_short(nbt_short *ptr) {
	return 0;
}

nbt_byte_array *init_byte_array(char *name, size_t size);
void free_byte_array(nbt_byte_array *ptr);

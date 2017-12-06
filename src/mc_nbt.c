# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "mc_nbt.h"
# include "mc_endian.h"
# include "utils.h"

/* Two functions for manipulating nbt byte type
   Just init is enough
 */
nbt_byte *init_byte(char *name) {
	nbt_byte *nbt = protected_malloc(sizeof(nbt_byte));
	nbt -> name = name;
	return nbt;
}

/* Generate a nbt tag in byte array
 * Size defined as follows
 * 1 byte: type indicator
 * 2 byte: length of name in unsigned integer
 * X bytes: tag name
 * 1 byte: payload
 */
int8_t *generate_byte_nbt(nbt_byte *nbt) {
	int size = 3 + strlen(nbt -> name);
	int8_t *output = protected_calloc(size, sizeof(int8_t));
	output[0] = 0x01u;
	// TODO
}

/* For short, or int16_t, java uses big endian, so we need to do conversion

   init_short: creates pointer to a 2-byte nbt type in big endian
   get_short: get btyes from nbt and convert to little endian
   set_short: acctpes a little endian short and convert to big endian
 */
nbt_short *init_short(char *name) {
	nbt_short *nbt = protected_malloc(sizeof(nbt_short));
	nbt -> name = name;
	return nbt;
};

int16_t get_short(nbt_short *ptr) {
	short_u *conv = protected_malloc(sizeof(short_u));
	for (int i = 0; i < 2; i++) {
		conv -> payload[1 - i] = ptr -> payload[i];
	}
	int16_t ret = conv -> little;
	free(conv);
	return ret;
}

void set_short(nbt_short *ptr, int16_t payload) {
	fill_e_short(ptr -> payload, payload);
}

/* For int, or int32_t, java uses big endian, so we need to do conversion

   init_int: creates pointer to a 4-byte nbt type in big endian
   get_int: get btyes from nbt and convert to little endian
   set_int: acctpes a little endian int and convert to big endian
 */
nbt_int *init_int(char *name) {
	nbt_int *nbt = protected_malloc(sizeof(nbt_int));
	nbt -> name = name;
	return nbt;
};

int32_t get_int(nbt_int *ptr) {
	int_u *conv = protected_malloc(sizeof(int_u));
	for (int i = 0; i < 4; i++) {
		conv -> payload[3 - i] = ptr -> payload[i];
	}
	int32_t ret = conv -> little;
	free(conv);
	return ret;
}

void set_int(nbt_int *ptr, int32_t payload) {
	fill_e_int(ptr -> payload, payload);
}

/* Simple self-contained byte array nbt

   init_byte_array: returns a pointer to initialized fixed size byte array
   free_byte_array: gracefully destory a byte array
 */
nbt_byte_array *init_byte_array(char *name, size_t size) {
	nbt_byte_array *nbt = protected_malloc(sizeof(nbt_byte_array));
	nbt -> name = name;
	nbt -> size = size;
	nbt -> payload = protected_calloc(size, sizeof(unsigned char));
	return nbt;
};

void free_byte_array(nbt_byte_array *ptr) {
	free(ptr -> payload);
	free(ptr);
};

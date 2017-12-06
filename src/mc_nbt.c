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

/* Generate a byte type nbt tag in byte array
 * Size defined as follows
 * byte 0:
       type indicator 0x01
 * byte 1 to 2: 
       length of name in unsigned integer
 * byte 3 to 2 + name length:
       tag name
 * byte 3 + name_length:
       payload
 */
unsigned char *generate_byte_nbt(nbt_byte *nbt) {
	int name_length = strlen(nbt -> name);
	int size = 3 + name_length;
	unsigned char *output = protected_calloc(size, sizeof(unsigned char));
	output[0] = 0x01u;
	fill_e_short(output + 1, name_length);
	if(name_length) {
		memcpy(output + 3, nbt -> name, name_length);
	}
	output[3 + name_length] = nbt -> payload;
	return output;
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

/* Generate a short type nbt tag in byte array
 * Size defined as follows
 * byte 0:
       type indicator 0x02
 * byte 1 -> 2:
       length of name in unsigned integer
 * byte 3 -> 2 + name length:
 *     tag name
 * byte 3 + name_length to 4 + name length:
 *     payload
 */
unsigned char *generate_short_nbt(nbt_short *nbt) {
	int name_length = strlen(nbt -> name);
	int size = 4 + name_length;
	unsigned char *output = protected_calloc(size, sizeof(unsigned char));
	output[0] = 0x02u;
	fill_e_short(output + 1, name_length);
	if(name_length) {
		memcpy(output + 3, nbt -> name, name_length);
	}
	memcpy(output + 3 + name_length, nbt -> payload, 2);
	return output;
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

/* Generate a int type nbt tag in byte array
 * Size defined as follows
 * byte 0:
       type indicator 0x03
 * byte 1 -> 2:
       length of name in unsigned integer
 * byte 3 -> 2 + name length:
 *     tag name
 * byte 3 + name_length to 6 + name length:
 *     payload
 */
unsigned char *generate_int_nbt(nbt_int *nbt) {
	int name_length = strlen(nbt -> name);
	int size = 6 + name_length;
	unsigned char *output = protected_calloc(size, sizeof(unsigned char));
	output[0] = 0x03u;
	fill_e_short(output + 1, name_length);
	if(name_length) {
		memcpy(output + 3, nbt -> name, name_length);
	}
	memcpy(output + 3 + name_length, nbt -> payload, 4);
	return output;
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

/* Generate a byte array type nbt tag in byte array
 * Size defined as follows
 * byte 0:
       type indicator 0x07
 * byte 1 -> 2:
       length of name in unsigned integer
 * byte 3 -> 2 + name length:
 *     tag name
 * byte 3 + name_length to 6 + name length:
 *     length of payload, big endian integer
 * byte 7 + name length to 6 + name length + payload length:
 *     payload
 */
unsigned char *generate_byte_array_nbt(nbt_byte_array *nbt) {
	int name_length = strlen(nbt -> name);
	int payload_length = (nbt -> size);
	int size = 6 + name_length + payload_length;
	unsigned char *output = protected_calloc(size, sizeof(unsigned char));
	output[0] = 0x07u;
	fill_e_short(output + 1, name_length);
	if(name_length) {
		memcpy(output + 3, nbt -> name, name_length);
	}
	fill_e_int(output + 3 + name_length, payload_length);
	memcpy(output + 7 + name_length, nbt -> payload, payload_length);
	return output;
}


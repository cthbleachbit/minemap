# include <stddef.h>

# ifndef MM_NBT_H
# define MM_NBT_H

// a single byte, signed
typedef struct {
	char *name;
	signed char payload;
} nbt_byte;

// a 2-byte short integer, signed and in BIG ENDIAN
typedef struct {
	char *name;
	unsigned char payload[2];
} nbt_short;

// a 4-byte integer, signed and in BIG ENDIAN
typedef struct {
	char *name;
	unsigned char payload[4];
} nbt_int;

// a byte array
typedef struct {
	char *name;
	size_t size;
	unsigned char *payload;
} nbt_byte_array;

nbt_byte *init_byte(char *name);

nbt_short *init_short(char *name);
// To do endian conversions here.
short get_short(nbt_short *ptr);
void set_short(nbt_short *ptr, short payload);

nbt_int *init_int(char *name);
// To do endian conversions here.
int get_int(nbt_int *ptr);
void set_int(nbt_int *ptr, short payload);

nbt_byte_array *init_byte_array(char *name, size_t size);
void free_byte_array(nbt_byte_array *ptr);
# endif

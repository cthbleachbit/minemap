# ifndef MM_NBT_H
# define MM_NBT_H

// a single byte, signed
typedef struct {
	char *name;
	signed char nbt_byte;

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
void free_byte(nbt_byte *ptr);

nbt_short *init_short(char *name);
void free_short(nbt_short *ptr);

nbt_int *init_int(char *name);
void free_int(nbt_int *ptr);

nbt_byte_array *init_byte_array(char *name, size_t size);
void free_byte_array(nbt_byte_array *ptr);
# endif

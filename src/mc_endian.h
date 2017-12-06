# include <stdint.h>

# ifndef MC_ENDIAN
# define MC_ENDIAN

typedef union {
	unsigned char payload[2];
	int16_t little;
} short_u;

typedef union {
	unsigned char payload[4];
	int32_t little;
} int_u;

/* Deal with in-place value replacement.
 * Presumably not safe
 */
void fill_e_short(unsigned char *loc, int16_t input);
void fill_e_int(unsigned char *loc, int32_t input);
# endif

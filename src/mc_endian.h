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

# endif

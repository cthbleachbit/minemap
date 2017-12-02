# include <stdint.h>

# ifndef MC_ENDIAN
# define MC_ENDIAN

typedef union {
	char payload[2];
	int8_t little;
} short_u;

typedef union {
	char payload[2];
	int16_t little;
} int_u;

# endif

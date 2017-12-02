# ifndef MC_ENDIAN
# define MC_ENDIAN

typedef union {
	char[2] payload;
	short little;
} short_u;

# endif

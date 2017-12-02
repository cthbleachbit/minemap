# include <stdio.h>
# include <stdlib.h>

# ifndef MC_UTILS
# define MC_UTILS
void die(const int ret, char *errmsg) {
	fprintf(stderr, "%s", errmsg);
	exit(ret);
}

void* protected_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		die(127, "Memory allocation failed. Aborting.\n");
	}
	return ptr;
}

void* protected_calloc(size_t num, size_t size) {
	void *ptr = calloc(num, size);
	if (ptr == NULL) {
		die(127, "Memory allocation failed. Aborting.\n");
	}
	return ptr;
}

# endif

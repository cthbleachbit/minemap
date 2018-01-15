# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "utils.h"

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

FILE *protected_fopen(const char *pathname, const char *mode) {
	FILE *ptr = fopen(pathname, mode);
	if (ptr == NULL) {
		fprintf(stderr, "Error: File \"%s\" cannot be opened.\n", pathname);
		exit(1);
	} else {
		return ptr;
	}
}

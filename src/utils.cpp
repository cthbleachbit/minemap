/* Minemap
 *
 * util.cpp: Utilities
 */

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "utils.h"

void die(const int ret, const std::string &errmsg) {
	std::cerr << errmsg << std::endl;
	exit(ret);
}

void *protected_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		die(127, "Memory allocation failed. Aborting.");
	}
	return ptr;
}

void *protected_calloc(size_t num, size_t size) {
	void *ptr = calloc(num, size);
	if (ptr == NULL) {
		die(127, "Memory allocation failed. Aborting.");
	}
	return ptr;
}

FILE *protected_fopen(const std::string &pathname, const char *mode) {
	FILE *ptr = fopen(pathname.c_str(), mode);
	if (ptr == NULL) {
		perror(pathname.c_str());
		exit(1);
	}
	else {
		return ptr;
	}
}

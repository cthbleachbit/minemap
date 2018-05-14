/* Minemap
 *
 * util.h: Utilities
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef MC_UTILS
#define MC_UTILS

void die(const int ret, char *errmsg);
void *protected_malloc(size_t size);
void *protected_calloc(size_t num, size_t size);
FILE *protected_fopen(const char *pathname, const char *mode);

# endif

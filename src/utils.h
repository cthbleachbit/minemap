/* Minemap
 *
 * util.h: Utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef MC_UTILS
#define MC_UTILS

void die(const int ret, const std::string &errmsg);
void *protected_malloc(size_t size);
void *protected_calloc(size_t num, size_t size);
FILE *protected_fopen(const std::string &pathname, const char *mode);

# endif

/* Minemap
 * 
 * mc_endian.c: Big / Little Conversion
 */

#include <stdint.h>
#include <stdlib.h>
#include "mc_endian.h"
#include "utils.h"

/* Deal with in-place value replacement in byte sequences.
 * Presumably not safe
 */
void fill_e_short(unsigned char *loc, int16_t input) {
	short_u *conv = protected_malloc(sizeof(short_u));
	conv -> little = input;
	for (int i = 0; i < 2; i++) {
		loc[1 - i] = conv -> payload[i];
	}
	free(conv);
}
void fill_e_int(unsigned char *loc, int32_t input) {
	int_u *conv = protected_malloc(sizeof(int_u));
	conv -> little = input;
	for (int i = 0; i < 4; i++) {
		loc[3 - i] = conv -> payload[i];
	}
	free(conv);
}

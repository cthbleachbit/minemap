# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include "mc_endian.h"
# include "mc_nbt.h"

void test_16(int16_t payload) {
	nbt_short *nbt_s1 = init_short("Test");
	short_u *s1 = malloc(sizeof(short_u));
	s1 -> little = payload;
	set_short(nbt_s1, payload);
	unsigned char *be = nbt_s1 -> payload;
	assert(be[0] == s1 -> payload[1]);
	assert(be[1] == s1 -> payload[0]);
	free(nbt_s1);
	free(s1);
}

int main() {
	test_16(0x1234);
	test_16(0x4854);
	test_16(0xfbaa);
	test_16(0xffff);
}

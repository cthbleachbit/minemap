# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include "mc_endian.h"
# include "mc_nbt.h"

void test_32(int32_t payload) {
	nbt_int *nbt_i1 = init_int("Test");
	int_u *i1 = malloc(sizeof(int_u));
	i1 -> little = payload;
	set_int(nbt_i1, payload);
	unsigned char *be = nbt_i1 -> payload;
	for (int i = 0; i < 4; i++) {
		assert(be[i] == i1 -> payload[3-i]);
	}
	free(nbt_i1);
	free(i1);
}

int main() {
	test_32(0x1234ffff);
	test_32(0x482ab354);
	test_32(0x0000fbaa);
	test_32(0xffff0000);
}

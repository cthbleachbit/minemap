# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mc_endian.h"
# include "mc_nbt.h"

unsigned char short_reference[] = 
		{0x02, // short type
		0x00, 0x06, // name length 6 in big endian
		0x68, 0x65, 0x69, 0x67, 0x68, 0x74, // "height"
		0x00, 0x80}; //payload = 128

int main() {
	nbt_short *nbt = init_short("height");
	set_short(nbt, 0x80);
	unsigned char *bin_nbt = generate_short_nbt(nbt);
	for (int i = 0; i < (signed int) sizeof(short_reference); i++) {
		printf("%x\t%x\n", bin_nbt[i], short_reference[i]);
	}
	return memcmp(bin_nbt, short_reference, sizeof(short_reference));
}

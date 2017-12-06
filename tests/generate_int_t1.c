# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mc_endian.h"
# include "mc_nbt.h"

unsigned char int_reference[] = 
		{0x03, // int type
		0x00, 0x07, // name length 7 in big endian
		0x78, 0x43, 0x65, 0x6e, 0x74, 0x65, 0x72,// "xCenter"
		0x05, 0xf5, 0xe1, 0x00}; //payload = 100000000

int main() {
	nbt_int *nbt = init_int("xCenter");
	set_int(nbt, 100000000);
	unsigned char *bin_nbt = generate_int_nbt(nbt);
	for (int i = 0; i < (signed int) sizeof(int_reference); i++) {
		printf("%x\t%x\n", bin_nbt[i], int_reference[i]);
	}
	return memcmp(bin_nbt, int_reference, sizeof(int_reference));
}

# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mc_endian.h"
# include "mc_nbt.h"

unsigned char byte_scale_reference[9] = 
		{0x01, // byte type
		0x00, 0x05, // name length 5 in big endian
		0x73, 0x63, 0x61, 0x6c, 0x65, // "scale"
		0x01}; //payload = 1

int main() {
	nbt_byte *nbt = init_byte("scale");
	nbt -> payload = 0x01;
	unsigned char *bin_nbt = generate_byte_nbt(nbt);
	for (int i = 0; i < (signed int) sizeof(byte_scale_reference); i++) {
		printf("%x\t%x\n", bin_nbt[i], byte_scale_reference[i]);
	}
	return memcmp(bin_nbt, byte_scale_reference, sizeof(byte_scale_reference));
}

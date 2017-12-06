# include <assert.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mc_endian.h"
# include "mc_nbt.h"

unsigned char reference[] = 
		{0x07, // byte type
		0x00, 0x06, // name length 6 in big endian
		0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x73, // "colors"
		0x00, 0x00, 0x00, 0x10, // payload length 16 in big endian
		// in this case starts at offset +13
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

int main() {
	nbt_byte_array *nbt = init_byte_array("colors", 16);
	memcpy(nbt -> payload, reference + 13, 16);
	unsigned char *bin_nbt = generate_byte_array_nbt(nbt);
	for (int i = 0; i < (signed int) sizeof(reference); i++) {
		printf("%x\t%x\n", bin_nbt[i], reference[i]);
	}
	return memcmp(bin_nbt, reference, sizeof(reference));
}

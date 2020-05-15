/* Minemap
 *
 * pamenim: Main program: convert map to png
 */

#include <cstdio>

void usage() {
	printf("pamenim <options>\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, gzipped nbt input\n");
	printf("\t-g, --game VER\n");
	printf("\t\tRequired, Minecraft game version this map is exported from\n");
	printf("\t\tSelect from the following values: \n");
	printf("\t\t\t1.12 for game version [1.12, +oo)\n");
	printf("\t\t\t1.8  for game version [1.8,  1.12)\n");
	printf("\t\tOther versions are not supported yet.\n");
	printf("\t-o, --output OUTPUT\n");
	printf("\t\tRequired, output picture\n");
}

int main(int argc, char **argv) {
	// STUB
	return 0;
}
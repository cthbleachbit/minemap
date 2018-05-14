/* Minemap
 *
 * pamenim: Main program: convert map to png
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ImageMagick-7/MagickCore/MagickCore.h>

#include "mc_map.h"
#include "utils.h"

char *map_buf;
ExceptionInfo exception;

char *ipath = NULL;
char *opath = NULL;

void usage() {
	printf("pamenim <options>\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, nbt input\n");
	printf("\t-o OUTPUT\n");
	printf("\t\tRequired, output picture\n");
}

int main(int argc, char **argv) {
	int i = 0;
	while (i < argc) {
		if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			ipath = argv[i];
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			i++;
			opath = argv[i];
		} else {
			usage();
			exit(1);
		}
	}
	if ((!ipath) || (!opath)) {
		usage();
		exit(1);
	}
	char *path = getcwd(path, 0);
	MagickCoreGenesis(path, MagickTrue);

	// Load Minecraft Map
	// Cleanup
	MagickCoreTerminus();
	free(path);
}

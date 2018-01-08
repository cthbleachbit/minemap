#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ImageMagick-7/MagickCore/MagickCore.h>

void usage() {
	printf("%s\n", "Usage:");
	printf("%s\n", "\tgen_color_map <input.txt> <output.png>");
}

int main(int argc, char **argv) {
	if (argc != 3) {
		usage();
		return 1;
	}
	char *input_filename = argv[1];
	char *output_filename = argv[2];
	FILE *input_file = fopen(input_filename, "r");
	if (input_file == NULL) {
		fprintf(stderr, "%s\n", "Fail to open file");
		return 1;
	}
	// Determine input file size
	fseek(input_file, 0L, SEEK_END);
	int size = ftell(input_file);
	rewind(input_file);
	// Initialize Magick
	char *path = getcwd(path, 0);
	MagickCoreGenesis(path, MagickTrue);
	// Create new image
	
	// End
	MagickCoreTerminus();
	free(path);
	fclose(input_file);
}

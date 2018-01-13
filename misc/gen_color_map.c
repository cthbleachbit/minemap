#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ImageMagick-7/MagickCore/MagickCore.h>

/* Used to convert text describing colors into gif palette
 *
 * This application is not designed with security in mind (and there is no 
 * checks and memfrees), and is onlt used to generate static gif files.
 */

void usage() {
	printf("%s\n", "Usage:");
	printf("%s\n", "\tgen_color_map <input.txt> <output.gif>");
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
	// Determine numer of lines in input
	char c;
	int lines = 0;
	while ((c = fgetc(input_file)) != EOF)
	{
		if (c == '\n') {
			lines++;
		}
	}
	rewind(input_file);
	printf("Number of colors in this file: %i\n", lines);
	
	// Initialize Magick
	char *path = getcwd(path, 0);
	MagickCoreGenesis(path, MagickTrue);
	
	// Parse to array of colors
	char *line_buffer = malloc(LINE_MAX);
	unsigned char *colors = malloc(sizeof(short) * 4 * lines);
	for (int i = 0; i < lines; i++) {
		fgets(line_buffer, LINE_MAX, input_file);
		unsigned char r, g, b, a;
		sscanf(line_buffer, "{%hhd, %hhd, %hhd, %hhd},", &r, &g, &b, &a);
		colors[i * 4] = r;
		colors[i * 4 + 1] = g;
		colors[i * 4 + 2] = b;
		colors[i * 4 + 3] = a;
		//printf("Index %i:\t%i\t%i\t%i\t%i\n", i , r, g, b, a);
	}
	
	// Create new image
	int height = lines / 4;
	printf("Generating 4 x %i palette.\n", height);
	ExceptionInfo *exception = malloc(sizeof(ExceptionInfo));
	exception->signature = MagickCoreSignature;
	Image *image = ConstituteImage(4, height, "RGBA", CharPixel, colors, exception);
	ImageInfo *info = CloneImageInfo(NULL);
	info->file = fopen(output_filename, "w+b");
	strcpy(info->filename, output_filename);
	strcpy(info->magick, "gif"); 
	WriteImage (info, image, exception);
	
	// End
	MagickCoreTerminus();
	free(path);
	fclose(input_file);
}

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <ImageMagick-7/MagickCore/MagickCore.h>

# include "utils.h"

int main(int argc, char **argv);
void usage();

void usage() {
	printf("minemap <palette.gif> <input image> <map_X.dat>\n");
	printf("\tpalette.gif: colors in specific minecraft version\n");
	printf("\tmap_X/dat: output in NBT format\n");
}

int main(int argc, char **argv) {
	if (argc != 4) {
		usage();
		return 0;
	}
	// Init Magick Core
	char *path = getcwd(path, 0);
	MagickCoreGenesis(path, MagickTrue);
	
	ExceptionInfo *exception = malloc(sizeof(ExceptionInfo));
	exception->signature = MagickCoreSignature;
	ImageInfo *palette_im_f = CloneImageInfo(NULL);
	palette_im_f->file = protected_fopen(argv[1], "r+b");
	ImageInfo *input_im_f = CloneImageInfo(NULL);
	input_im_f->file = fopen(argv[2], "r+b");
	Image *palette = ReadImage(palette_im_f, exception);
	
	MagickCoreTerminus();
	free(path);
	return 0;
}

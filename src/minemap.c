# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <zlib.h>
# include <ImageMagick-7/MagickCore/MagickCore.h>

# include "mc_map.h"
# include "utils.h"

char verbose = 0;
char no_gz = 0;
char floydsteinberg = 0;

int main(int argc, char **argv);
void usage();

void usage() {
	printf("Usage: minemap <options>\n");
	printf("\n");
	printf("-f, --floydsteinberg\n");
	printf("\tOptional, turn on Floyd Steinberg dithering\n");
	printf("\tDefault to no dithering.\n");
	printf("\n");
	printf("-i, --input INPUT\n");
	printf("\tRequired, image input\n");
	printf("\n");
	printf("--no-gz\n");
	printf("\tDo not gzip generated NBT file\n");
	printf("\n");
	printf("-o, --output FILE\n");
	printf("\tRequired, output file in NBT format\n");
	printf("\n");
	printf("-p, --palette PALETTE.GIF\n");
	printf("\tRequired, colors in specific minecraft version, installed to\n");
	printf("\t/usr/share/minemap/palettes\n");
	printf("\n");
	printf("-v, --verbose\n");
	printf("\tOptional, Turn on verbose output and store intermediate files\n");
	printf("\tinto /tmp for debug purposes\n");
}

int main(int argc, char **argv) {
	char *palette_path = NULL;
	char *input_path = NULL;
	char *output_path = NULL;
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--palette") == 0) {
			palette_path = argv[i+1];
			i++;
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			input_path = argv[i+1];
			i++;
		} else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--floydsteinberg") == 0) {
			floydsteinberg = 1;
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			output_path = argv[i+1];
			i++;
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "--no-gz") == 0) {
			no_gz = 1;
		}
		i++;
	}
	
	if ((!palette_path) || (!input_path) || (!output_path)) {
		usage();
		exit(1);
	};
	
	// Init Magick Core
	char *path = getcwd(path, 0);
	MagickCoreGenesis(path, MagickTrue);
	
	// Loading Image
	ExceptionInfo *exception = AcquireExceptionInfo();
	exception->signature = MagickCoreSignature;
	ImageInfo *palette_im_f = CloneImageInfo(NULL);
	palette_im_f->file = protected_fopen(palette_path, "rb");
	ImageInfo *input_im_f = CloneImageInfo(NULL);
	input_im_f->file = protected_fopen(input_path, "rb");
	Image *palette = ReadImage(palette_im_f, exception);
	CatchException(exception);
	
	Image *input = ReadImage(input_im_f, exception);
	CatchException(exception);
	
	// Scale input to 128x128
	Image *output = ResizeImage(input, 128, 128, LanczosFilter, exception);
	QuantizeInfo *quantize_info = CloneQuantizeInfo(NULL);
	if (floydsteinberg) {
		quantize_info->dither_method = FloydSteinbergDitherMethod;
	} else {
		quantize_info->dither_method = NoDitherMethod;
	}
	RemapImage(quantize_info, output, palette, exception);
	DestroyQuantizeInfo(quantize_info);
	
	if (verbose) {
		ImageInfo *dbg_img = CloneImageInfo(NULL);
		dbg_img->file = fopen("/tmp/remapped.gif", "w+b");
		strcpy(dbg_img->filename, "remapped.gif");
		strcpy(dbg_img->magick, "gif");
		WriteImage (dbg_img, output, exception);
		DestroyImageInfo(dbg_img);
	}
	
	PixelInfo *orig_pix = protected_malloc(sizeof(PixelInfo));
	PixelInfo *palette_pix = protected_malloc(sizeof(PixelInfo));
	int palette_col = palette->columns;
	int palette_row = palette->rows;
	int palette_number = palette_col * palette_row;
	mc_map *map = init_map(0, 1, 128, 128, 16777216, 16777216);
	unsigned char *map_payload = map -> colors -> payload;
	for (i = 0; i < 16384; i++) {
		GetOneVirtualPixelInfo(output,
		                       EdgeVirtualPixelMethod,
		                       i % 128,
		                       i / 128,
		                       orig_pix,
		                       exception);
		if (verbose) {
			fprintf(stderr, "Pixel at %i, %i:\n%f\t%f\t%f\t%f\n",
			        i % 128, i / 128,
			        orig_pix->red,
			        orig_pix->green,
			        orig_pix->blue,
			        orig_pix->alpha);
		}
		for (int j = 0; j < palette_number; j++) {
			GetOneVirtualPixelInfo(palette,
			                       EdgeVirtualPixelMethod,
			                       j % palette_col,
			                       j / palette_col,
			                       palette_pix,
			                       exception);
			if (orig_pix->red == palette_pix->red &&
			    orig_pix->green == palette_pix->green &&
			    orig_pix->blue == palette_pix->blue) {
				map_payload[i] = j;
				if (verbose) {
					fprintf(stderr, "Matched color id %i\n", j);
				}
				break;
			}
		}
	}
	
	// Close all Imagemagick handles
	DestroyImage(palette);
	DestroyImage(input);
	DestroyImage(output);
	DestroyImageInfo(palette_im_f);
	DestroyImageInfo(input_im_f);
	DestroyExceptionInfo(exception);
	
	// Generate map binary
	int size_map = size_map_raw(map);
	printf("Uncompressed map file size: %i bytes\n", size_map);
	unsigned char *map_binary = protected_malloc(size_map);
	generate_map_raw(map_binary, map);
	free_map(map);
	
	if (no_gz) {
		FILE *output_nbt = protected_fopen(output_path, "wb");
		fwrite(map_binary, 1, size_map, output_nbt);
		fclose(output_nbt);
	} else {
		gzFile output_gz = gzopen(output_path, "wb");
		gzwrite(output_gz, map_binary, size_map);
		gzclose(output_gz);
	}
	
	free(map_binary);
	MagickCoreTerminus();
	free(path);
	return 0;
}

/* Minemap
 *
 * minemap.c: Main program: convert picture to map
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>
#include <ImageMagick-7/MagickCore/MagickCore.h>

#include "mc_map.h"
#include "utils.h"

char verbose = 0;
char no_gz = 0;
DitherMethod dithering = NoDitherMethod;;

int main(int argc, char **argv);
void usage();

void usage() {
	printf("minemap <options>\n");
	printf("\t-d, --dithering ALGORITHM\n");
	printf("\t\tOptional, turn on dithering, available algorithms as follows\n");
	printf("\t\t\triemersma\n");
	printf("\t\t\tfloydsteinberg\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, image input\n");
	printf("\t--no-gz\n");
	printf("\t\tDo not gzip generated NBT file\n");
	printf("\t-o, --output FILE\n");
	printf("\t\tRequired, output file in NBT format\n");
	printf("\t-e, --export FILE\n");
	printf("\t\tOptional, export the result of color reduction in png format\n");
	printf("\t-p, --palette PALETTE.GIF\n");
	printf("\t\tRequired, colors in specific minecraft version\n");
	printf("\t-v, --verbose\n");
	printf("\t\tOptional, Turn on verbose output\n");
}

int main(int argc, char **argv) {
	char *palette_path = NULL;
	char *input_path = NULL;
	char *output_path = NULL;
	char *export_path = NULL;
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--palette") == 0) {
			i++;
			palette_path = argv[i];
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			input_path = argv[i];
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dithering") == 0) {
			i++;
			if (strcmp(argv[i], "riemersma") == 0) {
				dithering = RiemersmaDitherMethod;
			} else if (strcmp(argv[i], "floydsteinberg") == 0) {
				dithering = FloydSteinbergDitherMethod;
			} else {
				fprintf(stderr, "Unknown dithering method\n");
				exit(1);
			} 
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			i++;
			output_path = argv[i];
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0) {
			i++;
			export_path = argv[i];
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
	quantize_info->dither_method = dithering;
	RemapImage(quantize_info, output, palette, exception);
	DestroyQuantizeInfo(quantize_info);
	
	if (export_path) {
		ImageInfo *export_img = CloneImageInfo(NULL);
		export_img->file = fopen(export_path, "w+b");
		strcpy(export_img->filename, export_path);
		strcpy(export_img->magick, "png");
		WriteImage (export_img, output, exception);
		DestroyImageInfo(export_img);
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
	free(palette_pix);
	free(orig_pix);

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

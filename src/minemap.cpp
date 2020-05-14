/* Minemap
 *
 * minemap.cpp: Main program: convert picture to map
 */

#include <ImageMagick-7/MagickCore/MagickCore.h>
#include <nbtp/libnbtp.h>
#include <cstring>

#include <string>
#include <iostream>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <boost/iostreams/filtering_stream.hpp>

#include "utils.h"
#include "Map.h"

#define DEFAULT_X_CENTER 1 << 22
#define DEFAULT_Z_CENTER 1 << 22

bool verbose = false;

bool no_gz = false;

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
	std::string palette_path;
	std::string input_path;
	std::string output_path;
	std::string export_path;

	// Parse Parameters
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--palette") == 0) {
			i++;
			palette_path = argv[i];
		}
		else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			input_path = argv[i];
		}
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dithering") == 0) {
			i++;
			if (strcmp(argv[i], "riemersma") == 0) {
				dithering = RiemersmaDitherMethod;
			}
			else if (strcmp(argv[i], "floydsteinberg") == 0) {
				dithering = FloydSteinbergDitherMethod;
			}
			else {
				std::cerr << "Unknown dithering method" << std::endl;
				exit(1);
			}
		}
		else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			i++;
			output_path = argv[i];
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		}
		else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0) {
			i++;
			export_path = argv[i];
		}
		else if (strcmp(argv[i], "--no-gz") == 0) {
			no_gz = true;
		}
		i++;
	}

	if (palette_path.empty() || input_path.empty() || output_path.empty()) {
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

	if (!export_path.empty()) {
		ImageInfo *export_img = CloneImageInfo(NULL);
		export_img->file = protected_fopen(export_path, "w+b");
		strcpy(export_img->filename, export_path.c_str());
		strcpy(export_img->magick, "png");
		WriteImage(export_img, output, exception);
		DestroyImageInfo(export_img);
	}

	PixelInfo *orig_pix = static_cast<PixelInfo *>(protected_malloc(sizeof(PixelInfo)));
	PixelInfo *palette_pix = static_cast<PixelInfo *>(protected_malloc(sizeof(PixelInfo)));
	int palette_col = palette->columns;
	int palette_row = palette->rows;
	int palette_number = palette_col * palette_row;

	// Create Map Payload
	auto map_tag = NBTP::CompoundTag();
	auto data_tag = Minemap::Map::makeMapData(1, 1, 128, 128, DEFAULT_X_CENTER, DEFAULT_Z_CENTER);
	map_tag.insert("data", data_tag);
	auto colors_tag = (NBTP::BytesTag*) data_tag->getPayload()["colors"].get();
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
				colors_tag->insert(std::make_shared<NBTP::ByteTag>(static_cast<int8_t>(j)));
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
	std::vector<char> out_array;
	boost::iostreams::back_insert_device<std::vector<char>> snk{out_array};

	std::ofstream output_file(output_path, std::ios::binary);

	boost::iostreams::filtering_ostream os;
	if (!no_gz) {
		os.push(boost::iostreams::gzip_compressor{});
	}
	os.push(output_file);

	NBTP::TagIO::writeRoot(os, map_tag);
	os << std::flush;
	os.pop();
	output_file.flush();

	MagickCoreTerminus();
	free(path);
	return 0;
}

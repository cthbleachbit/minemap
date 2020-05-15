/* Minemap
 *
 * minemap.cpp: Main program: convert picture to map
 */

#include <nbtp/libnbtp.h>
#include <cstring>

#include <string>
#include <iostream>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <Magick++.h>

#include "Map.h"
#include "VersionSpec.h"

bool verbose = false;

bool no_gz = false;

Magick::DitherMethod dithering = Magick::DitherMethod::NoDitherMethod;

int main(int argc, char **argv);
void usage();

namespace Minemap {
	bool inline colorEqual(const Magick::ColorRGB &q1, const Magick::ColorRGB &q2) {
		return (q1.red() == q2.red() && q1.green() == q2.green() && q1.blue() == q2.blue());
	}
}

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
	printf("\t-g, --game VER\n");
	printf("\t\tRequired, Minecraft game version this map is going to be used in\n");
	printf("\t\tValid values: 1.8, 1.12\n");
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
		if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--game") == 0) {
			i++;
			palette_path = Minemap::verSpecToPalettePath(Minemap::verSpecFromString(argv[i]));
		}
		else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			input_path = argv[i];
		}
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dithering") == 0) {
			i++;
			if (strcmp(argv[i], "riemersma") == 0) {
				dithering = Magick::DitherMethod::RiemersmaDitherMethod;
			}
			else if (strcmp(argv[i], "floydsteinberg") == 0) {
				dithering = Magick::DitherMethod::FloydSteinbergDitherMethod;
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

	std::cout << "Using palette GIF " << palette_path << std::endl;

	// Create Map Payload
	auto map_tag = NBTP::CompoundTag();
	auto data_tag = Minemap::Map::makeMapData();
	map_tag.insert("data", data_tag);
	auto colors_tag = (NBTP::BytesTag*) data_tag->getPayload()["colors"].get();

	{
		// Init Magick Core
		Magick::InitializeMagick(*argv);

		// Loading Image
		Magick::Image palette_img;
		Magick::Image input_img;
		Magick::Image output_img;
		try {
			palette_img.read(palette_path);
			palette_img.colorSpace(Magick::RGBColorspace);
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		try {
			input_img.read(input_path);
			input_img.colorSpace(Magick::RGBColorspace);
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		output_img = input_img;
		output_img.modifyImage();
		// Scale input to 128x128
		output_img.resize("128x128!");
		output_img.colorSpace(Magick::RGBColorspace);
		// Execute remap
		output_img.quantizeDitherMethod(dithering);
		output_img.quantizeColorSpace(Magick::RGBColorspace);
		output_img.map(palette_img, dithering != Magick::DitherMethod::NoDitherMethod);

		if (!export_path.empty()) {
			palette_img.write(export_path);
		}

		int palette_width = palette_img.size().width();
		int palette_height = palette_img.size().height();
		int palette_number = palette_width * palette_height;

		for (i = 0; i < 16384; i++) {
			ssize_t col = i % 128;
			ssize_t row = i / 128;
			Magick::ColorRGB output_pix = output_img.pixelColor(col, row);
			if (verbose) {
				fprintf(stderr,
						"Pixel at %li, %li: (%f, %f, %f)\n",
						col, row,
						output_pix.red(),
						output_pix.green(),
						output_pix.blue());
			}
			int j;
			for (j = 0; j < palette_number; j++) {
				ssize_t palette_col = j % palette_width;
				ssize_t palette_row = j / palette_width;
				Magick::ColorRGB palette_pix = palette_img.pixelColor(palette_col, palette_row);
				if (Minemap::colorEqual(palette_pix, output_pix)) {
					colors_tag->insert(std::make_shared<NBTP::ByteTag>(static_cast<int8_t>(j)));
					if (verbose) {
						fprintf(stderr, "Matched color id %i\n", j);
					}
					break;
				}
			}
			if (j == palette_number) {
				fprintf(stderr, "Error: No color match for pixel at %li, %li\n", col, row);
				exit(1);
			}
		}
	}

	// Generate map binary
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

	return 0;
}

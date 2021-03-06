/* Minemap
 *
 * minemap.cpp: Main program: convert picture to map
 */

#include <libnbtp.h>
#include <cstring>

#include <string>
#include <iostream>
#include <Magick++.h>

#include "common.h"
#include "Map.h"
#include "VersionSpec.h"
#include "ColorMap.h"
#include "constants.h"

#include "GZStream.h"

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#endif

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
	printf("\t\tOptional, do not gzip generated NBT file\n");
	printf("\t-o, --output FILE\n");
	printf("\t\tRequired, output file in NBT format\n");
	printf("\t-e, --export FILE\n");
	printf("\t\tOptional, export the result of color reduction in png format\n");
	printf("\t-g, --game VER\n");
	printf("\t\tRequired, Minecraft game version this map is going to be used in\n");
	printf("\t\tSelect from the following values: \n");
	for (Minemap::VersionSpec verSpec : Minemap::SUPPORTED_VERSION) {
		printf("\t\t\t%8s for game version %s\n",
			   Minemap::verSpecToString(verSpec).c_str(), Minemap::verSpecToVerRange(verSpec).c_str());
	}
	printf("\t\tOlder versions are not supported.\n");
	printf("\t-v, --verbose\n");
	printf("\t\tOptional, Turn on verbose output\n");
	printf("\n");
	print_version();
}

int main(int argc, char **argv) {
#if defined(_WIN32) || defined(_WIN64)
	_wputenv_s(L"MAGICK_CODER_MODULE_PATH", WIN_MAGICK_CODER_MODULE_PATH);
#endif
	using namespace Minemap;

	// Task properties
	VersionSpec mc_ver = VersionSpec::INVALID;
	std::string palette_path;
	std::string input_path;
	std::string output_path;
	std::string export_path;
	bool verbose = false;
	bool no_gz = false;
	Magick::DitherMethod dithering = Magick::DitherMethod::NoDitherMethod;

	// Parse Parameters
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--game") == 0) {
			i++;
			mc_ver = verSpecFromString(argv[i]);
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

	if (mc_ver == VersionSpec::INVALID) {
		std::cout << MISSING_GAME_VER << std::endl;
		usage();
		exit(1);
	}
	else if (input_path.empty()) {
		std::cout << MISSING_IN_FILE << std::endl;
		usage();
		exit(1);
	}
	else if (output_path.empty()) {
		std::cout << MISSING_OUT_FILE << std::endl;
		usage();
		exit(1);
	}

	palette_path = verSpecToPalettePath(mc_ver);

	// Create Template Map Payload
	auto map_tag = Map::makeMapRoot(mc_ver);
	NBTP::CompoundTag *data_tag = (NBTP::CompoundTag *) map_tag->getPayload()["data"].get();
	auto colors_tag = (NBTP::BytesTag *) data_tag->getPayload()["colors"].get();

	{
		// Init Magick Core
		Magick::InitializeMagick(*argv);

		// Loading Image
		Magick::Image palette_img;
		Magick::Image input_img;
		Magick::Image output_img;
		try {
			palette_img.read(palette_path);
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		std::cout << "Using palette GIF " << palette_path << std::endl;

		try {
			input_img.read(input_path);
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		output_img = input_img;
		output_img.modifyImage();
		// Scale input to 128x128
		output_img.resize("128x128!");
		// Execute remap
		output_img.quantizeDitherMethod(dithering);
		output_img.map(palette_img, dithering != Magick::DitherMethod::NoDitherMethod);

		if (!export_path.empty()) {
			output_img.write(export_path);
		}

		auto palette_lookup_table = loadColorMapFromPalette(palette_img);

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
			auto lookup = palette_lookup_table->reverse();
			auto itr = lookup.find(TupleRGB(output_pix));
			if (itr == lookup.end()) {
				fprintf(stderr, "Error: No color match for pixel at %li, %li\n", col, row);
				exit(1);
			}
			else {
				if (verbose) {
					fprintf(stderr, "Matched color code %i\n", itr->second);
				}
				colors_tag->insert(std::make_shared<NBTP::ByteTag>(itr->second));
			}
		}
	}

	{
		// Generate map binary
		std::unique_ptr<std::ostream> os = nullptr;
		if (no_gz) {
			os = std::make_unique<std::ofstream>(output_path, std::ios::binary);
		} else {
			os = std::make_unique<oGZStream>(output_path.c_str());
		}

		NBTP::TagIO::writeRoot(*os, *map_tag);
		os->flush();
	}

	return 0;
}

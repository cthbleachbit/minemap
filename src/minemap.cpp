/* Minemap
 *
 * minemap.cpp: Main program: convert picture to map
 */

#include <libnbtp.h>
#include <cstring>

#include <string>
#include <iostream>
#include <Magick++.h>
#include <inttypes.h>

#include "libminemap.h"
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

#ifdef USE_GM
namespace Magick {
	enum DitherMethod {
		NoDitherMethod,
		RiemersmaDitherMethod,
		FloydSteinbergDitherMethod
	};
};
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
	printf("\t\tRequired, MINIMUM game version this map can be used in\n");
	printf("\t\tSelect from the following values: \n");
	for (const auto &itr: Minemap::SUPPORTED_VERSIONS) {
		printf("\t\t\t%8s for game version %s\n", itr.name.c_str(), itr.versionRange.c_str());
	}
	printf("\t\tOlder versions are not supported.\n");
	printf("\t-v, --verbose\n");
	printf("\t\tOptional, Turn on verbose output\n");
	printf("\n");
	print_version();
}

int main(int argc, char **argv) {
	using namespace Minemap;

	// Task properties
	Version mc_ver = Version::INVALID;
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
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			input_path = argv[i];
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dithering") == 0) {
			i++;
			if (strcmp(argv[i], "riemersma") == 0) {
				dithering = Magick::DitherMethod::RiemersmaDitherMethod;
			} else if (strcmp(argv[i], "floydsteinberg") == 0) {
				dithering = Magick::DitherMethod::FloydSteinbergDitherMethod;
			} else {
				std::cerr << "Unknown dithering method" << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			i++;
			output_path = argv[i];
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		} else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0) {
			i++;
			export_path = argv[i];
		} else if (strcmp(argv[i], "--no-gz") == 0) {
			no_gz = true;
		}
		i++;
	}

	if (mc_ver == Version::INVALID) {
		std::cout << MISSING_GAME_VER << std::endl;
		usage();
		exit(1);
	} else if (input_path.empty()) {
		std::cout << MISSING_IN_FILE << std::endl;
		usage();
		exit(1);
	} else if (output_path.empty()) {
		std::cout << MISSING_OUT_FILE << std::endl;
		usage();
		exit(1);
	}

	palette_path = absolute(verSpecToPalettePath(mc_ver)).string();

	// Create Template Map Payload
	struct Map::MapGeometry geometry;
	auto map_tag = Map::makeMapRoot(mc_ver, geometry);
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
		// Scale input to width x width
		Magick::Geometry resize_geometry;
		resize_geometry.width(geometry.width);
		resize_geometry.height(geometry.height);
		output_img.sample(resize_geometry);
		// Execute remap
#ifndef USE_GM
		output_img.quantizeDitherMethod(dithering);
#endif
		output_img.map(palette_img, dithering != Magick::DitherMethod::NoDitherMethod);

		if (!export_path.empty()) {
			output_img.write(export_path);
		}

		auto palette_lookup_table = loadColorMapFromPalette(palette_img);

		for (i = 0; i < geometry.width * geometry.width; i++) {
			ssize_t col = i % geometry.width;
			ssize_t row = i / geometry.width;
			Magick::ColorRGB output_pix = output_img.pixelColor(col, row);
			if (verbose) {
				fprintf(stderr,
				        "Pixel at %" PRIi64 ", %" PRIi64 ": (%f, %f, %f)\n",
				        col, row,
				        output_pix.red(),
				        output_pix.green(),
				        output_pix.blue());
			}
			auto colorCode = palette_lookup_table->lookup(TupleRGB(output_pix));
			if (!colorCode.has_value()) {
				fprintf(stderr, "Error: No color match for pixel at %" PRIi64 ", %" PRIi64 "\n", col, row);
				exit(1);
			} else {
				if (verbose) {
					fprintf(stderr, "Matched color code %i for pixel at %" PRIi64 ", %" PRIi64 "\n", colorCode.value(), col, row);
				}
				colors_tag->insert(std::make_shared<NBTP::ByteTag>(colorCode.value()));
			}
		}
	}

	{
		// Generate map binary
		std::unique_ptr<std::ostream> os;
		if (no_gz) {
			os = std::make_unique<std::ofstream>(output_path, std::ios::binary);
		} else {
			os = std::make_unique<oGZStream>(output_path.c_str());
		}

		Map::saveMap(*os, *map_tag);
		os->flush();
	}

	return 0;
}

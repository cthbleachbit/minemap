/* Minemap
 *
 * minemap.cpp: Main program: convert picture to map
 */

#include <libnbtp.h>
#include <cstring>

#include <string>
#include <cassert>
#include <iostream>
#include <Magick++.h>
#include <filesystem>

#include "libminemap.h"
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
	std::cout << MINEMAP_USAGE;
	Minemap::prettyPrintSupportedVersions();
	Minemap::print_version("minemap");
}

int main(int argc, char **argv) {
	using namespace Minemap;

	initializeLocale();

	// Task properties
	Version mc_ver = Version::INVALID;
	std::string input_path;
	std::string output_path;
	std::string export_path;
	bool no_gz = false;
	bool dithering = false;
	bool replace_if_exist = false;

	// Parse Parameters
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--game") == 0) {
			i++;
			if (i >= argc) {
				break;
			}
			mc_ver = verSpecFromString(argv[i]);
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
			i++;
			if (i >= argc) {
				break;
			}
			input_path = argv[i];
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dithering") == 0) {
			dithering = true;
		} else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--replace") == 0) {
			replace_if_exist = true;
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			i++;
			if (i >= argc) {
				break;
			}
			output_path = argv[i];
		} else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0) {
			i++;
			if (i >= argc) {
				break;
			}
			export_path = argv[i];
		} else if (strcmp(argv[i], "--no-gz") == 0) {
			no_gz = true;
		} else {
			std::cerr << localizedFormat(INVALID_ARGUMENT, argv[i]) << std::endl;
			usage();
			exit(1);
		}
		i++;
	}

	if (mc_ver == Version::INVALID) {
		std::cerr << MISSING_GAME_VER << std::endl;
		usage();
		exit(1);
	} else if (input_path.empty()) {
		std::cerr << MISSING_IN_FILE << std::endl;
		usage();
		exit(1);
	} else if (output_path.empty()) {
		std::cerr << MISSING_OUT_FILE << std::endl;
		usage();
		exit(1);
	}

	// Create Template Map Payload
	struct Map::MapGeometry geometry;
	std::shared_ptr<NBTP::Tag> map_tag = nullptr;
	{
		// Check if we need to read existing nbt for replacement
		auto stat = std::filesystem::status(output_path);
		bool output_exist = is_regular_file(stat);

		if (output_exist & replace_if_exist) {
			// Read existing map
			std::unique_ptr<std::istream> existing;
			if (no_gz) {
				existing = std::make_unique<std::ifstream>(output_path, std::ios::binary);
			} else {
				existing = std::make_unique<iGZStream>(output_path.c_str());
			}
			ssize_t parsed_bytes;

			// Catch invalid input files
			try {
				map_tag = NBTP::TagIO::parseRoot(*existing, parsed_bytes);
				if (map_tag->typeCode() != NBTP::COMPOUND) {
					throw NBTP::TagParseException(0, localizedFormat(MAP_NOT_COMPOUND, NBTP::TypeNames[map_tag->typeCode()]));
				}
				// Overwrite dataversion tag
				insertDataVersion(dynamic_cast<NBTP::CompoundTag &>(*map_tag), mc_ver);
			} catch (const NBTP::TagParseException &e) {
				std::cerr << e.what() << std::endl;
				std::cerr << MINEMAP_PARSE_FAIL << std::endl;
				map_tag = Map::makeMapRoot(mc_ver, geometry);
			}
		} else {
			map_tag = Map::makeMapRoot(mc_ver, geometry);
		}
	}

	// At this point we have a valid map tag structure to work with
	NBTP::BytesTag *colors_tag;
	try {
		colors_tag = Minemap::Map::getModifiableColors(map_tag);
		colors_tag->getPayload().clear();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	{
		// Init Magick Core
		Magick::InitializeMagick(*argv);

		// Loading Image
		Magick::Image palette_img;
		Magick::Image input_img;
		Magick::Image mapped_img;
		try {
			palette_img = verspec_to_image(verSpecToPaletteData(mc_ver));
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		try {
			input_img.read(input_path);
		}
		catch (Magick::Exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}

		input_img.modifyImage();
		// Scale input to width x width
		Magick::Geometry resize_geometry;
		resize_geometry.aspect(false);
		resize_geometry.width(geometry.width);
		resize_geometry.height(geometry.height);
		input_img.sample(resize_geometry);
		// Apply dithering only to a copy of the input
		mapped_img = input_img;
		mapped_img.modifyImage();
		// Execute remap
		mapped_img.map(palette_img, dithering);
		if (!export_path.empty()) {
			mapped_img.write(export_path);
		}

		try {
			auto color_map_from_data = loadColorMapFromVersion(verSpecToPaletteData(mc_ver));
			Minemap::mapped_to_tag(input_img, mapped_img, color_map_from_data, colors_tag);
		} catch (std::runtime_error &e) {
			std::cerr << e.what() << std::endl;
			exit(1);
		}

		// Write Result image
		if (!export_path.empty()) {
			mapped_img.write(export_path);
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

		Map::saveMap(*os, dynamic_cast<NBTP::CompoundTag &>(*map_tag));
		os->flush();
	}

	return 0;
}

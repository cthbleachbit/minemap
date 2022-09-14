/* Minemap
 *
 * pamenim: Main program: convert map to png
 */

#include <cstdio>
#include <libnbtp.h>
#include <iostream>
#include <cstring>

#include "libminemap.h"
#include "ColorMap.h"
#include "VersionSpec.h"
#include "constants.h"

#include "GZStream.h"
#include "Map.h"

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#endif

void usage() {
	printf("pamenim <options>\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, nbt input\n");
	printf("\t--no-gz\n");
	printf("\t\tOptional, indicates that the input file is uncompressed\n");
	printf("\t-g, --game VER\n");
	printf("\t\tRequired, Minecraft game version this map is exported from\n");
	printf("\t\tSelect from the following values: \n");
	Minemap::prettyPrintSupportedVersions();
	printf("\t\tOlder versions are not supported.\n");
	printf("\t-o, --output OUTPUT\n");
	printf("\t\tRequired, output picture\n");
	printf("\n");
	Minemap::print_version();
}

int main(int argc, char **argv) {

	initializeLocale();

	// Argument parsing
	using namespace Minemap;
	Version mc_ver = Version::INVALID;
	std::string input_path;
	std::string output_path;
	bool no_gz = false;

	{
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
			} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				i++;
				if (i >= argc) {
					break;
				}
				output_path = argv[i];
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
	}

	// Start by getting bimap for color mapping
	Magick::InitializeMagick(*argv);
	auto palette_lookup_table = loadColorMapFromVersion(verSpecToPaletteData(mc_ver));

	// Get input color array ready
	std::shared_ptr<NBTP::Tag> root_tag;
	{
		std::unique_ptr<std::istream> is;
		if (no_gz) {
			is = std::make_unique<std::ifstream>(input_path, std::ios::binary);
		} else {
			is = std::make_unique<iGZStream>(input_path.c_str());
		}

		ssize_t parsed_bytes;
		root_tag = NBTP::TagIO::parseRoot(*is, parsed_bytes);
	}

	NBTP::ListTag::List colors_list;
	try {
		colors_list = Minemap::Map::getModifiableColors(root_tag)->getPayload();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	// Convert
	std::unique_ptr<std::array<double, 16384 * 4>> pixelStore = nullptr;
	try {
		pixelStore = Minemap::tag_to_pixelstore(colors_list, palette_lookup_table);
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	Magick::Image output_img(128, 128, "RGBA", Magick::DoublePixel, pixelStore->data());
	output_img.write(output_path);

	return 0;
}
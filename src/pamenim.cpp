/* Minemap
 *
 * pamenim: Main program: convert map to png
 */

#include <cstdio>
#include <libnbtp.h>
#include <iostream>
#include <cstring>

#include "libminemap.h"
#include "common.h"
#include "ColorMap.h"
#include "VersionSpec.h"
#include "constants.h"

#include "GZStream.h"

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
	for (const auto &itr : Minemap::SUPPORTED_VERSIONS) {
		printf("\t\t\t%8s for game version %s\n", itr.name.c_str(), itr.versionRange.c_str());
	}
	printf("\t\tOlder versions are not supported.\n");
	printf("\t-o, --output OUTPUT\n");
	printf("\t\tRequired, output picture\n");
	printf("\n");
	print_version();
}

int main(int argc, char **argv) {
#if defined(_WIN32) || defined(_WIN64)
	_wputenv_s(L"MAGICK_CODER_MODULE_PATH", WIN_MAGICK_CODER_MODULE_PATH);
#endif
	// Argument parsing
	using namespace Minemap;
	Version mc_ver = Version::INVALID;
	std::string palette_path;
	std::string input_path;
	std::string output_path;
	bool no_gz = false;

	{
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
			else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				i++;
				output_path = argv[i];
			}
			else if (strcmp(argv[i], "--no-gz") == 0) {
				no_gz = true;
			}
			i++;
		}

		if (mc_ver == Version::INVALID) {
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
	}

	// Start by getting bimap for color mapping
	Magick::InitializeMagick(*argv);
	palette_path = verSpecToPalettePath(mc_ver);
	std::cout << "Using palette GIF " << palette_path << std::endl;
	auto palette_lookup_table = loadColorMapFromPalette(Magick::Image(palette_path));

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

	if (root_tag->typeCode() != NBTP::TagType::COMPOUND) {
		throw std::invalid_argument(ROOT_NOT_COMPOUND);
	}
	NBTP::Tag *data_tag = ((NBTP::CompoundTag *) root_tag.get())->getPayload()["data"].get();
	if (data_tag->typeCode() != NBTP::TagType::COMPOUND) {
		throw std::invalid_argument("Map data is not a compound. Is this a map nbt file?");
	}
	NBTP::Tag *bytes_tag = ((NBTP::CompoundTag *) data_tag)->getPayload()["colors"].get();
	if (bytes_tag->typeCode() != NBTP::TagType::BYTES) {
		throw std::invalid_argument("Color data is not a byte array. Is this a map nbt file?");
	}
	auto colors_list = ((NBTP::BytesTag *) bytes_tag)->getPayload();

	// We only care about the first 128*128 bytes
	double pixelStore[16384 * 4];
	for (int i = 0; i < 16384; i++) {
		MapColorCode colorIndex = ((NBTP::ByteTag *) colors_list[i].get())->getPayload();
		auto rgb = palette_lookup_table->lookup(colorIndex);

		if (!rgb.has_value()) {
			std::cerr << "Is this a map nbt file? Color code is out of range: " << colorIndex << std::endl;
			exit(1);
		}

		pixelStore[i * 4] = rgb->r;
		pixelStore[i * 4 + 1] = rgb->g;
		pixelStore[i * 4 + 2] = rgb->b;
		pixelStore[i * 4 + 3] = colorIndex < 4 ? 0 : 1;
	}

	Magick::Image output_img(128, 128, "RGBA", MagickCore::DoublePixel, pixelStore);
	output_img.write(output_path);

	return 0;
}
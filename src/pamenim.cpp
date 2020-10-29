/* Minemap
 *
 * pamenim: Main program: convert map to png
 */

#include <cstdio>
#include <libnbtp.h>
#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "common.h"
#include "ColorMap.h"
#include "VersionSpec.h"
#include "constants.h"

void usage() {
	printf("pamenim <options>\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, nbt input\n");
	printf("\t--no-gz\n");
	printf("\t\tOptional, indicates that the input file is uncompressed\n");
	printf("\t-g, --game VER\n");
	printf("\t\tRequired, Minecraft game version this map is exported from\n");
	printf("\t\tSelect from the following values: \n");
	for (Minemap::VersionSpec verSpec : Minemap::SUPPORTED_VERSION) {
		printf("\t\t\t%8s for game version %s\n",
			   Minemap::verSpecToString(verSpec).c_str(), Minemap::verSpecToVerRange(verSpec).c_str());
	}
	printf("\t\tOlder versions are not supported.\n");
	printf("\t-o, --output OUTPUT\n");
	printf("\t\tRequired, output picture\n");
#if defined(_MSC_VER)
	printf("Executable built at %s %s", __DATE__, __TIME__);
#endif
}

int main(int argc, char **argv) {
	// Argument parsing
	using namespace Minemap;
	VersionSpec mc_ver = VersionSpec::INVALID;
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
	}

	// Start by getting bimap for color mapping
	Magick::InitializeMagick(*argv);
	palette_path = verSpecToPalettePath(mc_ver);
	std::cout << "Using palette GIF " << palette_path << std::endl;
	auto palette_lookup_table = loadColorMapFromPalette(Magick::Image(palette_path));

	// Get input color array ready
	std::shared_ptr<NBTP::Tag> root_tag;
	{
		boost::iostreams::filtering_istream is;
		std::ifstream input_file(input_path, std::ios::binary);
		if (!no_gz) {
			is.push(boost::iostreams::gzip_decompressor());
		}
		is.push(input_file);
		ssize_t parsed_bytes;
		root_tag = NBTP::TagIO::parseRoot(is, parsed_bytes);
	}
	if (root_tag->typeCode() != NBTP::TagType::COMPOUND) {
		throw std::invalid_argument(ROOT_NOT_COMPOUND);
		exit(1);
	}
	NBTP::Tag *data_tag = ((NBTP::CompoundTag *) root_tag.get())->getPayload()["data"].get();
	if (data_tag->typeCode() != NBTP::TagType::COMPOUND) {
		throw std::invalid_argument("Map data is not a compound. Is this a map nbt file?");
		exit(1);
	}
	NBTP::Tag *bytes_tag = ((NBTP::CompoundTag *) data_tag)->getPayload()["colors"].get();
	if (bytes_tag->typeCode() != NBTP::TagType::BYTES) {
		throw std::invalid_argument("Color data is not a byte array. Is this a map nbt file?");
		exit(1);
	}
	auto colors_list = ((NBTP::BytesTag *) bytes_tag)->getPayload();

	// We only care about the first 128*128 bytes
	double pixelStore[16384 * 3];
	for (int i = 0; i < 16384; i++) {
		int8_t colorIndex = ((NBTP::ByteTag *) colors_list[i].get())->getPayload();
		TupleRGB rgb;
		try {
			rgb = palette_lookup_table->right.at(colorIndex);
		}
		catch (std::runtime_error &e) {
			std::cerr << e.what() << " Is this a map nbt file?" << std::endl;
			exit(1);
		}

		pixelStore[i * 3] = rgb.r;
		pixelStore[i * 3 + 1] = rgb.g;
		pixelStore[i * 3 + 2] = rgb.b;
	}

	Magick::Image output_img(128, 128, "RGB", MagickCore::DoublePixel, pixelStore);
	output_img.write(output_path);

	return 0;
}
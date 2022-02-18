/* Minemap marker editor
 *
 * minemarker.cpp: Main program: edit map banners and markers
 */

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#endif

#include <cstdio>
#include <memory>
#include <iostream>
#include <string>
#include <tags/CompoundTag.h>
#include "libminemap.h"
#include "GZStream.h"

void usage();

int main(int argc, char **argv);

void handleFrame(int argc, char **argv, std::shared_ptr<NBTP::Tag> root_ptr);

void handleBanner(int argc, char **argv, std::shared_ptr<NBTP::Tag> root_ptr);

void usage() {
	printf("minemarkers <options> [operation]\n");
	printf("Where <options> may be:\n");
	printf("\t-i, --input INPUT\n");
	printf("\t\tRequired, nbt input to modify\n");
	printf("\t--no-gz\n");
	printf("\t\tOptional, indicates that the input file is uncompressed\n");
	printf("Where banner operation may be one of\n");
	printf("\tbanner ls\n");
	printf("\t\tPrint a numbered list of banners contained in the map\n");
	printf("\tbanner mk abs:<x>:<y>:<z> <color>:<name>\n");
	printf("\t\tAdd/Replace a banner marker at absolute XYZ coordinates. name field is optional\n");
	printf("\tbanner mk rel:<x>:<y>:<z> <color>:<name>\n");
	printf("\t\tAdd/Replace a banner marker at absolute Y but XZ relative to map center. name field is optional\n");
	printf("\tbanner rm abs:<x>:<y>:<z>\n");
	printf("\t\tDelete all banner marker at absolute XYZ coordinates\n");
	printf("\tbanner rm rel:<x>:<y>:<z>\n");
	printf("\t\tDelete all banner marker at absolute Y but XZ relative to map center\n");
	printf("\tbanner rm idx:<index>\n");
	printf("\t\tDelete banner at specified index\n");
	printf("Where frame operation may be one of\n");
	printf("\tframe ls\n");
	printf("\t\tPrint a numbered list of banners contained in the map\n");
	printf("\tframe mk abs:<x>:<y>:<z> <rotation>\n");
	printf("\t\tAdd/Replace a frame marker at absolute XYZ coordinates\n");
	printf("\tframe mk rel:<x>:<y>:<z> <rotation>\n");
	printf("\t\tAdd/Replace a frame marker at absolute Y but XZ relative to map center\n");
	printf("\tframe rm abs:<x>:<y>:<z>\n");
	printf("\t\tDelete all frame marker at absolute XYZ coordinates\n");
	printf("\tframe rm rel:<x>:<y>:<z>\n");
	printf("\t\tDelete all frame marker at absolute Y but XZ relative to map center\n");
	printf("\tframe rm entity:<index>\n");
	printf("\t\tDelete the frame marker with specific entity ID\n");
	printf("\t");
	printf("\n");
	Minemap::print_version();
}

namespace Minemap {
	enum class MarkerOps {
		LS,
		MK,
		RM,
		INVALID
	};

	enum class MarkerType {
		BANNER,
		FRAME,
		INVALID
	};
}

int main(int argc, char **argv) {
	using namespace Minemap;
	std::string input_path;
	MarkerOps ops = MarkerOps::INVALID;
	MarkerType type = MarkerType::INVALID;

	char **subargv = nullptr;
	int subargc = 0;
	bool no_gz = false;
	{
		// Argument passing
		int i = 1;
		while (i < argc) {
			if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
				i++;
				if (i >= argc) {
					break;
				}
				input_path = argv[i];
			} else if (strcmp(argv[i], "--no-gz") == 0) {
				no_gz = true;
			} else if (strcmp(argv[i], "frame") == 0) {
				type = Minemap::MarkerType::FRAME;
				break;
			} else if (strcmp(argv[i], "banner") == 0) {
				type = Minemap::MarkerType::BANNER;;
				break;
			} else {
				std::cerr << fmt::format(INVALID_ARGUMENT, argv[i]) << std::endl;
				usage();
				exit(1);
			}
			i++;
		}

		if (input_path.empty()) {
			std::cerr << MISSING_IN_FILE << std::endl;
			usage();
			exit(1);
		}


		subargc = argc - i;
		subargv = argv + i;
	}

	// Get input color array ready
	std::shared_ptr<NBTP::Tag> root_tag;
	{
		std::unique_ptr<std::istream> is;
		if (no_gz) {
			is = std::make_unique<std::ifstream>(input_path, std::ios::binary);
		} else {
			is = std::make_unique<Minemap::iGZStream>(input_path.c_str());
		}

		ssize_t parsed_bytes;
		root_tag = NBTP::TagIO::parseRoot(*is, parsed_bytes);
	}

	switch (type) {
		case MarkerType::FRAME:
			handleFrame(subargc, subargv, root_tag);
			break;
		case MarkerType::BANNER:
			handleBanner(subargc, subargv, root_tag);
			break;
		default:
			std::cerr << MISSING_MARKER << std::endl;
			usage();
			exit(1);
	}
	return 0;
}

/**
 * handle frame marker operations - arguments in the form of "frame <cmd> <args>"
 * @param argc
 * @param argv
 * @param root_ptr
 */
void handleFrame(int argc, char **argv, std::shared_ptr<NBTP::Tag> root_ptr) {
	// TODO
}

/**
 * handle banner marker operations - arguments in the form of "banner <cmd> <args>"
 * @param argc
 * @param argv
 * @param root_ptr
 */
void handleBanner(int argc, char **argv, std::shared_ptr<NBTP::Tag> root_ptr) {
	// TODO
}
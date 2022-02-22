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
#include "Map.h"
#include "Marker.h"

void usage();

int main(int argc, char **argv);

bool handleFrame(int argc, char **argv,
                 std::shared_ptr<NBTP::Tag> root_ptr,
                 NBTP::IntTag::V xOffset,
                 NBTP::IntTag::V zOffset);

bool handleBanner(int argc, char **argv,
                  std::shared_ptr<NBTP::Tag> root_ptr,
                  NBTP::IntTag::V xOffset,
                  NBTP::IntTag::V zOffset);

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
    printf("\tbanner mk abs:<x>:<y>:<z> <color> <name>\n");
    printf("\t\tAdd/Replace a banner marker at absolute XYZ coordinates. name field is optional\n");
    printf("\tbanner mk rel:<x>:<y>:<z> <color> <name>\n");
    printf("\t\tAdd/Replace a banner marker at absolute Y but XZ relative to map center. name field is optional\n");
    printf("\tbanner rm abs:<x>:<y>:<z>\n");
    printf("\t\tDelete all banner marker at absolute XYZ coordinates\n");
    printf("\tbanner rm rel:<x>:<y>:<z>\n");
    printf("\t\tDelete all banner marker at absolute Y but XZ relative to map center\n");
    printf("\tbanner rmidx <index>\n");
    printf("\t\tDelete banner at specified index\n");
#if 0
    // It seems that frame markers are not protected on a locked map
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
#endif
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

    NBTP::IntTag::V xOffset = 0;
    NBTP::IntTag::V zOffset = 0;
    {
        if (root_tag->typeCode() != NBTP::TagType::COMPOUND) {
            throw std::runtime_error(fmt::format(MAP_NOT_COMPOUND, NBTP::TypeNames[root_tag->typeCode()]));
        }
        NBTP::Tag *data_tag = ((NBTP::CompoundTag *) root_tag.get())->getPayload()["data"].get();
        if (data_tag->typeCode() != NBTP::TagType::COMPOUND) {
            throw std::runtime_error(fmt::format(MAP_NOT_COMPOUND, NBTP::TypeNames[root_tag->typeCode()]));
        }
        auto data = (NBTP::CompoundTag *) data_tag;
        auto xOffset_tag = data->lookup("xCenter");
        auto zOffset_tag = data->lookup("zCenter");
        if (xOffset_tag && xOffset_tag->typeCode() == NBTP::INT) {
            xOffset = ((NBTP::IntTag *) xOffset_tag.get())->getPayload();
        }
        if (zOffset_tag && zOffset_tag->typeCode() == NBTP::INT) {
            zOffset = ((NBTP::IntTag *) zOffset_tag.get())->getPayload();
        }
    }

    bool changed;

    switch (type) {
        case MarkerType::FRAME:
            changed = handleFrame(subargc, subargv, root_tag, xOffset, zOffset);
            break;
        case MarkerType::BANNER:
            changed = handleBanner(subargc, subargv, root_tag, xOffset, zOffset);
            break;
        default:
            std::cerr << MISSING_MARKER << std::endl;
            usage();
            exit(1);
    }

    if (changed) {
        // Write out the map
        std::unique_ptr<std::ostream> os;
        if (no_gz) {
            os = std::make_unique<std::ofstream>(input_path, std::ios::binary);
        } else {
            os = std::make_unique<Minemap::oGZStream>(input_path.c_str());
        }

        Map::saveMap(*os, *((NBTP::CompoundTag *) root_tag.get()));
        os->flush();
    }
    return 0;
}

/**
 * handle frame marker operations - arguments in the form of "frame <cmd> <args>"
 * @param argc
 * @param argv
 * @param root_ptr
 */
bool handleFrame(int argc, char **argv,
                 std::shared_ptr<NBTP::Tag> root_ptr,
                 NBTP::IntTag::V xOffset,
                 NBTP::IntTag::V zOffset) {
    // FIXME: Not implemented
    // Frame markers are not protected even in locked maps. Manually added frame markers may disappear as chunks update
    // themselves.
    std::cout << "NOT IMPLEMENTED YET" << std::endl;
    return false;
}

/**
 * handle banner marker operations - arguments in the form of "banner <cmd> <args>"
 * @param argc
 * @param argv
 * @param root_ptr
 * @return true if the map was modified
 */
bool handleBanner(int argc, char **argv,
                  std::shared_ptr<NBTP::Tag> root_ptr,
                  NBTP::IntTag::V xOffset,
                  NBTP::IntTag::V zOffset) {
    auto banners_tag = Minemap::Map::getModifiableBanners(root_ptr);
    auto &banners_list = banners_tag->getPayload();
    if (argc < 2) {
        std::cerr << MISSING_MARKER << std::endl;
        return false;
    }
    if (strcmp("ls", argv[1]) == 0) { // argc = 2
        for (auto itr = banners_list.cbegin(); itr < banners_list.cend(); itr++) {
            Minemap::Banner banner(*itr);
            std::cout << fmt::format("{:08}\t{}", itr - banners_list.cbegin(), banner) << std::endl;
        }
        return false;
    } else if (strcmp("mk", argv[1]) == 0) { // argc = 4 or 5, argv[2] = pos, argv[3] = banner spec
        if (argc < 4 || argc > 5) {
            std::cerr << MISSING_MARKER << std::endl;
            return false;
        }
        Minemap::MarkerPosition position(argv[2], xOffset, zOffset);
        Minemap::Banner banner(position, argv[3], argc == 5 ? std::make_optional<std::string>(argv[4]) : std::nullopt);
        banners_list.push_back(banner.toCompound());
        return true;
    } else if (strcmp("rm", argv[1]) == 0) { // argc = 3, argv[2] = rel:x:y:z or abs:x:y:z
        // TODO
        return true;
    } else if (strcmp("rmidx", argv[1]) == 0) { // argc = 3, argv[2] = index
        // TODO
        return true;
    } else {
        return false;
    }
}
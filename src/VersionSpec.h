//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_VERSIONSPEC_H
#define MINEMAP_VERSIONSPEC_H

#include <string>
#include <utility>
#include <array>
#include <filesystem>
#include <optional>
#include <libnbtp.h>
#include "constants.h"

#ifndef MINEMAP_PALETTE_DIR
#define MINEMAP_PALETTE_DIR "/usr/share/minemap/palettes"
#endif

// Quick hand for defining a version spec
#define DEFINE_VERSION_SPEC(ver_start, ver_end, dv) \
VersionSpec{ \
    (ver_start), \
    "[" ver_start ", " ver_end ")", \
    "rgba-" ver_start ".gif", \
    (dv) \
}

namespace Minemap {
	// A list of supported versions.
	// END_OF_VERSION is used to sanity check SUPPORTED_VERSIONS array and enum boundary.
	enum Version {
		INVALID = -1,
		MC_1_8 = 0,
		MC_1_12 = 1,
		MC_1_16 = 2,
		MC_1_17 = 3,
		END_OF_VERSION,
	};

	struct VersionSpec {
		// Human-readable name of this version
		const char name[5] = "NONE";
		// Human-readable range of game version this VersionSpec can be used in
		const char versionRange[20] = "NONE";
		// Palette file name
		const char palettePath[20] = "Invalid";
		// Corresponding data version tag of this version
		const std::optional<NBTP::IntTag::V> dataVersion = std::nullopt;
	};

	constexpr std::array<VersionSpec, END_OF_VERSION> SUPPORTED_VERSIONS = {
			DEFINE_VERSION_SPEC("1.8", "1.12", std::nullopt), // 1.8.1-pre1
			DEFINE_VERSION_SPEC("1.12", "1.16", 1128), // 1.12-17w17a
			DEFINE_VERSION_SPEC("1.16", "1.17", 2562), // 1.16-pre-6
			DEFINE_VERSION_SPEC("1.17", "+oo", 2699), // 21w10a and 1.17
	};

	/**
	 * Match a version from human readable string
	 * @param verString a version string, e.g. 1.17
	 * @return corresponding Version or INVALID if version string is invalid
	 */
	Version verSpecFromString(const std::string &verString) noexcept;

	/**
	 * Convert a version to an absolute palette path.
	 * The directory where the palette GIFs reside is defined in MINEMAP_PALETTE_DIR
	 * @param ver version number
	 * @throws std::runtime_error(INVALID_GAME_VER) if version is invalid
	 * @return absolute path to the palette file
	 */
	std::filesystem::path verSpecToPalettePath(Version ver);

	/**
	 * Insert the data version tag depending on the game version this map is intended for
	 * @param root    the root tag to operate on
	 * @param ver     Version spec
	 */
	void insertDataVersion(NBTP::CompoundTag &root, Version ver);
}
#endif //VERSIONSPEC_H

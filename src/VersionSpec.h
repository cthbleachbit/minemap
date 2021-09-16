//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_VERSIONSPEC_H
#define MINEMAP_VERSIONSPEC_H

#include <string>
#include <utility>
#include <libnbtp.h>
#include "common.h"
#include "constants.h"

#ifndef MINEMAP_PALETTE_DIR
#define MINEMAP_PALETTE_DIR "/usr/share/minemap/palettes"
#endif

#define MINEMAP_PALETTE_FILE "rgba-{}.gif"

namespace Minemap {
	/**
	 * When a new version is added make sure the array paletteFiles in VersionSpec.cpp is updated too.
	 */
	enum Version {
		INVALID = -1,
		MC_1_8 = 0,
		MC_1_12,
		MC_1_16,
		MC_1_17,
		END_OF_VERSION
	};

	struct VersionSpec {
		// Human readable name of this version
		std::string name = "Invalid";
		// Human readable range of game version this VersionSpec can be used in
		std::string versionRange = "Invalid";
		// Corresponding data version tag of this version
		std::optional<NBTP::IntTag::V> dataVersion = std::nullopt;
	};

	const std::array<VersionSpec, 4> versions = {
			VersionSpec{.name = "1.8", .versionRange="[1.8, 1.12)", .dataVersion = std::nullopt}, // 1.8.1-pre1
			VersionSpec{.name = "1.12", .versionRange="[1.12, 1.16)", .dataVersion = 1128}, // 1.12-17w17a
			VersionSpec{.name = "1.16", .versionRange="[1.16, 1.17)", .dataVersion = 2562}, // 1.16-pre-6
			VersionSpec{.name = "1.17", .versionRange="[1.17, +oo)", .dataVersion = 2699}, // 21w10a and 1.17
	};

	// A weird way to say that the number of version specs must equal to the maximum enum
	static_assert(std::tuple_size<decltype(versions)>::value == END_OF_VERSION);

	/**
	 * Match a version from human readable string
	 * @param verString a version string, e.g. 1.17
	 * @return corresponding Version or INVALID if version string is invalid
	 */
	Version verSpecFromString(const std::string &verString) noexcept;

	/** Convert a version to a human readable name
	 * @param ver version specification
	 * @throws std::runtime_error(INVALID_GAME_VER) if version is invalid
	 * @return A string representation of the version specification
	 */
	const std::string &verSpecToString(Version ver) noexcept;

	/**
	 * Convert a version specification to the range of Minecraft versions that accepts the
	 * encoding specified by the palette
	 * @param ver version specification
	 * @throws std::runtime_error(INVALID_GAME_VER) if version is invalid
	 * @return a human readable representation of the range
	 */
	const std::string& verSpecToVerRange(Version ver);

	/**
	 * Convert a version to an absolute palette path.
	 * The directory where the palette GIFs reside is defined in MINEMAP_PALETTE_DIR
	 * @param ver version number
	 * @throws std::runtime_error(INVALID_GAME_VER) if version is invalid
	 * @return absolute path to the palette file
	 */
	std::string verSpecToPalettePath(Version ver);

	/**
	 * Insert the data version tag depending on the game version this map is intended for
	 * @param root    the root tag to operate on
	 * @param ver     Version spec
	 */
	void insertDataVersion(NBTP::CompoundTag &root, Version ver);
}
#endif //VERSIONSPEC_H

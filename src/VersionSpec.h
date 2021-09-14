//
// Created by cth451 on 2020/05/15.
//

#ifndef VERSIONSPEC_H
#define VERSIONSPEC_H

#include <string>
#include <libnbtp.h>
#include "common.h"

#ifndef MINEMAP_PALETTE_DIR
#define MINEMAP_PALETTE_DIR "/usr/share/minemap/palettes"
#endif

#define MINEMAP_PALETTE_FILE "rgba-{}.gif"

namespace Minemap {
	/**
	 * When a new version is added make sure the array paletteFiles in VersionSpec.cpp is updated too.
	 */
	enum VersionSpec {
		INVALID = 0,
		MC_1_8,
		MC_1_12,
		MC_1_16,
		MC_1_17,
	};

	static VersionSpec SUPPORTED_VERSION[] = {MC_1_8, MC_1_12, MC_1_16, MC_1_17};

	VersionSpec verSpecFromString(const std::string &verString);

	/**
	 * @param ver
	 * @return A string representation of the version specification
	 */
	std::string verSpecToString(VersionSpec ver);

	/**
	 * Convert a version specification to the range of Minecraft versions that accepts the
	 * encoding specified by the palette
	 * @param ver version specification
	 * @return    game version range
	 */
	std::string verSpecToVerRange(VersionSpec ver);

	std::string verSpecToPalettePath(VersionSpec ver);

	/**
	 * Insert the data version tag depending on the game version this map is intended for
	 * @param root    the root tag to operate on
	 * @param ver     Version spec
	 */
	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver);
}
#endif //VERSIONSPEC_H

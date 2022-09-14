//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_VERSIONSPEC_H
#define MINEMAP_VERSIONSPEC_H

#include <string>
#include <array>
#include <optional>
#include <libnbtp.h>
#include "constants.h"

// Quick hand for defining a version spec
#define DEFINE_VERSION_SPEC(ver_start, ver_end, ident, dv) \
VersionSpec{ \
    (ver_start), \
    "[" ver_start ", " ver_end ")", \
    ident ## _DATA, \
    ident ## _HEIGHT, \
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
		// Palette data (flat array, generated)
		const uint8_t *paletteData = {};
		// Palette height (in pixels, generated)
		const size_t paletteHeight = 0;
		// Corresponding data version tag of this version
		const std::optional<NBTP::IntTag::V> dataVersion = std::nullopt;
	};

	extern const std::array<VersionSpec, END_OF_VERSION> SUPPORTED_VERSIONS;

	/**
	 * Match a version from human readable string
	 * @param verString a version string, e.g. 1.17
	 * @return corresponding Version or INVALID if version string is invalid
	 */
	Version verSpecFromString(const std::string &verString) noexcept;

	/**
	 * Fetch VersionSpec for external usage
	 * @param ver   Version number
	 * @return corresponding VersionSpec structure, or throws error if the version is invalid
	 */
	const VersionSpec verSpecToPaletteData(Version ver);

	/**
	 * Pretty print all supported Minecraft versions of this program to stdout.
	*/
	inline constexpr void prettyPrintSupportedVersions() {
		for (const VersionSpec &ver: SUPPORTED_VERSIONS) {
			printf("\t\t\t%8s for game version %s\n", ver.name, ver.versionRange);
		}
		printf("\t\tOlder versions are not supported.\n"
		       "\n");
	}

	/**
	 * Insert the data version tag depending on the game version this map is intended for
	 * @param root    the root tag to operate on
	 * @param ver     Version spec
	 */
	void insertDataVersion(NBTP::CompoundTag &root, Version ver);
}
#endif //VERSIONSPEC_H

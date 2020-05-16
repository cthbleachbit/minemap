//
// Created by cth451 on 2020/05/15.
//

#ifndef VERSIONSPEC_H
#define VERSIONSPEC_H

#include <string>
#include <libnbtp.h>
#include <boost/filesystem.hpp>

#ifndef MINEMAP_PALETTE_DIR
#define MINEMAP_PALETTE_DIR "/usr/share/minemap/palettes"
#endif

#define MINEMAP_PALETTE_EXPR std::string(MINEMAP_PALETTE_DIR) + boost::filesystem::path::separator + "rgba-%s.gif"

namespace Minemap {
	enum VersionSpec {
		INVALID,
		MC_1_8,
		MC_1_12,
	};

	VersionSpec verSpecFromString(const std::string &verString);

	std::string verSpecToPalettePath(VersionSpec ver);

	/**
	 * Insert the data version tag if the map is to be used for Minecraft >= 1.12
	 * @param root    the root tag to operate on
	 * @param ver     Version spec
	 */
	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver);
}
#endif //VERSIONSPEC_H

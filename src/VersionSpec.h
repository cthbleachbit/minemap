//
// Created by cth451 on 2020/05/15.
//

#ifndef VERSIONSPEC_H
#define VERSIONSPEC_H

#include <string>
#include <libnbtp.h>

#ifndef MINEMAP_PALETTE_DIR
#define MINEMAP_PALETTE_DIR "/usr/share/minemap/palettes"
#endif

#define MINEMAP_PALETTE_EXPR MINEMAP_PALETTE_DIR "/rgba-%s.gif"

namespace Minemap {
	enum VersionSpec {
		INVALID,
		MC_1_8,
		MC_1_12,
	};

	VersionSpec verSpecFromString(const std::string &verString);

	std::string verSpecToPalettePath(VersionSpec ver);

	std::shared_ptr<NBTP::CompoundTag::Compound> extraDataForVersion(VersionSpec ver);

	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver);
}
#endif //VERSIONSPEC_H

//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"
#include <boost/format.hpp>
#include <iostream>

namespace Minemap {
	VersionSpec verSpecFromString(const std::string &verString) {
		if (verString == "1.8") return VersionSpec::MC_1_8;
		if (verString == "1.12") return VersionSpec::MC_1_12;
		return VersionSpec::INVALID;
	}

	std::string verSpecToPalettePath(VersionSpec ver) {
		std::string versionString;
		switch (ver) {
			case INVALID:
				throw std::runtime_error(INVALID_GAME_VER);
			case MC_1_8:
				versionString = "1.8";
				break;
			case MC_1_12:
				versionString = "1.12";
				break;
		}
		return (boost::format(MINEMAP_PALETTE_EXPR) % versionString).str();
	}

	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver) {
		switch (ver) {
			case MC_1_8:
				break;
			case MC_1_12:
				root.insert("DataVersion", std::make_shared<NBTP::IntTag>(1343));
				break;
			default:
				throw std::runtime_error(INVALID_GAME_VER);
		}
	}
}
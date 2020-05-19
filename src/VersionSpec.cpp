//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"
#include <boost/format.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <iostream>

namespace Minemap {
	VersionSpec verSpecFromString(const std::string &verString) {
		if (verString == "1.8") return VersionSpec::MC_1_8;
		if (verString == "1.12") return VersionSpec::MC_1_12;
		return VersionSpec::INVALID;
	}

	static inline std::string verSpecToFileName(VersionSpec ver) {
		switch (ver) {
			case INVALID:
				throw std::runtime_error(INVALID_GAME_VER);
			case MC_1_8:
				return "1.8";
			case MC_1_12:
				return "1.12";
		}
	}

	std::string verSpecToPalettePath(VersionSpec ver) {
		std::string versionString = verSpecToFileName(ver);
		auto loc = boost::filesystem::path(MINEMAP_PALETTE_DIR);
		return loc.append((boost::format(MINEMAP_PALETTE_FILE) % versionString).str()).string();
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

	std::string VerSpecToFallbackPalettePath(VersionSpec ver) {
		std::string versionString = verSpecToFileName(ver);
		auto loc = boost::dll::program_location().parent_path();
		return loc.append((boost::format(MINEMAP_PALETTE_FILE) % versionString).str()).string();
	}
}
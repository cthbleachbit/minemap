//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"
#include <filesystem>
#include <iostream>

namespace Minemap {

	static const char* paletteFiles[] = {
		"",
		"rgba-1.8.gif",
		"rgba-1.12.gif",
		"rgba-1.16.gif",
		"rgba-1.17.gif",
	};

	VersionSpec verSpecFromString(const std::string &verString) {
		if (verString == "1.8") return VersionSpec::MC_1_8;
		if (verString == "1.12") return VersionSpec::MC_1_12;
		if (verString == "1.16") return VersionSpec::MC_1_16;
		if (verString == "1.17") return VersionSpec::MC_1_17;
		return VersionSpec::INVALID;
	}

	std::string verSpecToString(VersionSpec ver) {
		switch (ver) {
			case INVALID:
			default:
				throw std::runtime_error(INVALID_GAME_VER);
			case MC_1_8:
				return "1.8";
			case MC_1_12:
				return "1.12";
			case MC_1_16:
				return "1.16";
			case MC_1_17:
				return "1.17";
		}
	}

	std::string verSpecToVerRange(VersionSpec ver) {
		switch (ver) {
			case INVALID:
			default:
				throw std::runtime_error(INVALID_GAME_VER);
			case MC_1_8:
				return "[1.8, 1,12)";
			case MC_1_12:
				return "[1.12, 1.16)";
			case MC_1_16:
				return "[1.16, 1.17)";
			case MC_1_17:
				return "[1.17, +oo)";
		}
	}

	std::string verSpecToPalettePath(VersionSpec ver) {
		auto loc = std::filesystem::path(MINEMAP_PALETTE_DIR);
		return loc.append(paletteFiles[ver]).string();
	}

	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver) {
		switch (ver) {
			case MC_1_8: // 1.8.1-pre1
				// Data version isn't introduced to the game yet
				break;
			case MC_1_12: // 1.12-17w17a
				root.insert("DataVersion", std::make_shared<NBTP::IntTag>(1128));
				break;
			case MC_1_16: // 1.16-pre-6
				root.insert("DataVersion", std::make_shared<NBTP::IntTag>(2562));
				break;
			case MC_1_17: // 21w10a and 1.17
				root.insert("DataVersion", std::make_shared<NBTP::IntTag>(2699));
				break;
			default:
				throw std::runtime_error(INVALID_GAME_VER);
		}
	}
}
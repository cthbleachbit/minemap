//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"
#include <filesystem>
#include <iostream>

namespace Minemap {

	static const char* paletteFiles[] = {
		"rgba-1.8.gif",
		"rgba-1.12.gif",
		"rgba-1.16.gif",
		"rgba-1.17.gif",
	};

	const std::string &verSpecToVerRange(Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		return versions[ver].versionRange;
	}

	std::string verSpecToPalettePath(Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		auto loc = std::filesystem::path(MINEMAP_PALETTE_DIR);
		return loc.append(paletteFiles[ver]).string();
	}

	void insertDataVersion(NBTP::CompoundTag &root, Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		auto dv = versions[ver].dataVersion;
		if (dv.has_value()) {
			root.insert("DataVersion", std::make_shared<NBTP::IntTag>(dv.value()));
		}
	}

	Version verSpecFromString(const std::string &verString) noexcept {
		auto comparePredicate = [&verString](const VersionSpec& spec) {return spec.name == verString;};
		auto itr = std::find_if(versions.cbegin(), versions.cend(), comparePredicate);
		return itr == versions.cend() ? Version::INVALID : static_cast<Version>(itr - versions.cbegin());
	}

	const std::string &verSpecToString(Version ver) noexcept {
		if (ver < 0 || ver >= Version::END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		return versions[ver].name;
	}
}
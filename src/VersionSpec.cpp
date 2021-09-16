//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"

namespace Minemap {

	std::string verSpecToPalettePath(Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		return SUPPORTED_VERSIONS[ver].palettePath;
	}

	void insertDataVersion(NBTP::CompoundTag &root, Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		auto dv = SUPPORTED_VERSIONS[ver].dataVersion;
		if (dv.has_value()) {
			root.insert("DataVersion", std::make_shared<NBTP::IntTag>(dv.value()));
		}
	}

	Version verSpecFromString(const std::string &verString) noexcept {
		auto comparePredicate = [&verString](const VersionSpec &spec) { return spec.name == verString; };
		auto itr = std::find_if(SUPPORTED_VERSIONS.cbegin(), SUPPORTED_VERSIONS.cend(), comparePredicate);
		if (itr == SUPPORTED_VERSIONS.cend()) {
			return Version::INVALID;
		} else {
			return static_cast<Version>(itr - SUPPORTED_VERSIONS.cbegin());
		}
	}
}
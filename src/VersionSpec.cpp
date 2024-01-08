//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"

#include "color_maps.gen.h"

#include <algorithm>
#include <memory>

namespace Minemap {

	constexpr std::array<VersionSpec, END_OF_VERSION> SUPPORTED_VERSIONS = {
			DEFINE_VERSION_SPEC("1.8",  "1.8.x",  "1.12.x", MC_1_8,  std::nullopt), // 1.8.1-pre1
			DEFINE_VERSION_SPEC("1.12", "1.12.x", "1.16.x", MC_1_12, 1128), // 1.12-17w17a
			DEFINE_VERSION_SPEC("1.16", "1.16.x", "1.17.x", MC_1_16, 2562), // 1.16-pre-6
			DEFINE_VERSION_SPEC("1.17", "1.17.x", "+oo",    MC_1_17, 2699), // 21w10a and 1.17
	};

	/**
	 * Fetch VersionSpec for external usage
	 * @param ver   Version number
	 * @return corresponding VersionSpec structure, or throws error if the version is invalid
	 */
	const VersionSpec verSpecToPaletteData(Version ver) {
		switch (ver) {
			case MC_1_8:
				return SUPPORTED_VERSIONS[MC_1_8];
			case MC_1_12:
				return SUPPORTED_VERSIONS[MC_1_12];
			case MC_1_16:
				return SUPPORTED_VERSIONS[MC_1_16];
			case MC_1_17:
				return SUPPORTED_VERSIONS[MC_1_17];
			default:
				throw std::runtime_error(INVALID_GAME_VER);
		}
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

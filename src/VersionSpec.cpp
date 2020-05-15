//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include <boost/format.hpp>

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
				throw std::runtime_error("Invalid game version");
			case MC_1_8:
				versionString = "1.8";
				break;
			case MC_1_12:
				versionString = "1.12";
				break;
		}
		return (boost::format(MINEMAP_PALETTE_EXPR) % versionString).str();
	}
	std::shared_ptr<NBTP::CompoundTag::Compound> extraDataForVersion(VersionSpec ver) {
		auto extra = std::make_shared<NBTP::CompoundTag::Compound>();
		switch (ver) {
			case MC_1_8:
				break;
			case MC_1_12:
				(*extra)["trackingPosition"] = std::make_shared<NBTP::ByteTag>(0);
				(*extra)["unlimitedTracking"] = std::make_shared<NBTP::ByteTag>(0);
				(*extra)["locked"] = std::make_shared<NBTP::ByteTag>(1);
				(*extra)["banners"] = std::make_shared<NBTP::ListTag>(NBTP::TagType::COMPOUND);
				(*extra)["frames"] = std::make_shared<NBTP::ListTag>(NBTP::TagType::COMPOUND);
				break;
			default:
				throw std::runtime_error("Invalid game version");
		}
		return extra;
	}
	void insertDataVersion(NBTP::CompoundTag &root, VersionSpec ver) {
		switch (ver) {
			case MC_1_8:
				break;
			case MC_1_12:
				root.insert("DataVersion", std::make_shared<NBTP::IntTag>(1343));
				break;
			default:
				throw std::runtime_error("Invalid game version");
		}
	}
}
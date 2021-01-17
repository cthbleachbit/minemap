//
// Created by cth451 on 2020/05/14.
//

#include <boost/format.hpp>
#include "constants.h"
#include "VersionSpec.h"
#include "Map.h"

// the default center coordinates should be close enough to the "borderlands" that players would not normally reach
#define DEFAULT_X_CENTER 1 << 30
#define DEFAULT_Z_CENTER 1 << 30

namespace Minemap {
	/**
	 * This file handles creation of bare bone map structures and fills in optional parameters.
	 */
	namespace Map {

		using namespace NBTP;

		static struct MapGeometry DEFAULT_GEOMETRY = {
			1,
			1,
			128,
			128,
			DEFAULT_X_CENTER,
			DEFAULT_Z_CENTER
		};

		/**
		 * Per-version specific contents
		 *
		 * This function should be extended every time a new Minecraft version introduced changes in map item format.
		 *
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             A compound tag containing new tags added after that minecraft version
		 */
		inline std::unique_ptr<CompoundTag::Compound>
		extraDataForVersion(VersionSpec ver, const MapGeometry &geometry) {
			auto extra = std::make_unique<CompoundTag::Compound>();
			switch (ver) {
				case MC_1_8:
					(*extra)["width"] = std::make_shared<ShortTag>(geometry.width);
					(*extra)["height"] = std::make_shared<ShortTag>(geometry.height);
				case MC_1_12:
					(*extra)["trackingPosition"] = std::make_shared<ByteTag>(0);
					(*extra)["unlimitedTracking"] = std::make_shared<ByteTag>(0);
					(*extra)["locked"] = std::make_shared<ByteTag>(1);
					(*extra)["banners"] = std::make_shared<ListTag>(NBTP::COMPOUND);
					(*extra)["frames"] = std::make_shared<ListTag>(NBTP::COMPOUND);
				case MC_1_16:
					break;
				case INVALID:
				default:
					throw std::runtime_error(INVALID_GAME_VER);
			}
			return extra;
		}

		/**
		 * Constructs the data tag that holds the majority of map data
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map data tag
		 */
		std::shared_ptr<CompoundTag> makeMapData(VersionSpec ver, const struct MapGeometry &geometry) {
			std::shared_ptr<CompoundTag> data = std::make_shared<CompoundTag>();

			auto extra = extraDataForVersion(ver, geometry);

			data->insert("scale", std::make_shared<ShortTag>(geometry.scale));
			data->insert("dimension", std::make_shared<ByteTag>(geometry.dim));
			data->insert("xCenter", std::make_shared<IntTag>(geometry.xC));
			data->insert("zCenter", std::make_shared<IntTag>(geometry.zC));
			data->insert("colors", std::make_shared<BytesTag>());

			for (const auto &tag : *extra) {
				data->insert(tag.first, tag.second);
			}

			return data;
		}

		/**
		 * Constructs a map tag according for specified version of minecraft
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver, const struct MapGeometry &geometry) {
			auto map_tag = std::make_shared<NBTP::CompoundTag>();
			auto data_tag = Map::makeMapData(ver, geometry);
			map_tag->insert("data", data_tag);
			insertDataVersion(*map_tag, ver);
			return map_tag;
		}

		/**
		 * Constructs a map tag according for specified version of minecraft with default geometry
		 * @param ver          Minecraft version specification
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag> makeMapRoot(VersionSpec ver) {
			return makeMapRoot(ver, DEFAULT_GEOMETRY);
		}
	}
}

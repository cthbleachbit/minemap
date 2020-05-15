//
// Created by cth451 on 2020/05/14.
//

#include "Map.h"

namespace Minemap {
	namespace Map {

		static struct MapGeometry DEFAULT_GEOMETRY = {1, 1, 128, 128, DEFAULT_X_CENTER, DEFAULT_Z_CENTER};

		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(VersionSpec ver, const struct MapGeometry &geometry) {
			std::shared_ptr<NBTP::CompoundTag> data = std::make_shared<NBTP::CompoundTag>();

			auto extra = extraDataForVersion(ver);

			data->insert("scale", std::make_shared<NBTP::ShortTag>(geometry.scale));
			data->insert("dimension", std::make_shared<NBTP::ByteTag>(geometry.dim));
			data->insert("width", std::make_shared<NBTP::ShortTag>(geometry.width));
			data->insert("height", std::make_shared<NBTP::ShortTag>(geometry.height));
			data->insert("xCenter", std::make_shared<NBTP::IntTag>(geometry.xC));
			data->insert("zCenter", std::make_shared<NBTP::IntTag>(geometry.zC));
			data->insert("colors", std::make_shared<NBTP::BytesTag>());

			for (const auto &tag : *extra) {
				data->insert(tag.first, tag.second);
			}

			return data;
		}

		std::shared_ptr<NBTP::CompoundTag> makeMapData(VersionSpec ver) {
			return makeMapData(ver, DEFAULT_GEOMETRY);
		}

		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver, const struct MapGeometry &geometry) {
			auto map_tag = std::make_shared<NBTP::CompoundTag>();
			auto data_tag = Map::makeMapData(ver, geometry);
			map_tag->insert("data", data_tag);
			insertDataVersion(*map_tag, ver);
			return map_tag;
		}

		std::shared_ptr<NBTP::CompoundTag> makeMapRoot(VersionSpec ver) {
			return makeMapRoot(ver, DEFAULT_GEOMETRY);
		}
	}
}

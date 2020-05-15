//
// Created by cth451 on 2020/05/14.
//

#include "Map.h"
namespace Minemap {
	namespace Map {
		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(char scale, char dim, int16_t width, int16_t height, int32_t xC, int32_t zC) {
			std::shared_ptr<NBTP::CompoundTag> data = std::make_shared<NBTP::CompoundTag>();
			data->insert("scale", std::make_shared<NBTP::ShortTag>(scale));
			data->insert("dimension", std::make_shared<NBTP::ByteTag>(dim));
			data->insert("width", std::make_shared<NBTP::ShortTag>(width));
			data->insert("height", std::make_shared<NBTP::ShortTag>(height));
			data->insert("xCenter", std::make_shared<NBTP::IntTag>(xC));
			data->insert("zCenter", std::make_shared<NBTP::IntTag>(zC));
			data->insert("colors", std::make_shared<NBTP::BytesTag>());
			return data;
		}

		std::shared_ptr<NBTP::CompoundTag> makeMapData() {
			return makeMapData(1, 1, 128, 128, DEFAULT_X_CENTER, DEFAULT_Z_CENTER);
		}

	}
}

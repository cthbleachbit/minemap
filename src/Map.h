//
// Created by cth451 on 2020/05/14.
//

#ifndef MAP_H
#define MAP_H

#include "libnbtp.h"
#include "VersionSpec.h"

#define DEFAULT_X_CENTER 1 << 30
#define DEFAULT_Z_CENTER 1 << 30

namespace Minemap {
	namespace Map {
		struct MapGeometry {
			char scale;
			char dim;
			int16_t width;
			int16_t height;
			int32_t xC;
			int32_t zC;
		};

		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(VersionSpec ver, const struct MapGeometry &geometry);

		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(VersionSpec ver);

		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver, const struct MapGeometry &geometry);

		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver);
	}
}


#endif //MAP_H

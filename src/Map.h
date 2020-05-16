//
// Created by cth451 on 2020/05/14.
//

#ifndef MAP_H
#define MAP_H

#include "libnbtp.h"
#include "VersionSpec.h"

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

		/**
		 * Constructs the data tag that holds the majority of map data
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map data tag
		 */
		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(VersionSpec ver, const struct MapGeometry &geometry);

		/**
		 * Constructs a root tag according for specified version of minecraft
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver, const struct MapGeometry &geometry);

		/**
		 * Constructs a root tag according for specified version of minecraft with default geometry
		 * @param ver          Minecraft version specification
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag>
		makeMapRoot(VersionSpec ver);
	}
}


#endif //MAP_H

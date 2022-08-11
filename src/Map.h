//
// Created by cth451 on 2020/05/14.
//

#ifndef MINEMAP_MAP_H
#define MINEMAP_MAP_H

#include "libnbtp.h"
#include "VersionSpec.h"
#include <optional>

// the default center coordinates should be close enough to the "borderlands" that players would not normally reach
#define DEFAULT_X_CENTER 1 << 30
#define DEFAULT_Z_CENTER 1 << 30

namespace Minemap {
	namespace Map {
		struct MapGeometry {
			char scale = 1;
			char dim = 1;
			int16_t width = 128;
			int16_t height = 128;
			int32_t xC = DEFAULT_X_CENTER;
			int32_t zC = DEFAULT_Z_CENTER;

			constexpr MapGeometry() = default;

			MapGeometry(char _scale, char _dim, int16_t _width, int16_t _height, int32_t _xC, int16_t _zC) noexcept
					: scale(_scale), dim(_dim), height(_height), xC(_xC), zC(_zC) {};

			MapGeometry(int16_t _width, int16_t _height) noexcept
					: width(_width), height(_height) {};

			bool operator==(const MapGeometry &) const = default;
		};

		/**
		 * Constructs the data tag that holds the majority of map data
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map data tag
		 */
		std::shared_ptr<NBTP::CompoundTag> makeMapData(Version ver, const struct MapGeometry &geometry);

		/**
		 * Constructs a root tag according for specified version of minecraft
		 * @param ver          Minecraft version specification
		 * @param geometry     Map geometry
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag> makeMapRoot(Version ver, const struct MapGeometry &geometry);

		/**
		 * Obtain modifiable pointer to the colors array contained in a map
		 * @param root_ptr          pointer to a populated map tag
		 * @throw runtime_error     if the map is malformed or doesn't look like a map item
		 */
		NBTP::BytesTag *getModifiableColors(std::shared_ptr<NBTP::Tag> root_ptr);

		/**
		 * Obtain modifiable pointer to the banners array contained in a map (1.16+ only)
		 * @param root_ptr          pointer to a populated map tag
		 * @throw runtime_error     if the tag doesn't look like a map item or doesn't have banners tag
		 */
		NBTP::ListTag *getModifiableBanners(std::shared_ptr<NBTP::Tag> root_ptr);

		/**
		 * Constructs a root tag according for specified version of minecraft with default geometry
		 * @param ver          Minecraft version specification
		 * @return             Map root tag
		 */
		std::shared_ptr<NBTP::CompoundTag> makeMapRoot(Version ver);

		void saveMap(std::ostream &os, NBTP::CompoundTag &tag);
	}
}


#endif //MAP_H

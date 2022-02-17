//
// Created by cth451 on 2022/2/17.
//

#ifndef MINEMAP_MARKER_H
#define MINEMAP_MARKER_H

#include <tags/CompoundTag.h>
#include <tags/IntTag.h>
#include <cinttypes>
#include <optional>
#include "libminemap.h"

namespace Minemap {

#define BANNERCOLOR(c) \
    c(white)             \
    c(orange)             \
    c(magenta)         \
    c(light_blue) \
    c(yellow)     \
    c(lime)       \
    c(pink)       \
    c(gray)       \
    c(light_gray) \
    c(cyan)       \
    c(purple)     \
    c(blue)       \
    c(brown)      \
    c(green)      \
    c(red)        \
    c(black)

	constexpr const char* bannerColors[] {
			BANNERCOLOR(CREATE_STRINGS)
	};
	static_assert(sizeof(bannerColors) / sizeof(bannerColors[0]) == 16);

	/**
	 * Checks if input is a valid banner color identifier
	 * @param color  input color string
	 * @return       true if color is valid
	 */
	inline bool validateBannerColor(const std::string &color) {
		return std::any_of(bannerColors, bannerColors + 16, [color](const char* i){return color == i;});
	}

	struct MarkerPosition {
		NBTP::IntTag::V x = 0;
		NBTP::IntTag::V y = 64;
		NBTP::IntTag::V z = 0;
		NBTP::IntTag::V xOffset = 0;
		NBTP::IntTag::V zOffset = 0;
	};

	struct Banner {
		MarkerPosition position;
		std::string color;
		std::optional<std::string> name = std::nullopt;
	};

	struct Frame {
		NBTP::IntTag::V entityId = 0;
		NBTP::IntTag::V rotation = 0;
		MarkerPosition position;
	};
}

#endif //MINEMAP_MARKER_H

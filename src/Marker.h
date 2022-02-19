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

	constexpr const char *bannerColors[]{
			BANNERCOLOR(CREATE_STRINGS)
	};
	static_assert(sizeof(bannerColors) / sizeof(bannerColors[0]) == 16);

	/**
	 * Checks if input is a valid banner color identifier
	 * @param color  input color string
	 * @return       true if color is valid
	 */
	inline bool validateBannerColor(const std::string &color) {
		return std::any_of(bannerColors, bannerColors + 16, [color](const char *i) { return color == i; });
	}

	struct MarkerPosition {
		// Marker position relative to (xOffset, 0, zOffset)
		NBTP::IntTag::V x = 0;
		NBTP::IntTag::V y = 64;
		NBTP::IntTag::V z = 0;
		// Map center coordinates - set to 0 to indicate absolute position
		NBTP::IntTag::V xOffset = 0;
		NBTP::IntTag::V zOffset = 0;

		MarkerPosition(NBTP::IntTag::V x, NBTP::IntTag::V y, NBTP::IntTag::V z, NBTP::IntTag::V xOffset,
		               NBTP::IntTag::V zOffset) noexcept: x(x), y(y), z(z), xOffset(xOffset), zOffset(zOffset) {};

		/**
		 * Create a marker position object based
		 * @param spec       "rel:<x>:<y>:<z>" or "abs:<x>:<y>:<z>"
		 * @param xOffset    xOffset from map
		 * @param zOffset    yOffset from map
		 */
		MarkerPosition(const std::string &spec, NBTP::IntTag::V xOffset = 0, NBTP::IntTag::V zOffset = 0);

		/**
		 * Compare two position (taking offset into account)
		 * @param pos
		 * @return
		 */
		std::partial_ordering operator<=>(const MarkerPosition &pos) const noexcept;

		/**
		 * Equivalence short hand
		 * @param pos
		 * @return
		 */
		bool operator==(const MarkerPosition &pos) const noexcept {
			return std::is_eq(*this <=> pos);
		}

		/**
		 * Inequivalence short hand
		 * @param pos
		 * @return
		 */
		bool operator!=(const MarkerPosition &pos) const noexcept = default;

		/**
		 * Generate a compound tag from this position
		 * Note that this merges offset and create a tag with absolute offset
		 * @return
		 */
		std::shared_ptr<NBTP::CompoundTag> toCompound() const noexcept;
	};

	struct Banner {
		MarkerPosition position{0, 0, 0, 0, 0};
		std::string color;
		std::optional<std::string> name = std::nullopt;

		Banner(const MarkerPosition position,
		       const std::string &color,
		       std::optional<std::string> name = std::nullopt)
		noexcept: position(position), color(color), name(std::move(name)) {};

		/**
		 * Generate a compound tag from this banner
		 * Note that this merges offset and create a tag with absolute offset
		 * @return
		 */
		std::shared_ptr<NBTP::CompoundTag> toCompound() const noexcept;

		/**
		 * Create a banner from "<position>
		 * @param spec       "rel:<x>:<y>:<z>" or "abs:<x>:<y>:<z>"
		 * @param xOffset    xOffset from map
		 * @param zOffset    yOffset from map
		 */
		Banner(const std::string &spec, NBTP::IntTag::V xOffset = 0, NBTP::IntTag::V zOffset = 0);
	};

	struct Frame {
		NBTP::IntTag::V entityId = 0;
		NBTP::IntTag::V rotation = 0;
		MarkerPosition position;
	};
}

template<>
struct fmt::formatter<Minemap::MarkerPosition> {
	// presentation mode - a = absolute, r = relative
	char presentation = 'a';

	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'a' || *it == 'r')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template<typename FormatContext>
	auto format(const Minemap::MarkerPosition &p, FormatContext &ctx) -> decltype(ctx.out()) {
		return presentation == 'a' ?
		       format_to(ctx.out(), "abs:{}:{}:{}", p.x + p.xOffset, p.y, p.z + p.zOffset) :
		       format_to(ctx.out(), "rel:{}:{}:{}", p.x, p.y, p.z);
	}
};

template<>
struct fmt::formatter<Minemap::Banner> {
	// presentation mode - a = absolute, r = relative
	char presentation = 'a';

	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'a' || *it == 'r')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template<typename FormatContext>
	auto format(const Minemap::Banner &b, FormatContext &ctx) -> decltype(ctx.out()) {
		std::string n = b.name.has_value() ? b.name.value() : "";
		return presentation == 'a' ?
		       format_to(ctx.out(), "{:a} {}:{}", b.position, b.color, n) :
		       format_to(ctx.out(), "{:r} {}:{}", b.position, b.color, n);
	}
};

#endif //MINEMAP_MARKER_H

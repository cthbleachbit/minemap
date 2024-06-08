/**
 * @file libminemap entry point
 */

#ifndef MINEMAP_LIBMINEMAP_H
#define MINEMAP_LIBMINEMAP_H

#include <optional>
#include <tags/ListTag.h>

#include <Magick++.h>

#include "Magick++/Include.h"
#include "VersionSpec.h"

#if __has_attribute(target_clones) && !defined(_WIN32)
# if (defined(__amd64__) || defined(__x86_64__))
#  define MINEMAP_CLONE __attribute((target_clones("default", "sse2", "sse3", "sse4.1", "sse4.2", "popcnt", "avx", "avx2")))
# elif (defined(__PPC64__))
#  define MINEMAP_CLONE __attribute__((target_clones("default","cpu=power9")))
# else
#  pragma message( "Warning: Target clone not supported yet" )
#  define MINEMAP_CLONE
# endif
#else
#pragma message( "Warning: No target clone support" )
#define MINEMAP_CLONE
#endif

#define CREATE_ENUM(name) \
    F_##name,

#define CREATE_STRINGS(name) \
    #name,

/**
 * Forward declarations
 */
namespace Magick {
	class Image;
}

namespace Minemap {
	class ColorMap;
}

/**
 * Main namespace
 */
namespace Minemap {

	/**
	 * Print version for the program - the version string is specified at compile time.
	 * @param program_base program base name
	 */
	void print_version(const std::string& program_base = "minemap") noexcept;

	/**
	 * Insert mapped pixels from palette to colors_tag
	 * @param input_img
	 *        original input image scaled down to 128x128 - only used for checking transparency
	 * @param mapped_img
	 *        mapped image that only contains colors from the palette
	 * @param palette_lookup_table
	 *        LUT constructed from the palette image
	 * @param colors_tag
	 *        NBTP::BytesTag to store the mapped pixels
	 * @return
	 */
	void mapped_to_tag(const Magick::Image &input_img,
	                   const Magick::Image &mapped_img,
	                   const std::shared_ptr<ColorMap> &palette_lookup_table,
	                   NBTP::BytesTag *colors_tag);

	/**
	 * Read list of bytes from a map object and return an array of RGBA
	 * @param colors_tag
	 * 	      ListTag payload containing colors
	 * @param palette_lookup_table
	 *        LUT constructed from the palette image
	 * @return  array of RGBA for all 128x128 pixels
	 */
	std::unique_ptr<std::array<double, 16384 * 4>>
	tag_to_pixelstore(const NBTP::ListTag::List &colors_tag,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table);

	/**
	 * Load a version-specified palette into a Magick Image for use as dithering color candidates
	 *
	 * Note that the first row (i.e. first 4 colors) are stripped off - color codes 0-3 are transparent in-game.
	 * An opaque pixel should never map to color code 0-3.
	 * @param version
	 * @return
	 */
	const inline Magick::Image verspec_to_image(const VersionSpec version) {
		auto data = version.paletteData;
		// Skip 1 row, i.e. 16 bytes for 4x RGBA
		Magick::Image image(4, version.paletteHeight - 1, "RGBA", Magick::StorageType::CharPixel, data + 16);
		return image;
	}
}

#endif //MINEMAP_LIBMINEMAP_H

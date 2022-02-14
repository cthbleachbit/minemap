//
// Created by cth451 on 2021/9/15.
//

#ifndef MINEMAP_LIBMINEMAP_H
#define MINEMAP_LIBMINEMAP_H

#include <optional>
#include <tags/BytesTag.h>
#include <tags/ListTag.h>

#include <Magick++.h>

#include "Magick++/Include.h"
#include "VersionSpec.h"

#if __has_attribute(target_clones) && !defined(_WIN32)
# if (defined(__amd64__) || defined(__x86_64__))
# define __MINEMAP_CLONE__ __attribute((target_clones("default", "sse2", "sse3", "sse4.1", "sse4.2", "popcnt", "avx", "avx2")))
# elif (defined(__aarch64__))
# define __MINEMAP_CLONE__ __attribute((target_clones("default", "simd", "sve2")))
# elif (defined(__PPC64__))
# define __MINEMAP_CLONE__ __attribute__((target_clones("default","cpu=power9")))
# endif
#else
#define __MINEMAP_CLONE__
#endif

namespace Magick {
	class Image;
}

namespace Minemap {
	class ColorMap;

	void print_version() noexcept;

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

	const inline Magick::Image verspec_to_image(const VersionSpec version) {
		auto data = version.paletteData;
		Magick::Image image(4, version.paletteHeight, "RGBA", Magick::StorageType::CharPixel, data);
		return image;
	}
}

#endif //MINEMAP_LIBMINEMAP_H

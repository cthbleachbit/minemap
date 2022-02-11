//
// Created by cth451 on 2021/9/15.
//

#ifndef MINEMAP_LIBMINEMAP_H
#define MINEMAP_LIBMINEMAP_H

#include <optional>

#include "Map.h"
#include "ColorMap.h"

namespace Minemap {
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
	int mapped_to_tag(const Magick::Image &input_img, const Magick::Image &mapped_img,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table,
	                  NBTP::BytesTag *colors_tag);
}

#endif //MINEMAP_LIBMINEMAP_H

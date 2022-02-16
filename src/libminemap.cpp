//
// Created by cth451 on 2021/9/15.
//

#include <fmt/format.h>
#include <iostream>
#include <memory>
#include "libminemap.h"
#include "Map.h"
#include "ColorMap.h"

namespace Minemap {
	void print_version() noexcept {
		std::cout << VERSION_MESSAGE << std::endl;
	}

	__MINEMAP_CLONE__
	void _clone_mapped_to_tag(const Magick::Image &input_img,
	                          const Magick::Image &mapped_img,
	                          const std::shared_ptr<ColorMap> &palette_lookup_table,
	                          NBTP::BytesTag *colors_tag) {
#ifdef USE_GM
		bool hasAlpha = input_img.matte();
#else
		bool hasAlpha = input_img.alpha();
#endif
		size_t width = input_img.columns();
		size_t height = input_img.rows();
		for (size_t i = 0; i < width * height; i++) {
			ssize_t col = i % width;
			ssize_t row = i / height;
			Magick::ColorRGB output_pix = mapped_img.pixelColor(col, row);
#ifdef USE_GM
			// WTF? Alpha in GM is exactly opposite - alpha = 65535 under 16b <=> transparent
			bool is_transparent = hasAlpha && (input_img.pixelColor(col, row).alphaQuantum() == MaxRGB);
#else
			// alpha = 0 <=> transparent
			bool is_transparent = hasAlpha && (input_img.pixelColor(col, row).quantumAlpha() == 0.0f);
#endif
			std::optional<MapColorCode> colorCode;
			if (is_transparent) {
				colorCode = 0;
			} else {
				colorCode = palette_lookup_table->lookup(TupleRGB(output_pix));
				if (!colorCode.has_value()) {
					std::string no_match = fmt::format(COLOR_MISMATCH, col, row);
					throw std::runtime_error(no_match);
				}
			}
			colors_tag->insert(std::make_shared<NBTP::ByteTag>(colorCode.value()));
		}
	}

	void mapped_to_tag(const Magick::Image &input_img,
	                   const Magick::Image &mapped_img,
	                   const std::shared_ptr<ColorMap> &palette_lookup_table,
	                   NBTP::BytesTag *colors_tag) {
		_clone_mapped_to_tag(input_img, mapped_img, palette_lookup_table, colors_tag);
	}

	__MINEMAP_CLONE__
	std::unique_ptr<std::array<double, 16384 * 4>>
	_clone_tag_to_pixelstore(const NBTP::ListTag::List &colors_list,
	                         const std::shared_ptr<ColorMap> &palette_lookup_table) {
		auto pixel_store = std::make_unique<std::array<double, 16384 * 4>>();
		auto pixel_array = pixel_store->data();
		for (int i = 0; i < 16384; i++) {
			MapColorCode colorIndex = ((NBTP::ByteTag *) colors_list[i].get())->getPayload();
			auto rgb = palette_lookup_table->lookup(colorIndex);

			if (!rgb.has_value()) {
				std::string error_string = fmt::format(COLOR_OUT_OF_RANGE, (uint16_t) colorIndex, i);
				throw std::runtime_error(error_string);
			}

			pixel_array[i * 4] = rgb->r;
			pixel_array[i * 4 + 1] = rgb->g;
			pixel_array[i * 4 + 2] = rgb->b;
#ifdef USE_GM
			pixel_array[i * 4 + 3] = colorIndex < 4 ? 0 : MaxRGBDouble;
#else
			pixel_array[i * 4 + 3] = colorIndex < 4 ? 0 : 1.0;
#endif
		}
		return pixel_store;
	}

	std::unique_ptr<std::array<double, 16384 * 4>>
	tag_to_pixelstore(const NBTP::ListTag::List &colors_tag,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table) {
		return _clone_tag_to_pixelstore(colors_tag, palette_lookup_table);
	}
}
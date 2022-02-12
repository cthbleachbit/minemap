//
// Created by cth451 on 2021/9/15.
//

#include <fmt/format.h>
#include <iostream>
#include <Magick++.h>
#include <memory>
#include "libminemap.h"
#include "Map.h"
#include "ColorMap.h"

namespace Minemap {
	void print_version() noexcept {
		std::string version_string = fmt::format("Application version {}\n", MINEMAP_APP_VER);
		std::cout << version_string << std::endl;
	}

	__MINEMAP_CLONE__
	void _clone_mapped_to_tag(const Magick::Image &input_img, const Magick::Image &mapped_img,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table,
	                  NBTP::BytesTag *colors_tag) {
		const Magick::Quantum *quantum = input_img.getConstPixels(0, 0, input_img.columns(), input_img.rows());
		size_t width = input_img.columns();
		size_t height = input_img.rows();
		for (size_t i = 0; i < width * height; i++) {
			ssize_t col = i % width;
			ssize_t row = i / height;
			Magick::ColorRGB output_pix = mapped_img.pixelColor(col, row);
			bool is_transparent = (quantum[4 * i + 3] == 0.0f);
			std::optional<MapColorCode> colorCode;
			if (is_transparent) {
				colorCode = 0;
			} else {
				colorCode = palette_lookup_table->lookup(TupleRGB(output_pix));
				if (!colorCode.has_value()) {
					std::string no_match = fmt::format("Error: No color match for pixel at ({}, {})", col, row);
					throw std::runtime_error(no_match);
				}
			}
			colors_tag->insert(std::make_shared<NBTP::ByteTag>(colorCode.value()));
		}
	}

	void mapped_to_tag(const Magick::Image &input_img, const Magick::Image &mapped_img,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table,
	                  NBTP::BytesTag *colors_tag) {
		_clone_mapped_to_tag(input_img, mapped_img, palette_lookup_table, colors_tag);
	}
}